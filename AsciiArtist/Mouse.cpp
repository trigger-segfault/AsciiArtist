/*=================================================================|
|* File:				Mouse.cpp								  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Definition of the Mouse listener class.			  *|
|=================================================================*/

#include "Stdafx.h"
#include "Mouse.h"

using namespace std;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;

//=================================================================|
// DEFINITIONS													   |
//=================================================================/

/* Returns true if the flag is applied to the attributes. */
#define GetFlag(attributes, flag)	(((attributes) & (flag)) == (flag))

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region Mouse
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Input::Mouse::Mouse() {
	_members.MousePoint		= Coord();
	_members.MousePointLast	= Coord();
	_members.MouseClient		= Coord();
	_members.MouseClientLast	= Coord();
	_members.MouseCoord		= Coord();
	_members.MouseCoordLast	= Coord();
	_members.MouseWheelDelta	= Coord();
	_members.ButtonStates		= new ControlHandler[NumButtons];
	_members.Disabled			= false;
	_members.MouseInside		= false;

	for (int i = 0; i < NumButtons; i++) {
		_members.ButtonStates[i] = ControlHandler();
	}
}
PowerConsole::Input::Mouse::~Mouse() {
	delete[] _members.ButtonStates;
	_members.ButtonStates = nullptr;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Input::Mouse::Update(INPUT_RECORD& inRec, bool active, Coord clientPosition, Coord consoleSize, Coord fontSize) {
	
	// Get the mouse position
	_members.MouseCoordLast		= _members.MouseCoord;
	_members.MouseClientLast	= _members.MouseClient;
	_members.MousePointLast		= _members.MousePoint;

	_members.MouseWheelDelta	= Coord();

	POINT mousePoint;
	if (active && GetCursorPos(&mousePoint) != 0) {
		_members.MousePoint		= mousePoint;
		_members.MouseClient	= _members.MousePoint - clientPosition;
		_members.MouseCoord		= _members.MouseClient / fontSize;
	}

	// Get any button events
	bool buttons[NumButtons];
	buttons[(int)MouseButtons::None]		= false;
	buttons[(int)MouseButtons::Left]		= GetAsyncKeyState((int)Keys::LButton) != 0;
	buttons[(int)MouseButtons::Right]		= GetAsyncKeyState((int)Keys::RButton) != 0;
	buttons[(int)MouseButtons::Middle]		= GetAsyncKeyState((int)Keys::MButton) != 0;
	buttons[(int)MouseButtons::XButton1]	= GetAsyncKeyState((int)Keys::XButton1) != 0;
	buttons[(int)MouseButtons::XButton2]	= GetAsyncKeyState((int)Keys::XButton2) != 0;
	bool doubleClick = false;

	if (inRec.EventType == MOUSE_EVENT && active) {
		if (inRec.Event.MouseEvent.dwEventFlags == 0x0000 || inRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK) {
			unsigned long buttonState = inRec.Event.MouseEvent.dwButtonState;
			
			buttons[(int)MouseButtons::Left]		= GetFlag(buttonState, FROM_LEFT_1ST_BUTTON_PRESSED);
			buttons[(int)MouseButtons::Right]		= GetFlag(buttonState, RIGHTMOST_BUTTON_PRESSED);
			buttons[(int)MouseButtons::Middle]		= GetFlag(buttonState, FROM_LEFT_2ND_BUTTON_PRESSED);
			buttons[(int)MouseButtons::XButton1]	= GetFlag(buttonState, FROM_LEFT_3RD_BUTTON_PRESSED);
			buttons[(int)MouseButtons::XButton2]	= GetFlag(buttonState, FROM_LEFT_4TH_BUTTON_PRESSED);
			doubleClick = inRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK;
		}
		else if (inRec.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED) {
			_members.MouseWheelDelta.Y = (signed short)((inRec.Event.MouseEvent.dwButtonState & 0xFFFF0000) >> 16);
		}
		else if(inRec.Event.MouseEvent.dwEventFlags == MOUSE_HWHEELED) {
			_members.MouseWheelDelta.X = (signed short)((inRec.Event.MouseEvent.dwButtonState & 0xFFFF0000) >> 16);
		}
	}

	// Update the button states
	for (int i = 0; i < NumButtons; i++) {
		_members.ButtonStates[i].Update(buttons[i] && active, false, buttons[i] && doubleClick && active, false);
	}

	_members.MouseInside = _members.MouseCoord >= Coord(0, 0) && _members.MouseCoord < consoleSize;
}

#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

void PowerConsole::Input::Mouse::Reset(bool release) {
	// Reset each of the keys
	for (int i = 0; i < NumButtons; i++) {
		_members.ButtonStates[i].Reset(release);
	}
	
	// Reset the the mouse wheel state
	_members.MouseWheelDelta = Coord(0, 0);
}
void PowerConsole::Input::Mouse::Enable() {
	// Reset each of the keys
	for (int i = 0; i < NumButtons; i++) {
		_members.ButtonStates[i].Enable();
	}

	_members.Disabled = false;
}
void PowerConsole::Input::Mouse::Disable(bool untilRelease) {
	// Reset each of the keys
	for (int i = 0; i < NumButtons; i++) {
		_members.ButtonStates[i].Disable(untilRelease);
	}
	
	// Reset the the mouse wheel state
	_members.MouseWheelDelta = Coord(0, 0);

	if (!untilRelease)
		_members.Disabled = true;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events
//--------------------------------
#pragma region Button State

bool PowerConsole::Input::Mouse::IsButtonDoubleClicked(MouseButtons buttonCode) {
	return _members.ButtonStates[(int)buttonCode].IsDoubleClicked();
}
bool PowerConsole::Input::Mouse::IsButtonDoubleClicked(int buttonCode) {
	return _members.ButtonStates[buttonCode].IsDoubleClicked();
}
bool PowerConsole::Input::Mouse::IsButtonPressed(MouseButtons buttonCode) {
	return _members.ButtonStates[(int)buttonCode].IsPressed();
}
bool PowerConsole::Input::Mouse::IsButtonPressed(int buttonCode) {
	return _members.ButtonStates[buttonCode].IsPressed();
}
bool PowerConsole::Input::Mouse::IsButtonDown(MouseButtons buttonCode) {
	return _members.ButtonStates[(int)buttonCode].IsDown();
}
bool PowerConsole::Input::Mouse::IsButtonDown(int buttonCode) {
	return _members.ButtonStates[buttonCode].IsDown();
}
bool PowerConsole::Input::Mouse::IsButtonReleased(MouseButtons buttonCode) {
	return _members.ButtonStates[(int)buttonCode].IsReleased();
}
bool PowerConsole::Input::Mouse::IsButtonReleased(int buttonCode) {
	return _members.ButtonStates[buttonCode].IsReleased();
}
bool PowerConsole::Input::Mouse::IsButtonUp(MouseButtons buttonCode) {
	return _members.ButtonStates[(int)buttonCode].IsUp();
}
bool PowerConsole::Input::Mouse::IsButtonUp(int buttonCode) {
	return _members.ButtonStates[buttonCode].IsUp();
}

#pragma endregion
//--------------------------------
#pragma region Any Button State

bool PowerConsole::Input::Mouse::IsAnyButtonDoubleClicked() {
	for (int i = 0; i < NumButtons; i++) {
		if (_members.ButtonStates[i].IsDoubleClicked() && i >= 0x07)
			return true;
	}
	return false;
}
bool PowerConsole::Input::Mouse::IsAnyButtonPressed() {
	for (int i = 0; i < NumButtons; i++) {
		if (_members.ButtonStates[i].IsPressed() && i >= 0x07)
			return true;
	}
	return false;
}
bool PowerConsole::Input::Mouse::IsAnyButtonDown() {
	for (int i = 0; i < NumButtons; i++) {
		if (_members.ButtonStates[i].IsDown() && i >= 0x07)
			return true;
	}
	return false;
}
bool PowerConsole::Input::Mouse::IsAnyButtonReleased() {
	for (int i = 0; i < NumButtons; i++) {
		if (_members.ButtonStates[i].IsReleased() && i >= 0x07)
			return true;
	}
	return false;
}
bool PowerConsole::Input::Mouse::IsAnyButtonUp() {
	for (int i = 0; i < NumButtons; i++) {
		if (_members.ButtonStates[i].IsUp() && i >= 0x07)
			return true;
	}
	return false;
}

#pragma endregion
//--------------------------------
#pragma region Mouse Position

Coord PowerConsole::Input::Mouse::GetPosition() {
	return _members.MouseCoord;
}
bool PowerConsole::Input::Mouse::IsMouseInsideWindow() {
	return _members.MouseInside;
}

#pragma endregion
//--------------------------------
#pragma region Scroll Wheel

bool PowerConsole::Input::Mouse::IsWheelScrolledUp() {
	return _members.MouseWheelDelta.Y > 0;
}
bool PowerConsole::Input::Mouse::IsWheelScrolledDown() {
	return _members.MouseWheelDelta.Y < 0;
}
bool PowerConsole::Input::Mouse::IsWheelScrolledLeft() {
	return _members.MouseWheelDelta.X > 0;
}
bool PowerConsole::Input::Mouse::IsWheelScrolledRight() {
	return _members.MouseWheelDelta.X < 0;
}
Coord PowerConsole::Input::Mouse::GetWheelDelta() {
	return _members.MouseWheelDelta;
}

#pragma endregion
//--------------------------------
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|
