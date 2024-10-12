//
// Created by n3dry on 04.10.24.
//

#include "ELFLoader.h"
#include <cstdio>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace ELF {

static uint64_t s_pageSize {};

static uint64_t pageAlign(uint64_t n)
{
    return (n + (s_pageSize - 1)) & ~(s_pageSize - 1);
}

static const char* symbolTypeFor(uint8_t type)
{
    switch (type) {
    case STT_NOTYPE:
        return "NOTYPE";
    case STT_OBJECT:
        return "OBJECT";
    case STT_FUNC:
        return "FUNC";
    case STT_SECTION:
        return "SECTION";
    case STT_FILE:
        return "FILE";
    case STT_COMMON:
        return "COMMON";
    case STT_TLS:
        return "TLS";
    default:
        ASSERT(false);
        return "Unknown";
    }
}

ELFLoader::ELFLoader(ADS::String elfFilePath)
    : m_elfFilePath(ADS::move(elfFilePath))
{
    m_externalSymbols.set("puts", reinterpret_cast<char*>(puts));
}

ELFLoader::~ELFLoader()
{
    clear();
}

void ELFLoader::clear()
{
    m_funcSymbols.clear();
    if (m_mappedElfFile) {
        munmap(m_mappedElfFile, m_mappedFileSize);
        m_mappedElfFile = nullptr;
        m_mappedFileSize = {};
    }

    if (m_runtimeMemory) {
        munmap(m_runtimeMemory, m_runtimeMemorySize);
        m_runtimeMemory = nullptr;
        m_runtimeMemorySize = {};
        m_runtimeMemoryRodataSection = nullptr;
        m_runtimeRodataSectionSize = {};
        m_runtimeMemoryTextSection = nullptr;
        m_runtimeTextSectionSize = {};
        m_runtimeMemoryDataSection = nullptr;
        m_runtimeDataSectionSize = {};
    }
}

bool ELFLoader::load()
{
    if (m_elfFilePath.isEmpty())
        return false;

    clear();

    if (s_pageSize == 0)
        s_pageSize = sysconf(_SC_PAGESIZE);

    const int fd = open(m_elfFilePath.cStr(), O_RDONLY);
    ASSERT(fd != -1);

    struct stat sb {};
    fstat(fd, &sb);
    m_mappedFileSize = sb.st_size;

    m_mappedElfFile = static_cast<char*>(mmap(nullptr, m_mappedFileSize, PROT_READ, MAP_PRIVATE, fd, 0));
    ASSERT(m_mappedElfFile != MAP_FAILED);
    close(fd);

    if (!isValidElfFile()) {
        clear();
        return false;
    }

    ParseAndLoad();
    return true;
}

void ELFLoader::dump() const
{
    const Elf64_Ehdr* const elfHeader = header();

    ASSERT(elfHeader->e_machine == EM_X86_64);

    printf("[ELFLoader]\n");
    printf("    type: %u\n", elfHeader->e_type);
    printf("    machine: %u\n", elfHeader->e_machine);
    printf("    version: %u\n", elfHeader->e_version);
    printf("    entry point address: %p\n", reinterpret_cast<void*>(elfHeader->e_entry));
    printf("    program header offset: %lu\n", elfHeader->e_phoff);
    printf("    section header offset: %lu\n", elfHeader->e_shoff);

    printf("    section headers: %u\n", elfHeader->e_shnum);
    for (ADS::size_t i = 0; i < elfHeader->e_shnum; ++i) {
        const Elf64_Shdr& shdr = sectionHeader()[i];
        printf("        section header #%zu '%s':\n", i, sectionHeaderStringTable() + shdr.sh_name);
        printf("        name: %u\n", shdr.sh_name);
        printf("        type: %u\n", shdr.sh_type);
        printf("        offset: %lu\n", shdr.sh_offset);
        printf("        size: %lu\n", shdr.sh_size);
        puts("");
    }

    printf("    program headers: %u\n", elfHeader->e_phnum);
    for (ADS::size_t i = 0; i < elfHeader->e_phnum; ++i) {
        const Elf64_Phdr& phdr = programHeader()[i];
        printf("        program header #%zu:\n", i);
        printf("        type: %u\n", phdr.p_type);
        printf("        offset: %lu\n", phdr.p_offset);
        printf("        virtual address: %p\n", reinterpret_cast<void*>(phdr.p_vaddr));
        printf("        physical address: %p\n", reinterpret_cast<void*>(phdr.p_paddr));
        printf("        flags: %u\n", phdr.p_flags);
        puts("");
    }

    printf("Symbol table '%s':\n", sectionHeaderStringTable() + m_symbolTableSectionHeader->sh_name);
    printf("%-5s %-8s %-8s %-20s\n", "Num", "Value", "Type", "Name");
    forEachSymbolIndexed([this](ADS::size_t index, const Elf64_Sym& symbol) {
        const char* const symbolStr = getStringFromSectionStringTable(*m_symbolTableSectionHeader, symbol.st_name);
        printf("%-5zu %-8lx %-8s %-20s\n", index, symbol.st_value, symbolTypeFor(ELF64_ST_TYPE(symbol.st_info)), symbolStr);
    });
    puts("");
}

