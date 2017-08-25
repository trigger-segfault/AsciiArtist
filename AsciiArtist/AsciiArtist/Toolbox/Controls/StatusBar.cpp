/*=================================================================|
|* File:				StatusBar.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "StatusBar.h"

using namespace AsciiArtist::Toolbox::Controls;
using namespace PowerConsole::Drawing;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

StatusBar::StatusBar()
 :	message(""),
	messageColor(0x07),
	messageTimer(0) {
	eventInitialize().add(bindEvent(StatusBar::onInitialize));
	eventTick().add(bindEvent(StatusBar::onTick));
}
void StatusBar::onInitialize(ControlSPtr sender, EventArgs& e) {
	labelPoint = makeControl(Label);
	labelPoint->setSize(Point2I(19, 1));
	labelPoint->setMargin(Thickness(0, 0, 0, 0));
	labelPoint->setText(" Point: ");
	children().add(labelPoint);

	labelSize = makeControl(Label);
	labelSize->setSize(Point2I(18, 1));
	labelSize->setMargin(Thickness(22, 0, 0, 0));
	labelSize->setText(" Size: ");
	children().add(labelSize);

	panelMessage = makeControl(Panel);
	panelMessage->setSize(Point2I(SIZE_AUTO, 1));
	panelMessage->setMargin(Thickness(44, 0, 0, 0));
	panelMessage->setHorizontalAlignment(HorizontalAlignments::Stretch);
	panelMessage->eventPaint().add(bindEvent(StatusBar::onMessagePaint));
	children().add(panelMessage);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void StatusBar::sendMessage(const std::string& message, unsigned char messageColor) {
	this->message = message;
	this->messageColor = messageColor;
	this->messageTimer = MESSAGE_DURATION;
}

void StatusBar::setLabelPoint(Point2I point) {
	if (point == -Point2I::ONE)
		labelPoint->setText(" Point: ");
	else
		labelPoint->setText(" Point: " + std::to_string(point.x) + ", " + std::to_string(point.y));
}
void StatusBar::setLabelSize(Point2I size) {
	if (size == -Point2I::ONE)
		labelSize->setText(" Size: ");
	else
		labelSize->setText(" Size: " + std::to_string(size.x) + ", " + std::to_string(size.y));
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void StatusBar::onMessagePaint(ControlSPtr sender, PaintEventArgs& e) {
	if (messageTimer > 0)
		e.graphics->drawString(Point2I::ZERO, message, Pixel(' ', messageColor));
}
void StatusBar::onTick(ControlSPtr sender, TickEventArgs& e) {
	if (messageTimer > 0)
		messageTimer -= e.timeSinceLastTick;
}

#pragma endregion
//=================================================================|
