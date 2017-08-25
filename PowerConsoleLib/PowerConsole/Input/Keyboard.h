/*=================================================================|
|* File:				Keyboard.h								  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Decleration of the Keyboard listener class.		  *|
|=================================================================*/

#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#define NOMINMAX
#include <windows.h>
#include <array>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Input/Hotkey.h>
#include <PowerConsole/Input/InputHandler.h>

namespace PowerConsole::Input {
//=================================================================|
// PREDEFINITIONS												   |
//=================================================================/
DECLARE_CLASS_AND_SMART_PTRS(Keyboard);

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The class for recording keyboard input events. */
class Keyboard {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The number of keys in the array. */
	static const unsigned int NUM_KEYS = 256;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The current character that was typed. */
	char charTyped;
	/* The current key that was typed. */
	Keys keyTyped;
	/* The list of key states. */
	std::array<InputHandler, NUM_KEYS> keyStates;
	/* True if the keyboard is disabled. */
	bool disabled;
	/* True if the keyboard has been disabled until release and will release inputs next tick. */
	bool disableNextTick;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs and sets up the keyboard. */
	Keyboard();
	
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Updates the keyboard state. */
	void update(INPUT_RECORD& inRec, bool active);
	
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management
	
	/* Resets all the key states. */
	void reset(bool release);
	/* Enables all the keys. */
	void enable();
	/* Disables all the keys. */
	void disable(bool releaseNextTick);
	
	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
	//--------------------------------
	#pragma region Key State

	/* Returns true if the specified key was pressed. */
	bool isKeyPressed(Keys keyCode) const;
	/* Returns true if the specified key was pressed. */
	bool isKeyPressed(int keyCode) const;
	/* Returns true if the specified key is down. */
	bool isKeyDown(Keys keyCode) const;
	/* Returns true if the specified key is down. */
	bool isKeyDown(int keyCode) const;
	/* Returns true if the specified key was released. */
	bool isKeyReleased(Keys keyCode) const;
	/* Returns true if the specified key was released. */
	bool isKeyReleased(int keyCode) const;
	/* Returns true if the specified key is up. */
	bool isKeyUp(Keys keyCode) const;
	/* Returns true if the specified key is up. */
	bool isKeyUp(int keyCode) const;

	/* Returns true if the state of the specified key has changed states. */
	bool hasKeyStateChanged(Keys keyCode) const;
	/* Returns true if the state of the specified key has changed states. */
	bool hasKeyStateChanged(int keyCode) const;
	/* Gets the state of the specified key. */
	InputStates getKeyState(Keys keyCode) const;
	/* Gets the state of the specified key. */
	InputStates getKeyState(int keyCode) const;

	#pragma endregion
	//--------------------------------
	#pragma region Any Key State

	/* Returns true if any key was pressed. */
	bool isAnyKeyPressed() const;
	/* Returns true if any key is down. */
	bool isAnyKeyDown() const;
	/* Returns true if any key was released. */
	bool isAnyKeyReleased() const;
	/* Returns true if any key is up. */
	bool isAnyKeyUp() const;
	
	#pragma endregion
	//--------------------------------
	#pragma region Hotkey State

	/* Gets the modifiers that are down. */
	Modifiers getModifiers() const;
	/* Returns true if the specified modifiers are down. */
	bool isModifiersDown(Modifiers modifiers) const;
	/* Returns true if the specified hotkey was pressed. */
	bool isHotkeyPressed(Hotkey hotkey) const;
	/* Returns true if the specified hotkey is down. */
	bool isHotkeyDown(Hotkey hotkey) const;

	#pragma endregion
	//--------------------------------
	#pragma region Typing

	/* Returns true if the specified key was typed. */
	bool isKeyTyped(Keys keyCode) const;
	/* Returns true if the specified key was typed. */
	bool isKeyTyped(int keyCode) const;
	/* Returns true if any key was typed. */
	bool isAnyKeyTyped() const;
	/* Gets the key that was typed. */
	Keys getKeyTyped() const;
	/* Returns true if a character was typed. */
	bool isCharTyped() const;
	/* Gets the character that was typed. */
	char getCharTyped() const;
	
	#pragma endregion
	//--------------------------------
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
