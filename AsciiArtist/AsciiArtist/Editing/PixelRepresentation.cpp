/*=================================================================|
|* File:				PixelRepresentation.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PixelRepresentation.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Helpers/Flags.h>

using namespace AsciiArtist::Editing;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========== CONSTANTS ===========
#pragma region Constants

const Pixel PixelRepresentation::TRANSPARENT_CHAR	= Pixel(Characters::GRADIENT_HEAVY, toColor(BLACK, DARK_CYAN));
const Pixel PixelRepresentation::TRANSPARENT_COLOR	= Pixel(Characters::GRADIENT_HEAVY, toColor(BLACK, WHITE));
const Pixel PixelRepresentation::INVERT_COLOR		= Pixel(Characters::GRADIENT_HEAVY, toColor(DARK_GRAY, BLACK));
const Pixel PixelRepresentation::LIGHTEN_COLOR		= Pixel(Characters::GRADIENT_HEAVY, toColor(BLACK, CYAN));
const Pixel PixelRepresentation::DARKEN_COLOR		= Pixel(Characters::GRADIENT_HEAVY, toColor(DARK_CYAN, BLACK));
const Pixel PixelRepresentation::UNKNOWN_COLOR		= Pixel('?', toColor(GRAY, BLACK));

#pragma endregion
//========== FUNCTIONS ===========
#pragma region Functions

Pixel PixelRepresentation::changePixelFromPart(Pixel inPixel, Pixel partPixel, PixelParts part) {
	if (part == PixelParts::Character) {
		if (partPixel.matches(TRANSPARENT_CHAR)) {
			inPixel.character = ' ';
			setFlag(inPixel.attributes, PixelAttributes::Char, false);
		}
		else if (partPixel.color == toColor(BLACK, GRAY)) {
			inPixel.character = partPixel.character;
			setFlag(inPixel.attributes, PixelAttributes::Char, true);
		}
	}
	else {
		bool background = (part == PixelParts::Background);
		inPixel.attributes &= ~(!background ? PixelAttributes::ForegroundAll : PixelAttributes::BackgroundAll);

		if (partPixel.matches(TRANSPARENT_COLOR))
			setFlag(inPixel.attributes, (!background ? PixelAttributes::FColor : PixelAttributes::BColor), false);
		else if (partPixel.matches(INVERT_COLOR))
			setFlag(inPixel.attributes, (!background ? PixelAttributes::FInvert : PixelAttributes::BInvert), true);
		else if (partPixel.matches(LIGHTEN_COLOR))
			setFlag(inPixel.attributes, (!background ? PixelAttributes::FLighten : PixelAttributes::BLighten), true);
		else if (partPixel.matches(DARKEN_COLOR))
			setFlag(inPixel.attributes, (!background ? PixelAttributes::FDarken : PixelAttributes::BDarken), true);
		else {
			setFlag(inPixel.attributes, (!background ? PixelAttributes::FColor : PixelAttributes::BColor), true);
			inPixel.color = (!background ?
				setFColor(inPixel.color, getFColor(partPixel.color)) :
				setBColor(inPixel.color, toBColor(getFColor(partPixel.color)))
			);
		}
	}
	return inPixel;
}

Pixel PixelRepresentation::getPixelPart(Pixel inPixel, PixelParts part) {
	if (part == PixelParts::Character) {
		if (!getFlag(inPixel.attributes, PixelAttributes::Char))
			return TRANSPARENT_CHAR;
		else
			return Pixel(inPixel.character, toColor(BLACK, GRAY));
	}
	else {
		bool background = (part == PixelParts::Background);
		PixelAttributes attr = (inPixel.attributes & (!background ? PixelAttributes::ForegroundAll : PixelAttributes::BackgroundAll));

		if (attr == (!background ? PixelAttributes::FColor : PixelAttributes::BColor))
			return Pixel((!background ? Characters::BLOCK_FULL : ' '), inPixel.color);
		else if (attr == PixelAttributes::None)
			return TRANSPARENT_COLOR;
		else if (attr == (!background ? PixelAttributes::FInvert : PixelAttributes::BInvert))
			return INVERT_COLOR;
		else if (attr == (!background ? PixelAttributes::FLighten : PixelAttributes::BLighten))
			return LIGHTEN_COLOR;
		else if (attr == (!background ? PixelAttributes::FDarken : PixelAttributes::BDarken))
			return DARKEN_COLOR;
		else
			return UNKNOWN_COLOR;
	}
}

#pragma endregion
//=================================================================|
