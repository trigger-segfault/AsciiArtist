/*=================================================================|
|* File:				FrameListBox.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FrameListBox.h"
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/Flags.h>

using namespace PowerConsole::Input;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiArtist::Toolbox;
using namespace AsciiArtist::Toolbox::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

FrameListBox::FrameListBox()
	:	// Items
	collection(),
	selection(-1),
	hoverIndex(-1),
	pressedIndex(-1),
	previewing(false),
	frameSpeedMode(false),

	// Events
	_eventSelectionChanged(EventDirections::FirstInFirstCalled),
	_eventPreviewChanged(EventDirections::FirstInFirstCalled),
	_eventSelectionDoubleClicked(EventDirections::FirstInFirstCalled) {

	eventInitialize().add(bindEvent(FrameListBox::onInitialize));
	eventPaint().add(bindEvent(FrameListBox::onPaint));
	eventMouseMoved().add(bindEvent(FrameListBox::onMouseMoved));
	eventMouseWheel().add(bindEvent(FrameListBox::onMouseWheel));
	eventMouseButtonGlobal().add(bindEvent(FrameListBox::onMouseButtonGlobal));
	eventDoubleClick().add(bindEvent(FrameListBox::onDoubleClick));
	eventKeyGlobal().add(bindEvent(FrameListBox::onKeyGlobal));
}
void FrameListBox::onInitialize(ControlSPtr sender, EventArgs& e) {
	collection.setOwner(shared_from_this());
	collection.eventFrameAdded().add(bindEvent(FrameListBox::onFrameAdded));
	collection.eventFrameRemoved().add(bindEvent(FrameListBox::onFrameRemoved));
	setPadding(Thickness(1));
	setIncrement(Point2I(2));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Selection
FrameInfo FrameListBox::getSelectedItem() const {
	if (selection == -1)
		return FrameInfo();
	return collection[selection];
}
int FrameListBox::getSelectedIndex() const {
	return selection;
}
void FrameListBox::setSelectedIndex(int index) {
	if (index < -1 && index >= collection.count())
		throw std::out_of_range("Invalid index in 'ListBox::setSelectedIndex(...)'");

	if (index == -1 && !collection.isEmpty())
		index = 0;
	if (index != selection) {
		int oldSelection = selection;
		selection = index;
		_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(selection, oldSelection));
	}
}
int FrameListBox::getPreviewIndex() const {
	return (previewing ? hoverIndex : -1);
}

// Frame Speed Mode
bool FrameListBox::getFrameSpeedMode() const {
	return frameSpeedMode;
}
void FrameListBox::setFrameSpeedMode(bool enabled) {
	frameSpeedMode = enabled;
}

// Items
FrameCollection& FrameListBox::items() {
	return collection;
}
const FrameCollection& FrameListBox::items() const {
	return collection;
}

#pragma endregion
//========== SCROLLING ===========
#pragma region Scrolling

void FrameListBox::scrollIntoView(int index) {
	int y = index - getViewPosition().y;
	int offset = GMath::min(getViewportSize().y / 2, 2);
	if (y - offset < 0) {
		setViewPosition(Point2I(0, GMath::max(y - offset, 0)), true);
	}
	else if (y + offset >= getViewportSize().y) {
		setViewPosition(Point2I(0, y + offset - getViewportSize().y + 1), true);
	}
}
void FrameListBox::scrollSelectionIntoView() {
	scrollIntoView(GMath::max(0, selection));
}
int FrameListBox::getItemIndexFromMousePosition(Point2I position) const {
	if (getLocalVisibleArea().containsPoint(position) && position >= getPadding().topLeft() && position - getPadding().topLeft() < getViewportSize()) {
		int index = position.y - getPadding().top + getViewPosition().y;
		return (index < collection.count() ? index : -1);
	}
	return -1;
}
void FrameListBox::updateScrollBars() {
	resizeScrollBars(Point2I(getActualSize().x, collection.count() + BOTTOM_SPACING));
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<SelectionChangedEventArgs>& FrameListBox::eventSelectionChanged() {
	return _eventSelectionChanged;
}
Event<SelectionChangedEventArgs>& FrameListBox::eventPreviewChanged() {
	return _eventPreviewChanged;
}
Event<EventArgs>& FrameListBox::eventSelectionDoubleClicked() {
	return _eventSelectionDoubleClicked;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void FrameListBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	// Start the region for the file list
	g->startRegion(getPadding().topLeft(), (getLocalVisibleArea() - getPadding().topLeft()).intersect(getViewportSize()));

	unsigned char itemColor = toColor(BLACK, GRAY);
	unsigned char itemHoverColor = toColor(BLACK, WHITE);
	unsigned char itemPressColor = toColor(BLACK, DARK_GRAY);
	unsigned char selectionColor = toColor(BLUE, WHITE);
	unsigned char selectionHoverColor = toColor(BLUE, GRAY);
	unsigned char selectionPressColor = toColor(BLUE, DARK_GRAY);
	unsigned char previewColor = toColor(DARK_RED, WHITE);
	unsigned char previewHoverColor = toColor(DARK_RED, GRAY);
	unsigned char previewPressColor = toColor(DARK_RED, DARK_GRAY);

	unsigned char color;
	for (int i = getViewPosition().y; i < getViewPosition().y + getViewportSize().y && i < collection.count(); i++) {
		color = itemColor;
		if (i == selection) {
			color = selectionColor;
			if (i == pressedIndex)
				color = selectionPressColor;
			else if (i == hoverIndex && pressedIndex == -1)
				color = selectionHoverColor;
		}
		else if (previewing && i == hoverIndex) {
			color = previewColor;
			if (i == pressedIndex)
				color = previewPressColor;
			else
				color = previewHoverColor;
		}
		else if (i == pressedIndex) {
			color = itemPressColor;
		}
		else if (i == hoverIndex && pressedIndex == -1) {
			color = itemHoverColor;
		}
		std::string name;
		if (frameSpeedMode)
			name = std::to_string(collection[i].frameSpeed) + "ms (" + std::to_string(i + 1) + ")";
		else
			name = (collection[i].name.empty() ? "frame " + std::to_string(i + 1) : collection[i].name);
		g->drawString(Point2I(1, i - getViewPosition().y), name, Pixel(' ', color));
	}

	g->endRegion();
}
void FrameListBox::onFrameAdded(ControlSPtr sender, FrameModifiedEventArgs& e) {
	updateScrollBars();
	scrollIntoView(e.index);
	onMouseMoved(shared_from_this(), MouseEventArgs(getMousePosition(), getMousePositionLast()));
}
void FrameListBox::onFrameRemoved(ControlSPtr sender, FrameModifiedEventArgs& e) {
	if (collection.isEmpty()) {
		updateScrollBars();
		onMouseMoved(shared_from_this(), MouseEventArgs(getMousePosition(), getMousePositionLast()));
	}
	else {
		updateScrollBars();
	}
}
void FrameListBox::onMouseMoved(ControlSPtr sender, MouseEventArgs& e) {
	int oldHoverIndex = hoverIndex;
	hoverIndex = getItemIndexFromMousePosition(e.newPosition);
	if (oldHoverIndex != hoverIndex && previewing) {
		_eventPreviewChanged(shared_from_this(), SelectionChangedEventArgs(hoverIndex, oldHoverIndex));
	}
}
void FrameListBox::onMouseWheel(ControlSPtr sender, MouseWheelEventArgs& e) {
	onMouseMoved(sender, MouseEventArgs(e.newPosition, e.previousPosition));
}
void FrameListBox::onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (hoverIndex != -1) {
			pressedIndex = hoverIndex;
			int oldSelection = selection;
			if (pressedIndex != selection) {
				selection = hoverIndex;
				_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(selection, oldSelection));
			}
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left)) {
		pressedIndex = -1;
	}
}
void FrameListBox::onDoubleClick(ControlSPtr sender, MouseEventArgs& e) {
	if (getItemIndexFromMousePosition(e.newPosition) != -1) {
		_eventSelectionDoubleClicked(shared_from_this());
	}
}
void FrameListBox::onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e) {
	bool oldPreviewing = previewing;
	previewing = getFlag(e.modifiers, Modifiers::Shift);
	if (oldPreviewing != previewing && hoverIndex != -1) {
		_eventPreviewChanged(shared_from_this(), SelectionChangedEventArgs(
			previewing ? hoverIndex : -1,
			oldPreviewing ? hoverIndex : -1
		));
	}
}

#pragma endregion
//=================================================================|
