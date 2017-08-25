/*=================================================================|
|* File:				PixelRepresentation.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PIXEL_REPRESENTATION_H
#define PIXEL_REPRESENTATION_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/Pixel.h>

using namespace PowerConsole::Drawing;

namespace AsciiArtist::Editing {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum class PixelParts {
	Character,
	Foreground,
	Background
};

class PixelRepresentation {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	static const Pixel TRANSPARENT_CHAR;
	static const Pixel TRANSPARENT_COLOR;
	static const Pixel INVERT_COLOR;
	static const Pixel LIGHTEN_COLOR;
	static const Pixel DARKEN_COLOR;
	static const Pixel UNKNOWN_COLOR;

	#pragma endregion
	//========== FUNCTIONS ===========
	#pragma region Functions

	/* Changes the pixel based on the input pixel part. */
	static Pixel changePixelFromPart(Pixel inPixel, Pixel partPixel, PixelParts part);
	/* Gets the pixel that represents the specific part of the input pixel. */
	static Pixel getPixelPart(Pixel inPixel, PixelParts part);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
