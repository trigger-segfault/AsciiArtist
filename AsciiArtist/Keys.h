/*=================================================================|
|* File:				Keys.h									  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/19/2013								  *|
|*																  *|
|* Description: Definition of the virtual keys enumeration.		  *|
|=================================================================*/

#pragma once
#ifndef KEYS_ENUM_H
#define KEYS_ENUM_H

#include "Stdafx.h"

using namespace std;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of event handling classes. */
namespace Input {

//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* Specifies key codes and modifiers. Enumeration taken from .NET Framework. */
enum class Keys : unsigned int {
	
	//========== CONSTANTS ===========
	#pragma region Non-Specific
	
	/* No key pressed. */
	None = 0x00,
	
	#pragma endregion
	//--------------------------------
	#pragma region Mouse Buttons
	
	/* The left mouse button. */
	LButton = 0x01,
	/* The right mouse button. */
	RButton = 0x02,
	/* The middle mouse button (three-button mouse). */
	MButton = 0x04,
	/* The first x mouse button (five-button mouse). */
	XButton1 = 0x05,
	/* The second x mouse button (five-button mouse). */
	XButton2 = 0x06,
	
	#pragma endregion
	//--------------------------------
	#pragma region Command Keys
	
	/* The BACKSPACE key. */
	Back = 0x08,
	/* The TAB key. */
	Tab = 0x09,
	/* The ENTER key. */
	Enter = 0x0D,
	/* The RETURN key. */
	Return = 0x0D,
	/* The ESC key. */
	Escape = 0x1B,
	/* The SPACEBAR key. */
	Space = 0x20,
	
	/* The INS key. */
	Insert = 0x2D,
	/* The DEL key. */
	Delete = 0x2E,
	
	#pragma endregion
	//--------------------------------
	#pragma region Navigation Keys
	
	/* The PAGE UP key. */
	Prior = 0x21,
	/* The PAGE UP key. */
	PageUp = 0x21,
	/* The PAGE DOWN key. */
	PageDown = 0x22,
	/* The PAGE DOWN key. */
	Next = 0x22,
	/* The END key. */
	End = 0x23,
	/* The HOME key. */
	Home = 0x24,
	
	/* The LEFT ARROW key. */
	Left = 0x25,
	/* The UP ARROW key. */
	Up = 0x26,
	/* The RIGHT ARROW key. */
	Right = 0x27,
	/* The DOWN ARROW key. */
	Down = 0x28,
	
	#pragma endregion
	//--------------------------------
	#pragma region Special Keys
	
	/* The CANCEL key. */
	Cancel = 0x03,
	/* The LINEFEED key. */
	LineFeed = 0x0A,
	/* The CLEAR key. */
	Clear = 0x0C,
	/* The PAUSE key. */
	Pause = 0x13,
	/* The SELECT key. */
	Select = 0x29,
	/* The PRINT key. */
	Print = 0x2A,
	/* The EXECUTE key. */
	Execute = 0x2B,
	/* The PRINT SCREEN key. */
	Snapshot = 0x2C,
	/* The PRINT SCREEN key. */
	PrintScreen = 0x2C,
	/* The HELP key. */
	Help = 0x2F,
	/* The computer sleep key. */
	Sleep = 0x5F,

	/* The left Windows logo key (Microsoft Natural Keyboard). */
	LWin = 0x5B,
	/* The right Windows logo key (Microsoft Natural Keyboard). */
	RWin = 0x5C,
	/* The application key (Microsoft Natural Keyboard). */
	Apps = 0x5D,
	
	#pragma endregion
	//--------------------------------
	#pragma region Number Keys
	
	/* The 0 key. */
	D0 = 0x30,
	/* The 1 key. */
	D1 = 0x31,
	/* The 2 key. */
	D2 = 0x32,
	/* The 3 key. */
	D3 = 0x33,
	/* The 4 key. */
	D4 = 0x34,
	/* The 5 key. */
	D5 = 0x35,
	/* The 6 key. */
	D6 = 0x36,
	/* The 7 key. */
	D7 = 0x37,
	/* The 8 key. */
	D8 = 0x38,
	/* The 9 key. */
	D9 = 0x39,
	
