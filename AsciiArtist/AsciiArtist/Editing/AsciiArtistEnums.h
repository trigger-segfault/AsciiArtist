/*=================================================================|
|* File:				AsciiArtistEnums.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_ARTIST_ENUMS_H
#define ASCII_ARTIST_ENUMS_H

#include <PowerConsole/Declarations.h>

namespace AsciiArtist::Editing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The different types of tools. */
enum class ToolTypes : short {
	None = -1,
	Pen = 0,
	Line,
	Square,
	Circle,
	Flood,
	//Replace,
	Text,
	Select,
	Borders,
	Eyedropper
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
