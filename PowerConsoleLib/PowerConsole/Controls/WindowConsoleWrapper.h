/*=================================================================|
|* File:				WindowConsoleWrapper.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef WINDOW_CONSOLE_WRAPPER_H
#define WINDOW_CONSOLE_WRAPPER_H

#include <array>
#include <memory>
#include <PowerConsole/ConsoleSettings.h>

namespace PowerConsole::Controls {
DECLARE_CLASS_AND_SMART_PTRS(WindowConsoleWrapper);
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A wrapper for the ConsoleSettings class for use with Windows. This leaves out functionality that should not be used with windows. */
class WindowConsoleWrapper {
	//=========== MEMBERS ============
	#pragma region Members

	/* The console settings to use. */
	Pc::ConsoleSettingsSPtr console;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs an empty window console wrapper. */
	WindowConsoleWrapper();
	/* Constructs the window console wrapper. */
	WindowConsoleWrapper(Pc::ConsoleSettingsSPtr console);
	/* Changes all the console settings to their default. */
	void loadDefaults(bool resetDimensions = false);

	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Console Dimensions

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
	void setRasterFontSize(Pc::RasterFontSizes rasterSize);
	/* Sets the size of the true type console font. */
	void setTrueTypeFontSize(int trueTypeSize);
	/* Gets the console font type. */
	ConsoleFonts getFontType();
	/* Sets the console font type. */
	void setFontType(Pc::ConsoleFonts font);
	/* Gets the console font weight. */
	Pc::ConsoleFontWeights getFontWeight();
	/* Sets the console font weight. */
	void setFontWeight(Pc::ConsoleFontWeights weight);
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
	//bool isResizable();
	/* Sets whether the window is resizable. */
	//void setResizable(bool resizable);
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
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management
public:
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

	/* Flashes the window. */
	void flashWindow();
	/* Plays the specified systom sound. */
	void playSystemSound(SystemSounds sound, bool async = true);
	/* Plays the specified sound resource. */
	void playSoundFromResourceById(int id, bool async = true);
	/* Plays the specified sound file. */
	void playSoundFromFile(const std::string& path, bool async = true);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
