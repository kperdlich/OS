////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   gdt.c
//  Version:     v1.00
//  Created:     18/5/2014 by Kevin.
//  Description: Implement the "Global Description Table"
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "gdt.h"

extern void gdt_flush(uInt32_t);
gdt_entry gdt_entries[GDT_ENTRIES];
gdt_pointer gdt_ptr;

static void gdt_set_gate(sInt32_t, uInt32_t, uInt32_t, uInt8_t, uInt8_t);
static void init_gdt();

void gdt_install()
{
	init_gdt();
}

static void init_gdt()
{
	
	gdt_ptr.limit = (sizeof(gdt_entry) * GDT_ENTRIES) -1;
	gdt_ptr.base = (uInt32_t)&gdt_entries;
	
	gdt_set_gate(0, 0, 0, 0, 0); // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

	gdt_flush((uInt32_t)&gdt_ptr);
}

static void gdt_set_gate(sInt32_t num, uInt32_t base, uInt32_t limit, uInt8_t access, uInt8_t gran)
{
	gdt_entries[num].base_low = (base & 0xFFFF); 
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_low = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access = access;
}