/*=================================================================|
|* File:				ControlNew.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CONTROL_NEW_H
#define CONTROL_NEW_H

#include <string>
#include <memory>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/GeometryEnums.h>
#include <PowerConsole/Geometry/Rectangle.h>
#include <PowerConsole/Controls/Events/CommandCollection.h>
#include <PowerConsole/Drawing/Graphics.h>

/* Creates a control of the specified type. It's recommend you use this over any other method. */
#define makeControl(controlType, ...) (std::static_pointer_cast<controlType>(std::make_shared<controlType>(__VA_ARGS__)->initialize()))
/* Creates a control of the specified type. It's recommend you use this over any other method. */
#define makePrivateControl(controlType, ...) (std::static_pointer_cast<controlType>(std::shared_ptr<controlType>(new controlType(__VA_ARGS__))->initialize()))
/* Creates a window of the specified type. It's recommend you use this over any other method. */
#define makeWindow(windowType, ...) makeControl(windowType, __VA_ARGS__)
/* Creates a window of the specified type. It's recommend you use this over any other method. */
#define makePrivateWindow(windowType, ...) makePrivateControl(windowType, __VA_ARGS__)
/* Creates a dialog of the specified type. It's recommend you use this over any other method. */
#define makeDialog(dialogType, ...) makeControl(dialogType, __VA_ARGS__)
/* Creates a dialog of the specified type. It's recommend you use this over any other method. */
#define makePrivateDialog(dialogType, ...) makePrivateControl(dialogType, __VA_ARGS__)

namespace PowerConsole::Controls {
DECLARE_CLASS_AND_SMART_PTRS(Control);
DECLARE_CLASS_AND_SMART_PTRS(Window);
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The base for all controls and windows. */
class Control : public std::enable_shared_from_this<Control> {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The code for using auto sizing in the size property. */
	static const int SIZE_AUTO = -1;
	/* Used to disable a maximum or minimum size property. */
	static const int MINMAX_NONE = 0;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Control Tree
	/* True if the control has been initialized. */
	bool initialized;
	/* The parent containing this control. */
	ControlWPtr parent;

	// General
	/* The unique name of the control. */
	std::string name;
	/* True if the control is enabled. */
	bool enabled;
	/* True if the control is visible. */
	bool visible;

	// Input
	/* True if the control has focus. */
	bool focused;
	/* True if the mouse is inside the control. */
	bool mouseInside;
	/* The current mouse position of the control.. */
	PcGeo::Point2I mousePosition;
	/* The last mouse position of the control. Used to check if the mouse has moved within the control. */
	PcGeo::Point2I mousePositionLast;
	/* The mouse states for pressing inside the control. If a button was pressed inside the control. */
	/* The local button event will fire when that button is released, even if outsite the control. */
	bool mouseButtonStates[5];
	/* The collection of commands for the window. */
	CommandCollection commandCollection;

	// Local Layout
	/* The desired size of the control. */
	PcGeo::Point2I size;
	/* The margins for the control. */
	PcGeo::Thickness margin;
	/* The horizontal alignment of the control. */
	PcGeo::HorizontalAlignments horizontalAlignment;
	/* The vertical alignment of the control. */
	PcGeo::VerticalAlignments verticalAlignment;

	// Container Layout
	/* The actual boundaries of the control. */
	PcGeo::Rectangle2I actualBounds;
	/* The actual visible area of the control. */
	PcGeo::Rectangle2I actualVisibleArea;
	/* The boundaries of the control within its container. This should only be set by the control's parent. */
	PcGeo::Rectangle2I containerBounds;
	/* The visible area of the control in the container. This should only be set by the control's parent. */
	PcGeo::Rectangle2I containerVisibleArea;

	// Events
	/* Called when the control is initialized. */
	Event<EventArgs> _eventInitialize;
	/* Called when the layout of the control has changed. */
	Event<LayoutChangedEventArgs> _eventLayoutChanged;

