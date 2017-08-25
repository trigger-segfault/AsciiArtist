/*=================================================================|
|* File:				ConsolePalette.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONSOLE_PALETTE_H
#define CONSOLE_PALETTE_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/IOStream.h>
#include <PowerConsole/IO/Clipboard.h>
#include <PowerConsole/Drawing/Color.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

namespace PowerConsole {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ConsolePaletteInfo {
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The name of the clipboard format used for console palettes. */
	static const std::string CLIPBOARD_FORMAT_NAME;
	/* The id of the clipboard format used for console palettes. */
	static const PcIO::ClipboardFormat CLIPBOARD_FORMAT;

	#pragma endregion
};

class ConsolePalette {
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The number of colors in the palette. */
	static const int NUM_COLORS = 16;
	/* The default console palette. */
	static const ConsolePalette DEFAULT;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The palette of colors. */
	PcDraw::ColorB palette[NUM_COLORS];

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default console palette. */
	ConsolePalette();
	/* Constructs the console palette with the specified colors. */
	ConsolePalette(std::initializer_list<PcDraw::ColorB> initList);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Gets the color specified index in the palette. */
	PcDraw::ColorB& operator[](int index);
	/* Gets the frame at the specified index. */
	const PcDraw::ColorB& operator[](int index) const;

	#pragma endregion
	//========= INPUT/OUTPUT =========
	#pragma region Input/Output

	// Input
	/* Loads the palette the input stream. */
	bool load(PcIO::InputStream* in);
	/* Loads the palette the from file. */
	bool loadFile(const std::string& path);
	/* Loads the palette from the resources. */
	bool loadResource(unsigned int id, const std::string& type = "CONSOLE_PALETTE");
	/* Loads the palette from the clipboard. */
	bool loadClipboard();

	// Output
	/* Saves the palette to the output stream. */
	bool save(PcIO::OutputStream* out);
	/* Saves the palette to file. */
	bool saveFile(const std::string& path);
	/* Saves the palette to the clipboard. */
	bool saveClipboard();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
