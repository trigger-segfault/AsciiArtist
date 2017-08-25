/*=================================================================|
|* File:				InputHandler.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "InputHandler.h"

using namespace PowerConsole::Input;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region InputHandler
//========= CONSTRUCTORS =========
#pragma region InputHandler::Constructors

InputHandler::InputHandler() : state(InputStates::Up),
	disabledState(DisableStates::DisabledUntilRelease), doubleClicked(false), clicked(false),
	typed(false), holdTime(0) {}

#pragma endregion
//=========== UPDATING ===========
#pragma region InputHandler::Updating

void InputHandler::update(bool down, bool typed, bool doubleClicked, bool clicked) {
	// Update the duration of the control state
	holdTime++;

	// Update the control state based on its down state
	if (down) {
		if (disabledState == DisableStates::Enabled) {
			if (state == InputStates::Pressed)
				state = InputStates::Down;
			if (state == InputStates::Released || state == InputStates::Up) {
				state = InputStates::Pressed;
				holdTime = 0;
			}
		}
		else if (state == InputStates::Released) {
			state = InputStates::Up;
		}
	}
	else {
		if (disabledState == DisableStates::Enabled) {
			if (state == InputStates::Released)
				state = InputStates::Up;
			if (state == InputStates::Pressed || state == InputStates::Down) {
				state = InputStates::Released;
				holdTime = 0;
			}
		}
		else if (disabledState == DisableStates::DisabledUntilRelease) {
			disabledState	= DisableStates::Enabled;
			state			= InputStates::Up;
		}
	}

	// Update the double pressed and typed state
	if (disabledState == DisableStates::Enabled) {
		this->doubleClicked	= doubleClicked;
		this->clicked		= clicked;
		this->typed			= typed;
	}
}

#pragma endregion
//========== MANAGEMENT ==========
#pragma region InputHandler::Management

void InputHandler::reset(bool release) {
	if ((state == InputStates::Pressed || state == InputStates::Down) && release)
		state = InputStates::Released;
	else
		state = InputStates::Up;
	doubleClicked	= false;
	clicked			= false;
	typed			= false;
	holdTime		= 0;
}
void InputHandler::enable() {
	disabledState = DisableStates::Enabled;
}
void InputHandler::disable(bool untilRelease) {
	if (untilRelease) {
		if (state == InputStates::Down || state == InputStates::Pressed) {
			state = InputStates::Released;
			disabledState = DisableStates::DisabledUntilRelease;
		}
	}
	else {
		if (state == InputStates::Down || state == InputStates::Pressed)
			state = InputStates::Released;
		else
			state = InputStates::Up;
		disabledState = DisableStates::Disabled;
	}
	doubleClicked	= false;
	clicked			= false;
	typed			= false;
	holdTime		= 0;
}

#pragma endregion
//============ EVENTS ============
#pragma region InputHandler::Events

bool InputHandler::isDoubleClicked() const {
	return (doubleClicked && disabledState == DisableStates::Enabled);
}
bool InputHandler::isClicked() const {
	return ((doubleClicked || clicked) && disabledState == DisableStates::Enabled);
}
bool InputHandler::isPressed() const {
	return (state == InputStates::Pressed && disabledState == DisableStates::Enabled);
}
bool InputHandler::isDown() const {
	return ((state == InputStates::Pressed || state == InputStates::Down) &&
			disabledState == DisableStates::Enabled);
}
bool InputHandler::isReleased() const {
	return (state == InputStates::Released);
}
bool InputHandler::isUp() const {
	return (state == InputStates::Released || state == InputStates::Up);
}
bool InputHandler::isTyped() const {
	return (typed && disabledState == DisableStates::Enabled);
}
bool InputHandler::isDisabled() const {
	return (disabledState != DisableStates::Enabled);
}
int InputHandler::getHoldTime() const {
	return holdTime;
}
InputStates InputHandler::getState() const {
	return state;
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
