/*=================================================================|
|* File:				Graphics.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Graphics.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
//=================================================================|
// DEFINITIONS													   |
//=================================================================/

/* The current translation. */
#define tpoint (options.translation + options.extraTranslation)
/* The current region's point. */
#define rpoint options.region.point
/* The current region's size. */
#define rsize options.region.size
/* The current region */
#define r options.region
/* True if the region is unlocked. */
#define unlock options.unlockRegion

/* The current frame's size. */
#define fsize frame->frameSize

/* Makes sure the x point is within the frame and also within the region. */
#define leftBoundsCheck(px) ((tpoint.x + (px) >= 0) && (unlock ? true : (tpoint.x + (px) >= rpoint.x)))
/* Makes sure the x point is within the frame and also within the region. */
#define rightBoundsCheck(px) ((tpoint.x + (px) < fsize.x) && (unlock ? true : (tpoint.x + (px) < rpoint.x + rsize.x)))
/* Makes sure the x point is within the frame and also within the region. */
#define xBoundsCheck(px) (leftBoundsCheck(px) && rightBoundsCheck(px))

/* Makes sure the y point is within the frame and also within the region. */
#define topBoundsCheck(py) ((tpoint.y + (py) >= 0) && (unlock ? true : (tpoint.y + (py) >= rpoint.y)))
/* Makes sure the y point is within the frame and also within the region. */
#define bottomBoundsCheck(py) ((tpoint.y + (py) < fsize.y) && (unlock ? true : (tpoint.y + (py) < rpoint.y + rsize.y)))
/* Makes sure the y point is within the frame and also within the region. */
#define yBoundsCheck(py) (topBoundsCheck(py) && bottomBoundsCheck(py))

/* Makes sure the point is within the frame and also within the region. */
#define topLeftBoundsCheck(point) (leftBoundsCheck((point).x) && topBoundsCheck((point).y))
/* Makes sure the point is within the frame and also within the region. */
#define bottomRightBoundsCheck(point) (rightBoundsCheck((point).x) && bottomBoundsCheck((point).y))
/* Makes sure the point is within the frame and also within the region. */
#define boundsCheck(point) (xBoundsCheck((point).x) && yBoundsCheck((point).y))

/* Makes sure the point is within the frame and also within the region. */
//#define boundsCheck(point) ((tpoint + (point) >= Point2I::ZERO) && (tpoint + (point) < fsize) && (unlock ? true : ((tpoint + (point) >= rpoint) && (tpoint + (point) < rpoint + rsize))))
/* Makes sure the point is within the frame and also within the region. Leaves out primary bounds checking. */
//#define rboundsCheck(point) (((point) >= Point2I::ZERO) && ((point) < rsize))

/* Gets the pixel at the relative point in the grid. */
#define toPixel(point) frame->pixels[pointToIndex(tpoint + (point), fsize.x)]
/* Gets the pixel at the relative point in the grid. */
#define toPixelxy(px, py) frame->pixels[xyToIndex(tpoint.x + (px), tpoint.y + (py), fsize.x)]

// Mod Option Flags
#define replace getFlag(options.modOptions, PixelOptions::Replace)
#define opposite getFlag(options.modOptions, PixelOptions::Opposite)
#define inverse getFlag(options.modOptions, PixelOptions::Inverse)
#define noPixel getFlag(options.modOptions, PixelOptions::NoPixel)
#define noAttributes (getFlag(options.modOptions, PixelOptions::NoAttributes) || !getFlag(frame->format, ImageFormats::Attributes))
#define copySpecial getFlag(options.modOptions, PixelOptions::CopySpecial)

//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

GraphicsOptions::GraphicsOptions(Rectangle2I startRegion)
 :	translation(Point2I::ZERO), region(startRegion),
	extraTranslation(Point2I::ZERO), unlockRegion(false),
	modAttributes(PixelAttributes::All), modOptions(PixelOptions::None),
	connectGlobalBorders(false), currentFrame(0) {}
GraphicsOptions::GraphicsOptions(Point2I newTranslation, Rectangle2I newRegion, const GraphicsOptions& previousOptions)
 :	translation(previousOptions.translation + newTranslation),
	extraTranslation(Point2I::ZERO), unlockRegion(false),
	region(newRegion.translate(previousOptions.translation + newTranslation)),
	modAttributes(PixelAttributes::All), modOptions(PixelOptions::None),
	connectGlobalBorders(false), currentFrame(0) {}

//--------------------------------

Graphics::Graphics(AsciiImage* asciiImage)
	: asciiImage(asciiImage), asciiAnimation(nullptr),
	frame(asciiImage), options(asciiImage->size()) {}
Graphics::Graphics(AsciiImageSPtr asciiImage)
	:	Graphics(asciiImage.get()) {}
Graphics::Graphics(AsciiAnimation* asciiAnimation)
	:	asciiAnimation(asciiAnimation), asciiImage(nullptr),
		frame(&asciiAnimation->at(0)), options(asciiAnimation->size()) {}
