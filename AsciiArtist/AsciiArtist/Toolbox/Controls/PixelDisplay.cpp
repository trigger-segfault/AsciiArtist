/*=================================================================|
|* File:				PixelDisplay.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PixelDisplay.h"
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <AsciiArtist/Editing/PixelRepresentation.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Toolbox;
using namespace AsciiArtist::Toolbox::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

PixelDisplay::PixelDisplay()
 :	// Settings
	singular(false),
	allowInput(true),
	leftToRight(true),

	primaryPixel('A', 0x07),
	secondaryPixel(' ', 0x00),

	// Events
	_eventPixelChanged(EventDirections::FirstInFirstCalled) {

	setSizeSafe(Point2I(10, 1));

	eventPaint().add(bindEvent(PixelDisplay::onPaint));
	eventMouseButton().add(bindEvent(PixelDisplay::onMouseButton));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

Pixel PixelDisplay::getDrawingPixel(bool secondary) const {
	return (secondary ? secondaryPixel : primaryPixel);
}
void PixelDisplay::setDrawingPixel(Pixel pixel, bool secondary) {
	(secondary ? secondaryPixel : primaryPixel) = pixel;
}
bool PixelDisplay::isSingular() const {
	return singular;
}
void PixelDisplay::setSingular(bool singular) {
	this->singular = singular;
	setSize(Point2I(!singular ? 10 : 5, 1));
}
bool PixelDisplay::getAllowInput() const {
	return allowInput;
}
void PixelDisplay::setAllowInput(bool allowInput) {
	this->allowInput = allowInput;
}
bool PixelDisplay::isLeftToRight() const {
	return leftToRight;
}
void PixelDisplay::setLeftToRight(bool leftToRight) {
	this->leftToRight = leftToRight;
}
bool PixelDisplay::getShowCharacter() const {
	return showCharacter;
}
void PixelDisplay::setShowCharacter(bool showCharacter) {
	this->showCharacter = showCharacter;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<PixelChangedEventArgs>& PixelDisplay::eventPixelChanged() {
	return _eventPixelChanged;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void PixelDisplay::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	Point2I pixelPoint = Point2I::ZERO;
	if (leftToRight) {
		if (singular)
			pixelPoint = Point2I(showCharacter ? 4 : 3, 0);
		else
			pixelPoint = Point2I(showCharacter ? 7 : 6, 0);
	}
	Point2I partPoint = Point2I::ZERO;
	if (!leftToRight) {
		if (singular)
			partPoint = Point2I(2, 0);
		else
			partPoint = Point2I(showCharacter ? 4 : 6, 0);
	}

	int offset = (showCharacter ? 0 : -1);

	g->setPixel(pixelPoint, primaryPixel);

	if (showCharacter)
		g->setPixel(partPoint, PixelRepresentation::getPixelPart(primaryPixel, PixelParts::Character));
	g->setPixel(partPoint + Point2I(1 + offset, 0), PixelRepresentation::getPixelPart(primaryPixel, PixelParts::Foreground));
	g->setPixel(partPoint + Point2I(2 + offset, 0), PixelRepresentation::getPixelPart(primaryPixel, PixelParts::Background));

	if (!singular) {
		g->setPixel(pixelPoint + Point2I(2, 0), secondaryPixel);

		if (showCharacter)
			g->setPixel(partPoint + Point2I(3, 0), PixelRepresentation::getPixelPart(secondaryPixel, PixelParts::Character));
		g->setPixel(partPoint + Point2I(4 + offset * 2, 0), PixelRepresentation::getPixelPart(secondaryPixel, PixelParts::Foreground));
		g->setPixel(partPoint + Point2I(5 + offset * 2, 0), PixelRepresentation::getPixelPart(secondaryPixel, PixelParts::Background));
	}
}
void PixelDisplay::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (allowInput && e.isButtonPressed(MouseButtons::Middle)) {
		Point2I partPoint = Point2I::ZERO;
		if (!leftToRight) {
			if (singular)
				partPoint = Point2I(2, 0);
			else
				partPoint = Point2I(showCharacter ? 4 : 6, 0);
		}

		Rectangle2I swap1 = Rectangle2I(partPoint + Point2I(showCharacter ? 1 : 0, 0), 2, 1);
		Rectangle2I swap2 = Rectangle2I(partPoint + Point2I(showCharacter ? 4 : 2, 0), 2, 1);
		if (swap1.containsPoint(e.newPosition) || swap2.containsPoint(e.newPosition)) {
			// Swap colors
			bool secondary = swap2.containsPoint(e.newPosition);
			Pixel& output = (secondary ? secondaryPixel : primaryPixel);
			PixelAttributes fore = (PixelAttributes)((unsigned int)(output.attributes & PixelAttributes::BackgroundAll) >> 1);
			PixelAttributes back = (PixelAttributes)((unsigned int)(output.attributes & PixelAttributes::ForegroundAll) << 1);
			output.color = swapColor(output.color);
			setFlag(output.attributes, PixelAttributes::ColorAll, false);
			setFlag(output.attributes, fore | back, true);
			_eventPixelChanged(shared_from_this(), PixelChangedEventArgs(output, secondary));
		}
	}
}

#pragma endregion
//=================================================================|
