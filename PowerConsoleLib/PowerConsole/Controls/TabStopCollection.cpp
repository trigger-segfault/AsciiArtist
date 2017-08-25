/*=================================================================|
|* File:				TabStopCollection.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TabStopCollection.h"
#include <PowerConsole/Controls/Window.h>

using namespace PowerConsole::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

TabStopCollection::TabStopCollection() {

}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void TabStopCollection::setOwner(WindowSPtr owner) {
	this->owner = owner;
}

ControlSPtr TabStopCollection::getNextTabStop() {
	if (tabStops.empty())
		return nullptr;
	int startIndex = getIndex();
	int index = (startIndex + 1) % (int)tabStops.size();
	while (!tabStops[index]->isEnabled() || !tabStops[index]->isVisible()) {
		if (index == startIndex)
			return nullptr;
		index = (index + 1) % (int)tabStops.size();
	}
	return tabStops[index];
}
bool TabStopCollection::isFocusTabStop() const {
	auto it = std::find_if(tabStops.begin(), tabStops.end(),
		[](ControlSPtr control) { return control->hasFocus(); }
	);
	return it != tabStops.end();
}
int TabStopCollection::getIndex() const {
	auto it = std::find_if(tabStops.begin(), tabStops.end(),
		[](ControlSPtr control) { return control->hasFocus(); }
	);
	if (it != tabStops.end())
		return std::distance(tabStops.begin(), it);
	return -1;
}
int TabStopCollection::count() const {
	return (int)tabStops.size();
}
bool TabStopCollection::isEmpty() const {
	return tabStops.empty();
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

ControlSPtr TabStopCollection::operator[](int index) {
	if (index < 0 || index >= (int)tabStops.size())
		throw std::out_of_range("Invalid index in 'TabStopCollection::operator[]'");
	return tabStops[index];
}
const ControlSPtr TabStopCollection::operator[](int index) const {
	if (index < 0 || index >= (int)tabStops.size())
		throw std::out_of_range("Invalid index in 'TabStopCollection::operator[]'");
	return tabStops[index];
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

ControlSPtr TabStopCollection::at(int index) {
	if (index < 0 || index >= (int)tabStops.size())
		throw std::out_of_range("Invalid index in 'TabStopCollection::at(...)'");
	return tabStops[index];
}
const ControlSPtr TabStopCollection::at(int index) const {
	if (index < 0 || index >= (int)tabStops.size())
		throw std::out_of_range("Invalid index in 'TabStopCollection::at(...)'");
	return tabStops[index];
}
int TabStopCollection::add(ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'TabStopCollection::add(...)'");
	if (std::find(tabStops.begin(), tabStops.end(), control) != tabStops.end())
		throw std::exception("Tab stop already in collection in 'TabStopCollection::add(...)'");

	tabStops.push_back(control);
	return (int)tabStops.size() - 1;
}
void TabStopCollection::insert(int index, ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'TabStopCollection::insert(...)'");
	if (index < 0 || index >(int)tabStops.size())
		throw std::out_of_range("Invalid index in 'TabStopCollection::insert(...)'");
	if (std::find(tabStops.begin(), tabStops.end(), control) != tabStops.end())
		throw std::exception("Tab stop already in collection in 'TabStopCollection::insert(...)'");

	tabStops.insert(tabStops.begin() + index, control);
}
int TabStopCollection::remove(ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'TabStopCollection::remove(...)'");
	auto it = std::find(tabStops.begin(), tabStops.end(), control);
	if (it == tabStops.end())
		throw std::out_of_range("Control does not exist in 'TabStopCollection::remove(...)'");

	tabStops.erase(it);
	return distance(tabStops.begin(), it);
}
void TabStopCollection::removeAt(int index) {
	if (index < 0 || index >= (int)tabStops.size())
		throw std::out_of_range("Invalid index in 'TabStopCollection::removeAt(...)'");

	tabStops.erase(tabStops.begin() + index);
}
void TabStopCollection::clear() {
	tabStops.clear();
}

#pragma endregion
//========== SEARCHING ===========
#pragma region Searching

bool TabStopCollection::contains(ControlCSPtr control) const {
	return std::find(tabStops.begin(), tabStops.end(), control) != tabStops.end();
}

int TabStopCollection::indexOf(ControlCSPtr control) const {
	auto it = std::find(tabStops.begin(), tabStops.end(), control);
	return (it != tabStops.end() ? std::distance(tabStops.begin(), it) : -1);
}

#pragma endregion
//=================================================================|
