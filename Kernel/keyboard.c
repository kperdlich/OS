////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   keyboard.c
//  Version:     v1.00
//  Created:     26/4/2014 by Kevin.
//  Description: Implement methods for keyboard actions
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////


#include "keyboard.h"

unsigned char keymap[128] = // const -> .rodata = work // without const -> .data = wont work
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


void keyboard_init()
{
    while( inportb(0x64)&1 )
        inportb(0x60);
}

unsigned char FetchAndAnalyzeScancode()
{
	Console* con = GetConsole();
	unsigned char scancode;
	scancode = inportb(0x60);
	
	
	if (scancode & 0x80)
	{
		/* You can use this one to see if the user released the
		*  shift, alt, or control keys... */
		
	} else {

		unsigned char asciiChar = keymap[scancode];
		k_print(&asciiChar, ColorBlack | ColorRed, con);		
	}
}

unsigned char k_getch() // Scancode --> ASCII
{
	
}

void keyboard_handler(registers_t regs)
{
	Console* con = GetConsole();	
	FetchAndAnalyzeScancode();	
}

void keyboard_install()
{	
	register_interrupt_handler(IRQ1, keyboard_handler);
	keyboard_init();
}

void keyboard_uninstall()
{
	unregister_interrupt_handler(IRQ1);
}

