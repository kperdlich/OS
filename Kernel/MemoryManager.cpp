//
// Created by n3dry on 29.10.24.
//

#include "MemoryManager.h"
#include "Assert.h"
#include "Kmalloc.h"
#include "Kprintf.h"

namespace MemoryManager {

static const char* MemoryMapTypeToString(uint32_t type)
{
    switch (type) {
    case MULTIBOOT_MEMORY_AVAILABLE:
        return "AVAILABLE";
    case MULTIBOOT_MEMORY_RESERVED:
        return "RESERVED";
    case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
        return "RECLAIMABLE";
    case MULTIBOOT_MEMORY_NVS:
        return "NVS";
    case MULTIBOOT_MEMORY_BADRAM:
        return "BADRAM";
    default:
        ASSERT_NOT_REACHED();
        return nullptr;
    }
}

extern "C" void* kernelEnd; // set by the linker

void Initialize(multiboot_info_t& multibootInfo)
{
    ASSERT(multibootInfo.flags & MULTIBOOT_INFO_MEM_MAP);

    dbgPrintf("[MemoryManager] Reading memory map from multiboot info...\n");
    void* heapMemorySection = nullptr;
    ADS::size_t heapMemorySectionSize = 0;
    for (ADS::size_t i = 0; i < multibootInfo.mmap_length; i += sizeof(multiboot_memory_map_t)) {
        const multiboot_memory_map_t* const memoryMapEntry = reinterpret_cast<multiboot_memory_map_t*>(multibootInfo.mmap_addr + i);
        const bool sectionContainsKernel = memoryMapEntry->addr < reinterpret_cast<uint32_t>(&kernelEnd)
            && (memoryMapEntry->addr + memoryMapEntry->len) > reinterpret_cast<uint32_t>(&kernelEnd);
        // FIXME: This is bit weird! We want to get the biggest available memory region for the heap, but most likely the kernel lives inside it too..
        if (memoryMapEntry->type == MULTIBOOT_MEMORY_AVAILABLE && memoryMapEntry->len > heapMemorySectionSize && sectionContainsKernel) {
            heapMemorySection = reinterpret_cast<void*>(memoryMapEntry->addr);
            heapMemorySectionSize = static_cast<ADS::size_t>(memoryMapEntry->len);
        }
        dbgPrintf("[MemoryManager] Size: 0x%x | Start: 0x%x | End: 0x%x | Len: %u Bytes | Type: %s\n",
            memoryMapEntry->size,
            static_cast<uint32_t>(memoryMapEntry->addr),
            static_cast<uint32_t>(memoryMapEntry->addr) + static_cast<uint32_t>(memoryMapEntry->len),
            static_cast<uint32_t>(memoryMapEntry->len),
            MemoryMapTypeToString(memoryMapEntry->type));
    }

    dbgPrintf("[MemoryManager] Located Kernel: Memory Section: %p | Ends: %p\n", heapMemorySection, &kernelEnd);

    const uint32_t kernelEndAligned = alignToPage(reinterpret_cast<uint32_t>(&kernelEnd));
    ASSERT(reinterpret_cast<uint32_t>(kernelEndAligned) > reinterpret_cast<uint32_t>(&kernelEnd));
    const uint32_t kernelAlignedSize = kernelEndAligned - reinterpret_cast<uint32_t>(heapMemorySection);
    void* const heapStart = reinterpret_cast<void*>(kernelEndAligned);
    const ADS::size_t heapSize = heapMemorySectionSize - kernelAlignedSize;
    Heap::initialize(heapStart, heapSize);

    dbgPrintf("MemoryManager initialized\n");
}

uint32_t alignToPage(uint32_t addr)
{
    return (addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}

bool isPageAligned(uint32_t addr)
{
    return (addr % MemoryManager::PAGE_SIZE) == 0;
}

}
