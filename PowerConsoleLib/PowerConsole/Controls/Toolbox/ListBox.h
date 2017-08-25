/*=================================================================|
|* File:				ListBox.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef LIST_BOX_H
#define LIST_BOX_H

#include <PowerConsole/Controls/Toolbox/ScrollBars.h>
#include <PowerConsole/Controls/ItemCollection.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ListBox : public ScrollBars {

	//=========== MEMBERS ============
	#pragma region Members
private:
	// Items
	/* The collection of items. */
	ItemCollection collection;
	/* The currently selected list item. -1 if there is no selection. */
	int selection;
	/* The index of the selection currently being hovered over. */
	int hoverIndex;
	/* The index that has been pressed down by the mouse. */
	int pressedIndex;

	// Settings
	/* True if no having no selection is allowed. */
	bool allowNoSelection;
	/* True if arrow key navigation is allowed. */
	bool allowArrowNavigation;
	/* The amount of line spacing to leave at the bottom for scrolling. */
	int bottomSpacing;
	/* True if the listbox automatically scrolls the added item into view. */
	bool scrollAddedIntoView;

	// Events
	/* Called when the selection of the list has changed. */
	Event<SelectionChangedEventArgs> _eventSelectionChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	ListBox();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Selection
	/* Gets the currently selected list item. Returns "" if there is no selection. */
	std::string getSelectedItem() const;
	/* Gets the index currently selected list item. Returns -1 if there is no selection. */
	int getSelectedIndex() const;
	/* Sets the index currently selected list item. Use -1 to unset any selection. */
	void setSelectedIndex(int index);
	/* Gets if no having no selection is allowed. */
	bool isNoSelectionAllowed() const;
	/* Sets if no having no selection is allowed. */
	void setNoSelectionAllowed(bool allowed);
	/* Gets if arrow key navigation is allowed. */
	bool isArrowNavigationAllowed() const;
	/* Sets if arrow key navigation is allowed. */
	void setArrowNavigationAllowed(bool allowed);
	/* Gets the amount of line spacing to leave at the bottom for scrolling. */
	int getBottomSpacing() const;
	/* Sets the amount of line spacing to leave at the bottom for scrolling. */
	void setBottomSpacing(int bottomSpacing);
	/* Gets if the listbox automatically scrolls the added item into view. */
	bool isAddedItemScrolledIntoView() const;
	/* Gets if the listbox automatically scrolls the added item into view. */
	void setAddedItemScrolledIntoView(bool scrollAddedIntoView);

	// Items
	/* Gets the collection of items contained by this control. */
	ItemCollection& items();
	/* Gets the collection of items contained by this control. */
	const ItemCollection& items() const;

	#pragma endregion
	//========== SCROLLING ===========
	#pragma region Scrolling

	/* Scrolls the specified index into view. */
	void scrollIntoView(int index);
	/* Scrolls the selection into view. */
	void scrollSelectionIntoView();
	/* Gets the item being hovered over by the mouse position. */
	int getItemIndexFromMousePosition(PcGeo::Point2I position) const;
private:
	/* Updates the dimensions of the scrollbar after a property has been changed. */
	void updateScrollBars();

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
public:
	/* Called when an item has been added to the collection. */
	Event<ItemModifiedEventArgs>& eventItemAdded();
	/* Called when an item has been removed from the collection. */
	Event<ItemModifiedEventArgs>& eventItemRemoved();
	/* Called when the selection of the list has changed. */
	Event<SelectionChangedEventArgs>& eventSelectionChanged();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onItemAdded(ControlSPtr sender, ItemModifiedEventArgs& e);
	void onItemRemoved(ControlSPtr sender, ItemModifiedEventArgs& e);
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);
	void onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e);
	void onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e);

	#pragma endregion
	//======= COMMAND HANDLERS =======
	#pragma region Command Handlers

	void onUpCommand(ControlSPtr sender, EventArgs& e);
	void onDownCommand(ControlSPtr sender, EventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(ListBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
