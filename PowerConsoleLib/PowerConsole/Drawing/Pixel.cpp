/*=================================================================|
|* File:				Pixel.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Pixel.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Drawing;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

Pixel::Pixel()
	: character(' '), color(0x07), attributes(PixelAttributes::Fill) {}
Pixel::Pixel(unsigned char character, unsigned char color)
	: character(character), color(color), attributes(PixelAttributes::Fill) {}
Pixel::Pixel(unsigned char character, unsigned char color, PixelAttributes attributes)
	: character(character), color(color), attributes(attributes) {}

#pragma endregion
//========= COMPARISONS ==========
#pragma region Comparisons

bool Pixel::matches(Pixel pixel, bool specific) {
	if (attributes == pixel.attributes) {
		if (character == pixel.character && color == pixel.color)
			return true;
		if (!specific) {
			// Check if the characters are matching because both foreground and
			// background colors are the same
			bool fullChar1 = (getFColor(color) == getBColor(color) ||
				character == 0 || character == 32 || character == 255 || character == 219);
			char fullColor1 = (character == 219 ? getFColor(color) : getBColor(color));
			bool fullChar2 = (getFColor(pixel.color) == getBColor(pixel.color) ||
				pixel.character == 0 || pixel.character == 32 || pixel.character == 255 || pixel.character == 219);
			char fullColor2 = (pixel.character == 219 ? getFColor(pixel.color) : getBColor(pixel.color));
			if (fullChar1 && fullChar2 && fullColor1 == fullColor2)
				return true;

			// Check if the characters are matching because they are inverse characters
			bool inverse = (color == swapColor(pixel.color));
			if (inverse) {
				if ((character == 7 && pixel.character == 8) ||
					(character == 8 && pixel.character == 7))
					return true;
				if ((character == 9 && pixel.character == 10) ||
					(character == 10 && pixel.character == 9))
					return true;
				if ((character == 220 && pixel.character == 223) ||
					(character == 223 && pixel.character == 220))
					return true;
				if ((character == 221 && pixel.character == 222) ||
					(character == 222 && pixel.character == 221))
					return true;
			}
		}
	}
	return false;
}

#pragma endregion

//=================================================================|
