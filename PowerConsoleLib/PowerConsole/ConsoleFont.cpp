/*=================================================================|
|* File:				ConsoleFont.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ConsoleFont.h"
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/ResourceStream.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/IO/Path.h>

using namespace PowerConsole;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const std::string ConsoleFontInfo::FILE_EXTENSION = ".confnt";
const std::string ConsoleFontInfo::CLIPBOARD_FORMAT_NAME = "Console Font";
const PcIO::ClipboardFormat ConsoleFontInfo::CLIPBOARD_FORMAT = Clipboard::registerFormat(ConsoleFontInfo::CLIPBOARD_FORMAT_NAME);

const ConsoleFont ConsoleFont::DEFAULT = ConsoleFont(ConsoleFonts::Terminal, RasterFontSizes::R8x12);

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

ConsoleFont::ConsoleFont()
 :	font(ConsoleFonts::Terminal), size(Point2I(8, 12)),
	weight(ConsoleFontWeights::Regular) {}
ConsoleFont::ConsoleFont(ConsoleFonts type, Point2I size)
 :	ConsoleFont() {
	setFontType(type);
	setFontSize(size);
}
ConsoleFont::ConsoleFont(ConsoleFonts type, RasterFontSizes rasterSize)
 :	ConsoleFont() {
	setFontType(type);
	setRasterFontSize(rasterSize);
}
ConsoleFont::ConsoleFont(ConsoleFonts type, int trueTypeSize, ConsoleFontWeights weight)
 :	ConsoleFont() {
	setFontType(type);
	setTrueTypeFontSize(trueTypeSize);
	setFontWeight(weight);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

ConsoleFonts ConsoleFont::getFontType() const {
	return font;
}
void ConsoleFont::setFontType(ConsoleFonts type) {
	this->font = type;
	if (type == ConsoleFonts::Terminal) {
		if (size.x == 0)
			size = Point2I(8, 12);
		weight = (size.x == 6 ? ConsoleFontWeights::Bold : ConsoleFontWeights::Regular);
	}
	else if (type == ConsoleFonts::Consolas || type == ConsoleFonts::LucidaConsole) {
		size.x = 0;
	}
}
Point2I ConsoleFont::getFontSize() const {
	return size;
}
void ConsoleFont::setFontSize(Point2I size) {
	if (font == ConsoleFonts::Terminal) {
		if (isValidFontSize(size)) {
			this->size = size;
			if (size == Point2I(6, 8))
				weight = ConsoleFontWeights::Bold;
			else
				weight = ConsoleFontWeights::Regular;
		}
	}
	else {
		this->size = Point2I(0, std::max(1, std::min(100, size.y)));
	}
}
RasterFontSizes ConsoleFont::getRasterFontSize() const {
	if (font == ConsoleFonts::Terminal) {
		return toRasterSize(size.x, size.y);
	}
	return RasterFontSizes::None;
}
void ConsoleFont::setRasterFontSize(RasterFontSizes rasterSize) {
	if (font == ConsoleFonts::Terminal && isValidRasterFontSize(rasterSize)) {
		size = getRasterSize(rasterSize);
		if (rasterSize == RasterFontSizes::R6x8)
			weight = ConsoleFontWeights::Bold;
		else
			weight = ConsoleFontWeights::Regular;
	}
}
int ConsoleFont::getTrueTypeFontSize() const {
	return size.y;
}
void ConsoleFont::setTrueTypeFontSize(int trueTypeSize) {
	if (font != ConsoleFonts::Terminal) {
		size = Point2I(0, std::max(1, std::min(100, trueTypeSize)));
	}
}
ConsoleFontWeights ConsoleFont::getFontWeight() const {
	return weight;
}
void ConsoleFont::setFontWeight(ConsoleFontWeights weight) {
	if (font != ConsoleFonts::Terminal) {
		this->weight = weight;
	}
}

#pragma endregion
//========= INPUT/OUTPUT =========
#pragma region Input/Output

// Input
bool ConsoleFont::load(InputStream* in) {
	bool oldEndian = in->isBigEndian();
	in->setBigEndian(false);

	ConsoleFonts newFont = (ConsoleFonts)in->readUChar();
	Point2I newSize;
	newSize.x = in->readUChar();
	newSize.y = in->readUChar();
	ConsoleFontWeights newWeight = (ConsoleFontWeights)in->readUShort();

	// Error checking time
	bool error = false;
	if (!in->hasError()) {
		if (newFont == ConsoleFonts::Terminal || newFont == ConsoleFonts::Consolas || newFont == ConsoleFonts::LucidaConsole) {
			if (newFont == ConsoleFonts::Terminal) {
				if (!isValidFontSize(newSize))
					error = true;
				if (newSize == Point2I(6, 8)) {
					if (newWeight != ConsoleFontWeights::Bold)
						error = true;
				}
				else if (newWeight != ConsoleFontWeights::Regular) {
					error = true;
				}
			}
			else {
				if (newSize.x != 0 || newSize.y < 1 || newSize.y > 100)
					error = true;
			}
			if (newWeight != ConsoleFontWeights::Regular && newWeight != ConsoleFontWeights::Bold)
				error = true;

			if (!error) {
				font = newFont;
				size = newSize;
				weight = newWeight;
			}
		}
		else {
			error = true;
		}
	}

	in->setBigEndian(oldEndian);
	return !in->hasError() && !error;
}
bool ConsoleFont::loadFile(const std::string& path) {
	FileInputStream in = FileInputStream();
	if (in.open(path)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool ConsoleFont::loadResource(unsigned int id, const std::string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(id, type)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool ConsoleFont::loadClipboard() {
	if (Clipboard::containsFormat(ConsoleFontInfo::CLIPBOARD_FORMAT)) {
		ClipboardInputStream in = ClipboardInputStream();
		if (in.open()) {
			bool success = false;
			if (in.getData(ConsoleFontInfo::CLIPBOARD_FORMAT)) {
				success = load(&in);
			}
			in.close();
			return success;
		}
	}
	return false;
}

// Output
bool ConsoleFont::save(OutputStream* out) {
	bool oldEndian = out->isBigEndian();
	out->setBigEndian(false);

	out->writeUChar((unsigned char)font);
	out->writeUChar((unsigned char)size.x);
	out->writeUChar((unsigned char)size.y);
	out->writeUShort((unsigned short)weight);

	out->setBigEndian(oldEndian);
	return !out->hasError();
}
bool ConsoleFont::saveFile(const std::string& path) {
	FileOutputStream out = FileOutputStream();
	if (out.open(path)) {
		bool success = save(&out);
		out.close();
		return success;
	}
	return false;
}
bool ConsoleFont::saveClipboard() {
	ClipboardOutputStream out = ClipboardOutputStream();
	if (out.open()) {
		bool success = false;
		if (save(&out)) {
			success = out.setData(ConsoleFontInfo::CLIPBOARD_FORMAT);
		}
		out.close();
		return success;
	}
	return false;
}

#pragma endregion
//=================================================================|
