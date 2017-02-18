/*=================================================================|
|* File:				ConsoleApp.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Decleration of the Keyboard listener class.		  *|
|=================================================================*/

#include "Stdafx.h"
#include "ConsoleApp.h"
#include "AsciiImage.h"
#include "Geometry.h"
#include "Keys.h"
#include "MouseButtons.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ControlHandler.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;

//=================================================================|
// DEFINITIONS													   |
//=================================================================/

#define hWnd _windowData.HWnd
#define hOut _consoleData.HOut
#define hIn _consoleData.HIn

/* Returns true if the flag is applied to the attributes. */
#define GetFlag(attributes, flag)	(((attributes) & (flag)) == (flag))

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ConsoleApp
//========= CONSTRUCTORS =========
#pragma region Constructors
//--------------------------------
#pragma region Constructors

/* Constructs the default console app. */
PowerConsole::ConsoleApp::ConsoleApp(int argc, char** argv) {
	_members.Running		= true;
	_members.Keyboard		= new Keyboard();
	_members.Mouse			= new Mouse();
	_members.FrameDuration	= 0;
	_members.FPS			= 60;
	_members.FrameStart		= clock();
	_members.Graphics		= new AsciiImage(Coord(80, 25), FormatBasic);
	_members.FrameCurrent	= nullptr;
	_members.FrameNext		= nullptr;
	_members.Argc			= argc;
	_members.Argv			= argv;
	
	_consoleData.HOut		= nullptr;
	_consoleData.HIn		= nullptr;

	/*_consoleData.Buffer		= nullptr;
	_consoleData.BufferLast	= nullptr;
	_consoleData.FontSize	= Coord(8, 12);

	_consoleData.ScreenInfoChanged = false;*/
	
	_windowData.HWnd			= nullptr;

	/*_windowData.Active			= true;
	_windowData.WindowTitle		= "";
	_windowData.WindowTitleNew	= "";
	_windowData.MouseVisible	= true;
	_windowData.MouseVisibleNew		= true;
	_windowData.MenuItemsEnabled	= true;
	_windowData.MenuItemsEnabledNew = true;
	_windowData.CloseButtonEnabled		= true;
	_windowData.CloseButtonEnabledNew	= true;
	_windowData.WindowMoved				= false;
	
	_windowData.ActiveChanged			= false;
	_windowData.WindowStyleChanged		= false;
	_windowData.WindowTitleChanged		= false;
	_windowData.MouseVisibleChanged		= false;
	_windowData.WindowPositionChanged	= false;
	_windowData.MenuItemsChanged		= false;*/

}
/* Cleans up the console app. */
PowerConsole::ConsoleApp::~ConsoleApp() {
	_consoleData.HOut = nullptr;
	_consoleData.HIn = nullptr;
	_windowData.HWnd = nullptr;
	if (_consoleData.Buffer != nullptr) {
		//delete[] _consoleData.Buffer;
		_consoleData.Buffer = nullptr;
	}
	if (_consoleData.BufferLast != nullptr) {
		//delete[] _consoleData.BufferLast;
		_consoleData.BufferLast = nullptr;
	}
}

#pragma endregion
//--------------------------------
#pragma region Initialization

