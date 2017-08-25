/*=================================================================|
|* File:				Mouse.h									  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Decleration of the Mouse listener class.		  *|
|=================================================================*/

#pragma once
#ifndef MOUSE_H
#define MOUSE_H

#define NOMINMAX
#include <windows.h>
#include <array>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/Input/MouseButtons.h>
#include <Powerconsole/Input/InputHandler.h>

namespace PowerConsole::Input {
DECLARE_CLASS_AND_SMART_PTRS(Mouse);
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The class for gathering mouse input. */
class Mouse {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The number of mouse buttons in the array. */
	static const unsigned int NUM_BUTTONS = 6;

	/* The number of mouse wheels in the array. */
	static const unsigned int NUM_WHEELS = 3;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
		
	// Mouse Position
	/* The current mouse position. */
	PcGeo::Point2I mouseGlobal;
	/* The last mouse position. */
	PcGeo::Point2I mouseGlobalLast;
	/* The current client mouse position. */
	PcGeo::Point2I mouseClient;
	/* The last client mouse position. */
	PcGeo::Point2I mouseClientLast;
	/* The current mouse coordinates. */
	PcGeo::Point2I mouseConsole;
	/* The last mouse coordinates. */
	PcGeo::Point2I mouseConsoleLast;
	/* True if the mouse is inside the window. */
	bool mouseInside;

	// Mouse Wheel
	/* The scroll distance of the mouse wheel. */
	PcGeo::Point2I mouseWheelDelta;

	// Button States
	/* The list of mouse states. */
	std::array<InputHandler, NUM_BUTTONS> buttonStates;
	/* True if the mouse is disabled. */
	bool disabled;
	/* True if the mouse has been disabled until release and will release inputs next tick. */
	bool disableNextTick;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs and sets up the mouse. */
	Mouse();
	
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Updates the mouse states. */
	void update(INPUT_RECORD& inRec, bool active, PcGeo::Point2I clientWindowPosition, PcGeo::Point2I consoleSize, PcGeo::Point2I bufferPosition, PcGeo::Point2I fontSize);
	
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management
	
	/* Resets all of the mouse button states. */
	void reset(bool release);
	/* Enables all of the mouse buttons. */
	void enable();
	/* Disables all of the mouse buttons. */
	void disable(bool releaseNextTick);
	
	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
	//--------------------------------
	#pragma region Button State

	/* Returns true if the specified mouse button was double clicked. */
	bool isButtonDoubleClicked(MouseButtons buttonCode) const;
	/* Returns true if the specified mouse button was double clicked. */
	bool isButtonDoubleClicked(int buttonCode) const;
	/* Returns true if the specified mouse button was pressed. */
	bool isButtonPressed(MouseButtons buttonCode) const;
	/* Returns true if the specified mouse button was pressed. */
	bool isButtonPressed(int buttonCode) const;
	/* Returns true if the specified mouse button is down. */
	bool isButtonDown(MouseButtons buttonCode) const;
	/* Returns true if the specified mouse button is down. */
	bool isButtonDown(int buttonCode) const;
	/* Returns true if the specified mouse button was released. */
	bool isButtonReleased(MouseButtons buttonCode) const;
	/* Returns true if the specified mouse button was released. */
	bool isButtonReleased(int buttonCode) const;
	/* Returns true if the specified mouse button is up. */
	bool isButtonUp(MouseButtons buttonCode) const;
	/* Returns true if the specified mouse button is up. */
	bool isButtonUp(int buttonCode) const;
	/* Returns true if the state of the specified mouse button has changed states. */
	bool hasButtonStateChanged(MouseButtons buttonCode) const;
	/* Returns true if the state of the specified mouse button has changed states. */
	bool hasButtonStateChanged(int buttonCode) const;
	/* Gets the state of the specified mouse button. */
	InputStates getButtonState(MouseButtons buttonCode) const;
	/* Gets the state of the specified mouse button. */
	InputStates getButtonState(int buttonCode) const;
	
	#pragma endregion
	//--------------------------------
	#pragma region Any Button State

	/* Returns true if any mouse button was double clicked. */
	bool isAnyButtonDoubleClicked() const;
	/* Returns true if any mouse button was pressed. */
	bool isAnyButtonPressed() const;
	/* Returns true if any mouse button is down. */
	bool isAnyButtonDown() const;
	/* Returns true if any mouse button was released. */
	bool isAnyButtonReleased() const;
	/* Returns true if any mouse button is up. */
	bool isAnyButtonUp() const;
	
	#pragma endregion
	//--------------------------------
	#pragma region Mouse Position

	// Mouse Position
	/* Gets the current mouse position. */
	PcGeo::Point2I getPosition() const;
	/* Gets the last mouse position. */
	PcGeo::Point2I getLastPosition() const;
	/* Gets the current client mouse position. */
	PcGeo::Point2F getClientPosition() const;
	/* Gets the last client mouse position. */
	PcGeo::Point2F getLastClientPosition() const;
	/* Returns true if the mouse has moved since the last update. */
	bool hasMoved() const;
	/* Returns true if the mouse is inside the window. */
	bool isMouseInsideWindow() const;
	
	#pragma endregion
	//--------------------------------
	#pragma region Scroll Wheel

	// Mouse Wheel
	/* Returns true if the mouse wheel was scrolled up. */
	bool isWheelScrolledUp() const;
	/* Returns true if the mouse wheel was scrolled down. */
	bool isWheelScrolledDown() const;
	/* Returns true if the mouse wheel was scrolled left. */
	bool isWheelScrolledLeft() const;
	/* Returns true if the mouse wheel was scrolled right. */
	bool isWheelScrolledRight() const;
	/* Returns true if the specified scroll wheel has been scrolled. */
	bool isWheelScrolled(MouseWheels wheelCode) const;
	/* Returns true if the specified scroll wheel has been scrolled. */
	bool isWheelScrolled(int wheelCode) const;
	/* Gets the mouse wheel delta position. */
	int getWheelDelta(MouseWheels wheelCode) const;
	/* Gets the mouse wheel delta position. */
	int getWheelDelta(int wheelCode) const;
	/* Gets the mouse wheel delta position. */
	PcGeo::Point2I getWheelDelta() const;

	#pragma endregion
	//--------------------------------
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
