////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   kmain.c
//  Version:     v1.00
//  Created:     29/3/2014 by Kevin.
//  Description: 32 Bit Kernel entry point
// -------------------------------------------------------------------------
//  History:
//	Info: IRQ 13 Problem -> show int 8, 10 -> 14
//
////////////////////////////////////////////////////////////////////////////

#include "video.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "timer.h"
#include "keyboard.h"


uInt8_t setupGDTState[] = "Setup GDT";
uInt8_t setupIDTState[] = "Setup IDT";
uInt8_t setupISRState[] = "Setup ISR's";
uInt8_t setupIRQState[] = "Setup IRQ";
uInt8_t setupTimerState[] = "Setup Timer";
uInt8_t setupKeyboard[] = "Setup Keyboard";

void main()
{	
	__asm__ volatile("cli");

	Console* con = InitConsole();
	clrConsoleScreen(con);	

	printKernelInformations(con);
	
	k_print(setupGDTState, ColorBlack | ColorLightGray, con);
	gdt_install();		//
	k_print("........................[", ColorBlack | ColorLightGray, con);
	k_print("Ok", ColorBlack | ColorGreen, con);
	k_print("]\n", ColorBlack | ColorLightGray, con);
	
	
	k_print(setupIDTState, ColorBlack | ColorLightGray, con);
	idt_install();		//
	k_print("........................[", ColorBlack | ColorLightGray, con);
	k_print("Ok", ColorBlack | ColorGreen, con);
	k_print("]\n", ColorBlack | ColorLightGray, con);


	k_print(setupISRState, ColorBlack | ColorLightGray, con);
	isr_install();		//
	k_print("......................[", ColorBlack | ColorLightGray, con);
	k_print("Ok", ColorBlack | ColorGreen, con);
	k_print("]\n", ColorBlack | ColorLightGray, con);

	k_print(setupIRQState, ColorBlack | ColorLightGray, con);
	irq_install();		//
	k_print("........................[", ColorBlack | ColorLightGray, con);
	k_print("Ok", ColorBlack | ColorGreen, con);
	k_print("]\n", ColorBlack | ColorLightGray, con);

	k_print(setupTimerState, ColorBlack | ColorLightGray, con);
	timer_install(100);		//
	k_print("......................[", ColorBlack | ColorLightGray, con);
	k_print("Ok", ColorBlack | ColorGreen, con);
	k_print("]\n", ColorBlack | ColorLightGray, con);

	k_print(setupKeyboard, ColorBlack | ColorLightGray, con);
	keyboard_install();		//
	k_print("...................[", ColorBlack | ColorLightGray, con);
	k_print("Ok", ColorBlack | ColorGreen, con);
	k_print("]\n", ColorBlack | ColorLightGray, con);
	
	
	__asm__ volatile("sti");

	__asm__ volatile("int $0x1");

	for(;;);   
}