	/* Called when a mouse button is moved while inside the control. */
	Event<MouseEventArgs> _eventMouseMoved;
	/* Called when a mouse is moved. */
	Event<MouseEventArgs> _eventMouseMovedGlobal;
	/* Called when a mouse leaves the control's visible area. */
	Event<MouseEventArgs> _eventMouseEnter;
	/* Called when a mouse leaves the control's visible area. */
	Event<MouseEventArgs> _eventMouseLeave;
	/* Called when a mouse button is pressed or released inside the control. */
	Event<MouseButtonEventArgs> _eventMouseButton;
	/* Called when a mouse button is pressed or released. */
	Event<MouseButtonEventArgs> _eventMouseButtonGlobal;
	/* Called when a mouse wheel is scrolled inside the control. */
	Event<MouseWheelEventArgs> _eventMouseWheel;
	/* Called when a mouse wheel is scrolled. */
	Event<MouseWheelEventArgs> _eventMouseWheelGlobal;
	/* Called when a mouse button is clicked inside the control. */
	Event<MouseEventArgs> _eventClick;
	/* Called when a mouse button is double clicked inside the control. */
	Event<MouseEventArgs> _eventDoubleClick;

	/* Called when a key is pressed or released while the control has focus. */
	Event<KeyboardEventArgs> _eventKey;
	/* Called when a key is pressed or released. */
	Event<KeyboardEventArgs> _eventKeyGlobal;
	/* Called when a key has been pressed or held down and repeated while the control has focus. */
	Event<KeyTypedEventArgs> _eventKeyTyped;
	/* Called when a key has been pressed or held down and repeated. */
	Event<KeyTypedEventArgs> _eventKeyTypedGlobal;

	/* Called when the control gains focus. */
	Event<EventArgs> _eventFocusGained;
	/* Called when the control loses focus. */
	Event<EventArgs> _eventFocusLost;

	/* Called to update the control. */
	Event<TickEventArgs> _eventTick;
	/* Called to draw the control. */
	Event<PaintEventArgs> _eventPaint;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the base for all controls. */
	Control();
	/* Initializes the control. */
	ControlSPtr initialize();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Safe Setters
protected:
	/* Sets the parent of the control without any callbacks. */
	void setParentSafe(ControlWPtr parent);

	/* Sets the size of the control without any callbacks. */
	void setSizeSafe(PcGeo::Point2I size);
	/* Sets the margin of the control without any callbacks. */
	void setMarginSafe(PcGeo::Thickness margin);
	/* Sets the horizontal alignment of the control without any callbacks. */
	void setHorizontalAlignmentSafe(PcGeo::HorizontalAlignments horizontalAlignment);
	/* Sets the vertical alignment of the control without any callbacks. */
	void setVerticalAlignmentSafe(PcGeo::VerticalAlignments verticalAlignment);

	/* Sets the actual bounds of the control without any callbacks. */
	void setActualBoundsSafe(PcGeo::Rectangle2I actualBounds);
	/* Sets the actual position of the control without any callbacks. */
	void setActualPositionSafe(PcGeo::Point2I actualPosition);
	/* Sets the actual size of the control without any callbacks. */
	void setActualSizeSafe(PcGeo::Point2I actualSize);
	/* Sets the actual visible area of the control without any callbacks. */
	void setActualVisibleAreaSafe(PcGeo::Rectangle2I actualVisibleArea);
	/* Sets the container bounds of the control without any callbacks. */
	void setContainerBoundsSafe(PcGeo::Rectangle2I containerBounds);
	/* Sets the container position of the control without any callbacks. */
	void setContainerPositionSafe(PcGeo::Point2I containerPosition);
	/* Sets the container size of the control without any callbacks. */
	void setContainerSizeSafe(PcGeo::Point2I containerSize);
	/* Sets the container visible area of the control without any callbacks. */
	void setContainerVisibleAreaSafe(PcGeo::Rectangle2I containerVisibleArea);

	#pragma endregion
	//--------------------------------
	#pragma region Control Tree
public:
	/* Returns true if the control has been initialized. */
	bool isInitialized() const;
	/* Returns true if the control has a parent. */
	bool hasParent() const;
	/* Gets the parent of the control. */
	ControlSPtr getParent();
	/* Sets the parent of the control. Only the parent or parent's collection should call this. */
	virtual void setParent(ControlSPtr parent);
	/* Gets the root container holding this control. */
	ControlSPtr getRoot();
	/* Gets the window containing this control. */
	virtual WindowSPtr getWindow();

	#pragma endregion
	//--------------------------------
	#pragma region Commands

