/*=================================================================|
|* File:				ConsoleApp.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Decleration of the Keyboard listener class.		  *|
|=================================================================*/

#pragma once
#ifndef CONSOLE_APP_H
#define CONSOLE_APP_H

#include "Stdafx.h"
#include "AsciiImage.h"
#include "Geometry.h"
#include "Keys.h"
#include "MouseButtons.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ControlHandler.h"
#include "HiddenWinAPI.h"
#include "Frame.h"
#include "Component.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {

//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* The different types of styles available for the window. */
enum WindowStyles : unsigned short {
	/* The window has no styles. */
	StyleNone			= 0x0000,
	/* All the window styles. */
	StyleAll			= 0x00F7,


	/* The window can be resized manually. */
	StyleResizable		= 0x0001,
	/* The window uses the tool window style border. */
	StyleToolWindow		= 0x0002,
	/* The window can accept files being dropped into it. */
	StyleAcceptFiles	= 0x0004,

	/* The window's close button is enabled. */
	StyleCloseButton	= 0x0010,
	/* The window's maximize button is enabled. */
	StyleMaximizeButton	= 0x0020,
	/* The window's minimize button is enabled. */
	StyleMinimizeButton	= 0x0040,

	/* The console menu items are enabled. */
	StyleMenuItems		= 0x0080
};

#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The class that runs the Power Console application. */
class ConsoleApp {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
	//--------------------------------
private:
	#pragma region ConsoleData

	/* The structure for storing data about the console . */
	struct _ConsoleData {

		// Buffer
		/* The current character buffer. */
		CHAR_INFO* Buffer;
		/* The previous character buffer. */
		CHAR_INFO* BufferLast;
	
		// Screen
		/* The current screen info. */
		CONSOLE_SCREEN_BUFFER_INFOEX ScreenInfo;
		/* The next screen info. */
		CONSOLE_SCREEN_BUFFER_INFOEX ScreenInfoNew;
		/* True if the screen info needs to be updated. */
		bool ScreenInfoChanged;
		
		// Font
		/* The current size of the console font. */
		Coord FontSize;

		HANDLE HOut;
		HANDLE HIn;
	};

	#pragma endregion
	//--------------------------------
	#pragma region WindowData
	/* The structure for storing data about the window . */
	struct _WindowData {
		
		// Window Info
		/* The current window info. */
		WINDOWINFO WindowInfo;
		/* The next window info. */
		WINDOWINFO WindowInfoNew;
		/* True if the window position needs to be updated. */
		bool WindowPositionChanged;
		/* True if the window style needs to be updated. */
		bool WindowStyleChanged;

		// Menu Items
		/* The list of console menu items. */
		MENUITEMINFOA MenuItemInfo[4];
		/* The positions of the console menu items. */
		unsigned short MenuItemPositions[4];
		/* True if the console menu items are currently enabled. */
		bool MenuItemsEnabled;
		/* True if the console menu items will be enabled. */
		bool MenuItemsEnabledNew;
		/* True if the close button is currently enabled. */
		bool CloseButtonEnabled;
		/* True if the close button will be enabled. */
		bool CloseButtonEnabledNew;
		/* True if the menu items were changed. */
		bool MenuItemsChanged;

		// Window Title
		/* The current title of the window. */
		string WindowTitle;
		/* The next title of the window. */
		string WindowTitleNew;
		/* True if the window title needs to be updated. */
		bool WindowTitleChanged;

		// Windows Settings
		/* The current screen resolution. */
		Coord ScreenResolution;
		
		// Mouse
		/* True if the mouse is currently visible. */
		bool MouseVisible;
		/* True if the mouse will be visible. */
		bool MouseVisibleNew;
		/* True if the mouse visibility was changed. */
		bool MouseVisibleChanged;

		// Focus
		/* True if the window is currently active. */
		bool Active;
		/* True if the window active state changed. */
		bool ActiveChanged;

		// Events
		/* True if the window was moved. */
		bool WindowMoved;

		HWND HWnd;
	};

	#pragma endregion
	//--------------------------------
	#pragma region ConsoleAppMembers

	/* The structure for storing members for the console app . */
	struct _ConsoleAppMembers {
		
		// Event Data
		/* The structure for the input record event. */
		INPUT_RECORD InRec;

		// Update Rate
		/* The point in time at which the frame started. */
		int FrameStart;
		/* The duration of the last frame. */
		int FrameDuration;
		/* The frame rate of the program. */
		double FPS;
		
		/* The keyboard input handler. */
		Keyboard* Keyboard;
		/* The mouse input handler. */
		Mouse* Mouse;
		
		/* True if the application is still running. */
		bool Running;
		
		/* The graphics to draw to. */
		AsciiImage* Graphics;

		/* The list of frames in the app. */
		vector<Frame*> Frames;
		Frame* FrameCurrent;
		Frame* FrameNext;

		int Argc;
		char** Argv;
		
	};
	#pragma endregion
	//--------------------------------
	#pragma region Data

	/* The structure of console data. */
	_ConsoleData _consoleData;
	/* The structure of window data. */
	_WindowData _windowData;
protected:
	/* The structure of console app members. */
	_ConsoleAppMembers _members;
	
	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default console app. */
	ConsoleApp(int argc, char** argv);
	/* Cleans up the console app. */
	~ConsoleApp();
private:
	/* Initializes the console screen info. */
	void _InitializeScreen();
	/* Initializes the console buffer. */
	void _InitializeBuffer();
	/* Initializes the console IO mode. */
	void _InitializeIOMode();
	/* Initializes the console cursor info. */
	void _InitializeCursor();
	/* Initializes the window info. */
	void _InitializeWindow();
	/* Initializes the console menu items. */
	void _InitializeMenuItems();
	/* Initializes the console mouse info. */
	void _InitializeMouse();
public:
	/* Initializes the console app. */
	virtual void Initialize();
	/* Uninitializes the console app. */
	virtual void Uninitialize();
	/* Runs the console app. */
	void Run();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Program Arguments

