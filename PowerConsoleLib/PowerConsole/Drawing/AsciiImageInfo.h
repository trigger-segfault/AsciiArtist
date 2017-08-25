/*=================================================================|
|* File:				AsciiImageInfo.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_IMAGE_INFO_H
#define ASCII_IMAGE_INFO_H

#include <string>
#include <memory>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/IO/Clipboard.h>
#include <PowerConsole/IO/IOStream.h>
#include <PowerConsole/Drawing/AsciiImageHeaders.h>

namespace PowerConsole::Drawing {
DECLARE_CLASS_AND_SMART_PTRS(Graphics);
DECLARE_CLASS_AND_SMART_PTRS(AsciiImageFrame);
DECLARE_CLASS_AND_SMART_PTRS(AsciiImage);
DECLARE_CLASS_AND_SMART_PTRS(AsciiAnimation);
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A static class for managing ascii image information. */
class AsciiImageInfo {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The current file version used for normal ascii images. */
	static const unsigned short FILE_VERSION;
	/* The current file version used for lightweight ascii images. */
	static const unsigned short SMALL_FILE_VERSION;
	/* The signature used for normal ascii images. */
	static const std::string FILE_SIGNATURE;
	/* The signature used for lightweight ascii images. */
	static const std::string SMALL_FILE_SIGNATURE;
	/* The maximum size recomended for an ascii image. */
	static const PcGeo::Point2I MAX_SIZE;
	/* The maximum number of frames recomended for an ascii image. */
	static const unsigned int MAX_FRAMES;
	/* The name of the clipboard format used for ascii images. */
	static const std::string CLIPBOARD_FORMAT_NAME;
	/* The id of the clipboard format used for ascii images. */
	static const PcIO::ClipboardFormat CLIPBOARD_FORMAT;

	#pragma endregion
	//========= INPUT/OUTPUT =========
	#pragma region Input/Output

	/* Creates the header of the specified ascii image. */
	static AsciiImageHeader createHeader(const AsciiImage& image, unsigned short version = FILE_VERSION, bool smallFile = false);
	/* Creates the header of the specified ascii animation. */
	static AsciiImageHeader createHeader(const AsciiAnimation& image, unsigned short version = FILE_VERSION, bool smallFile = false);

	/* Returns true if the specified file is a valid ascii image. */
	static bool isFileAsciiImage(const std::string& path);

	/* Loads the specified pixel from the file stream. */
	static bool loadPixel(PcIO::InputStream* in, const AsciiImageHeader& header, Pixel& pixel);
	/* Saves the specified pixel to the file stream. */
	static bool savePixel(PcIO::OutputStream* out, const AsciiImageHeader& header, Pixel& pixel);

	/* Loads the ascii image header from the file stream. */
	static bool loadHeader(PcIO::InputStream* in, AsciiImageHeader& header, bool loadFrameHeaders);
	/* Loads the ascii image frame header from the file stream. */
	static bool loadFrameHeader(PcIO::InputStream* in, AsciiImageHeader& header, AsciiImageFrameHeader& frame);
	/* Saves the ascii image header to the file stream. */
	static bool saveHeader(PcIO::OutputStream* out, AsciiImageHeader& header, bool saveFrameHeaders);
	/* Saves the ascii image frame header to the file stream. */
	static bool saveFrameHeader(PcIO::OutputStream* out, AsciiImageHeader& header, AsciiImageFrameHeader& frame);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