const Elf64_Ehdr* ELFLoader::header() const
{
    ASSERT(m_mappedElfFile != nullptr);
    return reinterpret_cast<Elf64_Ehdr*>(m_mappedElfFile);
}

bool ELFLoader::isValidElfFile() const
{
    return memcmp(header()->e_ident, ELFMAG, SELFMAG) == 0;
}

const Elf64_Shdr* ELFLoader::sectionHeader() const
{
    return reinterpret_cast<Elf64_Shdr*>(m_mappedElfFile + header()->e_shoff);
}

const char* ELFLoader::sectionHeaderStringTable() const
{
    const Elf64_Shdr& sectionHeaderStringTable = sectionHeader()[header()->e_shstrndx];
    return m_mappedElfFile + sectionHeaderStringTable.sh_offset;
}

const Elf64_Phdr* ELFLoader::programHeader() const
{
    return reinterpret_cast<Elf64_Phdr*>(m_mappedElfFile + header()->e_phoff);
}

void* ELFLoader::findFunc(const ADS::String& name)
{
    if (name.isEmpty())
        return nullptr;

    const Elf64_Sym* symbol = nullptr;
    if (!m_funcSymbols.tryGetValue(name, symbol))
        return nullptr;

    return m_runtimeMemoryTextSection + symbol->st_value;
}

const char* ELFLoader::getStringFromSectionStringTable(const Elf64_Shdr& section, ADS::size_t stringTableIndex) const
{
    const Elf64_Shdr* const stringTableSection = &sectionHeader()[section.sh_link];
    const char* const stringTable = m_mappedElfFile + stringTableSection->sh_offset;
    return stringTable + stringTableIndex;
}

const Elf64_Sym* ELFLoader::symbolTable() const
{
    return reinterpret_cast<const Elf64_Sym*>(m_mappedElfFile + m_symbolTableSectionHeader->sh_offset);
}

const Elf64_Sym* ELFLoader::symbolByIndex(uint32_t index) const
{
    return &symbolTable()[index];
}

