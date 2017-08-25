/*=================================================================|
|* File:				ContentControl.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONTENT_CONTROL_H
#define CONTENT_CONTROL_H

#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Controls/IContentContainer.h>
#include <PowerConsole/Controls/ControlCollection.h>
#include <PowerConsole/Controls/Events/EventArgs.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The base class for containing a single control. */
class ContentControl : public Control, public IContentContainer {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The content for this container. */
	ControlSPtr content;

	// Events
	/* Called when the content for the control has been set. */
	Event<ContentModifiedEventArgs> _eventContentAdded;
	/* Called when the content has been removed from the control. */
	Event<ContentModifiedEventArgs> _eventContentRemoved;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the base content control. */
	ContentControl();
	/* Deconstructs the base content control. */
	~ContentControl();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Returns true if the control or any of its children has the specified name. */
	bool containsName(const std::string& name) const override;

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Sets the layout of the content. */
	virtual void updateContentLayout();

	/* Iterates through all controls contained within this one. */
	void forEach(std::function<void(ControlSPtr control)> func) override;
	/* Draws the control to the graphics object. */
	void repaint(PcDraw::GraphicsSPtr g) override;

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the content for the container. */
	ControlSPtr getContent() override;
	/* Sets the specified content for the container. */
	void setContent(ControlSPtr control) override;
	/* Removes the content from the container. */
	void removeContent() override;

	/* Returns true if this control has any content. */
	bool hasContent() const override;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the content for the control has been set. */
	Event<ContentModifiedEventArgs>& eventContentAdded() override;
	/* Called when the content has been removed from the control. */
	Event<ContentModifiedEventArgs>& eventContentRemoved() override;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onContentAdded(ControlSPtr sender, ContentModifiedEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
