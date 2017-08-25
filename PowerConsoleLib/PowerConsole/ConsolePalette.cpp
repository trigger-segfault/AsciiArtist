/*=================================================================|
|* File:				ConsolePalette.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ConsolePalette.h"
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/ResourceStream.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/IO/Path.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const std::string ConsolePaletteInfo::CLIPBOARD_FORMAT_NAME = "Console Palette";
const PcIO::ClipboardFormat ConsolePaletteInfo::CLIPBOARD_FORMAT = Clipboard::registerFormat(ConsolePaletteInfo::CLIPBOARD_FORMAT_NAME);

const ConsolePalette ConsolePalette::DEFAULT = ConsolePalette{
	ColorB(  0,   0,   0),
	ColorB(  0,   0, 128),
	ColorB(  0, 128,   0),
	ColorB(  0, 128, 128),
	ColorB(128,   0,   0),
	ColorB(128,   0, 128),
	ColorB(128, 128,   0),
	ColorB(192, 192, 192),
	ColorB(128, 128, 128),
	ColorB(  0,   0, 255),
	ColorB(  0, 255,   0),
	ColorB(  0, 255, 255),
	ColorB(255,   0,   0),
	ColorB(255,   0, 255),
	ColorB(255, 255,   0),
	ColorB(255, 255, 255)
};

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

ConsolePalette::ConsolePalette() {
	for (int i = 0; i < NUM_COLORS; i++) {
		palette[i] = ColorB(0, 0, 0);
	}
}
ConsolePalette::ConsolePalette(std::initializer_list<ColorB> initList) {
	if ((int)initList.size() != NUM_COLORS)
		throw std::invalid_argument("Initializer musicList size does not match the number of colors for an Console Palette");
	int index = 0;
	for (const ColorB& color : initList) {
		palette[index] = color;
		index++;
	}
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

ColorB& ConsolePalette::operator[](int index) {
	return palette[index];
}
const ColorB& ConsolePalette::operator[](int index) const {
	return palette[index];
}

#pragma endregion
//========= INPUT/OUTPUT =========
#pragma region Input/Output

// Input
bool ConsolePalette::load(InputStream* in) {
	bool oldEndian = in->isBigEndian();
	in->setBigEndian(false);

	ColorB newPalette[NUM_COLORS];
	for (int i = 0; i < NUM_COLORS; i++) {
		newPalette[i] = ColorB(in->readUInt());
	}
	if (!in->hasError()) {
		for (int i = 0; i < NUM_COLORS; i++) {
			palette[i] = newPalette[i];
		}
	}

	in->setBigEndian(oldEndian);
	return !in->hasError();
}
bool ConsolePalette::loadFile(const std::string& path) {
	FileInputStream in = FileInputStream();
	if (in.open(path)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool ConsolePalette::loadResource(unsigned int id, const std::string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.open(id, type)) {
		bool success = load(&in);
		in.close();
		return success;
	}
	return false;
}
bool ConsolePalette::loadClipboard() {
	if (Clipboard::containsFormat(ConsolePaletteInfo::CLIPBOARD_FORMAT)) {
		ClipboardInputStream in = ClipboardInputStream();
		if (in.open()) {
			bool success = false;
			if (in.getData(ConsolePaletteInfo::CLIPBOARD_FORMAT)) {
				success = load(&in);
			}
			in.close();
			return success;
		}
	}
	return false;
}

// Output
bool ConsolePalette::save(OutputStream* out) {
	bool oldEndian = out->isBigEndian();
	out->setBigEndian(false);

	for (int i = 0; i < NUM_COLORS; i++) {
		out->writeUInt((COLORREF)palette[i]);
	}

	out->setBigEndian(oldEndian);
	return !out->hasError();
}
bool ConsolePalette::saveFile(const std::string& path) {
	FileOutputStream out = FileOutputStream();
	if (out.open(path)) {
		bool success = save(&out);
		out.close();
		return success;
	}
	return false;
}
bool ConsolePalette::saveClipboard() {
	ClipboardOutputStream out = ClipboardOutputStream();
	if (out.open()) {
		bool success = false;
		if (save(&out)) {
			success = out.setData(ConsolePaletteInfo::CLIPBOARD_FORMAT);
		}
		out.close();
		return success;
	}
	return false;
}

#pragma endregion
//=================================================================|
