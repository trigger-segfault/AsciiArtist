/*=================================================================|
|* File:				AsciiImage.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AsciiImage.h"
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/IO/ResourceStream.h>
#include <PowerConsole/Drawing/Graphics.h>
#include <PowerConsole/Helpers/Flags.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
using namespace PowerConsole::Helpers;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

AsciiImage::AsciiImage()
	: AsciiImageFrame() {}
AsciiImage::AsciiImage(Point2I size, ImageFormats format)
	: AsciiImageFrame(size, format) {}
AsciiImage::AsciiImage(Point2I size, ImageFormats format, Pixel background)
	: AsciiImageFrame(size, format, background) {}
AsciiImage::AsciiImage(ImageFormats format, Pixel background, const AsciiImageFrame& image)
	: AsciiImageFrame(format, background, image) {}
AsciiImage::AsciiImage(ImageFormats format, Pixel background, AsciiImageFrame&& image)
	: AsciiImageFrame(format, background, std::move(image)) {
	name = image.getName();
}
AsciiImage::AsciiImage(Rectangle2I region, ImageFormats format, Pixel background, const AsciiImageFrame& image)
	: AsciiImageFrame(region,format, background, image) {}
AsciiImage::AsciiImage(const AsciiImageFrame& image)
	: AsciiImageFrame(image) {
	name = image.getName();
}
AsciiImage::AsciiImage(AsciiImageFrame&& image)
	: AsciiImageFrame(std::move(image)) {
	name = image.getName();
}
AsciiImage::AsciiImage(const AsciiImage& image)
	: AsciiImageFrame(image) {
	name = image.getName();
}
AsciiImage::AsciiImage(AsciiImage&& image)
	: AsciiImageFrame(std::move(image)) {
	name = image.getName();
}
AsciiImage::~AsciiImage() {

}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

AsciiImage& AsciiImage::operator=(const AsciiImageFrame& image) {
	if (this != &image) {
		AsciiImageFrame::operator=(image);
		name = image.getName();
	}
	return *this;
}
AsciiImage& AsciiImage::operator=(AsciiImageFrame&& image) {
	if (this != &image) {
		AsciiImageFrame::operator=(std::move(image));
		name = image.getName();
	}
	return *this;
}
AsciiImage& AsciiImage::operator=(const AsciiImage& image) {
	if (this != &image) {
		AsciiImageFrame::operator=(image);
		name = image.getName();
	}
	return *this;
}
AsciiImage& AsciiImage::operator=(AsciiImage&& image) {
	if (this != &image) {
		AsciiImageFrame::operator=(std::move(image));
		name = image.getName();
	}
	return *this;
}

#pragma endregion
//========= INPUT/OUTPUT =========
#pragma region Input/Output

// Input
bool AsciiImage::load(InputStream* in) {
	AsciiImageHeader header = AsciiImageHeader();
	if (AsciiImageInfo::loadHeader(in, header, true)) {
		in->setPointer(header.fileStart + header.frameList[0].frameOffset + header.frameHeaderSize, false);
		bool success = AsciiImageFrame::load(in, header, header.frameList[0]);
		if (!success) {
			*this = AsciiImage();
		}
		return success;
	}
	return false;
}
bool AsciiImage::loadFile(const std::string& path) {
	FileInputStream in = FileInputStream();
	if (in.open(path)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool AsciiImage::loadResource(const std::string& name, const std::string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(name, type)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool AsciiImage::loadResource(const std::string& name, unsigned int typeId) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(name, typeId)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool AsciiImage::loadResource(unsigned int id, const std::string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(id, type)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;

}
bool AsciiImage::loadResource(unsigned int id, unsigned int typeId) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(id, typeId)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;

}
bool AsciiImage::loadClipboard() {
	if (Clipboard::containsFormat(AsciiImageInfo::CLIPBOARD_FORMAT)) {
		ClipboardInputStream in = ClipboardInputStream();
		if (in.open()) {
			bool success = false;
			if (in.getData(AsciiImageInfo::CLIPBOARD_FORMAT)) {
				success = load(&in);
			}
			in.close();
			return success;
		}
	}
	else if (Clipboard::containsFileDrop()) {
		ClipboardInputStream in = ClipboardInputStream();
		if (in.open()) {
			std::string firstFile = Clipboard::getFileDropNames()[0];
			in.close();
			if (AsciiImageInfo::isFileAsciiImage(firstFile)) {
				return loadFile(firstFile) && !in.hasError();
			}
		}
	}
	return false;
}

// Output
bool AsciiImage::save(OutputStream* out, unsigned short version, bool smallFile) const {
	AsciiImageHeader header = AsciiImageInfo::createHeader(*this, version, smallFile);

	if (AsciiImageInfo::saveHeader(out, header, true)) {
		out->setPointer(header.fileStart + header.frameList[0].frameOffset + header.frameHeaderSize, false);
		return AsciiImageFrame::save(out, header, header.frameList[0]);
	}
	return false;
}
bool AsciiImage::saveFile(const std::string& path, unsigned short version, bool smallFile) const {
	FileOutputStream out = FileOutputStream();
	if (out.open(path)) {
		bool success = save(&out, version, smallFile);
		out.close();
		return success;
	}
	return false;
}
bool AsciiImage::saveClipboard(unsigned short version, bool smallFile) const {
	ClipboardOutputStream out = ClipboardOutputStream();
	if (out.open()) {
		bool success = false;
		if (save(&out, version, smallFile)) {
			success = out.setData(AsciiImageInfo::CLIPBOARD_FORMAT);
		}
		out.close();
		return success;
	}
	return false;
}

#pragma endregion
//========= MODIFICATION =========
#pragma region Modification

void AsciiImage::resize(Point2I size) {
	AsciiImageFrame::resize(size);
}
void AsciiImage::rotate(int rotations) {
	AsciiImageFrame::rotate(rotations);
}
void AsciiImage::rotateAndResize(int rotations) {
	AsciiImageFrame::rotateAndResize(rotations);
}
void AsciiImage::translate(Point2I distance) {
	AsciiImageFrame::translate(distance);
}
void AsciiImage::flip(Orientations axis) {
	AsciiImageFrame::flip(axis);
}
void AsciiImage::clear() {
	AsciiImageFrame::clear();
}
GraphicsSPtr AsciiImage::createGraphics() {
	return std::shared_ptr<Graphics>(new Graphics(this));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void AsciiImage::setBackground(Pixel background) {
	this->background = background;
	if (!getFlag(format, ImageFormats::Attributes))
		this->background.attributes = PixelAttributes::Fill;
}
void AsciiImage::setFormat(ImageFormats format) {
	AsciiImageFrame::setFormat(format);
}
void AsciiImage::setFrameSpeed(unsigned int frameSpeed) {
	this->frameSpeed = frameSpeed;
}
void AsciiImage::setName(const std::string& name) {
	this->name = name;
}

#pragma endregion
//=================================================================|
