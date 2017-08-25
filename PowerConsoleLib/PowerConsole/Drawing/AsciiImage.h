/*=================================================================|
|* File:				AsciiImage.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_IMAGE_H
#define ASCII_IMAGE_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiImageFrame.h>

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A class to manage and contain a fully controllable ascii image. */
class AsciiImage : public AsciiImageFrame {

	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default ascii image. */
	AsciiImage();
	/* Constructs an ascii image with the specified size. */
	AsciiImage(PcGeo::Point2I size, ImageFormats format);
	/* Constructs an ascii image with the specified size and background. */
	AsciiImage(PcGeo::Point2I size, ImageFormats format, Pixel background);
	/* Constructs a copy of the specified ascii image with the new format. */
	AsciiImage(ImageFormats format, Pixel background, const AsciiImageFrame& image);
	/* Constructs a moved copy of the specified ascii image with the new format. */
	AsciiImage(ImageFormats format, Pixel background, AsciiImageFrame&& image);
	/* Constructs a copy of part of the specified ascii image with the new format. */
	AsciiImage(PcGeo::Rectangle2I region, ImageFormats format, Pixel background, const AsciiImageFrame& image);
	/* Constructs a copy of the specified ascii image. */
	AsciiImage(const AsciiImageFrame& image);
	/* Constructs a moved copy of the specified ascii image. */
	AsciiImage(AsciiImageFrame&& image);
	/* Constructs a copy of the specified ascii image. */
	AsciiImage(const AsciiImage& image);
	/* Constructs a moved copy of the specified ascii image. */
	AsciiImage(AsciiImage&& image);
	/* Deconstructs the ascii image. */
	~AsciiImage();

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* The copy assignment operator. */
	AsciiImage& operator=(const AsciiImageFrame& image);
	/* The move assignment operator. */
	AsciiImage& operator=(AsciiImageFrame&& image);
	/* The copy assignment operator. */
	AsciiImage& operator=(const AsciiImage& image);
	/* The move assignment operator. */
	AsciiImage& operator=(AsciiImage&& image);

	#pragma endregion
	//========= INPUT/OUTPUT =========
	#pragma region Input/Output

	// Input
	/* Loads the ascii image from the input stream. */
	bool load(PcIO::InputStream* in);
	/* Loads the ascii image from file. */
	bool loadFile(const std::string& path);
	/* Loads the ascii image from the resources. */
	bool loadResource(const std::string& name, const std::string& type = "ASCII_IMAGE");
	/* Loads the ascii image from the resources. */
	bool loadResource(const std::string& name, unsigned int typeId);
	/* Loads the ascii image from the resources. */
	bool loadResource(unsigned int id, const std::string& type = "ASCII_IMAGE");
	/* Loads the ascii image from the resources. */
	bool loadResource(unsigned int id, unsigned int typeId);
	/* Loads the ascii image from the clipboard. */
	bool loadClipboard();

	// Output
	/* Saves the ascii image to the output stream. */
	bool save(PcIO::OutputStream* out, unsigned short version = AsciiImageInfo::FILE_VERSION, bool smallFile = false) const;
	/* Saves the ascii image to file. */
	bool saveFile(const std::string& path, unsigned short version = AsciiImageInfo::FILE_VERSION, bool smallFile = false) const;
	/* Saves the ascii image to the clipboard. */
	bool saveClipboard(unsigned short version = AsciiImageInfo::FILE_VERSION, bool smallFile = false) const;

	#pragma endregion
	//========= MODIFICATION =========
	#pragma region Modification

	/* Resizes the ascii image. */
	void resize(PcGeo::Point2I size);
	/* Rotates the ascii image by the specified number of 90 degree rotations. */
	void rotate(int rotations);
	/* Rotates the image and resizes to compensate for non-square dimensions. */
	void rotateAndResize(int rotations);
	/* Translates the ascii image by the specified distance. */
	void translate(PcGeo::Point2I distance);
	/* Flips the ascii image either horizontally or vertically. */
	void flip(PcGeo::Orientations axis);
	/* Clears the image. */
	void clear();
	/* Creates a graphics object for this image. */
	GraphicsSPtr createGraphics();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Sets the background fill used. */
	void setBackground(Pixel background);
	/* Sets the format of the image. */
	void setFormat(ImageFormats format);
	/* Sets the frame speed of the image. */
	void setFrameSpeed(unsigned int frameSpeed);
	/* Sets the name of the frame. */
	void setName(const std::string& name);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
