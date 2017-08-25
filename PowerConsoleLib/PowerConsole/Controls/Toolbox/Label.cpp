/*=================================================================|
|* File:				Label.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Label.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

Label::Label()
	: text("Label"),
	textAlignment(HorizontalAlignments::Left) {
	eventInitialize().add(bindEvent(Label::onInitialize));
	eventPaint().add(bindEvent(Label::onPaint));

	setSizeSafe(Point2I(SIZE_AUTO));
}
void Label::onInitialize(ControlSPtr sender, EventArgs& e) {
	
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string Label::getText() const {
	return text;
}
void Label::setText(const std::string& text) {
	this->text = text;
	if (getSize().x == SIZE_AUTO)
		updateLayout(true, false, false, false, false);
}
HorizontalAlignments Label::getTextAlignment() const {
	return textAlignment;
}
void Label::setTextAlignment(HorizontalAlignments textAlignment) {
	this->textAlignment = textAlignment;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I Label::autoSizeBehavior() const {
	return Point2I((int)text.length(), 1);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Label::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	unsigned char color = toColor(BLACK, GRAY);
	if (!isEnabled())
		color = toColor(BLACK, DARK_GRAY);
	e.graphics->drawString(Point2I::ZERO, text, Pixel(' ', color), textAlignment, getActualSize().x);
}

#pragma endregion
//=================================================================|
