/*=================================================================|
|* File:				Graphics.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <stack>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/Pixel.h>
#include <PowerConsole/Geometry/Rectangle.h>
#include <PowerConsole/Geometry/GeometryEnums.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <PowerConsole/Drawing/Borders.h>

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The graphics options structure used in the Graphics class. */
struct GraphicsOptions {

	//=========== MEMBERS ============
	#pragma region Members

	/* The translation of the graphics. */
	PcGeo::Point2I translation;
	/* The modifiable translation of the graphics. */
	PcGeo::Point2I extraTranslation;
	/* The region the graphics is allowed to draw in. */
	PcGeo::Rectangle2I region;
	/* True if the region bounds are unlocked. */
	bool unlockRegion;
	/* The mod attributes to use when drawing. */
	PixelAttributes modAttributes;
	/* The mod options to use when drawing. */
	PixelOptions modOptions;
	/* True if borders can connect outside of the region. */
	bool connectGlobalBorders;
	/* The current frame of the animation to draw to. */
	int currentFrame;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the starting graphics option. */
	GraphicsOptions(PcGeo::Rectangle2I startRegion);
	/* Extends the previous graphics region by constructing one related to the last one. */
	GraphicsOptions(PcGeo::Point2I newTranslation, PcGeo::Rectangle2I newRegion, const GraphicsOptions& previousOptions);

	#pragma endregion
};

/* Disables border connections in specified directions. */
struct DisabledBorders {

	/* True if left border connections are disabled. */
	bool left;
	/* True if top border connections are disabled. */
	bool top;
	/* True if right border connections are disabled. */
	bool right;
	/* True if bottom border connections are disabled. */
	bool bottom;
	/* True if the inside of the border rectable is disabled. */
	bool inside;
	/* True if the outside of the border rectable is disabled. */
	bool outside;

	/* Constructs disabled borders struct with no disables. */
	inline DisabledBorders()
		: left(false), top(false), right(false), bottom(false), inside(false), outside(false) {}
	inline DisabledBorders(bool all)
		: left(all), top(all), right(all), bottom(all), inside(all), outside(all) {}
	/* Constructs a disabled borders struct. */
	inline DisabledBorders(bool left, bool top, bool right, bool bottom)
		: left(left), top(top), right(right), bottom(bottom), inside(inside), outside(outside) {}
	/* Constructs a disabled borders struct. */
	inline DisabledBorders(bool inside, bool outside)
		: left(false), top(false), right(false), bottom(false), inside(inside), outside(outside) {}
	/* Constructs a disabled borders struct. */
	inline DisabledBorders(bool left, bool top, bool right, bool bottom, bool inside, bool outside)
		: left(left), top(top), right(right), bottom(bottom), inside(inside), outside(outside) {}
};

/* The graphics class for drawing to an ascii image. */
class Graphics {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The options region stack. */
	std::stack<GraphicsOptions> optionsStack;
	/* The current graphics options. */
	GraphicsOptions options;

	/* The current frame being drawn to. */
	AsciiImageFrame* frame;
	/* The current image containing the frame. */
	AsciiImage* asciiImage;
	/* The current animation containing the frames. */
	AsciiAnimation* asciiAnimation;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs a graphics object with a standard ascii image. */
	Graphics(AsciiImage* asciiImage);
	/* Constructs a graphics object with a standard ascii image. */
	Graphics(AsciiImageSPtr asciiImage);
	/* Constructs a graphics object with an ascii animation. */
	Graphics(AsciiAnimation* asciiAnimation);
	/* Constructs a graphics object with an ascii animation. */
	Graphics(AsciiAnimationSPtr asciiAnimation);

