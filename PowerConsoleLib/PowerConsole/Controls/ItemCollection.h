/*=================================================================|
|* File:				ItemCollection.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ITEM_COLLECTION_H
#define ITEM_COLLECTION_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ItemCollection {

	//=========== MEMBERS ============
	#pragma region Members

	// Container
	/* The owner for this collection. */
	ControlWPtr owner;
	/* The container for the items. */
	std::vector<std::string> items;

	// Events
	/* Called when an item has been added to the collection. */
	Event<ItemModifiedEventArgs> _eventItemAdded;
	/* Called when an item has been removed from the collection. */
	Event<ItemModifiedEventArgs> _eventItemRemoved;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constucts the item collection. */
	ItemCollection();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Sets the owner of the item collection. This should only ever be called by the owner. */
	void setOwner(ControlSPtr owner);

	/* Gets the number of items in the collection. */
	int count() const;
	/* Gets if the collection has no items in it. */
	bool isEmpty() const;

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Gets the item at the specified index. */
	std::string& operator[](int index);
	/* Gets the item at the specified index. */
	const std::string& operator[](int index) const;

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the item at the specified index. */
	std::string& at(int index);
	/* Gets the item at the specified index. */
	const std::string& at(int index) const;
	/* Adds the specified item to the collection. */
	int add(const std::string& item);
	/* Inserts the specified item at the specified index in the collection. */
	void insert(int index, const std::string& item);
	/* Removes the specified item from the collection. */
	int remove(const std::string& item);
	/* Removes the item at the specified index from the collection. */
	void removeAt(int index);
	/* Removes all items from the collection. */
	void clear();

	// Moving
	/* Swaps the specified items. */
	void swap(int indexA, int indexB);
	/* Moves the specified item though the list. */
	void move(int index, int newIndex, bool relative = false);

	#pragma endregion
	//========== SEARCHING ===========
	#pragma region Searching

	/* Returns true if the specified item is in the collection. */
	bool contains(const std::string& item) const;

	/* Returns the index of the specified item. Returns -1 if the control does not exist. */
	int indexOf(const std::string& item) const;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when an item has been added to the collection. */
	Event<ItemModifiedEventArgs>& eventItemAdded();
	/* Called when an item has been removed from the collection. */
	Event<ItemModifiedEventArgs>& eventItemRemoved();

	#pragma endregion
	//========== ITERATORS ===========
	#pragma region Iterators

	typedef typename std::vector<std::string>::const_iterator const_iterator;

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
