////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   stdlib.h
//  Version:     v1.00
//  Created:     29/4/2014 by Kevin.
//  Description: define standard library 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "types.h"
#include "string.h"


void* malloc(size_t size);

void free(void* ptr);



#endif