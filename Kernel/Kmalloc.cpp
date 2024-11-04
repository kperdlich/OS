//
// Created by n3dry on 30.10.24.
//
#include "Kmalloc.h"
#include "Assert.h"
#include "Kprintf.h"
#include "Paging.h"
#include "StdLib.h"

namespace Heap {

class HeapBlockTableImpl final {
public:
    static constexpr const uint32_t BlockSize = Paging::PAGE_SIZE;

    class BlockTableEntry final {
    public:
        static constexpr const uint8_t IS_FREE_FLAG = 0x00;
        static constexpr const uint8_t IS_TAKEN_FLAG = 0x01;
        static constexpr const uint8_t IS_FIRST_FLAG = 0x02;
        static constexpr const uint8_t HAS_NEXT_FLAG = 0x04;

        [[nodiscard]] inline bool isFree() const
        {
            return m_data == IS_FREE_FLAG;
        }

        [[nodiscard]] inline bool isFirst() const
        {
            return m_data & IS_FIRST_FLAG;
        }

        [[nodiscard]] inline bool isTaken() const
        {
            return m_data & IS_TAKEN_FLAG;
        }

        [[nodiscard]] inline bool hasNext() const
        {
            return m_data & HAS_NEXT_FLAG;
        }

        [[nodiscard]] inline uint8_t data() const
        {
            return m_data;
        }

        inline void setToFree()
        {
            m_data = IS_FREE_FLAG;
        }

        inline void setTakenFlag()
        {
            m_data |= IS_TAKEN_FLAG;
        }

        inline void setIsFirstFlag()
        {
            m_data |= IS_FIRST_FLAG;
        }

        inline void setHasNextFlag()
        {
            m_data |= HAS_NEXT_FLAG;
        }

    private:
        uint8_t m_data;
    };

    HeapBlockTableImpl() = default;
    HeapBlockTableImpl(const HeapBlockTableImpl&) = delete;
    HeapBlockTableImpl(HeapBlockTableImpl&&) = delete;
    HeapBlockTableImpl& operator=(const HeapBlockTableImpl&) = delete;
    HeapBlockTableImpl& operator=(HeapBlockTableImpl&&) = delete;

    void create(void* heapStart, ADS::size_t heapSize)
    {
        static_assert(sizeof(BlockTableEntry) == sizeof(uint8_t));
        ASSERT(!m_initialized);

        m_totalBlockTableEntries = heapSize / BlockSize;
        m_blockTableEntries = reinterpret_cast<BlockTableEntry*>(heapStart);
        memset(m_blockTableEntries, BlockTableEntry::IS_FREE_FLAG, m_totalBlockTableEntries);
        const void* const availableMemoryAfterHeapBlockTable = static_cast<char*>(heapStart) + (m_totalBlockTableEntries * sizeof(BlockTableEntry));
        m_pageAlignedHeapStart = reinterpret_cast<void*>(Paging::alignToPage(reinterpret_cast<uint32_t>(availableMemoryAfterHeapBlockTable)));
        dbgPrintf("[Heap] Initialized: Block Table: %p | Total Blocks: %u | Block Size: %u Bytes | Heap Start: %p\n", m_blockTableEntries, m_totalBlockTableEntries, BlockSize, m_pageAlignedHeapStart);
        m_initialized = true;
    }

    void* malloc(ADS::size_t size)
    {
        static_assert(sizeof(uint32_t) == sizeof(ADS::size_t));
        ASSERT(m_initialized);

        // Find free blocks (First-Fit)
        const ADS::size_t requiredBlocks = Paging::alignToPage(size) / BlockSize;
        ADS::size_t blockCounter = 0;
        ADS::size_t startBlock = UINT32_MAX;
        for (ADS::size_t i = 0; i < m_totalBlockTableEntries; ++i) {
            if (!m_blockTableEntries[i].isFree()) {
                blockCounter = 0;
                startBlock = UINT32_MAX;
                continue;
            }
            if (startBlock == UINT32_MAX)
                startBlock = i;
            ++blockCounter;
            if (blockCounter == requiredBlocks)
                break;
        }

        // Out of memory :(
        ASSERT(startBlock != UINT32_MAX);

        // Mark blocks accordingly
        const ADS::size_t endBlock = startBlock + blockCounter - 1;
        for (ADS::size_t i = startBlock; i <= endBlock; ++i) {
            BlockTableEntry& entry = m_blockTableEntries[i];
            entry.setToFree();
            entry.setTakenFlag();
            if (i == 0)
                entry.setIsFirstFlag();
            if (i < endBlock)
                entry.setHasNextFlag();
#if 0
            dbgPrintf("[Heap] Allocated Block Entry[%u]: 0x%x | Taken: %u | First: %u | Next: %u\n", i, entry.data(), entry.isTaken(), entry.isFirst(), entry.hasNext());
#endif
        }

        // Get pointer to memory
        void* const ptr = reinterpret_cast<char*>(m_pageAlignedHeapStart) + (startBlock * BlockSize);
#if 0
        dbgPrintf("[Heap] Allocated Size: %u Bytes | Start Block: %u | End Block: %u | Total Blocks: %u | Ptr: %p\n", size, startBlock, endBlock, blockCounter, ptr);
#endif
        return ptr;
    }

    void free(void* ptr)
    {
        ASSERT(m_initialized);

        ADS::size_t freedBlocks {};
        const ADS::size_t blockIndex = (reinterpret_cast<char*>(ptr) - reinterpret_cast<char*>(m_pageAlignedHeapStart)) / BlockSize;
        for (ADS::size_t i = blockIndex; i < m_totalBlockTableEntries; ++i) {
            BlockTableEntry& entry = m_blockTableEntries[i];
            BlockTableEntry copy = entry;
            ASSERT(entry.isTaken());
            entry.setToFree();
            ++freedBlocks;
            if (!copy.hasNext())
                break;
        }
#if 0
        dbgPrintf("[Heap] Freed Memory: %p | Start Block Index: %u | Freed Blocks: %u\n", ptr, blockIndex, freedBlocks);
#endif
    }

private:
    BlockTableEntry* m_blockTableEntries { nullptr };
    ADS::size_t m_totalBlockTableEntries {};
    void* m_pageAlignedHeapStart { nullptr };
    bool m_initialized { false };
};

static HeapBlockTableImpl s_heap {};

void initialize(void* heapStart, ADS::size_t heapSize)
{
    ASSERT(heapStart != nullptr);
    ASSERT(heapSize > 0);
    ASSERT(Paging::isPageAligned(reinterpret_cast<uint32_t>(heapStart)));

    s_heap.create(heapStart, heapSize);
}

}

void* kmalloc(ADS::size_t size)
{
    return Heap::s_heap.malloc(size);
}

void kfree(void* ptr)
{
    return Heap::s_heap.free(ptr);
}

void* operator new(ADS::size_t size)
{
    return kmalloc(size);
}

void* operator new[](ADS::size_t size)
{
    return kmalloc(size);
}

void operator delete(void* ptr)
{
    return kfree(ptr);
}

void operator delete[](void* ptr)
{
    return kfree(ptr);
}

void operator delete(void* ptr, ADS::size_t)
{
    return kfree(ptr);
}

void operator delete[](void* ptr, ADS::size_t)
{
    return kfree(ptr);
}
