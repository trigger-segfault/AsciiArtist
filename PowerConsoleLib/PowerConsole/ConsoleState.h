/*=================================================================|
|* File:				ConsoleState.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONSOLE_STATE_H
#define CONSOLE_STATE_H

#include <string>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/ConsoleEnums.h>

namespace PowerConsole {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ConsoleState {

	//=========== MEMBERS ============
	#pragma region Members
	//--------------------------------
	#pragma region General

	/* True if the console state have been loaded. */
	bool loaded;

	#pragma endregion
	//--------------------------------
	#pragma region Handles

	/* The console window handle. */
	HWND hWnd;
	/* The console output handle. */
	HANDLE hOut;
	/* The console input handle. */
	HANDLE hIn;

	#pragma endregion
	//--------------------------------
	#pragma region Console Data
	
	// Console Info
	/* The default screen info. */
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfo;
	/* The default cursor info. */
	CONSOLE_CURSOR_INFO cursorInfo;
	/* The default font info. */
	CONSOLE_FONT_INFOEX fontInfo;

	// IO Mode
	/* The default input mode. */
	ConsoleInputModes inputMode;
	/* The default output mode. */
	ConsoleOutputModes outputMode;

	#pragma endregion
	//--------------------------------
	#pragma region Window Data

	// Window Info
	/* The default window info. */
	WINDOWINFO windowInfo;
	/* Greater than or equal to zero if the mouse is visible. */
	int mouseVisibleState;

	// Window Visuals
	/* The default title of the window. */
	std::string windowTitle;
	/* The default big icon used by the console window. */
	HICON hIcon;
	/* The default small icon used by the console window. */
	HICON hIconSm;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs an unloaded console state. */
	ConsoleState();

	#pragma endregion
	//============ STATE =============
	#pragma region Statee

	/* Stores the current console state. */
	void storeCurrentState(bool clearScreen = true);
	/* Reverts to the stored console state. */
	void revertToStoredState(bool clearScreen = true);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
