/*=================================================================|
|* File:				ConsoleSettings.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ConsoleSettings.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/HiddenWinAPI.h>
#include <PowerConsole/SystemMenuManager.h>
#include <PowerConsole/Controls/Window.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/PowerConsoleSettings.h>

using namespace PowerConsole;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
using namespace PowerConsole::Helpers;

#define basicMode (this->owner.expired())
#define windowMode (!this->owner.expired())
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

ConsoleSettings::ConsoleSettings(bool loaded) {
	this->loaded = loaded;

	// Buffer
	bufferSize = Point2I::ZERO;
	buffer = nullptr;
	bufferLast = nullptr;
	// Handles
	hWnd = GetConsoleWindow();
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hIn = GetStdHandle(STD_INPUT_HANDLE);

	// Screen Info
	screenInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(hOut, &screenInfo);

	// Font Info
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hOut, false, &fontInfo);

	// Cursor Info
	GetConsoleCursorInfo(hOut, &cursorInfo);

	// IO Mode
	GetConsoleMode(hIn, (LPDWORD)&inputMode);
	GetConsoleMode(hOut, (LPDWORD)&outputMode);

	// Window Style
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &windowInfo);

	// Window Title
	char cTitle[MAX_PATH] = { 0 };
	GetConsoleTitleA(cTitle, MAX_PATH);
	windowTitle = cTitle;

	// Window Icons
	hIcon = nullptr;
	hIconSm = nullptr;

	// Mouse Visible
	mouseVisible = ShowConsoleCursor(hOut, true) > 0;
	ShowConsoleCursor(hOut, false);

	// Window Active
	active = (GetForegroundWindow() == hWnd);

	// Menu Items
	closeButtonEnabled = SystemMenuManager::isCloseMenuItemEnabled();
	menuItemsEnabled = SystemMenuManager::isConsoleMenuItemsEnabled();
}
ConsoleSettings::~ConsoleSettings() {
	// Clean up custom icons if they were used
	if (hIcon != nullptr) DestroyIcon(hIcon);
	if (hIconSm != nullptr) DestroyIcon(hIconSm);
}
void ConsoleSettings::start() {
	if (loaded)
		throw std::exception("Cannot start console when already loaded");

	// This is to fix and force-disable the quick edit mode that breaks mouse input events
	if (windowMode) {
		ConsoleInputModes newInputMode = ConsoleInputModes::EchoInput | ConsoleInputModes::LineInput | ConsoleInputModes::ExtendedFlags | ConsoleInputModes::AutoPosition;
		SetConsoleMode(hIn, (DWORD)newInputMode);
	}

	applyChanges();

	loaded = true;
}
void ConsoleSettings::loadDefaults(bool resetDimensions) {
	#pragma region Screen Info

	if (resetDimensions) {
		if (windowMode) {
			screenInfo.srWindow = Rectangle2I(80, 25);
			screenInfo.dwSize = Point2I(80, 25);
		}
		else {
			screenInfo.srWindow = Rectangle2I(80, 25);
			screenInfo.dwSize = Point2I(80, 300);
		}
	}
	screenInfo.dwMaximumWindowSize = GetLargestConsoleWindowSize(hOut);
	screenInfo.bFullscreenSupported = false;

	// Cursor
	screenInfo.dwCursorPosition = Point2I::ZERO;

	// Attributes
	screenInfo.wAttributes = 0x07;
	screenInfo.wPopupAttributes = 0x5F;

	// Color Table
	screenInfo.ColorTable[0] = RGB(0, 0, 0);
	screenInfo.ColorTable[1] = RGB(0, 0, 128);
	screenInfo.ColorTable[2] = RGB(0, 128, 0);
	screenInfo.ColorTable[3] = RGB(0, 128, 128);
	screenInfo.ColorTable[4] = RGB(128, 0, 0);
	screenInfo.ColorTable[5] = RGB(128, 0, 128);
	screenInfo.ColorTable[6] = RGB(128, 128, 0);
	screenInfo.ColorTable[7] = RGB(192, 192, 192);
	screenInfo.ColorTable[8] = RGB(128, 128, 128);
	screenInfo.ColorTable[9] = RGB(0, 0, 255);
	screenInfo.ColorTable[10] = RGB(0, 255, 0);
	screenInfo.ColorTable[11] = RGB(0, 255, 255);
	screenInfo.ColorTable[12] = RGB(255, 0, 0);
	screenInfo.ColorTable[13] = RGB(255, 0, 255);
	screenInfo.ColorTable[14] = RGB(255, 255, 0);
	screenInfo.ColorTable[15] = RGB(255, 255, 255);

	#pragma endregion

	#pragma region Font Info

	wcscpy_s(fontInfo.FaceName, L"Terminal");
	fontInfo.dwFontSize = getRasterSize(RasterFontSizes::R8x12);
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_REGULAR;

	#pragma endregion

	#pragma region Cursor Info

	cursorInfo.bVisible = basicMode;
	cursorInfo.dwSize = 25;

	#pragma endregion

	#pragma region IO Mode

	if (windowMode) {
		inputMode =
			ConsoleInputModes::EchoInput | ConsoleInputModes::MouseInput;
	}
	else {
		inputMode = ConsoleInputModes::ProcessedInput | ConsoleInputModes::LineInput |
			ConsoleInputModes::EchoInput | ConsoleInputModes::InsertMode |
			ConsoleInputModes::ExtendedFlags | ConsoleInputModes::AutoPosition;
	}
	outputMode = ConsoleOutputModes::ProcessedOutput | ConsoleOutputModes::WrapAtEolOutput;

	#pragma endregion

	#pragma region Menu Items

	closeButtonEnabled = true;
	///TODO: Windows 10 has fucked everything up. Leave access to the
	///      Properties menu so that users can fix all the broken shit.
	menuItemsEnabled = true; //basicMode;

	#pragma endregion

	#pragma region Mouse

	mouseVisible = true;
	int visibleState = ShowConsoleCursor(hOut, true);
	if (visibleState > 0)
		while (ShowConsoleCursor(hOut, false) > 0);
	else if (visibleState < 0)
		while (ShowConsoleCursor(hOut, true) < 0);

	#pragma endregion

	#pragma region Window Style

	setFlag(windowInfo.dwStyle, WS_MAXIMIZEBOX, false);
	setFlag(windowInfo.dwStyle, WS_THICKFRAME, false);

	#pragma endregion

	if (loaded) {
		applyChanges();
	}
}

//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region PowerConsole

void ConsoleSettings::setOwner(WindowSPtr owner) {
	this->owner = owner;
}

#pragma endregion
//--------------------------------
#pragma region Console Cursor

Point2I ConsoleSettings::getCursorPosition() {
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfo);

	return screenInfo.dwCursorPosition;
}
void ConsoleSettings::setCursorPosition(Point2I position) {
	if (SetConsoleCursorPosition(hOut, position) != 0) {
		if (windowMode)
			screenInfo.dwCursorPosition = position;
	}
}
unsigned char ConsoleSettings::getCurrentColor() {
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfo);

	return (unsigned char)screenInfo.wAttributes;
}
void ConsoleSettings::setCurrentColor(unsigned char color) {
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfoNew = screenInfo;
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);

	screenInfoNew.wAttributes = (WORD)color;

	applyScreenInfoChanges(screenInfoNew);
}
int ConsoleSettings::getCursorFill() {
	if (basicMode)
		GetConsoleCursorInfo(hOut, &cursorInfo);

	return (int)cursorInfo.dwSize;
}
void ConsoleSettings::setCursorFill(int fill) {
	if (fill < 1 || fill > 100)
		throw std::exception("Fill is not between 1 and 100 in 'Console::setCursorFill(...)'");

	CONSOLE_CURSOR_INFO cursorInfoNew = cursorInfo;
	if (basicMode)
		GetConsoleCursorInfo(hOut, &cursorInfoNew);

	cursorInfoNew.dwSize = (DWORD)fill;

	applyCursorInfoChanges(cursorInfoNew);
}
bool ConsoleSettings::isCursorVisible() {
	if (basicMode)
		GetConsoleCursorInfo(hOut, &cursorInfo);

	return cursorInfo.bVisible != 0;
}
void ConsoleSettings::setCursorVisible(bool visible) {
	CONSOLE_CURSOR_INFO cursorInfoNew = cursorInfo;
	if (basicMode)
		GetConsoleCursorInfo(hOut, &cursorInfoNew);

	cursorInfoNew.bVisible = visible;

	applyCursorInfoChanges(cursorInfoNew);
}

#pragma endregion
//--------------------------------
#pragma region Console IO Mode

ConsoleInputModes ConsoleSettings::getConsoleInputMode() {
	if (basicMode)
		GetConsoleMode(hIn, (LPDWORD)&inputMode);

	return inputMode;
}
void ConsoleSettings::setConsoleInputMode(ConsoleInputModes inputMode) {
	applyInputModeChanges(inputMode);
}
ConsoleOutputModes ConsoleSettings::getConsoleOutputMode() {
	if (basicMode)
		GetConsoleMode(hOut, (LPDWORD)&outputMode);

	return outputMode;
}
void ConsoleSettings::setConsoleOutputMode(ConsoleOutputModes outputMode) {
	applyOutputModeChanges(outputMode);
}

#pragma endregion
//--------------------------------
#pragma region Console Dimensions

Point2I ConsoleSettings::getConsoleSize() {
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfo);

	return Point2I(screenInfo.srWindow, true);
}
void ConsoleSettings::setConsoleSize(Point2I consoleSize) {
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfoNew = screenInfo;
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);

	screenInfoNew.srWindow = Rectangle2I(Point2I(screenInfoNew.srWindow, false), consoleSize);

	applyScreenInfoChanges(screenInfoNew);
}
Point2I ConsoleSettings::getBufferPosition() {
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfo);

	return Point2I(screenInfo.srWindow, false);
}
void ConsoleSettings::setBufferPosition(Point2I bufferPosition) {
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfoNew = screenInfo;
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);

	screenInfoNew.srWindow = Rectangle2I(bufferPosition, Point2I(screenInfoNew.srWindow, true));

	applyScreenInfoChanges(screenInfoNew);
}
Point2I ConsoleSettings::getBufferSize() {
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfo);

	return screenInfo.dwSize;
}
void ConsoleSettings::setBufferSize(Point2I bufferSize) {
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfoNew = screenInfo;
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);

	screenInfoNew.dwSize = bufferSize;

	applyScreenInfoChanges(screenInfoNew);
}
void ConsoleSettings::setConsoleScreen(Point2I consoleSize, Point2I bufferPosition, Point2I bufferSize) {
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfoNew = screenInfo;
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);

	screenInfoNew.srWindow = Rectangle2I(bufferPosition, consoleSize);
	screenInfoNew.dwSize = bufferSize;

	applyScreenInfoChanges(screenInfoNew);
}
Point2I ConsoleSettings::getConsoleMaxSize() {
	return GetLargestConsoleWindowSize(hOut);
}
Point2I ConsoleSettings::getConsoleMinSize() {
	return Point2I(2, 2);
}

#pragma endregion
//--------------------------------
#pragma region Window Dimensions

Point2I ConsoleSettings::getWindowSize() {
	if (basicMode)
		GetWindowInfo(hWnd, &windowInfo);

	return Point2I(windowInfo.rcWindow, true);
}
Point2I ConsoleSettings::getClientSize() {
	if (basicMode)
		GetWindowInfo(hWnd, &windowInfo);

	return Point2I(windowInfo.rcClient, true);
}
Point2I ConsoleSettings::getWindowPosition() {
	if (basicMode)
		GetWindowInfo(hWnd, &windowInfo);

	return Point2I(windowInfo.rcWindow, false);
}
void ConsoleSettings::setWindowPosition(Point2I position) {
	WINDOWINFO windowInfoNew = windowInfo;
	if (basicMode)
		GetWindowInfo(hWnd, &windowInfoNew);

	Point2I windowSize = Rectangle2I(windowInfoNew.rcWindow).size;
	Point2I clientSize = Rectangle2I(windowInfoNew.rcClient).size;
	Point2I windowClientDif = Rectangle2I(windowInfoNew.rcClient).point - Rectangle2I(windowInfoNew.rcWindow).point;
	windowInfoNew.rcWindow = Rectangle2I(position, windowSize);
	windowInfoNew.rcClient = Rectangle2I(position + windowClientDif, clientSize);

	applyWindowPositionChanges(windowInfoNew);
}
Point2I ConsoleSettings::getClientPosition() {
	if (basicMode)
		GetWindowInfo(hWnd, &windowInfo);

	return Point2I(windowInfo.rcClient, false);
}
void ConsoleSettings::setClientPosition(Point2I position) {
	WINDOWINFO windowInfoNew = windowInfo;
	if (basicMode)
		GetWindowInfo(hWnd, &windowInfoNew);

	Point2I windowClientDif = Rectangle2I(windowInfoNew.rcClient).point - Rectangle2I(windowInfoNew.rcWindow).point;

	setWindowPosition(position - windowClientDif);
}
Rectangle2I ConsoleSettings::getCurrentMonitor() {
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
	if (GetMonitorInfoA(hMonitor, &monitorInfo) != 0) {
		Rectangle2I monitorRect = monitorInfo.rcMonitor;
		monitorRect.size -= 1;
		return monitorRect;
	}
	return Rectangle2I::ZERO;
}

#pragma endregion
//--------------------------------
#pragma region Console Visuals

ColorB ConsoleSettings::getPaletteColor(int index) {
	if (index < 0 || index >= NUM_PALETTE_COLORS)
		throw std::out_of_range("Index must be between 0 and 15 in 'ConsoleSettings::getPaletteColor(...)'");

	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfo);

	unsigned int rawColor = screenInfo.ColorTable[index];
	return ColorB(screenInfo.ColorTable[index]);
}
void ConsoleSettings::setPaletteColor(int index, ColorB color) {
	if (index < 0 || index >= NUM_PALETTE_COLORS)
		throw std::out_of_range("Index must be between 0 and 15 in 'ConsoleSettings::setPaletteColor(...)'");

	CONSOLE_SCREEN_BUFFER_INFOEX screenInfoNew = screenInfo;
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);

	screenInfoNew.ColorTable[index] = (COLORREF)color;
	
	applyScreenInfoChanges(screenInfoNew);
}
ConsolePalette ConsoleSettings::getPalette() {
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfo);
	ConsolePalette palette;

	for (int i = 0; i < NUM_PALETTE_COLORS; i++) {
		palette[i] = ColorB(screenInfo.ColorTable[i]);
	}
	return palette;
}
void ConsoleSettings::setPalette(const ConsolePalette& palette) {
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfoNew = screenInfo;
	if (basicMode)
		GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);

	for (int i = 0; i < NUM_PALETTE_COLORS; i++) {
		screenInfoNew.ColorTable[i] = (COLORREF)palette[i];
	}

	applyScreenInfoChanges(screenInfoNew);
}
Point2I ConsoleSettings::getFontSize() {
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfo);

	return fontInfo.dwFontSize;
}
RasterFontSizes ConsoleSettings::getRasterFontSize() {
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfo);

	if (isValidFontSize(fontInfo.dwFontSize)) {
		return toRasterSize(fontInfo.dwFontSize.X, fontInfo.dwFontSize.Y);
	}
	return RasterFontSizes::None;
}
int ConsoleSettings::getTrueTypeFontSize() {
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfo);

	return fontInfo.dwFontSize.Y;
}
void ConsoleSettings::setFontSize(Point2I size) {
	CONSOLE_FONT_INFOEX fontInfoNew = fontInfo;
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfoNew);

	fontInfoNew.dwFontSize = size;

	applyFontInfoChanges(fontInfoNew);
}
void ConsoleSettings::setRasterFontSize(RasterFontSizes rasterSize) {
	CONSOLE_FONT_INFOEX fontInfoNew = fontInfo;
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfoNew);

	fontInfoNew.dwFontSize = getRasterSize(rasterSize);

	applyFontInfoChanges(fontInfoNew);
}
void ConsoleSettings::setTrueTypeFontSize(int trueTypeSize) {
	CONSOLE_FONT_INFOEX fontInfoNew = fontInfo;
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfoNew);

	fontInfoNew.dwFontSize = Point2I(0, trueTypeSize);

	applyFontInfoChanges(fontInfoNew);
}
ConsoleFonts ConsoleSettings::getFontType() {
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfo);

	std::wstring name = fontInfo.FaceName;
	if (name == L"Terminal") return ConsoleFonts::Terminal;
	if (name == L"Consolas") return ConsoleFonts::Consolas;
	if (name == L"Lucida Console") return ConsoleFonts::LucidaConsole;
	return ConsoleFonts::None;
}
void ConsoleSettings::setFontType(ConsoleFonts font) {
	CONSOLE_FONT_INFOEX fontInfoNew = fontInfo;
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfoNew);

	std::wstring name;
	switch (font) {
	case ConsoleFonts::Terminal:
		name = L"Terminal";
		fontInfoNew.FontWeight = (fontInfoNew.dwFontSize.Y == 6 ? FW_BOLD : FW_REGULAR);
		break;
	case ConsoleFonts::Consolas:
		name = L"Consolas";
		fontInfoNew.dwFontSize.X = 0;
		break;
	case ConsoleFonts::LucidaConsole:
		name = L"Lucida Console";
		fontInfoNew.dwFontSize.X = 0;
		break;
	}
	wcscpy_s(fontInfoNew.FaceName, name.c_str());
	fontInfoNew.FontFamily = FF_DONTCARE;

	applyFontInfoChanges(fontInfoNew);
}
ConsoleFontWeights ConsoleSettings::getFontWeight() {
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfo);

	switch (fontInfo.FontWeight) {
	case FW_NORMAL: return ConsoleFontWeights::Regular;
	case FW_BOLD: return ConsoleFontWeights::Bold;
	}
	return ConsoleFontWeights::None;
}
void ConsoleSettings::setFontWeight(ConsoleFontWeights weight) {
	CONSOLE_FONT_INFOEX fontInfoNew =fontInfo;
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfoNew);

	switch (weight) {
	case ConsoleFontWeights::Regular:
	case ConsoleFontWeights::Bold:
		fontInfoNew.FontWeight = (unsigned int)weight;
		applyFontInfoChanges(fontInfoNew);
		break;
	}
}
ConsoleFont ConsoleSettings::getFont() {
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfo);

	ConsoleFont font = ConsoleFont::DEFAULT;

	std::wstring name = fontInfo.FaceName;
	if (name == L"Terminal") font.setFontType(ConsoleFonts::Terminal);
	else if (name == L"Consolas") font.setFontType(ConsoleFonts::Consolas);
	else if (name == L"Lucida Console") font.setFontType(ConsoleFonts::LucidaConsole);

	font.setFontSize(fontInfo.dwFontSize);
	font.setFontWeight((ConsoleFontWeights)fontInfo.FontWeight);

	return font;
}
void ConsoleSettings::setFont(ConsoleFont font) {
	CONSOLE_FONT_INFOEX fontInfoNew = fontInfo;
	if (basicMode)
		GetCurrentConsoleFontEx(hOut, false, &fontInfoNew);

	std::wstring name;
	switch (font.getFontType()) {
	case ConsoleFonts::Terminal:
		name = L"Terminal";
		break;
	case ConsoleFonts::Consolas:
		name = L"Consolas";
		break;
	case ConsoleFonts::LucidaConsole:
		name = L"Lucida Console";
		break;
	}

	wcscpy_s(fontInfoNew.FaceName, name.c_str());
	fontInfoNew.FontWeight = (UINT)font.getFontWeight();
	fontInfoNew.dwFontSize = font.getFontSize();
	fontInfoNew.FontFamily = FF_DONTCARE;

	applyFontInfoChanges(fontInfoNew);
}

#pragma endregion
//--------------------------------
#pragma region Window Visuals

std::string ConsoleSettings::getTitle() {
	if (basicMode) {
		char cTitle[MAX_PATH] = { 0 };
		GetConsoleTitleA(cTitle, MAX_PATH);
		return cTitle;
	}
	return windowTitle;
}
void ConsoleSettings::setTitle(const std::string& title) {
	applyWindowTitleChanges(title);
}
void ConsoleSettings::setIconFromFile(const std::string& path, int index) {
	HICON hIconNew, hIconSmNew;
	if (ExtractIconExA(path.c_str(), index, &hIconNew, &hIconSmNew, 1) != 0) {
		applyWindowIconChanges(hIconNew, hIconSmNew);
	}
}
void ConsoleSettings::setIconFromResourceByIndex(int index) {
	if (index < 0)
		throw std::out_of_range("Index must be greater than or equal to zero in 'Console::setIconFromResourceByIndex(...)'");

	char cExecutablePath[MAX_PATH] = { 0 };
	if (GetModuleFileNameA(GetModuleHandleA(nullptr), cExecutablePath, MAX_PATH) != 0) {
		HICON hIconNew, hIconSmNew;
		if (ExtractIconExA(cExecutablePath, index, &hIconNew, &hIconSmNew, 1) != 0) {
			applyWindowIconChanges(hIconNew, hIconSmNew);
		}
	}
}
void ConsoleSettings::setIconFromResourceById(int id) {
	if (id < 0)
		throw std::out_of_range("Id must be greater than or equal to zero in 'Console::setIconFromResourceById(...)'");

	char cExecutablePath[MAX_PATH] = { 0 };
	if (GetModuleFileNameA(GetModuleHandleA(nullptr), cExecutablePath, MAX_PATH) != 0) {
		HICON hIconNew, hIconSmNew;
		if (ExtractIconExA(cExecutablePath, -id, &hIconNew, &hIconSmNew, 1) != 0) {
			applyWindowIconChanges(hIconNew, hIconSmNew);
		}
	}
}
bool ConsoleSettings::isMouseVisible() {
	int mouseVisibleNew = (mouseVisible ? 1 : 0);
	if (basicMode) {
		mouseVisibleNew = ShowConsoleCursor(hOut, true) - 1;
		ShowConsoleCursor(hOut, false);
	}

	return mouseVisibleNew >= 0;
}
void ConsoleSettings::setMouseVisible(bool visible) {
	applyMouseChanges(visible);
}

#pragma endregion
//--------------------------------
#pragma region Window Settings

bool ConsoleSettings::isResizable() {
	if (basicMode)
		windowInfo.dwStyle = GetWindowLongA(hWnd, GWL_STYLE);

	return getFlag(windowInfo.dwStyle, WS_THICKFRAME);
}
void ConsoleSettings::setResizable(bool resizable) {
	DWORD styleNew = windowInfo.dwStyle;
	if (basicMode)
		styleNew = GetWindowLongA(hWnd, GWL_STYLE);

	setFlag(styleNew, WS_THICKFRAME, resizable);

	applyWindowStyleChanges(styleNew);
}
bool ConsoleSettings::hasToolWindowBorder() {
	if (basicMode)
		windowInfo.dwExStyle = GetWindowLongA(hWnd, GWL_EXSTYLE);

	return getFlag(windowInfo.dwExStyle, WS_EX_TOOLWINDOW);
}
void ConsoleSettings::setToolWindowBorder(bool enabled) {
	DWORD exStyleNew = windowInfo.dwExStyle;
	if (basicMode)
		exStyleNew = GetWindowLongA(hWnd, GWL_EXSTYLE);

	setFlag(exStyleNew, WS_EX_TOOLWINDOW, enabled);

	applyWindowStyleChanges(exStyleNew);
}
bool ConsoleSettings::isMenuItemsEnabled() {
	if (basicMode)
		menuItemsEnabled = SystemMenuManager::isConsoleMenuItemsEnabled();
	return menuItemsEnabled;
}
void ConsoleSettings::setMenuItemsEnabled(bool enabled) {
	bool closeButtonEnabledNew = closeButtonEnabled;
	if (basicMode)
		closeButtonEnabledNew = SystemMenuManager::isCloseMenuItemEnabled();

	applySystemMenuChanges(closeButtonEnabledNew, enabled);
}
bool ConsoleSettings::isCloseEnabled() {
	if (basicMode)
		closeButtonEnabled = SystemMenuManager::isCloseMenuItemEnabled();
	return closeButtonEnabled;
}
void ConsoleSettings::setCloseEnabled(bool enabled) {
	bool menuItemsEnabledNew = menuItemsEnabled;
	if (basicMode)
		menuItemsEnabledNew = SystemMenuManager::isConsoleMenuItemsEnabled();

	applySystemMenuChanges(enabled, menuItemsEnabledNew);
}
bool ConsoleSettings::isMaximizeEnabled() {
	if (basicMode)
		windowInfo.dwStyle = GetWindowLongA(hWnd, GWL_STYLE);

	return getFlag(windowInfo.dwStyle, WS_MAXIMIZEBOX);
}
void ConsoleSettings::setMaximizeEnabled(bool enabled) {
	DWORD styleNew = windowInfo.dwStyle;
	if (basicMode)
		styleNew = GetWindowLongA(hWnd, GWL_STYLE);

	setFlag(styleNew, WS_MAXIMIZEBOX, enabled);

	applyWindowStyleChanges(styleNew);
}
bool ConsoleSettings::isMinimizeEnabled() {
	if (basicMode)
		windowInfo.dwStyle = GetWindowLongA(hWnd, GWL_STYLE);

	return getFlag(windowInfo.dwStyle, WS_MINIMIZEBOX);
}
void ConsoleSettings::setMinimizeEnabled(bool enabled) {
	DWORD styleNew = windowInfo.dwStyle;
	if (basicMode)
		styleNew = GetWindowLongA(hWnd, GWL_STYLE);

	setFlag(styleNew, WS_MINIMIZEBOX, enabled);

	applyWindowStyleChanges(styleNew);
}

#pragma endregion
//--------------------------------
#pragma region Handles

HWND ConsoleSettings::getWindowHandle() {
	return hWnd;
}
HANDLE ConsoleSettings::getOuputHandle() {
	return hOut;
}
HANDLE ConsoleSettings::getInputHandle() {
	return hIn;
}

#pragma endregion
//--------------------------------
#pragma region Input

INPUT_RECORD ConsoleSettings::nextInputRecord() {
	INPUT_RECORD inRec = INPUT_RECORD();

	// Check if there are any console events.
	DWORD numRead = 0;
	bool hasEvent = false;
	const int waitDuration = 1;
	if (waitDuration > 0) {
		hasEvent = (WaitForSingleObject(hIn, waitDuration) == WAIT_OBJECT_0);
	}
	else {
		GetNumberOfConsoleInputEvents(hIn, &numRead);
		hasEvent = true; //numRead > 0;
	}

	if (hasEvent) {
		// Record the console event
		ReadConsoleInputA(hIn, &inRec, 1, &numRead);
	}
	else {
		// Mark the event type as invalid
		inRec.EventType = 0;
	}
	return inRec;
}
void ConsoleSettings::clearInputRecords() {
	FlushConsoleInputBuffer(hIn);
}

#pragma endregion
//--------------------------------
#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

bool ConsoleSettings::isActive() {
	if (basicMode)
		return (GetForegroundWindow() == hWnd);
	else
		return active;
}
void ConsoleSettings::activate() {
	if (basicMode) {
		SetForegroundWindow(hWnd);
	}
	else if (!active) {
		SetForegroundWindow(hWnd);
		WindowSPtr window = owner.lock();
		window->eventWindowActivated()(window);
	}
}
void ConsoleSettings::maximizeWindow() {
	ShowWindow(hWnd, SW_MAXIMIZE);
}
void ConsoleSettings::minimizeWindow() {
	ShowWindow(hWnd, SW_MINIMIZE);
}
void ConsoleSettings::showWindow() {
	ShowWindow(hWnd, SW_NORMAL);
}
void ConsoleSettings::hideWindow() {
	ShowWindow(hWnd, SW_HIDE);
}
bool ConsoleSettings::isWindowVisible() {
	return IsWindowVisible(hWnd) != 0;
}
void ConsoleSettings::centerWindow() {
	WINDOWINFO windowInfoNew = windowInfo;
	if (basicMode)
		GetWindowInfo(hWnd, &windowInfoNew);

	Point2I windowSize = Rectangle2I(windowInfoNew.rcWindow).size;
	Point2I clientSize = Rectangle2I(windowInfoNew.rcClient).size;

	Point2I windowClientDif = Rectangle2I(windowInfoNew.rcClient).point - Rectangle2I(windowInfoNew.rcWindow).point;
	Point2I trueClientSize = Point2I(screenInfo.srWindow, true) * fontInfo.dwFontSize;
	Rectangle2I monitorRect = getCurrentMonitor();

	Point2I clientPosition = monitorRect.point + (monitorRect.size - trueClientSize) / 2;

	windowInfoNew.rcWindow = Rectangle2I(clientPosition - windowClientDif, trueClientSize + windowSize - clientSize);
	windowInfoNew.rcClient = Rectangle2I(clientPosition, trueClientSize);

	applyWindowPositionChanges(windowInfoNew);
}
void ConsoleSettings::pause() {
	_getch();
	std::cout << std::endl;
}
void ConsoleSettings::pause(const std::string& message) {
	std::cout << message;
	_getch();
	std::cout << std::endl;
}
void ConsoleSettings::clear() {
	if (basicMode) {
		/*GetConsoleMode(hIn, (LPDWORD)&inputMode);
		GetConsoleMode(hOut, (LPDWORD)&outputMode);*/
	}

	//system("cls");
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

	/*applyInputModeChanges(inputMode);
	applyOutputModeChanges(outputMode);*/
}

