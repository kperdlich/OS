////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   string.c
//  Version:     v1.00
//  Created:     13/4/2014 by Kevin.
//  Description: implements methods to work with string
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////


//     0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//--------------------------------------------------------------------
//0   NUL SOH STX ETX EOT ENQ ACK BEL BS  HT  LF  VT  FF  CR  SO  SI
//1   DLE DC1 DC2 DC3 DC4 NAK SYN ETB CAN EM  SUB ESC FS  GS  RS  US
//2    SP  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /
//3    0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?
//4    @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O
//5    P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _
//6    `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o
//7    p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~ DEL


#include "string.h"

void ConvertInt2String(sInt32_t value, unsigned char* valuestring)
{	
	int tenth, min_flag;
	unsigned char swap, *p;
	min_flag = 0;

	if (0 > value)
	{
		*valuestring++ = '-';
		value = -SIGNEDINTMAX > value ? min_flag = SIGNEDINTMAX : -value; //  wenn zahl kleiner als min int dann setze sie auf min int, ansonsten = value = (-1 * -value) / + value; 
	}

	p = valuestring; // p zeigt auf zu erstellenden string

	do
	{
		tenth = value / 10; // factor 10 because of dezimal system
		*p++ = (unsigned char)(value - 10 * tenth + '0'); // add rest to null ascii (48)
		value = tenth;
	}
	while (value != 0);

	if (min_flag != 0)
	{
		++*valuestring;
	}
	*p-- = '\0';

	while (p > valuestring)
	{
		swap = *valuestring;
		*valuestring++ = *p;
		*p-- = swap;
	}

}

// without null terminator!!!
size_t strlen(const char* str)
{
	size_t length = 0;
	char* tmpStr = (char*) str;

	while(*tmpStr++ != '\0')
	{
		length++;
	}
	return length;
}

void* memset(void* ptr, int value, size_t num)
{
	char* start = (char*) ptr;
	while(num--)
	{		
		// num will now be decremented
		((char*)ptr)[num] = value;
	}
	return start;
}