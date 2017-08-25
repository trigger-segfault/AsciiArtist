/*=================================================================|
|* File:				ConsoleEnums.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONSOLE_ENUMS_H
#define CONSOLE_ENUMS_H

#define NOMINMAX
#include <windows.h>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Point.h>

namespace PowerConsole {
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

#define defineRasterSize(width, height) R##width##x##height = (unsigned short)(((unsigned short)((unsigned char)width) << 8) | (unsigned char)height)
#define toRasterSize(width, height) (RasterFontSizes)(((unsigned char)(width) << 8) | (unsigned char)(height))
#define getRasterSize(rasterFontSize) Point2I(((unsigned short)(rasterFontSize) & 0xFF00) >> 8, (unsigned short)(rasterFontSize) & 0x00FF)
#define getRasterWidth(rasterFontSize) (int)(((unsigned short)(rasterFontSize) & 0xFF00) >> 8)
#define getRasterHeight(rasterFontSize) (int)((unsigned short)(rasterFontSize) & 0x00FF)

/* The different available console raster font sizes. */
enum class RasterFontSizes : unsigned short {
	None = 0,
	defineRasterSize(4, 6),
	defineRasterSize(6, 8),
	defineRasterSize(8, 8),
	defineRasterSize(16, 8),
	defineRasterSize(5, 12),
	defineRasterSize(7, 12),
	defineRasterSize(8, 12),
	defineRasterSize(16, 12),
	defineRasterSize(12, 16),
	defineRasterSize(10, 18)
};
#undef defineRasterSize

/* Returns true if the font size is valid to use. */
bool isValidFontSize(PcGeo::Point2I size);
/* Returns true if the raster font size is valid to use. */
bool isValidRasterFontSize(RasterFontSizes rasterSize);

/* The different available console fonts. */
enum class ConsoleFonts : unsigned char {
	None = 0,
	Terminal,
	Consolas,
	LucidaConsole
};
/* The different available console font weights. */
enum class ConsoleFontWeights : unsigned int {
	None = 0,
	Regular = FW_NORMAL,
	Bold = FW_BOLD
};

/* The different flags for the console input mode. */
enum class ConsoleInputModes : unsigned long {
	/* No flags are enabled. */
	None = 0,
	/* When disabled, reading input from the console will not output the characters the user is writing. Good for passwords. */
	EchoInput = ENABLE_ECHO_INPUT,
	/* Required for 'InsertMode' and 'QuickEditMode' to work. */
	ExtendedFlags = ENABLE_EXTENDED_FLAGS,
	/* Required for using console input. */
	InsertMode = ENABLE_INSERT_MODE,
	/* Reading console input will not return until enter is pressed. Otherwise it will return after one or more characters are avilable. */
	LineInput = ENABLE_LINE_INPUT,
	/* Mouse input is reported to the event log. */
	MouseInput = ENABLE_MOUSE_INPUT,
	/* Enables Ctrl+C to be processed by the console. */
	ProcessedInput = ENABLE_PROCESSED_INPUT,
	/* Allows the user to select and edit text with the mouse. */
	QuickEditMode = ENABLE_QUICK_EDIT_MODE,
	/* Changes to the console's size and buffer are reported to the event log. */
	WindowInput = ENABLE_WINDOW_INPUT,
	/* Undefined but enabled by default. */
	AutoPosition = ENABLE_AUTO_POSITION
};
/* The different flags for the console output mode. */
enum class ConsoleOutputModes : unsigned long {
	/* No flags are enabled. */
	None = 0,
	/* Characters 0-31 will be processed as actions instead of as characters when being output. */
	ProcessedOutput = ENABLE_PROCESSED_OUTPUT,
	/* Enables line wrapping when reaching the right side of the buffer. */
	WrapAtEolOutput = ENABLE_WRAP_AT_EOL_OUTPUT
};
DEFINE_ENUM_FLAG_OPERATORS(ConsoleInputModes)
DEFINE_ENUM_FLAG_OPERATORS(ConsoleOutputModes)

/* The different available system sounds to play. */
enum class SystemSounds {
	None,
	Alert,
	Information,
	Warning,
	Error
};

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
