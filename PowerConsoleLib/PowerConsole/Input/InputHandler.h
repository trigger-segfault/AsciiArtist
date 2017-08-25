/*=================================================================|
|* File:				InputHandler.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Decleration of the control handler class.		  *|
|=================================================================*/

#pragma once
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole::Input {
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* The different states a control can be in. */
enum class InputStates : short {
	Up = 0,
	Released = 1,
	Down = 2,
	Pressed = 3
};
/* The pressed state of a control. */
enum class PressedStates : short {
	Released = 1,
	Pressed = 3
};
/* The different disabled states a control can be in. */
enum class DisableStates : short {
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
class InputHandler {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The state of the control. */
	InputStates state;
	/* The disabled state of the control. */
	/* This state starts as disabled until release to help fix key presses going into and out of new dialogs. */
	DisableStates disabledState;
	/* True if the control was double clicked. */
	bool doubleClicked;
	/* <summary> True if the control was clicked. */
	bool clicked;
	/* <summary> True if the control was typed. */
	bool typed;
	/* The time the control has been held for since being pressed, or released. */
	int holdTime;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default control. */
	InputHandler();
	
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the control state. */
	void update(bool down, bool typed = false, bool doubleClicked = false, bool clicked = false);
	
	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management

	/* Resets the control state. */
	void reset(bool release = false);
	/* Enables the control. */
	void enable();
	/* Disables the control. */
	void disable(bool untilRelease = false);
	
	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Returns true if the control was double clicked. */
	bool isDoubleClicked() const;
	/* Returns true if the control was clicked. */
	bool isClicked() const;
	/* Returns true if the control was pressed. */
	bool isPressed() const;
	/* Returns true if the control is down. */
	bool isDown() const;
	/* Returns true if the control was released. */
	bool isReleased() const;
	/* Returns true if the control is up. */
	bool isUp() const;
	/* Returns true if the control was typed. */
	bool isTyped() const;
	/* Returns true if the control is disabled. */
	bool isDisabled() const;
	/* Gets the time the key has been held for since being pressed, or released. */
	int getHoldTime() const;
	/* Gets the state of the control. */
	InputStates getState() const;
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