/* Initializes the console screen info. */
void PowerConsole::ConsoleApp::_InitializeScreen() {
	// Screen Info
	CONSOLE_SCREEN_BUFFER_INFOEX screenInfoBase = {sizeof(CONSOLE_SCREEN_BUFFER_INFOEX)};
	_consoleData.ScreenInfo = screenInfoBase;

	_consoleData.ScreenInfo.srWindow	= SmallRect(Coord(0, 0), Coord(80, 25));
	_consoleData.ScreenInfo.dwSize		= Coord(80, 25);
	_consoleData.ScreenInfo.dwMaximumWindowSize		= GetLargestConsoleWindowSize(hOut);
	_consoleData.ScreenInfo.bFullscreenSupported	= false;

	// Cursor
	_consoleData.ScreenInfo.dwCursorPosition	= Coord(0, 0);

	// Attributes
	_consoleData.ScreenInfo.wAttributes			= 0x07;
	_consoleData.ScreenInfo.wPopupAttributes	= 0x5F;

	// Color Table
	_consoleData.ScreenInfo.ColorTable[0]	= RGB(0, 0, 0);
	_consoleData.ScreenInfo.ColorTable[1]	= RGB(0, 0, 128);
	_consoleData.ScreenInfo.ColorTable[2]	= RGB(0, 128, 0);
	_consoleData.ScreenInfo.ColorTable[3]	= RGB(0, 128, 128);
	_consoleData.ScreenInfo.ColorTable[4]	= RGB(128, 0, 0);
	_consoleData.ScreenInfo.ColorTable[5]	= RGB(128, 0, 128);
	_consoleData.ScreenInfo.ColorTable[6]	= RGB(128, 128, 0);
	_consoleData.ScreenInfo.ColorTable[7]	= RGB(192, 192, 192);
	_consoleData.ScreenInfo.ColorTable[8]	= RGB(128, 128, 128);
	_consoleData.ScreenInfo.ColorTable[9]	= RGB(0, 0, 255);
	_consoleData.ScreenInfo.ColorTable[10]	= RGB(0, 255, 0);
	_consoleData.ScreenInfo.ColorTable[11]	= RGB(0, 255, 255);
	_consoleData.ScreenInfo.ColorTable[12]	= RGB(255, 0, 0);
	_consoleData.ScreenInfo.ColorTable[13]	= RGB(255, 0, 255);
	_consoleData.ScreenInfo.ColorTable[14]	= RGB(255, 255, 0);
	_consoleData.ScreenInfo.ColorTable[15]	= RGB(255, 255, 255);

	_consoleData.ScreenInfoNew				= _consoleData.ScreenInfo;
	_consoleData.ScreenInfoChanged			= false;

	_consoleData.ScreenInfo.srWindow.Right	+= 1;
	_consoleData.ScreenInfo.srWindow.Bottom	+= 1;
	SetConsoleScreenBufferInfoEx(hOut, &_consoleData.ScreenInfo);
	_consoleData.ScreenInfo.srWindow.Right	-= 1;
	_consoleData.ScreenInfo.srWindow.Bottom	-= 1;
	
	CONSOLE_FONT_INFO fontInfo;
	if (GetCurrentConsoleFont(hOut, false, &fontInfo) != 0)
		_consoleData.FontSize	= fontInfo.dwFontSize;
	else
		_consoleData.FontSize	= Coord(8, 12);

	system("cls");
}
/* Initializes the console buffer. */
void PowerConsole::ConsoleApp::_InitializeBuffer() {
	int bufferLength = (int)_consoleData.ScreenInfo.dwSize.X * (int)_consoleData.ScreenInfo.dwSize.Y;
	_consoleData.Buffer = new CHAR_INFO[bufferLength];
	_consoleData.BufferLast = new CHAR_INFO[bufferLength];
	for (int i = 0; i < bufferLength; i++)
	{
		_consoleData.Buffer[i].Char.AsciiChar		= ' ';
		_consoleData.Buffer[i].Attributes			= _consoleData.ScreenInfo.wAttributes;
		_consoleData.BufferLast[i].Char.AsciiChar	= ' ';
		_consoleData.BufferLast[i].Attributes		= _consoleData.ScreenInfo.wAttributes;
	}
}
/* Initializes the console IO mode. */
void PowerConsole::ConsoleApp::_InitializeIOMode() {
	DWORD outputMode = ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT;
	DWORD inputMode = ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT |
					ENABLE_MOUSE_INPUT;// | ENABLE_WINDOW_INPUT;
	
	SetConsoleMode(hIn, inputMode);
	SetConsoleMode(hOut, outputMode);
}
/* Initializes the console cursor info. */
void PowerConsole::ConsoleApp::_InitializeCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = false;
	cursorInfo.dwSize	= 25;
	SetConsoleCursorInfo(hOut, &cursorInfo);
}
/* Initializes the window info. */
void PowerConsole::ConsoleApp::_InitializeWindow() {
	
	// Window info
	WINDOWINFO windowInfo	= {sizeof(WINDOWINFO)};
	_windowData.WindowInfo		= windowInfo;
	_windowData.WindowInfoNew		= windowInfo;
	_windowData.WindowStyleChanged	= false;
	_windowData.WindowPositionChanged	= false;

	// Window title
	_windowData.WindowTitle			= "";
	_windowData.WindowTitleNew		= "";
	_windowData.WindowTitleChanged	= false;
	char cTitle[MAX_PATH];
	if (GetModuleFileNameA(nullptr, cTitle, MAX_PATH) != 0) {
		string title = cTitle;
		for (int i = title.length() - 1; i >= 0; i--) {
			if (title[i] == '\\' && i != title.length()) {
				_windowData.WindowTitle = (title.substr(i + 1, title.length() - i - 5));
				if (SetConsoleTitleA(_windowData.WindowTitle.c_str()) != 0) {
					_windowData.WindowTitleNew	= _windowData.WindowTitle;
				}
				else {
					_windowData.WindowTitle		= cTitle;
					_windowData.WindowTitleNew	= cTitle;
				}
				break;
			}
		}
	}
	else if (GetConsoleTitleA(cTitle, MAX_PATH) != 0) {
		_windowData.WindowTitle		= cTitle;
		_windowData.WindowTitleNew	= cTitle;
	}

	// Windows Settings
	_windowData.ScreenResolution	= Coord(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	_windowData.WindowMoved		= false;

	// Focus
	_windowData.Active			= GetForegroundWindow() == hWnd;
	_windowData.ActiveChanged		= false;
}
/* Initializes the console menu items. */
void PowerConsole::ConsoleApp::_InitializeMenuItems() {
	HMENU hMenu = GetSystemMenu(hWnd, false);
	if (hMenu != nullptr) {
		for (int i = GetMenuItemCount(hMenu) - 1; i >= 0; i--) {
			MENUITEMINFOA item = {sizeof(MENUITEMINFOA)};

			item.fMask = MIIM_STRING;
			if (GetMenuItemInfoA(hMenu, i, true, &item) == 0) {
				break;
			}
			item.cch++;
			item.dwTypeData = new char[item.cch];
			item.fMask = MIIM_ID | MIIM_FTYPE | MIIM_DATA | MIIM_STATE | MIIM_CHECKMARKS | MIIM_SUBMENU | MIIM_STRING | MIIM_BITMAP; 
			if (GetMenuItemInfoA(hMenu, i, true, &item) == 0) {
				break;
			}

			string name = item.dwTypeData;
			if (name == "&Close") {
				_windowData.MenuItemInfo[0]		= item;
				_windowData.MenuItemPositions[0]	= i;
			}
			else if (name == "&Edit") {
				_windowData.MenuItemInfo[1]		= item;
				_windowData.MenuItemPositions[1]	= i;
			}
			else if (name == "&Defaults") {
				_windowData.MenuItemInfo[2]		= item;
				_windowData.MenuItemPositions[2]	= i;
			}
			else if (name == "&Properties") {
				_windowData.MenuItemInfo[3]		= item;
				_windowData.MenuItemPositions[3]	= i;
			}
		}
	}

	_windowData.MenuItemsEnabled		= true;
	_windowData.MenuItemsEnabledNew	= true;
	_windowData.CloseButtonEnabled	= true;
	_windowData.CloseButtonEnabledNew	= true;
	_windowData.MenuItemsChanged		= false;
}
/* Initializes the console mouse info. */
void PowerConsole::ConsoleApp::_InitializeMouse() {
	_windowData.MouseVisible			= true;
	_windowData.MouseVisibleNew		= true;
	_windowData.MouseVisibleChanged	= false;
}

#pragma endregion
//--------------------------------
#pragma region Final Initialization

/* Initializes the console app. */
void PowerConsole::ConsoleApp::Initialize() {

	_InitializeScreen();
	_InitializeBuffer();
	_InitializeIOMode();
	_InitializeCursor();
	
	_InitializeWindow();
	_InitializeMenuItems();
	_InitializeMouse();

}
/* Uninitializes the console app. */
void PowerConsole::ConsoleApp::Uninitialize() {
	
	delete this->_members.Graphics;
	this->_members.Graphics = nullptr;
	delete this->_members.Keyboard;
	this->_members.Keyboard = nullptr;
	delete this->_members.Mouse;
	this->_members.Mouse = nullptr;

}
/* Runs the console app. */
void PowerConsole::ConsoleApp::Run() {
	
	_windowData.HWnd	= GetConsoleWindow();
	_consoleData.HOut	= GetStdHandle(STD_OUTPUT_HANDLE);
	_consoleData.HIn	= GetStdHandle(STD_INPUT_HANDLE);

	/*_InitializeScreen();
	_InitializeBuffer();
	_InitializeIOMode();
	_InitializeCursor();
	
	_InitializeWindow();
	_InitializeMenuItems();
	_InitializeMouse();*/

	Initialize();

	_MainLoop();

	Uninitialize();
}

#pragma endregion
//--------------------------------
#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Program Arguments

int PowerConsole::ConsoleApp::GetNumArguments() {
	return _members.Argc;
}
string PowerConsole::ConsoleApp::GetArgument(int index) {
	return string(_members.Argv[index]);
}
	
#pragma endregion
//--------------------------------
#pragma region Console Dimensions

/* Gets the size of the console screen. */
Coord PowerConsole::ConsoleApp::GetConsoleSize() {
	return Coord(_consoleData.ScreenInfo.dwSize);
}
/* Sets the size of the console screen. */
void PowerConsole::ConsoleApp::SetConsoleSize(Coord size) {
	_consoleData.ScreenInfoNew.srWindow	= SmallRect(0, 0, size);
	_consoleData.ScreenInfoNew.dwSize		= size;
	_consoleData.ScreenInfoChanged			= true;
}
/* Gets the maximum size of the console screen. */
Coord PowerConsole::ConsoleApp::GetMaxConsoleSize() {
	return Coord(_consoleData.ScreenInfo.dwMaximumWindowSize);
	//return Coord(GetLargestConsoleWindowSize(hOut));
}
	
#pragma endregion
//--------------------------------
#pragma region Window Dimensions

/* Gets the size of the window. */
Coord PowerConsole::ConsoleApp::GetWindowSize() {
	return Coord(_windowData.WindowInfo.rcWindow, true);
}
/* Gets the client size of the window. */
Coord PowerConsole::ConsoleApp::GetClientSize() {
	return Coord(_windowData.WindowInfo.rcClient, true);
}
/* Gets the position of the window. */
Coord PowerConsole::ConsoleApp::GetWindowPosition() {
	return Coord(_windowData.WindowInfo.rcWindow, false);
}
/* Gets the client position of the window. */
Coord PowerConsole::ConsoleApp::GetClientPosition() {
	return Coord(_windowData.WindowInfo.rcClient, false);
}
/* Sets the position of the window. */
void PowerConsole::ConsoleApp::SetWindowPosition(Coord position) {
	Coord windowSize		= Coord(_windowData.WindowInfoNew.rcWindow, true);
	Coord windowSizeClient	= Coord(_windowData.WindowInfoNew.rcClient, true);
	Coord windowClientDif	= Coord(_windowData.WindowInfoNew.rcClient, false) - Coord(_windowData.WindowInfoNew.rcWindow, false);
	_windowData.WindowInfoNew.rcWindow	= SmallRect(position, windowSize);
	_windowData.WindowInfoNew.rcClient	= SmallRect(position + windowClientDif, windowSizeClient);
	_windowData.WindowPositionChanged	= true;
}
/* Sets the client position of the window. */
void PowerConsole::ConsoleApp::SetClientPosition(Coord position) {
	Coord windowSize		= Coord(_windowData.WindowInfoNew.rcWindow, true);
	Coord windowSizeClient	= Coord(_windowData.WindowInfoNew.rcClient, true);
	Coord windowClientDif	= Coord(_windowData.WindowInfoNew.rcClient, false) - Coord(_windowData.WindowInfoNew.rcWindow, false);
	_windowData.WindowInfoNew.rcWindow	= SmallRect(position - windowClientDif, windowSize);
	_windowData.WindowInfoNew.rcClient	= SmallRect(position, windowSizeClient);
	_windowData.WindowPositionChanged	= true;
}
	
#pragma endregion
//--------------------------------
#pragma region Console Visuals
	
/* Gets the specified color used by the console. */
void PowerConsole::ConsoleApp::GetColor(int index) {

}
/* Sets the specified color used by the console. */
void PowerConsole::ConsoleApp::SetColor(int index) {

}
/* Gets the list of 16 colors used by the console. */
void PowerConsole::ConsoleApp::GetColors() {

}
/* Sets the list of 16 colors used by the console. */
void PowerConsole::ConsoleApp::SetColors() {

}
/* Gets the size of the console font. */
Coord PowerConsole::ConsoleApp::GetFontSize() {
	return _consoleData.FontSize;
}
	
#pragma endregion
//--------------------------------
#pragma region Window Visuals

/* Gets the title of the window. */
string PowerConsole::ConsoleApp::GetTitle() {
	return _windowData.WindowTitle;
}
/* Sets the title of the window. */
void PowerConsole::ConsoleApp::SetTitle(const string& title) {
	_windowData.WindowTitleNew		= title;
	_windowData.WindowTitleChanged	= true;
}
/* Gets whether the mouse is visible when inside the window. */
bool PowerConsole::ConsoleApp::IsMouseVisible() {
	return _windowData.MouseVisible;
}
/* Sets whether the mouse is visible when inside the window. */
void PowerConsole::ConsoleApp::SetMouseVisible(bool visible) {
	_windowData.MouseVisibleNew		= visible;
	_windowData.MouseVisibleChanged	= true;
}
/* Gets whether the window is resizable. */
bool PowerConsole::ConsoleApp::IsResizable() {
	return false;
}
/* Sets whether the window is resizable. */
void PowerConsole::ConsoleApp::SetResizable(bool resizable) {

}
/* Gets the style of the window. */
unsigned short PowerConsole::ConsoleApp::GetStyle() {
	unsigned short style = StyleNone;

	if (GetFlag(_windowData.WindowInfo.dwStyle, WS_THICKFRAME))
		style |= StyleResizable;
	if (GetFlag(_windowData.WindowInfo.dwExStyle, WS_EX_TOOLWINDOW))
		style |= StyleToolWindow;
	if (GetFlag(_windowData.WindowInfo.dwExStyle, WS_EX_ACCEPTFILES))
		style |= StyleAcceptFiles;
	
	if (GetFlag(_windowData.WindowInfo.dwStyle, WS_MAXIMIZEBOX))
		style |= StyleMaximizeButton;
	if (GetFlag(_windowData.WindowInfo.dwStyle, WS_MINIMIZEBOX))
		style |= StyleMinimizeButton;
	if (_windowData.CloseButtonEnabled)
		style |= StyleCloseButton;
	if (_windowData.MenuItemsEnabled)
		style |= StyleMenuItems;

	return style;
}
/* Sets the style of the window. */
void PowerConsole::ConsoleApp::SetStyle(unsigned short style) {
	if (GetFlag(style, StyleResizable))
		_windowData.WindowInfoNew.dwStyle		|= WS_THICKFRAME;
	else
		_windowData.WindowInfoNew.dwStyle		&= ~WS_THICKFRAME;
	if (GetFlag(style, StyleToolWindow))
		_windowData.WindowInfoNew.dwExStyle		|= WS_EX_TOOLWINDOW;
	else
		_windowData.WindowInfoNew.dwExStyle		&= ~WS_EX_TOOLWINDOW;
	if (GetFlag(style, StyleAcceptFiles))
		_windowData.WindowInfoNew.dwExStyle		|= WS_EX_ACCEPTFILES;
	else
		_windowData.WindowInfoNew.dwExStyle		&= ~WS_EX_ACCEPTFILES;
	
	if (GetFlag(style, StyleMaximizeButton))
		_windowData.WindowInfoNew.dwStyle		|= WS_MAXIMIZEBOX;
	else
		_windowData.WindowInfoNew.dwStyle		&= ~WS_MAXIMIZEBOX;
	if (GetFlag(style, StyleMinimizeButton))
		_windowData.WindowInfoNew.dwStyle		|= WS_MINIMIZEBOX;
	else
		_windowData.WindowInfoNew.dwStyle		&= ~WS_MINIMIZEBOX;
	_windowData.WindowStyleChanged	= true;

	_windowData.CloseButtonEnabledNew	= GetFlag(style, StyleCloseButton);
	_windowData.MenuItemsEnabledNew		= GetFlag(style, StyleMenuItems);
	_windowData.MenuItemsChanged		=
		(_windowData.CloseButtonEnabled != _windowData.CloseButtonEnabledNew ||
		 _windowData.MenuItemsEnabled   != _windowData.MenuItemsEnabledNew);
}
	
#pragma endregion
//--------------------------------
#pragma region Information

/* Gets whether the window is active. */
bool PowerConsole::ConsoleApp::IsActive() {
	return _windowData.Active;
}
/* Gets whether the window has focus. */
bool PowerConsole::ConsoleApp::IsFocus() {
	return false;
}
/* Gets the number of milliseconds per update in the application. */
int PowerConsole::ConsoleApp::GetUpdateRate() {
	return _members.FrameDuration;
}
	
#pragma endregion
//--------------------------------
#pragma region Handles

HWND PowerConsole::ConsoleApp::GetWindowHandle() {
	return _windowData.HWnd;
}
HANDLE PowerConsole::ConsoleApp::GetOuputHandle() {
	return _consoleData.HOut;
}
HANDLE PowerConsole::ConsoleApp::GetInputHandle() {
	return _consoleData.HIn;
}
	
#pragma endregion
//--------------------------------
#pragma region Frames

unsigned int PowerConsole::ConsoleApp::GetNumFrames() {
	return _members.Frames.size();
}
vector<Frame*>& PowerConsole::ConsoleApp::GetFrames() {
	return _members.Frames;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating
//--------------------------------
#pragma region Console Changes

/* Applies any changes made to the screen. */
void PowerConsole::ConsoleApp::_ApplyScreenChanges() {
	if (_consoleData.ScreenInfoChanged) {
		_consoleData.ScreenInfoChanged = false;
		CONSOLE_SCREEN_BUFFER_INFO cursorInfo;
		
		GetConsoleScreenBufferInfo(hOut, &cursorInfo);
		
		_consoleData.ScreenInfoNew.srWindow.Right		+= 1;
		_consoleData.ScreenInfoNew.srWindow.Bottom		+= 1;
		_consoleData.ScreenInfoNew.dwCursorPosition	= cursorInfo.dwCursorPosition;
		SetConsoleScreenBufferInfoEx(hOut, &_consoleData.ScreenInfoNew);

		_consoleData.ScreenInfoNew.srWindow.Right		-= 1;
		_consoleData.ScreenInfoNew.srWindow.Bottom		-= 1;

		bool resized = (Coord(_consoleData.ScreenInfo.dwSize) != Coord(_consoleData.ScreenInfoNew.dwSize));
		Coord maxWindowSize	= _consoleData.ScreenInfo.dwMaximumWindowSize;

		if (resized)
			_ResizeBuffer();

		_consoleData.ScreenInfo	= _consoleData.ScreenInfoNew;
		_consoleData.ScreenInfo.dwMaximumWindowSize = maxWindowSize;
	}
}
/* Resizes the console buffer. */
void PowerConsole::ConsoleApp::_ResizeBuffer() {
	int bufferLength = (int)_consoleData.ScreenInfoNew.dwSize.X * (int)_consoleData.ScreenInfoNew.dwSize.Y;
	CHAR_INFO* newBuffer		= new CHAR_INFO[bufferLength];
	CHAR_INFO* newBufferLast	= new CHAR_INFO[bufferLength];

	for (int i = 0; i < _consoleData.ScreenInfoNew.dwSize.Y; i++) {
		for (int j = 0; j < _consoleData.ScreenInfoNew.dwSize.X; j++) {
			int index		= j + i * _consoleData.ScreenInfoNew.dwSize.X;
			int indexOld	= j + i * _consoleData.ScreenInfo.dwSize.X;

			if (Coord(j, i) < Coord(_consoleData.ScreenInfo.dwSize)) {
				newBuffer[index]		= _consoleData.Buffer[indexOld];
				newBufferLast[index]	= _consoleData.BufferLast[indexOld];
			}
			else {
				newBuffer[index].Char.AsciiChar		= ' ';
				newBuffer[index].Attributes			= _consoleData.ScreenInfo.wAttributes;
				newBufferLast[index].Char.AsciiChar	= ' ';
				newBufferLast[index].Attributes		= _consoleData.ScreenInfo.wAttributes;
			}
		}
	}

	if (Coord(_consoleData.ScreenInfoNew.dwSize) != _members.Graphics->Size()) {
		_members.Graphics->Resize(Coord(_consoleData.ScreenInfoNew.dwSize));
	}

	delete[] _consoleData.Buffer;
	delete[] _consoleData.BufferLast;
	_consoleData.Buffer		= newBuffer;
	_consoleData.BufferLast	= newBufferLast;
	newBuffer				= nullptr;
	newBufferLast			= nullptr;
}
/* Updates and reads the console's settings. */
void PowerConsole::ConsoleApp::_UpdateConsole() {

	// Check for changes in the screen and buffer
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	if (GetConsoleScreenBufferInfo(hOut, &screenInfo) != 0) {
		if (Coord(_consoleData.ScreenInfo.dwSize) != Coord(screenInfo.dwSize)) {
			_consoleData.ScreenInfo.dwSize		= screenInfo.dwSize;
			_consoleData.ScreenInfoNew.dwSize	= screenInfo.dwSize;

			_ResizeBuffer();
		}
	}

	CONSOLE_FONT_INFO fontInfo;
	if (GetCurrentConsoleFont(hOut, false, &fontInfo) != 0)
		_consoleData.FontSize					= fontInfo.dwFontSize;
	else
		_consoleData.FontSize					= Coord(8, 12);
}

#pragma endregion
//--------------------------------
#pragma region Window Changes

/* Applies any changes made to the window info. */
void PowerConsole::ConsoleApp::_ApplyWindowChanges() {
	if (_windowData.WindowTitleChanged) {
		_windowData.WindowTitleChanged = false;

		if (SetConsoleTitleA(_windowData.WindowTitleNew.c_str()) != 0) {
			_windowData.WindowTitle = _windowData.WindowTitleNew;
		}
	}
	if (_windowData.WindowStyleChanged) {
		_windowData.WindowStyleChanged = false;

		if (SetWindowLongA(hWnd, GWL_STYLE, _windowData.WindowInfoNew.dwStyle) == 0) {
			
		}
		else if (SetWindowLongA(hWnd, GWL_STYLE, _windowData.WindowInfoNew.dwExStyle) == 0) {
			SetWindowLongA(hWnd, GWL_STYLE, _windowData.WindowInfo.dwStyle);
		}
		else {
			_windowData.WindowInfo.dwStyle	= _windowData.WindowInfoNew.dwStyle;
			_windowData.WindowInfo.dwExStyle	= _windowData.WindowInfoNew.dwExStyle;
		}
	}
	if (_windowData.WindowPositionChanged) {
		_windowData.WindowPositionChanged = false;

		if (SetWindowPos(hWnd, nullptr, _windowData.WindowInfoNew.rcWindow.left, _windowData.WindowInfoNew.rcWindow.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE) != 0) {
			_windowData.WindowInfo.rcClient	= _windowData.WindowInfoNew.rcClient;
			_windowData.WindowInfo.rcWindow	= _windowData.WindowInfoNew.rcWindow;
		}
	}
}
/* Applies any changes made to the console menu items. */
void PowerConsole::ConsoleApp::_ApplyMenuItemChanges() {
	if (_windowData.MenuItemsChanged) {
		_windowData.MenuItemsChanged = false;

		bool error = false;
		HMENU hMenu = GetSystemMenu(hWnd, false);

		if (hMenu != nullptr) {
			for (int i = GetMenuItemCount(hMenu) - 1; i >= 0; i--) {
				MENUITEMINFOA item = {sizeof(MENUITEMINFOA)};

				item.fMask = MIIM_STRING;
				if (GetMenuItemInfoA(hMenu, i, true, &item) == 0) {
					error = true;
					break;
				}
				item.cch++;
				item.dwTypeData = new char[item.cch];
				if (GetMenuItemInfoA(hMenu, i, true, &item) == 0) {
					error = true;
					break;
				}

				string name = item.dwTypeData;
				if ((name == "&Properties" ||
					name == "&Edit" ||
					name == "&Defaults") &&
					!_windowData.MenuItemsEnabledNew) {
					if (RemoveMenu(hMenu, i, MF_BYPOSITION) == 0) {
						error = true;
						break;
					}
				}
				else if (name == "&Close" && !_windowData.CloseButtonEnabledNew) {
					if (RemoveMenu(hMenu, i, MF_BYPOSITION) == 0) {
						error = true;
						break;
					}
				}
			}

			if (_windowData.CloseButtonEnabledNew && !_windowData.CloseButtonEnabled && !error) {
				if (InsertMenuItemA(hMenu, _windowData.MenuItemPositions[0], true, &_windowData.MenuItemInfo[0]) == 0) {
					error = true;
				}
			}
			if (_windowData.MenuItemsEnabledNew && !_windowData.MenuItemsEnabled && !error) {
				for (int i = 1; i < 4; i++) {
					unsigned int menuPos = _windowData.MenuItemPositions[i] + (_windowData.CloseButtonEnabledNew ? 0 : -1);
					if (InsertMenuItemA(hMenu, menuPos, true, &_windowData.MenuItemInfo[i]) == 0) {
						error = true;
						break;
					}
				}
			}
		}
		else {
			error = true;
		}
		DrawMenuBar(hWnd);

		if (!error) {
			_windowData.CloseButtonEnabled	= _windowData.CloseButtonEnabledNew;
			_windowData.MenuItemsEnabled		= _windowData.MenuItemsEnabledNew;
		}
	}
}
/* Applies any changes made to the mouse. */
void PowerConsole::ConsoleApp::_ApplyMouseChanges() {
	if (_windowData.MouseVisibleChanged) {
		_windowData.MouseVisibleChanged = false;
		
		// The mouse visible state is a counter and should always be brought down
		//  to either 0 or 1
		int visibleState = ShowConsoleCursor(hOut, _windowData.MouseVisibleNew);
		if (visibleState > (_windowData.MouseVisibleNew ? 0 : -1)) {
			while (ShowConsoleCursor(hOut, false) > (_windowData.MouseVisibleNew ? 0 : -1));
		}
		else if (visibleState < (_windowData.MouseVisibleNew ? 0 : -1)) {
			while (ShowConsoleCursor(hOut, true) < (_windowData.MouseVisibleNew ? 0 : -1));
		}
		_windowData.MouseVisible	= _windowData.MouseVisibleNew;
	}
}
/* Updates and reads the window's settings. */
void PowerConsole::ConsoleApp::_UpdateWindow() {
	_windowData.WindowMoved	= false;
	_windowData.ActiveChanged	= false;

	WINDOWINFO windowInfo = {sizeof(WINDOWINFO)};
	if (GetWindowInfo(hWnd, &windowInfo) != 0) {
		if (SmallRect(windowInfo.rcClient) != SmallRect(_windowData.WindowInfo.rcClient)) {
			_windowData.WindowMoved = Coord(windowInfo.rcClient, false) != Coord(_windowData.WindowInfo.rcClient, false);
			_windowData.WindowInfo = windowInfo;
		}
	}
	if (GetForegroundWindow() == hWnd) {
		_windowData.ActiveChanged = !_windowData.Active;
		_windowData.Active = true;
	}
	else {
		_windowData.ActiveChanged = _windowData.Active;
		_windowData.Active = false;
	}
}

#pragma endregion
//--------------------------------
#pragma region Application Logic

void PowerConsole::ConsoleApp::_MainLoop() {
	bool runOnce = false;
	//_members.Graphics->SaveFile("graphics0.asc", 2);
	while (_members.Running) {
		
		_ApplyScreenChanges();
		_UpdateConsole();

		_ApplyWindowChanges();
		_ApplyMenuItemChanges();
		_ApplyMouseChanges();
		_UpdateWindow();
		//_members.Graphics->SaveFile("graphics1.asc", 2);
		
		// Update the frame time
		_members.FrameDuration	= clock() - _members.FrameStart;
		_members.FrameStart	= clock();
		_members.FPS			= 1000.0 / _members.FrameDuration;
		
		//_members.Graphics->SaveFile("graphics2.asc", 2);

		// Create a new input record
		_members.InRec = INPUT_RECORD();
		//_members.Graphics->SaveFile("graphics3.asc", 2);

		// Check if there are any console events
		DWORD NumRead	= 0;
		bool hasEvent	= false;
		int waitDuration = 1;
		if (waitDuration > 0) {
			hasEvent = (WaitForSingleObject(hIn, waitDuration) == WAIT_OBJECT_0);
		}
		else {
			GetNumberOfConsoleInputEvents(hIn, &NumRead);
			hasEvent = true; //NumRead > 0;
		}
		//_members.Graphics->SaveFile("graphics4.asc", 2);

		if (hasEvent) {
			// Record the console event
			ReadConsoleInputA(hIn, &_members.InRec, 1, &NumRead);
		}
		else {
			// Mark the event type as invalid
			_members.InRec.EventType = 0;
		}
		
		//_members.Graphics->SaveFile("graphics5.asc", 2);
		_members.Keyboard->Update(_members.InRec, _windowData.Active);
		//_members.Graphics->SaveFile("graphics6.asc", 2);
		_members.Mouse->Update(_members.InRec, _windowData.Active, Coord(_windowData.WindowInfo.rcClient, false), Coord(_consoleData.ScreenInfo.dwSize), _consoleData.FontSize);
		
		//_members.Graphics->SaveFile("graphics7.asc", 2);
		Update();

		Draw();

		_WriteBuffer();

		_UpdateFrameChange();
		//break;
		
		runOnce = true;
		//break;
	}
}
void PowerConsole::ConsoleApp::_UpdateFrameChange() {
	if (_members.FrameNext != nullptr) {

		_members.FrameCurrent->OnLeave();
		if (!FrameExists(_members.FrameCurrent)) {
			_members.FrameCurrent->Uninitialize();
			delete _members.FrameCurrent;
		}

		_members.FrameCurrent = _members.FrameNext;
		_members.FrameNext = nullptr;

		if (!_members.FrameCurrent->IsInitialized()) {
			_members.FrameCurrent->Initialize(this);
		}
		_members.FrameCurrent->OnEnter();
	}
}
void PowerConsole::ConsoleApp::Update() {
	if (_members.FrameCurrent != nullptr) {
		_members.FrameCurrent->Update();
	}
}
	
#pragma endregion
//--------------------------------
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::ConsoleApp::_WriteBuffer() {
	bool changed = false;

	for (int i = 0; i < (int)_consoleData.ScreenInfo.dwSize.X; i++) {
		for (int j = 0; j < (int)_consoleData.ScreenInfo.dwSize.Y; j++) {
			_consoleData.Buffer[i + j * (int)_consoleData.ScreenInfo.dwSize.X].Char.AsciiChar	= (CHAR)_members.Graphics->GetPixel(Coord(i, j)).Char;
			_consoleData.Buffer[i + j * (int)_consoleData.ScreenInfo.dwSize.X].Attributes		= (WORD)_members.Graphics->GetPixel(Coord(i, j)).Color;
		}
	}

	for (int i = 0; i < _consoleData.ScreenInfo.dwSize.Y; i++) {
		for (int j = 0;  j < _consoleData.ScreenInfo.dwSize.X; j++) {

			int index = j + i * _consoleData.ScreenInfo.dwSize.X;
			if (_consoleData.Buffer[index].Char.AsciiChar != _consoleData.BufferLast[index].Char.AsciiChar ||
				_consoleData.Buffer[index].Attributes     != _consoleData.BufferLast[index].Attributes) {

				changed = true;
				_consoleData.BufferLast[index] = _consoleData.Buffer[index];
			}
		}
	}

	// Don't write anything if nothing has changed
	if (changed) {
		SMALL_RECT srect = SmallRect(0, 0, Coord(_consoleData.ScreenInfo.dwSize));
		WriteConsoleOutputA(hOut, _consoleData.Buffer, _consoleData.ScreenInfo.dwSize, Coord::Zero, &srect);
	}
}
void PowerConsole::ConsoleApp::Draw() {
	if (_members.FrameCurrent != nullptr) {
		_members.FrameCurrent->Draw(*_members.Graphics);
	}
}
	
#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

void PowerConsole::ConsoleApp::Show() {

}
void PowerConsole::ConsoleApp::Hide() {

}
void PowerConsole::ConsoleApp::Exit() {
	_members.Running = false;
}
void PowerConsole::ConsoleApp::Pause() {

}
	
#pragma endregion
//============ FRAMES ============
#pragma region Frames
	
void PowerConsole::ConsoleApp::ChangeFrame(Frame* frame) {
	_members.FrameNext = frame;
}
void PowerConsole::ConsoleApp::ChangeFrame(const string& id) {
	_members.FrameNext = GetFrame(id);
}
void PowerConsole::ConsoleApp::ChangeFrame(int index) {
	_members.FrameNext = GetFrame(index);
}

void PowerConsole::ConsoleApp::ClearFrames() {
	for (int i = 0; i < (int)_members.Frames.size(); i++) {
		//if (_app != nullptr) {
		_members.Frames[i]->Uninitialize();
		//}
		delete _members.Frames[i];
	}
	_members.Frames.clear();
}

void PowerConsole::ConsoleApp::AddFrame(Frame* frame) {
	if (!frame->IsInitialized()) {
		_members.Frames.push_back(frame);
		//if (_app != nullptr) {
		frame->Initialize(this);
		//}
	}
}
void PowerConsole::ConsoleApp::RemoveFrame(Frame* frame) {
	for (int i = 0; i < (int)_members.Frames.size(); i++) {
		if (_members.Frames[i] == frame) {
			//if (_app != nullptr) {
			_members.Frames[i]->Uninitialize();
			//}
			delete _members.Frames[i];
			_members.Frames.erase(_members.Frames.begin() + i);
			return;
		}
	}
}
void PowerConsole::ConsoleApp::RemoveFrame(const string& id) {
	for (int i = 0; i < (int)_members.Frames.size(); i++) {
		if (_members.Frames[i]->GetID() == id) {
			//if (_app != nullptr) {
			_members.Frames[i]->Uninitialize();
			//}
			delete _members.Frames[i];
			_members.Frames.erase(_members.Frames.begin() + i);
			return;
		}
	}
}
void PowerConsole::ConsoleApp::RemoveFrame(int index) {
	if (index < (int)_members.Frames.size()) {
		//if (_app != nullptr) {
		_members.Frames[index]->Uninitialize();
		//}
		delete _members.Frames[index];
		_members.Frames.erase(_members.Frames.begin() + index);
		return;
	}
}

Frame* PowerConsole::ConsoleApp::GetFrame(const string& id) {
	for (int i = 0; i < (int)_members.Frames.size(); i++) {
		if (_members.Frames[i]->GetID() == id) {
			return _members.Frames[i];
		}
	}
	return nullptr;
}
Frame* PowerConsole::ConsoleApp::GetFrame(int index) {
	if (index < (int)_members.Frames.size()) {
		return _members.Frames[index];
	}
	return nullptr;
}

bool PowerConsole::ConsoleApp::FrameExists(Frame* frame) {
	for (int i = 0; i < (int)_members.Frames.size(); i++) {
		if (_members.Frames[i] == frame) {
			return true;
		}
	}
	return false;
}
bool PowerConsole::ConsoleApp::FrameExists(const string& id) {
	for (int i = 0; i < (int)_members.Frames.size(); i++) {
		if (_members.Frames[i]->GetID() == id) {
			return true;
		}
	}
	return false;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Keyboard* PowerConsole::ConsoleApp::GetKeyboard() {
	return _members.Keyboard;
}
Mouse* PowerConsole::ConsoleApp::GetMouse() {
	return _members.Mouse;
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|
// UNDEFINITIONS												   |
//=================================================================/

#undef hWnd 
#undef hOut
#undef hIn

//=================================================================|
