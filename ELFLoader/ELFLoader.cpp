//
// Created by n3dry on 04.10.24.
//

#include "ELFLoader.h"
#include <cstdio>

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
    if (m_elfFile) {
        free(m_elfFile);
        m_elfFile = nullptr;
    }
}

bool ELFLoader::load()
{
    if (m_elfFilePath.isEmpty())
        return false;

    if (m_elfFile) {
        free(m_elfFile);
        m_elfFile = nullptr;
    }

    FILE* const file = fopen(m_elfFilePath.cStr(), "rb");
    if (!file)
        return false;

    fseek(file, 0, SEEK_END);
    const size_t fileSize = ftell(file);
    rewind(file);

    m_elfFile = malloc(fileSize);
    ASSERT(m_elfFile != nullptr);
    size_t readSize = fread(m_elfFile, 1, fileSize, file);
    ASSERT(readSize == fileSize);
    fclose(file);

    if (!isValidElfFile()) {
        free(m_elfFile);
        m_elfFile = nullptr;
        return false;
    }

    loadSymbols();
    return true;
}

void ELFLoader::dump() const
{
    const Elf32_Ehdr* const elfHeader = header();

    ASSERT(elfHeader->e_machine == EM_386);

    printf("[ELFLoader]\n");
    printf("    type: %u\n", elfHeader->e_type);
    printf("    machine: %u\n", elfHeader->e_machine);
    printf("    version: %u\n", elfHeader->e_version);
    printf("    entry point address: %p\n", reinterpret_cast<void*>(elfHeader->e_entry));
    printf("    program header offset: %u\n", elfHeader->e_phoff);
    printf("    section header offset: %u\n", elfHeader->e_shoff);

    printf("    section headers: %u\n", elfHeader->e_shnum);
    for (ADS::size_t i = 0; i < elfHeader->e_shnum; ++i) {
        const Elf32_Shdr& shdr = sectionHeader()[i];
        printf("        section header #%zu '%s':\n", i, sectionHeaderStringTable() + shdr.sh_name);
        printf("        name: %u\n", shdr.sh_name);
        printf("        type: %u\n", shdr.sh_type);
        printf("        offset: %u\n", shdr.sh_offset);
        printf("        size: %u\n", shdr.sh_size);
        printf("\n");
    }

    printf("    program headers: %u\n", elfHeader->e_phnum);
    for (ADS::size_t i = 0; i < elfHeader->e_phnum; ++i) {
        const Elf32_Phdr& phdr = programHeader()[i];
        printf("        program header #%zu:\n", i);
        printf("        type: %u\n", phdr.p_type);
        printf("        offset: %u\n", phdr.p_offset);
        printf("        virtual address: %p\n", reinterpret_cast<void*>(phdr.p_vaddr));
        printf("        physical address: %p\n", reinterpret_cast<void*>(phdr.p_paddr));
        printf("        flags: %u\n", phdr.p_flags);
        printf("\n");
    }

    printf("Symbol table '%s':\n", sectionHeaderStringTable() + m_symbolTableSectionHeader->sh_name);
    printf("%-5s %-8s %-8s %-20s\n", "Num", "Value", "Type", "Name");
    forEachSymbolIndexed([this](ADS::size_t index, const Elf32_Sym& symbol) {
        const char* const symbolStr = reinterpret_cast<const char*>(getStringFromSectionStringTable(*m_symbolTableSectionHeader, symbol.st_name));
        printf("%-5zu %-8x %-8s %-20s\n", index, symbol.st_value, symbolTypeFor(ELF32_ST_TYPE(symbol.st_info)), symbolStr);
    });
}

const Elf32_Ehdr* ELFLoader::header() const
{
    ASSERT(m_elfFile != nullptr);
    return reinterpret_cast<Elf32_Ehdr*>(m_elfFile);
}

bool ELFLoader::isValidElfFile() const
{
    return memcmp(header()->e_ident, ELFMAG, SELFMAG) == 0;
}

const Elf32_Shdr* ELFLoader::sectionHeader() const
{
    return reinterpret_cast<Elf32_Shdr*>(
        reinterpret_cast<uint8_t*>(m_elfFile) + header()->e_shoff);
}

const uint8_t* ELFLoader::sectionHeaderStringTable() const
{
    const Elf32_Shdr& sectionHeaderStringTable = sectionHeader()[header()->e_shstrndx];
    return reinterpret_cast<uint8_t*>(m_elfFile) + sectionHeaderStringTable.sh_offset;
}

const Elf32_Phdr* ELFLoader::programHeader() const
{
    return reinterpret_cast<Elf32_Phdr*>(
        reinterpret_cast<uint8_t*>(m_elfFile) + header()->e_phoff);
}

void* ELFLoader::findFunc(const ADS::String& name)
{
    if (name.isEmpty())
        return nullptr;

    const Elf32_Sym* symbol = nullptr;
    if (!m_funcSymbols.tryGetValue(name, symbol))
        return nullptr;

    return reinterpret_cast<uint8_t*>(m_elfFile) + m_symbolTableSectionHeader->sh_offset + symbol->st_value;
}

const uint8_t* ELFLoader::getStringFromSectionStringTable(const Elf32_Shdr& section, ADS::size_t stringTableIndex) const
{
    const Elf32_Shdr* const stringTableSection = &sectionHeader()[section.sh_link];
    const uint8_t* const stringTable = reinterpret_cast<uint8_t*>(m_elfFile) + stringTableSection->sh_offset;
    return reinterpret_cast<const uint8_t*>(stringTable + stringTableIndex);
}

void ELFLoader::loadSymbols()
{
    for (ADS::size_t i = 0; i < header()->e_shnum; ++i) {
        const Elf32_Shdr& sh = sectionHeader()[i];
        if (sh.sh_type == SHT_SYMTAB) {
            m_symbolTableSectionHeader = &sh;
            break;
        }
    }
    ASSERT(m_symbolTableSectionHeader != nullptr);

    forEachSymbolIndexed([this](ADS::size_t, const Elf32_Sym& symbol) {
        if (ELF32_ST_TYPE(symbol.st_info) == STT_FUNC) {
            const char* const symbolStr = reinterpret_cast<const char*>(getStringFromSectionStringTable(*m_symbolTableSectionHeader, symbol.st_name));
            m_funcSymbols.set(symbolStr, &symbol);
        }
    });
}

}