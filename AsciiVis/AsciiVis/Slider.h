/*=================================================================|
|* File:				Slider.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SLIDER_H
#define SLIDER_H

#include <PowerConsole/Controls/Control.h>
#include <AsciiVis/Colors.h>

using namespace PowerConsole::Controls;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Slider : public Control {

	//========== CONSTANTS ===========
	#pragma region Constants


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	float value;
	bool hover;
	bool dragging;

	// Events
	Event<FloatChangedEventArgs> _eventValueChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	Slider();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	float getValue() const;
	void setValue(float value);

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
public:
	Event<FloatChangedEventArgs>& eventValueChanged();

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

DECLARE_SMART_PTRS(Slider);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
