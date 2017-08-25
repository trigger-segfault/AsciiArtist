/*=================================================================|
|* File:				CommandCollection.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef COMMAND_COLLECTION_H
#define COMMAND_COLLECTION_H

#include "stdafx.h"
#include <PowerConsole/Controls/Events/Command.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A collection of commands to check for execution. */
class CommandCollection {
	//=========== MEMBERS ============
	#pragma region Members

	// Container
	/* The owner for this collection. */
	ControlWPtr owner;
	/* The container for the commands. */
	std::vector<Command> commands;

	// Events
	/* Called when an command has been added to the collection. */
	Event<CommandModifiedEventArgs> _eventCommandAdded;
	/* Called when an command has been removed from the collection. */
	Event<CommandModifiedEventArgs> _eventCommandRemoved;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constucts the item collection. */
	CommandCollection();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Sets the owner of the item collection. This should only ever be called by the owner. */
	void setOwner(ControlSPtr owner);

	/* Gets the number of commands in the collection. */
	int count() const;
	/* Gets if the collection has no commands in it. */
	bool isEmpty() const;

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Gets the command with the specified name. */
	Command& operator[](const std::string& name);
	/* Gets the command with the specified name. */
	const Command& operator[](const std::string& name) const;

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the command with the specified name. */
	Command& at(const std::string& name);
	/* Gets the command with the specified name. */
	const Command& at(const std::string& name) const;
	/* Adds the specified command to the collection. */
	void add(const Command& command);
	/* Adds the specified command to the collection. */
	void add(Command&& command);
	/* Removes the command with the specified name from the collection. */
	void remove(const std::string& name);
	/* Removes all commands from the collection. */
	void clear();

	#pragma endregion
	//========== SEARCHING ===========
	#pragma region Searching

	/* Returns true if a command with the specified name is in the collection. */
	bool contains(const std::string& name) const;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when an command has been added to the collection. */
	Event<CommandModifiedEventArgs>& eventCommandAdded();
	/* Called when an command has been removed from the collection. */
	Event<CommandModifiedEventArgs>& eventCommandRemoved();

	#pragma endregion
	//========== ITERATORS ===========
	#pragma region Iterators

	typedef typename std::vector<Command>::iterator iterator;
	typedef typename std::vector<Command>::const_iterator const_iterator;

	/* Returns the beginning iterator of the container. */
	iterator begin();
	/* Returns the constant beginning iterator of the container. */
	const_iterator begin() const;
	/* Returns the ending iterator of the container. */
	iterator end();
	/* Returns the constant ending iterator of the container. */
	const_iterator end() const;

	#pragma endregion
};

//=================================================================|
} /* Trigger::PowerConsole::Controls */
#endif /* COMMAND_COLLECTION_H */
//=================================================================|
