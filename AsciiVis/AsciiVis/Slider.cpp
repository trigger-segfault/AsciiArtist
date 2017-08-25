/*=================================================================|
|* File:				Slider.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Slider.h"
#include <AsciiVis/MusicLibrary.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Controls/Window.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Slider::Slider() :
	value(0.0f),
	hover(false),
	dragging(false),
	
	_eventValueChanged(EventDirections::FirstInFirstCalled) {

	eventPaint().add(bindEvent(Slider::onPaint));
	eventTick().add(bindEvent(Slider::onTick));
	eventMouseMovedGlobal().add(bindEvent(Slider::onMouseMovedGlobal));
	eventMouseButton().add(bindEvent(Slider::onMouseButton));

	setSizeSafe(Point2I(14, 1));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties


float Slider::getValue() const {
	return value;
}
void Slider::setValue(float value) {
	this->value = GMath::clamp(value, 0.0f, 1.0f);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<FloatChangedEventArgs>& Slider::eventValueChanged() {
	return _eventValueChanged;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Slider::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	
	int position = GMath::min(getActualSize().x - 1, (int)(value * (getActualSize().x - 1)));

	g->drawRectangle(Rectangle2I(getActualSize().x, 1), Pixel(Borders::LINE_THIN_HORIZONTAL, toColor(DARK_NAVY, LIGHT_NAVY)), true);
	g->setPixel(Point2I(position, 0), Pixel(Characters::BLOCK_FULL, toColor(DARK_NAVY, (hover || dragging ? WHITE : LIGHT_LIGHT_NAVY))));
}
void Slider::onTick(ControlSPtr sender, TickEventArgs& e) {
	onMouseMovedGlobal(shared_from_this(), MouseEventArgs(getMousePosition(), getMousePositionLast()));
}
void Slider::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	hover = false;

	int position = GMath::min(getActualSize().x - 1, (int)(value * (getActualSize().x - 1)));

	if (e.newPosition.x == position && e.newPosition.y == 0) {
		hover = true;
	}
	e.newPosition.x = GMath::clamp(e.newPosition.x, 0, getActualSize().x - 1);
	if (dragging && e.newPosition.x != position) {
		float oldValue = value;
		value = GMath::clamp((float)e.newPosition.x / (float)(getActualSize().x - 1), 0.0f, 1.0f);
		_eventValueChanged(shared_from_this(), FloatChangedEventArgs(value, oldValue));
	}
}
void Slider::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (e.newPosition.x >= 0 && e.newPosition.x < getActualSize().x) {
			dragging = true;
			if (!hover)
				onMouseMovedGlobal(shared_from_this(), e);
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left)) {
		dragging = false;
	}
}

#pragma endregion
//=================================================================|
