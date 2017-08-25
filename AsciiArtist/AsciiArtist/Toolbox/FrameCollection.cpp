/*=================================================================|
|* File:				FrameCollection.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FrameCollection.h"

using namespace AsciiArtist::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

FrameCollection::FrameCollection()
	:	// Container
	owner(),
	items(),

	// Events
	_eventFrameAdded(EventDirections::FirstInFirstCalled),
	_eventFrameRemoved(EventDirections::FirstInLastCalled) {

}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void FrameCollection::setOwner(ControlSPtr owner) {
	this->owner = owner;
}

int FrameCollection::count() const {
	return (int)items.size();
}
bool FrameCollection::isEmpty() const {
	return items.empty();
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

FrameInfo& FrameCollection::operator[](int index) {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'FrameCollection::operator[]'");
	return items[index];
}
const FrameInfo& FrameCollection::operator[](int index) const {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'FrameCollection::operator[]'");
	return items[index];
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

FrameInfo& FrameCollection::at(int index) {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'FrameCollection::at(...)'");
	return items[index];
}
const FrameInfo& FrameCollection::at(int index) const {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'FrameCollection::at(...)'");
	return items[index];
}
int FrameCollection::add(const FrameInfo& item) {
	items.push_back(item);
	eventFrameAdded()(owner.lock(), FrameModifiedEventArgs(item, (int)items.size() - 1));
	return (int)items.size() - 1;
}
void FrameCollection::insert(int index, const FrameInfo& item) {
	if (index < 0 || index >(int)items.size())
		throw std::out_of_range("Invalid index in 'FrameCollection::insert(...)'");

	items.insert(items.begin() + index, item);
	eventFrameAdded()(owner.lock(), FrameModifiedEventArgs(item, index));
}
void FrameCollection::removeAt(int index) {
	if (index < 0 || index >= (int)items.size())
		throw std::out_of_range("Invalid index in 'FrameCollection::removeAt(...)'");

	// Clean up
	FrameInfo removedItem = items[index];
	items.erase(items.begin() + index);
	eventFrameRemoved()(owner.lock(), FrameModifiedEventArgs(removedItem, index));
}
void FrameCollection::clear() {

	std::vector<FrameInfo> itemsForEvent;
	items.swap(itemsForEvent);
	for (int i = 0; i < (int)itemsForEvent.size(); i++) {
		eventFrameRemoved()(owner.lock(), FrameModifiedEventArgs(itemsForEvent[i], i));
	}
}

// Moving
void FrameCollection::swap(int indexA, int indexB) {
	FrameInfo moveItem = std::move(items[indexA]);
	items[indexA] = std::move(items[indexB]);
	items[indexB] = std::move(moveItem);
	// Swap item event
}
void FrameCollection::move(int index, int newIndex, bool relative) {
	int finalNewIndex = newIndex + (relative ? index : 0);
	FrameInfo moveItem = std::move(items[index]);
	items.erase(items.begin() + index);
	items.insert(items.begin() + finalNewIndex, std::move(moveItem));
	// Move item event
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<FrameModifiedEventArgs>& FrameCollection::eventFrameAdded() {
	return _eventFrameAdded;
}
Event<FrameModifiedEventArgs>& FrameCollection::eventFrameRemoved() {
	return _eventFrameRemoved;
}

#pragma endregion
//========== ITERATORS ===========
#pragma region Iterators

FrameCollection::const_iterator FrameCollection::begin() const {
	return items.begin();
}
FrameCollection::const_iterator FrameCollection::end() const {
	return items.end();
}

#pragma endregion
//=================================================================|
