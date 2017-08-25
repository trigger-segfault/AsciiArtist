/*=================================================================|
|* File:				Hotkey.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef HOTKEY_H
#define HOTKEY_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Input/Keys.h>

namespace PowerConsole::Input {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A structure for containing a command hotkey. */
struct Hotkey {

	//=========== MEMBERS ============
	#pragma region Members

	/* Key that needs to be pressed. */
	Keys key;
	/* The modifier keys that need to be down. */
	Modifiers modifiers;
	/* True if the hotkey can be repeated by holding the key down. */
	bool repeatable;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs an empty hotkey. */
	Hotkey();
	/* Constructs the specified hotkey. */
	Hotkey(Keys key, bool repeatable);
	/* Constructs the specified hotkey. */
	Hotkey(Keys key, Modifiers modifiers = Modifiers::None, bool repeatable = false);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Returns true if the hotkeys are the same. */
	bool operator==(Hotkey hotkey) const;
	/* Returns true if the hotkeys are not the same. */
	bool operator!=(Hotkey hotkey) const;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Returns true if the hotkey has any modifiers. */
	bool hasModifiers() const;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
