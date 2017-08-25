/*=================================================================|
|* File:				AsciiImageFrame.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_IMAGE_FRAME_H
#define ASCII_IMAGE_FRAME_H

#include <string>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Rectangle.h>
#include <PowerConsole/Geometry/GeometryEnums.h>
#include <PowerConsole/Drawing/AsciiImageInfo.h>

#define xyToIndex(x, y, width) ((x) + ((y) * (width)))
#define pointToIndex(p, width) ((p).x + ((p).y * (width)))
#define indexToX(x, width) ((x) % (width))
#define indexToY(y, width) ((y) / (width))

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The base class for an image composed of characters, colors, and attributes. */
class AsciiImageFrame {

	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The size of the ascii image. */
	PcGeo::Point2I frameSize;
	/* The background fill of the ascii image. */
	Pixel background;
	/* The format of the ascii image. */
	ImageFormats format;
	/* The frame speed of the ascii image. */
	unsigned int frameSpeed;
	/* The name of the frame. */
	std::string name;
	/* The 1D array of pixels in the ascii image. */
	Pixel* pixels;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
protected:
	/* Constructs the default ascii image. */
	AsciiImageFrame();
	/* Constructs an ascii image with the specified size. */
	AsciiImageFrame(PcGeo::Point2I size, ImageFormats format);
	/* Constructs an ascii image with the specified size and background. */
	AsciiImageFrame(PcGeo::Point2I size, ImageFormats format, Pixel background);
	/* Constructs a copy of the specified ascii image with the new format. */
	AsciiImageFrame(ImageFormats format, Pixel background, const AsciiImageFrame& image);
	/* Constructs a moved copy of the specified ascii image with the new format. */
	AsciiImageFrame(ImageFormats format, Pixel background, AsciiImageFrame&& image);
	/* Constructs a copy of the region of the specified ascii image with the new format. */
	AsciiImageFrame(PcGeo::Rectangle2I region, ImageFormats format, Pixel background, const AsciiImageFrame& image);
	/* Constructs a copy of the specified ascii image. */
	AsciiImageFrame(const AsciiImageFrame& image);
public:
	/* Constructs a moved copy of the specified ascii image. */
	AsciiImageFrame(AsciiImageFrame&& image);
	/* Deconstructs the ascii image. */
	~AsciiImageFrame();

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators
protected:
	/* The copy assignment operator. */
	AsciiImageFrame& operator=(const AsciiImageFrame& image);
	/* The move assignment operator. */
	AsciiImageFrame& operator=(AsciiImageFrame&& image);

	#pragma endregion
	//========== PROTECTED ===========
	#pragma region Protected

	/* Cleans up the allocated data in the ascii image. */
	void cleanUp();
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
	/* Applies the format to the ascii image to make sure the formatting doesn't conflict. */
	void setFormat(ImageFormats format);
public:
	/* Loads the ascii image from the input stream. */
	bool load(PcIO::InputStream* in, const AsciiImageHeader& header, const AsciiImageFrameHeader& frame, bool isLayer = false);
	/* Saves the ascii image to the output stream. */
	bool save(PcIO::OutputStream* out, const AsciiImageHeader& header, const AsciiImageFrameHeader& frame, bool isLayer = false) const;

	#pragma endregion
	//============ PIXELS ============
	#pragma region Pixels
public:
	/* Gets the character at the specified point in the image. */
	unsigned char getChar(PcGeo::Point2I point) const;
	/* Gets the color at the specified point in the image. */
	unsigned char getColor(PcGeo::Point2I point) const;
	/* Gets the attributes at the specified point in the image. */
	PixelAttributes getAttributes(PcGeo::Point2I point) const;
	/* Gets the pixel at the specified point in the image. */
	Pixel getPixel(PcGeo::Point2I point) const;
	/* Tests the attributes at the specified point in the image. */
	bool testAttributes(PcGeo::Point2I point, PixelAttributes attributes, bool all) const;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the size of the image. */
	PcGeo::Point2I size() const;
	/* Gets the background fill used. */
	Pixel getBackground() const;
	/* Gets the format of the image. */
	ImageFormats getFormat() const;
	/* Gets the frame speed of the image. */
	unsigned int getFrameSpeed() const;
	/* Gets the name of the frame. */
	std::string getName() const;
	/* Gets the array of pixels in the ascii image, the data is formatted as [x][y]. */
	Pixel*const getPixels() const;

	#pragma endregion
	//=========== FRIENDS ============
	#pragma region Friends

	friend AsciiImageInfo;
	friend AsciiAnimation;
	friend std::vector<AsciiImageFrame>;
	friend Graphics;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
