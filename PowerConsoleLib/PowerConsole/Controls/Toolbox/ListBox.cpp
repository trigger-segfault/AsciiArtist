/*=================================================================|
|* File:				ListBox.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ListBox.h"
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Input;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ListBox::ListBox() 
 :	// Items
	collection(),
	selection(-1),
	hoverIndex(-1),
	pressedIndex(-1),

	// Settings
	allowNoSelection(true),
	allowArrowNavigation(false),
	bottomSpacing(0),

	// Events
	_eventSelectionChanged(EventDirections::FirstInFirstCalled) {

	eventInitialize().add(bindEvent(ListBox::onInitialize));
	eventPaint().add(bindEvent(ListBox::onPaint));
	eventItemAdded().add(bindEvent(ListBox::onItemAdded));
	eventItemRemoved().add(bindEvent(ListBox::onItemRemoved));
	eventMouseMovedGlobal().add(bindEvent(ListBox::onMouseMovedGlobal));
	eventMouseWheelGlobal().add(bindEvent(ListBox::onMouseWheelGlobal));
	eventMouseButtonGlobal().add(bindEvent(ListBox::onMouseButtonGlobal));

	commands().add(Command("Up", Hotkey(Keys::Up, true), true, bindEvent(ListBox::onUpCommand)));
	commands().add(Command("Down", Hotkey(Keys::Down, true), true, bindEvent(ListBox::onDownCommand)));
}
void ListBox::onInitialize(ControlSPtr sender, EventArgs& e) {
	collection.setOwner(shared_from_this());
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Selection
std::string ListBox::getSelectedItem() const {
	if (selection == -1)
		return "";
	return collection[selection];
}
int ListBox::getSelectedIndex() const {
	return selection;
}
void ListBox::setSelectedIndex(int index) {
	if (index < -1 && index >= collection.count())
		throw std::out_of_range("Invalid index in 'ListBox::setSelectedIndex(...)'");

	if (index == -1 && !allowNoSelection && !collection.isEmpty())
		index = 0;
	if (index != selection) {
		int oldSelection = selection;
		selection = index;
		_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(selection, oldSelection));
	}
}
bool ListBox::isNoSelectionAllowed() const {
	return allowNoSelection;
}
void ListBox::setNoSelectionAllowed(bool allowed) {
	this->allowNoSelection = allowed;
	if (!allowed && selection == -1 && !collection.isEmpty()) {
		selection = 0;
		_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(0, -1));
	}
}
bool ListBox::isArrowNavigationAllowed() const {
	return allowArrowNavigation;
}
void ListBox::setArrowNavigationAllowed(bool allowed) {
	this->allowArrowNavigation = allowed;
}
int ListBox::getBottomSpacing() const {
	return bottomSpacing;
}
void ListBox::setBottomSpacing(int bottomSpacing) {
	this->bottomSpacing = bottomSpacing;
	updateScrollBars();
}
bool ListBox::isAddedItemScrolledIntoView() const {
	return scrollAddedIntoView;
}
void ListBox::setAddedItemScrolledIntoView(bool scrollAddedIntoView) {
	this->scrollAddedIntoView = scrollAddedIntoView;
}

// Items
ItemCollection& ListBox::items() {
	return collection;
}
const ItemCollection& ListBox::items() const {
	return collection;
}

#pragma endregion
//========== SCROLLING ===========
#pragma region Scrolling

void ListBox::scrollIntoView(int index) {
	int y = index - getViewPosition().y;
	int offset = GMath::min(getViewportSize().y / 2, 2);
	if (y - offset < 0) {
		setViewPosition(Point2I(0, GMath::max(y - offset, 0)), true);
	}
	else if (y + offset >= getViewportSize().y) {
		setViewPosition(Point2I(0, y + offset - getViewportSize().y + 1), true);
	}
}
void ListBox::scrollSelectionIntoView() {
	scrollIntoView(GMath::max(0, selection));
}
int ListBox::getItemIndexFromMousePosition(Point2I position) const {
	if (getLocalVisibleArea().containsPoint(position) && position >= getPadding().topLeft() && position - getPadding().topLeft() < getViewportSize()) {
		int index = position.y - getPadding().top + getViewPosition().y;
		return (index < collection.count() ? index : -1);
	}
	return -1;
}
void ListBox::updateScrollBars() {
	resizeScrollBars(Point2I(getActualSize().x, collection.count() + bottomSpacing));
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<ItemModifiedEventArgs>& ListBox::eventItemAdded() {
	return collection.eventItemAdded();
}
Event<ItemModifiedEventArgs>& ListBox::eventItemRemoved() {
	return collection.eventItemRemoved();
}
Event<SelectionChangedEventArgs>& ListBox::eventSelectionChanged() {
	return _eventSelectionChanged;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ListBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	g->startRegion(getPadding().topLeft(), (getLocalVisibleArea() - getPadding().topLeft()).intersect(getViewportSize()));

	unsigned char itemColor = toColor(BLACK, GRAY);
	unsigned char itemHoverColor = toColor(BLACK, WHITE);
	unsigned char itemPressColor = toColor(BLACK, DARK_GRAY);
	unsigned char itemDisabledColor = toColor(BLACK, DARK_GRAY);
	unsigned char selectionColor = toColor(BLUE, WHITE);
	unsigned char selectionHoverColor = toColor(BLUE, GRAY);
	unsigned char selectionPressColor = toColor(BLUE, DARK_GRAY);
	unsigned char selectionDisabledColor = toColor(DARK_GRAY, GRAY);

	unsigned char color;
	for (int i = getViewPosition().y; i < getViewPosition().y + getViewportSize().y && i < collection.count(); i++) {
		color = itemColor;
		if (isEnabled()) {
			if (i == selection) {
				color = selectionColor;
				if (i == pressedIndex)
					color = selectionPressColor;
				else if (i == hoverIndex && pressedIndex == -1)
					color = selectionHoverColor;
			}
			else if (i == pressedIndex) {
				color = itemPressColor;
			}
			else if (i == hoverIndex && pressedIndex == -1) {
				color = itemHoverColor;
			}
		}
		else {
			if (i == selection)
				color = selectionDisabledColor;
			else
				color = itemDisabledColor;
		}
		g->drawString(Point2I(1, i - getViewPosition().y), collection[i], Pixel(' ', color));
	}

	g->endRegion();
}
void ListBox::onItemAdded(ControlSPtr sender, ItemModifiedEventArgs& e) {
	int oldSelection = selection;
	if (!allowNoSelection && selection == -1)
		selection = 0;
	updateScrollBars();
	if (scrollAddedIntoView)
		scrollIntoView(e.index);
	if (!allowNoSelection && oldSelection == -1)
		_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(0, -1));
}
void ListBox::onItemRemoved(ControlSPtr sender, ItemModifiedEventArgs& e) {
	if (collection.isEmpty()) {
		int oldSelection = selection;
		selection = -1;
		updateScrollBars();
		_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(-1, oldSelection));
	}
	else {
		updateScrollBars();
	}
}
void ListBox::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	hoverIndex = getItemIndexFromMousePosition(e.newPosition);
}
void ListBox::onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e) {
	onMouseMovedGlobal(sender, MouseEventArgs(e.newPosition, e.previousPosition));
}
void ListBox::onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (hoverIndex != -1) {
			pressedIndex = hoverIndex;
			int oldSelection = selection;
			if (pressedIndex != selection) {
				selection = hoverIndex;
				_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(selection, oldSelection));
			}
			else if (allowNoSelection) {
				selection = -1;
				_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(selection, oldSelection));
			}
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left)) {
		pressedIndex = -1;
	}
}

#pragma endregion
//======= COMMAND HANDLERS =======
#pragma region Command Handlers

void ListBox::onUpCommand(ControlSPtr sender, EventArgs& e) {
	if (allowArrowNavigation && !items().isEmpty()) {
		if (selection < 1) {
			setSelectedIndex(items().count() - 1);
		}
		else {
			setSelectedIndex(selection - 1);
		}
	}
}
void ListBox::onDownCommand(ControlSPtr sender, EventArgs& e) {
	if (allowArrowNavigation && !items().isEmpty()) {
		if (selection == -1 || selection + 1 == items().count()) {
			setSelectedIndex(0);
		}
		else {
			setSelectedIndex(selection + 1);
		}
	}
}

#pragma endregion
//=================================================================|
