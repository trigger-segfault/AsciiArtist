/*=================================================================|
|* File:				AsciiImageHeaders.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_IMAGE_HEADERS_H
#define ASCII_IMAGE_HEADERS_H

#include <string>
#include <vector>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/Drawing/Pixel.h>

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The frame header for the ascii image file structure. */
struct AsciiImageFrameHeader {

	//=========== MEMBERS ============
	#pragma region Members

	// File Information
	/* The starting offset of the frame. */
	unsigned int frameOffset;
	/* The size of each frame layer. */
	unsigned int frameSize;

	// Frame Information
	/* The name of the frame. */
	std::string name;
	/* The dimensions of the frame. */
	PcGeo::Point2I size;
	/* The animation speed of the frame. */
	unsigned int frameSpeed;
	/* The number of layers in the frame. */
	unsigned short layers;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default ascii image frame header. */
	AsciiImageFrameHeader();

	#pragma endregion
};

/* The file header for the ascii image file structure. */
struct AsciiImageHeader {

	//=========== MEMBERS ============
	#pragma region Members

	// File Information
	/* The signature of the file. */
	std::string signature;
	/* The version of the file. */
	unsigned short version;
	/* The size of the file. */
	unsigned int fileSize;
	/* The starting position of the file. */
	unsigned long long fileStart;

	// Header Information
	/* The size of the header. */
	unsigned int headerSize;
	/* The offset from the begginning of this file to the header. */
	unsigned int headerOffset;
	/* The size of the frame header. */
	unsigned int frameHeaderSize;
	/* The offset from the begginning of this file to the image array. */
	unsigned int frameArrayOffset;
	/* The size of a single pixel array of a frame. */
	unsigned int frameSize;

	// Data Sizes
	/* The number of bytes for each attribute. */
	unsigned short bytesPerAttribute;
	/* The length of the longest name for the frame headers. */
	unsigned short bytesPerName;

	// Image Information
	/* The format of the image. */
	ImageFormats format;
	/* The number of frames in the image. */
	unsigned short frameCount;
	/* The dimensions of the image. */
	PcGeo::Point2I size;
	/* The background pixel of the image. */
	Pixel background;
	/* The animation speed of the image. */
	unsigned int animationSpeed;

	/* The list of frames in the image. */
	std::vector<AsciiImageFrameHeader> frameList;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default ascii image header. */
	AsciiImageHeader();

	#pragma endregion
};
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
