/*=================================================================|
|* File:				ItemCollection.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ItemCollection.h"

using namespace PowerConsole::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ItemCollection::ItemCollection()
 :	// Container
	owner(),
	items(),

	// Events
	_eventItemAdded(EventDirections::FirstInFirstCalled),
	_eventItemRemoved(EventDirections::FirstInLastCalled) {
	
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void ItemCollection::setOwner(ControlSPtr owner) {
	this->owner = owner;
}

int ItemCollection::count() const {
	return (int)items.size();
}
bool ItemCollection::isEmpty() const {
	return items.empty();
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

std::string& ItemCollection::operator[](int index) {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'ItemCollection::operator[]'");
	return items[index];
}
const std::string& ItemCollection::operator[](int index) const {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'ItemCollection::operator[]'");
	return items[index];
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

std::string& ItemCollection::at(int index) {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'ItemCollection::at(...)'");
	return items[index];
}
const std::string& ItemCollection::at(int index) const {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'ItemCollection::at(...)'");
	return items[index];
}
int ItemCollection::add(const std::string& item) {
	items.push_back(item);
	eventItemAdded()(owner.lock(), ItemModifiedEventArgs(item, (int)items.size() - 1));
	return (int)items.size() - 1;
}
void ItemCollection::insert(int index, const std::string& item) {
	if (index < 0 || index > (int)items.size())
		throw std::out_of_range("Invalid index in 'ItemCollection::insert(...)'");

	items.insert(items.begin() + index, item);
	eventItemAdded()(owner.lock(), ItemModifiedEventArgs(item, index));
}
int ItemCollection::remove(const std::string& item) {
	auto it = std::find(items.begin(), items.end(), item);
	if (it == items.end())
		throw std::out_of_range("Control does not exist in 'ItemCollection::remove(...)'");

	// Clean up
	items.erase(it);
	int index = std::distance(items.begin(), it);
	eventItemRemoved()(owner.lock(), ItemModifiedEventArgs(item, index));
	return index;
}
void ItemCollection::removeAt(int index) {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'ItemCollection::removeAt(...)'");

	// Clean up
	std::string removedItem = items[index];
	items.erase(items.begin() + index);
	eventItemRemoved()(owner.lock(), ItemModifiedEventArgs(removedItem, index));
}
void ItemCollection::clear() {
	
	std::vector<std::string> itemsForEvent;
	items.swap(itemsForEvent);
	for (int i = 0; i < (int)itemsForEvent.size(); i++) {
		eventItemRemoved()(owner.lock(), ItemModifiedEventArgs(itemsForEvent[i], i));
	}
}

// Moving
void ItemCollection::swap(int indexA, int indexB) {
	std::string moveItem = std::move(items[indexA]);
	items[indexA] = std::move(items[indexB]);
	items[indexB] = std::move(moveItem);
	// Swap item event
}
void ItemCollection::move(int index, int newIndex, bool relative) {
	int finalNewIndex = newIndex + (relative ? index : 0);
	std::string moveItem = std::move(items[index]);
	items.erase(items.begin() + index);
	items.insert(items.begin() + finalNewIndex, std::move(moveItem));
	// Move item event
}

#pragma endregion
//========== SEARCHING ===========
#pragma region Searching

bool ItemCollection::contains(const std::string& item) const {
	return std::find(items.begin(), items.end(), item) != items.end();
}

int ItemCollection::indexOf(const std::string& item) const {
	auto it = std::find(items.begin(), items.end(), item);
	return (it != items.end() ? std::distance(items.begin(), it) : -1);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<ItemModifiedEventArgs>& ItemCollection::eventItemAdded() {
	return _eventItemAdded;
}
Event<ItemModifiedEventArgs>& ItemCollection::eventItemRemoved() {
	return _eventItemRemoved;
}

#pragma endregion
//========== ITERATORS ===========
#pragma region Iterators

ItemCollection::const_iterator ItemCollection::begin() const {
	return items.begin();
}
ItemCollection::const_iterator ItemCollection::end() const {
	return items.end();
}

#pragma endregion
//=================================================================|
