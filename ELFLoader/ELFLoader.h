//
// Created by n3dry on 04.10.24.
//

#pragma once

#include "ELF.h"
#include "String.h"

namespace ELF {

class ELFLoader final {
public:
    explicit ELFLoader(ADS::String elfFilePath);
    ~ELFLoader();

    bool load();
    void dump();

private:
    bool isValidElfFile();
    Elf32_Ehdr* header();
    Elf32_Shdr* sectionHeader();
    Elf32_Phdr* programHeader();
    uint8_t* sectionHeaderStringTable();

private:
    ADS::String m_elfFilePath;
    void* m_elfFile { nullptr };
};

} // ELF