	/* Gets the collection of commands for the window. */
	CommandCollection& commands();
	/* Gets the collection of commands for the window. */
	const CommandCollection& commands() const;

	#pragma endregion
	//--------------------------------
	#pragma region General

	/* Returns true if the control or any of its children has the specified name. */
	virtual bool containsName(const std::string& name) const;
	/* Returns true if the control has a name. */
	bool hasName() const;
	/* Gets the unique name of the control. */
	std::string getName() const;
	/* Sets the unique name of the control. */
	void setName(const std::string& name);
	/* Gets if the control is enabled. */
	bool isEnabled() const;
	/* Sets if the control is enabled. */
	void setEnabled(bool enabled);
	/* Gets if the control is visible based on its parents' visibility. */
	bool isActuallyVisible();
	/* Gets if the control is visible. */
	bool isVisible() const;
	/* Sets if the control is visible. */
	void setVisible(bool visible);

	#pragma endregion
	//--------------------------------
	#pragma region Local Layout

	/* Gets the size of the control. */
	virtual PcGeo::Point2I getSize() const;
	/* Sets the size of the control. */
	virtual void setSize(PcGeo::Point2I size);
	/* Gets the size with auto behavior applied. */
	PcGeo::Point2I getSizeWithAuto() const;
	/* Gets the margin of the control. */
	PcGeo::Thickness getMargin() const;
	/* Sets the margin of the control. */
	virtual void setMargin(PcGeo::Thickness margin);
	/* Gets the horizontal alignment of the control. */
	PcGeo::HorizontalAlignments getHorizontalAlignment() const;
	/* Sets the horizontal alignment of the control. */
	virtual void setHorizontalAlignment(PcGeo::HorizontalAlignments horizontalAlignment);
	/* Gets the vertical alignment of the control. */
	PcGeo::VerticalAlignments getVerticalAlignment() const;
	/* Sets the vertical alignment of the control. */
	virtual void setVerticalAlignment(PcGeo::VerticalAlignments verticalAlignment);

	#pragma endregion
	//--------------------------------
	#pragma region Container Layout

	/* Sets the boundaries of the control within its container. This should only be set by the control's parent. */
	virtual void setContainerBounds(PcGeo::Rectangle2I containerBounds);
	/* Sets the visible area of the control within it's container. This should only be set by the control's parent. */
	virtual void setContainerVisibleArea(PcGeo::Rectangle2I containerVisibleArea);
	/* Sets the boundaries and visible area of the control within it's container. This should only be set by the control's parent. */
	virtual void setContainerLayout(PcGeo::Rectangle2I containerBounds, PcGeo::Rectangle2I containerVisibleArea);
	/* Gets the boundaries of the control within its container. */
	PcGeo::Rectangle2I getContainerBounds() const;
	/* Gets the position of the control within the container. */
	PcGeo::Point2I getContainerPosition() const;
	/* Gets the size of the container for the control. */
	PcGeo::Point2I getContainerSize() const;
	/* Gets the visible area of the control within it's container. */
	PcGeo::Rectangle2I getContainerVisibleArea() const;
	/* Gets the position of the control within the container. */
	PcGeo::Point2I getLocalPosition() const;
	/* Gets the global position of the control within its root parent. */
	PcGeo::Point2I getGlobalPosition() const;
	/* Gets the local visible area of the control. */
	PcGeo::Rectangle2I getLocalVisibleArea() const;
	/* Gets the global visible area of the control. */
	PcGeo::Rectangle2I getGlobalVisibleArea() const;
	/* Gets the actual boundaries of the control based on its margins and the container size. */
	PcGeo::Rectangle2I getActualBounds() const;
	/* Gets the actual position of the control based on its margins. */
	PcGeo::Point2I getActualPosition() const;
	/* Gets the actual size of the control based on its margins and the container size. */
	PcGeo::Point2I getActualSize() const;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating
public:
	/* Gets the auto size of the control when not stretching. */
	virtual PcGeo::Point2I autoSizeBehavior() const;
	/* Updates the layout of the control after changes have been made to the control's layout properties. */
	virtual void updateLayout(bool sizeChanged, bool marginChanged, bool alignmentChanged, bool positionChanged, bool visibleAreaChanged);
	/* Updates the layout of the control if the auto size has changed. */
	void updateAutoSize(bool xChanged = true, bool yChanged = true);

