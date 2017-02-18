//================================================================
// ASCII IMAGE HEADER:
//================================================================

#pragma once
#ifndef ASCII_IMAGE_H
#define ASCII_IMAGE_H

#include "Stdafx.h"
#include "IOStream.h"
#include "Geometry.h"

using namespace std;
using namespace PowerConsole::IOStream;
using namespace PowerConsole::Geometry;

//================================================================
// NAMESPACE:
//================================================================
/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of drawing and ascii graphics classes. */
namespace Drawing {
//================================================================
// PREDEFINITIONS:
//================================================================
class AsciiImageBase;
class AsciiImage;
class AsciiAnimation;
//================================================================
// ENUMERATIONS:
//================================================================
/* The types of attributes for pixels. */
enum PixelAttributes : unsigned short {
	/* No attributes. */
	AttributeNone		= 0x0000,
	/* All attributes. */
	AttributeAll		= 0x0FF1,
	/* All foreground attributes. */
	AttributeFAll		= 0x0550,
	/* All background attributes. */
	AttributeBAll		= 0x0AA0,
	
	/* Character is not transparent. */
	AttributeChar		= 0x0001,
	/* Foreground color is not transparent. */
	AttributeFColor		= 0x0010,
	/* Background color is not transparent. */
	AttributeBColor		= 0x0020,
	/* Color is not transparent. */
	AttributeColor		= 0x0030,
	/* Character and color are not transparent. */
	AttributeFill		= 0x0031,
	
	/* Foreground color is inverted. */
	AttributeFInvert	= 0x0040,
	/* Background color is inverted. */
	AttributeBInvert	= 0x0080,
	/* Color is inverted. */
	AttributeInvert		= 0x00C0,

	/* Foreground color is lightened. */
	AttributeFLighten	= 0x0100,
	/* Background color is lightened. */
	AttributeBLighten	= 0x0200,
	/* Color is lightened. */
	AttributeLighten	= 0x0300,

	/* Foreground color is darkened. */
	AttributeFDarken	= 0x0400,
	/* Background color is darkened. */
	AttributeBDarken	= 0x0800,
	/* Color is darkened. */
	AttributeDarken		= 0x0C00
};
/* The types of options available when drawing with ascii images. */
enum PixelOptions : unsigned short {
	/* No options. */
	OptionNone			= 0x0000,
	/* All options. */
	OptionAll			= 0x003F,

	/* Attributes and characters/colors will be applied whether or not they are true. */
	OptionReplace		= 0x0001,
	/* Attributes and characters/colors will be applied when they are false. */
	OptionOpposite		= 0x0002,
	/* Attributes will be removed instead of applied. */
	OptionInverse		= 0x0004,
	/* Special attributes are copied instead of applied. */
	OptionCopySpecial	= 0x0008,
	/* Characters, colors, and specials will not be applied. */
	OptionNoPixel		= 0x0010,
	/* Attributes will not be applied. */
	OptionNoAttributes	= 0x0020
};
/* The types of formats available for ascii images. */
enum ImageFormats : unsigned short {
	/* The image has no special formatting. */
	FormatBasic			= 0x0000,
	/* All formatting options are enabled. */
	FormatAll			= 0x0071,

	/* Attributes are used in the image. */
	FormatAttributes	= 0x0001,
	/* The image is an animation and has multiple frames, each with speeds. */
	FormatAnimation		= 0x0010,
	/* The image can have different sizes for each frame in the image. */
	FormatMultiSize		= 0x0020,
	/* The image can have layers for each frame. */
	FormatLayered		= 0x0040
};
/* The alignment options for drawing strings and images. */
enum class Alignment : short {
	Left	= 0,
	Top		= 0,
	Center	= 1,
	Right	= 2,
	Bottom	= 2
};
//================================================================
// STRUCTURES:
//================================================================
/* A basic class to store data about an ascii pixel. */
struct Pixel {

	//=========== MEMBERS ============

