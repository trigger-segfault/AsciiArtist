/*=================================================================|
|* File:				Mouse.cpp								  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Definition of the Mouse listener class.			  *|
|=================================================================*/

#include "stdafx.h"
#include "Mouse.h"
#include <PowerConsole/Input/Keys.h>

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
#pragma region Mouse
//========= CONSTRUCTORS =========
#pragma region Constructors

Mouse::Mouse()
 :	mouseGlobal(Point2I::ZERO),
	mouseGlobalLast( Point2I::ZERO),
	mouseClient(Point2I::ZERO),
	mouseClientLast(Point2I::ZERO),
	mouseConsole(Point2I::ZERO),
	mouseConsoleLast(Point2I::ZERO),
	mouseWheelDelta(Point2I::ZERO),
	disabled(false),
	disableNextTick(false),
	mouseInside(false) {
	
	for (int i = 0; i < NUM_BUTTONS; i++) {
		buttonStates[i] = InputHandler();
	}
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void Mouse::update(INPUT_RECORD& inRec, bool active, Point2I clientWindowPosition, Point2I consoleSize, Point2I bufferPosition, Point2I fontSize) {
	
	// Get the mouse position
	mouseConsoleLast	= mouseConsole;
	mouseClientLast	= mouseClient;
	mouseGlobalLast	= mouseGlobal;

	mouseWheelDelta	= Point2I::ZERO;

	POINT ptMouseGlobal;
	if (active && GetCursorPos(&ptMouseGlobal) != 0) {
		mouseGlobal	= ptMouseGlobal;
		mouseClient	= mouseGlobal - clientWindowPosition;
		mouseConsole = mouseClient / fontSize + bufferPosition;
	}

	// Get any button events
	bool buttons[NUM_BUTTONS];
	buttons[(int)MouseButtons::None]		= false;
	buttons[(int)MouseButtons::Left]		= GetAsyncKeyState((int)Keys::LButton) != 0;
	buttons[(int)MouseButtons::Right]		= GetAsyncKeyState((int)Keys::RButton) != 0;
	buttons[(int)MouseButtons::Middle]		= GetAsyncKeyState((int)Keys::MButton) != 0;
	buttons[(int)MouseButtons::XButton1]	= GetAsyncKeyState((int)Keys::XButton1) != 0;
	buttons[(int)MouseButtons::XButton2]	= GetAsyncKeyState((int)Keys::XButton2) != 0;
	bool doubleClick = false;

	if (inRec.EventType == MOUSE_EVENT && active) {
		if (inRec.Event.MouseEvent.dwEventFlags == 0x0000 || inRec.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK) {
			unsigned long buttonState = inRec.Event.MouseEvent.dwButtonState;
			
			buttons[(int)MouseButtons::Left]		= GetFlag(buttonState, FROM_LEFT_1ST_BUTTON_PRESSED);
			buttons[(int)MouseButtons::Right]		= GetFlag(buttonState, RIGHTMOST_BUTTON_PRESSED);
			buttons[(int)MouseButtons::Middle]		= GetFlag(buttonState, FROM_LEFT_2ND_BUTTON_PRESSED);
			buttons[(int)MouseButtons::XButton1]	= GetFlag(buttonState, FROM_LEFT_3RD_BUTTON_PRESSED);
			buttons[(int)MouseButtons::XButton2]	= GetFlag(buttonState, FROM_LEFT_4TH_BUTTON_PRESSED);
			
			doubleClick = inRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK;
		}
		else if (inRec.Event.MouseEvent.dwEventFlags & MOUSE_WHEELED) {
			mouseWheelDelta.y = (signed short)((inRec.Event.MouseEvent.dwButtonState & 0xFFFF0000) >> 16);
		}
		else if(inRec.Event.MouseEvent.dwEventFlags & MOUSE_HWHEELED) {
			mouseWheelDelta.x = (signed short)((inRec.Event.MouseEvent.dwButtonState & 0xFFFF0000) >> 16);
		}
	}

	// Update the button states
	for (int i = 0; i < NUM_BUTTONS; i++) {
		buttonStates[i].update(buttons[i] && active, false, buttons[i] && doubleClick && active, false);
	}

	if (disableNextTick) {
		disableNextTick = false;
		for (int i = 0; i < NUM_BUTTONS; i++) {
			buttonStates[i].disable(true);
		}
	}

	mouseInside = (mouseConsole >= Point2I::ZERO && mouseConsole < consoleSize);
}

#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

void Mouse::reset(bool release) {
	// Reset each of the keys
	for (int i = 0; i < NUM_BUTTONS; i++) {
		buttonStates[i].reset(release);
	}
	
	// Reset the the mouse wheel state
	mouseWheelDelta = Point2I::ZERO;
}
void Mouse::enable() {
	// Reset each of the keys
	for (int i = 0; i < NUM_BUTTONS; i++) {
		buttonStates[i].enable();
	}

	disabled = false;
}
void Mouse::disable(bool releaseNextTick) {
	if (!releaseNextTick) {
		// Reset each of the mouse buttons
		for (int i = 0; i < NUM_BUTTONS; i++) {
			buttonStates[i].disable(false);
		}

		// Reset the the mouse wheel state
		mouseWheelDelta = Point2I::ZERO;

		disabled = true;
	}
	else {
		disableNextTick = true;
	}
}

#pragma endregion
//============ EVENTS ============
#pragma region Events
//--------------------------------
#pragma region Button State

bool Mouse::isButtonDoubleClicked(MouseButtons buttonCode) const {
	return buttonStates[(int)buttonCode].isDoubleClicked();
}
bool Mouse::isButtonDoubleClicked(int buttonCode) const {
	return buttonStates[buttonCode].isDoubleClicked();
}
bool Mouse::isButtonPressed(MouseButtons buttonCode) const {
	return buttonStates[(int)buttonCode].isPressed();
}
bool Mouse::isButtonPressed(int buttonCode) const {
	return buttonStates[buttonCode].isPressed();
}
bool Mouse::isButtonDown(MouseButtons buttonCode) const {
	return buttonStates[(int)buttonCode].isDown();
}
bool Mouse::isButtonDown(int buttonCode) const {
	return buttonStates[buttonCode].isDown();
}
bool Mouse::isButtonReleased(MouseButtons buttonCode) const {
	return buttonStates[(int)buttonCode].isReleased();
}
bool Mouse::isButtonReleased(int buttonCode) const {
	return buttonStates[buttonCode].isReleased();
}
bool Mouse::isButtonUp(MouseButtons buttonCode) const {
	return buttonStates[(int)buttonCode].isUp();
}
bool Mouse::isButtonUp(int buttonCode) const {
	return buttonStates[buttonCode].isUp();
}

bool Mouse::hasButtonStateChanged(MouseButtons buttonCode) const {
	return buttonStates[(int)buttonCode].isPressed() || buttonStates[(int)buttonCode].isReleased();
}
bool Mouse::hasButtonStateChanged(int buttonCode) const {
	return buttonStates[buttonCode].isPressed() || buttonStates[buttonCode].isReleased();
}
InputStates Mouse::getButtonState(MouseButtons buttonCode) const {
	return buttonStates[(int)buttonCode].getState();
}
InputStates Mouse::getButtonState(int buttonCode) const {
	return buttonStates[buttonCode].getState();
}

#pragma endregion
//--------------------------------
#pragma region Any Button State

bool Mouse::isAnyButtonDoubleClicked() const {
	for (int i = 1; i < NUM_BUTTONS; i++) {
		if (buttonStates[i].isDoubleClicked() && i >= 0x07)
			return true;
	}
	return false;
}
bool Mouse::isAnyButtonPressed() const {
	for (int i = 1; i < NUM_BUTTONS; i++) {
		if (buttonStates[i].isPressed() && i >= 0x07)
			return true;
	}
	return false;
}
bool Mouse::isAnyButtonDown() const {
	for (int i = 1; i < NUM_BUTTONS; i++) {
		if (buttonStates[i].isDown() && i >= 0x07)
			return true;
	}
	return false;
}
bool Mouse::isAnyButtonReleased() const {
	for (int i = 1; i < NUM_BUTTONS; i++) {
		if (buttonStates[i].isReleased() && i >= 0x07)
			return true;
	}
	return false;
}
bool Mouse::isAnyButtonUp() const {
	for (int i = 1; i < NUM_BUTTONS; i++) {
		if (buttonStates[i].isUp() && i >= 0x07)
			return true;
	}
	return false;
}

#pragma endregion
//--------------------------------
#pragma region Mouse Position

Point2I Mouse::getPosition() const {
	return mouseConsole;
}
Point2I Mouse::getLastPosition() const {
	return mouseConsoleLast;
}
Point2F Mouse::getClientPosition() const {
	return mouseClient;
}
Point2F Mouse::getLastClientPosition() const {
	return mouseClientLast;
}
bool Mouse::hasMoved() const {
	return mouseConsole != mouseConsoleLast;
}
bool Mouse::isMouseInsideWindow() const {
	return mouseInside;
}

#pragma endregion
//--------------------------------
#pragma region Scroll Wheel

bool Mouse::isWheelScrolledUp() const {
	return mouseWheelDelta.y > 0;
}
bool Mouse::isWheelScrolledDown() const {
	return mouseWheelDelta.y < 0;
}
bool Mouse::isWheelScrolledLeft() const {
	return mouseWheelDelta.x > 0;
}
bool Mouse::isWheelScrolledRight() const {
	return mouseWheelDelta.x < 0;
}
bool Mouse::isWheelScrolled(MouseWheels wheelCode) const {
	if (wheelCode == MouseWheels::Horizontal)
		return mouseWheelDelta.x != 0;
	else if (wheelCode == MouseWheels::Vertical)
		return mouseWheelDelta.y != 0;
	return false;
}
bool Mouse::isWheelScrolled(int wheelCode) const {
	if ((MouseWheels)wheelCode == MouseWheels::Horizontal)
		return mouseWheelDelta.x != 0;
	else if ((MouseWheels)wheelCode == MouseWheels::Vertical)
		return mouseWheelDelta.y != 0;
	return false;
}
int Mouse::getWheelDelta(MouseWheels wheelCode) const {
	if (wheelCode == MouseWheels::Horizontal)
		return mouseWheelDelta.x;
	else if (wheelCode == MouseWheels::Vertical)
		return mouseWheelDelta.y;
	return 0;
}
int Mouse::getWheelDelta(int wheelCode) const {
	if ((MouseWheels)wheelCode == MouseWheels::Horizontal)
		return mouseWheelDelta.x;
	else if ((MouseWheels)wheelCode == MouseWheels::Vertical)
		return mouseWheelDelta.y;
	return 0;
}
Point2I Mouse::getWheelDelta() const {
	return mouseWheelDelta;
}

#pragma endregion
//--------------------------------
#pragma endregion
//================================
#pragma endregion
//=================================================================|
