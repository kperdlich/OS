////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   video.c
//  Version:     v1.00
//  Created:     12/4/2014 by Kevin.
//  Description: implement video.h
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "video.h"


//*************************** Static Begin ***************************//

Console debugConsole;


static void resetConsolePos(Console *pos)
{
	pos->X = XMIN;
	pos->Y = YMIN;	
}

static void jumpToNewLine(Console *pos)
{
	pos->X = XMIN;
	pos->Y++;
}

static void checkConsole(Console *con)
{
	if (con->X > XMAX)
	{	
		jumpToNewLine(con);

		if (con->Y > YMAX)
		{
			// TODO - instead of jumping to the first row push all rows one row up (need heap for it)
			resetConsolePos(con);	
		}
	}
}

static void updateCursor(Console *pos)
{
	uInt16_t effPos = pos->calcEffectiveConsolePosition(pos);

	outportb(VGACursorCrtLow, 0x0E);
	outportb(0x3D5, (uInt8_t)((effPos>>8)&0xFF));

	outportb(VGACursorCrtLow, 0x0F);
	outportb(0x3D5, (uInt8_t)(effPos&0xFF));

}

static uInt16_t calcEffectivPosition(uInt8_t x, uInt8_t y)
{
	return (y * (XMAX +1)) + x;
}


static uInt16_t calcEffectiveConsolePosition(Console *self)
{
	return calcEffectivPosition(self->X, self->Y);
}


static uInt32_t calc_VPtr(uInt8_t x, uInt8_t y)
{
	uInt16_t *memoryPtr;
	memoryPtr = (uInt16_t*) VideoMemory;
	uInt16_t i;
	
	for(i = 0; i < (y * 80) + x; i++)
	{
		memoryPtr++;
	}

	return (uInt32_t) memoryPtr;
}

static uInt32_t calc_vPtrForConsole(Console *pos)
{
	return calc_VPtr(pos->X, pos->Y);
}

static void putC(unsigned char character, uInt8_t color, uInt32_t memory)
{	
	unsigned char *memoryPtr;
	memoryPtr = (unsigned char*) memory; 
	*memoryPtr = character;
	memoryPtr++;
	*memoryPtr = color;
}

static void handleNewLine(Console *con)
{
	jumpToNewLine(con);
	checkConsole(con);
	updateCursor(con);
}

static void updateConsolePos(Console *pos)
{	
	pos->X++;
	checkConsole(pos);
	updateCursor(pos);
}


//*************************** Static End ***************************//


Console* InitConsole()
{
	Console* dCon = GetConsole();
	dCon->calcEffectiveConsolePosition = calcEffectiveConsolePosition;
	//clrConsoleScreen(dCon);	
	//resetConsolePos(dCon);
	dCon->Y = 1;
	dCon->X = 0;
	updateCursor(dCon);
	return dCon;
}

Console* GetConsole()
{
	return &debugConsole;
}


void k_print(unsigned char* string, uInt8_t color, Console *pos)
{
	unsigned char* tmpString = (unsigned char*) string;
	while(*tmpString != '\0')
	{
		uInt32_t memoryPtr = calc_vPtrForConsole(pos);
		
		switch(*tmpString)
		{
			case '\n':
				handleNewLine(pos);
				break;		

			default:
				putC(*tmpString, color, memoryPtr);
				updateConsolePos(pos);
				break;
		}			
		tmpString++;		
	}
}

void clrConsoleScreen(Console *console)
{
	uInt8_t *memoryPtr = (uInt8_t*) VideoMemory;
	uInt16_t i = 0;
	for(i = 0; i < (XMAX+1) * (YMAX+1); i++)
	{
		*memoryPtr = ' ';
		memoryPtr++;
		*memoryPtr = ColorLightGray;
		memoryPtr++;
	}
	resetConsolePos(console);	
	updateCursor(console);	
}

void printKernelInformations(Console *console)
{	
	k_print("IntoxOS " __VERSION__ " - Created by Kevin Perdlich \n", ColorBlack | ColorLightGray, console);
	k_print("Compiled at " __DATE__ " " __TIME__ "\n\n", ColorBlack | ColorLightGray, console);	
	//k_print(">", ColorBlack | ColorLightGray, console);
}