	#pragma endregion
	//--------------------------------
	#pragma region Letter Keys
	
	/* The A key. */
	A = 0x41,
	/* The B key. */
	B = 0x42,
	/* The C key. */
	C = 0x43,
	/* The D key. */
	D = 0x44,
	/* The E key. */
	E = 0x45,
	/* The F key. */
	F = 0x46,
	/* The G key. */
	G = 0x47,
	/* The H key. */
	H = 0x48,
	/* The I key. */
	I = 0x49,
	/* The J key. */
	J = 0x4A,
	/* The K key. */
	K = 0x4B,
	/* The L key. */
	L = 0x4C,
	/* The M key. */
	M = 0x4D,
	/* The N key. */
	N = 0x4E,
	/* The O key. */
	O = 0x4F,
	/* The P key. */
	P = 0x50,
	/* The Q key. */
	Q = 0x51,
	/* The R key. */
	R = 0x52,
	/* The S key. */
	S = 0x53,
	/* The T key. */
	T = 0x54,
	/* The U key. */
	U = 0x55,
	/* The V key. */
	V = 0x56,
	/* The W key. */
	W = 0x57,
	/* The X key. */
	X = 0x58,
	/* The Y key. */
	Y = 0x59,
	/* The Z key. */
	Z = 0x5A,
	
	#pragma endregion
	//--------------------------------
	#pragma region NumPad Keys
	
	/* The 0 key on the numeric keypad. */
	NumPad0 = 0x60,
	/* The 1 key on the numeric keypad. */
	NumPad1 = 0x61,
	/* The 2 key on the numeric keypad. */
	NumPad2 = 0x62,
	/* The 3 key on the numeric keypad. */
	NumPad3 = 0x63,
	/* The 4 key on the numeric keypad. */
	NumPad4 = 0x64,
	/* The 5 key on the numeric keypad. */
	NumPad5 = 0x65,
	/* The 6 key on the numeric keypad. */
	NumPad6 = 0x66,
	/* The 7 key on the numeric keypad. */
	NumPad7 = 0x67,
	/* The 8 key on the numeric keypad. */
	NumPad8 = 0x68,
	/* The 9 key on the numeric keypad. */
	NumPad9 = 0x69,
	
	/* The multiply key. */
	Multiply = 0x6A,
	/* The add key. */
	Add = 0x6B,
	/* The separator key. */
	Separator = 0x6C,
	/* The subtract key. */
	Subtract = 0x6D,
	/* The decimal key. */
	Decimal = 0x6E,
	/* The divide key. */
	Divide = 0x6F,
	
	#pragma endregion
	//--------------------------------
	#pragma region Function Keys
	
	/* The F1 key. */
	F1 = 0x70,
	/* The F2 key. */
	F2 = 0x71,
	/* The F3 key. */
	F3 = 0x72,
	/* The F4 key. */
	F4 = 0x73,
	/* The F5 key. */
	F5 = 0x74,
	/* The F6 key. */
	F6 = 0x75,
	/* The F7 key. */
	F7 = 0x76,
	/* The F8 key. */
	F8 = 0x77,
	/* The F9 key. */
	F9 = 0x78,
	/* The F10 key. */
	F10 = 0x79,
	/* The F11 key. */
	F11 = 0x7A,
	/* The F12 key. */
	F12 = 0x7B,
	/* The F13 key. */
	F13 = 0x7C,
	/* The F14 key. */
	F14 = 0x7D,
	/* The F15 key. */
	F15 = 0x7E,
	/* The F16 key. */
	F16 = 0x7F,
	/* The F17 key. */
	F17 = 0x80,
	/* The F18 key. */
	F18 = 0x81,
	/* The F19 key. */
	F19 = 0x82,
	/* The F20 key. */
	F20 = 0x83,
	/* The F21 key. */
	F21 = 0x84,
	/* The F22 key. */
	F22 = 0x85,
	/* The F23 key. */
	F23 = 0x86,
	/* The F24 key. */
	F24 = 0x87,
	
