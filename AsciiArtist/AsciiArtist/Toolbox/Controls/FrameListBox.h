/*=================================================================|
|* File:				FrameListBox.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FRAME_LIST_BOX_H
#define FRAME_LIST_BOX_H

#include <PowerConsole/Controls/Toolbox/ScrollBars.h>
#include <AsciiArtist/Toolbox/FrameCollection.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiArtist::Toolbox::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

class FrameListBox : public ScrollBars {

	//========== CONSTANTS ===========
	#pragma region Constants

	/* The amount of line spacing to leave at the bottom for scrolling. */
	static const int BOTTOM_SPACING = 2;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Items
	/* The collection of items. */
	FrameCollection collection;
	/* The currently selected list item. -1 if there is no selection. */
	int selection;
	/* The index of the selection currently being hovered over. */
	int hoverIndex;
	/* The index that has been pressed down by the mouse. */
	int pressedIndex;
	/* True if the hover index is being previewed. */
	bool previewing;
	/* True if frame speeds are shown instead of names. */
	bool frameSpeedMode;

	// Events
	/* Called when the selection of the list has changed. */
	Event<SelectionChangedEventArgs> _eventSelectionChanged;
	/* Called when the preview of the list has changed. */
	Event<SelectionChangedEventArgs> _eventPreviewChanged;
	/* Called when the selection has been double clicked. */
	Event<EventArgs> _eventSelectionDoubleClicked;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	FrameListBox();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Selection
	/* Gets the currently selected list item. Returns "" if there is no selection. */
	FrameInfo getSelectedItem() const;
	/* Gets the index currently selected list item. Returns -1 if there is no selection. */
	int getSelectedIndex() const;
	/* Sets the index currently selected list item. Use -1 to unset any selection. */
	void setSelectedIndex(int index);
	/* Gets the index currently previewed list item. Returns -1 if there is no preview. */
	int getPreviewIndex() const;

	// Frame Speed Mode
	/* Gets if the list is showing frame speeds. */
	bool getFrameSpeedMode() const;
	/* Sets if the list is showing frame speeds. */
	void setFrameSpeedMode(bool enabled);

	// Items
	/* Gets the collection of items contained by this control. */
	FrameCollection& items();
	/* Gets the collection of items contained by this control. */
	const FrameCollection& items() const;

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
	/* Called when the selection of the list has changed. */
	Event<SelectionChangedEventArgs>& eventSelectionChanged();
	/* Called when the preview of the list has changed. */
	Event<SelectionChangedEventArgs>& eventPreviewChanged();
	/* Called when the selection has been double clicked. */
	Event<EventArgs>& eventSelectionDoubleClicked();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onFrameAdded(ControlSPtr sender, FrameModifiedEventArgs& e);
	void onFrameRemoved(ControlSPtr sender, FrameModifiedEventArgs& e);
	void onMouseMoved(ControlSPtr sender, MouseEventArgs& e);
	void onMouseWheel(ControlSPtr sender, MouseWheelEventArgs& e);
	void onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e);
	void onDoubleClick(ControlSPtr sender, MouseEventArgs& e);
	void onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e);

	#pragma endregion

};

DECLARE_SMART_PTRS(FrameListBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|

