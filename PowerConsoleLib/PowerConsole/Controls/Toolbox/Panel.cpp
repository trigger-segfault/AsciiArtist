/*=================================================================|
|* File:				Panel.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Panel.h"

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

Panel::Panel()
 :	backgroundImage(nullptr),
	backgroundStretched(false) {
	eventPaint().add(bindEvent(Panel::onPaint));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

AsciiImageFrameSPtr Panel::getBackgroundImage() const {
	return backgroundImage;
}
void Panel::setBackgroudImage(AsciiImageFrameSPtr backgroundImage) {
	this->backgroundImage = backgroundImage;
}

bool Panel::isBackgroundStretched() {
	return backgroundStretched;
}
void Panel::setBackgroundStretched(bool backgroundStretched) {
	this->backgroundStretched = backgroundStretched;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Panel::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	if (backgroundImage != nullptr) {
		e.graphics->drawImage(Point2I::ZERO, *backgroundImage);

		if (backgroundStretched) {
			// Stretch the background horizontally
			for (int i = backgroundImage->size().x; i < getActualSize().x; i++) {
				e.graphics->drawImage(Point2I(i, 0), Rectangle2I(backgroundImage->size().x - 1, 0, 1, backgroundImage->size().y), *backgroundImage);
			}
			// Stretch the background vertically
			for (int i = backgroundImage->size().y; i < getActualSize().y; i++) {
				e.graphics->drawImage(Point2I(0, i), Rectangle2I(0, backgroundImage->size().y - 1, backgroundImage->size().x, 1), *backgroundImage);
			}
		}
	}
}

#pragma endregion
//=================================================================|