void ConsoleSettings::flashWindow() {
	FlashWindow(hWnd, true);
}
void ConsoleSettings::playSystemSound(SystemSounds sound, bool async) {
	std::string soundName;;
	switch (sound) {
	case SystemSounds::Alert: soundName = "SystemDefault"; break;
	case SystemSounds::Information: soundName = "SystemAsterisk"; break;
	case SystemSounds::Warning: soundName = "SystemExclamation"; break;
	case SystemSounds::Error: soundName = "SystemHand"; break;
	default:
		return;
	}
	PlaySoundA(soundName.c_str(), nullptr, SND_NODEFAULT | SND_ALIAS | (async ? SND_ASYNC : SND_SYNC));
}
void ConsoleSettings::playSoundFromResourceById(int id, bool async) {
	PlaySoundA(MAKEINTRESOURCEA(id), GetModuleHandleA(nullptr), SND_NODEFAULT | SND_RESOURCE | (async ? SND_ASYNC : SND_SYNC));
}
void ConsoleSettings::playSoundFromFile(const std::string& path, bool async) {
	PlaySoundA(path.c_str(), nullptr, SND_NODEFAULT | SND_FILENAME | (async ? SND_ASYNC : SND_SYNC));
}

#pragma endregion
//============ OUTPUT ============
#pragma region Output

