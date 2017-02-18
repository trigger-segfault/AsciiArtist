//================================================================
// KEYS HEADER:
//================================================================
#pragma once
#ifndef KEYS_H
#define KEYS_H

#include "stdafx.h"

using namespace std;
//================================================================
// NAMESPACE:
//================================================================
/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of event handling classes. */
namespace Listeners {
//================================================================
// ENUMERATIONS:
//================================================================
/* The types of controls representable by codes. */
enum class ControlTypes {
	Key = 0,
	Mouse = 1,
	GamePad = 2
};
DEFINE_ENUM_FLAG_OPERATORS(Keys)
/* The list of all keys available on the keyboard. */
enum class Keys {
	Any = -1,
	/** No key pressed. */
	None = 0x00,
	
	// Mouse
	/** The left mouse button. */
	LButton = 0x01,
	/** The right mouse button. */
	RButton = 0x02,
	/** The middle mouse button (three-button mouse). */
	MButton = 0x04,
	/** The first x mouse button (five-button mouse). */
	XButton1 = 0x05,
	/** The second x mouse button (five-button mouse). */
	XButton2 = 0x06,

	// Special
	Break = 0x03,
	/** The BACKSPACE key. */
	Back = 0x08,
	/** The TAB key. */
	Tab = 0x09,
	Clear = 0x0C,
	/** The ENTER key. */
	Enter = 0x0D,
	/** The SHIFT modifier key. */
	Shift = 0x10,
	/** The CTRL modifier key. */
	Control = 0x11,
	/** The ALT modifier key. */
	Alt = 0x12,
	/** The PAUSE key. */
	Pause = 0x13,
	/** The CAPS LOCK key. */
	CapsLock = 0x14,
	/** The ESC key. */
	Escape = 0x1B,
	/** The SPACEBAR key. */
	Space = 0x20,
	/** The PAGE UP key. */
	PageUp = 0x21,
	/** The PAGE UP key. */
	Prior = 0x21,
	/** The PAGE DOWN key. */
	Next = 0x22,
	/** The PAGE DOWN key. */
	PageDown = 0x22,
	/** The END key. */
	End = 0x23,
	/** The HOME key. */
	Home = 0x24,
	/** The LEFT ARROW key. */
	Left = 0x25,
	/** The UP ARROW key. */
	Up = 0x26,
	/** The RIGHT ARROW key. */
	Right = 0x27,
	/** The DOWN ARROW key. */
	Down = 0x28,
	Select = 0x29,
	/** The PRINT key. */
	Print = 0x2A,
	/** The EXECUTE key. */
	Execute = 0x2B,
	/** The PRINT SCREEN key. */
	PrintScreen = 0x2C,
	/** The PRINT SCREEN key. */
	Snapshot = 0x2C,
	/** The INS key. */
	Insert = 0x2D,
	/** The DEL key. */
	Delete = 0x2E,
	/** The HELP key. */
	Help = 0x2F,

	// Numbers
	/** The 0 key. */
	D0 = 0x30,
	/** The 1 key. */
	D1 = 0x31,
	/** The 2 key. */
	D2 = 0x32,
	/** The 3 key. */
	D3 = 0x33,
	/** The 4 key. */
	D4 = 0x34,
	/** The 5 key. */
	D5 = 0x35,
	/** The 6 key. */
	D6 = 0x36,
	/** The 7 key. */
	D7 = 0x37,
	/** The 8 key. */
	D8 = 0x38,
	/** The 9 key. */
	D9 = 0x39,

	// Letters
	/** The A key. */
	A = 0x41,
	/** The B key. */
	B = 0x42,
	/** The C key. */
	C = 0x43,
	/** The D key. */
	D = 0x44,
	/** The E key. */
	E = 0x45,
	/** The F key. */
	F = 0x46,
	/** The G key. */
	G = 0x47,
	/** The H key. */
	H = 0x48,
	/** The I key. */
	I = 0x49,
	/** The J key. */
	J = 0x4A,
	/** The K key. */
	K = 0x4B,
	/** The L key. */
	L = 0x4C,
	/** The M key. */
	M = 0x4D,
	/** The N key. */
	N = 0x4E,
	/** The O key. */
	O = 0x4F,
	/** The P key. */
	P = 0x50,
	/** The Q key. */
	Q = 0x51,
	/** The R key. */
	R = 0x52,
	/** The S key. */
	S = 0x53,
	/** The T key. */
	T = 0x54,
	/** The U key. */
	U = 0x55,
	/** The V key. */
	V = 0x56,
	/** The W key. */
	W = 0x57,
	/** The X key. */
	X = 0x58,
	/** The Y key. */
	Y = 0x59,
	/** The Z key. */
	Z = 0x5A,
	/** The left Windows logo key (Microsoft Natural Keyboard). */
	LWin = 0x5B,
	/** The right Windows logo key (Microsoft Natural Keyboard). */
	RWin = 0x5C,
	Applications = 0x5D,
	Sleep = 0x5F,

	// Numpad
	/** The 0 key on the numeric keypad. */
	NumPad0 = 0x60,
	/** The 1 key on the numeric keypad. */
	NumPad1 = 0x61,
	/** The 2 key on the numeric keypad. */
	NumPad2 = 0x62,
	/** The 3 key on the numeric keypad. */
	NumPad3 = 0x63,
	/** The 4 key on the numeric keypad. */
	NumPad4 = 0x64,
	/** The 5 key on the numeric keypad. */
	NumPad5 = 0x65,
	/** The 6 key on the numeric keypad. */
	NumPad6 = 0x66,
	/** The 7 key on the numeric keypad. */
	NumPad7 = 0x67,
	/** The 8 key on the numeric keypad. */
	NumPad8 = 0x68,
	/** The 9 key on the numeric keypad. */
	NumPad9 = 0x69,
	Multiply = 0x6A,
	Add = 0x6B,
	Seperator = 0x6C,
	Subtract = 0x6D,
	/** The decimal key. */
	Decimal = 0x6E,
	/** The divide key. */
	Divide = 0x6F,

