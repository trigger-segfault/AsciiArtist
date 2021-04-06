/*=================================================================|
|* File:				AsciiImageFrame.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AsciiImageFrame.h"
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

AsciiImageFrame::AsciiImageFrame()
	: AsciiImageFrame(Point2I::ONE, ImageFormats::Basic, Pixel()) {}
AsciiImageFrame::AsciiImageFrame(Point2I size, ImageFormats format)
	: AsciiImageFrame(size, format, Pixel()) {}
AsciiImageFrame::AsciiImageFrame(Point2I size, ImageFormats format, Pixel background) {
	this->frameSize = size;
	this->format = format;
	this->background = background;
	this->frameSpeed = AsciiImageInfo::DEFAULT_ANIMATION_SPEED;
	this->name = "";

	this->pixels = new Pixel[this->frameSize.x * this->frameSize.y];
	for (int i = 0; i < this->frameSize.x * this->frameSize.y; i++) {
		this->pixels[i] = this->background;
	}

	// Make sure the image follows the format
	if (!getFlag(this->format, ImageFormats::Attributes)) {
		this->background.attributes = PixelAttributes::Fill;
	}
}
AsciiImageFrame::AsciiImageFrame(ImageFormats format, Pixel background, const AsciiImageFrame& image) {
	this->frameSize = image.frameSize;
	this->format = format;
	this->background = background;
	this->frameSpeed = image.frameSpeed;
	this->name = image.name;

	this->pixels = new Pixel[this->frameSize.x * this->frameSize.y];
	for (int i = 0; i < this->frameSize.x * this->frameSize.y; i++) {
		this->pixels[i] = image.pixels[i];
	}

	// Make sure the image follows the format.
	if (!getFlag(this->format, ImageFormats::Attributes)) {
		this->background.attributes = PixelAttributes::Fill;

		// Only bother with this if the copied image had any attributes in the first place.
		if (getFlag(image.format, ImageFormats::Attributes)) {

			for (int i = 0; i < this->frameSize.x * this->frameSize.y; i++) {
				this->pixels[i].attributes = PixelAttributes::Fill;
			}
		}
	}
}
AsciiImageFrame::AsciiImageFrame(ImageFormats format, Pixel background, AsciiImageFrame&& image) {
	this->frameSize = image.frameSize;
	this->format = format;
	this->background = background;
	this->frameSpeed = image.frameSpeed;
	this->name = image.name;

	this->pixels = image.pixels;
	image.pixels = nullptr;

	// Make sure the image follows the format.
	if (!getFlag(this->format, ImageFormats::Attributes)) {
		this->background.attributes = PixelAttributes::Fill;

		// Only bother with this if the copied image had any attributes in the first place.
		if (getFlag(image.format, ImageFormats::Attributes)) {
			for (int i = 0; i < this->frameSize.x * this->frameSize.y; i++) {
				this->pixels[i].attributes = PixelAttributes::Fill;
			}
		}
	}
}
AsciiImageFrame::AsciiImageFrame(Rectangle2I region, ImageFormats format, Pixel background, const AsciiImageFrame& image) {
	region = region.absRect();

	this->frameSize = region.size;
	this->format = format;
	this->background = background;
	this->frameSpeed = image.frameSpeed;
	this->name = image.name;


	this->pixels = new Pixel[this->frameSize.x * this->frameSize.y];
	for (int y = 0; y < this->frameSize.y; y++) {
		for (int x = 0; x < this->frameSize.x; x++) {
			Point2I p = Point2I(x, y);
			if (p + region.point >= Point2I::ZERO && p + region.point < image.frameSize)
				this->pixels[xyToIndex(x, y, this->frameSize.x)] = image.pixels[pointToIndex(p + region.point, image.frameSize.x)];
			else
				this->pixels[xyToIndex(x, y, this->frameSize.x)] = this->background;
		}
	}

	// Make sure the image follows the format.
	if (!getFlag(this->format, ImageFormats::Attributes)) {
		this->background.attributes = PixelAttributes::Fill;

		// Only bother with this if the copied image had any attributes in the first place.
		if (getFlag(image.format, ImageFormats::Attributes)) {
			for (int i = 0; i < this->frameSize.x * this->frameSize.y; i++) {
				this->pixels[i].attributes = PixelAttributes::Fill;
			}
		}
	}
}
AsciiImageFrame::AsciiImageFrame(const AsciiImageFrame& image) {
	this->frameSize = image.frameSize;
	this->format = image.format;
	this->background = image.background;
	this->frameSpeed = image.frameSpeed;
	this->name = image.name;

	this->pixels = new Pixel[this->frameSize.x * this->frameSize.y];
	for (int i = 0; i < this->frameSize.x * this->frameSize.y; i++) {
		this->pixels[i] = image.pixels[i];
	}
}
AsciiImageFrame::AsciiImageFrame(AsciiImageFrame&& image) {
	this->frameSize = image.frameSize;
	this->format = image.format;
	this->background = image.background;
	this->frameSpeed = image.frameSpeed;
	this->name = image.name;

	this->pixels = image.pixels;
	image.pixels = nullptr;
}
AsciiImageFrame::~AsciiImageFrame() {
	cleanUp();
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

AsciiImageFrame& AsciiImageFrame::operator=(const AsciiImageFrame& image) {
	if (this != &image) {
		if (frameSize == image.frameSize && format == image.format) {
			for (int i = 0; i < this->frameSize.x * this->frameSize.y; i++) {
				this->pixels[i] = image.pixels[i];
			}
			this->background = image.background;
			this->frameSpeed = image.frameSpeed;
			this->name = image.name;
		}
		else {
			cleanUp();

			this->frameSize = image.frameSize;
			this->format = image.format;
			this->background = image.background;
			this->frameSpeed = image.frameSpeed;
			this->name = image.name;

			this->pixels = new Pixel[this->frameSize.x * this->frameSize.y];
			for (int i = 0; i < this->frameSize.x * this->frameSize.y; i++) {
				this->pixels[i] = image.pixels[i];
			}
		}
	}
	return *this;
}
AsciiImageFrame& AsciiImageFrame::operator=(AsciiImageFrame&& image) {
	if (this != &image) {
		cleanUp();

		this->frameSize = image.frameSize;
		this->format = format;
		this->background = background;
		this->frameSpeed = image.frameSpeed;
		this->name = image.name;

		this->pixels = image.pixels;
		image.pixels = nullptr;
	}
	return *this;
}

#pragma endregion
//========== PROTECTED ===========
#pragma region Protected

void AsciiImageFrame::cleanUp() {
	if (pixels != nullptr) {
		delete[] pixels;
	}
}
void AsciiImageFrame::resize(Point2I size) {
	if (frameSize != size && size >= Point2I::ONE && size <= AsciiImageInfo::MAX_SIZE) {
		Pixel* newPixels = new Pixel[size.x * size.y];
		for (int y = 0; y < size.y; y++) {
			for (int x = 0; x < size.x; x++) {
				if (Point2I(x, y) < frameSize)
					newPixels[xyToIndex(x, y, size.x)] = pixels[xyToIndex(x, y, frameSize.x)];
				else
					newPixels[xyToIndex(x, y, size.x)] = background;
			}
		}

		cleanUp();
		frameSize = size;
		pixels = newPixels;
	}
}
void AsciiImageFrame::rotate(int rotations) {
	rotations = GMath::wrap(rotations, 4);
	if (rotations != 0) {
		Pixel* newPixels = new Pixel[frameSize.x * frameSize.y];
		for (int y = 0; y < frameSize.y; y++) {
			for (int x = 0; x < frameSize.x; x++) {
				Point2I newPoint = Point2I(x, y);
				// Rotate the new point the correct amount of times.
				for (int k = 0; k < rotations; k++)
					Point2I(newPoint.y, frameSize.x - newPoint.x - 1);

				if (newPoint >= Point2I::ZERO && newPoint < frameSize)
					newPixels[xyToIndex(x, y, frameSize.x)] = pixels[pointToIndex(newPoint, frameSize.x)];
				else
					newPixels[xyToIndex(x, y, frameSize.x)] = background;
			}
		}
	}
}
void AsciiImageFrame::rotateAndResize(int rotations) {
	rotations = GMath::wrap(rotations, 4);
	if (rotations != 0) {
		if (frameSize.x == frameSize.y || rotations != 2) {
			// We can do a normal rotate.
			rotate(rotations);
		}
		else {
			// The more difficult way
			Pixel* newPixels = new Pixel[frameSize.x * frameSize.y];
			for (int x = 0; x < frameSize.y; x++) {
				for (int y = 0; y < frameSize.x; y++) {
					Point2I newPoint = Point2I(x, y);
					// Rotate the new point the correct amount of times.
					for (int k = 0; k < rotations; k++)
						Point2I(newPoint.y, frameSize.y - newPoint.x - 1);

					if (newPoint >= Point2I::ZERO && newPoint < frameSize)
						newPixels[xyToIndex(x, y, frameSize.y)] = pixels[pointToIndex(newPoint, frameSize.x)];
					else
						newPixels[xyToIndex(x, y, frameSize.y)] = background;
				}
			}
		}
	}
}
void AsciiImageFrame::translate(Point2I distance) {
	// Determine which direction we should iterate in for the x and y.
	bool xp = (distance.x < 0);
	bool yp = (distance.y < 0);
	for (int y = (yp ? 0 : frameSize.y - 1); (yp ? y < frameSize.y : y >= 0); (yp ? y++ : y--)) {
		for (int x = (xp ? 0 : frameSize.x - 1); (xp ? x < frameSize.x : x >= 0); (xp ? x++ : x--)) {
			Point2I newPoint = Point2I(x, y) - distance;

			if (newPoint >= Point2I::ZERO && newPoint < frameSize)
				pixels[xyToIndex(x, y, frameSize.x)] = pixels[pointToIndex(newPoint, frameSize.x)];
			else
				pixels[xyToIndex(x, y, frameSize.x)] = background;
		}
	}
}
void AsciiImageFrame::flip(Orientations axis) {
	Pixel* newPixels = new Pixel[frameSize.x * frameSize.y];
	for (int y = 0; y < frameSize.y; y++) {
		for (int x = 0; x < frameSize.x; x++) {
			Point2I newPoint = Point2I(x, y);
			if (axis == Orientations::Vertical)
				newPoint.y = frameSize.y - newPoint.y - 1;
			else
				newPoint.x = frameSize.x - newPoint.x - 1;

			if (newPoint >= Point2I::ZERO && newPoint < frameSize)
				newPixels[xyToIndex(x, y, frameSize.x)] = pixels[pointToIndex(newPoint, frameSize.x)];
			else
				newPixels[xyToIndex(x, y, frameSize.x)] = background;
		}
	}

	cleanUp();
	pixels = newPixels;
}
void AsciiImageFrame::clear() {
	for (int i = 0; i < frameSize.x * frameSize.y; i++) {
		pixels[i] = background;
	}
}
void AsciiImageFrame::setFormat(ImageFormats format) {
	// Make sure the image follows the format.
	if (!getFlag(format, ImageFormats::Attributes)) {
		this->background.attributes = PixelAttributes::Fill;

		// Only bother with this if the copied image had any attributes in the first place.
		if (getFlag(this->format, ImageFormats::Attributes)) {
			for (int i = 0; i < frameSize.x * frameSize.y; i++) {
				pixels[i].attributes = PixelAttributes::Fill;
			}
		}
	}
	this->format = format;
}
bool AsciiImageFrame::load(InputStream* in, const AsciiImageHeader& header, const AsciiImageFrameHeader& frame, bool isLayer) {
	if (in->isOpen() && !in->hasError()) {
		// Load the final layer to draw to.
		Pixel* newPixels = new Pixel[frame.size.x * frame.size.y];

		if (header.bytesPerAttribute == 2 && (header.signature != AsciiImageInfo::FILE_SIGNATURE || header.version >= 3)) {
			in->read(newPixels, frame.size.x * frame.size.y);
			if (header.signature == AsciiImageInfo::FILE_SIGNATURE || header.version == 3) {
				for (int i = 0; i < frame.size.x * frame.size.y; i++) {
					newPixels[i].attributes = PixelAttributes::Fill;
				}
			}
		}
		else {
			for (int i = 0; i < frame.size.x * frame.size.y; i++) {
				newPixels[i] = header.background;
			}
			// Legacy loading:
			// Unlike all other 2D for loops, the IO loops must iterate in the order of for y for x
			// This way the file will be saved and loaded row-by-row like most other image formats.
			for (int y = 0; y < frame.size.y; y++) {
				for (int x = 0; x < frame.size.x; x++) {
					AsciiImageInfo::loadPixel(in, header, newPixels[xyToIndex(x, y, frame.size.x)]);
				}
			}
		}

		// Only overwrite the image once it has been successfully loaded.
		if (!in->hasError()) {
			cleanUp();
			frameSize = frame.size;
			format = header.format;
			frameSpeed = frame.frameSpeed;
			background = header.background;
			name = frame.name;
			pixels = newPixels;
			return true;
		}
	}
	return false;
}
bool AsciiImageFrame::save(OutputStream* out, const AsciiImageHeader& header, const AsciiImageFrameHeader& frame, bool isLayer) const {
	if (out->isOpen() && !out->hasError()) {

		if (header.bytesPerAttribute == 2 && (header.signature != AsciiImageInfo::FILE_SIGNATURE || header.version >= 3)) {
			out->write(pixels, frame.size.x * frame.size.y);
		}
		else {
			// Unlike all other 2D for loops, the IO loops must iterate in the order of for y for x.
			// This way the file will be saved and loaded row-by-row like most other image formats.
			for (int y = 0; y < frameSize.y; y++) {
				for (int x = 0; x < frameSize.x; x++) {
					AsciiImageInfo::savePixel(out, header, pixels[xyToIndex(x, y, frameSize.x)]);
				}
			}
		}

		return !out->hasError();
	}
	return false;
}

#pragma endregion
//============ PIXELS ============
#pragma region Pixels

unsigned char AsciiImageFrame::getChar(Point2I point) const {
	if (point >= Point2I::ZERO && point < frameSize)
		return pixels[pointToIndex(point, frameSize.x)].character;
	return '\0';
}
unsigned char AsciiImageFrame::getColor(Point2I point) const {
	if (point >= Point2I::ZERO && point < frameSize)
		return pixels[pointToIndex(point, frameSize.x)].color;
	return 0x00;
}
PixelAttributes AsciiImageFrame::getAttributes(Point2I point) const {
	if (point >= Point2I::ZERO && point < frameSize)
		return pixels[pointToIndex(point, frameSize.x)].attributes;
	return PixelAttributes::None;
}
Pixel AsciiImageFrame::getPixel(Point2I point) const {
	if (point >= Point2I::ZERO && point < frameSize)
		return pixels[pointToIndex(point, frameSize.x)];
	return Pixel();
}
bool AsciiImageFrame::testAttributes(Point2I point, PixelAttributes attributes, bool all) const {
	if (point >= Point2I::ZERO && point < frameSize) {
		if (all)
			return getFlag(pixels[pointToIndex(point, frameSize.x)].attributes, attributes);
		else
			return (pixels[pointToIndex(point, frameSize.x)].attributes & attributes) != PixelAttributes::None;
	}
	return (attributes == PixelAttributes::None);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

Point2I AsciiImageFrame::size() const {
	return frameSize;
}
Pixel AsciiImageFrame::getBackground() const {
	return background;
}
ImageFormats AsciiImageFrame::getFormat() const {
	return format;
}
unsigned int AsciiImageFrame::getFrameSpeed() const {
	return frameSpeed;
}
std::string AsciiImageFrame::getName() const {
	return name;
}
Pixel*const AsciiImageFrame::getPixels() const {
	return pixels;
}

#pragma endregion

//=================================================================|
