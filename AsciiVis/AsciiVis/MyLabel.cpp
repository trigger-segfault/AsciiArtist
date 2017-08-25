/*=================================================================|
|* File:				Label.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "MyLabel.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <AsciiVis/Colors.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

MyLabel::MyLabel()
	: text("Label"),
	textAlignment(HorizontalAlignments::Left) {
	eventInitialize().add(bindEvent(MyLabel::onInitialize));
	eventPaint().add(bindEvent(MyLabel::onPaint));

	setSizeSafe(Point2I(SIZE_AUTO));
}
void MyLabel::onInitialize(ControlSPtr sender, EventArgs& e) {

}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string MyLabel::getText() const {
	return text;
}
void MyLabel::setText(const std::string& text) {
	this->text = text;
	if (getSize().x == SIZE_AUTO)
		updateLayout(true, false, false, false, false);
}
HorizontalAlignments MyLabel::getTextAlignment() const {
	return textAlignment;
}
void MyLabel::setTextAlignment(HorizontalAlignments textAlignment) {
	this->textAlignment = textAlignment;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I MyLabel::autoSizeBehavior() const {
	return Point2I((int)text.length(), 1);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void MyLabel::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	unsigned char color = toColor(DARK_NAVY, LIGHT_LIGHT_NAVY);
	if (!isEnabled())
		color = toColor(DARK_NAVY, NAVY);
	e.graphics->drawString(Point2I::ZERO, text, Pixel(' ', color), textAlignment, getActualSize().x);
}

#pragma endregion
//=================================================================|
