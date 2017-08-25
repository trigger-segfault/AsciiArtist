/*=================================================================|
|* File:				ConsoleEnums.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ConsoleEnums.h"

using namespace PowerConsole;
using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/

bool Pc::isValidFontSize(Point2I size) {
	return isValidRasterFontSize(toRasterSize(size.x, size.y));
}
bool Pc::isValidRasterFontSize(RasterFontSizes rasterSize) {
	switch (rasterSize) {
	case RasterFontSizes::R4x6:
	case RasterFontSizes::R6x8:
	case RasterFontSizes::R8x8:
	case RasterFontSizes::R16x8:
	case RasterFontSizes::R5x12:
	case RasterFontSizes::R7x12:
	case RasterFontSizes::R8x12:
	case RasterFontSizes::R16x12:
	case RasterFontSizes::R12x16:
	case RasterFontSizes::R10x18:
		return true;
	default:
		return false;
	}
}

//=================================================================|
