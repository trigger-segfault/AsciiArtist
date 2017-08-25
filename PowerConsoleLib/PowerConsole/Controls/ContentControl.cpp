/*=================================================================|
|* File:				ContentControl.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ContentControl.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ContentControl::ContentControl()
 :	// Content
	content(nullptr),
	
	// Events
	_eventContentAdded(EventDirections::FirstInFirstCalled),
	_eventContentRemoved(EventDirections::FirstInFirstCalled) {
	
	eventLayoutChanged().add(bindEvent(ContentControl::onLayoutChanged));
	_eventContentAdded.add(bindEvent(ContentControl::onContentAdded));
}
ContentControl::~ContentControl() {
	if (content != nullptr) {
		content->setParent(nullptr);
		//removeContent();
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool ContentControl::containsName(const std::string& name) const {
	if (name.empty())
		return false;
	if (!getName().empty() && getName() == name)
		return true;

	if (content != nullptr && content->containsName(name)) {
		return true;
	}
	return false;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void ContentControl::updateContentLayout() {
	if (content == nullptr)
		throw std::exception("'ContentControl::updateContentLayout()' should not be called when no content is set");
	content->setContainerLayout(Rectangle2I(getActualSize()), getLocalVisibleArea());
}

void ContentControl::forEach(std::function<void(ControlSPtr control)> func) {
	func(shared_from_this());
	if (content != nullptr) {
		content->forEach(func);
	}
}
void ContentControl::repaint(GraphicsSPtr g) {
	g->startRegion(getLocalPosition(), getLocalVisibleArea());
	eventPaint()(shared_from_this(), PaintEventArgs(g));
	g->resetOptions();

	if (content != nullptr) {
		if (content->isVisible())
			content->repaint(g);
	}
	g->endRegion();
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

ControlSPtr ContentControl::getContent() {
	return content;
}
void ContentControl::setContent(ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'ContentControl::setContent(...)'");
	if (control->hasParent())
		throw std::exception("Control already has parent in 'ContentControl::setContent(...)'");

	if (content != nullptr) {
		removeContent();
	}
	control->setParent(shared_from_this());
	content = control;

	_eventContentAdded(shared_from_this(), ContentModifiedEventArgs(control));
}
void ContentControl::removeContent() {
	if (content == nullptr)
		throw std::exception("No content to remove in 'ContentControl::removeContent()'");

	ControlSPtr removedContent = content;
	content->setParent(nullptr);
	content = nullptr;

	_eventContentRemoved(shared_from_this(), ContentModifiedEventArgs(removedContent));
}

bool ContentControl::hasContent() const {
	return (content != nullptr);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<ContentModifiedEventArgs>& ContentControl::eventContentAdded() {
	return _eventContentAdded;
}
Event<ContentModifiedEventArgs>& ContentControl::eventContentRemoved() {
	return _eventContentRemoved;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ContentControl::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if ((e.sizeChanged || e.visibleAreaChanged) && content != nullptr) {
		updateContentLayout();
	}
}
void ContentControl::onContentAdded(ControlSPtr sender, ContentModifiedEventArgs& e) {
	updateContentLayout();
}

#pragma endregion
//=================================================================|
