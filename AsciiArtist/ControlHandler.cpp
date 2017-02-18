/*=================================================================|
|* File:				ControlHandler.cpp						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Definition of the control handler class.		  *|
|=================================================================*/

#include "Stdafx.h"
#include "ControlHandler.h"

using namespace std;
using namespace PowerConsole::Input;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ControlHandler
//========= CONSTRUCTORS =========
#pragma region ControlHandler::Constructors

PowerConsole::Input::ControlHandler::ControlHandler() : _state(ControlState::Up),
	_disabledState(DisableState::Enabled), _doubleClicked(false), _clicked(false),
	_typed(false), _holdTime(0) {}

#pragma endregion
//=========== UPDATING ===========
#pragma region ControlHandler::Updating

void PowerConsole::Input::ControlHandler::Update(bool down, bool typed, bool doubleClicked, bool clicked) {
	// Update the duration of the control state
	_holdTime++;

	// Update the control state based on its down state
	if (down) {
		if (_disabledState == DisableState::Enabled) {
			if (_state == ControlState::Pressed)
				_state = ControlState::Down;
			if (_state == ControlState::Released || _state == ControlState::Up) {
				_state = ControlState::Pressed;
				_holdTime = 0;
			}
		}
	}
	else {
		if (_disabledState == DisableState::Enabled) {
			if (_state == ControlState::Released)
				_state = ControlState::Up;
			if (_state == ControlState::Pressed || _state == ControlState::Down) {
				_state = ControlState::Released;
				_holdTime = 0;
			}
		}
		else if (_disabledState == DisableState::DisabledUntilRelease) {
			_disabledState	= DisableState::Enabled;
			_state			= ControlState::Up;
		}
	}

	// Update the double pressed and typed state
	if (_disabledState == DisableState::Enabled) {
		_doubleClicked	= doubleClicked;
		_clicked			= clicked;
		_typed			= typed;
	}
}

#pragma endregion
//========== MANAGEMENT ==========
#pragma region ControlHandler::Management

void PowerConsole::Input::ControlHandler::Reset(bool release) {
	if ((_state == ControlState::Pressed || _state == ControlState::Down) && release)
		_state = ControlState::Released;
	else
		_state = ControlState::Up;
	_doubleClicked	= false;
	_clicked		= false;
	_typed			= false;
	_holdTime		= 0;
}
void PowerConsole::Input::ControlHandler::Enable() {
	_disabledState = DisableState::Enabled;
}
void PowerConsole::Input::ControlHandler::Disable(bool untilRelease) {
	if (untilRelease) {
		if (_state != ControlState::Up && _state != ControlState::Released)
			_disabledState = DisableState::DisabledUntilRelease;
		else
			_state = ControlState::Up;
	}
	else {
		_state			= ControlState::Up;
		_disabledState	= DisableState::Disabled;
	}
	_doubleClicked	= false;
	_clicked		= false;
	_typed			= false;
	_holdTime		= 0;
}

#pragma endregion
//============ EVENTS ============
#pragma region ControlHandler::Events

bool PowerConsole::Input::ControlHandler::IsDoubleClicked() {
	return (_doubleClicked && _disabledState == DisableState::Enabled);
}
bool PowerConsole::Input::ControlHandler::IsClicked() {
	return ((_doubleClicked || _clicked) && _disabledState == DisableState::Enabled);
}
bool PowerConsole::Input::ControlHandler::IsPressed() {
	return (_state == ControlState::Pressed && _disabledState == DisableState::Enabled);
}
bool PowerConsole::Input::ControlHandler::IsDown() {
	return ((_state == ControlState::Pressed || _state == ControlState::Down) &&
			_disabledState == DisableState::Enabled);
}
bool PowerConsole::Input::ControlHandler::IsReleased() {
	return (_state == ControlState::Released && _disabledState == DisableState::Enabled);
}
bool PowerConsole::Input::ControlHandler::IsUp() {
	return (_state == ControlState::Released || _state == ControlState::Up ||
			_disabledState != DisableState::Enabled);
}
bool PowerConsole::Input::ControlHandler::IsTyped() {
	return (_typed && _disabledState == DisableState::Enabled);
}
bool PowerConsole::Input::ControlHandler::IsDisabled() {
	return (_disabledState != DisableState::Enabled);
}
int PowerConsole::Input::ControlHandler::GetHoldTime() {
	return _holdTime;
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|
