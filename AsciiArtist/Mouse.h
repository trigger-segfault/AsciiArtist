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

#include "Stdafx.h"
#include "Keys.h"
#include "MouseButtons.h"
#include "ControlHandler.h"
#include "Geometry.h"

using namespace std;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of event handling classes. */
namespace Input {

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
	static const unsigned int NumButtons	= 6;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The structure to store the data about the mouse class . */
	struct _MouseMembers {
		
		// Mouse Position
		/* The current mouse position. */
		Coord MousePoint;
		/* The last mouse position. */
		Coord MousePointLast;
		/* The current client mouse position. */
		Coord MouseClient;
		/* The last client mouse position. */
		Coord MouseClientLast;
		/* The current mouse coordinates. */
		Coord MouseCoord;
		/* The last mouse coordinates. */
		Coord MouseCoordLast;
		/* True if the mouse is inside the window. */
		bool MouseInside;

		// Mouse Wheel
		/* The scroll distance of the mouse wheel. */
		Coord MouseWheelDelta;

		// Button States
		/* The list of mouse states. */
		ControlHandler* ButtonStates;
		/* True if the mouse is disabled. */
		bool Disabled;
	};
	/* The collection of data in the mouse class. */
	_MouseMembers _members;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Initializes and sets up the mouse. */
	Mouse();
	/* Uninitializes and cleans up the mouse. */
	~Mouse();
	
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Updates the mouse state. */
	void Update(INPUT_RECORD& inRec, bool active, Coord clientPosition, Coord consoleSize, Coord fontSize);
	
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management
	
	/* Resets all the mouse button states. */
	void Reset(bool release);
	/* Enables all the mouse buttons. */
	void Enable();
	/* Disables all the mouse buttons. */
	void Disable(bool untilRelease);
	
	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
	//--------------------------------
	#pragma region Button State

	/* Returns true if the specified mouse button was double clicked. */
	bool IsButtonDoubleClicked(MouseButtons buttonCode);
	/* Returns true if the specified mouse button was double clicked. */
	bool IsButtonDoubleClicked(int buttonCode);
	/* Returns true if the specified mouse button was pressed. */
	bool IsButtonPressed(MouseButtons buttonCode);
	/* Returns true if the specified mouse button was pressed. */
	bool IsButtonPressed(int buttonCode);
	/* Returns true if the specified mouse button is down. */
	bool IsButtonDown(MouseButtons buttonCode);
	/* Returns true if the specified mouse button is down. */
	bool IsButtonDown(int buttonCode);
	/* Returns true if the specified mouse button was released. */
	bool IsButtonReleased(MouseButtons buttonCode);
	/* Returns true if the specified mouse button was released. */
	bool IsButtonReleased(int buttonCode);
	/* Returns true if the specified mouse button is up. */
	bool IsButtonUp(MouseButtons buttonCode);
	/* Returns true if the specified mouse button is up. */
	bool IsButtonUp(int buttonCode);
	
	#pragma endregion
	//--------------------------------
	#pragma region Any Button State

	/* Returns true if any mouse button was double clicked. */
	bool IsAnyButtonDoubleClicked();
	/* Returns true if any mouse button was pressed. */
	bool IsAnyButtonPressed();
	/* Returns true if any mouse button is down. */
	bool IsAnyButtonDown();
	/* Returns true if any mouse button was released. */
	bool IsAnyButtonReleased();
	/* Returns true if any mouse button is up. */
	bool IsAnyButtonUp();
	
	#pragma endregion
	//--------------------------------
	#pragma region Mouse Position

	// Mouse Position
	/* Gets the current mouse position. */
	Coord GetPosition();
	/* Returns true if the mouse is inside the window. */
	bool IsMouseInsideWindow();
	
	#pragma endregion
	//--------------------------------
	#pragma region Scroll Wheel

	// Mouse Wheel
	/* Returns true if the mouse wheel was scrolled up. */
	bool IsWheelScrolledUp();
	/* Returns true if the mouse wheel was scrolled down. */
	bool IsWheelScrolledDown();
	/* Returns true if the mouse wheel was scrolled left. */
	bool IsWheelScrolledLeft();
	/* Returns true if the mouse wheel was scrolled right. */
	bool IsWheelScrolledRight();
	/* Gets the mouse wheel delta position. */
	Coord GetWheelDelta();

	#pragma endregion
	//--------------------------------
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace Input */
} /* namespace PowerConsole */
#endif /* MOUSE_H */
//=================================================================|
