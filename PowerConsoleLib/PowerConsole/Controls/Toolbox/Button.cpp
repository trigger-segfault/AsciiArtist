/*=================================================================|
|* File:				Button.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Button.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Controls/Window.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Button::Button() 
 :	text("Button"),
	textAlignment(HorizontalAlignments::Center) {
	eventInitialize().add(bindEvent(Button::onInitialize));
	eventPaint().add(bindEvent(Button::onPaint));
	eventKey().add(bindEvent(Button::onKey));

	setSizeSafe(Point2I(SIZE_AUTO));
}
void Button::onInitialize(ControlSPtr sender, EventArgs& e) {

}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string Button::getText() const {
	return text;
}
void Button::setText(const std::string& text) {
	this->text = text;
	if (getSize().x == SIZE_AUTO)
		updateLayout(true, false, false, false, false);
}
HorizontalAlignments Button::getTextAlignment() const {
	return textAlignment;
}
void Button::setTextAlignment(HorizontalAlignments textAlignment) {
	this->textAlignment = textAlignment;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I Button::autoSizeBehavior() const {
	return Point2I((int)text.length(), 1);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Button::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	unsigned char color = toColor(BLACK, GRAY);
	if (!isEnabled())
		color = toColor(BLACK, DARK_GRAY);
	else if (isMouseDown())
		color = toColor(BLACK, DARK_GRAY);
	else if (isMouseInside())
		color = toColor(BLACK, WHITE);
	e.graphics->drawString(Point2I::ZERO, text, Pixel(' ', color), textAlignment, getActualSize().x);
}
void Button::onKey(ControlSPtr sender, KeyboardEventArgs& e) {
	if (e.isKeyPressed(Keys::Enter)) {
		eventClick()(shared_from_this(), MouseEventArgs(
			getWindow()->mouse().getPosition() - getLocalPosition(),
			getMousePositionLast()
		));
	}
}

#pragma endregion
//=================================================================|