std::string ConsoleSettings::readLine() {
	std::string str = "";
	std::getline(std::cin, str);
	return str;
}

void ConsoleSettings::writeImage(const AsciiImageFrame& image, int indentation) {
	Point2I cursor = getCursorPosition();
	if (cursor.x > 0) {
		cursor.x = 0;
		cursor.y++;
		std::cout << std::endl;
	}
	for (int i = 0; i < image.size().y; i++)
		std::cout << std::endl;
	Point2I bufferSize = getBufferSize();
	// No need to draw more than the width of the console buffer.
	Point2I finalSize = {
		GMath::min(image.size().x, bufferSize.x - indentation),
		GMath::min(image.size().y, bufferSize.y - 1)
	};

	// bufferSize.y - 1 here because the next line will be empty for output and input
	int outputPosition = GMath::max(0, image.size().y - bufferSize.y - 1);

	CHAR_INFO* buffer = new CHAR_INFO[finalSize.x * image.size().y];
	for (int x = 0; x < finalSize.x; x++) {
		for (int y = 0; y < image.size().y; y++) {
			buffer[x + y * finalSize.x].Char.AsciiChar = (CHAR)image.getPixel(Point2I(x, y)).character;
			buffer[x + y * finalSize.x].Attributes = (WORD)image.getPixel(Point2I(x, y)).color;
		}
	}

	SMALL_RECT writeRegion = Rectangle2I(cursor + Point2I(indentation, 0), finalSize);
	WriteConsoleOutputA(hOut, buffer, finalSize, Point2I(0, outputPosition), &writeRegion);

	// Delete the buffer, we're done with it.
	delete[] buffer;
}

