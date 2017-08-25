/*=================================================================|
|* File:				ConsoleSettings.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONSOLE_SETTINGS_H
#define CONSOLE_SETTINGS_H

#include <string>
#include <iostream>
#include <sstream>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <PowerConsole/ConsoleFont.h>
#include <PowerConsole/ConsolePalette.h>
#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/ConsoleEnums.h>

DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(PowerConsole::Controls, Window);

namespace PowerConsole {
DECLARE_CLASS(Console);
DECLARE_CLASS_AND_SMART_PTRS(ConsoleSettings);
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A wrapper for managing changes to the console. */
class ConsoleSettings {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The number of different colors the console can use. */
	static const int NUM_PALETTE_COLORS = 16;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
	//--------------------------------
	#pragma region Handles

	/* The console output handle. */
	HANDLE hOut;
	/* The console input handle. */
	HANDLE hIn;
	/* The console window handle. */
	HWND hWnd;

	#pragma endregion
	//--------------------------------
	#pragma region PowerConsole Data

	// Window
	/* The window containing these settings. */
	PcCtrls::WindowWPtr owner;

	// Buffer
	/* The size of the buffer. */
	PcGeo::Point2I bufferSize;
	/* The current character buffer. */
	CHAR_INFO* buffer;
	/* The previous character buffer. */
	CHAR_INFO* bufferLast;

	// Loading
	/* True if the console settings has loaded. */
	bool loaded;

	#pragma endregion
	//--------------------------------
	#pragma region Console Data

	// Console Info
	/* The current screen info. */
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfo;
	/* The current font info. */
	CONSOLE_FONT_INFOEX fontInfo;
	/* The current cursor info. */
	CONSOLE_CURSOR_INFO cursorInfo;

	// IO Modes
	/* The current input mode. */
	ConsoleInputModes inputMode;
	/* The current output mode. */
	ConsoleOutputModes outputMode;

	#pragma endregion
	//--------------------------------
	#pragma region Window Data

	// Window Info
	/* The current window info. */
	WINDOWINFO windowInfo;
	/* True if the mouse is currently visible. */
	bool mouseVisible;
	/* True if the window is currently active. */
	bool active;

	// Menu items
	/* True if the console menu items are currently enabled. */
	bool menuItemsEnabled;
	/* True if the close button is currently enabled. */
	bool closeButtonEnabled;

	// Window Visuals
	/* The current title of the window. */
	std::string windowTitle;
	/* The current big icon used by the console window. */
	HICON hIcon;
	/* The current small icon used by the console window. */
	HICON hIconSm;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the console settings. */
	ConsoleSettings(bool loaded = false);
	/* Deconstructs console settings. */
	~ConsoleSettings();
	/* Starts the console and begins applying changes when made. */
	void start();
	/* Changes all the console settings to their default. */
	void loadDefaults(bool resetDimensions = false);

	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region PowerConsole

	/* Sets the parent of the console settings. */
	void setOwner(PcCtrls::WindowSPtr owner);

	#pragma endregion
	//--------------------------------
	#pragma region Console Cursor

	/* Gets the console cursor position. */
	PcGeo::Point2I getCursorPosition();
	/* Sets the console cursor position. */
	void setCursorPosition(PcGeo::Point2I position);
	/* Gets the color of text written to the console. */
	unsigned char getCurrentColor();
	/* Sets the color of text written to the console. */
	void setCurrentColor(unsigned char color);
	/* Gets the console cursor fill. */
	int getCursorFill();
	/* Sets the console cursor fill. */
	void setCursorFill(int fill);
	/* Gets if the console cursor is visible. */
	bool isCursorVisible();
	/* Sets if the console cursor is visible. */
	void setCursorVisible(bool visible);

	#pragma endregion
	//--------------------------------
	#pragma region Console IO Mode

	/* Gets the console input mode. */
	ConsoleInputModes getConsoleInputMode();
	/* Sets the console input mode. */
	void setConsoleInputMode(ConsoleInputModes inputMode);
	/* Gets the console output mode. */
	ConsoleOutputModes getConsoleOutputMode();
	/* Sets the console output mode. */
	void setConsoleOutputMode(ConsoleOutputModes outputMode);

	#pragma endregion
	//--------------------------------
	#pragma region Console Dimensions

	/* Gets the size of the console screen. */
	PcGeo::Point2I getConsoleSize();
	/* Sets the size of the console screen. */
	void setConsoleSize(PcGeo::Point2I consoleSize);
	/* Gets the position of the console buffer. */
	PcGeo::Point2I getBufferPosition();
	/* Sets the position of the console buffer. */
	void setBufferPosition(PcGeo::Point2I bufferPosition);
	/* Gets the size of the console buffer. */
	PcGeo::Point2I getBufferSize();
	/* Sets the size of the console buffer. */
	void setBufferSize(PcGeo::Point2I bufferSize);
	/* Sets the dimensions of the console screen. */
	void setConsoleScreen(PcGeo::Point2I consoleSize, PcGeo::Point2I bufferPosition, PcGeo::Point2I bufferSize);
	/* Gets the maximum size of the console window. */
	PcGeo::Point2I getConsoleMaxSize();
	/* Gets the minimum size of the console window. */
	PcGeo::Point2I getConsoleMinSize();

