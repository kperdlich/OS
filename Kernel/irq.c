#include "irq.h"

void irq_remap()
{
	// remap the irq table
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);
}


void irq_install()
{
	void (*handler)(registers_t) = 0;
	memset(&interrupt_handlers, 0, sizeof(handler) * IDT_LIMIT);
	
	irq_remap();

	idt_set_gate(32, (uInt32_t) irq0, 0x08, 0x8E);
	idt_set_gate(33, (uInt32_t) irq1, 0x08, 0x8E);
	idt_set_gate(34, (uInt32_t) irq2, 0x08, 0x8E);
	idt_set_gate(35, (uInt32_t) irq3, 0x08, 0x8E);
	idt_set_gate(36, (uInt32_t) irq4, 0x08, 0x8E);
	idt_set_gate(37, (uInt32_t) irq5, 0x08, 0x8E);
	idt_set_gate(38, (uInt32_t) irq6, 0x08, 0x8E);
	idt_set_gate(39, (uInt32_t) irq7, 0x08, 0x8E);
	idt_set_gate(40, (uInt32_t) irq8, 0x08, 0x8E);
	idt_set_gate(41, (uInt32_t) irq9, 0x08, 0x8E);
	idt_set_gate(42, (uInt32_t) irq10, 0x08, 0x8E);
	idt_set_gate(43, (uInt32_t) irq11, 0x08, 0x8E);
	idt_set_gate(44, (uInt32_t) irq12, 0x08, 0x8E);
	idt_set_gate(45, (uInt32_t) irq13, 0x08, 0x8E);
	idt_set_gate(46, (uInt32_t) irq14, 0x08, 0x8E);
	idt_set_gate(47, (uInt32_t) irq15, 0x08, 0x8E);
}

void register_interrupt_handler(uInt8_t n, void (*handler)(registers_t))
{
    interrupt_handlers[n] = handler;
}

void unregister_interrupt_handler(uInt8_t intHandler)
{
	interrupt_handlers[intHandler] = 0;
}

void irq_handler(registers_t regs)
{	

	Console* con = GetConsole();
	

	// custom handler exist?
	if (interrupt_handlers[regs.int_no] != 0)
	{		// run custom method

		void (*handler)(registers_t) = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	// Master IRQ: 0 -> 7 = 32 -> 39
	// Slave IRQ: 8 -> 15 = 40 -> 47

	if (regs.int_no >= 40)      
	{
		// send reset signal to slave
		outportb(0xA0, 0x20);
	}
	// Send reset signal to master
	outportb(0x20, 0x20);
}

