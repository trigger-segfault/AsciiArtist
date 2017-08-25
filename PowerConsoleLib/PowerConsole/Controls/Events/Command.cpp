/*=================================================================|
|* File:				Command.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Command.h"

using namespace PowerConsole::Controls;
using namespace PowerConsole::Input;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Command::Command()
	: Event<EventArgs>(EventDirections::FirstInLastCalled), name(""), hotkey(), requiresFocus(false) {}
Command::Command(const std::string& name, Hotkey hotkey, bool requiresFocus)
	: Event<EventArgs>(EventDirections::FirstInLastCalled), name(name), hotkey(hotkey), requiresFocus(requiresFocus) {}
Command::Command(const std::string& name, Hotkey hotkey, bool requiresFocus, const std::string& functionName, ArgsFunction function)
	: Event<EventArgs>(EventDirections::FirstInLastCalled), name(name), hotkey(hotkey), requiresFocus(requiresFocus) {
	add(functionName, function);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string Command::getName() const {
	return name;
}

bool Command::hasHotkey() const {
	return hotkey.key != Keys::None;
}
Hotkey Command::getHotkey() const {
	return hotkey;
}
void Command::setHotkey(Hotkey hotkey) {
	this->hotkey = hotkey;
}

bool Command::isFocusRequired() const {
	return requiresFocus;
}
void Command::setFocusRequired(bool requiresFocus) {
	this->requiresFocus = requiresFocus;
}

#pragma endregion
//=================================================================|
