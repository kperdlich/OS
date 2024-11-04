//
// Created by n3dry on 29.10.24.
//

#pragma once

#include "Multiboot.h"
#include "Types.h"

namespace MemoryManager {

constexpr const ADS::size_t PAGE_SIZE = 4096;

bool isPageAligned(uint32_t addr);
uint32_t alignToPage(uint32_t addr);
void initialize(multiboot_info_t& multibootInfo);

}