void ELFLoader::ParseAndLoad()
{
    const Elf64_Shdr* textSectionHeader = findSectionHeader([this](const Elf64_Shdr& sh) {
        const ADS::String sectionName = sectionHeaderStringTable() + sh.sh_name;
        if (sectionName == ".text") {
            return true;
        }
        return false;
    });

    const Elf64_Shdr* dataSectionHeader = findSectionHeader([this](const Elf64_Shdr& sh) {
        const ADS::String sectionName = sectionHeaderStringTable() + sh.sh_name;
        if (sectionName == ".data") {
            return true;
        }
        return false;
    });

    const Elf64_Shdr* rodataSectionHeader = findSectionHeader([this](const Elf64_Shdr& sh) {
        const ADS::String sectionName = sectionHeaderStringTable() + sh.sh_name;
        if (sectionName == ".rodata") {
            return true;
        }
        return false;
    });

    m_symbolTableSectionHeader = findSectionHeader([](const Elf64_Shdr& sh) {
        return sh.sh_type == SHT_SYMTAB;
    });

    ASSERT(m_symbolTableSectionHeader != nullptr);
    ASSERT(textSectionHeader != nullptr);

    m_runtimeMemorySize = pageAlign(textSectionHeader->sh_size);
    if (dataSectionHeader)
        m_runtimeMemorySize += pageAlign(dataSectionHeader->sh_size);
    if (rodataSectionHeader)
        m_runtimeMemorySize += pageAlign(rodataSectionHeader->sh_size);

    m_runtimeMemory = static_cast<char*>(mmap(nullptr, m_runtimeMemorySize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    ASSERT(m_runtimeMemory != MAP_FAILED);

    m_runtimeMemoryTextSection = m_runtimeMemory;
    m_runtimeTextSectionSize = m_runtimeMemorySize;

    if (dataSectionHeader) {
        m_runtimeDataSectionSize = pageAlign(dataSectionHeader->sh_size);
        m_runtimeMemoryDataSection = m_runtimeMemory + m_runtimeDataSectionSize;
    }

    if (rodataSectionHeader) {
        m_runtimeRodataSectionSize = pageAlign(rodataSectionHeader->sh_size);
        m_runtimeMemoryRodataSection = m_runtimeMemory + m_runtimeRodataSectionSize;
    }

    // copy sections from ELF
    memcpy(m_runtimeMemoryTextSection, m_mappedElfFile + textSectionHeader->sh_offset, textSectionHeader->sh_size);
    if (dataSectionHeader)
        memcpy(m_runtimeMemoryDataSection, m_mappedElfFile + dataSectionHeader->sh_offset, dataSectionHeader->sh_size);
    if (rodataSectionHeader)
        memcpy(m_runtimeMemoryRodataSection, m_mappedElfFile + rodataSectionHeader->sh_offset, rodataSectionHeader->sh_size);

    // Load and cache func symbols
    forEachSymbolIndexed([this](ADS::size_t, const Elf64_Sym& symbol) {
        if (ELF64_ST_TYPE(symbol.st_info) == STT_FUNC) {
            const char* const symbolStr = getStringFromSectionStringTable(*m_symbolTableSectionHeader, symbol.st_name);
            m_funcSymbols.set(symbolStr, &symbol);
        }
    });

    applyRelocations();

    // make the `.text` copy readonly and executable
    const int protectTextSectionRet = mprotect(m_runtimeMemoryTextSection, m_runtimeTextSectionSize, PROT_READ | PROT_EXEC);
    ASSERT(protectTextSectionRet == 0);

    // make `.rodata` readonly
    if (m_runtimeMemoryRodataSection) {
        const int protectRoDataSectionRet = mprotect(m_runtimeMemoryRodataSection, m_runtimeRodataSectionSize, PROT_READ);
        ASSERT(protectRoDataSectionRet == 0);
    }
}

void ELFLoader::applyRelocations()
{
    // For now only .text section
    const Elf64_Shdr* relaTextSection = findSectionHeader([this](const Elf64_Shdr& sh) {
        ADS::String sectionName = sectionHeaderStringTable() + sh.sh_name;
        if (sectionName == ".rela.text") {
            return true;
        }
        return false;
    });

    if (relaTextSection)
        applyRelocation(*relaTextSection, m_runtimeMemory);
}

void ELFLoader::applyRelocation(const Elf64_Shdr& section, char* runtimeSectionMemory)
{
    const ADS::size_t numRelocations = section.sh_size / section.sh_entsize;
    const Elf64_Rela* const relocations = reinterpret_cast<Elf64_Rela*>(m_mappedElfFile + section.sh_offset);
    for (ADS::size_t i = 0; i < numRelocations; i++) {
        const uint32_t symbolIndex = ELF64_R_SYM(relocations[i].r_info);
        const uint32_t type = ELF64_R_TYPE(relocations[i].r_info);
        char* const addressToPatch = runtimeSectionMemory + relocations[i].r_offset;
        const Elf64_Sym* const symbol = symbolByIndex(symbolIndex);

        const char* symbolAddress = nullptr;
        const bool isExternalSymbol = symbol->st_shndx == SHN_UNDEF;
        if (isExternalSymbol) {
            const ADS::String symbolName = getStringFromSectionStringTable(*m_symbolTableSectionHeader, symbol->st_name);
            const char* const externalSymbol = m_externalSymbols.getValueOrDefault(symbolName);
            ASSERT(externalSymbol != nullptr);
            symbolAddress = externalSymbol;
            printf("Relocation: patch external symbol '%s'\n", symbolName.cStr());
        } else {
            const Elf64_Shdr& symbolSection = sectionHeader()[symbol->st_shndx];
            const ADS::String symbolName = getStringFromSectionStringTable(*m_symbolTableSectionHeader, symbol->st_name);
            const ADS::String sectionName = sectionHeaderStringTable() + symbolSection.sh_name;
            const char* const sectionRuntimeAddress = getRuntimeAddressFromSectionName(sectionName);
            ASSERT(sectionRuntimeAddress != nullptr);
            symbolAddress = sectionRuntimeAddress + symbol->st_value;
            printf("Relocation: patch internal symbol; section '%s', symbol '%s'\n", sectionName.cStr(), symbolName.cStr());
        }

        switch (type) {
        case R_X86_64_PC32:
            // S + A - P, 32 bit output, S == L here
        case R_X86_64_PLT32:
            // L + A - P, 32 bit output
            *reinterpret_cast<uint32_t*>(addressToPatch) = symbolAddress + relocations[i].r_addend - addressToPatch;
            break;
        default:
            ASSERT(false);
            break;
        }
    }
}

const char* ELFLoader::getRuntimeAddressFromSectionName(const ADS::String& sectionName) const
{
    // FIXME: Not ideal :/
    if (sectionName == ".text")
        return m_runtimeMemoryTextSection;

    if (sectionName == ".data")
        return m_runtimeMemoryDataSection;

    if (sectionName == ".rodata")
        return m_runtimeMemoryRodataSection;

    return nullptr;
}

}