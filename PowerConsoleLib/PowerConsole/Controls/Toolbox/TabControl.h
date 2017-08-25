/*=================================================================|
|* File:				TabControl.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TAB_CONTROL_H
#define TAB_CONTROL_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/TabCollection.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class TabControl : public Control {
	//=========== MEMBERS ============
	#pragma region Members

	/* The collection of tabs. */
	TabCollection collection;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tab control. */
	TabControl();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties


	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Returns true if the control or any of its children has the specified name. */
	bool containsName(const std::string& name) const override;

	/* Gets the collection of all the children contained by this control. */
	ControlCollection& children();
	/* Gets the collection of all the children contained by this control. */
	const ControlCollection& children() const;

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Updates the layout of the control after changes have been made to the control's layout properties. */
	//virtual void updateLayout(bool sizeChanged, bool marginChanged, bool alignmentChanged, bool positionChanged, bool visibleAreaChanged);
	/* Sets the layout of the specified child. */
	virtual void updateChildLayout(ControlSPtr control);

	/* Iterates through all controls contained within this one. */
	void forEach(std::function<void(ControlSPtr control)> func) override;
	/* Draws the control to the graphics object. */
	void repaint(PcDraw::GraphicsSPtr g) override;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when a child has been added to the collection. */
	Event<ChildModifiedEventArgs>& eventChildAdded();
	/* Called when a child has been removed from the collection. */
	Event<ChildModifiedEventArgs>& eventChildRemoved();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onKey(ControlSPtr sender, KeyboardEventArgs& e);
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onChildAdded(ControlSPtr sender, ChildModifiedEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
