//
// Created by Kevin on 21.11.22.
//

#include "IO.h"

void IO::outb(unsigned int port, unsigned char value)
{
     asm volatile("outb %%al, %%dx"::"d"(port), "a"(value));
}
