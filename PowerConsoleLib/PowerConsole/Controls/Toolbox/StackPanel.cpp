/*=================================================================|
|* File:				StackPanel.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "StackPanel.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

StackPanel::StackPanel() 
 :	orientation(Orientations::Vertical),
	spacing(0) {

	eventInitialize().add(bindEvent(StackPanel::onInitialize));
	eventLayoutChanged().add(bindEvent(StackPanel::onLayoutChanged));
	eventChildAdded().add(bindEvent(StackPanel::onChildAdded));
	eventChildRemoved().add(bindEvent(StackPanel::onChildRemoved));
}
void StackPanel::onInitialize(ControlSPtr sender, EventArgs& e) {
	setSize(Point2I(SIZE_AUTO));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

Orientations StackPanel::getOrientation() const {
	return orientation;
}
void StackPanel::setOrientation(Orientations orientation) {
	this->orientation = orientation;
}
int StackPanel::getSpacing() const {
	return spacing;
}
void StackPanel::setSpacing(int spacing) {
	this->spacing = spacing;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I StackPanel::autoSizeBehavior() const {
	int size = 0;
	if (orientation == Orientations::Horizontal) {
		for (ControlSPtr control : children())
			size += GMath::max(0, control->getSizeWithAuto().x);
		return Point2I(size + ((children().count() - 1) * spacing), 0);
	}
	else {
		for (ControlSPtr control : children())
			size += GMath::max(0, control->getSizeWithAuto().y);
		return Point2I(0, size + ((children().count() - 1) * spacing));
	}
}
void StackPanel::updateChildLayout(ControlSPtr control) {
	int offset = 0;
	Rectangle2I boundsRect;
	for (ControlSPtr child : children()) {
		if (child == control) {
			if (orientation == Orientations::Horizontal) {
				boundsRect = Rectangle2I(offset, 0, child->getSizeWithAuto().x, getActualSize().y);
				child->setContainerLayout(
					boundsRect,
					boundsRect.intersection(getLocalVisibleArea())
				);
			}
			else {
				boundsRect = Rectangle2I(0, offset, getActualSize().x, child->getSizeWithAuto().y);
				child->setContainerLayout(
					boundsRect,
					boundsRect.intersection(getLocalVisibleArea())
				);
			}
			break;
		}
		offset += GMath::max(0, orientation == Orientations::Horizontal ? child->getSizeWithAuto().x : child->getSizeWithAuto().y) + spacing;
	}
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void StackPanel::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged || e.visibleAreaChanged || e.positionChanged) {
		int offset = 0;
		Rectangle2I boundsRect, boundsRect2;
		for (ControlSPtr child : children()) {
			if (orientation == Orientations::Horizontal) {
				boundsRect = Rectangle2I(offset, 0, child->getSizeWithAuto().x, getActualSize().y);
				child->setContainerLayout(
					boundsRect,
					boundsRect.intersection(getLocalVisibleArea())
				);
			}
			else {
				boundsRect = Rectangle2I(0, offset, getActualSize().x, child->getSizeWithAuto().y);
				boundsRect2 = boundsRect.intersection(getLocalVisibleArea());
				child->setContainerLayout(
					boundsRect,
					boundsRect.intersection(getLocalVisibleArea())
				);
			}
			offset += GMath::max(0, orientation == Orientations::Horizontal ? child->getSizeWithAuto().x : child->getSizeWithAuto().y) + spacing;
		}
	}
}
void StackPanel::onChildAdded(ControlSPtr sender, ChildModifiedEventArgs& e) {
	e.control->eventLayoutChanged().add(bindEvent(StackPanel::onChildLayoutChanged));
	if ((orientation == Orientations::Horizontal && getSize().x == SIZE_AUTO) ||
		(orientation == Orientations::Vertical && getSize().y == SIZE_AUTO)) {
		updateLayout(true, false, false, false, false);
	}
}
void StackPanel::onChildRemoved(ControlSPtr sender, ChildModifiedEventArgs& e) {
	e.control->eventLayoutChanged().remove(unbindEvent(StackPanel::onChildLayoutChanged));
	if ((orientation == Orientations::Horizontal && getSize().x == SIZE_AUTO) ||
		(orientation == Orientations::Vertical && getSize().y == SIZE_AUTO)) {
		updateLayout(true, false, false, false, false);
	}
}
void StackPanel::onChildLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		updateLayout(true, false, false, false, false);
		/*int offset = 0;
		bool senderHit = false;
		for (ControlSPtr child : children()) {
			if (child == sender) {
				senderHit = true;
			}
			if (senderHit) {
				if (orientation == Orientations::Horizontal) {
					child->setContainerLayout(
						Rectangle2I(offset, 0, child->getSize().x, getActualSize().y),
						Rectangle2I(offset, 0, child->getSize().x, getActualSize().y).intersect(getLocalVisibleArea())
					);
				}
				else {
					child->setContainerLayout(
						Rectangle2I(0, offset, getActualSize().x, child->getSize().y),
						Rectangle2I(0, offset, getActualSize().x, child->getSize().y).intersect(getLocalVisibleArea())
					);
				}
			}
			offset += GMath::max(0, orientation == Orientations::Horizontal ? child->getSize().x : child->getSize().y);
		}*/
	}
}

#pragma endregion
//=================================================================|
