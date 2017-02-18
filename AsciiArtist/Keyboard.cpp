/*=================================================================|
|* File:				Keyboard.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Definition of the Keyboard listener class.		  *|
|=================================================================*/

#include "Stdafx.h"
#include "Keyboard.h"

using namespace std;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region Keyboard
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Input::Keyboard::Keyboard() {
	_members.CharTyped	= '\0';
	_members.KeyStates	= new ControlHandler[NumKeys];
	_members.Disabled	= false;

	for (int i = 0; i < NumKeys; i++) {
		_members.KeyStates[i] = ControlHandler();
	}
}
PowerConsole::Input::Keyboard::~Keyboard() {
	delete[] _members.KeyStates;
	_members.KeyStates = nullptr;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Input::Keyboard::Update(INPUT_RECORD& inRec, bool active) {
	
	// Reset the typed character
	_members.CharTyped	= '\0';
	int keyTyped		= 0;

	// Get any typed events
	if (inRec.EventType == KEY_EVENT) {
		if (inRec.Event.KeyEvent.bKeyDown != 0) {
			keyTyped		= (int)inRec.Event.KeyEvent.wVirtualKeyCode;
			_members.CharTyped	= (char)inRec.Event.KeyEvent.uChar.AsciiChar;
		}
	}
	if (!active) {
		keyTyped = 0;
		_members.CharTyped	= '\0';
	}

	// Update the key states
	for (int i = 0; i < NumKeys; i++) {
		_members.KeyStates[i].Update(GetAsyncKeyState(i) != 0 && active, i == keyTyped && i != 0);
	}
}

#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

void PowerConsole::Input::Keyboard::Reset(bool release) {
	// Reset each of the keys
	for (int i = 0; i < NumKeys; i++) {
		_members.KeyStates[i].Reset(release);
	}
	
	// Reset the typed character
	_members.CharTyped		= '\0';
}
void PowerConsole::Input::Keyboard::Enable() {
	// Reset each of the keys
	for (int i = 0; i < NumKeys; i++) {
		_members.KeyStates[i].Enable();
	}

	_members.Disabled = false;
}
void PowerConsole::Input::Keyboard::Disable(bool untilRelease) {
	// Reset each of the keys
	for (int i = 0; i < NumKeys; i++) {
		_members.KeyStates[i].Disable(untilRelease);
	}

	// Reset the typed character
	_members.CharTyped		= '\0';

	if (!untilRelease)
		_members.Disabled = true;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events
//--------------------------------
#pragma region Key State

bool PowerConsole::Input::Keyboard::IsKeyPressed(Keys keyCode) {
	return _members.KeyStates[(int)keyCode].IsPressed();
}
bool PowerConsole::Input::Keyboard::IsKeyPressed(int keyCode) {
	return _members.KeyStates[keyCode].IsPressed();
}
bool PowerConsole::Input::Keyboard::IsKeyDown(Keys keyCode) {
	return _members.KeyStates[(int)keyCode].IsDown();
}
bool PowerConsole::Input::Keyboard::IsKeyDown(int keyCode) {
	return _members.KeyStates[keyCode].IsDown();
}
bool PowerConsole::Input::Keyboard::IsKeyReleased(Keys keyCode) {
	return _members.KeyStates[(int)keyCode].IsReleased();
}
bool PowerConsole::Input::Keyboard::IsKeyReleased(int keyCode) {
	return _members.KeyStates[keyCode].IsReleased();
}
bool PowerConsole::Input::Keyboard::IsKeyUp(Keys keyCode) {
	return _members.KeyStates[(int)keyCode].IsUp();
}
bool PowerConsole::Input::Keyboard::IsKeyUp(int keyCode) {
	return _members.KeyStates[keyCode].IsUp();
}

#pragma endregion
//--------------------------------
#pragma region Any Key State

bool PowerConsole::Input::Keyboard::IsAnyKeyPressed() {
	for (int i = 0; i < NumKeys; i++) {
		if (_members.KeyStates[i].IsPressed() && i >= 0x07)
			return true;
	}
	return false;
}
bool PowerConsole::Input::Keyboard::IsAnyKeyDown() {
	for (int i = 0; i < NumKeys; i++) {
		if (_members.KeyStates[i].IsDown() && i >= 0x07)
			return true;
	}
	return false;
}
bool PowerConsole::Input::Keyboard::IsAnyKeyReleased() {
	for (int i = 0; i < NumKeys; i++) {
		if (_members.KeyStates[i].IsReleased() && i >= 0x07)
			return true;
	}
	return false;
}
bool PowerConsole::Input::Keyboard::IsAnyKeyUp() {
	for (int i = 0; i < NumKeys; i++) {
		if (_members.KeyStates[i].IsUp() && i >= 0x07)
			return true;
	}
	return false;
}

#pragma endregion
//--------------------------------
#pragma region Typing

bool PowerConsole::Input::Keyboard::IsKeyTyped(Keys keyCode) {
	return _members.KeyStates[(int)keyCode].IsTyped();
}
bool PowerConsole::Input::Keyboard::IsKeyTyped(int keyCode) {
	return _members.KeyStates[keyCode].IsTyped();
}
bool PowerConsole::Input::Keyboard::IsAnyKeyTyped() {
	for (int i = 0; i < NumKeys; i++) {
		if (_members.KeyStates[i].IsTyped() && i >= 0x07)
			return true;
	}
	return false;
}
bool PowerConsole::Input::Keyboard::IsCharTyped() {
	return _members.CharTyped != '\0';
}
char PowerConsole::Input::Keyboard::GetCharTyped() {
	return _members.CharTyped;
}

#pragma endregion
//--------------------------------
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|
