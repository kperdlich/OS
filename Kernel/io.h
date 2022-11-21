////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   io.h
//  Version:     v1.00
//  Created:     17/4/2014 by Kevin.
//  Description: define basic io Methods
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IO_H___
#define __IO_H___

#include "types.h"

// asm ( "assembly code"
//            : output operands                  /* optional */
//            : input operands                   /* optional */
//            : list of clobbered registers      /* optional */
//);

#define UseIntelSyntax asm(".intel_syntax noprefix");
#define UseAtAtSyntax asm(".att_syntax prefix");

void outportb(uInt32_t port, uInt8_t value);
uInt32_t inportb(uInt32_t port);

#endif