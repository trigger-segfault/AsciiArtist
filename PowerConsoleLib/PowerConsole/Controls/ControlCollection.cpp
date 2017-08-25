/*=================================================================|
|* File:				ControlCollection.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ControlCollection.h"

using namespace PowerConsole::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ControlCollection::ControlCollection()
 :	// Container
	owner(),
	children(),

	// Events
	_eventChildAdded(EventDirections::FirstInFirstCalled),
	_eventChildRemoved(EventDirections::FirstInLastCalled) {}

ControlCollection::~ControlCollection() {
	for (ControlSPtr control : children) {
		control->setParent(nullptr);
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void ControlCollection::setOwner(ControlSPtr owner) {
	this->owner = owner;
}

int ControlCollection::count() const {
	return (int)children.size();
}
bool ControlCollection::isEmpty() const {
	return children.empty();
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

ControlSPtr ControlCollection::operator[](int index) {
	if (index < 0 || index >= (int)children.size())
		throw std::out_of_range("Invalid index in 'ControlCollection::operator[]'");
	return children[index];
}
const ControlSPtr ControlCollection::operator[](int index) const {
	if (index < 0 || index >= (int)children.size())
		throw std::out_of_range("Invalid index in 'ControlCollection::operator[]'");
	return children[index];
}
ControlSPtr ControlCollection::operator[](const std::string& name) {
	auto it = std::find_if(children.begin(), children.end(),
		[&name](const ControlSPtr& control) { return control->getName() == name; }
	);
	if (it == children.end())
		throw std::out_of_range("No control with that name in 'ControlCollection::operator[]'");
	return *it;
}
const ControlSPtr ControlCollection::operator[](const std::string& name) const {
	auto it = std::find_if(children.begin(), children.end(),
		[&name](const ControlSPtr& control) { return control->getName() == name; }
	);
	if (it == children.end())
		throw std::out_of_range("No control with that name in 'ControlCollection::operator[]'");
	return *it;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void ControlCollection::forEach(std::function<void(ControlSPtr control)> func) {
	for (ControlSPtr control : children) {
		control->forEach(func);
	}
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

ControlSPtr ControlCollection::at(int index) {
	if (index < 0 || index >= (int)children.size())
		throw std::out_of_range("Invalid index in 'ControlCollection::at(...)'");
	return children[index];
}
const ControlSPtr ControlCollection::at(int index) const {
	if (index < 0 || index >= (int)children.size())
		throw std::out_of_range("Invalid index in 'ControlCollection::at(...)'");
	return children[index];
}
ControlSPtr ControlCollection::at(const std::string& name) {
	auto it = std::find_if(children.begin(), children.end(),
		[&name](const ControlSPtr& control) { return control->getName() == name; }
	);
	if (it == children.end())
		throw std::out_of_range("No control with that name in 'ControlCollection::at(...)'");
	return *it;
}
const ControlSPtr ControlCollection::at(const std::string& name) const {
	auto it = std::find_if(children.begin(), children.end(),
		[&name](const ControlSPtr& control) { return control->getName() == name; }
	);
	if (it == children.end())
		throw std::out_of_range("No control with that name in 'ControlCollection::at(...)'");
	return *it;
}
int ControlCollection::add(ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'ControlCollection::add(...)'");
	if (control->hasParent())
		throw std::exception("Control already has parent in 'ControlCollection::add(...)'");

	auto parent = this->owner.lock();
	control->forEach([parent](ControlSPtr control) {
		if (!control->getName().empty() && parent->getRoot()->containsName(control->getName()))
			throw std::exception("Name in control already exists in control set in ControlCollection::add(...)'");
	});

	// Initialize
	control->setParent(parent);

	children.push_back(control);
	eventChildAdded()(owner.lock(), ChildModifiedEventArgs(control, (int)children.size() - 1));
	return (int)children.size() - 1;
}
void ControlCollection::insert(int index, ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'ControlCollection::insert(...)'");
	if (control->hasParent())
		throw std::exception("Control already has parent in 'ControlCollection::insert(...)'");
	if (index < 0 || index > (int)children.size())
		throw std::out_of_range("Invalid index in 'ControlCollection::insert(...)'");

	auto parent = this->owner.lock();
	control->forEach([parent](ControlSPtr control) {
		if (!control->getName().empty() && parent->getRoot()->containsName(control->getName()))
			throw std::exception("Name in control already exists in control set in ControlCollection::insert(...)'");
	});

	// Initialize
	control->setParent(parent);

	children.insert(children.begin() + index, control);
	eventChildAdded()(owner.lock(), ChildModifiedEventArgs(control, index));
}
int ControlCollection::remove(ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'ControlCollection::remove(...)'");
	auto it = std::find(children.begin(), children.end(), control);
	if (it == children.end())
		throw std::out_of_range("Control does not exist in 'ControlCollection::remove(...)'");

	// Clean up
	control->setParent(nullptr);

	children.erase(it);
	int index = std::distance(children.begin(), it);
	eventChildRemoved()(owner.lock(), ChildModifiedEventArgs(control, index));
	return index;
}
int ControlCollection::remove(const std::string& name) {
	auto it = std::find_if(children.begin(), children.end(),
		[&name](const ControlSPtr& control) { return control->getName() == name; }
	);
	if (it == children.end())
		throw std::out_of_range("No control with that name in 'ControlCollection::remove(...)'");

	// Clean up
	(*it)->setParent(nullptr);
	ControlSPtr control = *it;
	children.erase(it);
	int index = std::distance(children.begin(), it);
	eventChildRemoved()(owner.lock(), ChildModifiedEventArgs(*it, index));
	return index;
}
void ControlCollection::removeAt(int index) {
	if (index < 0 || index >= (int)children.size())
		throw std::out_of_range("Invalid index in 'ControlCollection::removeAt(...)'");

	// Clean up
	children[index]->setParent(nullptr);

	ControlSPtr removedControl = children[index];
	children.erase(children.begin() + index);
	eventChildRemoved()(owner.lock(), ChildModifiedEventArgs(removedControl, index));
}
void ControlCollection::clear() {
	// Clean up
	for (ControlSPtr control : children) {
		control->setParent(nullptr);
	}

	std::vector<ControlSPtr> childrenForEvent;
	children.swap(childrenForEvent);
	for (int i = 0; i < (int)childrenForEvent.size(); i++) {
		eventChildRemoved()(owner.lock(), ChildModifiedEventArgs(childrenForEvent[i], i));
	}
}

#pragma endregion
//========== SEARCHING ===========
#pragma region Searching

bool ControlCollection::contains(ControlCSPtr control) const {
	return std::find(children.begin(), children.end(), control) != children.end();
}
bool ControlCollection::contains(const std::string& name) const {
	return std::find_if(children.begin(), children.end(),
		[&name](const ControlSPtr& control) { return control->getName() == name; }
	) != children.end();
}

int ControlCollection::indexOf(ControlCSPtr control) const {
	auto it = std::find(children.begin(), children.end(), control);
	return (it != children.end() ? std::distance(children.begin(), it) : -1);
}
int ControlCollection::indexOf(const std::string& name) const {
	auto it = std::find_if(children.begin(), children.end(),
		[&name](const ControlSPtr& control) { return control->getName() == name; }
	);
	return (it != children.end() ? std::distance(children.begin(), it) : -1);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<ChildModifiedEventArgs>& ControlCollection::eventChildAdded() {
	return _eventChildAdded;
}
Event<ChildModifiedEventArgs>& ControlCollection::eventChildRemoved() {
	return _eventChildRemoved;
}

#pragma endregion
//========== ITERATORS ===========
#pragma region Iterators

ControlCollection::const_iterator ControlCollection::begin() const {
	return children.begin();
}
ControlCollection::const_iterator ControlCollection::end() const {
	return children.end();
}

#pragma endregion
//=================================================================|
