/*=================================================================|
|* File:				SplitContainer.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SPLIT_CONTAINER_H
#define SPLIT_CONTAINER_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class SplitContainer : public Control {

	//=========== MEMBERS ============
	#pragma region Members
private:
	// Resizing
	/* The position of the separator from the left side. */
	int separator;
	/* The position of the separator from the right side. */
	//int rightSeparator;
	/* True if the separator is being hovered over. */
	bool hovering;
	/* True if the separator is being dragged. */
	bool dragging;
	/* The minimum width of the left content. */
	int leftMinWidth;
	/* The maximum width of the left content. */
	int leftMaxWidth;
	/* The minimum width of the right content. */
	int rightMinWidth;
	/* The maximum width of the right content. */
	int rightMaxWidth;
	/* True if the right content is fixed. */
	bool rightFixed;

	// Content
	/* The left content for this container. */
	ControlSPtr leftContent;
	/* The right content for this container. */
	ControlSPtr rightContent;

	// Events
	/* Called when the left content for the control has been set. */
	Event<ContentModifiedEventArgs> _eventLeftContentAdded;
	/* Called when the right content for the control has been set. */
	Event<ContentModifiedEventArgs> _eventRightContentAdded;
	/* Called when the left content has been removed from the control. */
	Event<ContentModifiedEventArgs> _eventLeftContentRemoved;
	/* Called when the right content has been removed from the control. */
	Event<ContentModifiedEventArgs> _eventRightContentRemoved;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the split container control. */
	SplitContainer();
	/* Deconstructs the split container control. */
	~SplitContainer();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Returns true if the control or any of its children has the specified name. */
	bool containsName(const std::string& name) const override;

	/* Gets the minimum width of the left content. */
	int getLeftMinWidth() const;
	/* Gets the minimum width of the right content. */
	int getRightMinWidth() const;
	/* Sets the minimum width of the left content. */
	void setLeftMinWidth(int leftMinWidth);
	/* Sets the minimum width of the right content. */
	void setRightMinWidth(int rightMinWidth);

	/* Gets the maximum width of the left content. */
	int getLeftMaxWidth() const;
	/* Gets the maximum width of the right content. */
	int getRightMaxWidth() const;
	/* Sets the maximum width of the left content. */
	void setLeftMaxWidth(int leftMaxWidth);
	/* Sets the maximum width of the right content. */
	void setRightMaxWidth(int rightMaxWidth);

	/* Gets if the right content is fixed. Otherwise the left is fixed. */
	bool isRightFixed() const;
	/* Sets if the right content is fixed. Otherwise the left is fixed. */
	void setRightFixed(bool rightFixed);

	/* Gets the width of the left content. */
	int getLeftWidth() const;
	/* Gets the width of the right content. */
	int getRightWidth() const;
	/* Sets the width of the left content. */
	void setLeftWidth(int leftWidth);
	/* Sets the width of the right content. */
	void setRightWidth(int rightWidth);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Sets the layout of the content. */
	void updateContentLayout();

	/* Iterates through all controls contained within this one. */
	void forEach(std::function<void(ControlSPtr control)> func) override;
	/* Draws the control to the graphics object. */
	void repaint(PcDraw::GraphicsSPtr g) override;

private:
	/* Updates changes to the separator position. */
	void updateSeparatorPosition();
	/* Gets the x position of the separator. */
	int getSeparatorPosition() const;
public:
	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the left content for the container. */
	ControlSPtr getLeftContent();
	/* Gets the right content for the container. */
	ControlSPtr getRightContent();
	/* Sets the specified left content for the container. */
	void setLeftContent(ControlSPtr control);
	/* Sets the specified right ontent for the container. */
	void setRightContent(ControlSPtr control);
	/* Removes the left content from the container. */
	void removeLeftContent();
	/* Removes the right content from the container. */
	void removeRightContent();

	/* Returns true if this control has any left content. */
	bool hasLeftContent() const;
	/* Returns true if this control has any rigth content. */
	bool hasRightContent() const;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the left content for the control has been set. */
	Event<ContentModifiedEventArgs>& eventLeftContentAdded();
	/* Called when the right content for the control has been set. */
	Event<ContentModifiedEventArgs>& eventRightContentAdded();
	/* Called when the left content has been removed from the control. */
	Event<ContentModifiedEventArgs>& eventLeftContentRemoved();
	/* Called when the right content has been removed from the control. */
	Event<ContentModifiedEventArgs>& eventRightContentRemoved();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onContentAdded(ControlSPtr sender, ContentModifiedEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(SplitContainer);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
