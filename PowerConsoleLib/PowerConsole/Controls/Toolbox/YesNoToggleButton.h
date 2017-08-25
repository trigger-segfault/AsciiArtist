/*=================================================================|
|* File:				YesNoToggleButton.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef YES_NO_TOGGLE_BUTTON_H
#define YES_NO_TOGGLE_BUTTON_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/ProtectedContainerControl.h>
#include <PowerConsole/Controls/Toolbox/ToggleButton.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class YesNoToggleButton : public ProtectedContainerControl {
	
	//=========== MEMBERS ============
	#pragma region Members

	// Controls
	ToggleButtonSPtr toggleButtonYes;
	ToggleButtonSPtr toggleButtonNo;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the yes no toggle button. */
	YesNoToggleButton();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the yes no toggle button is checked. */
	bool isChecked() const;
	/* Sets the no toggle button  is checked. */
	void setChecked(bool checked);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const final;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onToggleButtonClick(ControlSPtr sender, MouseEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(YesNoToggleButton);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
