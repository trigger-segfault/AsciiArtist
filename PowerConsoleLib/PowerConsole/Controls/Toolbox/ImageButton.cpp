/*=================================================================|
|* File:				ImageButton.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ImageButton.h"
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

ImageButton::ImageButton()
 :	image() {
	eventInitialize().add(bindEvent(ImageButton::onInitialize));
	eventPaint().add(bindEvent(ImageButton::onPaint));

	setSizeSafe(Point2I(SIZE_AUTO));
}
void ImageButton::onInitialize(ControlSPtr sender, EventArgs& e) {

}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

const PcDraw::AsciiAnimation& ImageButton::getImage() const {
	return image;
}
void ImageButton::setImage(const PcDraw::AsciiAnimation& image) {
	this->image = image;
	if (getSize().x == SIZE_AUTO || getSize().y == SIZE_AUTO)
		updateLayout(true, false, false, false, false);
}
void ImageButton::setImage(PcDraw::AsciiAnimation&& image) {
	this->image = std::move(image);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I ImageButton::autoSizeBehavior() const {
	return image.size();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ImageButton::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	if (!isEnabled()) {
		if (image.containsName("Disabled"))
			g->drawImage(Point2I::ZERO, image["Disabled"]);
		else if (image.containsName("Pressed"))
			g->drawImage(Point2I::ZERO, image["Pressed"]);
		else if (image.containsName("Normal"))
			g->drawImage(Point2I::ZERO, image["Normal"]);
		else
			g->drawImage(Point2I::ZERO, image[0]);
	}
	else if (isMouseDown()) {
		if (image.containsName("Pressed"))
			g->drawImage(Point2I::ZERO, image["Pressed"]);
		else if (image.containsName("Hover"))
			g->drawImage(Point2I::ZERO, image["Hover"]);
		else if (image.containsName("Normal"))
			g->drawImage(Point2I::ZERO, image["Normal"]);
		else
			g->drawImage(Point2I::ZERO, image[0]);
	}
	else if (isMouseInside()) {
		if (image.containsName("Hover"))
			g->drawImage(Point2I::ZERO, image["Hover"]);
		else if (image.containsName("Normal"))
			g->drawImage(Point2I::ZERO, image["Normal"]);
		else
			g->drawImage(Point2I::ZERO, image[0]);
	}
	else {
		if (image.containsName("Normal"))
			g->drawImage(Point2I::ZERO, image["Normal"]);
		else
			g->drawImage(Point2I::ZERO, image[0]);
	}
}

#pragma endregion
//=================================================================|
