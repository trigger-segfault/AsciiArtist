/*=================================================================|
|* File:				Keyboard.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Keyboard.h"
#include <string>
#include <PowerConsole/Helpers/Flags.h>

using namespace PowerConsole::Input;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Keyboard
//========= CONSTRUCTORS =========
#pragma region Constructors

Keyboard::Keyboard() 
 :	charTyped('\0'),
	keyTyped(Keys::None),
	disabled(false),
	disableNextTick(false) {

	for (int i = 0; i < NUM_KEYS; i++) {
		keyStates[i] = InputHandler();
	}
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void Keyboard::update(INPUT_RECORD& inRec, bool active) {
	// Reset the typed character
	charTyped = '\0';
	keyTyped = Keys::None;
	
	// Get any typed events
	if (inRec.EventType == KEY_EVENT) {
		if (inRec.Event.KeyEvent.bKeyDown == TRUE) {
			keyTyped = (Keys)inRec.Event.KeyEvent.wVirtualKeyCode;
			charTyped = (char)inRec.Event.KeyEvent.uChar.AsciiChar;
		}
	}
	if (!active) {
		keyTyped = Keys::None;
		charTyped = '\0';
	}

	// Update the key states
	for (int i = 0; i < NUM_KEYS; i++) {
		keyStates[i].update(GetAsyncKeyState(i) != 0 && active, (Keys)i == keyTyped && i != 0);
	}

	if (disableNextTick) {
		disableNextTick = false;
		for (int i = 0; i < NUM_KEYS; i++) {
			keyStates[i].disable(true);
		}
	}
}

#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

void Keyboard::reset(bool release) {
	// Reset each of the keys
	for (int i = 0; i < NUM_KEYS; i++) {
		keyStates[i].reset(release);
	}
	
	// Reset the typed character
	charTyped		= '\0';
}
void Keyboard::enable() {
	// Reset each of the keys
	for (int i = 0; i < NUM_KEYS; i++) {
		keyStates[i].enable();
	}

	disabled = false;
}
void Keyboard::disable(bool releaseNextTick) {
	if (!releaseNextTick) {
		// Reset each of the keys
		for (int i = 0; i < NUM_KEYS; i++) {
			keyStates[i].disable(false);
		}

		// Reset the typed character
		charTyped = '\0';
		keyTyped = Keys::None;

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
#pragma region Key State

bool Keyboard::isKeyPressed(Keys keyCode) const {
	return keyStates[(int)keyCode].isPressed();
}
bool Keyboard::isKeyPressed(int keyCode) const {
	return keyStates[keyCode].isPressed();
}
bool Keyboard::isKeyDown(Keys keyCode) const {
	return keyStates[(int)keyCode].isDown();
}
bool Keyboard::isKeyDown(int keyCode) const {
	return keyStates[keyCode].isDown();
}
bool Keyboard::isKeyReleased(Keys keyCode) const {
	return keyStates[(int)keyCode].isReleased();
}
bool Keyboard::isKeyReleased(int keyCode) const {
	return keyStates[keyCode].isReleased();
}
bool Keyboard::isKeyUp(Keys keyCode) const {
	return keyStates[(int)keyCode].isUp();
}
bool Keyboard::isKeyUp(int keyCode) const {
	return keyStates[keyCode].isUp();
}

bool Keyboard::hasKeyStateChanged(Keys keyCode) const {
	return keyStates[(int)keyCode].isPressed() || keyStates[(int)keyCode].isReleased();
}
bool Keyboard::hasKeyStateChanged(int keyCode) const {
	return keyStates[keyCode].isPressed() || keyStates[keyCode].isReleased();
}
InputStates Keyboard::getKeyState(Keys keyCode) const {
	return keyStates[(int)keyCode].getState();
}
InputStates Keyboard::getKeyState(int keyCode) const {
	return keyStates[keyCode].getState();
}

#pragma endregion
//--------------------------------
#pragma region Any Key State

bool Keyboard::isAnyKeyPressed() const {
	for (int i = 7; i < NUM_KEYS; i++) {
		if (keyStates[i].isPressed())
			return true;
	}
	return false;
}
bool Keyboard::isAnyKeyDown() const {
	for (int i = 7; i < NUM_KEYS; i++) {
		if (keyStates[i].isDown())
			return true;
	}
	return false;
}
bool Keyboard::isAnyKeyReleased() const {
	for (int i = 7; i < NUM_KEYS; i++) {
		if (keyStates[i].isReleased())
			return true;
	}
	return false;
}
bool Keyboard::isAnyKeyUp() const {
	for (int i = 7; i < NUM_KEYS; i++) {
		if (keyStates[i].isUp())
			return true;
	}
	return false;
}

#pragma endregion
//--------------------------------
#pragma region Hotkey State

Modifiers Keyboard::getModifiers() const {
	Modifiers modifiers = Modifiers::None;
	setFlag(modifiers, Modifiers::Shift, (isKeyDown(Keys::Shift) || isKeyDown(Keys::LShift) || isKeyDown(Keys::RShift)));
	setFlag(modifiers, Modifiers::Control, (isKeyDown(Keys::Control) || isKeyDown(Keys::LControl) || isKeyDown(Keys::RControl)));
	setFlag(modifiers, Modifiers::Alt, (isKeyDown(Keys::Alt) || isKeyDown(Keys::LAlt) || isKeyDown(Keys::RAlt)));
	setFlag(modifiers, Modifiers::Windows, (isKeyDown(Keys::LWindows) || isKeyDown(Keys::RWindows)));
	return modifiers;
}
bool Keyboard::isModifiersDown(Modifiers modifiers) const {
	if (getFlag(modifiers, Modifiers::Shift) == (!isKeyDown(Keys::Shift) && !isKeyDown(Keys::LShift) && !isKeyDown(Keys::RShift)))
		return false;
	if (getFlag(modifiers, Modifiers::Control) == (!isKeyDown(Keys::Control) && !isKeyDown(Keys::LControl) && !isKeyDown(Keys::RControl)))
		return false;
	if (getFlag(modifiers, Modifiers::Alt) == (!isKeyDown(Keys::Alt) && !isKeyDown(Keys::LAlt) && !isKeyDown(Keys::RAlt)))
		return false;
	if (getFlag(modifiers, Modifiers::Windows) == (!isKeyDown(Keys::LWindows) && !isKeyDown(Keys::RWindows)))
		return false;
	return true;
}
bool Keyboard::isHotkeyPressed(Hotkey hotkey) const {
	return (((isKeyPressed(hotkey.key) && !hotkey.repeatable) || (isKeyTyped(hotkey.key) && hotkey.repeatable)) && isModifiersDown(hotkey.modifiers));
}
bool Keyboard::isHotkeyDown(Hotkey hotkey) const {
	return (isKeyDown(hotkey.key) && isModifiersDown(hotkey.modifiers));
}

#pragma endregion
//--------------------------------
#pragma region Typing

bool Keyboard::isKeyTyped(Keys keyCode) const {
	return keyTyped == keyCode && keyCode != Keys::None;
}
bool Keyboard::isKeyTyped(int keyCode) const {
	return keyTyped == (Keys)keyCode && keyCode != 0;
}
bool Keyboard::isAnyKeyTyped() const {
	return keyTyped != Keys::None;
}
bool Keyboard::isCharTyped() const {
	return charTyped != '\0';
}
char Keyboard::getCharTyped() const {
	return charTyped;
}
Keys Keyboard::getKeyTyped() const {
	return keyTyped;
}


#pragma endregion
//--------------------------------
#pragma endregion
//================================
#pragma endregion
//=================================================================|
