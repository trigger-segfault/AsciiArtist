/*=================================================================|
|* File:				AsciiAnimation.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AsciiAnimation.h"
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/IO/ResourceStream.h>
#include <PowerConsole/Drawing/Graphics.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

AsciiAnimation::AsciiAnimation()
	: AsciiAnimation(Point2I::ONE, ImageFormats::Animation | ImageFormats::Attributes, Pixel(), 1) {}
AsciiAnimation::AsciiAnimation(Point2I size, ImageFormats format, unsigned int frames)
	: AsciiAnimation(size, format,  Pixel(), frames) {}
AsciiAnimation::AsciiAnimation(Point2I size, ImageFormats format, Pixel background, unsigned int frames) {
	this->animationSpeed = 800;
	this->frames.push_back(AsciiImage(size, format, background));

	for (int i = 1; i < (int)frames; i++) {
		this->frames.push_back(AsciiImage(size, format, background));
	}
}
AsciiAnimation::AsciiAnimation(ImageFormats format, Pixel background, const AsciiImageFrame& image) {
	this->animationSpeed = 800;
	this->frames.push_back(AsciiImage(format, background, image));
}
AsciiAnimation::AsciiAnimation(ImageFormats format, Pixel background, AsciiImageFrame&& image) {
	this->animationSpeed = 800;
	this->frames.push_back(AsciiImage(format, background, std::move(image)));
}
AsciiAnimation::AsciiAnimation(Rectangle2I region, ImageFormats format, Pixel background, const AsciiImageFrame& image) {
	this->animationSpeed = 800;
	this->frames.push_back(AsciiImage(region, format, background, image));
}
AsciiAnimation::AsciiAnimation(ImageFormats format, Pixel background, const AsciiAnimation& animation) {
	this->animationSpeed = animation.animationSpeed;

	for (int i = 0; i < (int)animation.frames.size(); i++) {
		this->frames.push_back(AsciiImage(format, background, animation.frames[i]));
	}
}
AsciiAnimation::AsciiAnimation(ImageFormats format, Pixel background, AsciiAnimation&& animation) {
	this->animationSpeed = animation.animationSpeed;

	for (int i = 0; i < (int)animation.frames.size(); i++) {
		this->frames.push_back(AsciiImage(format, background, std::move(animation.frames[i])));
	}
	animation.frames.clear();
}
AsciiAnimation::AsciiAnimation(Rectangle2I region, ImageFormats format, Pixel background, const AsciiAnimation& animation) {
	this->animationSpeed = animation.animationSpeed;

	for (int i = 0; i < (int)animation.frames.size(); i++) {
		this->frames.push_back(AsciiImage(region, format, background, animation.frames[i]));
	}
}
AsciiAnimation::AsciiAnimation(const AsciiImageFrame& image) {
	this->animationSpeed = 800;

	this->frames.push_back(image);
}
AsciiAnimation::AsciiAnimation(AsciiImageFrame&& image) {
	this->animationSpeed = 800;

	this->frames.push_back(std::move(image));
}
AsciiAnimation::AsciiAnimation(const AsciiAnimation& animation) {
	this->animationSpeed = animation.animationSpeed;

	for (int i = 0; i < (int)animation.frames.size(); i++) {
		this->frames.push_back(animation.frames[i]);
	}
}
AsciiAnimation::AsciiAnimation(AsciiAnimation&& animation) {
	this->animationSpeed = animation.animationSpeed;

	for (int i = 0; i < (int)animation.frames.size(); i++) {
		this->frames.push_back(std::move(animation.frames[i]));
	}
	animation.frames.clear();
}
AsciiAnimation::~AsciiAnimation() {
	frames.clear();
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

// Assignment
AsciiAnimation& AsciiAnimation::operator=(const AsciiImageFrame& image) {
	animationSpeed = 800;
	frames.clear();
	frames.push_back(image);
	return *this;
}
AsciiAnimation& AsciiAnimation::operator=(AsciiImageFrame&& image) {
	animationSpeed = 800;
	frames.clear();
	frames.push_back(std::move(image));
	return *this;
}
AsciiAnimation& AsciiAnimation::operator=(const AsciiAnimation& animation) {
	this->animationSpeed = animation.animationSpeed;
	this->frames.clear();

	for (int i = 0; i < (int)animation.frames.size(); i++) {
		this->frames.push_back(animation.frames[i]);
	}
	return *this;
}
AsciiAnimation& AsciiAnimation::operator=(AsciiAnimation&& animation) {
	this->animationSpeed = animation.animationSpeed;
	this->frames.clear();

	for (int i = 0; i < (int)animation.frames.size(); i++) {
		this->frames.push_back(std::move(animation.frames[i]));
	}
	animation.frames.clear();
	return *this;
}

// Frames
AsciiImageFrame& AsciiAnimation::operator[](int index) {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::operator[]'");

	return frames[index];
}
const AsciiImageFrame& AsciiAnimation::operator[](int index) const {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::operator[]'");

	return frames[index];
}
AsciiImageFrame& AsciiAnimation::operator[](const std::string& name) {
	auto it = find_if(frames.begin(), frames.end(),
		[&name](const AsciiImage& iamge) { return iamge.name == name; }
	);
	if (it == frames.end())
		throw std::exception("Invalid name in 'AsciiAnimation::operator[]'");

	return *it;
}
const AsciiImageFrame& AsciiAnimation::operator[](const std::string& name) const {
	auto it = find_if(frames.begin(), frames.end(),
		[&name](const AsciiImage& iamge) { return iamge.name == name; }
	);
	if (it == frames.end())
		throw std::exception("Invalid name in 'AsciiAnimation::operator[]'");

	return *it;
}

#pragma endregion
//========= INPUT/OUTPUT =========
#pragma region Input/Output

// Input
bool AsciiAnimation::load(InputStream* in) {
	AsciiImageHeader header = AsciiImageHeader();
	if (AsciiImageInfo::loadHeader(in, header, true)) {
		bool success = true;
		std::vector<AsciiImage> newFrames;
		for (int i = 0; i < (int)header.frameCount && success; i++) {
			in->setPointer(header.fileStart + header.frameList[i].frameOffset + header.frameHeaderSize, false);
			AsciiImage frame = AsciiImage();
			success = frame.AsciiImageFrame::load(in, header, header.frameList[i]);
			newFrames.push_back(frame);
		}
		if (success) {
			animationSpeed = header.animationSpeed;
			frames.clear();
			frames = move(newFrames);
			return true;
		}
	}
	return false;
}
bool AsciiAnimation::loadFile(const std::string& path) {
	FileInputStream in = FileInputStream();
	if (in.open(path)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool AsciiAnimation::loadResource(const std::string& name, const std::string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(name, type)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool AsciiAnimation::loadResource(const std::string& name, unsigned int typeId) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(name, typeId)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool AsciiAnimation::loadResource(unsigned int id, const std::string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(id, type)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool AsciiAnimation::loadResource(unsigned int id, unsigned int typeId) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(id, typeId)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool AsciiAnimation::loadClipboard() {
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
bool AsciiAnimation::save(OutputStream* out, unsigned short version, bool smallFile) const {
	AsciiImageHeader header = AsciiImageInfo::createHeader(*this, version, smallFile);

	if (AsciiImageInfo::saveHeader(out, header, true)) {
		bool success = true;
		for (int i = 0; i < (int)frames.size() && success; i++) {
			out->setPointer(header.fileStart + header.frameList[i].frameOffset + header.frameHeaderSize, false);
			success = frames[i].AsciiImageFrame::save(out, header, header.frameList[i]);
		}
		return success;
	}
	return false;
}
bool AsciiAnimation::saveFile(const std::string& path, unsigned short version, bool smallFile) const {
	FileOutputStream out = FileOutputStream();
	if (out.open(path)) {
		bool success = save(&out, version, smallFile);
		out.close();
		return success;
	}
	return false;
}
bool AsciiAnimation::saveClipboard(unsigned short version, bool smallFile) const {
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
//============ FRAMES ============
#pragma region Frames

AsciiImageFrame& AsciiAnimation::at(int index) {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::operator[]'");

	return frames[index];
}
const AsciiImageFrame& AsciiAnimation::at(int index) const {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::operator[]'");

	return frames[index];
}
AsciiImageFrame& AsciiAnimation::at(const std::string& name) {
	auto it = std::find_if(frames.begin(), frames.end(),
		[&name](const AsciiImage& iamge) { return iamge.name == name; }
	);
	if (it == frames.end())
		throw std::exception("Invalid name in 'AsciiAnimation::at'");

	return *it;
}
const AsciiImageFrame& AsciiAnimation::at(const std::string& name) const {
	auto it = std::find_if(frames.begin(), frames.end(),
		[&name](const AsciiImage& iamge) { return iamge.name == name; }
	);
	if (it == frames.end())
		throw std::exception("Invalid name in 'AsciiAnimation::at'");

	return *it;
}
void AsciiAnimation::addFrame() {
	insertFrame((int)frames.size());
}
void AsciiAnimation::addFrame(const AsciiImageFrame& image) {
	insertFrame((int)frames.size(), std::move(image));
}
void AsciiAnimation::addFrame(AsciiImageFrame&& image) {
	insertFrame((int)frames.size(), image);
}
void AsciiAnimation::addFrame(Rectangle2I region, const AsciiImageFrame& image) {
	insertFrame((int)frames.size(), region, image);
}
void AsciiAnimation::insertFrame(int index) {
	if (index < 0 || index > (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::insertFrame'");
	int sizeToMimicIndex = (index > 0 ? index - 1 : 0);
	frames.insert(frames.begin() + index, AsciiImage(frames[sizeToMimicIndex].frameSize, frames[0].format, frames[0].background));
	frames[index].setFrameSpeed(frames[index == 0 ? 1 : index - 1].frameSpeed);
}
void AsciiAnimation::insertFrame(int index, const AsciiImageFrame& image) {
	if (index < 0 || index > (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::insertFrame'");
	if (getFlag(frames[0].format, ImageFormats::Multisize) || frames[0].frameSize == image.frameSize)
		frames.insert(frames.begin() + index, AsciiImage(frames[0].format, frames[0].background, image));
	else
		frames.insert(frames.begin() + index, AsciiImage(Rectangle2I(frames[0].frameSize), frames[0].format, frames[0].background, image));
}
void AsciiAnimation::insertFrame(int index, AsciiImageFrame&& image) {
	if (index < 0 || index > (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::insertFrame'");
	if (getFlag(frames[0].format, ImageFormats::Multisize) || frames[0].frameSize == image.frameSize)
		frames.insert(frames.begin() + index, AsciiImage(frames[0].format, frames[0].background, std::move(image)));
	else
		frames.insert(frames.begin() + index, AsciiImage(Rectangle2I(frames[0].frameSize), frames[0].format, frames[0].background, image));
}
void AsciiAnimation::insertFrame(int index, Rectangle2I region, const AsciiImageFrame& image) {
	if (index < 0 || index > (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::insertFrame'");
	if (!getFlag(frames[0].format, ImageFormats::Multisize) && region.size != frames[0].frameSize)
		throw std::exception("Animation is not multisize, cannot add from region of different size");
	frames.insert(frames.begin() + index, AsciiImage(region, frames[0].format, frames[0].background, image));
}
void AsciiAnimation::setFrame(int index, const AsciiImageFrame& image) {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::setFrame'");
	if (frames[index].frameSize == image.frameSize && frames[index].format == image.format) {
		Pixel background = frames[index].background;
		frames[index] = image;
		frames[index].background = background;
	}
	else if (getFlag(frames[0].format, ImageFormats::Multisize) || frames[0].frameSize == image.frameSize)
		frames[index] = AsciiImage(frames[0].format, frames[0].background, image);
	else
		frames[index] = AsciiImage(Rectangle2I(frames[0].frameSize), frames[0].format, frames[0].background, image);
}
void AsciiAnimation::setFrame(int index, AsciiImageFrame&& image) {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::setFrame'");
	if (getFlag(frames[0].format, ImageFormats::Multisize) || frames[0].frameSize == image.frameSize)
		frames[index] = AsciiImage(frames[0].format, frames[0].background, std::move(image));
	else
		frames[index] = AsciiImage(Rectangle2I(frames[0].frameSize), frames[0].format, frames[0].background, image);
}
void AsciiAnimation::setFrame(int index, Rectangle2I region, const AsciiImageFrame& image) {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::setFrame'");
	if (!getFlag(frames[0].format, ImageFormats::Multisize) && region.size != frames[0].frameSize)
		throw std::exception("Animation is not multisize, cannot set from region of different size");
	frames[index] = AsciiImage(region, frames[0].format, frames[0].background, image);
}
void AsciiAnimation::removeFrame() {
	frames.pop_back();
}
void AsciiAnimation::removeFrame(int index) {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::removeFrame'");
	frames.erase(frames.begin() + index);
}
void AsciiAnimation::swapFrame(int indexA, int indexB) {
	if (indexA < 0 || indexA >= (int)frames.size())
		throw std::out_of_range("Invalid indexA in 'AsciiAnimation::swapFrame'");
	if (indexB < 0 || indexB >= (int)frames.size())
		throw std::out_of_range("Invalid indexB in 'AsciiAnimation::swapFrame'");

	AsciiImage swapFrame = std::move(frames[indexA]);
	frames[indexA] = std::move(frames[indexB]);
	frames[indexB] = std::move(swapFrame);
}
void AsciiAnimation::moveFrame(int index, int newIndex, bool relative) {
	int finalNewIndex = newIndex + (relative ? index : 0);
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::moveFrame'");
	if (finalNewIndex < 0 || finalNewIndex >= (int)frames.size())
		throw std::out_of_range("Invalid new index in 'AsciiAnimation::moveFrame'");
	
	AsciiImage moveFrame = std::move(frames[index]);
	frames.erase(frames.begin() + index);
	frames.insert(frames.begin() + finalNewIndex, std::move(moveFrame));
}
void AsciiAnimation::clearFrames() {
	frames.erase(frames.begin() + 1, frames.end());
	frames[0].clear();
}

std::string AsciiAnimation::getName(int index) const {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::getName'");

	return frames[index].name;
}
void AsciiAnimation::setName(int index, const std::string& name) {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::setName'");

	frames[index].name = name;
}
bool AsciiAnimation::containsName(const std::string& name) const {
	return std::find_if(frames.begin(), frames.end(),
		[&name](const AsciiImage& image) { return image.name == name; }
	) != frames.end();
}

#pragma endregion
//========= MODIFICATION =========
#pragma region Modification

void AsciiAnimation::resize(Point2I size, int index) {
	if (index < -1 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::resize'");

	if (index != -1 && getFlag(frames[0].format, ImageFormats::Multisize)) {
		frames[index].resize(size);
	}
	else {
		for (int i = 0; i < (int)frames.size(); i++) {
			frames[i].resize(size);
		}
	}
}
void AsciiAnimation::rotate(int rotations, int index) {
	if (index < -1 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::rotate'");

	if (index != -1) {
		frames[index].rotate(rotations);
	}
	else {
		for (int i = 0; i < (int)frames.size(); i++) {
			frames[i].rotate(rotations);
		}
	}
}
void AsciiAnimation::rotateAndResize(int rotations, int index) {
	if (index < -1 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::rotateAndResize'");

	rotations = GMath::wrap(rotations, 4);

	if (index != -1 && (getFlag(frames[0].format, ImageFormats::Multisize) ||
		frames[0].frameSize.x == frames[0].frameSize.y || rotations % 2 == 0)) {
		frames[index].rotateAndResize(rotations);
	}
	else {
		for (int i = 0; i < (int)frames.size(); i++) {
			frames[index].rotateAndResize(rotations);
		}
	}
}
void AsciiAnimation::translate(Point2I distance, int index) {
	if (index < -1 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::translate'");

	if (index != -1) {
		frames[index].translate(distance);
	}
	else {
		for (int i = 0; i < (int)frames.size(); i++) {
			frames[i].translate(distance);
		}
	}
}
void AsciiAnimation::flip(Orientations axis, int index) {
	if (index < -1 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::flip'");

	if (index != -1) {
		frames[index].flip(axis);
	}
	else {
		for (int i = 0; i < (int)frames.size(); i++) {
			frames[i].flip(axis);
		}
	}
}
void AsciiAnimation::clear(int index) {
	if (index < -1 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::clear'");

	if (index != -1) {
		frames[index].clear();
	}
	else {
		for (int i = 0; i < (int)frames.size(); i++) {
			frames[i].clear();
		}
	}
}
GraphicsSPtr AsciiAnimation::createGraphics() {
	return std::shared_ptr<Graphics>(new Graphics(this));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Ascii Information
Point2I AsciiAnimation::size() const {
	Point2I maxSize = frames[0].size();
	for (int i = 1; i < (int)frames.size(); i++)
		maxSize = GMath::max(maxSize, frames[i].frameSize);
	return maxSize;
}
Pixel AsciiAnimation::getBackground() const {
	return frames[0].background;
}
void AsciiAnimation::setBackground(Pixel background) {
	for (int i = 0; i < (int)frames.size(); i++) {
		frames[i].background = background;
	}
}
unsigned int AsciiAnimation::frameCount() const {
	return frames.size();
}
ImageFormats AsciiAnimation::getFormat() const {
	return frames[0].format;
}
void AsciiAnimation::setFormat(ImageFormats format) {
	if (!getFlag(format, ImageFormats::Animation)) {
		frames.erase(frames.begin() + 1, frames.end());
		frames[0].setFormat(format);
	}
	else {
		Point2I biggestSize = size();
		for (int i = 0; i < (int)frames.size(); i++) {
			frames[i].setFormat(format);
			if (!getFlag(format, ImageFormats::Multisize))
				frames[i].resize(biggestSize);
		}
	}
}
unsigned int AsciiAnimation::getAnimationSpeed() const {
	return animationSpeed;
}
void AsciiAnimation::setAnimationSpeed(unsigned int animationSpeed) {
	this->animationSpeed = animationSpeed;
}
unsigned int AsciiAnimation::getFrameSpeed(int index) const {
	if (index < 0 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::getFrameSpeed'");
	return frames[index].frameSpeed;
}
void AsciiAnimation::setFrameSpeed(unsigned int frameSpeed, int index) {
	if (index < -1 || index >= (int)frames.size())
		throw std::out_of_range("Invalid index in 'AsciiAnimation::setFrameSpeed'");
	if (index != -1) {
		frames[index].frameSpeed = frameSpeed;
	}
	else {
		for (int i = 0; i < (int)frames.size(); i++)
			frames[i].frameSpeed = frameSpeed;
	}
}

#pragma endregion

//=================================================================|
