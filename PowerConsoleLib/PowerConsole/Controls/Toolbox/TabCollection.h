/*=================================================================|
|* File:				TabCollection.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TAB_COLLECTION_H
#define TAB_COLLECTION_H

#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Controls/Events/EventArgs.h>
#include <PowerConsole/Controls/Toolbox/Tab.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The collection of tabs used by a tab control. */
class TabCollection {

	//=========== MEMBERS ============
	#pragma region Members

	// Container
	/* The owner for this collection. */
	ControlWPtr owner;
	/* The container for the tabs. */
	std::vector<TabSPtr> tabs;

	// Events
	/* Called when a child has been added to the collection. */
	Event<ChildModifiedEventArgs> _eventChildAdded;
	/* Called when a child has been removed from the collection. */
	Event<ChildModifiedEventArgs> _eventChildRemoved;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constucts the tab collection. */
	TabCollection();
	/* Deconstructs the tab collection. */
	~TabCollection();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Sets the owner of the tab collection. This should only ever be called by the owner. */
	void setOwner(ControlSPtr owner);

	/* Gets the number of tabs in the collection. */
	int count() const;
	/* Gets if the collection has no tabs in it. */
	bool isEmpty() const;

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Gets the tab at the specified index. */
	TabSPtr operator[](int index);
	/* Gets the tab at the specified index. */
	const TabSPtr operator[](int index) const;

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Iterates through all controls contained within this one. */
	void forEach(std::function<void(ControlSPtr control)> func);

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the tab at the specified index. */
	TabSPtr at(int index);
	/* Gets the tab at the specified index. */
	const TabSPtr at(int index) const;
	/* Adds the specified tab to the collection. */
	int add(TabSPtr control);
	/* Inserts the specified tab at the specified index in the collection. */
	void insert(int index, TabSPtr control);
	/* Removes the specified tab from the collection. */
	int remove(TabSPtr control);
	/* Removes the tab at the specified index from the collection. */
	void removeAt(int index);
	/* Removes all tabs from the collection. */
	void clear();

	#pragma endregion
	//========== SEARCHING ===========
	#pragma region Searching

	/* Returns true if the specified tab is in the collection. */
	bool contains(TabCSPtr control) const;

	/* Returns the index of the specified tab. Returns -1 if the tab does not exist in the collection. */
	int indexOf(TabCSPtr control) const;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when a tab has been added to the collection. */
	Event<ChildModifiedEventArgs>& eventChildAdded();
	/* Called when a tab has been removed from the collection. */
	Event<ChildModifiedEventArgs>& eventChildRemoved();

	#pragma endregion
	//========== ITERATORS ===========
	#pragma region Iterators

	typedef typename std::vector<TabSPtr>::const_iterator const_iterator;

	/* Returns the constant beginning iterator of the container. */
	const_iterator begin() const;
	/* Returns the constant ending iterator of the container. */
	const_iterator end() const;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
