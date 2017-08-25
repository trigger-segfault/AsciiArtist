/*=================================================================|
|* File:				YesNoToggleButton.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef MY_YES_NO_TOGGLE_BUTTON_H
#define MY_YES_NO_TOGGLE_BUTTON_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/ProtectedContainerControl.h>
#include <AsciiVis/MyToggleButton.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

class MyYesNoToggleButton : public ProtectedContainerControl {

	//=========== MEMBERS ============
	#pragma region Members

	// Controls
	MyToggleButtonSPtr toggleButtonYes;
	MyToggleButtonSPtr toggleButtonNo;

	// Events
	Event<EventArgs> _eventChecked;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the yes no toggle button. */
	MyYesNoToggleButton();
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
	//============ EVENTS ============
	#pragma region Events
public:
	Event<EventArgs>& eventChecked();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onToggleButtonClick(ControlSPtr sender, MouseEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(MyYesNoToggleButton);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