	#pragma endregion
	//=========== REGIONS ============
	#pragma region Regions
public:
	/* Increments the region to a smaller area within the current region. */
	void startRegion(PcGeo::Point2I translation, PcGeo::Rectangle2I region);
	/* Decrements the region to the outside area containing the current region. */
	void endRegion();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the current draw region of the graphics object. */
	PcGeo::Rectangle2I getRegion() const;
	/* Gets the current draw translation of the graphics object. */
	PcGeo::Point2I getTranslation() const;
	/* Gets the current draw modifiable translation of the graphics object. */
	PcGeo::Point2I getExtraTranslation() const;
	/* Sets the current draw modifiable translation of the graphics object. */
	void setExtraTranslation(PcGeo::Point2I translation);
	/* Gets the current region level. */
	int getRegionLevel() const;
	/* Gets if the region is unlocked and free access is given to the image. */
	bool isRegionUnlocked() const;
	/* Sets if the region is unlocked and free access is given to the image. */
	void setRegionUnlocked(bool unlocked);
	/* Gets if borders can connect outside of the region. */
	bool getConnectGlobalBorders() const;
	/* Sets if borders can connect outside of the region. */
	void setConnectGlobalBorders(bool connectGlobalBorders);
	/* Resets the current options. */
	void resetOptions();

	/* Gets the total number of frames. */
	int getFrameCount() const;
	/* Gets the current frame being drawn to. */
	int getCurrentFrame() const;
	/* Sets the curremt frame being drawn to. */
	void setCurrentFrame(int index);

	/* Gets the modification attributes that will be used in drawing. */
	PixelAttributes getModAttributes() const;
	/* Sets the modification attributes that will be used in drawing. */
	void setModAttributes(PixelAttributes modAttributes);
	/* Gets the modification options that change how attributes are applied when drawing. */
	PixelOptions getModOptions() const;
	/* Sets the modification options that change how attributes are applied when drawing. */
	void setModOptions(PixelOptions modOptions);


	#pragma endregion
	//============ PIXELS ============
	#pragma region Pixels

	// Pixel Information
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
	//=========== DRAWING ============
	#pragma region Drawing

	// Pixel Precision
	/* Sets the character at the specified point in the image. */
	void setChar(PcGeo::Point2I point, unsigned char character);
	/* Sets the character at the specified point in the image. */
	void setChar(PcGeo::Point2I point, PcGeo::Point2I size, unsigned char character);
	/* Sets the character at the specified point in the image. */
	void setChar(PcGeo::Rectangle2I rect, unsigned char character);
	/* Sets the color at the specified point in the image. */
	void setColor(PcGeo::Point2I point, unsigned char color);
	/* Sets the color at the specified point in the image. */
	void setColor(PcGeo::Point2I point, PcGeo::Point2I size, unsigned char color);
	/* Sets the color at the specified point in the image. */
	void setColor(PcGeo::Rectangle2I rect, unsigned char color);
	/* Sets the attributes at the specified point in the image. */
	void setAttributes(PcGeo::Point2I point, PixelAttributes attributes);
	/* Sets the attributes at the specified point in the image. */
	void setAttributes(PcGeo::Point2I point, PcGeo::Point2I size, PixelAttributes attributes);
	/* Sets the attributes at the specified point in the image. */
	void setAttributes(PcGeo::Rectangle2I rect, PixelAttributes attributes);
	/* Applies the special attributes at the specified point in the image. */
	void setSpecial(PcGeo::Point2I point, PixelAttributes attributes);
	/* Applies the special attributes at the specified point in the image. */
	void setSpecial(PcGeo::Point2I point, PcGeo::Point2I size, PixelAttributes attributes);
	/* Applies the special attributes at the specified point in the image. */
	void setSpecial(PcGeo::Rectangle2I rect, PixelAttributes attributes);
	/* Sets the pixel at the specified point in the image. */
	void setPixel(PcGeo::Point2I point, Pixel pixel);
	/* Sets the pixel at the specified point in the image. */
	void setPixel(PcGeo::Point2I point, PcGeo::Point2I size, Pixel pixel);
	/* Sets the pixel at the specified point in the image. */
	void setPixel(PcGeo::Rectangle2I rect, Pixel pixel);

	// Fill
	/* Clears the image with the background color. */
	void clear();
	/* Fills the image with the specified pixel. */
	void fill(Pixel pixel);

