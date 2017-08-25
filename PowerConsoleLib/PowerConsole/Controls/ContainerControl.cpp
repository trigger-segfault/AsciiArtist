/*=================================================================|
|* File:				ContainerControl.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ContainerControl.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ContainerControl::ContainerControl()
 :	collection() {

	eventInitialize().add(bindEvent(ContainerControl::onInitialize));
	eventLayoutChanged().add(bindEvent(ContainerControl::onLayoutChanged));
	eventChildAdded().add(bindEvent(ContainerControl::onChildAdded));
}
void ContainerControl::onInitialize(ControlSPtr sender, EventArgs& e) {
	collection.setOwner(shared_from_this());
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool ContainerControl::containsName(const std::string& name) const {
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

ControlCollection& ContainerControl::children() {
	return collection;
}
const ControlCollection& ContainerControl::children() const {
	return collection;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating


/*void ContainerControl::updateLayout(bool sizeChanged, bool marginChanged, bool alignmentChanged, bool positionChanged, bool visibleAreaChanged) {
	Control::updateLayout(sizeChanged, marginChanged, alignmentChanged, positionChanged, visibleAreaChanged);
}*/
void ContainerControl::updateChildLayout(ControlSPtr control) {
	control->setContainerLayout(Rectangle2I(getActualSize()), getLocalVisibleArea());
}

void ContainerControl::forEach(std::function<void(ControlSPtr control)> func) {
	func(shared_from_this());
	collection.forEach(func);
}
void ContainerControl::repaint(GraphicsSPtr g) {
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

Event<ChildModifiedEventArgs>& ContainerControl::eventChildAdded() {
	return collection.eventChildAdded();
}
Event<ChildModifiedEventArgs>& ContainerControl::eventChildRemoved() {
	return collection.eventChildRemoved();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ContainerControl::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged || e.visibleAreaChanged) {
		for (ControlSPtr control : collection) {
			updateChildLayout(control);
		}
	}
}
void ContainerControl::onChildAdded(ControlSPtr sender, ChildModifiedEventArgs& e) {
	updateChildLayout(e.control);
}

#pragma endregion
//=================================================================|
