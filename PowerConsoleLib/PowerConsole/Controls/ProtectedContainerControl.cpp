/*=================================================================|
|* File:				ProtectedContainerControl.cpp			  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ProtectedContainerControl.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ProtectedContainerControl::ProtectedContainerControl()
	: collection() {

	eventInitialize().add(bindEvent(ProtectedContainerControl::onInitialize));
	eventLayoutChanged().add(bindEvent(ProtectedContainerControl::onLayoutChanged));
	eventChildAdded().add(bindEvent(ProtectedContainerControl::onChildAdded));
}
void ProtectedContainerControl::onInitialize(ControlSPtr sender, EventArgs& e) {
	collection.setOwner(shared_from_this());
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool ProtectedContainerControl::containsName(const std::string& name) const {
	if (name.empty())
		return false;
	if (!getName().empty() && getName() == name)
		return true;

	for (ControlSPtr control : children()) {
		if (control->containsName(name))
			return true;
	}
	return false;
}

ControlCollection& ProtectedContainerControl::children() {
	return collection;
}
const ControlCollection& ProtectedContainerControl::children() const {
	return collection;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating


/*void ContainerControl::updateLayout(bool sizeChanged, bool marginChanged, bool alignmentChanged, bool positionChanged, bool visibleAreaChanged) {
Control::updateLayout(sizeChanged, marginChanged, alignmentChanged, positionChanged, visibleAreaChanged);
}*/
void ProtectedContainerControl::updateChildLayout(ControlSPtr control) {
	control->setContainerLayout(Rectangle2I(getActualSize()), getLocalVisibleArea());
}

void ProtectedContainerControl::forEach(std::function<void(ControlSPtr control)> func) {
	func(shared_from_this());
	collection.forEach(func);
}
void ProtectedContainerControl::repaint(GraphicsSPtr g) {
	g->startRegion(getLocalPosition(), getLocalVisibleArea());
	eventPaint()(shared_from_this(), PaintEventArgs(g));
	g->resetOptions();

	for (ControlSPtr control : collection) {
		if (control->isVisible())
			control->repaint(g);
	}
	g->endRegion();
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<ChildModifiedEventArgs>& ProtectedContainerControl::eventChildAdded() {
	return collection.eventChildAdded();
}
Event<ChildModifiedEventArgs>& ProtectedContainerControl::eventChildRemoved() {
	return collection.eventChildRemoved();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ProtectedContainerControl::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged || e.visibleAreaChanged) {
		for (ControlSPtr control : collection) {
			updateChildLayout(control);
		}
	}
}
void ProtectedContainerControl::onChildAdded(ControlSPtr sender, ChildModifiedEventArgs& e) {
	updateChildLayout(e.control);
}

#pragma endregion
//=================================================================|
