//
// Created by n3dry on 04.11.24.
//

#include "Paging.h"
#include "Kmalloc.h"
#include "StdLib.h"

namespace Paging {

static PageDirectoryEntry* s_pageDirectory { nullptr };
static constexpr const uint32_t PAGE_TABLE_ENTRIES = 1024;

static PageTableEntry& lookupPageTableEntry(void* virtualAddress)
{
    ASSERT(s_pageDirectory != nullptr);
    const uint32_t pageDirectoryIndex = (reinterpret_cast<uint32_t>(virtualAddress) >> 22) & 0x3FF;
    const uint32_t pageTableIndex = (reinterpret_cast<uint32_t>(virtualAddress) >> 12) & 0x3FF;

    PageDirectoryEntry& pageDirectoryEntry = s_pageDirectory[pageDirectoryIndex];
    ASSERT(pageDirectoryEntry.isPresent());

    return pageDirectoryEntry.pageTable()[pageTableIndex];
}

static void loadPageDirectory(PageDirectoryEntry* pageDirectory)
{
    asm volatile("mov %0, %%cr3" : : "r"(pageDirectory));
}

static void enablePaging()
{
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0)); // Read CR0 into cr0 variable
    cr0 |= 0x80000000; // Set the PG bit (bit 31)
    asm volatile("mov %0, %%cr0" : : "r"(cr0)); // Write back to CR0
}

void initialize()
{
    static_assert(sizeof(PageDirectoryEntry) == sizeof(uint32_t));
    ASSERT(s_pageDirectory == nullptr);

    // Setup kernel page directory for entire 4GB address space (identity map everything).
    s_pageDirectory = reinterpret_cast<PageDirectoryEntry*>(kmalloc(PAGE_SIZE));
    memset(s_pageDirectory, 0, PAGE_SIZE);
    ADS::size_t offset = 0;
    for (ADS::size_t i = 0; i < PAGE_TABLE_ENTRIES; ++i) {
        PageTableEntry* const pageTable = reinterpret_cast<PageTableEntry*>(kmalloc(PAGE_SIZE));
        memset(pageTable, 0, PAGE_SIZE);
        for (ADS::size_t j = 0; j < PAGE_TABLE_ENTRIES; ++j) {
            PageTableEntry& pageTableEntry = pageTable[j];
            pageTableEntry.setPhysicalPageAddress(offset + (j * PAGE_SIZE));
            pageTableEntry.setReadWriteFlag();
            pageTableEntry.setPresentFlag();
            pageTableEntry.setUserSupervisorFlag();
        }
        offset += (PAGE_TABLE_ENTRIES * PAGE_SIZE);
        PageDirectoryEntry& entry = s_pageDirectory[i];
        entry.setPageTable(pageTable);
        entry.setReadWriteFlag();
        entry.setUserSupervisorFlag();
        entry.setPresentFlag();
    }

    // Protect first page frame (page-fault on nullptr dereference)
    PageTableEntry& nullptrPageTableEntry = lookupPageTableEntry(nullptr);
    nullptrPageTableEntry.clearAllFlags();

    loadPageDirectory(s_pageDirectory);
    dbgPrintf("[MemoryManager] Allocated and loaded Kernel Page Directory at %p\n", s_pageDirectory);

    enablePaging();
    dbgPrintf("[MemoryManager] Enabled Paging\n");
}

uint32_t alignToPage(uint32_t addr)
{
    return (addr + Paging::PAGE_SIZE - 1) & ~(Paging::PAGE_SIZE - 1);
}

bool isPageAligned(uint32_t addr)
{
    return (addr % Paging::PAGE_SIZE) == 0;
}

}