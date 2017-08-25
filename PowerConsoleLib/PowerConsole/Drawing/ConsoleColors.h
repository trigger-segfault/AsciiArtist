/*=================================================================|
|* File:				ConsoleColors.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONSOLE_COLORS_H
#define CONSOLE_COLORS_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole::Drawing {
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/

/* The types of default console colors. */
enum ConsoleColors : unsigned char {
	BLACK			= 0x0,
	DARK_BLUE		= 0x1,
	DARK_GREEN		= 0x2,
	DARK_CYAN		= 0x3,
	DARK_RED		= 0x4,
	DARK_MAGENTA	= 0x5,
	DARK_YELLOW		= 0x6,
	GRAY			= 0x7,
	DARK_GRAY		= 0x8,
	BLUE			= 0x9,
	GREEN			= 0xA,
	CYAN			= 0xB,
	RED				= 0xC,
	MAGENTA			= 0xD,
	YELLOW			= 0xE,
	WHITE			= 0xF
};

//=================================================================|
// FUNCTIONS													   |
//=================================================================/

/* Converts to console colors into a foreground and background. */
#define toColor(bcolor, fcolor) ((unsigned char)(((bcolor) << 4) | (fcolor)))
/* Converts to console colors into a shifted background color. */
#define toBColor(bcolor) ((unsigned char)((bcolor) << 4))

/* Gets the foreground color of the specified color. */
#define	getFColor(color)	((unsigned char)((color) & 0x0F))
/* Gets the background color of the specified color. */
#define	getBColor(color)	((unsigned char)(((color) & 0xF0) >> 4))
/* Swaps the foreground and background colors of the specified color. */
#define	swapColor(color)	((unsigned char)((((color) & 0x0F) << 4) | (((color) & 0xF0) >> 4)))

/* Sets the foreground color of the specified color. */
#define	setFColor(color, fcolor)	((unsigned char)(((color) & 0xF0) | ((fcolor) & 0x0F)))
/* Sets the background color of the specified color. */
#define	setBColor(color, bcolor)	((unsigned char)(((color) & 0x0F) | ((bcolor) & 0xF0)))

/* Inverts the specified color. */
#define invertColor(color)			((unsigned char)((unsigned char)0xFF - (color)))
/* Inverts the foreground color of the specified color. */
#define invertFColor(color)			setFColor(color, (unsigned char)0x0F - ((color) & 0x0F))
/* Inverts the background color of the specified color. */
#define invertBColor(color)			setBColor(color, (unsigned char)0xF0 - ((color) & 0xF0))

/* Lightens the specified color. */
#define lightenColor(color)			((unsigned char)((color) | 0x88))
/* Lightens the foreground color of the specified color. */
#define lightenFColor(color)		setFColor(color, (color) | 0x08)
/* Lightens the background color of the specified color. */
#define lightenBColor(color)		setBColor(color, (color) | 0x80)

/* Darkens the specified color. */
#define darkenColor(color)			((unsigned char)((color) & 0x77))
/* Darkens the foreground color of the specified color. */
#define darkenFColor(color)			setFColor(color, (color) & 0x07)
/* Darkens the background color of the specified color. */
#define darkenBColor(color)			setBColor(color, (color) & 0x70)

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
