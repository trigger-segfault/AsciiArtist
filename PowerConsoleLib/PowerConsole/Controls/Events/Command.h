/*=================================================================|
|* File:				Command.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include "stdafx.h"
#include <PowerConsole/Input/Hotkey.h>
#include <PowerConsole/Controls/Events/EventArgs.h>

namespace PowerConsole::Controls {
DECLARE_CLASS(Command);
//=================================================================|
// CLASSES														   |
//=================================================================/

/* An action to execute and bind to a hotkey. */
class Command : public Event<EventArgs> {

	//=========== MEMBERS ============
	#pragma region Members

	/* The unique name of the command. */
	std::string name;
	/* The keybind for the command. */
	PcInput::Hotkey hotkey;
	/* True if the control requires focus in order to execute the command. */
	bool requiresFocus;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs an empty command. */
	Command();
	/* Constructs a command with the specified settings. */
	Command(const std::string& name, PcInput::Hotkey hotkey, bool requiresFocus);
	/* Constructs a command with the specified settings. */
	Command(const std::string& name, PcInput::Hotkey hotkey, bool requiresFocus, const std::string& functionName, ArgsFunction function);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the unique name of the command. */
	std::string getName() const;

	/* Returns true if the command has a hotkey. */
	bool hasHotkey() const;
	/* Gets the hotkey for the command. */
	PcInput::Hotkey getHotkey() const;
	/* Sets the hotkey for the command. */
	void setHotkey(PcInput::Hotkey hotkey);

	/* Gets if the control requires focus in order to execute the command. */
	bool isFocusRequired() const;
	/* Sets if the control requires focus in order to execute the command. */
	void setFocusRequired(bool requiresFocus);

	#pragma endregion
};

/* Shows addition or removal of a command in a collection. */
struct CommandModifiedEventArgs : public EventArgs {

	/* The command that was added or removed. */
	Command command;

	inline CommandModifiedEventArgs()
		: EventArgs() {}//, command() {}
	inline CommandModifiedEventArgs(Command command)
		: EventArgs(), command(command) {}
};

//=================================================================|
} /* Trigger::PowerConsole::Controls */
#endif /* COMMAND_H */
//=================================================================|