#pragma endregion
//========= CONSOLE MISC =========
#pragma region Console Misc

void ConsoleSettings::writeBuffer(const AsciiImageFrame& image, bool forceRedraw) {
	bool changed = false;
	Rectangle2I changedArea = Rectangle2I(image.size(), -image.size());
	for (Point2I p = Point2I::ZERO; p.x < (int)screenInfo.dwSize.X && p.x < image.size().x; p.x++) {
		for (p.y = 0; p.y < (int)screenInfo.dwSize.Y && p.y < image.size().y; p.y++) {
			int index = p.x + p.y * (int)screenInfo.dwSize.X;

			buffer[index].Char.AsciiChar = (CHAR)image.getPixel(p).character;
			buffer[index].Attributes = (WORD)image.getPixel(p).color;
			if (buffer[index].Char.AsciiChar != bufferLast[index].Char.AsciiChar ||
				buffer[index].Attributes != bufferLast[index].Attributes) {

				changed = true;
				changedArea.topLeft(GMath::min(p, changedArea.topLeft()));
				changedArea.bottomRight(GMath::max(p, changedArea.bottomRight()));
				bufferLast[index] = buffer[index];
			}
		}
	}

	// Don't write anything if nothing has changed
	if (changed || forceRedraw) {
		bool justDebugInfoDrawing = (changedArea.size.y == 1 && changedArea.size.x <= 4);
		if (forceRedraw || (PowerConsoleSettings::getDrawEntireBuffer() && !justDebugInfoDrawing)) {
			SMALL_RECT srect = Rectangle2I(screenInfo.dwSize);
			WriteConsoleOutputA(hOut, buffer, screenInfo.dwSize, Point2I::ZERO, &srect);
		}
		else {
			SMALL_RECT srect = changedArea;
			WriteConsoleOutputA(hOut, buffer, screenInfo.dwSize, changedArea.topLeft(), &srect);
		}
	}
}
void ConsoleSettings::resizeBuffer(Point2I newSize) {
	int bufferLength = newSize.x * newSize.y;
	CHAR_INFO* newBuffer = new CHAR_INFO[bufferLength];
	CHAR_INFO* newBufferLast = new CHAR_INFO[bufferLength];

	for (Point2I p = Point2I::ZERO; p.y < newSize.y; p.y++) {
		for (p.x = 0; p.x < newSize.x; p.x++) {
			int newIndex = p.x + p.y * newSize.x;
			int oldIndex = p.x + p.y * (int)bufferSize.x;

			if (p < bufferSize && buffer != nullptr) {
				newBuffer[newIndex] = buffer[oldIndex];
				newBufferLast[newIndex] = bufferLast[oldIndex];
			}
			else {
				newBuffer[newIndex].Char.AsciiChar = ' ';
				newBuffer[newIndex].Attributes = (WORD)0x07;
				newBufferLast[newIndex].Char.AsciiChar = ' ';
				newBufferLast[newIndex].Attributes = (WORD)0x07;
			}
		}
	}

	// Cleanup
	if (buffer != nullptr) {
		delete[] buffer;
		delete[] bufferLast;
	}
	buffer = newBuffer;
	bufferLast = newBufferLast;
	newBuffer = nullptr;
	newBufferLast = nullptr;
	bufferSize = newSize;
}

