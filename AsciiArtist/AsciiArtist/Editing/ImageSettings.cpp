/*=================================================================|
|* File:				ImageSettings.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ImageSettings.h"

using namespace AsciiArtist::Editing;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

/* Constructs the ascii image settings. */
ImageSettings::ImageSettings()
 :	format(ImageFormats::Basic),
	background(),
	fileVersion(AsciiImageInfo::FILE_VERSION),
	smallFile(false) {}
/* Constructs the ascii image settings. */
ImageSettings::ImageSettings(ImageFormats format, Pixel background, unsigned short fileVersion, bool smallFile)
 :	format(format),
	background(background),
	fileVersion(fileVersion),
	smallFile(smallFile) {}

#pragma endregion
//=================================================================|
