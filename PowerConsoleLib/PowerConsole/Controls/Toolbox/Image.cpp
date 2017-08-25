/*=================================================================|
|* File:				Image.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Image.h"
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

Image::Image()
	: image() {
	eventPaint().add(bindEvent(Image::onPaint));

	setSizeSafe(Point2I(SIZE_AUTO));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

const AsciiImageFrame& Image::getImage() const {
	return image;
}
void Image::setImage(const AsciiImageFrame& image) {
	this->image = image;
	if (getSize().x == SIZE_AUTO || getSize().y == SIZE_AUTO)
		updateLayout(true, false, false, false, false);
}
void Image::setImage(AsciiImageFrame&& image) {
	this->image = std::move(image);
	if (getSize().x == SIZE_AUTO || getSize().y == SIZE_AUTO)
		updateLayout(true, false, false, false, false);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I Image::autoSizeBehavior() const {
	return image.size();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Image::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	e.graphics->drawImage(Point2I::ZERO, image);
}

#pragma endregion
//=================================================================|
