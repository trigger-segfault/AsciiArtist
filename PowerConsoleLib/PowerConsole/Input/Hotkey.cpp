/*=================================================================|
|* File:				Hotkey.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Hotkey.h"

using namespace PowerConsole::Input;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Hotkey::Hotkey()
	: key(Keys::None), modifiers(Modifiers::None), repeatable(false) {}
Hotkey::Hotkey(Keys key, bool repeatable)
	: key(key), modifiers(Modifiers::None), repeatable(repeatable) {}
Hotkey::Hotkey(Keys key, Modifiers modifiers, bool repeatable)
	: key(key), modifiers(modifiers), repeatable(repeatable) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

bool Hotkey::operator==(Hotkey hotkey) const {
	return (key == hotkey.key && modifiers == hotkey.modifiers);
}
bool Hotkey::operator!=(Hotkey hotkey) const {
	return (key != hotkey.key || modifiers != hotkey.modifiers);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool Hotkey::hasModifiers() const {
	return (modifiers == Modifiers::None);
}

#pragma endregion
//=================================================================|
