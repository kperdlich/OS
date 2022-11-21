////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   idt.h
//  Version:     v1.00
//  Created:     17/5/2014 by Kevin.
//  Description: define Interrupt Description Table
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IDT_H__
#define __IDT_H__

#include "types.h"
#include "io.h"
#include "string.h"
#include "video.h"


#define IDT_LIMIT 256


typedef struct registers_t
{
	uInt32_t ds;                  // Data segment selector
    uInt32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uInt32_t int_no, err_code;    // Interrupt number and error code (if applicable)
    uInt32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;


// A struct describing an interrupt gate.
struct idt_entry_struct
{
	uInt16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
	uInt16_t sel;                 // Kernel segment selector.
	uInt8_t  always0;             // This must always be zero.
	uInt8_t  flags;               // More flags. See documentation.
	uInt16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
    uInt16_t limit;
	uInt32_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;
extern void (*interrupt_handlers[256])(registers_t);



//typedef void (*isr_t)(registers_t);

//void isr_handler(registers_t regs);	
void idt_install();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
//extern isr_t interrupt_handlers[256];



#endif