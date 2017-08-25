/*=================================================================|
|* File:				Pixel.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PIXEL_H
#define PIXEL_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiEnums.h>

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A basic class to store data about an ascii pixel. */
struct Pixel {

	//=========== MEMBERS ============
	#pragma region Members

	/* The character of the pixel. */
	unsigned char character;
	/* The color of the pixel. */
	unsigned char color;
	/* The attributes of the pixel. */
	PixelAttributes attributes;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default pixel. */
	Pixel();
	/* Constructs a pixel with the specified character and color. */
	Pixel(unsigned char character, unsigned char color);
	/* Constructs a pixel with the specified character, color, and attributes. */
	Pixel(unsigned char character, unsigned char color, PixelAttributes attributes);

	#pragma endregion
	//========= COMPARISONS ==========
	#pragma region Comparisons

	/* Returns true if the pixel matches the specified pixel. */
	bool matches(Pixel pixel, bool specific = true);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
