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
    void dump() const;

private:
    [[nodiscard]] bool isValidElfFile() const;
    [[nodiscard]] const Elf32_Ehdr* header() const;
    [[nodiscard]] const Elf32_Shdr* sectionHeader() const;
    [[nodiscard]] const Elf32_Phdr* programHeader() const;
    [[nodiscard]] const uint8_t* sectionHeaderStringTable() const;

private:
    ADS::String m_elfFilePath;
    void* m_elfFile { nullptr };
};

} // ELF