	/* The character of the pixel. */
	unsigned char Char;
	/* The color of the pixel. */
	unsigned char Color;
	/* The attributes of the pixel. */
	unsigned short Attributes;
	
	//========= CONSTRUCTORS =========

	/* Constructs the default pixel. */
	Pixel();
	/* Constructs a pixel with the specified character and color. */
	Pixel(unsigned char character, unsigned char color);
	/* Constructs a pixel with the specified character, color, and attributes. */
	Pixel(unsigned char character, unsigned char color, unsigned short attributes);

	//========= INFORMATION ==========

	/* Returns true if the pixel matches the specified pixel. */
	bool Matches(Pixel pixel, bool specific = true);
	
	/* Sets the foreground color of the pixel. */
	//void SetFColor(unsigned char color);
	/* Sets the background color of the pixel. */
	//void SetBColor(unsigned char color);
	/* Sets the foreground attributes of the pixel. */
	//void SetFAttributes(unsigned char color);
	/* Sets the background attributes of the pixel. */
	//void SetBAttributes(unsigned char color);
};
inline bool operator==(Pixel a, Pixel b) {
	return	a.Char == b.Char &&
			a.Color == b.Color &&
			a.Attributes == b.Attributes;
}
inline bool operator!=(Pixel a, Pixel b) {
	return	a.Char != b.Char ||
			a.Color != b.Color ||
			a.Attributes != b.Attributes;
}
/* The frame header for the ascii image file structure. */
struct AsciiImageFrameHeader {
	
	//=========== MEMBERS ============
	
	// File Information
	/* The starting offset of the frame. */
	unsigned int FrameOffset;

	// Frame Information
	/* The name of the frame. */
	string Name;
	/* The dimensions of the frame. */
	Coord Size;
	/* The animation speed of the frame. */
	unsigned int FrameSpeed;
	/* The number of layers in the frame. */
	unsigned short Layers;
	
	//========= CONSTRUCTORS =========

	/* Constructs the default ascii image frame header. */
	AsciiImageFrameHeader();
};
/* The file header for the ascii image file structure. */
struct AsciiImageHeader {
	
	//=========== MEMBERS ============

	// File Information
	/* The signature of the file. */
	string Signature;
	/* The version of the file. */
	unsigned short Version;
	/* The size of the file. */
	unsigned int FileSize;
	/* The starting position of the file. */
	unsigned long long FileStart;
	
	// Header Information
	/* The size of the header. */
	unsigned int HeaderSize;
	/* The offset from the begginning of this file to the header. */
	unsigned int HeaderOffset;
	/* The size of the frame header. */
	unsigned int FrameHeaderSize;
	/* The offset from the begginning of this file to the image array. */
	unsigned int FrameArrayOffset;
	/* The size of a single pixel array of a frame. */
	unsigned int FrameSize;

	// Data Sizes
	/* The number of bytes for each attribute. */
	unsigned short BytesPerAttribute;
	/* The length of the longest name for the frame headers. */
	unsigned short BytesPerName;

	// Image Information
	/* The format of the image. */
	unsigned short Format;
	/* The number of frames in the image. */
	unsigned short Frames;
	/* The dimensions of the image. */
	Coord Size;
	/* The background pixel of the image. */
	Pixel Background;
	/* The animation speed of the image. */
	unsigned int AnimationSpeed;

	/* The list of frames in the image. */
	vector<AsciiImageFrameHeader> FrameList;
	
	//========= CONSTRUCTORS =========

	/* Constructs the default ascii image header. */
	AsciiImageHeader();
};
//================================================================
// CLASSES:
//================================================================
/* A static class for managing ascii image information. */
class AsciiImageInfo {
	
