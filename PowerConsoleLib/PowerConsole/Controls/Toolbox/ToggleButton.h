/*=================================================================|
|* File:				ToggleButton.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TOGGLE_BUTTON_H
#define TOGGLE_BUTTON_H

#include <PowerConsole/Controls/Toolbox/Button.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ToggleButton : public Button {

	//=========== MEMBERS ============
	#pragma region Members

	/* True if the button is checked. */
	bool checked;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the toggle button. */
	ToggleButton();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the button is checked. */
	bool isChecked() const;
	/* Sets the button is checked. */
	void setChecked(bool checked);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onClick(ControlSPtr sender, MouseEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(ToggleButton);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