	#pragma endregion
	//--------------------------------
	#pragma region Modifier Keys
	
	/* The SHIFT key. */
	ShiftKey = 0x10,
	/* The CTRL key. */
	ControlKey = 0x11,
	/* The ALT key. */
	Menu = 0x12,
	
	/* The CAPS LOCK key. */
	CapsLock = 0x14,
	/* The CAPS LOCK key. */
	Capital = 0x14,
	/* The NUM LOCK key. */
	NumLock = 0x90,
	/* The SCROLL LOCK key. */
	Scroll = 0x91,
	
	/* The left SHIFT key. */
	LShiftKey = 0xA0,
	/* The right SHIFT key. */
	RShiftKey = 0xA1,
	/* The left CTRL key. */
	LControlKey = 0xA2,
	/* The right CTRL key. */
	RControlKey = 0xA3,
	/* The left ALT key. */
	LMenu = 0xA4,
	/* The right ALT key. */
	RMenu = 0xA5,
	
	#pragma endregion
	//--------------------------------
	#pragma region Browser Keys
	
	/* The browser back key (Windows 2000 or later). */
	BrowserBack = 0xA6,
	/* The browser forward key (Windows 2000 or later). */
	BrowserForward = 0xA7,
	/* The browser refresh key (Windows 2000 or later). */
	BrowserRefresh = 0xA8,
	/* The browser stop key (Windows 2000 or later). */
	BrowserStop = 0xA9,
	/* The browser search key (Windows 2000 or later). */
	BrowserSearch = 0xAA,
	/* The browser favorites key (Windows 2000 or later). */
	BrowserFavorites = 0xAB,
	/* The browser home key (Windows 2000 or later). */
	BrowserHome = 0xAC,
	
	#pragma endregion
	//--------------------------------
	#pragma region Volume Keys
	
	/* The volume mute key (Windows 2000 or later). */
	VolumeMute = 0xAD,
	/* The volume down key (Windows 2000 or later). */
	VolumeDown = 0xAE,
	/* The volume up key (Windows 2000 or later). */
	VolumeUp = 0xAF,
	
	#pragma endregion
	//--------------------------------
	#pragma region Media Keys
	
	/* The media next track key (Windows 2000 or later). */
	MediaNextTrack = 0xB0,
	/* The media previous track key (Windows 2000 or later). */
	MediaPreviousTrack = 0xB1,
	/* The media Stop key (Windows 2000 or later). */
	MediaStop = 0xB2,
	/* The media play pause key (Windows 2000 or later). */
	MediaPlayPause = 0xB3,
	
	#pragma endregion
	//--------------------------------
	#pragma region Launch Keys
	
	/* The launch mail key (Windows 2000 or later). */
	LaunchMail = 0xB4,
	/* The select media key (Windows 2000 or later). */
	SelectMedia = 0xB5,
	/* The start application one key (Windows 2000 or later). */
	LaunchApplication1 = 0xB6,
	/* The start application two key (Windows 2000 or later). */
	LaunchApplication2 = 0xB7,
	
	#pragma endregion
	//--------------------------------
	#pragma region Original Equipment Manufacturer (OEM) Keys
	