	/* Iterates through all controls contained within this one. */
	virtual void forEach(std::function<void(ControlSPtr control)> func);
	/* Draws the control and all children to the graphics object. */
	virtual void repaint(PcDraw::GraphicsSPtr g);

	#pragma endregion
	//============ INPUT =============
	#pragma region Input

	/* Gets if the control has focus. */
	virtual bool hasFocus() const;
	/* Focuses the control. */
	virtual void focus();
	/* Unfocuses the control. */
	virtual void unfocus();

	/* Returns true if the mouse is inside the control's visible area. */
	bool isMouseInside() const;
	/* Returns trye if the control is being pressed by the mouse. */
	bool isMouseDown(PcInput::MouseButtons buttonCode = PcInput::MouseButtons::Left) const;
	/* Gets the current position of the mouse in the control. */
	PcGeo::Point2I getMousePosition() const;
	/* Gets the last position of the mouse in the control. Used to check if the mouse has moved within the control. */
	PcGeo::Point2I getMousePositionLast() const;
	/* Called to update the last mouse position when the mouse moved global event isn't called. */
	void updateMousePositionLast();
	/* Invalidates the last mouse position. Forcing the mouse moved event next tick. */
	void invalidateMousePosition();

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	// Basic
	/* Called when the control is initialized. */
	Event<EventArgs>& eventInitialize();
	/* Called when the layout of the control has changed. */
	Event<LayoutChangedEventArgs>& eventLayoutChanged();

	// Mouse
	/* Called when a mouse is moved while inside the control. */
	Event<MouseEventArgs>& eventMouseMoved();
	/* Called when a mouse is moved. */
	Event<MouseEventArgs>& eventMouseMovedGlobal();
	/* Called when a mouse leaves the control's visible area. */
	Event<MouseEventArgs>& eventMouseEnter();
	/* Called when a mouse leaves the control's visible area. */
	Event<MouseEventArgs>& eventMouseLeave();
	/* Called when a mouse button is pressed or released inside the control. */
	Event<MouseButtonEventArgs>& eventMouseButton();
	/* Called when a mouse button is pressed or released. */
	Event<MouseButtonEventArgs>& eventMouseButtonGlobal();
	/* Called when a mouse wheel is scrolled inside the control. */
	Event<MouseWheelEventArgs>& eventMouseWheel();
	/* Called when a mouse wheel is scrolled. */
	Event<MouseWheelEventArgs>& eventMouseWheelGlobal();
	/* Called when a mouse button is clicked inside the control. */
	Event<MouseEventArgs>& eventClick();
	/* Called when a mouse button is double clicked inside the control. */
	Event<MouseEventArgs>& eventDoubleClick();

	// Keyboard
	/* Called when a key is pressed or released while the control has focus. */
	Event<KeyboardEventArgs>& eventKey();
	/* Called when a key is pressed or released. */
	Event<KeyboardEventArgs>& eventKeyGlobal();
	/* Called when a key has been pressed or held down and repeated while the control has focus. */
	Event<KeyTypedEventArgs>& eventKeyTyped();
	/* Called when a key has been pressed or held down and repeated. */
	Event<KeyTypedEventArgs>& eventKeyTypedGlobal();

	// Focus
	/* Called when the control gains focus. */
	Event<EventArgs>& eventFocusGained();
	/* Called when the control loses focus. */
	Event<EventArgs>& eventFocusLost();
	
	// Commands
	/* Called when an command has been added to the collection. */
	Event<CommandModifiedEventArgs>& eventCommandAdded();
	/* Called when an command has been removed from the collection. */
	Event<CommandModifiedEventArgs>& eventCommandRemoved();

	// Update
	/* Called to update the control. */
	Event<TickEventArgs>& eventTick();
	/* Called to draw the control. */
	Event<PaintEventArgs>& eventPaint();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e);
	void onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e);
	void onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e);
	void onKeyTypedGlobal(ControlSPtr sender, KeyTypedEventArgs& e);
	void onFocusGained(ControlSPtr sender, EventArgs& e);
	void onFocusLost(ControlSPtr sender, EventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
