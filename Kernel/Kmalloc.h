//
// Created by n3dry on 30.10.24.
//

#pragma once

#include "Types.h"

namespace Heap {
    void initialize(void* heapStart, ADS::size_t heapSize);
}

void* kmalloc(ADS::size_t size);
void kfree(void* ptr);
