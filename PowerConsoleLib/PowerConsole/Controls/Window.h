/*=================================================================|
|* File:				Window.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef WINDOW_NEW_H
#define WINDOW_NEW_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/ConsoleSettings.h>
#include <PowerConsole/Controls/WindowConsoleWrapper.h>
#include <PowerConsole/Controls/ContainerControl.h>
#include <PowerConsole/Controls/TabStopCollection.h>
#include <PowerConsole/Input/Keyboard.h>
#include <PowerConsole/Input/Mouse.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/Color.h>
#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/ConsoleEnums.h>

namespace PowerConsole::Controls {
DECLARE_CLASS_AND_SMART_PTRS(Window);
DECLARE_CLASS_AND_SMART_PTRS(Dialog);
DECLARE_CLASS_AND_SMART_PTRS(MainWindow);
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Data used for drawing the resize handle pixel. */
struct ResizeHandlePixel {
	/* The character for the resize handle. */
	unsigned char character;
	/* The color of the resize handle when not hovering or resizing. */
	unsigned char normalColor;
	/* The color of the resize handle when hovering. */
	unsigned char hoverColor;
	/* The color of the resize handle when resizing. */
	unsigned char resizeColor;

	/* Constructs the default resize handle pixel. */
	inline ResizeHandlePixel()
	 :	character(PcDraw::Borders::CORNER_THICK_TOP_THICK_LEFT),
		normalColor(0x08), hoverColor(0x07), resizeColor(0x07) {}
	/* Constructs the resize handle pixel. */
	inline ResizeHandlePixel(unsigned char character, unsigned char normalColor, unsigned char hoverColor)
		: character(character), normalColor(normalColor),
		hoverColor(hoverColor), resizeColor(hoverColor) {}
	/* Constructs the resize handle pixel. */
	inline ResizeHandlePixel(unsigned char character, unsigned char normalColor, unsigned char hoverColor, unsigned char resizeColor)
		: character(character), normalColor(normalColor),
		hoverColor(hoverColor), resizeColor(resizeColor) {}
};

class Window : public ContainerControl {

	//=========== MEMBERS ============
	#pragma region Members

	// Main loop
	/* True if the window is still running. */
	bool running;
	/* The point in time at which the frame started. */
	int frameStart;
	/* The duration of the last frame. */
	int frameDuration;
	/* The frame rate of the program. */
	float fps;

	// Drawing
	/* The graphics to draw to. */
	PcDraw::AsciiImageSPtr graphicsImage;
	/* True if the buffer should be forced to redraw. */
	bool forceRedrawBuffer;

	/* The owner of this window. */
	WindowSPtr owner;
	/* The currently focused control. */
	ControlSPtr currentFocus;
	/* The default focus when none is set. */
	ControlSPtr defaultFocus;
	/* The tab stop collection. */
	TabStopCollectionSPtr tabStopCollection;
	/* The keyboard input handler. */
	PcInput::KeyboardSPtr keyboardInput;
	/* The mouse input handler. */
	PcInput::MouseSPtr mouseInput;
	/* The dialog currently being shown. */
	DialogSPtr dialog;

	// Resize Handle
	/* True if the mouse is dragging the resize corner. */
	bool resizing;
	/* True if the window has a resize handle. */
	bool resizeHandleEnabled;
	/* The pixel for the resize handle. */
	ResizeHandlePixel resizeHandlePixel;
	/* The soft maximum size of the console window. */
	PcGeo::Point2I maxSize;
	/* The soft minimum size of the console window. */
	PcGeo::Point2I minSize;

	// Console
	/* The console to make changes to. */
	Pc::ConsoleSettingsSPtr consoleSettings;
	/* The wrapper for other controls to access console settings. */
	WindowConsoleWrapperSPtr consoleWrapper;

	// Events
	/* Called when the window is loaded just before running. */
	Event<EventArgs> _eventLoad;
	/* Called when the window is unloaded just after running. */
	Event<EventArgs> _eventUnload;
	/* Called when the current focus in the window has changed. */
	Event<FocusChangedEventArgs> _eventFocusChanged;
	/* Called when the window position has changed. */
	Event<WindowMovedEventArgs> _eventWindowMoved;
	/* Called when the window has been activated. */
	Event<EventArgs> _eventWindowActivated;
	/* Called when the window has been deactivated. */
	Event<EventArgs> _eventWindowDeactivated;
	/* Called when debug information is being drawn. */
	Event<PaintEventArgs> _eventPaintDebug;
	/* Called to update the window even when in a dialog. */
	Event<TickEventArgs> _eventGlobalTick;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the window. */
	Window();
	/* Deconstructs window. */
	~Window();
private:
	/* Loads the window. */
	void load();
	/* Unloads the window. */
	void unload();
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* The main loop for running the console window. */
	void mainLoop();
	/* Handles control events. */
	void handleEvents();
public:
	/* Starts up a new dialog owned by this window. */
	bool showDialog(DialogSPtr dialog);

	/* Draws the control to the graphics object. */
	void repaint(PcDraw::GraphicsSPtr g) override;
	/* Repaints the window and writes to the console buffer. */
	void redraw();

