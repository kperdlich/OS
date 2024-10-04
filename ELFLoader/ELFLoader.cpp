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
    printf("[ELFLoader]\n");
    printf("    type: %u\n", header()->e_type);
    printf("    machine: %u\n", header()->e_machine);
    printf("    version: %u\n", header()->e_version);
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

}