	//========== CONSTANTS ===========
public:
	/* The current file version used for normal ascii images. */
	const static unsigned short FileVersion;
	/* The current file version used for lightweight ascii images. */
	const static unsigned short SmallFileVersion;
	/* The signature used for normal ascii images. */
	const static string FileSignature;
	/* The signature used for lightweight ascii images. */
	const static string SmallFileSignature;
	/* The maximum size recomended for an ascii image. */
	const static Coord MaxSize;
	/* The maximum number of frames recomended for an ascii image. */
	const static unsigned int MaxFrames;
	/* The name of the clipboard format used for ascii images. */
	const static string ClipboardFormatName;
	/* The id of the clipboard format used for ascii images. */
	const static unsigned int ClipboardFormat;

	//========== MANAGEMENT ==========
	
	/* Creates the header of the specified ascii image. */
	static AsciiImageHeader CreateHeader(const AsciiImage& image, unsigned short version = 3, bool smallFile = false);
	/* Creates the header of the specified ascii animation. */
	static AsciiImageHeader CreateHeader(const AsciiAnimation& image, unsigned short version = 3, bool smallFile = false);

	/* Returns true if the specified file is a valid ascii image. */
	static bool IsFileAsciiImage(const string& path);

	/* Loads the specified pixel from the file stream. */
	static bool LoadPixel(InputStream* in, const AsciiImageHeader& header, Pixel& pixel);
	/* Saves the specified pixel to the file stream. */
	static bool SavePixel(OutputStream* out, const AsciiImageHeader& header, Pixel& pixel);

	/* Loads the ascii image header from the file stream. */
	static bool LoadHeader(InputStream* in, AsciiImageHeader& header, bool loadFrameHeaders);
	/* Loads the ascii image frame header from the file stream. */
	static bool LoadFrameHeader(InputStream* in, AsciiImageHeader& header, AsciiImageFrameHeader& frame);
	/* Saves the ascii image header to the file stream. */
	static bool SaveHeader(OutputStream* out, AsciiImageHeader& header, bool saveFrameHeaders);
	/* Saves the ascii image frame header to the file stream. */
	static bool SaveFrameHeader(OutputStream* out, AsciiImageHeader& header, AsciiImageFrameHeader& frame);
};
/* The base class for an image composed of characters, colors, and attributes. */
class AsciiImageBase {

	//=========== MEMBERS ============
protected:
	/* The size of the ascii image. */
	Coord _Size;
	/* The background fill of the ascii image. */
	Pixel _Background;
	/* The format of the ascii image. */
	unsigned short _Format;
	/* The frame speed of the ascii image. */
	unsigned int _FrameSpeed;
	/* The 2D array of pixels in the ascii image. */
	Pixel** _Pixels;

	//========= CONSTRUCTORS =========
protected:
	/* Constructs the default ascii image. */
	AsciiImageBase();
	/* Constructs an ascii image with the specified size. */
	AsciiImageBase(Coord size, unsigned short format);
	/* Constructs an ascii image with the specified size and background. */
	AsciiImageBase(Coord size, unsigned short format, Pixel background);
	/* Constructs a copy of the specified ascii image with the new format. */
	AsciiImageBase(unsigned short format, Pixel background, const AsciiImageBase& image);
	/* Constructs a moved copy of the specified ascii image with the new format. */
	AsciiImageBase(unsigned short format, Pixel background, AsciiImageBase&& image);
	/* Constructs a copy of part of the specified ascii image with the new format. */
	AsciiImageBase(Coord part, Coord partSize, unsigned short format, Pixel background, const AsciiImageBase& image);
	/* Constructs a copy of the specified ascii image. */
	AsciiImageBase(const AsciiImageBase& image);
public:
	/* Constructs a moved copy of the specified ascii image. */
	AsciiImageBase(AsciiImageBase&& image);
	/* Deconstructs the ascii image. */
	~AsciiImageBase();
	
	//========== OPERATORS ===========
protected:
	/* The copy assignment operator. */
	AsciiImageBase& operator=(const AsciiImageBase& image);
	/* The move assignment operator. */
	AsciiImageBase& operator=(AsciiImageBase&& image);

