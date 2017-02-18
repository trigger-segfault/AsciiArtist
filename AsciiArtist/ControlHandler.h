/*=================================================================|
|* File:				ControlHandler.h						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Decleration of the control handler class.		  *|
|=================================================================*/

#pragma once
#ifndef CONTROL_HANDLER_H
#define CONTROL_HANDLER_H

#include "Stdafx.h"

using namespace std;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of event handling classes. */
namespace Input {

//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* The different states a control can be in. */
enum class ControlState : short {
	Up = 0,
	Released = 1,
	Down = 2,
	Pressed = 3
};
/* The different disabled states a control can be in. */
enum class DisableState : short {
	Enabled = 0,
	Disabled = 1,
	DisabledUntilRelease = 2
};

#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* A class to manage the state of a control. */
class ControlHandler {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The state of the control. */
	ControlState _state;
	/* The disabled state of the control. */
	DisableState _disabledState;
	/* True if the control was double clicked. */
	bool _doubleClicked;
	/* <summary> True if the control was clicked. */
	bool _clicked;
	/* <summary> True if the control was typed. */
	bool _typed;
	/* The time the control has been held for since being pressed, or released. */
	int _holdTime;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default control. */
	ControlHandler();
	
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the control state. */
	void Update(bool down, bool typed = false, bool doubleClicked = false, bool clicked = false);
	
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management

	/* Resets the control state. */
	void Reset(bool release = false);
	/* Enables the control. */
	void Enable();
	/* Disables the control. */
	void Disable(bool untilRelease = false);
	
	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Returns true if the control was double clicked. */
	bool IsDoubleClicked();
	/* Returns true if the control was clicked. */
	bool IsClicked();
	/* Returns true if the control was pressed. */
	bool IsPressed();
	/* Returns true if the control is down. */
	bool IsDown();
	/* Returns true if the control was released. */
	bool IsReleased();
	/* Returns true if the control is up. */
	bool IsUp();
	/* Returns true if the control was typed. */
	bool IsTyped();
	/* Returns true if the control is disabled. */
	bool IsDisabled();
	/* Gets the time the key has been held for since being pressed, or released. */
	int GetHoldTime();
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace Input */
} /* namespace PowerConsole */
#endif /* CONTROL_HANDLER_H */
//=================================================================|
