/*=================================================================|
|* File:				AsciiEnums.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_ENUMS_H
#define ASCII_ENUMS_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole::Drawing {
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/

/* The types of attributes for pixels. */
enum class PixelAttributes : unsigned short {
	/* No attributes. */
	None = 0x0000,
	/* All attributes. */
	All = 0x0FF1,
	/* All character attributes. */
	//CharAll = 0x0001,
	/* All foreground and background attributes. */
	ColorAll = 0x0FF0,
	/* All foreground attributes. */
	ForegroundAll = 0x0550,
	/* All background attributes. */
	BackgroundAll = 0x0AA0,

	/* Character is not transparent. */
	Char = 0x0001,
	/* Foreground color is not transparent. */
	FColor = 0x0010,
	/* Background color is not transparent. */
	BColor = 0x0020,
	/* Color is not transparent. */
	Color = 0x0030,
	/* Character and color are not transparent. */
	Fill = 0x0031,

	/* Foreground color is inverted. */
	FInvert = 0x0040,
	/* Background color is inverted. */
	BInvert = 0x0080,
	/* Color is inverted. */
	Invert = 0x00C0,

	/* Foreground color is lightened. */
	FLighten = 0x0100,
	/* Background color is lightened. */
	BLighten = 0x0200,
	/* Color is lightened. */
	Lighten = 0x0300,

	/* Foreground color is darkened. */
	FDarken = 0x0400,
	/* Background color is darkened. */
	BDarken = 0x0800,
	/* Color is darkened. */
	Darken = 0x0C00
};
/* The types of options available when drawing with ascii images. */
enum class PixelOptions : unsigned short {
	/* No options. */
	None = 0x0000,
	/* All options. */
	All = 0x003F,

	/* Attributes and characters/colors will be applied whether or not they are true. */
	Replace = 0x0001,
	/* Attributes and characters/colors will be applied when they are false. */
	Opposite = 0x0002,
	/* Attributes will be removed instead of applied. */
	Inverse = 0x0004,
	/* Special attributes are copied instead of applied. */
	CopySpecial = 0x0008,
	/* Characters, colors, and specials will not be applied. */
	NoPixel = 0x0010,
	/* Attributes will not be applied. */
	NoAttributes = 0x0020
};
/* The types of formats available for ascii images. */
enum class ImageFormats : unsigned short {
	/* The image has no special formatting. */
	Basic = 0x0000,
	/* All formatting options are enabled. */
	All = 0x0071,

	/* Attributes are used in the image. */
	Attributes = 0x0001,
	/* The image is an animation and has multiple frames, each with speeds. */
	Animation = 0x0010,
	/* The image can have different sizes for each frame in the image. */
	Multisize = 0x0020,
	/* The image can have layers for each frame. */
	Layered = 0x0040
};

DEFINE_ENUM_FLAG_OPERATORS(PixelAttributes)
DEFINE_ENUM_FLAG_OPERATORS(PixelOptions)
DEFINE_ENUM_FLAG_OPERATORS(ImageFormats)

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