	#pragma endregion
	//--------------------------------
	#pragma region Window Dimensions

	/* Gets the size of the window in pixels. */
	PcGeo::Point2I getWindowSize();
	/* Gets the client size of the window in pixels. */
	PcGeo::Point2I getClientSize();
	/* Gets the position of the window in pixels. */
	PcGeo::Point2I getWindowPosition();
	/* Sets the position of the window in pixels. */
	void setWindowPosition(PcGeo::Point2I position);
	/* Gets the client position of the window in pixels. */
	PcGeo::Point2I getClientPosition();
	/* Sets the client position of the window in pixels. */
	void setClientPosition(PcGeo::Point2I position);
	/* Gets the rectangle of the current monitor the window is most displayed in. */
	PcGeo::Rectangle2I getCurrentMonitor();

	#pragma endregion
	//--------------------------------
	#pragma region Console Visuals

	/* Gets the specified pallete color used by the console. */
	PcDraw::ColorB getPaletteColor(int index);
	/* Sets the specified pallete color used by the console. */
	void setPaletteColor(int index, PcDraw::ColorB color);
	/* Gets the list of 16 pallete colors used by the console. */
	ConsolePalette getPalette();
	/* Sets the list of 16 pallete colors used by the console. */
	void setPalette(const ConsolePalette& palette);
	/* Gets the size of the console font. */
	PcGeo::Point2I getFontSize();
	/* Gets the raster size enum of the console font. */
	RasterFontSizes getRasterFontSize();
	/* Gets the true type size of the console font. */
	int getTrueTypeFontSize();
	/* Sets the size of the console font. */
	void setFontSize(PcGeo::Point2I size);
	/* Sets the size of the raster console font. */
	void setRasterFontSize(RasterFontSizes rasterSize);
	/* Sets the size of the true type console font. */
	void setTrueTypeFontSize(int trueTypeSize);
	/* Gets the console font type. */
	ConsoleFonts getFontType();
	/* Sets the console font type. */
	void setFontType(ConsoleFonts font);
	/* Gets the console font weight. */
	ConsoleFontWeights getFontWeight();
	/* Sets the console font weight. */
	void setFontWeight(ConsoleFontWeights weight);
	/* Gets the console font. */
	ConsoleFont getFont();
	/* Sets the console font. */
	void setFont(ConsoleFont font);

	#pragma endregion
	//--------------------------------
	#pragma region Window Visuals

	/* Gets the title of the window. */
	std::string getTitle();
	/* Sets the title of the window. */
	void setTitle(const std::string& title);
	/* Sets the window icon to the specified file. */
	void setIconFromFile(const std::string& path, int index = 0);
	/* Sets the window icon to the specified resource in this application. */
	void setIconFromResourceByIndex(int index);
	/* Sets the window icon to the specified resource in this application. */
	void setIconFromResourceById(int id);
	/* Gets whether the mouse is visible when inside the window. */
	bool isMouseVisible();
	/* Sets whether the mouse is visible when inside the window. */
	void setMouseVisible(bool visible);

	#pragma endregion
	//--------------------------------
	#pragma region Window Settings

	/* Gets whether the window is resizable. */
	bool isResizable();
	/* Sets whether the window is resizable. */
	void setResizable(bool resizable);
	/* Gets if the window is using a tool window border style. */
	bool hasToolWindowBorder();
	/* Sets if the window is using a tool window border style. */
	void setToolWindowBorder(bool enabled);
	/* Gets if the misc menu items when right clicking the title bar are enabled. */
	bool isMenuItemsEnabled();
	/* Sets if the misc menu items when right clicking the title bar are enabled. */
	void setMenuItemsEnabled(bool enabled);
	/* Gets if the close button is enabled. */
	bool isCloseEnabled();
	/* Sets if the close button is enabled. */
	void setCloseEnabled(bool enabled);
	/* Gets if the maximize button is enabled. */
	bool isMaximizeEnabled();
	/* Sets if the maximize button is enabled. */
	void setMaximizeEnabled(bool enabled);
	/* Gets if the minimize button is enabled. */
	bool isMinimizeEnabled();
	/* Sets if the minimize button is enabled. */
	void setMinimizeEnabled(bool enabled);

	#pragma endregion
	//--------------------------------
	#pragma region Handles

	/* Gets the handle to the console window. */
	HWND getWindowHandle();
	/* Gets the console output handle. */
	HANDLE getOuputHandle();
	/* Gets the console input handle. */
	HANDLE getInputHandle();

	#pragma endregion
	//--------------------------------
	#pragma region Input

