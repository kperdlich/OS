////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   irq.h
//  Version:     v1.00
//  Created:     17/5/2014 by Kevin.
//  Description: define basic interrupt service request Methods
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IRQ_H__
#define __IRQ_H__

#include "idt.h"
#include "io.h"

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

void irq_install();
// void irq_remap();

void register_interrupt_handler(uInt8_t n, void (*handler)(registers_t));
void unregister_interrupt_handler(uInt8_t intHandler);

/* Own ISR pointing to individual IRQ handler instead of the regular 'fault_handler' function */
extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

#endif