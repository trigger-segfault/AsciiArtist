/*=================================================================|
|* File:				Seeker.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SEEKER_H
#define SEEKER_H

#include <PowerConsole/Controls/Control.h>
#include <AsciiVis/Colors.h>

using namespace PowerConsole::Controls;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Seeker : public Control {

	//========== CONSTANTS ===========
	#pragma region Constants

	static const int MAX_WIDTH = 26;
	static const int SCROLL_DELAY = 4000;
	static const int SCROLL_SPEED = 120;
	static const int SCROLL_SEPARATION = 4;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	bool hover;
	bool dragging;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	Seeker();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	int getSeekerWidth() const;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(Seeker);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
