/*=================================================================|
|* File:				AsciiImageFileDetails.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_IMAGE_FILE_DETAILS_H
#define ASCII_IMAGE_FILE_DETAILS_H

#include <string>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

struct AsciiImageFileDetails {
	//=========== MEMBERS ============
	#pragma region Members

	/* The size of the ascii image. */
	PcGeo::Point2I size;
	/* The number of frames in the ascii image. */
	unsigned int frameCount;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the ascii image file details */
	AsciiImageFileDetails();

	#pragma endregion
	//=========== LOADING ============
	#pragma region Loading

	/* Loads the specified ascii image file details. */
	bool load(const std::string& path);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
