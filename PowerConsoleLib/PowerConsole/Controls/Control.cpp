/*=================================================================|
|* File:				ControlNew.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Control.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Controls/Window.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Control::Control()
 :	// Control Tree
	initialized(false),
	parent(),

	// General
	name(""),
	enabled(true),
	visible(true),

	// Input
	focused(false),
	mouseInside(false),
	mousePosition(Point2I(std::numeric_limits<int>::max())),
	mousePositionLast(Point2I(std::numeric_limits<int>::max())),
	mouseButtonStates{ false },

	// Local Layout
	size(Point2I::ZERO),
	margin(Thickness::ZERO),
	horizontalAlignment(HorizontalAlignments::Left),
	verticalAlignment(VerticalAlignments::Top),

	// Container Layout
	actualBounds(Rectangle2I::ZERO),
	actualVisibleArea(Rectangle2I::ZERO),
	containerBounds(Rectangle2I::ZERO),
	containerVisibleArea(Rectangle2I::ZERO),

	// Events
	_eventInitialize(EventDirections::FirstInFirstCalled),
	_eventLayoutChanged(EventDirections::FirstInFirstCalled),

	_eventMouseMoved(EventDirections::FirstInFirstCalled),
	_eventMouseMovedGlobal(EventDirections::FirstInFirstCalled),
	_eventMouseEnter(EventDirections::FirstInFirstCalled),
	_eventMouseLeave(EventDirections::FirstInFirstCalled),
	_eventMouseButton(EventDirections::FirstInFirstCalled),
	_eventMouseButtonGlobal(EventDirections::FirstInFirstCalled),
	_eventMouseWheel(EventDirections::FirstInFirstCalled),
	_eventMouseWheelGlobal(EventDirections::FirstInFirstCalled),
	_eventClick(EventDirections::FirstInFirstCalled),
	_eventDoubleClick(EventDirections::FirstInFirstCalled),

	_eventKey(EventDirections::FirstInFirstCalled),
	_eventKeyGlobal(EventDirections::FirstInFirstCalled),
	_eventKeyTyped(EventDirections::FirstInFirstCalled),
	_eventKeyTypedGlobal(EventDirections::FirstInFirstCalled),

	_eventFocusGained(EventDirections::FirstInFirstCalled),
	_eventFocusLost(EventDirections::FirstInFirstCalled),

	_eventTick(EventDirections::FirstInFirstCalled),
	_eventPaint(EventDirections::FirstInFirstCalled) {

	_eventMouseMovedGlobal.add(bindEvent(Control::onMouseMovedGlobal));
	_eventMouseButtonGlobal.add(bindEvent(Control::onMouseButtonGlobal));
	_eventMouseWheelGlobal.add(bindEvent(Control::onMouseWheelGlobal));
	_eventKeyGlobal.add(bindEvent(Control::onKeyGlobal));
	_eventKeyTypedGlobal.add(bindEvent(Control::onKeyTypedGlobal));
	_eventFocusGained.add(bindEvent(Control::onFocusGained));
	_eventFocusLost.add(bindEvent(Control::onFocusLost));
}
ControlSPtr Control::initialize() {
	_eventInitialize(shared_from_this());
	initialized = true;
	return shared_from_this();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Safe Setters

void Control::setParentSafe(ControlWPtr parent) {
	this->parent = parent;
}

void Control::setSizeSafe(Point2I size) {
	this->size = size;
}
void Control::setMarginSafe(Thickness margin) {
	this->margin = margin;
}
void Control::setHorizontalAlignmentSafe(HorizontalAlignments horizontalAlignment) {
	this->horizontalAlignment = horizontalAlignment;
}
void Control::setVerticalAlignmentSafe(VerticalAlignments verticalAlignment) {
	this->verticalAlignment = verticalAlignment;
}

void Control::setActualBoundsSafe(Rectangle2I actualBounds) {
	this->actualBounds = actualBounds;
}
void Control::setActualPositionSafe(Point2I actualPosition) {
	this->actualBounds.point = actualPosition;
}
void Control::setActualSizeSafe(Point2I actualSize) {
	this->actualBounds.size = actualSize;
}
void Control::setActualVisibleAreaSafe(Rectangle2I actualVisibleArea) {
	this->actualVisibleArea = actualVisibleArea;
}
void Control::setContainerBoundsSafe(Rectangle2I containerBounds) {
	this->containerBounds = containerBounds;
}
void Control::setContainerPositionSafe(Point2I containerPosition) {
	this->containerBounds.point = containerPosition;
}
void Control::setContainerSizeSafe(Point2I containerSize) {
	this->containerBounds.size = containerSize;
}
void Control::setContainerVisibleAreaSafe(Rectangle2I containerVisibleArea) {
	this->containerVisibleArea = containerVisibleArea;
}

#pragma endregion
//--------------------------------
#pragma region Control Tree

bool Control::isInitialized() const {
	return initialized;
}
bool Control::hasParent() const {
	return !parent.expired();
}
ControlSPtr Control::getParent() {
	if (!parent.expired())
		return parent.lock();
	return nullptr;
}
void Control::setParent(ControlSPtr parent) {
	this->parent = parent;
}
ControlSPtr Control::getRoot() {
	if (!parent.expired())
		return parent.lock()->getRoot();
	return shared_from_this();
}
WindowSPtr Control::getWindow() {
	if (!parent.expired())
		return parent.lock()->getWindow();
	return nullptr;
}

#pragma endregion
//--------------------------------
#pragma region Commands

CommandCollection& Control::commands() {
	return commandCollection;
}
const CommandCollection& Control::commands() const {
	return commandCollection;
}

#pragma endregion
//--------------------------------
#pragma region General

bool Control::containsName(const std::string& name) const {
	if (!name.empty() && !this->name.empty())
		return this->name == name;
	return false;
}
bool Control::hasName() const {
	return !name.empty();
}
std::string Control::getName() const {
	return name;
}
void Control::setName(const std::string& name) {
	if (getRoot()->containsName(name))
		throw std::exception("Cannot set control name. Name already in use");
	this->name = name;
}
bool Control::isEnabled() const {
	return enabled;
}
void Control::setEnabled(bool enabled) {
	this->enabled = enabled;
	WindowSPtr window = getWindow();
	if (window != nullptr) {
		if (!enabled) {
			unfocus();
		}
		else if (visible) {
			if (window->getFocus() == nullptr && window->getFocusDefault() == shared_from_this()) {
				window->setFocus(shared_from_this());
			}
		}
	}
}
bool Control::isActuallyVisible() {
	if (visible) {
		if (hasParent())
			return getParent()->isActuallyVisible();
		return true;
	}
	return false;
}
bool Control::isVisible() const {
	return visible;
}
void Control::setVisible(bool visible) {
	this->visible = visible;
	WindowSPtr window = getWindow();
	if (window != nullptr) {
		if (!visible) {
			unfocus();
		}
		else if (enabled) {
			if (window->getFocus() == nullptr && window->getFocusDefault() == shared_from_this()) {
				window->setFocus(shared_from_this());
			}
		}
	}
}

#pragma endregion
//--------------------------------
#pragma region Local Layout

Point2I Control::getSize() const {
	return size;
}
void Control::setSize(Point2I size) {
	if (this->size != size) {
		this->size = size;
		updateLayout(true, false, false, false, false);
	}
}
Point2I Control::getSizeWithAuto() const {
	Point2I autoSize = autoSizeBehavior();
	return Point2I(size.x == SIZE_AUTO ? autoSize.x : GMath::max(0, size.x), size.y == SIZE_AUTO ? autoSize.y : GMath::max(0, size.y));
}
Thickness Control::getMargin() const {
	return margin;
}
void Control::setMargin(Thickness margin) {
	if (this->margin != margin) {
		this->margin = margin;
		updateLayout(false, true, false, false, false);
	}
}
HorizontalAlignments Control::getHorizontalAlignment() const {
	return horizontalAlignment;
}
void Control::setHorizontalAlignment(HorizontalAlignments horizontalAlignment) {
	if (this->horizontalAlignment != horizontalAlignment) {
		this->horizontalAlignment = horizontalAlignment;
		updateLayout(false, false, true, false, false);
	}
}
VerticalAlignments Control::getVerticalAlignment() const {
	return verticalAlignment;
}
void Control::setVerticalAlignment(VerticalAlignments verticalAlignment) {
	if (this->verticalAlignment != verticalAlignment) {
		this->verticalAlignment = verticalAlignment;
		updateLayout(false, false, true, false, false);
	}
}

#pragma endregion
//--------------------------------
#pragma region Container Layout

void Control::setContainerBounds(Rectangle2I containerBounds) {
	if (this->containerBounds != containerBounds) {
		bool sizeChanged = this->containerBounds.size != containerBounds.size;
		bool positionChanged = this->containerBounds.point != containerBounds.point;
		this->containerBounds = containerBounds;
		updateLayout(sizeChanged, false, false, positionChanged, false);
	}
}
void Control::setContainerVisibleArea(Rectangle2I containerVisibleArea) {
	if (this->containerVisibleArea != containerVisibleArea) {
		this->containerVisibleArea = containerVisibleArea;
		updateLayout(false, false, false, false, true);
	}
}
void Control::setContainerLayout(Rectangle2I containerBounds, Rectangle2I containerVisibleArea) {
	if (this->containerBounds != containerBounds || this->containerVisibleArea != containerVisibleArea) {
		bool sizeChanged = this->containerBounds.size != containerBounds.size;
		bool positionChanged = this->containerBounds.point != containerBounds.point;
		bool visibleAreaChanged = this->containerVisibleArea != containerVisibleArea;
		this->containerBounds = containerBounds;
		this->containerVisibleArea = containerVisibleArea;
		updateLayout(sizeChanged, false, false, positionChanged, visibleAreaChanged);
	}
}
Rectangle2I Control::getContainerBounds() const {
	return containerBounds;
}
Point2I Control::getContainerPosition() const {
	return containerBounds.point;
}
Point2I Control::getContainerSize() const {
	return containerBounds.size;
}
Rectangle2I Control::getContainerVisibleArea() const {
	return containerVisibleArea;
}
Point2I Control::getLocalPosition() const {
	return containerBounds.point + actualBounds.point;
}
Point2I Control::getGlobalPosition() const {
	if (!parent.expired()) {
		return parent.lock()->getGlobalPosition() + getLocalPosition();
	}
	return containerBounds.point + actualBounds.point;
}
Rectangle2I Control::getLocalVisibleArea() const {
	return actualVisibleArea;
}
Rectangle2I Control::getGlobalVisibleArea() const {
	return actualVisibleArea.translated(getGlobalPosition());
}
Rectangle2I Control::getActualBounds() const {
	return actualBounds;
}
Point2I Control::getActualPosition() const {
	return actualBounds.point;
}
Point2I Control::getActualSize() const {
	return actualBounds.size;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I Control::autoSizeBehavior() const {
	return Point2I::ZERO;
}
void Control::updateLayout(bool sizeChanged, bool marginChanged, bool alignmentChanged, bool positionChanged, bool visibleAreaChanged) {
	Point2I actualSizeOld = actualBounds.size;
	Rectangle2I actualVisibleAreaOld = actualVisibleArea;
	
	// Make sure size is treated as zero when set to auto.
	actualBounds = Rectangle2I(margin.topLeft(), GMath::max(Point2I::ZERO, size));
	Point2I autoSize = autoSizeBehavior();
	if (size.x == SIZE_AUTO)
		actualBounds.size.x = autoSize.x;
	if (size.y == SIZE_AUTO)
		actualBounds.size.y = autoSize.y;

	switch (horizontalAlignment) {
	case HorizontalAlignments::Center:
		actualBounds.point.x = (containerBounds.size.x - actualBounds.size.x - margin.horizontal()) / 2 + margin.left;
		break;
	case HorizontalAlignments::Right:
		actualBounds.point.x = containerBounds.size.x - margin.right - actualBounds.size.x;
		break;
	case HorizontalAlignments::Stretch:
		if (size.x == SIZE_AUTO)
			actualBounds.size.x = containerBounds.size.x - margin.horizontal();
		break;
	}

	switch (verticalAlignment) {
	case VerticalAlignments::Center:
		actualBounds.point.y = (containerBounds.size.y - actualBounds.size.y - margin.vertical()) / 2 + margin.top;
		break;
	case VerticalAlignments::Bottom:
		actualBounds.point.y = containerBounds.size.y - margin.bottom - actualBounds.size.y;
		break;
	case VerticalAlignments::Stretch:
		if (size.y == SIZE_AUTO)
			actualBounds.size.y = containerBounds.size.y - margin.vertical();
		break;
	}

	// Update visible area
	actualVisibleArea = containerVisibleArea.translated(-containerBounds.point);
	actualVisibleArea.intersect(Rectangle2I(containerBounds.size).inflated(-margin));
	actualVisibleArea.intersect(actualBounds).translate(-actualBounds.point);

	sizeChanged = actualBounds.size != actualSizeOld;
	visibleAreaChanged = actualVisibleArea != actualVisibleAreaOld;

	if (sizeChanged || marginChanged || alignmentChanged || positionChanged || visibleAreaChanged) {
		_eventLayoutChanged(shared_from_this(),
			LayoutChangedEventArgs(sizeChanged, marginChanged, alignmentChanged, positionChanged, visibleAreaChanged)
		);
	}
}
void Control::updateAutoSize(bool xChanged, bool yChanged) {
	if ((xChanged && size.x == SIZE_AUTO && horizontalAlignment != HorizontalAlignments::Stretch) ||
		(yChanged && size.y == SIZE_AUTO && verticalAlignment   != VerticalAlignments::Stretch)) {
		updateLayout(true, false, false, false, false);
	}
}

void Control::forEach(std::function<void(ControlSPtr control)> func) {
	func(shared_from_this());
}
void Control::repaint(GraphicsSPtr g) {
	g->startRegion(getLocalPosition(), getLocalVisibleArea());
	_eventPaint(shared_from_this(), PaintEventArgs(g));
	g->resetOptions();
	g->endRegion();
}

#pragma endregion
//============ INPUT =============
#pragma region Input

bool Control::hasFocus() const {
	return focused;
}
void Control::focus() {
	if (getWindow() == nullptr)
		throw std::exception("Cannot set currentFocus when control has no window");
	if (!focused && enabled && visible)
		getWindow()->setFocus(shared_from_this());
}
/* Unfocuses the control. */
void Control::unfocus() {
	if (getWindow() == nullptr)
		throw std::exception("Cannot unfocus when control has no window");
	if (focused)
		getWindow()->clearFocus();
}

