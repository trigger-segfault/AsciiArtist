/*=================================================================|
|* File:				ConsoleState.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ConsoleState.h"
#include <PowerConsole/HiddenWinAPI.h>
#include <PowerConsole/SystemMenuManager.h>

using namespace PowerConsole;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

ConsoleState::ConsoleState()
 :	// General
	loaded(false),

	// Handles
	hWnd(NULL),
	hOut(NULL),
	hIn(NULL),

	// Console Data
	screenInfo({ sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) }),
	cursorInfo(),
	fontInfo({ sizeof(CONSOLE_FONT_INFOEX) }),
	inputMode(ConsoleInputModes::None),
	outputMode(ConsoleOutputModes::None),

	// Window Data
	windowInfo({ sizeof(WINDOWINFO) }),
	mouseVisibleState(0),
	windowTitle(""),
	hIcon(NULL),
	hIconSm(NULL) {}

#pragma endregion
//=========== LOADING ============
#pragma region Loading

void ConsoleState::storeCurrentState(bool clearScreen) {
	// Clear the screen to start off

	// Handles
	hWnd = GetConsoleWindow();
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hIn = GetStdHandle(STD_INPUT_HANDLE);

	// Screen Info
	GetConsoleScreenBufferInfoEx(hOut, &screenInfo);

	// Font Info
	GetCurrentConsoleFontEx(hOut, false, &fontInfo);

	// Cursor Info
	GetConsoleCursorInfo(hOut, &cursorInfo);

	// IO Mode
	GetConsoleMode(hIn, (LPDWORD)&inputMode);
	GetConsoleMode(hOut, (LPDWORD)&outputMode);

	// Window Style
	GetWindowInfo(hWnd, &windowInfo);

	// Window Title
	char cTitle[MAX_PATH] = { 0 };
	GetConsoleTitleA(cTitle, MAX_PATH);
	windowTitle = cTitle;

	// Window Icons
	WNDCLASSEXA wndClass = { sizeof(WNDCLASSEXA) };
	char cClassName[MAX_PATH] = { 0 };
	// Should be "ConsoleWindowClass" but let's be safe and get it anyway.
	GetClassNameA(hWnd, cClassName, MAX_PATH);
	GetClassInfoExA(nullptr, cClassName, &wndClass);
	hIcon = wndClass.hIcon;
	hIconSm = wndClass.hIconSm;

	// Mouse Visible
	mouseVisibleState = ShowConsoleCursor(hOut, true) - 1;
	ShowConsoleCursor(hOut, false);

	if (clearScreen) {
		int length = (int)screenInfo.dwSize.X * (int)screenInfo.dwSize.Y;
		CHAR_INFO* buffer = new CHAR_INFO[length];
		for (int i = 0; i < length; i++) {
			buffer[i].Char.AsciiChar = ' ';
			buffer[i].Attributes = screenInfo.wAttributes;
		}
		SMALL_RECT srect = { 0, 0, screenInfo.dwSize.X - 1, screenInfo.dwSize.Y - 1 };
		COORD zero = { 0, 0 };
		WriteConsoleOutputA(hOut, buffer, screenInfo.dwSize, zero, &srect);
		delete[] buffer;
	}

	loaded = true;
}
void ConsoleState::revertToStoredState(bool clearScreen) {
	if (loaded) {
		SystemMenuManager::setMenuItemsEnabled(true, true);

		// Mouse Visible
		int visibleState = ShowConsoleCursor(hOut, true);
		if (visibleState > mouseVisibleState)
			while (ShowConsoleCursor(hOut, false) > mouseVisibleState);
		else if (visibleState < mouseVisibleState)
			while (ShowConsoleCursor(hOut, true) < mouseVisibleState);

		// Window Icons
		SendMessageA(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessageA(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
		SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);

		// Window Title
		SetConsoleTitleA(windowTitle.c_str());

		// Window Style
		SetWindowLongA(hWnd, GWL_STYLE, windowInfo.dwStyle);
		SetWindowLongA(hWnd, GWL_EXSTYLE, windowInfo.dwExStyle);

		// IO Mode
		SetConsoleMode(hIn, (DWORD)inputMode);
		SetConsoleMode(hOut, (DWORD)outputMode);

		// Cursor Info
		SetConsoleCursorInfo(hOut, &cursorInfo);

		// Font Info
		SetCurrentConsoleFontEx(hOut, false, &fontInfo);

		// Screen Info
		screenInfo.srWindow.Right += 1;
		screenInfo.srWindow.Bottom += 1;
		SetConsoleScreenBufferInfoEx(hOut, &screenInfo);
		screenInfo.srWindow.Right -= 1;
		screenInfo.srWindow.Bottom -= 1;

		// Clear the screen now that we're done
		if (clearScreen) {
			int length = (int)screenInfo.dwSize.X * (int)screenInfo.dwSize.Y;
			CHAR_INFO* buffer = new CHAR_INFO[length];
			for (int i = 0; i < length; i++) {
				buffer[i].Char.AsciiChar = ' ';
				buffer[i].Attributes = screenInfo.wAttributes;
			}
			SMALL_RECT srect = { 0, 0, screenInfo.dwSize.X - 1, screenInfo.dwSize.Y - 1 };
			COORD zero = { 0, 0 };
			WriteConsoleOutputA(hOut, buffer, screenInfo.dwSize, zero, &srect);
			delete[] buffer;
		}
	}
}

#pragma endregion
//=================================================================|
