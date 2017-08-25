/*=================================================================|
|* File:				ToggleButton.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ToggleButton.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ToggleButton::ToggleButton()
	: checked(false) {
	eventClick().add(bindEvent(ToggleButton::onClick));
	eventPaint().pop(); // Remove the base button drawing
	eventPaint().add(bindEvent(ToggleButton::onPaint));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool ToggleButton::isChecked() const {
	return checked;
}
void ToggleButton::setChecked(bool checked) {
	this->checked = checked;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ToggleButton::onClick(ControlSPtr sender, MouseEventArgs& e) {
	checked = !checked;
}
void ToggleButton::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	unsigned char color = toColor(BLACK, GRAY);
	if (checked) {
		color = toColor(BLUE, WHITE);
		if (!isEnabled())
			color = toColor(DARK_GRAY, GRAY);
		else if (isMouseDown())
			color = toColor(BLUE, DARK_GRAY);
		else if (isMouseInside())
			color = toColor(BLUE, GRAY);
	}
	else {
		if (!isEnabled())
			color = toColor(BLACK, DARK_GRAY);
		else if (isMouseDown())
			color = toColor(BLACK, DARK_GRAY);
		else if (isMouseInside())
			color = toColor(BLACK, WHITE);
	}
	e.graphics->drawString(Point2I::ZERO, getText(), Pixel(' ', color), getTextAlignment(), getActualSize().x);
	e.handled = true;
}

#pragma endregion
//=================================================================|
