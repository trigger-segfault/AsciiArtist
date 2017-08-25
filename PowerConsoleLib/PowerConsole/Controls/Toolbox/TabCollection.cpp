/*=================================================================|
|* File:				TabCollection.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TabCollection.h"

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

TabCollection::TabCollection()
	:	// Container
	owner(),
	tabs(),

	// Events
	_eventChildAdded(EventDirections::FirstInFirstCalled),
	_eventChildRemoved(EventDirections::FirstInLastCalled) {}

TabCollection::~TabCollection() {
	for (ControlSPtr tab : tabs) {
		tab->setParent(nullptr);
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void TabCollection::setOwner(ControlSPtr owner) {
	this->owner = owner;
}

int TabCollection::count() const {
	return (int)tabs.size();
}
bool TabCollection::isEmpty() const {
	return tabs.empty();
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

TabSPtr TabCollection::operator[](int index) {
	if (index < 0 || index >= (int)tabs.size())
		throw std::out_of_range("Invalid index in 'TabCollection::operator[]'");
	return tabs[index];
}
const TabSPtr TabCollection::operator[](int index) const {
	if (index < 0 || index >= (int)tabs.size())
		throw std::out_of_range("Invalid index in 'TabCollection::operator[]'");
	return tabs[index];
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void TabCollection::forEach(std::function<void(ControlSPtr control)> func) {
	for (TabSPtr tab : tabs) {
		tab->forEach(func);
	}
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

TabSPtr TabCollection::at(int index) {
	if (index < 0 || index >= (int)tabs.size())
		throw std::out_of_range("Invalid index in 'TabCollection::at(...)'");
	return tabs[index];
}
const TabSPtr TabCollection::at(int index) const {
	if (index < 0 || index >= (int)tabs.size())
		throw std::out_of_range("Invalid index in 'TabCollection::at(...)'");
	return tabs[index];
}
int TabCollection::add(TabSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'TabCollection::add(...)'");
	if (control->hasParent())
		throw std::exception("Control already has parent in 'TabCollection::add(...)'");

	auto parent = this->owner.lock();
	control->forEach([parent](ControlSPtr control) {
		if (!control->getName().empty() && parent->getRoot()->containsName(control->getName()))
			throw std::exception("Name in control already exists in control set in TabCollection::add(...)'");
	});

	// Initialize
	control->setParent(parent);

	tabs.push_back(control);
	eventChildAdded()(owner.lock(), ChildModifiedEventArgs(control, (int)tabs.size() - 1));
	return (int)tabs.size() - 1;
}
void TabCollection::insert(int index, TabSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'TabCollection::insert(...)'");
	if (control->hasParent())
		throw std::exception("Control already has parent in 'TabCollection::insert(...)'");
	if (index < 0 || index >(int)tabs.size())
		throw std::out_of_range("Invalid index in 'TabCollection::insert(...)'");

	auto parent = this->owner.lock();
	control->forEach([parent](ControlSPtr control) {
		if (!control->getName().empty() && parent->getRoot()->containsName(control->getName()))
			throw std::exception("Name in control already exists in control set in TabCollection::insert(...)'");
	});

	// Initialize
	control->setParent(parent);

	tabs.insert(tabs.begin() + index, control);
	eventChildAdded()(owner.lock(), ChildModifiedEventArgs(control, index));
}
int TabCollection::remove(TabSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'TabCollection::remove(...)'");
	auto it = std::find(tabs.begin(), tabs.end(), control);
	if (it == tabs.end())
		throw std::out_of_range("Control does not exist in 'TabCollection::remove(...)'");

	// Clean up
	control->setParent(nullptr);

	tabs.erase(it);
	int index = std::distance(tabs.begin(), it);
	eventChildRemoved()(owner.lock(), ChildModifiedEventArgs(control, index));
	return index;
}
void TabCollection::removeAt(int index) {
	if (index < 0 || index >= (int)tabs.size())
		throw std::out_of_range("Invalid index in 'TabCollection::removeAt(...)'");

	// Clean up
	tabs[index]->setParent(nullptr);

	ControlSPtr removedTab = tabs[index];
	tabs.erase(tabs.begin() + index);
	eventChildRemoved()(owner.lock(), ChildModifiedEventArgs(removedTab, index));
}
void TabCollection::clear() {
	// Clean up
	for (TabSPtr tab : tabs) {
		tab->setParent(nullptr);
	}

	std::vector<TabSPtr> tabsForEvent;
	tabs.swap(tabsForEvent);
	for (int i = 0; i < (int)tabsForEvent.size(); i++) {
		eventChildRemoved()(owner.lock(), ChildModifiedEventArgs(tabsForEvent[i], i));
	}
}

#pragma endregion
//========== SEARCHING ===========
#pragma region Searching

bool TabCollection::contains(TabCSPtr control) const {
	return std::find(tabs.begin(), tabs.end(), control) != tabs.end();
}
int TabCollection::indexOf(TabCSPtr control) const {
	auto it = std::find(tabs.begin(), tabs.end(), control);
	return (it != tabs.end() ? std::distance(tabs.begin(), it) : -1);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<ChildModifiedEventArgs>& TabCollection::eventChildAdded() {
	return _eventChildAdded;
}
Event<ChildModifiedEventArgs>& TabCollection::eventChildRemoved() {
	return _eventChildRemoved;
}

#pragma endregion
//========== ITERATORS ===========
#pragma region Iterators

TabCollection::const_iterator TabCollection::begin() const {
	return tabs.begin();
}
TabCollection::const_iterator TabCollection::end() const {
	return tabs.end();
}

#pragma endregion
//=================================================================|
