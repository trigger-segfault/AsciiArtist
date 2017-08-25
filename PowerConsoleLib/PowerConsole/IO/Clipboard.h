/*=================================================================|
|* File:				Clipboard.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <vector>
#include <string>
#define NOMINMAX
#include <Windows.h>
#include <PowerConsole/Declarations.h>

namespace PowerConsole::IO {
//=================================================================|
// PREDEFINITIONS												   |
//=================================================================/

/* A registered handle to a clipboard format. */
typedef unsigned int ClipboardFormat;

//=================================================================|
// CLASSES														   |
//=================================================================/

/* The static class for clipboard access. */
class Clipboard {
public:
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The clipboard format for file drops. */
	static const ClipboardFormat FILE_DROP_FORMAT = CF_HDROP;
	/* The clipboard format for plain text. */
	static const ClipboardFormat TEXT_FORMAT = CF_TEXT;

	#pragma endregion
	//========== CLIPBOARD ===========
	#pragma region Clipboard

	/* Registers the specified clipboard format and returns it's ID. */
	static ClipboardFormat registerFormat(const std::string& formatName);
	/* Returns true if the specified clipboard format is available. */
	static bool containsFormat(ClipboardFormat clipboardFormat);
	/* Returns the available clipboard Formats. */
	static std::vector<ClipboardFormat> getAvailableFormats();
	/* Returns the available clipboard Formats as their names. */
	static std::vector<std::string> getAvailableFormatNames();
	/* Returns true if the clipboard is empty. */
	static bool isClipboardEmpty();
	/* Empties the clipboard. */
	static void emptyClipboard();

	/* Returns true if the text clipboard format is available. */
	static bool containsText();
	/* Reads the text in the clipboard if there is any. */
	static std::string getText();
	/* Writes text to the clipboard. */
	static void setText(const std::string& text);

	/* Returns true if the file drop clipboard format is available. */
	static bool containsFileDrop();
	/* Reads the file names from the clipboard. */
	static std::vector<std::string> getFileDropNames();

	/* Returns the number of clipboard formats registered. */
	static int countClipboardFormats();
	/* Gets the name of the clipboard format id. */
	static std::string getClipboardFormatName(ClipboardFormat clipboardFormat);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