	/* The OEM Semicolon key on a US standard keyboard (Windows 2000 or later). */
	OemSemicolon = 0xBA,
	/* The OEM 1 key. */
	Oem1 = 0xBA,
	/* The OEM plus key on any country/region keyboard (Windows 2000 or later). */
	Oemplus = 0xBB,
	/* The OEM comma key on any country/region keyboard (Windows 2000 or later). */
	Oemcomma = 0xBC,
	/* The OEM minus key on any country/region keyboard (Windows 2000 or later). */
	OemMinus = 0xBD,
	/* The OEM period key on any country/region keyboard (Windows 2000 or later). */
	OemPeriod = 0xBE,
	/* The OEM 2 key. */
	Oem2 = 0xBF,
	/* The OEM question mark key on a US standard keyboard (Windows 2000 or later). */
	OemQuestion = 0xBF,
	/* The OEM 3 key. */
	Oem3 = 0xC0,
	/* The OEM tilde key on a US standard keyboard (Windows 2000 or later). */
	Oemtilde = 0xC0,
	/* The OEM 4 key. */
	Oem4 = 0xDB,
	/* The OEM open bracket key on a US standard keyboard (Windows 2000 or later). */
	OemOpenBrackets = 0xDB,
	/* The OEM pipe key on a US standard keyboard (Windows 2000 or later). */
	OemPipe = 0xDC,
	/* The OEM 5 key. */
	Oem5 = 0xDC,
	/* The OEM close bracket key on a US standard keyboard (Windows 2000 or later). */
	OemCloseBrackets = 0xDD,
	/* The OEM 6 key. */
	Oem6 = 0xDD,
	/* The OEM singled/double quote key on a US standard keyboard (Windows 2000 or later). */
	OemQuotes = 0xDE,
	/* The OEM 7 key. */
	Oem7 = 0xDE,
	/* The OEM 8 key. */
	Oem8 = 0xDF,
	/* The OEM 102 key. */
	Oem102 = 0xE2,
	/* The OEM angle bracket or backslash key on the RT 102 key keyboard (Windows 2000 or later). */
	OemBackslash = 0xE2,
	/* The CLEAR key. */
	OemClear = 0xFE,
	
	#pragma endregion
	//--------------------------------
	#pragma region Misc Keys
	
	/* The PROCESS KEY key. */
	ProcessKey = 0xE5,
	/* Used to pass Unicode characters as if they were keystrokes. The Packet key value is the low word of a 32-bit virtual-key value used for non-keyboard input methods. */
	Packet = 0xE7,
	/* The ATTN key. */
	Attn = 0xF6,
	/* The CRSEL key. */
	Crsel = 0xF7,
	/* The EXSEL key. */
	Exsel = 0xF8,
	/* The ERASE EOF key. */
	EraseEof = 0xF9,
	/* The PLAY key. */
	Play = 0xFA,
	/* The ZOOM key. */
	Zoom = 0xFB,
	/* A constant reserved for future use. */
	NoName = 0xFC,
	/* The PA1 key. */
	Pa1 = 0xFD,
	
	#pragma endregion
	//--------------------------------
	#pragma region Input Method Editor (IME) Keys
	
	/* The IME Hangul mode key. */
	HangulMode = 0x15,
	/* The IME Hanguel mode key. (maintained for compatibility; use HangulMode) */
	HanguelMode = 0x15,
	/* The IME Kana mode key. */
	KanaMode = 0x15,
	/* The IME Junja mode key. */
	JunjaMode = 0x17,
	/* The IME final mode key. */
	FinalMode = 0x18,
	/* The IME Kanji mode key. */
	KanjiMode = 0x19,
	/* The IME Hanja mode key. */
	HanjaMode = 0x19,
	
	/* The IME convert key. */
	IMEConvert = 0x1C,
	/* The IME nonconvert key. */
	IMENonconvert = 0x1D,
	/* The IME accept key, replaces System.Windows.Forms.Keys.IMEAceept. */
	IMEAccept = 0x1E,
	/* The IME accept key. Obsolete, use System.Windows.Forms.Keys.IMEAccept instead. */
	IMEAceept = 0x1E,
	/* The IME mode change key. */
	IMEModeChange = 0x1F,
	
	#pragma endregion
	//--------------------------------
	#pragma region Modifiers Flags
	
	/* The bitmask to extract a key code from a key value. */
	//KeyCode = 0xFFFF,
	/* The SHIFT modifier key. */
	//Shift = 0x10000,
	/* The CTRL modifier key. */
	//Control = 0x20000,
	/* The ALT modifier key. */
	//Alt = 0x40000,
	/* The bitmask to extract modifiers from a key value. */
	//Modifiers = 0xFFFF0000
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
// OPERATORS													   |
//=================================================================/
#pragma region Operators

/* Flag operators for Keys enum. */
//DEFINE_ENUM_FLAG_OPERATORS(Keys)

#pragma endregion
//=================================================================|
} /* namespace Input */
} /* namespace PowerConsole */
#endif /* KEYS_ENUM_H */
//=================================================================|
