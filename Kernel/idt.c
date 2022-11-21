/*
  -------------------------------------------------------------------------
    File name:   idt.c
    Version:     v1.00
    Created:     17/5/2014 by Kevin.
    Description: implement basic Interrupt Description Table methods
   -------------------------------------------------------------------------
    History:
    Todo:
  		What happens when custom irq handler will be interrupted? -> Solved, check custom handler before master/slave reset
*/

#include "idt.h"



void (*interrupt_handlers[256])(registers_t);

static idt_ptr_t idtptr;
static idt_entry_t idt_entries[IDT_LIMIT];
//static isr_t interrupt_handlers[256];

extern void idt_flush(uInt32_t);

void idt_install()
{	

	idtptr.limit = (sizeof(idt_entry_t) * IDT_LIMIT) -1;
	idtptr.base  = (uInt32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t)*IDT_LIMIT);
	//memset(&interrupt_handlers, 0, sizeof(isr_t) * IDT_LIMIT);

	// remap irq table	
	//irq_remap();

	/*idt_set_gate( 0, (uInt32_t)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uInt32_t)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uInt32_t)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (uInt32_t)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (uInt32_t)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (uInt32_t)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (uInt32_t)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (uInt32_t)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (uInt32_t)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (uInt32_t)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (uInt32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uInt32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uInt32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uInt32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uInt32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uInt32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uInt32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uInt32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uInt32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uInt32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uInt32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uInt32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uInt32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uInt32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uInt32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uInt32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uInt32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uInt32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uInt32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uInt32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uInt32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uInt32_t)isr31, 0x08, 0x8E);*/
	
	//irq_install();

    idt_flush((uInt32_t)&idtptr);
}

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
	idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags   = flags /* | 0x60 */;
}