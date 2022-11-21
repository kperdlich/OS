////////////////////////////////////////////////////////////////////////////
//
// -------------------------------------------------------------------------
//  File name:   video.h
//  Version:     v1.00
//  Created:     12/4/2014 by Kevin.
//  Description: header for all video methods
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "types.h"
#include "string.h"
#include "io.h"
#include "kernel.h"

#define ColorBlack					0x0
#define	ColorBlue					0x1
#define	ColorGreen					0x2
#define	ColorCyan					0x3
#define	ColorRed					0x4
#define	ColorMagenta				0x5
#define	ColorBrown					0x6
#define	ColorLightGray				0x7
#define	ColorDarkGray				0x8
#define	ColorLightBlue				0x9
#define	ColorLightGreen				0xA
#define	ColorLightCyan				0xB
#define	ColorLightRed				0xC
#define	ColorLightMagenta			0xD
#define	ColorYellow					0xE
#define	ColorWhite					0xF
#define YMAX						24
#define XMAX						79
#define YMIN						0x0
#define XMIN						0x0
#define VideoMemory					0xb8000
#define VGACursorCrtHigh			0x3D5
#define VGACursorCrtLow				0x3D4

typedef struct Console {

	uInt8_t X;
	uInt8_t Y;
	uInt16_t (*calcEffectiveConsolePosition)(struct Console*);
} Console;


Console* InitConsole();

Console* GetConsole();

void k_print(unsigned char* string, uInt8_t color, Console *pos);

void clrConsoleScreen(Console *console);

void printKernelInformations(Console *console);

#endif