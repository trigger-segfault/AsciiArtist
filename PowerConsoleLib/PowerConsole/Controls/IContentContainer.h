/*=================================================================|
|* File:				IContentContainer.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef I_CONTENT_CONTAINER_H
#define I_CONTENT_CONTAINER_H

#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Controls/Events/EventArgs.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The basic interface for a content container. */
class IContentContainer {

	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the content for the container. */
	virtual ControlSPtr getContent() = 0;
	/* Sets the specified content for the container. */
	virtual void setContent(ControlSPtr control) = 0;
	/* Removes the content from the container. */
	virtual void removeContent() = 0;

	/* Returns true if this control has any content. */
	virtual bool hasContent() const = 0;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the content for the control has been set. */
	virtual Event<ContentModifiedEventArgs>& eventContentAdded() = 0;
	/* Called when the content has been removed from the control. */
	virtual Event<ContentModifiedEventArgs>& eventContentRemoved() = 0;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
