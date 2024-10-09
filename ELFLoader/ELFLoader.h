//
// Created by n3dry on 04.10.24.
//

#pragma once

#include "ELF.h"
#include "HashMap.h"
#include "String.h"

namespace ELF {

class ELFLoader final {
public:
    explicit ELFLoader(ADS::String elfFilePath);
    ~ELFLoader();

    bool load();
    void dump() const;

    void* findFunc(const ADS::String& name);

private:
    void loadSymbols();

    [[nodiscard]] bool isValidElfFile() const;
    [[nodiscard]] const Elf64_Ehdr* header() const;
    [[nodiscard]] const Elf64_Shdr* sectionHeader() const;
    [[nodiscard]] const Elf64_Phdr* programHeader() const;
    [[nodiscard]] const char* sectionHeaderStringTable() const;
    [[nodiscard]] const char* getStringFromSectionStringTable(const Elf64_Shdr& section, ADS::size_t stringTableIndex) const;

    template<typename Func>
    void forEachSymbolIndexed(Func func) const;

private:
    ADS::HashMap<ADS::String, const Elf64_Sym*> m_funcSymbols;
    ADS::String m_elfFilePath;
    void* m_elfFile { nullptr };
    const Elf64_Shdr* m_symbolTableSectionHeader { nullptr };
    uint8_t* m_executableTextSection { nullptr };
};

template<typename Func>
void ELFLoader::forEachSymbolIndexed(Func func) const
{
    const uint8_t* const symbolData = reinterpret_cast<uint8_t*>(m_elfFile) + m_symbolTableSectionHeader->sh_offset;
    const ADS::size_t numSymbols = m_symbolTableSectionHeader->sh_size / sizeof(Elf64_Sym);

    for (ADS::size_t i = 0; i < numSymbols; ++i) {
        const Elf64_Sym* const symbol = reinterpret_cast<const Elf64_Sym*>(symbolData + i * sizeof(Elf64_Sym));
        func(i, *symbol);
    }
}

} // ELF