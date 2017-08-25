/*=================================================================|
|* File:				FrameCollection.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FRAME_COLLECTION_H
#define FRAME_COLLECTION_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>
#include <AsciiArtist/Toolbox/AsciiArtistEventArgs.h>

using namespace PowerConsole::Controls;

namespace AsciiArtist::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class FrameCollection {

	//=========== MEMBERS ============
	#pragma region Members

	// Container
	/* The owner for this collection. */
	ControlWPtr owner;
	/* The container for the items. */
	std::vector<FrameInfo> items;

	// Events
	/* Called when an item has been added to the collection. */
	Event<FrameModifiedEventArgs> _eventFrameAdded;
	/* Called when an item has been removed from the collection. */
	Event<FrameModifiedEventArgs> _eventFrameRemoved;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constucts the item collection. */
	FrameCollection();

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
	FrameInfo& operator[](int index);
	/* Gets the item at the specified index. */
	const FrameInfo& operator[](int index) const;

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the item at the specified index. */
	FrameInfo& at(int index);
	/* Gets the item at the specified index. */
	const FrameInfo& at(int index) const;
	/* Adds the specified item to the collection. */
	int add(const FrameInfo& item);
	/* Inserts the specified item at the specified index in the collection. */
	void insert(int index, const FrameInfo& item);
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
	//============ EVENTS ============
	#pragma region Events

	/* Called when an item has been added to the collection. */
	Event<FrameModifiedEventArgs>& eventFrameAdded();
	/* Called when an item has been removed from the collection. */
	Event<FrameModifiedEventArgs>& eventFrameRemoved();

	#pragma endregion
	//========== ITERATORS ===========
	#pragma region Iterators

	typedef typename std::vector<FrameInfo>::const_iterator const_iterator;

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