bool Control::isMouseInside() const {
	return mouseInside;
}
bool Control::isMouseDown(MouseButtons buttonCode) const {
	if (buttonCode == MouseButtons::None)
		return false;
	return mouseButtonStates[(int)buttonCode - 1];
}
Point2I Control::getMousePosition() const {
	return mousePosition;
}
Point2I Control::getMousePositionLast() const {
	return mousePositionLast;
}
void Control::updateMousePositionLast() {
	mousePositionLast = mousePosition;
}
void Control::invalidateMousePosition() {
	mousePositionLast = Point2I(std::numeric_limits<int>::max());
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<EventArgs>& Control::eventInitialize() {
	return _eventInitialize;
}
Event<LayoutChangedEventArgs>& Control::eventLayoutChanged() {
	return _eventLayoutChanged;
}

// Mouse
Event<MouseEventArgs>& Control::eventMouseMoved() {
	return _eventMouseMoved;
}
Event<MouseEventArgs>& Control::eventMouseMovedGlobal() {
	return _eventMouseMovedGlobal;
}
Event<MouseEventArgs>& Control::eventMouseEnter() {
	return _eventMouseEnter;
}
Event<MouseEventArgs>& Control::eventMouseLeave() {
	return _eventMouseLeave;
}
Event<MouseButtonEventArgs>& Control::eventMouseButton() {
	return _eventMouseButton;
}
Event<MouseButtonEventArgs>& Control::eventMouseButtonGlobal() {
	return _eventMouseButtonGlobal;
}
Event<MouseWheelEventArgs>& Control::eventMouseWheel() {
	return _eventMouseWheel;
}
Event<MouseWheelEventArgs>& Control::eventMouseWheelGlobal() {
	return _eventMouseWheelGlobal;
}
Event<MouseEventArgs>& Control::eventClick() {
	return _eventClick;
}
Event<MouseEventArgs>& Control::eventDoubleClick() {
	return _eventDoubleClick;
}

// Keyboard
Event<KeyboardEventArgs>& Control::eventKey() {
	return _eventKey;
}
Event<KeyboardEventArgs>& Control::eventKeyGlobal() {
	return _eventKeyGlobal;
}
Event<KeyTypedEventArgs>& Control::eventKeyTyped() {
	return _eventKeyTyped;
}
Event<KeyTypedEventArgs>& Control::eventKeyTypedGlobal() {
	return _eventKeyTypedGlobal;
}

// Focus
Event<EventArgs>& Control::eventFocusGained() {
	return _eventFocusGained;
}
Event<EventArgs>& Control::eventFocusLost() {
	return _eventFocusLost;
}

// Commands
Event<CommandModifiedEventArgs>& Control::eventCommandAdded() {
	return commandCollection.eventCommandAdded();
}
Event<CommandModifiedEventArgs>& Control::eventCommandRemoved() {
	return commandCollection.eventCommandRemoved();
}

// Update
Event<TickEventArgs>& Control::eventTick() {
	return _eventTick;
}
Event<PaintEventArgs>& Control::eventPaint() {
	return _eventPaint;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Control::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	mousePositionLast = mousePosition;
	mousePosition = e.newPosition;
	bool mouseInsideLast = mouseInside;
	mouseInside = getLocalVisibleArea().containsPoint(e.newPosition);

	// Check for local mouse moved event
	if (mouseInside || mouseInsideLast) {
		_eventMouseMoved(sender, e);
	}

	// Check for mouse enter/leave events
	if (mouseInside && !mouseInsideLast) {
		_eventMouseEnter(sender, e);
	}
	else if (!mouseInside && mouseInsideLast) {
		_eventMouseLeave(sender, e);
	}
}
void Control::onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e) {
	// Update mouse button states used for local release events
	// Check for local mouse button event
	if (mouseInside && e.buttonState == PressedStates::Pressed) {
		mouseButtonStates[(int)e.buttonChanged - 1] = true;
		_eventMouseButton(sender, e);
	}
	else if (e.buttonState == PressedStates::Released && (mouseInside || mouseButtonStates[(int)e.buttonChanged - 1])) {
		bool pressedInside = mouseButtonStates[(int)e.buttonChanged - 1];
		mouseButtonStates[(int)e.buttonChanged - 1] = false;
		_eventMouseButton(sender, e);
		// Check for click event
		if (e.buttonChanged == MouseButtons::Left && pressedInside && mouseInside) {
			_eventClick(sender, e);
		}
	}
}
void Control::onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e) {
	// Check for local mouse wheel event
	if (mouseInside) {
		_eventMouseWheel(sender, e);
	}
}
void Control::onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e) {
	// Check for local key event
	if (hasFocus()) {
		_eventKey(sender, e);
	}
}
void Control::onKeyTypedGlobal(ControlSPtr sender, KeyTypedEventArgs& e) {
	// Check for local key typed event
	if (hasFocus()) {
		_eventKeyTyped(sender, e);
	}
}
void Control::onFocusGained(ControlSPtr sender, EventArgs& e) {
	focused = true;
}
void Control::onFocusLost(ControlSPtr sender, EventArgs& e) {
	focused = false;
}

#pragma endregion
//=================================================================|
