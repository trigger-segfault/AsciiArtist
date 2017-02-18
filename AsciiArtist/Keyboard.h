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

#include "Stdafx.h"
#include "Keys.h"
#include "MouseButtons.h"
#include "ControlHandler.h"
#include "Geometry.h"

using namespace std;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of event handling classes. */
namespace Input {

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
	static const unsigned int NumKeys	= 256;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The structure to store the data about the keyboard class . */
	struct _KeyboardMembers {

		/* The current character that was typed. */
		char CharTyped;
		/* The list of key states. */
		ControlHandler* KeyStates;
		/* True if the keyboard is disabled. */
		bool Disabled;
	};
	/* The collection of data in the keyboard class. */
	_KeyboardMembers _members;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Initializes and sets up the keyboard. */
	Keyboard();
	/* Uninitializes and cleans up the keyboard. */
	~Keyboard();
	
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Updates the keyboard state. */
	void Update(INPUT_RECORD& inRec, bool active);
	
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management
	
	/* Resets all the key states. */
	void Reset(bool release);
	/* Enables all the keys. */
	void Enable();
	/* Disables all the keys. */
	void Disable(bool untilRelease);
	
	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
	//--------------------------------
	#pragma region Key State

	/* Returns true if the specified key was pressed. */
	bool IsKeyPressed(Keys keyCode);
	/* Returns true if the specified key was pressed. */
	bool IsKeyPressed(int keyCode);
	/* Returns true if the specified key is down. */
	bool IsKeyDown(Keys keyCode);
	/* Returns true if the specified key is down. */
	bool IsKeyDown(int keyCode);
	/* Returns true if the specified key was released. */
	bool IsKeyReleased(Keys keyCode);
	/* Returns true if the specified key was released. */
	bool IsKeyReleased(int keyCode);
	/* Returns true if the specified key is up. */
	bool IsKeyUp(Keys keyCode);
	/* Returns true if the specified key is up. */
	bool IsKeyUp(int keyCode);

	#pragma endregion
	//--------------------------------
	#pragma region Any Key State

	/* Returns true if any key was pressed. */
	bool IsAnyKeyPressed();
	/* Returns true if any key is down. */
	bool IsAnyKeyDown();
	/* Returns true if any key was released. */
	bool IsAnyKeyReleased();
	/* Returns true if any key is up. */
	bool IsAnyKeyUp();
	
	#pragma endregion
	//--------------------------------
	#pragma region Typing

	/* Returns true if the specified key was typed. */
	bool IsKeyTyped(Keys keyCode);
	/* Returns true if the specified key was typed. */
	bool IsKeyTyped(int keyCode);
	/* Returns true if any key was typed. */
	bool IsAnyKeyTyped();
	/* Returns true if a character was typed. */
	bool IsCharTyped();
	/* Gets the character that was typed. */
	char GetCharTyped();
	
	#pragma endregion
	//--------------------------------
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace Input */
} /* namespace PowerConsole */
#endif /* KEYBOARD_H */
//=================================================================|
