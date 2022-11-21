////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   io.h
//  Version:     v1.00
//  Created:     17/4/2014 by Kevin.
//  Description: implement basic io Methods 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "io.h"


void outportb(uInt32_t port, uInt8_t value)
{	
	 //UseAtAtSyntax;
	__asm__ volatile ("outb %%al,%%dx"::"d" (port), "a" (value));
}

uInt32_t inportb(uInt32_t port)
{
	uInt32_t result; // 8 bit should be enaugh
	//UseAtAtSyntax;
	__asm__ volatile ("inb %w1,%b0" : "=a"(result) : "d"(port));
	return result;
}