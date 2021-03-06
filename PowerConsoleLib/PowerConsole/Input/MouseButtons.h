/*=================================================================|
|* File:				MouseButtons.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Definition of the mouse buttons enumeration.	  *|
|=================================================================*/

#pragma once
#ifndef MOUSE_BUTTONS_ENUM_H
#define MOUSE_BUTTONS_ENUM_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole::Input {
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* Specifies constants that define which mouse button was pressed. Enumeration taken from .NET Framework */
enum class MouseButtons : unsigned int {
	
	//========== CONSTANTS ===========
	#pragma region Non-Specific
	
	/* No mouse button was pressed. */
	None = 0x00,
	
	#pragma endregion
	//--------------------------------
	#pragma region Basic Buttons

	/* The left mouse button was pressed. */
	Left = 0x01,
	//Left = 0x100000,
	/* The right mouse button was pressed. */
	Right = 0x02,
	//Right = 0x200000,
	/* The middle mouse button was pressed. */
	Middle = 0x03,
	//Middle = 0x400000,
	
	#pragma endregion
	//--------------------------------
	#pragma region Extra Buttons

	/* The first XButton was pressed. */
	XButton1 = 0x04,
	//XButton1 = 0x800000,
	/* The second XButton was pressed. */
	XButton2 = 0x05
	//XButton2 = 0x1000000
	
	#pragma endregion
};

/* The two types of mouse wheels, vertical and horizontal. */
enum class MouseWheels : unsigned int {

	//========== CONSTANTS ===========
	#pragma region Mouse Wheels

	/* No mouse wheel. */
	None = 0,
	/* The vertical mouse wheel. */
	Vertical = 1,
	/* The horizontal mouse wheel. */
	Horizontal = 2

	#pragma endregion
};

#pragma endregion
//=================================================================|
// OPERATORS													   |
//=================================================================/
#pragma region Operators

/* Flag operators for MouseButtons enum. */
//DEFINE_ENUM_FLAG_OPERATORS(MouseButtons)

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
