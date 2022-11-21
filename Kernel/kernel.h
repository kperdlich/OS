////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   kernel.h
//  Version:     v1.00
//  Created:     26/4/2014 by Kevin.
//  Description: handle some kernel informations
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "types.h"
#include "io.h"
#include "stdlib.h"


#define __VERSION__ "v1.0"

typedef struct registers
{
	uInt32_t gs, fs, es, ds;
	uInt32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uInt32_t int_no, error_code;
	uInt32_t eip, cs, eflags, useresp, ss;
} registers;


#endif