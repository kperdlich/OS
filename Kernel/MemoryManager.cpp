//
// Created by n3dry on 29.10.24.
//

#include "MemoryManager.h"
#include "Assert.h"
#include "Kprintf.h"
#include "Types.h"

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

void Initialize(multiboot_info_t& multibootInfo)
{
    ASSERT(multibootInfo.flags & MULTIBOOT_INFO_MEM_MAP);

    dbgPrintf("[MemoryManager] Reading memory map from multiboot info...\n");
    for (ADS::size_t i = 0; i < multibootInfo.mmap_length; i += sizeof(multiboot_memory_map_t)) {
        const multiboot_memory_map_t* const memoryMapEntry = reinterpret_cast<multiboot_memory_map_t*>(multibootInfo.mmap_addr + i);
        dbgPrintf("Size: 0x%x | Address: 0x%x | Len: 0x%x | Type: %s\n", memoryMapEntry->size, static_cast<uint32_t>(memoryMapEntry->addr), static_cast<uint32_t>(memoryMapEntry->len), MemoryMapTypeToString(memoryMapEntry->type));
    }

    dbgPrintf("MemoryManager initialized\n");
}

}
