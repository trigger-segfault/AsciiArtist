/*=================================================================|
|* File:				AsciiAnimation.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_ANIMATION_H
#define ASCII_ANIMATION_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiImage.h>

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A class to manage and contain a fully controllable ascii animation. */
class AsciiAnimation {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The animation speed of the ascii animation. */
	unsigned int animationSpeed;
	/* The list of frames in the ascii animation. */
	std::vector<AsciiImage> frames;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default ascii animation. */
	AsciiAnimation();
	/* Constructs an ascii animation with the specified size. */
	AsciiAnimation(PcGeo::Point2I size, ImageFormats format, unsigned int frames = 1);
	/* Constructs an ascii animation with the specified size and background. */
	AsciiAnimation(PcGeo::Point2I size, ImageFormats format, Pixel background, unsigned int frames = 1);
	/* Constructs a copy of the specified ascii image with the new format. */
	AsciiAnimation(ImageFormats format, Pixel background, const AsciiImageFrame& image);
	/* Constructs a moved copy of the specified ascii image with the new format. */
	AsciiAnimation(ImageFormats format, Pixel background, AsciiImageFrame&& image);
	/* Constructs a copy of part of the specified ascii image with the new format. */
	AsciiAnimation(PcGeo::Rectangle2I region, ImageFormats format, Pixel background, const AsciiImageFrame& image);
	/* Constructs a copy of the specified ascii animation with the new format. */
	AsciiAnimation(ImageFormats format, Pixel background, const AsciiAnimation& animation);
	/* Constructs a moved copy of the specified ascii animation with the new format. */
	AsciiAnimation(ImageFormats format, Pixel background, AsciiAnimation&& animation);
	/* Constructs a copy of part of the specified ascii animation with the new format. */
	AsciiAnimation(PcGeo::Rectangle2I region, ImageFormats format, Pixel background, const AsciiAnimation& animation);
	/* Constructs a copy of the specified ascii image. */
	AsciiAnimation(const AsciiImageFrame& image);
	/* Constructs a moved copy of the specified ascii image. */
	AsciiAnimation(AsciiImageFrame&& image);
	/* Constructs a copy of the specified ascii animation. */
	AsciiAnimation(const AsciiAnimation& animation);
	/* Constructs a moved copy of the specified ascii animation. */
	AsciiAnimation(AsciiAnimation&& animation);
	/* Deconstructs the ascii image. */
	~AsciiAnimation();

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	// Assignment
	/* The copy assignment operator. */
	AsciiAnimation& operator=(const AsciiImageFrame& image);
	/* The move assignment operator. */
	AsciiAnimation& operator=(AsciiImageFrame&& image);
	/* The copy assignment operator. */
	AsciiAnimation& operator=(const AsciiAnimation& animation);
	/* The move assignment operator. */
	AsciiAnimation& operator=(AsciiAnimation&& animation);

	// Frames
	/* Gets the frame at the specified index. */
	AsciiImageFrame& operator[](int index);
	/* Gets the frame at the specified index. */
	const AsciiImageFrame& operator[](int index) const;
	/* Gets the frame with the specified name. */
	AsciiImageFrame& operator[](const std::string& name);
	/* Gets the frame with the specified name. */
	const AsciiImageFrame& operator[](const std::string& name) const;

	#pragma endregion
	//========= INPUT/OUTPUT =========
	#pragma region Input/Output

	// Input
	/* Loads the ascii animation from the input stream. */
	bool load(PcIO::InputStream* in);
	/* Loads the ascii animation from file. */
	bool loadFile(const std::string& path);
	/* Loads the ascii animation from the resources. */
	bool loadResource(const std::string& name, const std::string& type = "ASCII_IMAGE");
	/* Loads the ascii animation from the resources. */
	bool loadResource(const std::string& name, unsigned int typeId);
	/* Loads the ascii animation from the resources. */
	bool loadResource(unsigned int id, const std::string& type = "ASCII_IMAGE");
	/* Loads the ascii animation from the resources. */
	bool loadResource(unsigned int id, unsigned int typeId);
	/* Loads the ascii animation from the clipboard. */
	bool loadClipboard();

	// Output
	/* Saves the ascii animation to the output stream. */
	bool save(PcIO::OutputStream* out, unsigned short version = AsciiImageInfo::FILE_VERSION, bool smallFile = false) const;
	/* Saves the ascii animation to file. */
	bool saveFile(const std::string& path, unsigned short version = AsciiImageInfo::FILE_VERSION, bool smallFile = false) const;
	/* Saves the ascii animation to the clipboard. */
	bool saveClipboard(unsigned short version = AsciiImageInfo::FILE_VERSION, bool smallFile = false) const;

