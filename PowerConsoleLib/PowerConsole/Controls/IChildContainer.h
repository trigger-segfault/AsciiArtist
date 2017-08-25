/*=================================================================|
|* File:				IChildContainer.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef I_CHILD_CONTAINER_H
#define I_CHILD_CONTAINER_H

#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Controls/Events/EventArgs.h>
#include <PowerConsole/Controls/ControlCollection.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The basic interface for a control container. */
class IChildContainer {

	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the collection of all the children contained by this control. */
	virtual ControlCollection& children() = 0;
	/* Gets the collection of all the children contained by this control. */
	virtual const ControlCollection& children() const = 0;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when a child has been added to the collection. */
	virtual Event<ChildModifiedEventArgs>& eventChildAdded() = 0;
	/* Called when a child has been removed from the collection. */
	virtual Event<ChildModifiedEventArgs>& eventChildRemoved() = 0;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
