/*=================================================================|
|* File:				Console.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/ConsoleSettings.h>

namespace PowerConsole {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Console {
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The number of different colors the console can use. */
	static const int NUM_PALETTE_COLORS = 16;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The console settings to use. */
	static ConsoleSettingsUPtr console;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Console Cursor
public:
	/* Gets the console cursor position. */
	static PcGeo::Point2I getCursorPosition();
	/* Sets the console cursor position. */
	static void setCursorPosition(PcGeo::Point2I position);
	/* Gets the color of text written to the console. */
	static unsigned char getCurrentColor();
	/* Sets the color of text written to the console. */
	static void setCurrentColor(unsigned char color);
	/* Gets the console cursor fill. */
	static int getCursorFill();
	/* Sets the console cursor fill. */
	static void setCursorFill(int fill);
	/* Gets if the console cursor is visible. */
	static bool isCursorVisible();
	/* Sets if the console cursor is visible. */
	static void setCursorVisible(bool visible);

	#pragma endregion
	//--------------------------------
	#pragma region Console IO Mode

	/* Gets the console input mode. */
	static ConsoleInputModes getConsoleInputMode();
	/* Sets the console input mode. */
	static void setConsoleInputMode(ConsoleInputModes inputMode);
	/* Gets the console output mode. */
	static ConsoleOutputModes getConsoleOutputMode();
	/* Sets the console output mode. */
	static void setConsoleOutputMode(ConsoleOutputModes outputMode);

	#pragma endregion
	//--------------------------------
	#pragma region Console Dimensions

	/* Gets the size of the console screen. */
	static PcGeo::Point2I getConsoleSize();
	/* Sets the size of the console screen. */
	static void setConsoleSize(PcGeo::Point2I consoleSize);
	/* Gets the position of the console buffer. */
	static PcGeo::Point2I getBufferPosition();
	/* Sets the position of the console buffer. */
	static void setBufferPosition(PcGeo::Point2I bufferPosition);
	/* Gets the size of the console buffer. */
	static PcGeo::Point2I getBufferSize();
	/* Sets the size of the console buffer. */
	static void setBufferSize(PcGeo::Point2I bufferSize);
	/* Sets the dimensions of the console screen. */
	static void setConsoleScreen(PcGeo::Point2I consoleSize, PcGeo::Point2I bufferPosition, PcGeo::Point2I bufferSize);
	/* Gets the maximum size of the console window. */
	static PcGeo::Point2I getConsoleMaxSize();
	/* Gets the minimum size of the console window. */
	static PcGeo::Point2I getConsoleMinSize();

	#pragma endregion
	//--------------------------------
	#pragma region Window Dimensions

	/* Gets the size of the window in pixels. */
	static PcGeo::Point2I getWindowSize();
	/* Gets the client size of the window in pixels. */
	static PcGeo::Point2I getClientSize();
	/* Gets the position of the window in pixels. */
	static PcGeo::Point2I getWindowPosition();
	/* Sets the position of the window in pixels. */
	static void setWindowPosition(PcGeo::Point2I position);
	/* Gets the client position of the window in pixels. */
	static PcGeo::Point2I getClientPosition();
	/* Sets the client position of the window in pixels. */
	static void setClientPosition(PcGeo::Point2I position);
	/* Gets the rectangle of the current monitor the window is most displayed in. */
	static PcGeo::Rectangle2I getCurrentMonitor();

	#pragma endregion
	//--------------------------------
	#pragma region Console Visuals

	/* Gets the specified pallete color used by the console. */
	static PcDraw::ColorB getPaletteColor(int index);
	/* Sets the specified pallete color used by the console. */
	static void setPaletteColor(int index, PcDraw::ColorB color);
	/* Gets the list of 16 pallete colors used by the console. */
	static ConsolePalette getPalette();
	/* Sets the list of 16 pallete colors used by the console. */
	static void setPalette(const ConsolePalette& palette);
	/* Gets the size of the console font. */
	static PcGeo::Point2I getFontSize();
	/* Gets the raster size enum of the console font. */
	static RasterFontSizes getRasterFontSize();
	/* Gets the true type size of the console font. */
	static int getTrueTypeFontSize();
	/* Sets the size of the console font. */
	static void setFontSize(PcGeo::Point2I size);
	/* Sets the size of the raster console font. */
	static void setRasterFontSize(RasterFontSizes rasterSize);
	/* Sets the size of the true type console font. */
	static void setTrueTypeFontSize(int trueTypeSize);
	/* Gets the console font type. */
	static ConsoleFonts getFontType();
	/* Sets the console font type. */
	static void setFontType(ConsoleFonts font);
	/* Gets the console font weight. */
	static ConsoleFontWeights getFontWeight();
	/* Sets the console font weight. */
	static void setFontWeight(ConsoleFontWeights weight);
	/* Gets the console font. */
	static ConsoleFont getFont();
	/* Sets the console font. */
	static void setFont(ConsoleFont font);

	#pragma endregion
	//--------------------------------
	#pragma region Window Visuals

	/* Gets the title of the window. */
	static std::string getTitle();
	/* Sets the title of the window. */
	static void setTitle(const std::string& title);
	/* Sets the window icon to the specified file. */
	static void setIconFromFile(const std::string& path, int index = 0);
	/* Sets the window icon to the specified resource in this application. */
	static void setIconFromResourceByIndex(int index);
	/* Sets the window icon to the specified resource in this application. */
	static void setIconFromResourceById(int id);
	/* Gets whether the mouse is visible when inside the window. */
	static bool isMouseVisible();
	/* Sets whether the mouse is visible when inside the window. */
	static void setMouseVisible(bool visible);

	#pragma endregion
	//--------------------------------
	#pragma region Window Settings

	/* Gets whether the window is resizable. */
	static bool isResizable();
	/* Sets whether the window is resizable. */
	static void setResizable(bool resizable);
	/* Gets if the window is using a tool window border style. */
	static bool hasToolWindowBorder();
	/* Sets if the window is using a tool window border style. */
	static void setToolWindowBorder(bool enabled);
	/* Gets if the misc menu items when right clicking the title bar are enabled. */
	static bool isMenuItemsEnabled();
	/* Sets if the misc menu items when right clicking the title bar are enabled. */
	static void setMenuItemsEnabled(bool enabled);
	/* Gets if the close button is enabled. */
	static bool isCloseEnabled();
	/* Sets if the close button is enabled. */
	static void setCloseEnabled(bool enabled);
	/* Gets if the maximize button is enabled. */
	static bool isMaximizeEnabled();
	/* Sets if the maximize button is enabled. */
	static void setMaximizeEnabled(bool enabled);
	/* Gets if the minimize button is enabled. */
	static bool isMinimizeEnabled();
	/* Sets if the minimize button is enabled. */
	static void setMinimizeEnabled(bool enabled);

	#pragma endregion
	//--------------------------------
	#pragma region Handles

	/* Gets the handle to the console window. */
	static HWND getWindowHandle();
	/* Gets the console output handle. */
	static HANDLE getOuputHandle();
	/* Gets the console input handle. */
	static HANDLE getInputHandle();

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management

	/* Gets whether the window is active. */
	static bool isActive();
	/* Activates the window. */
	static void activate();
	/* Maximizes the console window. */
	static void maximizeWindow();
	/* Minimizes the console window. */
	static void minimizeWindow();
	/* Shows the console window. */
	static void showWindow();
	/* Hides the console window. */
	static void hideWindow();
	/* Returns true if the console window is visible. */
	static bool isWindowVisible();
	/* Centers the window. */
	static void centerWindow();
	/* Pauses the console application until a key is pressed. */
	static void pause();
	/* Pauses the console application with a custom message until a key is pressed. */
	static void pause(const std::string& message);
	/* Clears the console screen. */
	static void clear();

	/* Flashes the window. */
	static void flashWindow();
	/* Plays the specified systom sound. */
	static void playSystemSound(SystemSounds sound, bool async = true);
	/* Plays the specified sound resource. */
	static void playSoundFromResourceById(int id, bool async = true);
	/* Plays the specified sound file. */
	static void playSoundFromFile(const std::string& path, bool async = true);

	#pragma endregion
	//============ OUTPUT ============
	#pragma region Output

	/* Reads the next line from the console input. */
	static std::string readLine();

	/* Writes an image to the console. */
	static void writeImage(const PcDraw::AsciiImageFrame& image, int indentation = 0);
	/* Writes the specified value as text to the console. */
	template<typename T> static void write(T val);
	/* Writes the specified value as a line to the console. */
	template<typename T> static void writeLine(T val);
	/* Writes the specified value as text to the visual studio debug console. */
	template<typename T> static void debug(T val);
	/* Writes the specified value as a line to the visual studio debug console. */
	template<typename T> static void debugLine(T val);

	#pragma endregion
};

template<typename T> void Console::write(T val) {
	console->write(val);
}
template<typename T> void Console::writeLine(T val) {
	console->writeLine(val);
}
template<typename T> void Console::debug(T val) {
	console->debug(val);
}
template<typename T> void Console::debugLine(T val) {
	console->debugLine(val);
}

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
