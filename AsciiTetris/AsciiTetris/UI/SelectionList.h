/*=================================================================|
|* File:				SelectionList.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SELECTION_LIST_H
#define SELECTION_LIST_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Drawing;

namespace AsciiTetris::UI {
//=================================================================|
// CLASSES														   |
//=================================================================/

struct SelectionItem {

	//=========== MEMBERS ============
	#pragma region Members

	/* The text of the item. */
	std::string text;
	/* The color of the item. */
	unsigned char color;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the selection item. */
	inline SelectionItem()
		: text(""), color(BLACK) {}
	/* Constructs the selection item. */
	inline SelectionItem(const std::string& text, unsigned char color)
		: text(text), color(toColor(darkenFColor(color), color)) {}

	#pragma endregion
};

class SelectionList : public Control {

	//========== CONSTANTS ===========
	#pragma region Constants

	/* The animation speed for the selection timer flashing. */
	static const int ANIMATION_LENGTH = 400 / (1000 / 60);

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* The list of items. */
	std::vector<SelectionItem> items;

	/* The current item selection. */
	int selection;
	/* The timer for the text. */
	int timer;

	// Events
	/* Called when an item is selected. */
	Event<EventArgs> _eventItemSelected;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	SelectionList();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the current selection. */
	int getSelectedIndex() const;

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Adds a selection item to the list. */
	void addItem(const std::string& text, unsigned char color);
	/* Renames a selection item in the list. */
	void renameItem(int index, const std::string& newText);

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const final;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when an item is selected. */
	Event<EventArgs>& eventItemSelected();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onKey(ControlSPtr sender, KeyboardEventArgs& e);
	void onKeyTyped(ControlSPtr sender, KeyTypedEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(SelectionList);


//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