	//========== MANAGEMENT ==========
protected:
	// Hidden
	/* Cleans up the allocated data in the ascii image. */
	void _Delete();
	/* Resizes the ascii image. */
	void _Resize(Coord size);
	/* Applies the format to the ascii image to make sure the formatting doesn't conflict. */
	void _SetFormat(unsigned short format);
	/* Loads the ascii image from the input stream. */
	bool _Load(InputStream* in, const AsciiImageHeader& header, const AsciiImageFrameHeader& frame, bool isLayer = false);
	/* Saves the ascii image to the output stream. */
	bool _Save(OutputStream* out, const AsciiImageHeader& header, const AsciiImageFrameHeader& frame, bool isLayer = false);

	//=========== DRAWING ============
public:
	// Fill
	/* Clears the image with the background color. */
	void Clear();
	/* Fills the image with the specified fill. */
	void Fill(Pixel pixel, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	
	// Pixel Precision
	/* Sets the character at the specified point in the image. */
	void SetChar(Coord point, unsigned char character, unsigned short modAttributes = AttributeChar);
	/* Sets the character at the specified point in the image. */
	void SetChar(Coord point, Coord size, unsigned char character, unsigned short modAttributes = AttributeChar);
	/* Sets the color at the specified point in the image. */
	void SetColor(Coord point, unsigned char color, unsigned short modAttributes = AttributeColor);
	/* Sets the color at the specified point in the image. */
	void SetColor(Coord point, Coord size, unsigned char color, unsigned short modAttributes = AttributeColor);
	/* Sets the attributes at the specified point in the image. */
	void SetAttributes(Coord point, unsigned short attributes, unsigned short modAttributes = AttributeAll);
	/* Sets the attributes at the specified point in the image. */
	void SetAttributes(Coord point, Coord size, unsigned short attributes, unsigned short modAttributes = AttributeAll);
	/* Applies the special attributes at the specified point in the image. */
	void SetSpecial(Coord point, unsigned short attributes);
	/* Applies the special attributes at the specified point in the image. */
	void SetSpecial(Coord point, Coord size, unsigned short attributes);
	/* Sets the pixel at the specified point in the image. */
	void SetPixel(Coord point, Pixel pixel, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Sets the pixel at the specified point in the image. */
	void SetPixel(Coord point, Coord size, Pixel pixel, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);

	// Advanced Drawing
	/* Draws the line to the image. */
	void DrawLine(Coord point, Coord size, Pixel pixel, bool asEnd = false, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Draws the rectangled to the image. */
	void DrawRect(Coord point, Coord size, Pixel pixel, bool fill = false, bool asEnd = false, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Draws the ellipse to the image. */
	void DrawEllipse(Coord point, Coord size, Pixel pixel, bool fill = false, bool asOrigin = false, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Floods all matching pixels at the point in the image. */
	void DrawFloodFill(Coord point, Pixel pixel, bool specific = true, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Replaces all matching pixels at the point in the image. */
	void DrawReplace(Coord point, Pixel pixel, bool specific = true, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Replaces all matching pixels in the image. */
	void DrawReplace(Pixel target, Pixel pixel, bool specific = true, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Draws the string to the image. */
	void DrawString(Coord point, string text, Pixel pixel, Alignment alignment = Alignment::Left, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Draws the image to the image. */
	void DrawImage(Coord point, const AsciiImageBase& image, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Draws the partial image to the image. */
	void DrawImage(Coord point, Coord part, Coord partSize, const AsciiImageBase& image, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	
	// Borders
	/* Draws the border line to the image. */
	void DrawBorderLine(Coord point, Coord size, Pixel pixel, bool thick, bool priority = false, bool specific = true, bool asEnd = false, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);
	/* Draws the border rectangled to the image. */
	void DrawBorderRect(Coord point, Coord size, Pixel pixel, bool thick, bool priority = false, bool specific = true, bool asEnd = false, unsigned short modAttributes = AttributeAll, unsigned short modOptions = OptionNone);

	// Transformation
	/* Translates the ascii image by the specified distance. */
	void Translate(Coord distance);
	/* Rotates the ascii image by the specified number of 90 degree rotations. */
	void Rotate(int rotations);
	/* Flips the ascii image either horizontally or vertically. */
	void Flip(bool vertical);

	//========= INFORMATION ==========
	
	// Pixel Information
	/* Gets the character at the specified point in the image. */
	unsigned char GetChar(Coord point) const;
	/* Gets the color at the specified point in the image. */
	unsigned char GetColor(Coord point) const;
	/* Gets the attributes at the specified point in the image. */
	unsigned short GetAttributes(Coord point) const;
	/* Tests the attributes at the specified point in the image. */
	bool GetAttributes(Coord point, unsigned short attributes) const;
	/* Gets the pixel at the specified point in the image. */
	Pixel GetPixel(Coord point) const;

	// Ascii Information
	/* Gets the size of the image. */
	Coord Size() const;
	/* Gets the background fill used. */
	Pixel GetBackground() const;
	/* Gets the format of the image. */
	unsigned short GetFormat() const;
	/* Gets the frame speed of the image. */
	unsigned int GetFrameSpeed() const;
	/* Gets the array of pixels in the ascii image, the data is formatted as [x][y]. */
	Pixel** GetPixels() const;
	
	//=========== FRIENDS ============

	friend AsciiImageInfo;
	friend AsciiAnimation;
	friend vector<AsciiImageBase>;
};
/* A class to manage and contain a fully controllable ascii image. */
class AsciiImage : public AsciiImageBase {

	//========= CONSTRUCTORS =========
public:
	/* Constructs the default ascii image. */
	AsciiImage();
	/* Constructs an ascii image with the specified size. */
	AsciiImage(Coord size, unsigned short format);
	/* Constructs an ascii image with the specified size and background. */
	AsciiImage(Coord size, unsigned short format, Pixel background);
	/* Constructs a copy of the specified ascii image with the new format. */
	AsciiImage(unsigned short format, Pixel background, const AsciiImageBase& image);
	/* Constructs a moved copy of the specified ascii image with the new format. */
	AsciiImage(unsigned short format, Pixel background, AsciiImageBase&& image);
	/* Constructs a copy of part of the specified ascii image with the new format. */
	AsciiImage(Coord part, Coord partSize, unsigned short format, Pixel background, const AsciiImageBase& image);
	/* Constructs a copy of the specified ascii image. */
	AsciiImage(const AsciiImageBase& image);
	/* Constructs a moved copy of the specified ascii image. */
	AsciiImage(AsciiImageBase&& image);
	/* Constructs a copy of the specified ascii image. */
	AsciiImage(const AsciiImage& image);
	/* Constructs a moved copy of the specified ascii image. */
	AsciiImage(AsciiImage&& image);
	/* Deconstructs the ascii image. */
	~AsciiImage();
	
	//========== OPERATORS ===========

	/* The copy assignment operator. */
	AsciiImage& operator=(const AsciiImageBase& image);
	/* The move assignment operator. */
	AsciiImage& operator=(AsciiImageBase&& image);
	/* The copy assignment operator. */
	AsciiImage& operator=(const AsciiImage& image);
	/* The move assignment operator. */
	AsciiImage& operator=(AsciiImage&& image);
	
	//========== MANAGEMENT ==========
	
	// Input
	/* Loads the ascii image from the input stream. */
	bool Load(InputStream* in);
	/* Loads the ascii image from file. */
	bool LoadFile(const string& path);
	/* Loads the ascii image from the resources. */
	bool LoadResource(const string& name, const string& type = "ASCII_IMAGE");
	/* Loads the ascii image from the resources. */
	bool LoadResource(const string& name, unsigned int typeId);
	/* Loads the ascii image from the resources. */
	bool LoadResource(unsigned int id, const string& type = "ASCII_IMAGE");
	/* Loads the ascii image from the resources. */
	bool LoadResource(unsigned int id, unsigned int typeId);
	/* Loads the ascii image from the clipboard. */
	bool LoadClipboard();

	// Output
	/* Saves the ascii image to the output stream. */
	bool Save(OutputStream* out, unsigned short version = 3, bool smallFile = false);
	/* Saves the ascii image to file. */
	bool SaveFile(const string& path, unsigned short version = 3, bool smallFile = false);
	/* Saves the ascii image to the clipboard. */
	bool SaveClipboard(unsigned short version = 3, bool smallFile = false);
	
	//=========== DRAWING ============
	
	/* Resizes the image. */
	void Resize(Coord size);
	/* Rotates the image and resizes to compensate for non-square dimensions. */
	void RotateAndResize(int rotations);

	//========= INFORMATION ==========
	
	/* Sets the background fill used. */
	void SetBackground(Pixel background);
	/* Sets the format of the image. */
	void SetFormat(unsigned short format);
	/* Sets the frame speed of the image. */
	void SetFrameSpeed(unsigned int frameSpeed);
};
/* A class to manage and contain a fully controllable ascii animation. */
class AsciiAnimation {

	//=========== MEMBERS ============
private:
	/* The animation speed of the ascii animation. */
	unsigned int _AnimationSpeed;
	/* The list of frames in the ascii animation. */
	vector<AsciiImage> _Frames;

	//========= CONSTRUCTORS =========
public:
	/* Constructs the default ascii animation. */
	AsciiAnimation();
	/* Constructs an ascii animation with the specified size. */
	AsciiAnimation(Coord size, unsigned short format, unsigned int frames = 1);
	/* Constructs an ascii animation with the specified size and background. */
	AsciiAnimation(Coord size, unsigned short format, Pixel background, unsigned int frames = 1);
	/* Constructs a copy of the specified ascii image with the new format. */
	AsciiAnimation(unsigned short format, Pixel background, const AsciiImageBase& image);
	/* Constructs a moved copy of the specified ascii image with the new format. */
	AsciiAnimation(unsigned short format, Pixel background, AsciiImageBase&& image);
	/* Constructs a copy of part of the specified ascii image with the new format. */
	AsciiAnimation(Coord part, Coord partSize, unsigned short format, Pixel background, const AsciiImageBase& image);
	/* Constructs a copy of the specified ascii animation with the new format. */
	AsciiAnimation(unsigned short format, Pixel background, const AsciiAnimation& image);
	/* Constructs a moved copy of the specified ascii animation with the new format. */
	AsciiAnimation(unsigned short format, Pixel background, AsciiAnimation&& image);
	/* Constructs a copy of part of the specified ascii animation with the new format. */
	AsciiAnimation(Coord part, Coord partSize, unsigned short format, Pixel background, const AsciiAnimation& image);
	/* Constructs a copy of the specified ascii image. */
	AsciiAnimation(const AsciiImageBase& image);
	/* Constructs a moved copy of the specified ascii image. */
	AsciiAnimation(AsciiImageBase&& image);
	/* Constructs a copy of the specified ascii animation. */
	AsciiAnimation(const AsciiAnimation& image);
	/* Constructs a moved copy of the specified ascii animation. */
	AsciiAnimation(AsciiAnimation&& image);
	/* Deconstructs the ascii image. */
	~AsciiAnimation();
	
	//========== OPERATORS ===========

	// Assignment
	/* The copy assignment operator. */
	AsciiAnimation& operator=(const AsciiImageBase& image);
	/* The move assignment operator. */
	AsciiAnimation& operator=(AsciiImageBase&& image);
	/* The copy assignment operator. */
	AsciiAnimation& operator=(const AsciiAnimation& image);
	/* The move assignment operator. */
	AsciiAnimation& operator=(AsciiAnimation&& image);
	
	// Frames
	/* Gets the frame at the specified index. */
	AsciiImageBase& operator[](int index) const;

	//========== MANAGEMENT ==========
	
	// Input
	/* Loads the ascii animation from the input stream. */
	bool Load(InputStream* in);
	/* Loads the ascii animation from file. */
	bool LoadFile(const string& path);
	/* Loads the ascii animation from the resources. */
	bool LoadResource(const string& name, const string& type = "ASCII_IMAGE");
	/* Loads the ascii animation from the resources. */
	bool LoadResource(const string& name, unsigned int typeId);
	/* Loads the ascii animation from the resources. */
	bool LoadResource(unsigned int id, const string& type = "ASCII_IMAGE");
	/* Loads the ascii animation from the resources. */
	bool LoadResource(unsigned int id, unsigned int typeId);
	/* Loads the ascii animation from the clipboard. */
	bool LoadClipboard();

	// Output
	/* Saves the ascii animation to the output stream. */
	bool Save(OutputStream* out, unsigned short version = 3, bool smallFile = false);
	/* Saves the ascii animation to file. */
	bool SaveFile(const string& path, unsigned short version = 3, bool smallFile = false);
	/* Saves the ascii animation to the clipboard. */
	bool SaveClipboard(unsigned short version = 3, bool smallFile = false);
	
	//============ FRAMES ============
	
	/* Gets the frame at the specified index. */
	AsciiImageBase& GetFrame(int index) const;
	/* Adds a frame to the end of the list. */
	void AddFrame();
	/* Adds a frame at the specified index in the list. */
	void AddFrame(int index);
	/* Adds a frame at the specified index in the list. */
	void AddFrame(int index, const AsciiImageBase& image);
	/* Adds a frame at the specified index in the list. */
	void AddFrame(int index, Coord part, Coord partSize, const AsciiImageBase& image);
	/* Sets the frame at the specified index in the list. */
	void SetFrame(int index, const AsciiImageBase& image);
	/* Sets the frame at the specified index in the list. */
	void SetFrame(int index, Coord part, Coord partSize, const AsciiImageBase& image);
	/* Removes the last frame in the list. */
	void RemoveFrame();
	/* Removes the frame from the specified index in the list. */
	void RemoveFrame(int index);
	/* Swaps the specified frames. */
	void SwapFrame(int index1, int index2);
	/* Moves the specified frame though the list. */
	void MoveFrame(int indexOld, int indexNew, bool relative = false);
	/* Clears the image and removes all but one of the frames. */
	void Clear();
	
	//=========== DRAWING ============
	
	/* Translates the ascii image by the specified distance. */
	void Translate(Coord distance, int index = -1);
	/* Rotates the ascii image by the specified number of 90 degree rotations. */
	void Rotate(int rotations, int index = -1);
	/* Flips the ascii image either horizontally or vertically. */
	void Flip(bool vertical, int index = -1);
	/* Resizes the image. */
	void Resize(Coord size, int index = -1);
	/* Rotates the image and resizes to compensate for non-square dimensions. */
	void RotateAndResize(int rotations, int index = -1);

	//========= INFORMATION ==========
	
	// Ascii Information
	/* Gets the largest frame size of the animation. */
	Coord Size() const;
	/* Gets the background fill used. */
	Pixel GetBackground() const;
	/* Sets the background fill used. */
	void SetBackground(Pixel background);
	/* Gets the number of frames in the animation. */
	unsigned int NumFrames() const;
	/* Gets the format of the image. */
	unsigned short GetFormat() const;
	/* Sets the format of the image. */
	void SetFormat(unsigned short format);
	/* Gets the animation speed of the the animation. */
	unsigned int GetAnimationSpeed() const;
	/* Sets the animation speed of the the animation. */
	void SetAnimationSpeed(unsigned int animationSpeed);
	/* Sets the frame speed of the specified frame. */
	void SetFrameSpeed(unsigned int frameSpeed, int index = -1);
	
	//=========== FRIENDS ============

	friend AsciiImageInfo;
};
//================================================================
// END ASCII IMAGE HEADER:
//================================================================
}}
#endif