////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   gdt.h
//  Version:     v1.00
//  Created:     17/5/2014 by Kevin.
//  Description: define the "Global Descriptor Table"
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __GDT_H__
#define __GDT_H__

#include "types.h"

#define GDT_ENTRIES 5

struct gdt_entry
{
	uInt16_t limit_low;
	uInt16_t base_low;
	uInt8_t base_middle;
	uInt8_t access;
	uInt8_t granularity;
	uInt8_t base_high;
} __attribute__((packed));

struct gdt_pointer
{
	uInt16_t limit;
	uInt32_t base;
} __attribute__((packed));


typedef struct gdt_entry gdt_entry;
typedef struct gdt_pointer gdt_pointer;

void gdt_install();

#endif