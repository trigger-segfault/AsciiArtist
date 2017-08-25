/*=================================================================|
|* File:				ControlCollection.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONTROL_COLLECTION_H
#define CONTROL_COLLECTION_H

#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Controls/Events/EventArgs.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The collection of controls used by a container. */
class ControlCollection {

	//=========== MEMBERS ============
	#pragma region Members

	// Container
	/* The owner for this collection. */
	ControlWPtr owner;
	/* The container for the child controls. */
	std::vector<ControlSPtr> children;

	// Events
	/* Called when a child has been added to the collection. */
	Event<ChildModifiedEventArgs> _eventChildAdded;
	/* Called when a child has been removed from the collection. */
	Event<ChildModifiedEventArgs> _eventChildRemoved;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constucts the control collection. */
	ControlCollection();
	/* Deconstructs the control collection. */
	~ControlCollection();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Sets the owner of the control collection. This should only ever be called by the owner. */
	void setOwner(ControlSPtr owner);

	/* Gets the number of controls in the collection. */
	int count() const;
	/* Gets if the collection has no controls in it. */
	bool isEmpty() const;

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Gets the control at the specified index. */
	ControlSPtr operator[](int index);
	/* Gets the control at the specified index. */
	const ControlSPtr operator[](int index) const;
	/* Gets the control with the specified name. */
	ControlSPtr operator[](const std::string& name);
	/* Gets the control with the specified name. */
	const ControlSPtr operator[](const std::string& name) const;

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Iterates through all controls contained within this one. */
	void forEach(std::function<void(ControlSPtr control)> func);

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the control at the specified index. */
	ControlSPtr at(int index);
	/* Gets the control at the specified index. */
	const ControlSPtr at(int index) const;
	/* Gets the control with the specified name. */
	ControlSPtr at(const std::string& name);
	/* Gets the control with the specified name. */
	const ControlSPtr at(const std::string& name) const;
	/* Adds the specified control to the collection. */
	int add(ControlSPtr control);
	/* Inserts the specified control at the specified index in the collection. */
	void insert(int index, ControlSPtr control);
	/* Removes the specified control from the collection. */
	int remove(ControlSPtr control);
	/* Removes the control with the specified name from the collection. */
	int remove(const std::string& name);
	/* Removes the control at the specified index from the collection. */
	void removeAt(int index);
	/* Removes all controls from the collection. */
	void clear();

	#pragma endregion
	//========== SEARCHING ===========
	#pragma region Searching

	/* Returns true if the specified control is in the collection. */
	bool contains(ControlCSPtr control) const;
	/* Returns true if a control with the specified name is in the collection. */
	bool contains(const std::string& name) const;

	/* Returns the index of the specified control. Returns -1 if the control does not exist. */
	int indexOf(ControlCSPtr control) const;
	/* Returns the index of the control with the specified name. Returns -1 if the control does not exist. */
	int indexOf(const std::string& name) const;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when a child has been added to the collection. */
	Event<ChildModifiedEventArgs>& eventChildAdded();
	/* Called when a child has been removed from the collection. */
	Event<ChildModifiedEventArgs>& eventChildRemoved();

	#pragma endregion
	//========== ITERATORS ===========
	#pragma region Iterators

	typedef typename std::vector<ControlSPtr>::const_iterator const_iterator;

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
