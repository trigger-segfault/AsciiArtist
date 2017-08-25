/*=================================================================|
|* File:				ImageSettings.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef IMAGE_SETTINGS_H
#define IMAGE_SETTINGS_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>

using namespace PowerConsole::Drawing;

namespace AsciiArtist::Editing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A structure for storing settings about an ascii image. */
struct ImageSettings {

	//=========== MEMBERS ============
	#pragma region Members

	// Format
	/* The format of the image. */
	ImageFormats format;

	// Background
	/* The background of the image. */
	Pixel background;

	// File Version
	/* The file version of the ascii image. */
	unsigned short fileVersion;
	/* The small file setting of the ascii image. */
	bool smallFile;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the image settings. */
	ImageSettings();
	/* Constructs the image settings. */
	ImageSettings(ImageFormats format, Pixel background, unsigned short fileVersion, bool smallFile);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
