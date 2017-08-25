/*=================================================================|
|* File:				MyToggleButton.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "MyToggleButton.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <AsciiVis/Colors.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

MyToggleButton::MyToggleButton()
	: text("Toggle Button"),
	textAlignment(HorizontalAlignments::Center),
	checked(false) {

	eventClick().add(bindEvent(MyToggleButton::onClick));
	eventPaint().add(bindEvent(MyToggleButton::onPaint));

	setSizeSafe(Point2I(SIZE_AUTO));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string MyToggleButton::getText() const {
	return text;
}
void MyToggleButton::setText(const std::string& text) {
	this->text = text;
	if (getSize().x == SIZE_AUTO)
		updateLayout(true, false, false, false, false);
}
HorizontalAlignments MyToggleButton::getTextAlignment() const {
	return textAlignment;
}
void MyToggleButton::setTextAlignment(HorizontalAlignments textAlignment) {
	this->textAlignment = textAlignment;
}
bool MyToggleButton::isChecked() const {
	return checked;
}
void MyToggleButton::setChecked(bool checked) {
	this->checked = checked;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I MyToggleButton::autoSizeBehavior() const {
	return Point2I((int)text.length(), 1);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void MyToggleButton::onClick(ControlSPtr sender, MouseEventArgs& e) {
	checked = !checked;
}
void MyToggleButton::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	unsigned char color = toColor(DARK_NAVY, LIGHT_LIGHT_NAVY);
	if (checked) {
		color = toColor(LIGHT_NAVY, WHITE);
		if (!isEnabled())
			color = toColor(NAVY, LIGHT_NAVY);
		else if (isMouseDown())
			color = toColor(NAVY, LIGHT_NAVY);
		else if (isMouseInside())
			color = toColor(LIGHT_NAVY, WHITE);
	}
	else {
		if (!isEnabled())
			color = toColor(DARK_NAVY, NAVY);
		else if (isMouseDown())
			color = toColor(DARK_NAVY, NAVY);
		else if (isMouseInside())
			color = toColor(DARK_NAVY, WHITE);
	}
	e.graphics->drawString(Point2I::ZERO, text, Pixel(' ', color), textAlignment, getActualSize().x);
	e.handled = true;
}

#pragma endregion
//=================================================================|
