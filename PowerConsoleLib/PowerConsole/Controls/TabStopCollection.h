/*=================================================================|
|* File:				TabStopCollection.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TAB_STOP_MANAGER_H
#define TAB_STOP_MANAGER_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

class TabStopCollection {

	//=========== MEMBERS ============
	#pragma region Members

	/* The owner of this manager. */
	WindowWPtr owner;

	/* The collection of registered tab stops. */
	std::vector<ControlSPtr> tabStops;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tab stop manager. */
	TabStopCollection();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Sets the owner of the tab stop manager. This should only ever be called by the owner. */
	void setOwner(WindowSPtr owner);

	/* Gets the next tab stop based on the window's current focus. */
	ControlSPtr getNextTabStop();
	/* Returns true if the current focus is a tab stop. */
	bool isFocusTabStop() const;
	/* Gets the current tab stop index. -1 if no registered tab stop is focused. */
	int getIndex() const;
	/* Gets the number of tab stops. */
	int count() const;
	/* Gets if the collection has no tab stops in it. */
	bool isEmpty() const;

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Gets the tab stop at the specified index. */
	ControlSPtr operator[](int index);
	/* Gets the tab stop at the specified index. */
	const ControlSPtr operator[](int index) const;

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the tab stop at the specified index. */
	ControlSPtr at(int index);
	/* Gets the tab stop at the specified index. */
	const ControlSPtr at(int index) const;
	/* Adds the specified control to the tab stop collection. */
	int add(ControlSPtr control);
	/* Inserts the specified control at the specified index in the tab stop collection. */
	void insert(int index, ControlSPtr control);
	/* Removes the specified control from the tab stop collection. */
	int remove(ControlSPtr control);
	/* Removes the control at the specified index from the tab stop collection. */
	void removeAt(int index);
	/* Removes all tab stops from the collection. */
	void clear();

	#pragma endregion
	//========== SEARCHING ===========
	#pragma region Searching

	/* Returns true if the specified tab stop is in the collection. */
	bool contains(ControlCSPtr control) const;

	/* Returns the index of the specified tab stop. Returns -1 if the control does not exist. */
	int indexOf(ControlCSPtr control) const;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events


	#pragma endregion

};

DECLARE_SMART_PTRS(TabStopCollection);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
