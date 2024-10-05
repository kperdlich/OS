//
// Created by n3dry on 04.10.24.
//

#include "ELFLoader.h"
#include <cstdio>

namespace ELF {

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

void ELFLoader::dump()
{
    const Elf32_Ehdr* elfHeader = header();

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
}

Elf32_Ehdr* ELFLoader::header()
{
    ASSERT(m_elfFile != nullptr);
    return reinterpret_cast<Elf32_Ehdr*>(m_elfFile);
}

bool ELFLoader::isValidElfFile()
{
    return memcmp(header()->e_ident, ELFMAG, SELFMAG) == 0;
}

Elf32_Shdr* ELFLoader::sectionHeader()
{
    return reinterpret_cast<Elf32_Shdr*>(
        reinterpret_cast<uint8_t*>(m_elfFile) + header()->e_shoff);
}

uint8_t* ELFLoader::sectionHeaderStringTable()
{
    // Get Section Header that contains the offset to the string table.
    const Elf32_Shdr& sectionHeaderStringTable = sectionHeader()[header()->e_shstrndx];
    // Lookup string table via offset.
    return reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(m_elfFile) + sectionHeaderStringTable.sh_offset);
}

Elf32_Phdr* ELFLoader::programHeader()
{
    return reinterpret_cast<Elf32_Phdr*>(
        reinterpret_cast<uint8_t*>(m_elfFile) + header()->e_phoff);
}

}