	// Function
	/** The F1 key. */
	F1 = 0x70,
	/** The F2 key. */
	F2 = 0x71,
	/** The F3 key. */
	F3 = 0x72,
	/** The F4 key. */
	F4 = 0x73,
	/** The F5 key. */
	F5 = 0x74,
	/** The F6 key. */
	F6 = 0x75,
	/** The F7 key. */
	F7 = 0x76,
	/** The F8 key. */
	F8 = 0x77,
	/** The F9 key. */
	F9 = 0x78,
	/** The F10 key. */
	F10 = 0x79,
	/** The F11 key. */
	F11 = 0x7A,
	/** The F12 key. */
	F12 = 0x7B,
	/** The F13 key. */
	F13 = 0x7C,
	/** The F14 key. */
	F14 = 0x7D,
	/** The F15 key. */
	F15 = 0x7E,
	/** The F16 key. */
	F16 = 0x7F,
	/** The F17 key. */
	F17 = 0x80,
	/** The F18 key. */
	F18 = 0x81,
	/** The F19 key. */
	F19 = 0x82,
	/** The F20 key. */
	F20 = 0x83,
	/** The F21 key. */
	F21 = 0x84,
	/** The F22 key. */
	F22 = 0x85,
	/** The F23 key. */
	F23 = 0x86,
	/** The F24 key. */
	F24 = 0x87,

	// Modifiers
	Numlock = 0x90,
	ScrollLock = 0x91,
	LeftShift = 0xA0,
	RightShift = 0xA1,
	LeftControl = 0xA2,
	RightContol = 0xA3,
	LeftAlt = 0xA4,
	RightAlt = 0xA5,

	// Browser
	/** The browser back key (Windows 2000 or later). */
	BrowserBack = 0xA6,
	/** The browser forward key (Windows 2000 or later). */
	BrowserForward = 0xA7,
	/** The browser refresh key (Windows 2000 or later). */
	BrowserRefresh = 0xA8,
	/** The browser stop key (Windows 2000 or later). */
	BrowserStop = 0xA9,
	/** The browser search key (Windows 2000 or later). */
	BrowserSearch = 0xAA,
	/** The browser favorites key (Windows 2000 or later). */
	BrowserFavorites = 0xAB,
	/** The browser home key (Windows 2000 or later). */
	BrowserHome = 0xAC,

	// Media
	/** The volume mute key (Windows 2000 or later). */
	VolumeMute = 0xAD,
	/** The volume down key (Windows 2000 or later). */
	VolumeDown = 0xAE,
	/** The volume up key (Windows 2000 or later). */
	VolumeUp = 0xAF,
	/**  */
	NextTrack = 0xB0,
	/**  */
	PreviousTrack = 0xB1,
	/**  */
	StopMedia = 0xB2,
	/**  */
	PlayPause = 0xB3,

	// Launch
	/**  */
	LaunchMail = 0xB4,
	/**  */
	SelectMedia = 0xB5,
	/**  */
	LaunchApp1 = 0xB6,
	/**  */
	LaunchApp2 = 0xB7,

	// OEM
	/**  */
	OEM1 = 0xBA,
	/**  */
	OEMPlus = 0xB8,
	/**  */
	OEMComma = 0xBC,
	/**  */
	OEMMinus = 0xBD,
	/**  */
	OEMPeriod = 0xBE,
	/**  */
	OEM2 = 0xBF,
	/**  */
	OEM3 = 0xC0,
	/**  */
	OEM4 = 0xDB,
	/**  */
	OEM5 = 0xDC,
	/**  */
	OEM6 = 0xDD,
	/**  */
	OEM7 = 0xDE,
	/**  */
	OEM8 = 0xDF,
	/**  */
	OEM102 = 0xE2,
	/**  */
	Process = 0xE5,
	/**  */
	Packet = 0xE7,
	/**  */
	Attn = 0xF6,
	/**  */
	CrSel = 0xF7,
	/**  */
	ExSel = 0xF8,
	/**  */
	EraseEOF = 0xF9,
	/**  */
	Play = 0xFA,
	/** The ZOOM key. */
	Zoom = 0xFB,
	/** The PA1 key. */
	PA1 = 0xFD,
	/** The CLEAR key. */
	OEMClear = 0xFE
};
/* The list of all buttons available on the mouse. */
enum class MouseButtons {
	
	Any = -1,
	None = 0x00,

	Left = 0x01,
	Right = 0x02,
	Middle = 0x03,
	X1 = 0x04,
	X2 = 0x05,

	WheelUp = 0x06,
	WheelDown = 0x07,
	WheelLeft = 0x08,
	WheelRight = 0x09
};
/* The list of all buttons available on the gampad. */
enum class GameButtons {
	Any = -1,
	None = 0x00,

	// basic
	A,
	B,
	X,
	Y,
	Start,
	Select,
	Home,

	// Triggers
	LeftShoulder,
	RightShoulder,
	LeftStickButton,
	RightStickButton,

	// DPad
	DPadRight,
	DPadDown,
	DPadLeft,
	DPadUp,

	// Thumb Sticks
	LeftStickRight,
	LeftStickDown,
	LeftStickLeft,
	LeftStickUp,

	RightStickRight,
	RightStickDown,
	RightStickLeft,
	RightStickUp,
};
//================================================================
// END KEYS HEADER:
//================================================================
}}
#endif