	// Advanced Drawing
	/* Draws the line to the image. */
	void drawLine(PcGeo::Point2I point, PcGeo::Point2I size, Pixel pixel, bool asEnd = false);
	/* Draws the rectangled to the image. */
	void drawRectangle(PcGeo::Point2I point, PcGeo::Point2I size, Pixel pixel, bool fill = false, bool asEnd = false);
	/* Draws the rectangled to the image. */
	void drawRectangle(PcGeo::Rectangle2I rect, Pixel pixel, bool fill = false);
	/* Draws the ellipse to the image. */
	void drawEllipse(PcGeo::Point2I point, PcGeo::Point2I size, Pixel pixel, bool fill = false, bool asOrigin = false);
	/* Floods all matching pixels at the point in the image. */
	void drawFloodFill(PcGeo::Point2I point, Pixel pixel, bool specific = true);
	/* Replaces all matching pixels at the point in the image. */
	void drawReplace(PcGeo::Point2I point, Pixel pixel, bool specific = true);
	/* Replaces all matching pixels in the image. */
	void drawReplace(Pixel target, Pixel pixel, bool specific = true);
	/* Draws the string to the image. */
	void drawString(PcGeo::Point2I point, const std::string& text, Pixel pixel, PcGeo::HorizontalAlignments alignment = PcGeo::HorizontalAlignments::Left);
	/* Draws the string to the image. */
	void drawString(PcGeo::Point2I point, const std::string& text, Pixel pixel, PcGeo::HorizontalAlignments alignment, int width);
	/* Draws the image to the image. */
	void drawImage(PcGeo::Point2I point, const AsciiImageFrame& image);
	/* Draws the partial image to the image. */
	void drawImage(PcGeo::Point2I point, PcGeo::Point2I part, PcGeo::Point2I partSize, const AsciiImageFrame& image);
	/* Draws the partial image to the image. */
	void drawImage(PcGeo::Point2I point, PcGeo::Rectangle2I region, const AsciiImageFrame& image);

	// Borders
	/* Draws the border rectangled to the image. */
	void drawBorder(PcGeo::Rectangle2I rect, Pixel pixel, bool thick, BorderThickness connections = BORDER_BOTH, ConnectionPriorities priorities = ConnectionPriorities::ThickBeforeThin);
	/* Draws the border rectangled to the image. */
	void drawBorder(PcGeo::Rectangle2I rect, Pixel pixel, bool thick, DisabledBorders disabledBorders, BorderThickness connections = BORDER_BOTH, ConnectionPriorities priorities = ConnectionPriorities::ThickBeforeThin);
	/* Sets the pixel for the border and gets the surrounding borders. */
	void setBorderPixel(PcGeo::Point2I point, unsigned char left, unsigned char top, unsigned char right, unsigned char bottom, Pixel pixel, BorderThickness connections = BORDER_BOTH, ConnectionPriorities priorities = ConnectionPriorities::ThickBeforeThin);
	/* Force-sets the pixel for the border and gets the surrounding borders. */
	void forceBorderPixel(PcGeo::Point2I point, unsigned char left, unsigned char top, unsigned char right, unsigned char bottom, Pixel pixel, BorderThickness connections = BORDER_BOTH, ConnectionPriorities priorities = ConnectionPriorities::ThickBeforeThin);

	// Transformation
	/* Translates the ascii image by the specified distance. */
	void translate(PcGeo::Point2I distance);
	/* Rotates the ascii image by the specified number of 90 degree rotations. */
	void rotate(int rotations);
	/* Flips the ascii image either horizontally or vertically. */
	void flip(PcGeo::Orientations axis);

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers

	/* Draws points of an unfinished ellipse. */
	void ellipseHelper(PcGeo::Point2I center, PcGeo::Point2I point, PcGeo::Point2I size, PcGeo::Point2I odd, Pixel pixel, bool fill);

	#pragma endregion
	//=========== FRIENDS ============
	#pragma region Friends

	friend AsciiImage;
	friend AsciiAnimation;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