void ConsoleSettings::checkForChanges() {
	WindowSPtr window = owner.lock();

	CONSOLE_SCREEN_BUFFER_INFO screenInfoCheck;
	if (GetConsoleScreenBufferInfo(hOut, &screenInfoCheck) != 0) {
		if ((Point2I)screenInfo.dwSize != screenInfoCheck.dwSize) {
			resizeBuffer(screenInfoCheck.dwSize);
			screenInfo.dwSize = screenInfoCheck.dwSize;
			window->setContainerLayout(Rectangle2I(screenInfoCheck.dwSize), Rectangle2I(screenInfoCheck.dwSize));
		}
		if ((Rectangle2I)screenInfo.srWindow != screenInfoCheck.srWindow) {
			screenInfo.srWindow = screenInfoCheck.srWindow;
		}
	}

	GetCurrentConsoleFontEx(hOut, false, &fontInfo);
	GetConsoleCursorInfo(hOut, &cursorInfo);

	WINDOWINFO windowInfoCheck = { sizeof(WINDOWINFO) };
	if (GetWindowInfo(hWnd, &windowInfoCheck) != 0) {
		Point2I oldWindow = Point2I(windowInfo.rcWindow, false);
		Point2I oldClient = Point2I(windowInfo.rcClient, false);
		windowInfo = windowInfoCheck;
		if (Point2I(windowInfo.rcClient, false) != Point2I(windowInfoCheck.rcClient, false)) {
			window->eventWindowMoved()(window, WindowMovedEventArgs(
				Point2I(windowInfo.rcWindow, false),
				oldWindow,
				Point2I(windowInfo.rcClient, false),
				oldClient
			));
		}
	}

	bool activeNew = (GetForegroundWindow() == hWnd);
	if (active != activeNew) {
		active = activeNew;
		if (activeNew)
			window->eventWindowActivated()(window);
		else
			window->eventWindowDeactivated()(window);
	}
}

