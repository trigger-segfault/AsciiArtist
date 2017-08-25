/*=================================================================|
|* File:				ColorPalette.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resources.h"
#include "ColorPalette.h"
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

ColorPalette::ColorPalette()
 :	// Settings
	singular(false),
	allowAttributes(true),

	image(nullptr),
	primaryPixel('A', 0x07),
	secondaryPixel(' ', 0x00),
	hoverPoint(-Point2I::ONE),
	// Events
	_eventPixelChanged(EventDirections::FirstInFirstCalled) {
	setSizeSafe(Point2I(10, 2));

	eventPaint().add(bindEvent(ColorPalette::onPaint));
	eventMouseMoved().add(bindEvent(ColorPalette::onMouseMoved));
	eventMouseButton().add(bindEvent(ColorPalette::onMouseButton));

	image = std::make_shared<AsciiImage>();
	image->loadResource(ASC_COLOR_PALETTE);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

Pixel ColorPalette::getDrawingPixel(bool secondary) const {
	return (secondary ? secondaryPixel : primaryPixel);
}
void ColorPalette::setDrawingPixel(Pixel pixel, bool secondary) {
	(secondary ? secondaryPixel : primaryPixel) = pixel;
}
bool ColorPalette::isSingular() const {
	return singular;
}
void ColorPalette::setSingular(bool singular) {
	this->singular = singular;
}
bool ColorPalette::getAllowAttributes() const {
	return allowAttributes;
}
void ColorPalette::setAllowAttributes(bool allowAttributes) {
	this->allowAttributes = allowAttributes;
	setSize(Point2I(allowAttributes ? 10 : 8, 2));
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<PixelChangedEventArgs>& ColorPalette::eventPixelChanged() {
	return _eventPixelChanged;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ColorPalette::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	// Draw the color palette
	if (allowAttributes)
		g->drawImage(Point2I::ZERO, *image);
	else
		g->drawImage(Point2I::ZERO, Rectangle2I(2, 0, 8, 2), *image);

	// Draw hovering in the color palette
	if (hoverPoint != -Point2I::ONE) {
		Pixel input = image->getPixel(hoverPoint + Point2I(!allowAttributes ? 2 : 0, 0));
		if (input.character == Characters::GRADIENT_HEAVY)
			g->setPixel(hoverPoint, Pixel(Characters::GRADIENT_MEDIUM, input.color));
		else if (hoverPoint.y == 0)// && getFColor(input.color) != BLUE)
			g->setPixel(hoverPoint, Pixel(10, toColor(BLACK, getFColor(input.color))));
		else
			g->setPixel(hoverPoint, Pixel(10, toColor(WHITE, getFColor(input.color))));
	}
}
void ColorPalette::onMouseMoved(ControlSPtr sender, MouseEventArgs& e) {
	if (e.newPosition >= 0 && e.newPosition < getActualSize()) {
		hoverPoint = e.newPosition;
	}
	else {
		hoverPoint = -Point2I::ONE;
	}
}
void ColorPalette::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if ((e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right)) && e.newPosition < getActualSize()) {
		bool secondary = getFlag(e.modifiers, Modifiers::Shift) && !singular;
		Pixel& output = (!secondary ? primaryPixel : secondaryPixel);
		Pixel input = image->getPixel(e.newPosition);
		bool background = e.isButtonPressed(MouseButtons::Right);
		output = PixelRepresentation::changePixelFromPart(output, input, !background ? PixelParts::Foreground : PixelParts::Background);
		if (!singular || !secondary)
			_eventPixelChanged(shared_from_this(), PixelChangedEventArgs(output, secondary));
	}
}

#pragma endregion
//=================================================================|