	/* Gets the input record structure. */
	INPUT_RECORD nextInputRecord();
	/* Clears all remaining input records in the buffer. */
	void clearInputRecords();

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management

	/* Gets whether the window is active. */
	bool isActive();
	/* Activates the window. */
	void activate();
	/* Maximizes the console window. */
	void maximizeWindow();
	/* Minimizes the console window. */
	void minimizeWindow();
	/* Shows the console window. */
	void showWindow();
	/* Hides the console window. */
	void hideWindow();
	/* Returns true if the console window is visible. */
	bool isWindowVisible();
	/* Centers the window. */
	void centerWindow();
	/* Pauses the console application until a key is pressed. */
	void pause();
	/* Pauses the console application with a custom message until a key is pressed. */
	void pause(const std::string& message);
	/* Clears the console screen. */
	void clear();

	/* Flashes the window. */
	void flashWindow();
	/* Plays the specified systom sound. */
	void playSystemSound(SystemSounds sound, bool async = true);
	/* Plays the specified sound resource. */
	void playSoundFromResourceById(int id, bool async = true);
	/* Plays the specified sound file. */
	void playSoundFromFile(const std::string& path, bool async = true);

	#pragma endregion
	//============ OUTPUT ============
	#pragma region Output

	/* Reads the next line from the console input. */
	std::string readLine();

	/* Writes an image to the console. */
	void writeImage(const PcDraw::AsciiImageFrame& image, int indentation = 0);
	/* Writes the specified value as text to the console. */
	template<typename T> void write(T val);
	/* Writes the specified value as a line to the console. */
	template<typename T> void writeLine(T val);
	/* Writes the specified value as text to the visual studio debug console. */
	template<typename T> void debug(T val);
	/* Writes the specified value as a line to the visual studio debug console. */
	template<typename T> void debugLine(T val);

	#pragma endregion
	//========= CONSOLE MISC =========
	#pragma region Console Misc
private:
	/* Writes the specified buffer to the console. */
	void writeBuffer(const PcDraw::AsciiImageFrame& image, bool forceRedraw);
	/* Resizes the drawing buffer. */
	void resizeBuffer(PcGeo::Point2I newSize);

	/* Checks for changes and updates the settings accordingly. */
	void checkForChanges();
	/* Applies changes to the console settings. */
	void applyChanges();
	/* Applies changes to the console screen info. */
	void applyScreenInfoChanges(CONSOLE_SCREEN_BUFFER_INFOEX& screenInfoNew, bool loading = false);
	/* Applies changes to the console font info. */
	void applyFontInfoChanges(CONSOLE_FONT_INFOEX& fontInfoNew, bool loading = false);
	/* Applies changes to the console cursor info. */
	void applyCursorInfoChanges(CONSOLE_CURSOR_INFO& cursorInfoNew, bool loading = false);
	/* Applies changes to the console input mode. */
	void applyInputModeChanges(ConsoleInputModes inputModeNew, bool loading = false);
	/* Applies changes to the console output mode. */
	void applyOutputModeChanges(ConsoleOutputModes outputModeNew, bool loading = false);
	/* Applies changes to the window style. */
	void applyWindowStyleChanges(DWORD styleNew, bool loading = false);
	/* Applies changes to the window exstyle. */
	void applyWindowExStyleChanges(DWORD exStyleNew, bool loading = false);
	/* Applies changes to the window position. */
	void applyWindowPositionChanges(WINDOWINFO& windowInfoNew, bool loading = false);
	/* Applies changes to the window title. */
	void applyWindowTitleChanges(const std::string& windowTitleNew, bool loading = false);
	/* Applies changes to the window icon. */
	void applyWindowIconChanges(HICON hIconNew, HICON hIconSmNew, bool loading = false);
	/* Applies changes to the mouse visibility. */
	void applyMouseChanges(bool mouseVisibleNew, bool loading = false);
	/* Applies changes to the system menu. */
	void applySystemMenuChanges(bool closeButtonEnabledNew, bool menuItemsEnabledNew, bool loading = false);

	#pragma endregion
	//=========== FRIENDS ============
	#pragma region Friends

	friend Console;
	friend PcCtrls::Window;

	#pragma endregion
};


template<typename T> void ConsoleSettings::write(T val) {
	std::cout << val;
}
template<typename T> void ConsoleSettings::writeLine(T val) {
	std::cout << val << std::endl;
}
template<typename T> void ConsoleSettings::debug(T val) {
	#ifdef _DEBUG
	std::ostringstream oss = std::ostringstream();
	oss << val;
	OutputDebugStringA(oss.str().c_str());
	#endif
}
template<typename T> void ConsoleSettings::debugLine(T val) {
	#ifdef _DEBUG
	std::ostringstream oss = std::ostringstream();
	oss << val << std::endl;
	OutputDebugStringA(oss.str().c_str());
	#endif
}

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
