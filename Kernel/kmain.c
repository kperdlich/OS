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

#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "keyboard.h"
#include "timer.h"
#include "video.h"

const char* setupGDTState = "Setup GDT";
const char* setupIDTState = "Setup IDT";
const char* setupISRState = "Setup ISR's";
const char* setupIRQState = "Setup IRQ";
const char* setupTimerState = "Setup Timer";
const char* setupKeyboard = "Setup Keyboard";

extern "C" void kmain()
{
    asm volatile("cli");

    Console* con = InitConsole();
    clrConsoleScreen(con);

    printKernelInformations(con);

    k_print(setupGDTState, ColorBlack | ColorLightGray, con);
    gdt_install(); //
    k_print("........................[", ColorBlack | ColorLightGray, con);
    k_print("Ok", ColorBlack | ColorGreen, con);
    k_print("]\n", ColorBlack | ColorLightGray, con);

    k_print(setupIDTState, ColorBlack | ColorLightGray, con);
    idt_install(); //
    k_print("........................[", ColorBlack | ColorLightGray, con);
    k_print("Ok", ColorBlack | ColorGreen, con);
    k_print("]\n", ColorBlack | ColorLightGray, con);

    k_print(setupISRState, ColorBlack | ColorLightGray, con);
    isr_install(); //
    k_print("......................[", ColorBlack | ColorLightGray, con);
    k_print("Ok", ColorBlack | ColorGreen, con);
    k_print("]\n", ColorBlack | ColorLightGray, con);

    k_print(setupIRQState, ColorBlack | ColorLightGray, con);
    irq_install(); //
    k_print("........................[", ColorBlack | ColorLightGray, con);
    k_print("Ok", ColorBlack | ColorGreen, con);
    k_print("]\n", ColorBlack | ColorLightGray, con);

    k_print(setupTimerState, ColorBlack | ColorLightGray, con);
    timer_install(100); //
    k_print("......................[", ColorBlack | ColorLightGray, con);
    k_print("Ok", ColorBlack | ColorGreen, con);
    k_print("]\n", ColorBlack | ColorLightGray, con);

    /*k_print(setupKeyboard, ColorBlack | ColorLightGray, con);
    keyboard_install();		//
    k_print("...................[", ColorBlack | ColorLightGray, con);
    k_print("Ok", ColorBlack | ColorGreen, con);
    k_print("]\n", ColorBlack | ColorLightGray, con);*/

    asm volatile("sti");

    asm volatile("int $0x1");

    for (;;)
        ;
}