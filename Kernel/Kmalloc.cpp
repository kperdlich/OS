//
// Created by n3dry on 30.10.24.
//
#include "Kmalloc.h"
#include "Assert.h"
#include "Kprintf.h"
#include "MemoryManager.h"

namespace Heap {

static void* s_heapStart { nullptr };
static ADS::size_t s_heapSize {};

void initialize(void* heapStart, ADS::size_t heapSize)
{
    ASSERT(heapStart != nullptr);
    ASSERT(heapSize > 0);
    ASSERT(MemoryManager::isPageAligned(reinterpret_cast<uint32_t>(heapStart)));

    s_heapSize = heapSize;
    s_heapStart = heapStart;
    dbgPrintf("Heap initialized: Start: %p | End: %p | Size: %u Bytes\n", s_heapStart, reinterpret_cast<uint32_t>(s_heapStart) + s_heapSize, s_heapSize);
}

}

void* kmalloc(ADS::size_t)
{
    return nullptr;
}

void kfree(void*)
{
}
