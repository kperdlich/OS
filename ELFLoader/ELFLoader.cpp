//
// Created by n3dry on 04.10.24.
//

#include "ELFLoader.h"
#include <cstdio>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace ELF {

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
}

ELFLoader::~ELFLoader()
{
    clear();
}

void ELFLoader::clear()
{
    if (m_mappedElfFile) {
        munmap(m_mappedElfFile, m_mappedFileSize);
        m_mappedElfFile = nullptr;
        m_mappedFileSize = {};
    }
}

bool ELFLoader::load()
{
    if (m_elfFilePath.isEmpty())
        return false;

    clear();

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

    loadSymbols();
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

    return m_executableTextSection + symbol->st_value;
}

const char* ELFLoader::getStringFromSectionStringTable(const Elf64_Shdr& section, ADS::size_t stringTableIndex) const
{
    const Elf64_Shdr* const stringTableSection = &sectionHeader()[section.sh_link];
    const char* const stringTable = m_mappedElfFile + stringTableSection->sh_offset;
    return stringTable + stringTableIndex;
}

void ELFLoader::loadSymbols()
{
    for (ADS::size_t i = 0; i < header()->e_shnum; ++i) {
        const Elf64_Shdr& sh = sectionHeader()[i];
        if (sh.sh_type == SHT_SYMTAB) {
            m_symbolTableSectionHeader = &sh;
        } else {
            const ADS::String symbolName = sectionHeaderStringTable() + sh.sh_name;
            if (symbolName == ".text") {
                m_executableTextSection = static_cast<char*>(mmap(nullptr, sh.sh_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
                ASSERT(m_executableTextSection != MAP_FAILED);
                memcpy(m_executableTextSection, m_mappedElfFile + sh.sh_offset, sh.sh_size);
                const int ret = mprotect(m_executableTextSection, sh.sh_size, PROT_READ | PROT_EXEC);
                ASSERT(ret == 0);
            }
        }
        if (m_symbolTableSectionHeader && m_executableTextSection)
            break;
    }

    ASSERT(m_symbolTableSectionHeader != nullptr);
    ASSERT(m_executableTextSection != nullptr);

    forEachSymbolIndexed([this](ADS::size_t, const Elf64_Sym& symbol) {
        if (ELF64_ST_TYPE(symbol.st_info) == STT_FUNC) {
            const char* const symbolStr = getStringFromSectionStringTable(*m_symbolTableSectionHeader, symbol.st_name);
            m_funcSymbols.set(symbolStr, &symbol);
        }
    });
}

}