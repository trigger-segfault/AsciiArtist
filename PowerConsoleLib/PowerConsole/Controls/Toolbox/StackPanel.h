/*=================================================================|
|* File:				StackPanel.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef STACK_PANEL_H
#define STACK_PANEL_H

#include <PowerConsole/Controls/ContainerControl.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A container that draws controls one after another in a horizontal or vertical orientation. */
class StackPanel : public ContainerControl {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The orientation of the stack panel. */
	PcGeo::Orientations orientation;
	/* The extra spacing between children. */
	int spacing;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the stack panel. */
	StackPanel();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the orientation of the stack panel. */
	PcGeo::Orientations getOrientation() const;
	/* Sets the orientation of the stack panel. */
	void setOrientation(PcGeo::Orientations orientation);
	/* Gets the extra spacing between children. */
	int getSpacing() const;
	/* Sets the extra spacing between children. */
	void setSpacing(int spacing);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const override;
	/* Sets the layout of the specified child. */
	void updateChildLayout(ControlSPtr control) override;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onChildAdded(ControlSPtr sender, ChildModifiedEventArgs& e);
	void onChildRemoved(ControlSPtr sender, ChildModifiedEventArgs& e);
	void onChildLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(StackPanel);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
