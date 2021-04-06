/*=================================================================|
|* File:				Settings.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Settings.h"
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/IO/Resources.h>
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Helpers/Parsing.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Geometry/GMath.h>
#include "resource.h"

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace AsciiArtist;
//=================================================================|
// CLASSES														   |
//=================================================================/
//=========== MEMBERS ============
#pragma region Members

IniDocumentSPtr Settings::settings = nullptr;

#pragma endregion
//========== FUNCTIONS ===========
#pragma region Functions

IniDocumentSPtr Settings::get() {
	return settings;
}
bool Settings::load() {
	settings = makeIniDocument();
	settings->setKeepComments(true);
	settings->setEscapeEnabled(false);
	std::string path = Path::combine(Path::getDirectory(Path::getExecutablePath()), "Settings.ini");
	if (Path::fileExists(path) && settings->load(path)) {
		return true;
	}
	else {
		std::string text = Resources::loadText(TXT_SETTINGS, "TEXT_FILE");
		settings->loadFromText(text);
		FileOutputStream out;
		out.open(path);
		if (out.isOpen()) {
			out.writeText(text);
			out.close();
		}
		return false;
	}
}
bool Settings::save() {
	std::string path = Path::combine(Path::getDirectory(Path::getExecutablePath()), "Settings.ini");
	return settings->save(path);
}

AsciiAnimationSPtr Settings::createDefaultImage() {
	auto section = settings->get("Image");
	Point2I size = section->getPoint2I("Size", Point2I(64, 32));
	size = GMath::clamp(size, Point2I::ONE, AsciiImageInfo::MAX_SIZE);
	int frameSpeed = section->getInt("FrameSpeed", RADIX_DECIMAL, 800);
	ImageFormats format = readFormat(section->getString("Format"));
	Pixel background = readPixel(
		section->getString("BackgroundCharacter"),
		section->getString("BackgroundForeground"),
		section->getString("BackgroundBackground")
	);
	auto image = std::make_shared<AsciiAnimation>(size, format, background);
	image->setFrameSpeed((unsigned int)frameSpeed);
	return image;
}

Pixel Settings::getDrawingPixel(bool secondary) {
	std::string prefix = (secondary ? "Secondary" : "Primary");
	auto section = settings->get("Editor");
	return readPixel(
		section->getString(prefix + "Character"),
		section->getString(prefix + "Foreground"),
		section->getString(prefix + "Background")
	);
}

int Settings::getDefaultToolIndex() {
	auto section = settings->get("Editor");
	std::string toolName = section->getString("Tool");
	std::string toolNames[] = { "Pen", "Line", "Square", "Circle", "Flood", "Border", "Text", "Select", "Eyedrpr" };
	for (int i = 0; i < 9; i++) {
		if (compareString(toolName, toolNames[i], false))
			return i;
	}
	return 0;
}
int Settings::getMaxUndos() {
	auto section = settings->get("Editor");
	return section->getInt("MaxUndos", RADIX_DECIMAL, 50);
}

bool Settings::getToolOption(const std::string& name, bool defaultValue) {
	auto section = settings->get("ToolOptions");
	return section->getBool(name, "true", defaultValue);
}
unsigned short Settings::getFileVersion() {
	auto section = settings->get("Image");
	std::string fileVersion = section->getString("FileVersion");
	if (compareString(fileVersion, "Version 2", false)) {
		return 2;
	}
	else if (compareString(fileVersion, "Version 3", false)) {
		return 3;
	}
	else if (compareString(fileVersion, "Small 1", false)) {
		return 1;
	}
	else {
		return 4;
	}
}
bool Settings::getSmallFile() {
	auto section = settings->get("Image");
	std::string fileVersion = section->getString("FileVersion");
	return compareString(fileVersion, "Small 1", false);
}

Pixel Settings::readPixel(const std::string& charText, const std::string& foreText, const std::string& backText) {
	std::string colorNames[] = {
		"Black", "DarkBlue", "DarkGreen", "DarkCyan", "DarkRed", "DarkMagenta", "DarkYellow", "Gray",
		"DarkGray", "Blue", "Green", "Cyan", "Red", "Magenta", "Yellow", "White"
	};

	Pixel output = Pixel(' ', 0x00);

	for (int i = 0; i < 16; i++) {
		if (compareString(foreText, colorNames[i], false))
			output.color = setFColor(output.color, (unsigned char)i);
		if (compareString(backText, colorNames[i], false))
			output.color = setBColor(output.color, toBColor((unsigned char)i));
	}
	bool foreRandom = compareString(foreText, "Random", false);
	bool backRandom = compareString(backText, "Random", false);
	for (int i = 0; i < 2; i++) {
		bool background = i == 1;
		const std::string& text = (background ? backText : foreText);
		output.attributes &= ~(!background ? PixelAttributes::ForegroundAll : PixelAttributes::BackgroundAll);

		if (compareString(text, "Transparent", false))
			setFlag(output.attributes, (!background ? PixelAttributes::FColor : PixelAttributes::BColor), false);
		else if (compareString(text, "Invert", false))
			setFlag(output.attributes, (!background ? PixelAttributes::FInvert : PixelAttributes::BInvert), true);
		else if (compareString(text, "Lighten", false))
			setFlag(output.attributes, (!background ? PixelAttributes::FLighten : PixelAttributes::BLighten), true);
		else if (compareString(text, "Darken", false))
			setFlag(output.attributes, (!background ? PixelAttributes::FDarken : PixelAttributes::BDarken), true);
		else {
			setFlag(output.attributes, (!background ? PixelAttributes::FColor : PixelAttributes::BColor), true);
			if (compareString(text, "Random", false)) {
				unsigned char color = (unsigned char)(rand() % 16);
				if (!background)
					output.color = setFColor(output.color, color);
				else
					output.color = setBColor(output.color, toBColor(color));
			}
		}
	}

	// If at least one of the colors is random, make sure
	// the foreground and background aren't the same.
	while (getFColor(output.color) == getBColor(output.color) && (foreRandom || backRandom)) {
		unsigned char color = (unsigned char)(rand() % 16);
		if (foreRandom)
			output.color = setFColor(output.color, color);
		else
			output.color = setBColor(output.color, toBColor(color));
	}
	if (compareString(charText, "Random", false)) {
		output.character = (unsigned char)(rand() % 256);
		while (output.character == 0 || output.character == 255) {
			output.character = (unsigned char)(rand() % 256);
		}
	}
	else if (compareString(charText, "Transparent", false)) {
		setFlag(output.attributes, PixelAttributes::Char, false);
	}
	else if ((charText.length() == 3 || charText.length() == 4) && charText.front() == '\'' && charText.back() == '\'') {
		if (charText.length() == 4 && charText[1] == '\\') {
			switch (charText[2]) {
			case '\\': output.character = '\\'; break;
			case '\"': output.character = '\"'; break;
			case '\'': output.character = '\''; break;
			case '0': output.character = '\0'; break;
			case 'a': output.character = '\a'; break;
			case 'b': output.character = '\b'; break;
			case 't': output.character = '\t'; break;
			case 'r': output.character = '\r'; break;
			case 'n': output.character = '\n'; break;
			}
		}
		else if (charText.length() == 3) {
			output.character = charText[1];
		}
	}
	else {
		try {
			output.character = Parsing::parseIntType<unsigned char>(charText, RADIX_ANY);
		} catch (...) {}
	}

	return output;
}
ImageFormats Settings::readFormat(const std::string& formatText) {
	std::map<std::string, ImageFormats> formatMap = {
		{ "Basic", ImageFormats::Basic },
		{ "Attributes", ImageFormats::Attributes },
		{ "Animation", ImageFormats::Animation },
		{ "Multisize", ImageFormats::Multisize }
	};
	ImageFormats imageFormat = ImageFormats::Basic;
	std::vector<std::string> formats;
	splitString(formatText, formats, "+");
	for (std::string& format : formats) {
		auto it = formatMap.find(format);
		if (it != formatMap.end())
			setFlag(imageFormat, it->second, true);
	}
	return imageFormat;
}
Keys Settings::getHotkey(const std::string& name) {
	auto section = settings->get("Hotkeys");
	std::string keyText = section->getString(name, "");
	if (keyText.length() == 1) {
		char c = keyText[0];
		if (c >= 'A' && c <= 'Z')
			return (Keys)((int)Keys::A + (int)(c - 'A'));
		else if (c >= 'a' && c <= 'z')
			return (Keys)((int)Keys::A + (int)(c - 'a'));
		else if (c >= '0' && c <= '9')
			return (Keys)((int)Keys::D0 + (int)(c - '0'));
	}
	return Keys::None;
}

#pragma endregion
//=================================================================|
