/*=================================================================|
|* File:				ConsoleFont.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONSOLE_FONT_H
#define CONSOLE_FONT_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/ConsoleEnums.h>
#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/IO/IOStream.h>
#include <PowerConsole/IO/Clipboard.h>
#include <PowerConsole/Drawing/Color.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

namespace PowerConsole {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ConsoleFontInfo {
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The recommended file extension for use with console font files. */
	static const std::string FILE_EXTENSION;
	/* The name of the clipboard format used for console fonts. */
	static const std::string CLIPBOARD_FORMAT_NAME;
	/* The id of the clipboard format used for console fonts. */
	static const PcIO::ClipboardFormat CLIPBOARD_FORMAT;

	#pragma endregion
};

class ConsoleFont {
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The default console font. */
	static const ConsoleFont DEFAULT;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The font type. */
	Pc::ConsoleFonts font;
	/* The font size. */
	PcGeo::Point2I size;
	/* The font weight. */
	Pc::ConsoleFontWeights weight;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default console font. */
	ConsoleFont();
	/* Constructs the console font with the specified settings. */
	ConsoleFont(ConsoleFonts type, PcGeo::Point2I size);
	/* Constructs the console font with the specified settings. */
	ConsoleFont(ConsoleFonts type, RasterFontSizes rasterSize);
	/* Constructs the console font with the specified settings. */
	ConsoleFont(ConsoleFonts type, int trueTypeSize, ConsoleFontWeights weight = ConsoleFontWeights::Regular);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the font type. */
	ConsoleFonts getFontType() const;
	/* Sets the font type. */
	void setFontType(ConsoleFonts type);
	/* Gets the font size. */
	PcGeo::Point2I getFontSize() const;
	/* Sets the font size. */
	void setFontSize(PcGeo::Point2I size);
	/* Gets the raster font size. */
	RasterFontSizes getRasterFontSize() const;
	/* Sets the raster font size. */
	void setRasterFontSize(RasterFontSizes rasterSize);
	/* Gets the true type font size. */
	int getTrueTypeFontSize() const;
	/* Sets the true type font size. */
	void setTrueTypeFontSize(int trueTypeSize);
	/* Gets the font weight. */
	ConsoleFontWeights getFontWeight() const;
	/* Sets the font weight. */
	void setFontWeight(ConsoleFontWeights weight);

	#pragma endregion
	//========= INPUT/OUTPUT =========
	#pragma region Input/Output

	// Input
	/* Loads the font the input stream. */
	bool load(PcIO::InputStream* in);
	/* Loads the font the from file. */
	bool loadFile(const std::string& path);
	/* Loads the font from the resources. */
	bool loadResource(unsigned int id, const std::string& type = "CONSOLE_FONT");
	/* Loads the font from the clipboard. */
	bool loadClipboard();

	// Output
	/* Saves the font to the output stream. */
	bool save(PcIO::OutputStream* out);
	/* Saves the font to file. */
	bool saveFile(const std::string& path);
	/* Saves the font to the clipboard. */
	bool saveClipboard();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