Graphics::Graphics(AsciiAnimationSPtr asciiAnimation)
	:	Graphics(asciiAnimation.get()) {}

#pragma endregion
//=========== REGIONS ============
#pragma region Regions

void Graphics::startRegion(Point2I translation, Rectangle2I region) {
	optionsStack.push(options);
	options = GraphicsOptions(translation, region, optionsStack.top());
}
void Graphics::endRegion() {
	if (optionsStack.empty())
		throw std::exception("Cannot end region. No region before this one exists in 'Graphics::endRegion()'");

	options = optionsStack.top();
	optionsStack.pop();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

Rectangle2I Graphics::getRegion() const {
	return options.region;
}
Point2I Graphics::getTranslation() const {
	return options.translation;
}
Point2I Graphics::getExtraTranslation() const {
	return options.extraTranslation;
}
void Graphics::setExtraTranslation(Point2I translation) {
	options.extraTranslation = translation;
}
int Graphics::getRegionLevel() const {
	return optionsStack.size();
}
bool Graphics::isRegionUnlocked() const {
	return options.unlockRegion;
}
void Graphics::setRegionUnlocked(bool unlocked) {
	options.unlockRegion = unlocked;
}
bool Graphics::getConnectGlobalBorders() const {
	return options.connectGlobalBorders;
}
void Graphics::setConnectGlobalBorders(bool connectGlobalBorders) {
	options.connectGlobalBorders = connectGlobalBorders;
}
void Graphics::resetOptions() {
	options.extraTranslation = false;
	options.unlockRegion = false;
	options.connectGlobalBorders = false;
	options.modAttributes = PixelAttributes::All;
	options.modOptions = PixelOptions::None;
	options.currentFrame = 0;
}

int Graphics::getFrameCount() const {
	if (asciiAnimation != nullptr)
		return (int)asciiAnimation->frameCount();
	return 1;
}
int Graphics::getCurrentFrame() const {
	return options.currentFrame;
}
void Graphics::setCurrentFrame(int index) {
	if (asciiAnimation != nullptr) {
		if (index < 0 || index >= (int)asciiAnimation->frameCount())
			throw std::out_of_range("Index out of range in 'Graphics::setCurrentFrame(...)'");
		options.currentFrame = index;
		frame = &asciiAnimation->at(index);
	}
	else if (index != 0) {
		throw std::out_of_range("Index out of range in 'Graphics::setCurrentFrame(...)'");
	}
}

PixelAttributes Graphics::getModAttributes() const {
	return options.modAttributes;
}
void Graphics::setModAttributes(PixelAttributes modAttributes) {
	options.modAttributes = modAttributes;
}
PixelOptions Graphics::getModOptions() const {
	return options.modOptions;
}
void Graphics::setModOptions(PixelOptions modOptions) {
	options.modOptions = modOptions;
}

#pragma endregion
//============ PIXELS ============
#pragma region Pixels

// Pixel Information
unsigned char Graphics::getChar(Point2I point) const {
	if (boundsCheck(point))
		return toPixel(point).character;
	return '\0';
}
unsigned char Graphics::getColor(Point2I point) const {
	if (boundsCheck(point))
		return toPixel(point).color;
	return 0x00;
}
PixelAttributes Graphics::getAttributes(Point2I point) const {
	if (boundsCheck(point))
		return toPixel(point).attributes;
	return PixelAttributes::None;
}
Pixel Graphics::getPixel(Point2I point) const {
	if (boundsCheck(point))
		return toPixel(point);
	return Pixel();
}
bool Graphics::testAttributes(Point2I point, PixelAttributes attributes, bool all) const {
	if (boundsCheck(point)) {
		if (all)
			return getFlag(toPixel(point).attributes, attributes);
		else
			return (toPixel(point).attributes & attributes) != PixelAttributes::None;
	}
	return (attributes == PixelAttributes::None);
}

#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

// Pixel Precision
void Graphics::setChar(Point2I point, unsigned char character) {
	if (boundsCheck(point)) {
		if (getFlag(options.modAttributes, PixelAttributes::Char))
			toPixel(point).character = character;
	}
}
void Graphics::setChar(Point2I point, Point2I size, unsigned char character) {
	setChar(Rectangle2I(point, size), character);
}
void Graphics::setChar(Rectangle2I rect, unsigned char character) {
	Point2I p;
	for (p.x = std::max(0, rect.point.x); p.x < rect.point.x + rect.size.x; p.x++) {
		for (p.y = std::max(0, rect.point.y); p.y < rect.point.y + rect.size.y; p.y++) {
			setChar(p, character);
		}
	}
}
void Graphics::setColor(Point2I point, unsigned char color) {
	if (boundsCheck(point)) {
		if (getFlag(options.modAttributes, PixelAttributes::FColor))
			toPixel(point).color = setFColor(toPixel(point).color, color);
		if (getFlag(options.modAttributes, PixelAttributes::BColor))
			toPixel(point).color = setBColor(toPixel(point).color, color);
	}
}
void Graphics::setColor(Point2I point, Point2I size, unsigned char color) {
	setColor(Rectangle2I(point, size), color);
}
void Graphics::setColor(Rectangle2I rect, unsigned char color) {
	Point2I p;
	for (p.x = std::max(0, rect.point.x); p.x < rect.point.x + rect.size.x; p.x++) {
		for (p.y = std::max(0, rect.point.y); p.y < rect.point.y + rect.size.y; p.y++) {
			setColor(p, color);
		}
	}
}
void Graphics::setAttributes(Point2I point, PixelAttributes attributes) {
	if (!noAttributes) {
		if (boundsCheck(point)) {
			toPixel(point).attributes = (toPixel(point).attributes | (attributes & options.modAttributes));
		}
	}
}
void Graphics::setAttributes(Point2I point, Point2I size, PixelAttributes attributes) {
	setAttributes(Rectangle2I(point, size), attributes);
}
void Graphics::setAttributes(Rectangle2I rect, PixelAttributes attributes) {
	if (!noAttributes) {
		Point2I p;
		for (p.x = std::max(0, rect.point.x); p.x < rect.point.x + rect.size.x; p.x++) {
			for (p.y = std::max(0, rect.point.y); p.y < rect.point.y + rect.size.y; p.y++) {
				setAttributes(p, attributes);
			}
		}
	}
}
void Graphics::setSpecial(Point2I point, PixelAttributes attributes) {
	if (boundsCheck(point)) {
		// Invert
		if (getFlag(attributes, PixelAttributes::FInvert))
			toPixel(point).color = invertFColor(toPixel(point).color);
		if (getFlag(attributes, PixelAttributes::BInvert))
			toPixel(point).color = invertBColor(toPixel(point).color);

		// Lighten
		if (getFlag(attributes, PixelAttributes::FLighten) && !getFlag(attributes, PixelAttributes::FDarken))
			toPixel(point).color = lightenFColor(toPixel(point).color);
		if (getFlag(attributes, PixelAttributes::BLighten) && !getFlag(attributes, PixelAttributes::BDarken))
			toPixel(point).color = lightenBColor(toPixel(point).color);

		// Darken
		if (getFlag(attributes, PixelAttributes::FDarken) && !getFlag(attributes, PixelAttributes::FLighten))
			toPixel(point).color = darkenFColor(toPixel(point).color);
		if (getFlag(attributes, PixelAttributes::BDarken) && !getFlag(attributes, PixelAttributes::BLighten))
			toPixel(point).color = darkenBColor(toPixel(point).color);
	}
}
void Graphics::setSpecial(Point2I point, Point2I size, PixelAttributes attributes) {
	setSpecial(Rectangle2I(point, size), attributes);
}
void Graphics::setSpecial(Rectangle2I rect, PixelAttributes attributes) {
	Point2I p;
	for (p.x = std::max(0, rect.point.x); p.x < rect.point.x + rect.size.x; p.x++) {
		for (p.y = std::max(0, rect.point.y); p.y < rect.point.y + rect.size.y; p.y++) {
			setSpecial(p, attributes);
		}
	}
}
void Graphics::setPixel(Point2I point, Pixel pixel) {
	if (boundsCheck(point)) {
		// Try to perform a less costly set pixel
		if (options.modAttributes == PixelAttributes::All && (options.modOptions == (PixelOptions::Replace | PixelOptions::CopySpecial))) {
			toPixel(point) = pixel;
		}
		else {
			Pixel& output = toPixel(point);

			//========== CHARACTER ===========

			// Apply the character to the destination
			if ((getFlag(pixel.attributes, PixelAttributes::Char) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::Char)) {
				if (!noPixel)
					output.character = pixel.character;
				if (!noAttributes)
					setFlag(output.attributes, PixelAttributes::Char, inverse != getFlag(pixel.attributes, PixelAttributes::Char));
			}

			//============ COLOR =============

			// Apply the foreground color to the destination
			if ((getFlag(pixel.attributes, PixelAttributes::FColor) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::FColor)) {
				if (!noPixel)
					output.color = setFColor(output.color, pixel.color);
				if (!noAttributes)
					setFlag(output.attributes, PixelAttributes::FColor, inverse != getFlag(pixel.attributes, PixelAttributes::FColor));
			}
			// Apply the background color to the destination
			if ((getFlag(pixel.attributes, PixelAttributes::BColor) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::BColor)) {
				if (!noPixel)
					output.color = setBColor(output.color, pixel.color);
				if (!noAttributes)
					setFlag(output.attributes, PixelAttributes::BColor, inverse != getFlag(pixel.attributes, PixelAttributes::BColor));
			}

			//============ INVERT ============

			// Apply the foreground invert to the destination
			if ((getFlag(pixel.attributes, PixelAttributes::FInvert) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::FInvert)) {
				if (!noPixel && !copySpecial)
					output.color = invertFColor(output.color);
				else if (!noAttributes && copySpecial)
					setFlag(output.attributes, PixelAttributes::FInvert, inverse != getFlag(pixel.attributes, PixelAttributes::FInvert));
			}
			// Apply the background invert to the destination
			if ((getFlag(pixel.attributes, PixelAttributes::BInvert) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::BInvert)) {
				if (!noPixel && !copySpecial)
					output.color = invertBColor(output.color);
				else if (!noAttributes && copySpecial)
					setFlag(output.attributes, PixelAttributes::BInvert, inverse != getFlag(pixel.attributes, PixelAttributes::BInvert));
			}

			//=========== LIGHTEN ============

			// Apply the foreground lighten to the destination
			if (((getFlag(pixel.attributes, PixelAttributes::FLighten) &&
				!getFlag(pixel.attributes, PixelAttributes::FDarken)) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::FLighten)) {
				if (!noPixel && !copySpecial)
					output.color = lightenFColor(output.color);
				else if (!noAttributes && copySpecial)
					setFlag(output.attributes, PixelAttributes::FLighten, inverse != getFlag(pixel.attributes, PixelAttributes::FLighten));
			}
			// Apply the background lighten to the destination
			if (((getFlag(pixel.attributes, PixelAttributes::BLighten) &&
				!getFlag(pixel.attributes, PixelAttributes::BDarken)) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::BLighten)) {
				if (!noPixel && !copySpecial)
					output.color = lightenBColor(output.color);
				else if (!noAttributes && copySpecial)
					setFlag(output.attributes, PixelAttributes::BLighten, inverse != getFlag(pixel.attributes, PixelAttributes::BLighten));
			}

			//============ DARKEN ============

			// Apply the foreground darken to the destination
			if (((getFlag(pixel.attributes, PixelAttributes::FDarken) &&
				!getFlag(pixel.attributes, PixelAttributes::FLighten)) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::FDarken)) {
				if (!noPixel && !copySpecial)
					output.color = darkenFColor(output.color);
				else if (!noAttributes && copySpecial)
					setFlag(output.attributes, PixelAttributes::FDarken, inverse != getFlag(pixel.attributes, PixelAttributes::FDarken));
			}
			// Apply the background darken to the destination
			if (((getFlag(pixel.attributes, PixelAttributes::BDarken) &&
				!getFlag(pixel.attributes, PixelAttributes::BLighten)) != opposite || replace) &&
				getFlag(options.modAttributes, PixelAttributes::FDarken)) {
				if (!noPixel && !copySpecial)
					output.color = darkenBColor(output.color);
				else if (!noAttributes && copySpecial)
					setFlag(output.attributes, PixelAttributes::BDarken, inverse != getFlag(pixel.attributes, PixelAttributes::BDarken));
			}
		}
	}
}
void Graphics::setPixel(Point2I point, Point2I size, Pixel pixel) {
	setPixel(Rectangle2I(point, size), pixel);
}
void Graphics::setPixel(Rectangle2I rect, Pixel pixel) {
	Point2I p;
	for (p.x = std::max(0, rect.point.x); p.x < rect.point.x + rect.size.x; p.x++) {
		for (p.y = std::max(0, rect.point.y); p.y < rect.point.y + rect.size.y; p.y++) {
			setPixel(p, pixel);
		}
	}
}

// Fill
void Graphics::clear() {
	Point2I p;
	for (p.x = rpoint.x - tpoint.x; p.x < rpoint.x - tpoint.x + rsize.x; p.x++) {
		if (!xBoundsCheck(p.x)) continue;
		for (p.y = rpoint.y - tpoint.y; p.y < rpoint.y - tpoint.y + rsize.y; p.y++) {
			if (!yBoundsCheck(p.y)) continue;
			toPixel(p) = frame->background;
		}
	}
}
void Graphics::fill(Pixel pixel) {
	Point2I p;
	for (p.x = rpoint.x - tpoint.x; p.x < rpoint.x - tpoint.x + rsize.x; p.x++) {
		if (!xBoundsCheck(p.x)) continue;
		for (p.y = rpoint.y - tpoint.y; p.y < rpoint.y - tpoint.y + rsize.y; p.y++) {
			if (!yBoundsCheck(p.y)) continue;
			toPixel(p) = pixel;
		}
	}
}

// Advanced Drawing
void Graphics::drawLine(Point2I point, Point2I size, Pixel pixel, bool asEnd) {
	if (asEnd)
		size -= point - Point2I::ONE;

	// Bresenham's algorithm
	//http://www.brackeen.com/vga/shapes.html#shapes3.0

	// Absolute distance
	Point2I absd = GMath::abs(size);
	// Sign of the distance
	Point2I inc = GMath::sign(size);

	setPixel(point, pixel);

	if (absd.x >= absd.y) {
		int D = 2 * absd.y - absd.x;
		for (Point2I p = point + Point2I(inc.x, 0); p.x != point.x + size.x + inc.x; p.x += inc.x) {
			if (D > 0) {
				p.y += inc.y;
				D += 2 * absd.y - 2 * absd.x;
			}
			else {
				D += 2 * absd.y;
			}
			setPixel(p, pixel);
		}
	}
	else {
		int D = 2 * absd.x - absd.y;
		for (Point2I p = point + Point2I(0, inc.y); p.y != point.y + size.y + inc.y; p.y += inc.y) {
			if (D > 0) {
				p.x += inc.x;
				D += 2 * absd.x - 2 * absd.y;
			}
			else {
				D += 2 * absd.x;
			}
			setPixel(p, pixel);
		}
	}
}
void Graphics::drawRectangle(Point2I point, Point2I size, Pixel pixel, bool fill, bool asEnd) {
	drawRectangle(Rectangle2I(point, (asEnd ? size - point - Point2I::ONE : size)), pixel, fill);
}
void Graphics::drawRectangle(Rectangle2I rect, Pixel pixel, bool fill) {
	rect = rect.absRect();

	if (fill) {
		setPixel(rect, pixel);
	}
	else {
		setPixel(rect.point, Point2I(rect.size.x, 1), pixel);
		setPixel(rect.point + Point2I(0, 1), Point2I(1, rect.size.y - 2), pixel);
		if (rect.size.y > 1)
			setPixel(rect.point + Point2I(0, rect.size.y - 1), Point2I(rect.size.x, 1), pixel);
		if (rect.size.x > 1)
			setPixel(rect.point + Point2I(rect.size.x - 1, 1), Point2I(1, rect.size.y - 2), pixel);
	}
}
void Graphics::drawEllipse(Point2I point, Point2I size, Pixel pixel, bool fill, bool asOrigin) {
	if (asOrigin) {
		point -= size;
		size *= 2;
	}
	Rectangle2I rect = Rectangle2I(point, size).absRect();
	point = rect.point;
	size = rect.size;
	
	
	// Taken from source code of jsgraphics.
	// http://www.walterzorn.de/en/jsgraphics/jsgraphics_e.htm

	// At least we can fix these circles issues since they're pretty simple
	if (size.x == 2 || size.y == 2) {
		setPixel(point, size, pixel);
		return;
	}

	Point2I radius = size / 2;
	Point2I odd = GMath::odd(size);
	Point2I center = point + radius;
	Point2I p = Point2I(0, radius.y);
	Point2I o = Point2I(0, radius.y);
	Point2I rr2 = Point2I((radius.x*radius.x) * 2, (radius.y*radius.y) * 2);
	Point2I rr4 = rr2 * 2;
	int st = (rr2.x / 2) * (1 - (radius.y * 2)) + rr2.y;
	int tt = (rr2.y / 2) - rr2.x * ((radius.y * 2) - 1);
	Point2I lineSize;

	while (p.y > 0) {
		if (st < 0) {
			st += rr2.y * ((p.x * 2) + 3);
			tt += rr4.y * (++p.x);
		}
		else if (tt < 0) {
			st += rr2.y * ((p.x * 2) + 3) - rr4.x * (p.y - 1);
			tt += rr4.y * (++p.x) - rr2.x * (((p.y--) * 2) - 3);
			lineSize = Point2I(p.x - o.x, o.y - p.y);
			if ((lineSize.x & 2) && (lineSize.y & 2)) {
				ellipseHelper(center, Point2I(p.x - 2, p.y + 2), Point2I::ONE, odd, pixel, fill);
				ellipseHelper(center, Point2I(p.x - 1, p.y + 1), Point2I::ONE, odd, pixel, fill);
			}
			else {
				ellipseHelper(center, Point2I(p.x - 1, o.y), lineSize, odd, pixel, fill);
			}
			o = p;
		}
		else {
			tt -= rr2.x * ((p.y * 2) - 3);
			st -= rr4.x * (--p.y);
		}
	}
	lineSize = Point2I(radius.x - o.x + 1, (o.y * 2) + odd.y);
	p.y = center.y - o.y;
	setPixel(Point2I(center.x - radius.x, p.y), lineSize, pixel);
	setPixel(Point2I(center.x + o.x + odd.x - 1, p.y), lineSize, pixel);
}
void Graphics::ellipseHelper(Point2I center, Point2I point, Point2I size, Point2I odd, Pixel pixel, bool fill) {
	Point2I topLeft = center - point;
	Point2I bottomRight = center + point + odd - size;
	if (bottomRight.x > topLeft.x + size.x) {
		if (fill) {
			setPixel(Point2I(bottomRight.x, topLeft.y), bottomRight + size - Point2I(bottomRight.x, topLeft.y), pixel);
		}
		else {
			setPixel(Point2I(bottomRight.x, topLeft.y), size, pixel);
			setPixel(bottomRight, size, pixel);
		}
	}
	else {
		size.x += bottomRight.x - topLeft.x;
	}
	if (fill) {
		setPixel(topLeft, Point2I(topLeft.x, bottomRight.y) + size - topLeft, pixel);
	}
	else {
		setPixel(topLeft, size, pixel);
		setPixel(Point2I(topLeft.x, bottomRight.y), size, pixel);
	}
}
void Graphics::drawFloodFill(Point2I point, Pixel pixel, bool specific) {
	// East West algorithm
	//http://en.wikipedia.org/wiki/Flood_fill

	if (!boundsCheck(point))
		return;
	Pixel target = toPixel(point);

	if (pixel.matches(target, specific))
		return;

	toPixel(point) = target;

	std::queue<Point2I> nodes;
	nodes.push(point);
	while (!nodes.empty()) {
		point = nodes.front();
		if (toPixel(point).matches(target, specific)) {
			int width = 1;
			// Travel as far left as possible
			for (; leftBoundsCheck(point.x - 1) && toPixelxy(point.x - 1, point.y).matches(target, specific); point.x--, width++);
			// Travel as far right as possible
			for (; rightBoundsCheck(point.x + width) && toPixelxy(point.x + width, point.y).matches(target, specific); width++);

			for (int i = 0; i < width; i++)
				toPixelxy(point.x + i, point.y) = pixel;

			// Then continue makes sure that only one node is added for every
			// range of pixels in a row
			bool northcontinue = false, southcontinue = false;
			for (int i = 0; i < width; ++i) {
				if (yBoundsCheck(point.y - 1)) {
					if (toPixelxy(point.x + i, point.y - 1).matches(target, specific)) {
						if (!northcontinue) {
							nodes.push(point + Point2I(i, -1));
							northcontinue = true;
						}
					}
					else {
						northcontinue = false;
					}
				}
				if (yBoundsCheck(point.y + 1)) {
					if (toPixelxy(point.x + i, point.y + 1).matches(target, specific)) {
						if (!southcontinue) {
							nodes.push(point + Point2I(i, 1));
							southcontinue = true;
						}
					}
					else {
						southcontinue = false;
					}
				}
			}

		}
		nodes.pop();
	}
}
void Graphics::drawReplace(Point2I point, Pixel pixel, bool specific) {
	if (!boundsCheck(point))
		return;

	Pixel target = toPixel(point);
	drawReplace(target, pixel, specific);
}
void Graphics::drawReplace(Pixel target, Pixel pixel, bool specific) {
	if (specific && target.matches(pixel))
		return;

	Point2I p;
	for (p.x = rpoint.x - tpoint.x; p.x < rpoint.x - tpoint.x + rsize.x; p.x++) {
		if (!xBoundsCheck(p.x)) continue;
		for (p.y = rpoint.y - tpoint.y; p.y < rpoint.y - tpoint.y + rsize.y; p.y++) {
			if (!yBoundsCheck(p.y)) continue;
			if (toPixel(p).matches(target, specific))
				toPixel(p) = pixel;
		}
	}
}
void Graphics::drawString(Point2I point, const std::string& text, Pixel pixel, HorizontalAlignments alignment) {
	if (alignment == HorizontalAlignments::Center || alignment == HorizontalAlignments::Stretch)
		point.x -= (int)text.length() / 2;
	else if (alignment == HorizontalAlignments::Right)
		point.x -= (int)text.length() - 1;

	for (int x = 0; x < (int)text.length(); x++) {
		setPixel(point + Point2I(x, 0), Pixel(text[x], pixel.color, pixel.attributes));
	}
}
void Graphics::drawString(Point2I point, const std::string& text, Pixel pixel, HorizontalAlignments alignment, int width) {
	if (alignment == HorizontalAlignments::Center || alignment == HorizontalAlignments::Stretch)
		point.x += (width - (int)text.length()) / 2;
	else if (alignment == HorizontalAlignments::Right)
		point.x += width - (int)text.length();

	for (int x = 0; x < (int)text.length(); x++) {
		setPixel(point + Point2I(x, 0), Pixel(text[x], pixel.color, pixel.attributes));
	}
}
void Graphics::drawImage(Point2I point, const AsciiImageFrame& image) {
	Point2I p;
	for (p.y = std::max(0, -point.y); (p.y < image.size().y); p.y++) {
		for (p.x = std::max(0, -point.x); (p.x < image.size().x); p.x++) {
			setPixel(point + p, image.pixels[pointToIndex(p, image.frameSize.x)]);
		}
	}
}
void Graphics::drawImage(Point2I point, Point2I part, Point2I partSize, const AsciiImageFrame& image) {
	drawImage(point, Rectangle2I(part, partSize), image);
}
void Graphics::drawImage(Point2I point, Rectangle2I region, const AsciiImageFrame& image) {
	region = region.absRect();

	Point2I p;
	for (p.y = std::max(0, -point.y); (p.y < region.size.y) && (p.y + region.point.y < image.size().y); p.y++) {
		if (p.y + region.point.y < 0) continue;
		for (p.x = std::max(0, -point.x); (p.x < region.size.x) && (p.x + region.point.x < image.size().x); p.x++) {
			if (p.x + region.point.x < 0) continue;
			setPixel(point + p, image.pixels[pointToIndex(p + region.point, image.frameSize.x)]);
		}
	}
}

// Borders
void Graphics::drawBorder(Rectangle2I rect, Pixel pixel, bool thick, BorderThickness connections, ConnectionPriorities priorities) {
	drawBorder(rect, pixel, thick, DisabledBorders(), connections, priorities);
}
void Graphics::drawBorder(Rectangle2I rect, Pixel pixel, bool thick, DisabledBorders disabledBorders, BorderThickness connections, ConnectionPriorities priorities) {
	unsigned char t = (thick ? 2 : 1);
	unsigned char dli = disabledBorders.left   || disabledBorders.inside  ? 3 : 0;	// Disabled left inside
	unsigned char dti = disabledBorders.top    || disabledBorders.inside  ? 3 : 0;	// Disabled top inside
	unsigned char dri = disabledBorders.right  || disabledBorders.inside  ? 3 : 0;	// Disabled right inside
	unsigned char dbi = disabledBorders.bottom || disabledBorders.inside  ? 3 : 0;	// Disabled bottom inside
	unsigned char dlo = disabledBorders.left   || disabledBorders.outside ? 3 : 0;	// Disabled left outside
	unsigned char dto = disabledBorders.top    || disabledBorders.outside ? 3 : 0;	// Disabled top outside
	unsigned char dro = disabledBorders.right  || disabledBorders.outside ? 3 : 0;	// Disabled right outside
	unsigned char dbo = disabledBorders.bottom || disabledBorders.outside ? 3 : 0;	// Disabled bottom outside
	if (rect.size.x != 0 && rect.size.y != 0) {
		if (rect.size.y == 1) {
			if (rect.size.x == 1) {
				// Draw a single point
				setBorderPixel(rect.point, dlo, dto, dro, dbo, pixel, connections, priorities);
			}
			else {
				// Draw a horizontal line
				// Endpoints
				setBorderPixel(rect.point, dlo, dto, t, dbo, pixel, connections, priorities);
				setBorderPixel(rect.point + Point2I(rect.size.x - 1, 0), t, dto, dro, dbo, pixel, connections, priorities);

				// Line
				for (int x = 1; x < rect.size.x - 1; x++) {
					setBorderPixel(rect.point + Point2I(x, 0), t, dto, t, dbo, pixel, connections, priorities);
				}
			}
		}
		else if (rect.size.x == 1) {
			// Draw a vertical line
			// Endpoints
			setBorderPixel(rect.point, dlo, dto, dro, t, pixel, connections, priorities);
			setBorderPixel(rect.point + Point2I(0, rect.size.y - 1), dlo, t, dro, dbo, pixel, connections, priorities);

			// Line
			for (int y = 1; y < rect.size.y - 1; y++) {
				setBorderPixel(rect.point + Point2I(0, y), dlo, t, dro, t, pixel, connections, priorities);
			}
		}
		else {
			// Draw a rectangle
			// Corners
			setBorderPixel(rect.point, dlo, dto, t, t, pixel, connections, priorities);
			setBorderPixel(rect.point + Point2I(rect.size.x - 1, 0), t, dto, dro, t, pixel, connections, priorities);
			setBorderPixel(rect.point + Point2I(0, rect.size.y - 1), dlo, t, t, dbo, pixel, connections, priorities);
			setBorderPixel(rect.point + rect.size - 1, t, t, dro, dbo, pixel, connections, priorities);

			// Lines
			for (int x = 1; x < rect.size.x - 1; x++) {
				setBorderPixel(rect.point + Point2I(x, 0), t, dto, t, dbi, pixel, connections, priorities);
				setBorderPixel(rect.point + Point2I(x, rect.size.y - 1), t, dti, t, dbo, pixel, connections, priorities);
			}
			for (int y = 1; y < rect.size.y - 1; y++) {
				setBorderPixel(rect.point + Point2I(0, y), dlo, t, dri, t, pixel, connections, priorities);
				setBorderPixel(rect.point + Point2I(rect.size.x - 1, y), dli, t, dro, t, pixel, connections, priorities);
			}
		}
	}
}
void Graphics::setBorderPixel(Point2I point, unsigned char left, unsigned char top, unsigned char right, unsigned char bottom, Pixel pixel, BorderThickness connections, ConnectionPriorities priorities) {
	#define getch(px, py) ( \
			(tpoint.x + px >= 0 && tpoint.x + px < fsize.x && \
			tpoint.y + py >= 0 && tpoint.y + py < fsize.y) ? \
			frame->pixels[xyToIndex(tpoint.x + px, tpoint.y + py, fsize.x)].character : '\0')
	#define getch2(px, py) ( \
			(tpoint.x + px >= 0 && tpoint.x + px < fsize.x && \
			tpoint.y + py >= 0 && tpoint.y + py < fsize.y && \
			tpoint.x + px >= rpoint.x && tpoint.x + px < rpoint.x + rsize.x && \
			tpoint.y + py >= rpoint.y && tpoint.y + py < rpoint.y + rsize.y) ? \
			frame->pixels[xyToIndex(tpoint.x + px, tpoint.y + py, fsize.x)].character : '\0')

	Thickness outside;
	
	if (options.connectGlobalBorders) {
		outside = Thickness(
			Borders::getBorderConnection(getch(point.x - 1, point.y)).right,
			Borders::getBorderConnection(getch(point.x, point.y - 1)).bottom,
			Borders::getBorderConnection(getch(point.x + 1, point.y)).left,
			Borders::getBorderConnection(getch(point.x, point.y + 1)).top
		);
	}
	else {
		outside = Thickness(
			Borders::getBorderConnection(getch2(point.x - 1, point.y)).right,
			Borders::getBorderConnection(getch2(point.x, point.y - 1)).bottom,
			Borders::getBorderConnection(getch2(point.x + 1, point.y)).left,
			Borders::getBorderConnection(getch2(point.x, point.y + 1)).top
		);
	}

	unsigned char borderChar = Borders::getBestConnection(
		left   != 0 ? left   : (getByteFlag(connections, outside.left)   ? outside.left   : 0),
		top    != 0 ? top    : (getByteFlag(connections, outside.top)    ? outside.top    : 0),
		right  != 0 ? right  : (getByteFlag(connections, outside.right)  ? outside.right  : 0),
		bottom != 0 ? bottom : (getByteFlag(connections, outside.bottom) ? outside.bottom : 0),
		true, priorities
	);

	if (borderChar != '\0') {
		setPixel(point, Pixel(borderChar, pixel.color, pixel.attributes));
	}

	#undef getch
	#undef getch2
}
void Graphics::forceBorderPixel(Point2I point, unsigned char left, unsigned char top, unsigned char right, unsigned char bottom, Pixel pixel, BorderThickness connections, ConnectionPriorities priorities) {
	#define getch(px, py) ( \
			(tpoint.x + px >= 0 && tpoint.x + px < fsize.x && \
			tpoint.y + py >= 0 && tpoint.y + py < fsize.y) ? \
			frame->pixels[xyToIndex(tpoint.x + px, tpoint.y + py, fsize.x)].character : '\0')
	#define getch2(px, py) ( \
			(tpoint.x + px >= 0 && tpoint.x + px < fsize.x && \
			tpoint.y + py >= 0 && tpoint.y + py < fsize.y && \
			tpoint.x + px >= rpoint.x && tpoint.x + px < rpoint.x + rsize.x && \
			tpoint.y + py >= rpoint.y && tpoint.y + py < rpoint.y + rsize.y) ? \
			frame->pixels[xyToIndex(tpoint.x + px, tpoint.y + py, fsize.x)].character : '\0')

	Thickness outside;
	unsigned char centerChar;

	if (options.connectGlobalBorders) {
		outside = Thickness(
			Borders::getBorderConnection(getch(point.x - 1, point.y)).right,
			Borders::getBorderConnection(getch(point.x, point.y - 1)).bottom,
			Borders::getBorderConnection(getch(point.x + 1, point.y)).left,
			Borders::getBorderConnection(getch(point.x, point.y + 1)).top
		);
		centerChar = Borders::getBorderConnection(getch(point.x, point.y)).character;
	}
	else {
		outside = Thickness(
			Borders::getBorderConnection(getch2(point.x - 1, point.y)).right,
			Borders::getBorderConnection(getch2(point.x, point.y - 1)).bottom,
			Borders::getBorderConnection(getch2(point.x + 1, point.y)).left,
			Borders::getBorderConnection(getch2(point.x, point.y + 1)).top
		);
		centerChar = Borders::getBorderConnection(getch2(point.x, point.y)).character;
	}

	unsigned char borderChar = Borders::getBestConnection(
		left != 0 ? left : (getByteFlag(connections, outside.left) ? outside.left : 0),
		top != 0 ? top : (getByteFlag(connections, outside.top) ? outside.top : 0),
		right != 0 ? right : (getByteFlag(connections, outside.right) ? outside.right : 0),
		bottom != 0 ? bottom : (getByteFlag(connections, outside.bottom) ? outside.bottom : 0),
		true, priorities
	);

	if (borderChar == '\0' && centerChar != '\0') {
		borderChar = ' ';
	}
	if (borderChar != '\0') {
		setPixel(point, Pixel(borderChar, pixel.color, pixel.attributes));
	}

	#undef getch
	#undef getch2
}

// Transformation
void Graphics::translate(Point2I distance) {
	if (!optionsStack.empty())
		throw std::exception("'Graphics::translate(...)' can only be done on the root region");

	frame->translate(distance);
}
void Graphics::rotate(int rotations) {
	if (!optionsStack.empty())
		throw std::exception("'Graphics::rotate(...)' can only be done on the root region");

	frame->rotate(rotations);
}
void Graphics::flip(Orientations axis) {
	if (!optionsStack.empty())
		throw std::exception("'Graphics::flip(...)' can only be done on the root region");

	frame->flip(axis);
}

#pragma endregion
//=================================================================|