void ConsoleSettings::applyChanges() {
	///TODO: Quick fix until proper handling of code pages is done.
	///      This is the code-page that EVERYTHING is built around.
	///      Handle restoring this, when done.
	SetConsoleOutputCP(437); // OEM - United States
	SetConsoleCP(437);       // Input code-page, possibly unneeded.

	applyScreenInfoChanges(screenInfo, true);
	applyFontInfoChanges(fontInfo, true);
	applyCursorInfoChanges(cursorInfo, true);
	applyInputModeChanges(inputMode, true);
	applyOutputModeChanges(outputMode, true);
	applyWindowStyleChanges(windowInfo.dwStyle, true);
	applyWindowExStyleChanges(windowInfo.dwExStyle, true);
	//applyWindowPositionChanges(windowInfo, true);
	applyWindowTitleChanges(windowTitle, true);
	applyWindowIconChanges(hIcon, hIconSm, true);
	applyMouseChanges(mouseVisible, true);
	applySystemMenuChanges(closeButtonEnabled, menuItemsEnabled, true);
}
void ConsoleSettings::applyScreenInfoChanges(CONSOLE_SCREEN_BUFFER_INFOEX& screenInfoNew, bool loading) {
	if (loaded || loading) {
		screenInfoNew.bFullscreenSupported = false;
		screenInfoNew.dwMaximumWindowSize = GetLargestConsoleWindowSize(hOut);
		// Adjust for 1-off issue when setting console dimensions
		screenInfoNew.srWindow.Right += 1;
		screenInfoNew.srWindow.Bottom += 1;
		if (SetConsoleScreenBufferInfoEx(hOut, &screenInfoNew) != 0) {
			// Adjust for 1-off issue when setting console dimensions
			screenInfoNew.srWindow.Right -= 1;
			screenInfoNew.srWindow.Bottom -= 1;

			if (windowMode) {
				Point2I oldSize = screenInfo.dwSize;
				screenInfo = screenInfoNew;
				if (oldSize != screenInfoNew.dwSize || loading) {
					WindowSPtr window = owner.lock();
					resizeBuffer(screenInfoNew.dwSize);
					WINDOWINFO windowInfoCheck = { sizeof(WINDOWINFO) };
					if (GetWindowInfo(hWnd, &windowInfoCheck) != 0) {
						windowInfo.rcWindow = Rectangle2I(Point2I(windowInfo.rcWindow, false), Point2I(windowInfoCheck.rcWindow, true));
						windowInfo.rcClient = Rectangle2I(Point2I(windowInfo.rcClient, false), Point2I(windowInfoCheck.rcClient, true));
					}
					window->setContainerLayout(Rectangle2I(screenInfoNew.dwSize), Rectangle2I(screenInfoNew.dwSize));
				}
			}
		}
		else if (loading) {
			GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);
			if (windowMode) {
				Point2I oldSize = screenInfo.dwSize;
				screenInfo = screenInfoNew;
				if (oldSize != screenInfoNew.dwSize || loading) {
					WindowSPtr window = owner.lock();
					resizeBuffer(screenInfoNew.dwSize);
					window->setContainerLayout(Rectangle2I(screenInfoNew.dwSize), Rectangle2I(screenInfoNew.dwSize));
				}
			}
		}
	}
	else {
		screenInfo = screenInfoNew;
	}
}
void ConsoleSettings::applyFontInfoChanges(CONSOLE_FONT_INFOEX& fontInfoNew, bool loading) {
	if (loaded || loading) {
		if (SetCurrentConsoleFontEx(hOut, false, &fontInfoNew) != 0) {

			// We need to set the screen info again, otherwise the right scrollbar will appear when it shouldn't
			if (windowMode) {
				fontInfo = fontInfoNew;
				screenInfo.bFullscreenSupported = false;
				screenInfo.dwMaximumWindowSize = GetLargestConsoleWindowSize(hOut);
				// Adjust for 1-off issue when setting console dimensions
				screenInfo.srWindow.Right += 1;
				screenInfo.srWindow.Bottom += 1;
				SetConsoleScreenBufferInfoEx(hOut, &screenInfo);
				screenInfo.srWindow.Right -= 1;
				screenInfo.srWindow.Bottom -= 1;
			}
			else {
				CONSOLE_SCREEN_BUFFER_INFOEX screenInfoNew = { sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) };
				if (GetConsoleScreenBufferInfoEx(hOut, &screenInfoNew) != 0) {
					screenInfoNew.bFullscreenSupported = false;
					screenInfoNew.dwMaximumWindowSize = GetLargestConsoleWindowSize(hOut);
					screenInfoNew.srWindow.Right += 1;
					screenInfoNew.srWindow.Bottom += 1;
					SetConsoleScreenBufferInfoEx(hOut, &screenInfoNew);
				}
			}
		}
		else if (loading) {
			GetCurrentConsoleFontEx(hOut, false, &fontInfo);
		}
	}
	else {
		fontInfo = fontInfoNew;
	}
}
void ConsoleSettings::applyCursorInfoChanges(CONSOLE_CURSOR_INFO& cursorInfoNew, bool loading) {
	if (loaded || loading) {
		if (SetConsoleCursorInfo(hOut, &cursorInfoNew) != 0) {
			if (windowMode)
				cursorInfo = cursorInfoNew;
		}
		else if (loading) {
			GetConsoleCursorInfo(hOut, &cursorInfo);
		}
	}
	else {
		cursorInfo = cursorInfoNew;
	}
}
void ConsoleSettings::applyInputModeChanges(ConsoleInputModes inputModeNew, bool loading) {
	if (loaded || loading) {
		if (SetConsoleMode(hIn, (DWORD)inputModeNew)) {
			if (windowMode)
				inputMode = inputModeNew;
		}
		else if (loading) {
			GetConsoleMode(hIn, (LPDWORD)&inputMode);
		}
	}
	else {
		inputMode = inputModeNew;
	}
}
void ConsoleSettings::applyOutputModeChanges(ConsoleOutputModes outputModeNew, bool loading) {
	if (loaded || loading) {
		if (SetConsoleMode(hOut, (DWORD)outputModeNew)) {
			if (windowMode)
				outputMode = outputModeNew;
		}
		else if (loading) {
			GetConsoleMode(hOut, (LPDWORD)&outputMode);
		}
	}
	else {
		outputMode = outputModeNew;
	}
}
void ConsoleSettings::applyWindowStyleChanges(DWORD styleNew, bool loading) {
	if (loaded || loading) {
		// Set these flags so the scroll bars reflect the buffer and console screen sizes
		bool hscroll = Point2I(screenInfo.srWindow, true).x != (int)screenInfo.dwSize.X;
		bool vscroll = Point2I(screenInfo.srWindow, true).x != (int)screenInfo.dwSize.X;
		setFlag(styleNew, WS_HSCROLL, hscroll);
		setFlag(styleNew, WS_VSCROLL, vscroll);

		SetLastError(0);
		if (SetWindowLongA(hWnd, GWL_STYLE, styleNew) != 0 || GetLastError() == 0) {
			if (windowMode)
				windowInfo.dwStyle = styleNew;
		}
		else if (loading) {
			windowInfo.dwStyle = GetWindowLongA(hWnd, GWL_STYLE);
		}
	}
	else {
		windowInfo.dwStyle = styleNew;
	}
}
void ConsoleSettings::applyWindowExStyleChanges(DWORD exStyleNew, bool loading) {
	if (loaded || loading) {
		SetLastError(0);
		if (SetWindowLongA(hWnd, GWL_EXSTYLE, exStyleNew) != 0 || GetLastError() == 0) {
			if (windowMode)
				windowInfo.dwExStyle = exStyleNew;
		}
		else if (loading) {
			windowInfo.dwExStyle = GetWindowLongA(hWnd, GWL_EXSTYLE);
		}
	}
	else {
		windowInfo.dwExStyle = exStyleNew;
	}
}
void ConsoleSettings::applyWindowPositionChanges(WINDOWINFO& windowInfoNew, bool loading) {
	if (loaded || loading) {
		if (SetWindowPos(hWnd, nullptr, windowInfoNew.rcWindow.left, windowInfoNew.rcWindow.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE) != 0) {
			if (windowMode) {
				windowInfo.rcWindow = windowInfoNew.rcWindow;
				windowInfo.rcClient = windowInfoNew.rcClient;
			}
		}
		else {
			GetWindowInfo(hWnd, &windowInfo);
		}
	}
	else {
		windowInfo.rcWindow = windowInfoNew.rcWindow;
		windowInfo.rcClient = windowInfoNew.rcClient;
	}
}
void ConsoleSettings::applyWindowTitleChanges(const std::string& windowTitleNew, bool loading) {
	if (loaded || loading) {
		if (SetConsoleTitleA(windowTitleNew.c_str()) != 0) {
			if (windowMode)
				windowTitle = windowTitleNew;
		}
		else if (loading) {
			char cTitle[MAX_PATH] = { 0 };
			GetConsoleTitleA(cTitle, MAX_PATH);
			windowTitle = cTitle;
		}
	}
	else {
		windowTitle = windowTitleNew;
	}
}
void ConsoleSettings::applyWindowIconChanges(HICON hIconNew, HICON hIconSmNew, bool loading) {
	if (loaded || loading) {
		if (hIconNew != nullptr) {
			if (hIcon != hIconNew && hIconSm != hIconSmNew) {
				if (hIcon != nullptr) DestroyIcon(hIcon);
				if (hIconSm != nullptr) DestroyIcon(hIconSm);

				hIcon = hIconNew;
				hIconSm = hIconSmNew;
			}

			// http://stackoverflow.com/questions/18314659/setting-program-icon-without-resources-using-the-win32-api
			// Change both icons to the same icon handle.
			SendMessageA(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			SendMessageA(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);

			// This will ensure that the application icon gets changed too.
			SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
		}
	}
	else {
		hIcon = hIconNew;
		hIconSm = hIconSmNew;
	}
}
void ConsoleSettings::applyMouseChanges(bool mouseVisibleNew, bool loading) {
	if (loaded || loading) {
		// The mouse visible state is a counter and should always be brought down
		//  to either 0 or 1
		int visibleState = ShowConsoleCursor(hOut, mouseVisibleNew);
		if (visibleState > (mouseVisibleNew ? 0 : -1)) {
			while (ShowConsoleCursor(hOut, false) > (mouseVisibleNew ? 0 : -1));
		}
		else if (visibleState < (mouseVisibleNew ? 0 : -1)) {
			while (ShowConsoleCursor(hOut, true) < (mouseVisibleNew ? 0 : -1));
		}
	}
	mouseVisible = mouseVisibleNew;
}
void ConsoleSettings::applySystemMenuChanges(bool closeButtonEnabledNew, bool menuItemsEnabledNew, bool loading) {
	if (loaded || loading) {
		if (SystemMenuManager::setMenuItemsEnabled(closeButtonEnabledNew, menuItemsEnabledNew)) {
			closeButtonEnabled = closeButtonEnabledNew;
			menuItemsEnabled = menuItemsEnabledNew;
		}
	}
	else {
		closeButtonEnabled = closeButtonEnabledNew;
		menuItemsEnabled = menuItemsEnabledNew;
	}
}

#pragma endregion
//=================================================================|