	/* Closes the window. */
	void close();

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the window is loaded just before running. */
	Event<EventArgs>& eventLoad();
	/* Called when the window is unloaded just after running. */
	Event<EventArgs>& eventUnload();
	/* Called when the current focus in the window has changed. */
	Event<FocusChangedEventArgs>& eventFocusChanged();
	/* Called when the window position has changed. */
	Event<WindowMovedEventArgs>& eventWindowMoved();
	/* Called when the window has been activated. */
	Event<EventArgs>& eventWindowActivated();
	/* Called when the window has been deactivated. */
	Event<EventArgs>& eventWindowDeactivated();
	/* Called when debug information is being drawn. */
	Event<PaintEventArgs>& eventPaintDebug();
	/* Called to update the window even when in a dialog. */
	Event<TickEventArgs>& eventGlobalTick();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e);
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onChildRemoved(ControlSPtr sender, ChildModifiedEventArgs& e);
	void onWindowActivated(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Control Tree
public:
	/* Sets the parent of the control. */
	void setParent(ControlSPtr parent) final;
	/* Returns true if the window has an owner. */
	bool hasOwner() const;
	/* Gets the owner of this window. */
	WindowSPtr getOwner();
	/* Sets the owner of the window. */
	virtual void setOwner(WindowSPtr owner);
	/* Gets the window containing this control. */
	WindowSPtr getWindow() final;
	/* Gets if the window is showing a dialog. */
	bool isShowingDialog() const;
	/* Gets the currently shown dialog. */
	DialogSPtr getDialog() const;
	/* Gets the current window being shown. */
	WindowSPtr getCurrentWindow();

	#pragma endregion
	//--------------------------------
	#pragma region Contained Classes

	/* Gets the wrapper for the console settings. */
	WindowConsoleWrapper& console();
	/* Gets the wrapper for the console settings. */
	const WindowConsoleWrapper& console() const;

	/* Gets the collection of tab stops. */
	TabStopCollection& tabStops();
	/* Gets the collection of tab stops. */
	const TabStopCollection& tabStops() const;

	/* Gets the keyboard input class. */
	const PcInput::Keyboard& keyboard();
	/* Gets the mouse input class. */
	const PcInput::Mouse& mouse();

	/* Gets the current keyboard input class. */
	const PcInput::Keyboard& globalKeyboard();
	/* Gets the current mouse input class. */
	const PcInput::Mouse& globalMouse();

	#pragma endregion
	//--------------------------------
	#pragma region Main Loop

	/* Returns true if the window has not been closed yet. */
	bool isRunning() const;
	/* Gets the duration of the last frame. */
	int getFrameDuration() const;
	/* Gets the frame rate of the program. */
	float getFPS() const;
	/* Forces the window's next write buffer to the console to draw even if no changes were made. */
	void forceRedraw();

	#pragma endregion
	//--------------------------------
	#pragma region Input

	/* Gets the currently focused control. */
	ControlSPtr getFocus();
	/* Sets the focus to the current control. */
	void setFocus(ControlSPtr control);
	/* Gets the default focus that will be set whenever all focus is removed. */
	ControlSPtr getFocusDefault() const;
	/* Sets the default focus that will be set whenever all focus is removed. */
	void setFocusDefault(ControlSPtr control);
	/* Clears the focus from the current control. */
	void clearFocus();
	/* Focuses on next tab stop based on the window's current focus. */
	void nextTabStop();

	#pragma endregion
	//--------------------------------
	#pragma region Local Layout

	/* Gets the size of the control. */
	PcGeo::Point2I getSize() const override;
	/* Sets the size of the control. */
	void setSize(PcGeo::Point2I size) override;
	/* Gets the maximum size of the console window. */
	PcGeo::Point2I getMaxSize();
	/* Sets the maximum size of the console window. */
	void setMaxSize(PcGeo::Point2I maxSize);
	/* Gets the minimum size of the console window. */
	PcGeo::Point2I getMinSize();
	/* Sets the minimum size of the console window. */
	void setMinSize(PcGeo::Point2I minSize);
	/* Gets the actual maximum size of the console window after both sizes are applied. */
	PcGeo::Point2I getActualMaxSize();
	/* Gets the actual minimum size of the console window after both sizes are applied. */
	PcGeo::Point2I getActualMinSize();

	/* Sets the margin of the control. */
	void setMargin(PcGeo::Thickness margin) final;
	/* Sets the horizontal alignment of the control. */
	void setHorizontalAlignment(PcGeo::HorizontalAlignments horizontalAlignment) final;
	/* Sets the vertical alignment of the control. */
	void setVerticalAlignment(PcGeo::VerticalAlignments verticalAlignment) final;

	#pragma endregion
	//--------------------------------
	#pragma region Resize Handle

	/* Gets if the window has a resize handle. */
	bool isResizeHandleEnabled();
	/* Sets if the window has a resize handle. */
	void setResizeHandleEnabled(bool resizeHandleEnabled);
	/* Gets the pixel for the resize handle. */
	ResizeHandlePixel getResizeHandlePixel() const;
	/* Sets the pixel for the resize handle. */
	void setResizeHandlePixel(ResizeHandlePixel pixel);

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== FRIENDS ============
	#pragma region Friends

	friend MainWindow;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
