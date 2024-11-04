//
// Created by n3dry on 04.11.24.
//

#pragma once

#include "Types.h"
namespace Paging {

constexpr const ADS::size_t PAGE_SIZE = 4096;

class PageTableEntry;

class PageDirectoryEntry final {
public:
    static constexpr const uint32_t PRESENT_FLAG = 0x01;
    static constexpr const uint32_t READ_WRITE_FLAG = 0x02;
    static constexpr const uint32_t USER_SUPERVISOR_FLAG = 0x04;
    static constexpr uint32_t ADDRESS_MASK = 0xFFFFF000;

    inline void setPageTable(PageTableEntry* pageTableBase)
    {
        m_data = (m_data & ~ADDRESS_MASK) | (reinterpret_cast<uint32_t>(pageTableBase) & ADDRESS_MASK);
    }

    [[nodiscard]] PageTableEntry* pageTable()
    {
        return reinterpret_cast<PageTableEntry*>(m_data & ADDRESS_MASK);
    }

    inline void setPresentFlag()
    {
        m_data |= PRESENT_FLAG;
    }

    inline void clearPresentFlag()
    {
        m_data &= ~PRESENT_FLAG;
    }

    inline void setReadWriteFlag()
    {
        m_data |= READ_WRITE_FLAG;
    }

    inline void setUserSupervisorFlag()
    {
        m_data |= USER_SUPERVISOR_FLAG;
    }

    [[nodiscard]] inline bool isPresent() const
    {
        return m_data & PRESENT_FLAG;
    }

private:
    uint32_t m_data {};
};

class PageTableEntry final {
public:
    static constexpr const uint32_t PRESENT_FLAG = 0x01;
    static constexpr const uint32_t READ_WRITE_FLAG = 0x02;
    static constexpr const uint32_t USER_SUPERVISOR_FLAG = 0x04;
    static constexpr uint32_t ADDRESS_MASK = 0xFFFFF000;

    inline void setPhysicalPageAddress(uint32_t value)
    {
        m_data = (m_data & ~ADDRESS_MASK) | (value & ADDRESS_MASK);
    }

    inline void setPresentFlag()
    {
        m_data |= PRESENT_FLAG;
    }

    inline void clearPresentFlag()
    {
        m_data &= ~PRESENT_FLAG;
    }

    inline void setReadWriteFlag()
    {
        m_data |= READ_WRITE_FLAG;
    }

    inline void setUserSupervisorFlag()
    {
        m_data |= USER_SUPERVISOR_FLAG;
    }

    [[nodiscard]] inline bool isPresent() const
    {
        return m_data & PRESENT_FLAG;
    }

    void clearAllFlags()
    {
        m_data = 0;
    }

private:
    uint32_t m_data {};
};

bool isPageAligned(uint32_t addr);
uint32_t alignToPage(uint32_t addr);
void initialize();

}