	/* Gets the number of arguments. */
	int GetNumArguments();
	/* Gets the argument at the specified index. */
	string GetArgument(int index);
	
	#pragma endregion
	//--------------------------------
	#pragma region Console Dimensions

	/* Gets the size of the console screen. */
	Coord GetConsoleSize();
	/* Sets the size of the console screen. */
	void SetConsoleSize(Coord size);
	/* Gets the maximum size of the console screen. */
	Coord GetMaxConsoleSize();
	
	#pragma endregion
	//--------------------------------
	#pragma region Window Dimensions

	/* Gets the size of the window. */
	Coord GetWindowSize();
	/* Gets the client size of the window. */
	Coord GetClientSize();
	/* Gets the position of the window. */
	Coord GetWindowPosition();
	/* Gets the client position of the window. */
	Coord GetClientPosition();
	/* Sets the position of the window. */
	void SetWindowPosition(Coord position);
	/* Sets the client position of the window. */
	void SetClientPosition(Coord position);
	
	#pragma endregion
	//--------------------------------
	#pragma region Console Visuals
	
	/* Gets the specified color used by the console. */
	void GetColor(int index);
	/* Sets the specified color used by the console. */
	void SetColor(int index);
	/* Gets the list of 16 colors used by the console. */
	void GetColors();
	/* Sets the list of 16 colors used by the console. */
	void SetColors();
	/* Gets the size of the console font. */
	Coord GetFontSize();
	
	#pragma endregion
	//--------------------------------
	#pragma region Window Visuals

	/* Gets the title of the window. */
	string GetTitle();
	/* Sets the title of the window. */
	void SetTitle(const string& title);
	/* Gets whether the mouse is visible when inside the window. */
	bool IsMouseVisible();
	/* Sets whether the mouse is visible when inside the window. */
	void SetMouseVisible(bool visible);
	/* Gets whether the window is resizable. */
	bool IsResizable();
	/* Sets whether the window is resizable. */
	void SetResizable(bool resizable);
	/* Gets the style of the window. */
	unsigned short GetStyle();
	/* Sets the style of the window. */
	void SetStyle(unsigned short style);
	
	#pragma endregion
	//--------------------------------
	#pragma region Information

	/* Gets whether the window is active. */
	bool IsActive();
	/* Gets whether the window has focus. */
	bool IsFocus();
	/* Gets the number of milliseconds per update in the application. */
	int GetUpdateRate();
	
	#pragma endregion
	//--------------------------------
	#pragma region Handles

	/* Gets the handle to the console window. */
	HWND GetWindowHandle();
	/* Gets the console output handle. */
	HANDLE GetOuputHandle();
	/* Gets the console input handle. */
	HANDLE GetInputHandle();
	
	#pragma endregion
	//--------------------------------
	#pragma region Frames
	
	/* Gets the number of frames in the application. */
	unsigned int GetNumFrames();
	/* Gets the list of frames in the application. */
	vector<Frame*>& GetFrames();

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating
private:
	/* Applies any changes made to the screen. */
	void _ApplyScreenChanges();
	/* Resizes the console buffer. */
	void _ResizeBuffer();
	/* Updates and reads the console's settings. */
	void _UpdateConsole();

	/* Applies any changes made to the window info. */
	void _ApplyWindowChanges();
	/* Applies any changes made to the console menu items. */
	void _ApplyMenuItemChanges();
	/* Applies any changes made to the mouse. */
	void _ApplyMouseChanges();
	/* Updates and reads the window's settings. */
	void _UpdateWindow();

	/* The main loop of the console app. */
	void _MainLoop();
	/* Changes to the new frame if one was specified. */
	void _UpdateFrameChange();
public:
	/* Called every step to update to the console. */
	virtual void Update();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing
private:
	/* Writes the current buffer to the console. */
	void _WriteBuffer();
public:
	/* Called every step to draw to the console. */
	virtual void Draw();
	
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management
	
	/* Shows the console application. */
	void Show();
	/* Hides the console application. */
	void Hide();
	/* Exits the console application. */
	void Exit();
	/* Pauses the console application until a key is pressed. */
	void Pause();
	
	#pragma endregion
	//============ FRAMES ============
	#pragma region Frames
	
	/* Changes the current frame to the specified frame. */
	void ChangeFrame(Frame* frame);
	/* Changes the current frame to the frame with the specified id. */
	void ChangeFrame(const string& id);
	/* Changes the current frame to the frame at the specified index. */
	void ChangeFrame(int index);

	/* Removes all the frames from the application. */
	void ClearFrames();

	/* Adds the specified frame to the application. */
	void AddFrame(Frame* frame);
	/* Removes the specified frame from the application. */
	void RemoveFrame(Frame* frame);
	/* Removes the first frame with the specified id from the application. */
	void RemoveFrame(const string& id);
	/* Removes the frame at the specified index from the application. */
	void RemoveFrame(int index);
	
	/* Gets the first frame with the specified id in the application. */
	Frame* GetFrame(const string& id);
	/* Gets the frame at the specified index in the application. */
	Frame* GetFrame(int index);
	
	/* Returns true if the specified frame exists in the application. */
	bool FrameExists(Frame* frame);
	/* Returns true if a frame with the specified id exists in the application. */
	bool FrameExists(const string& id);

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
	
	/* Get the keyboard event handler. */
	Keyboard* GetKeyboard();
	/* Get the mouse event handler. */
	Mouse* GetMouse();

	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace PowerConsole */
#endif /* CONSOLE_APP_H */
//=================================================================|
