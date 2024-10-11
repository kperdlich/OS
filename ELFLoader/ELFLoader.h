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
    void clear();

    void* findFunc(const ADS::String& name);

private:
    void loadSymbols();
    void applyRelocations();
    void applyRelocation(const Elf64_Shdr& section, char* runtimeSectionMemory);

    [[nodiscard]] bool isValidElfFile() const;
    [[nodiscard]] const Elf64_Ehdr* header() const;
    [[nodiscard]] const Elf64_Shdr* sectionHeader() const;
    [[nodiscard]] const Elf64_Phdr* programHeader() const;
    [[nodiscard]] const char* sectionHeaderStringTable() const;
    [[nodiscard]] const Elf64_Shdr* findSection(const char* sectionName) const;
    [[nodiscard]] const char* getStringFromSectionStringTable(const Elf64_Shdr& section, ADS::size_t stringTableIndex) const;
    [[nodiscard]] const Elf64_Sym* symbolTable() const;
    [[nodiscard]] const Elf64_Sym* symbolByIndex(uint32_t index) const;

    template<typename Func>
    void forEachSymbolIndexed(Func func) const;

private:
    ADS::HashMap<ADS::String, const Elf64_Sym*> m_funcSymbols;
    ADS::String m_elfFilePath;
    ADS::size_t m_mappedFileSize {};
    char* m_mappedElfFile { nullptr };
    const Elf64_Shdr* m_symbolTableSectionHeader { nullptr };
    char* m_executableTextSection { nullptr };
    ADS::size_t m_executableTextSectionSize {};
};

template<typename Func>
void ELFLoader::forEachSymbolIndexed(Func func) const
{
    const Elf64_Sym* const symbolData = reinterpret_cast<const Elf64_Sym*>(m_mappedElfFile + m_symbolTableSectionHeader->sh_offset);
    const ADS::size_t numSymbols = m_symbolTableSectionHeader->sh_size / sizeof(Elf64_Sym);

    for (ADS::size_t i = 0; i < numSymbols; ++i) {
        func(i, symbolData[i]);
    }
}

} // ELF