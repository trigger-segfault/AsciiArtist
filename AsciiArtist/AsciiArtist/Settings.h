/*=================================================================|
|* File:				Settings.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/IniDocument.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <PowerConsole/Input/Keys.h>

using namespace PowerConsole::IO;
using namespace PowerConsole::Input;
using namespace PowerConsole::Drawing;

namespace AsciiArtist {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Settings {
	
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The settings to read from. */
	static IniDocumentSPtr settings;

	#pragma endregion
	//========== FUNCTIONS ===========
	#pragma region Functions
public:
	/* Gets the ini settings. */
	static IniDocumentSPtr get();
	/* Loads the ini settings. */
	static bool load();
	/* Saves the ini settings. */
	static bool save();

	/* Creates the default image based on the settings. */
	static AsciiAnimationSPtr createDefaultImage();

	static Pixel getDrawingPixel(bool secondary);
	static int getDefaultToolIndex();
	static int getMaxUndos();

	static unsigned short getFileVersion();
	static bool getSmallFile();

	static bool getToolOption(const std::string& name, bool defaultValue);

	static Pixel readPixel(const std::string& charText, const std::string& foreText, const std::string& backText);
	static ImageFormats readFormat(const std::string& formatText);
	static Keys getHotkey(const std::string& name);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