	#pragma endregion
	//============ FRAMES ============
	#pragma region Frames

	// Frames
	/* Gets the frame at the specified index. */
	AsciiImageFrame& at(int index);
	/* Gets the frame at the specified index. */
	const AsciiImageFrame& at(int index) const;
	/* Gets the frame with the specified name. */
	AsciiImageFrame& at(const std::string& name);
	/* Gets the frame with the specified name. */
	const AsciiImageFrame& at(const std::string& name) const;
	/* Adds a frame to the end of the list. */
	void addFrame();
	/* Adds a frame to the end of the list. */
	void addFrame(const AsciiImageFrame& image);
	/* Adds a frame to the end of the list. */
	void addFrame(AsciiImageFrame&& image);
	/* Adds a frame to the end of the list. */
	void addFrame(PcGeo::Rectangle2I region, const AsciiImageFrame& image);
	/* Inserts a frame at the specified index in the list. */
	void insertFrame(int index);
	/* Inserts a frame at the specified index in the list. */
	void insertFrame(int index, const AsciiImageFrame& image);
	/* Inserts a frame at the specified index in the list. */
	void insertFrame(int index, AsciiImageFrame&& image);
	/* Inserts a frame at the specified index in the list. */
	void insertFrame(int index, PcGeo::Rectangle2I region, const AsciiImageFrame& image);
	/* Sets the frame at the specified index in the list. */
	void setFrame(int index, const AsciiImageFrame& image);
	/* Sets the frame at the specified index in the list. */
	void setFrame(int index, AsciiImageFrame&& image);
	/* Sets the frame at the specified index in the list. */
	void setFrame(int index, PcGeo::Rectangle2I region, const AsciiImageFrame& image);
	/* Removes the last frame in the list. */
	void removeFrame();
	/* Removes the frame from the specified index in the list. */
	void removeFrame(int index);
	/* Swaps the specified frames. */
	void swapFrame(int indexA, int indexB);
	/* Moves the specified frame though the list. */
	void moveFrame(int index, int newIndex, bool relative = false);
	/* Clears the image and removes all but one of the frames. */
	void clearFrames();

	// Frames with Names
	/* Gets the name of the specified frame. */
	std::string getName(int index) const;
	/* Sets the name of the specified frame. */
	void setName(int index, const std::string& name);
	/* Returns true if a frame with the specified name exists. */
	bool containsName(const std::string& name) const;

	#pragma endregion
	//========= MODIFICATION =========
	#pragma region Modification

	/* Resizes the image. */
	void resize(PcGeo::Point2I size, int index = -1);
	/* Rotates the ascii image by the specified number of 90 degree rotations. */
	void rotate(int rotations, int index = -1);
	/* Rotates the image and resizes to compensate for non-square dimensions. */
	void rotateAndResize(int rotations, int index = -1);
	/* Translates the ascii image by the specified distance. */
	void translate(PcGeo::Point2I distance, int index = -1);
	/* Flips the ascii image either horizontally or vertically. */
	void flip(PcGeo::Orientations axis, int index = -1);
	/* Clears the specified frame. */
	void clear(int index = -1);
	/* Creates a graphics object for this animation. */
	GraphicsSPtr createGraphics();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Ascii Information
	/* Gets the largest frame size of the animation. */
	PcGeo::Point2I size() const;
	/* Gets the background fill used. */
	Pixel getBackground() const;
	/* Sets the background fill used. */
	void setBackground(Pixel background);
	/* Gets the number of frames in the animation. */
	unsigned int frameCount() const;
	/* Gets the format of the image. */
	ImageFormats getFormat() const;
	/* Sets the format of the image. */
	void setFormat(ImageFormats format);
	/* Gets the animation speed of the the animation. */
	unsigned int getAnimationSpeed() const;
	/* Sets the animation speed of the the animation. */
	void setAnimationSpeed(unsigned int animationSpeed);
	/* Gets the frame speed of the specified frame. */
	unsigned int getFrameSpeed(int index) const;
	/* Sets the frame speed of the specified frame. */
	void setFrameSpeed(unsigned int frameSpeed, int index = -1);

	#pragma endregion
	//=========== FRIENDS ============
	#pragma region Friends

	friend AsciiImageInfo;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
