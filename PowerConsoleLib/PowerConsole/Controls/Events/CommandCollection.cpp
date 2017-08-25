/*=================================================================|
|* File:				CommandCollection.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "CommandCollection.h"

using namespace PowerConsole::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

CommandCollection::CommandCollection() 
 :	// Container
	owner(),
	commands(),
	
	// Events
	_eventCommandAdded(EventDirections::FirstInFirstCalled),
	_eventCommandRemoved(EventDirections::FirstInFirstCalled) {}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void CommandCollection::setOwner(ControlSPtr owner) {
	this->owner = owner;
}

int CommandCollection::count() const {
	return (int)commands.size();
}
bool CommandCollection::isEmpty() const {
	return commands.empty();
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

Command& CommandCollection::operator[](const std::string& name) {
	auto it = std::find_if(commands.begin(), commands.end(),
		[&name](const Command& command) { return command.getName() == name; }
	);
	if (it == commands.end())
		throw std::out_of_range("No command with that name in 'CommandCollection::operator[]'");
	return *it;
}
const Command& CommandCollection::operator[](const std::string& name) const {
	auto it = std::find_if(commands.begin(), commands.end(),
		[&name](const Command& command) { return command.getName() == name; }
	);
	if (it == commands.end())
		throw std::out_of_range("No command with that name in 'CommandCollection::operator[]'");
	return *it;
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

Command& CommandCollection::at(const std::string& name) {
	auto it = std::find_if(commands.begin(), commands.end(),
		[&name](const Command& command) { return command.getName() == name; }
	);
	if (it == commands.end())
		throw std::out_of_range("No command with that name in 'CommandCollection::at(...)'");
	return *it;
}
const Command& CommandCollection::at(const std::string& name) const {
	auto it = std::find_if(commands.begin(), commands.end(),
		[&name](const Command& command) { return command.getName() == name; }
	);
	if (it == commands.end())
		throw std::out_of_range("No command with that name in 'CommandCollection::at(...)'");
	return *it;
}
void CommandCollection::add(const Command& command) {
	if (contains(command.getName()))
		throw std::exception("Command with that name already exists in 'CommandCollection::add(...)'");
	commands.push_back(std::move(command));
	eventCommandAdded()(owner.lock(), CommandModifiedEventArgs(command));
}
void CommandCollection::add(Command&& command) {
	if (contains(command.getName()))
		throw std::exception("Command with that name already exists in 'CommandCollection::add(...)'");
	commands.push_back(command);
	eventCommandAdded()(owner.lock(), CommandModifiedEventArgs(command));
}
void CommandCollection::remove(const std::string& name) {
	auto it = std::find_if(commands.begin(), commands.end(),
		[&name](const Command& command) { return command.getName() == name; }
	);
	if (it == commands.end())
		throw std::out_of_range("No control with that name in 'CommandCollection::remove(...)'");

	// Clean up
	Command command = *it;
	commands.erase(it);
	int index = distance(commands.begin(), it);
	eventCommandRemoved()(owner.lock(), CommandModifiedEventArgs(command));
}
void CommandCollection::clear() {
	std::vector<Command> commandsForEvent;
	commands.swap(commandsForEvent);
	for (int i = 0; i < (int)commandsForEvent.size(); i++) {
		eventCommandRemoved()(owner.lock(), CommandModifiedEventArgs(commandsForEvent[i]));
	}
}

#pragma endregion
//========== SEARCHING ===========
#pragma region Searching

bool CommandCollection::contains(const std::string& name) const {
	return std::find_if(commands.begin(), commands.end(),
		[&name](const Command& command) { return command.getName() == name; }
	) != commands.end();
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<CommandModifiedEventArgs>& CommandCollection::eventCommandAdded() {
	return _eventCommandAdded;
}
Event<CommandModifiedEventArgs>& CommandCollection::eventCommandRemoved() {
	return _eventCommandRemoved;
}

#pragma endregion
//========== ITERATORS ===========
#pragma region Iterators

CommandCollection::iterator CommandCollection::begin() {
	return commands.begin();
}
CommandCollection::const_iterator CommandCollection::begin() const {
	return commands.begin();
}
CommandCollection::iterator CommandCollection::end() {
	return commands.end();
}
CommandCollection::const_iterator CommandCollection::end() const {
	return commands.end();
}

#pragma endregion
//=================================================================|
