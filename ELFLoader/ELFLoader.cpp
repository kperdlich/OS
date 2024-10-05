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

    printf("    section headers:\n");
    for (ADS::size_t i = 0; i < elfHeader->e_shnum; ++i) {
        const Elf32_Shdr& shdr = sectionHeader()[i];
        printf("        section header #%zu '%s':\n", i, sectionHeaderStringTable() + shdr.sh_name);
        printf("        name: %u\n", shdr.sh_name);
        printf("        type: %u\n", shdr.sh_type);
        printf("        offset: %u\n", shdr.sh_offset);
        printf("        size: %u\n", shdr.sh_size);
        printf("\n");
    }

    printf("    program headers:\n");
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

    // Dump symbols
    const Elf32_Shdr* symtab = nullptr;
    const Elf32_Shdr* strtab = nullptr;
    for (ADS::size_t i = 0; i < elfHeader->e_shnum; ++i) {
        const Elf32_Shdr& shdr = sectionHeader()[i];
        if (shdr.sh_type == SHT_SYMTAB) {
            symtab = &shdr;
            strtab = &sectionHeader()[shdr.sh_link];
            break;
        }
    }
    ASSERT(symtab != nullptr);
    ASSERT(strtab != nullptr);

    const uint8_t* const symtabData = reinterpret_cast<const uint8_t*>(m_elfFile) + symtab->sh_offset;
    const uint8_t* const strtabData = reinterpret_cast<const uint8_t*>(m_elfFile) + strtab->sh_offset;
    const ADS::size_t numSymbols = symtab->sh_size / sizeof(Elf32_Sym);

    printf("Symbol table '%s' contains %zu entries:\n", sectionHeaderStringTable() + symtab->sh_name, numSymbols);
    printf("%-5s %-8s %-20s\n", "Num", "Type", "Name");
    for (ADS::size_t i = 0; i < numSymbols; ++i) {
        const Elf32_Sym* const symbol = reinterpret_cast<const Elf32_Sym*>(symtabData + i * sizeof(Elf32_Sym));
        const char* const symbolStr = reinterpret_cast<const char*>(strtabData + symbol->st_name);
        printf("%-5zu %-8s %-20s\n", i, symbolTypeFor(ELF32_ST_TYPE(symbol->st_info)), symbolStr);
    }
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
    // Get Section Header that contains the offset to the string table.
    const Elf32_Shdr& sectionHeaderStringTable = sectionHeader()[header()->e_shstrndx];
    // Lookup string table via offset.
    return reinterpret_cast<uint8_t*>(m_elfFile) + sectionHeaderStringTable.sh_offset;
}

const Elf32_Phdr* ELFLoader::programHeader() const
{
    return reinterpret_cast<Elf32_Phdr*>(
        reinterpret_cast<uint8_t*>(m_elfFile) + header()->e_phoff);
}

}