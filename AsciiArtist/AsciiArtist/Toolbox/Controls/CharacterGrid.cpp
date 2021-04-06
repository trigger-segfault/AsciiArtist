/*=================================================================|
|* File:				CharacterGrid.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "CharacterGrid.h"
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

CharacterGrid::CharacterGrid()
 :	// Settings
	singular(false),
	allowAttributes(true),
	
	image(nullptr),
	primaryPixel('A', 0x07),
	secondaryPixel(' ', 0x00),
	hoverPoint(-Point2I::ONE),
	pressPoint(-Point2I::ONE),
	secondaryPress(false),
	// Events
	_eventPixelChanged(EventDirections::FirstInFirstCalled) {

	eventPaint().add(bindEvent(CharacterGrid::onPaint));
	eventMouseMoved().add(bindEvent(CharacterGrid::onMouseMoved));
	eventMouseButton().add(bindEvent(CharacterGrid::onMouseButton));

	image = std::make_shared<AsciiImage>();
	image->loadResource(ASC_CHARACTER_GRID);
	setSizeSafe(Point2I(20, SIZE_AUTO));

	primaryCharPoint = findCharPoint(primaryPixel);
	secondaryCharPoint = findCharPoint(secondaryPixel);
}
void CharacterGrid::onInitialize(ControlSPtr sender, EventArgs& e) {}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

Pixel CharacterGrid::getDrawingPixel(bool secondary) const {
	return (secondary ? secondaryPixel : primaryPixel);
}
void CharacterGrid::setDrawingPixel(Pixel pixel, bool secondary) {
	(secondary ? secondaryPixel : primaryPixel) = pixel;
	(secondary ? secondaryCharPoint : primaryCharPoint) = findCharPoint(pixel);
}
bool CharacterGrid::isSingular() const {
	return singular;
}
void CharacterGrid::setSingular(bool singular) {
	this->singular = singular;
	if (singular)
		secondaryCharPoint = -Point2I::ONE;
}
bool CharacterGrid::getAllowAttributes() const {
	return allowAttributes;
}
void CharacterGrid::setAllowAttributes(bool allowAttributes) {
	this->allowAttributes = allowAttributes;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<PixelChangedEventArgs>& CharacterGrid::eventPixelChanged() {
	return _eventPixelChanged;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I CharacterGrid::autoSizeBehavior() const {
	return image->size();
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

Point2I CharacterGrid::findCharPoint(Pixel pixel) const {
	bool attributeChar = !getFlag(pixel.attributes, PixelAttributes::Char);
	for (Point2I p = Point2I::ZERO; p.x < image->size().x; p.x++) {
		for (p.y = 0; p.y < image->size().y; p.y++) {
			Pixel input = image->getPixel(p);
			if (attributeChar && input.matches(PixelRepresentation::TRANSPARENT_CHAR)) {
				return p;
			}
			else if (input.color == toColor(BLACK, GRAY)) {
				if (input.character == pixel.character) {
					return p;
				}
				else if (input.character == ' ' && (pixel.character == '\0' || pixel.character == 255)) {
					return p;
				}
			}
		}
	}
	return -Point2I::ONE;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void CharacterGrid::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	// Draw the color palette
	g->drawImage(Point2I::ZERO, *image);

	if (!allowAttributes) {
		Point2I p;
		for (p.x = 0; p.x < image->size().x; p.x++) {
			for (p.y = 0; p.y < image->size().y; p.y++) {
				if (image->getPixel(p).matches(PixelRepresentation::TRANSPARENT_CHAR))
					g->setColor(p, toColor(BLACK, DARK_GRAY));
			}
		}
	}

	const unsigned char hoverColor = toColor(BLACK, WHITE);
	const unsigned char pressColor = toColor(BLACK, DARK_GRAY);
	const unsigned char selectColor = toColor(BLUE, WHITE);
	const unsigned char selectHoverColor = toColor(BLUE, GRAY);
	const unsigned char selectPressColor = toColor(BLUE, DARK_GRAY);

	const unsigned char transHoverColor = toColor(BLACK, CYAN);
	const unsigned char transPressColor = toColor(BLACK, DARK_CYAN);
	const unsigned char transSelectColor = toColor(BLUE, CYAN);
	const unsigned char transSelectHoverColor = toColor(WHITE, CYAN);
	const unsigned char transSelectPressColor = toColor(BLUE, DARK_CYAN);

	if (image->getPixel(hoverPoint).matches(PixelRepresentation::TRANSPARENT_CHAR))
		g->setColor(hoverPoint, transHoverColor);
	else
		g->setColor(hoverPoint, hoverColor);

	if (pressPoint != -Point2I::ONE) {
		if (image->getPixel(pressPoint).matches(PixelRepresentation::TRANSPARENT_CHAR))
			g->setColor(pressPoint, transPressColor);
		else
			g->setColor(pressPoint, pressColor);
	}

	if (image->getPixel(primaryCharPoint).matches(PixelRepresentation::TRANSPARENT_CHAR)) {
		if (pressPoint == primaryCharPoint)
			g->setColor(primaryCharPoint, transSelectPressColor);
		else if (hoverPoint == primaryCharPoint)
			g->setColor(primaryCharPoint, transSelectHoverColor);
		else
			g->setColor(primaryCharPoint, transSelectColor);
	}
	else {
		if (pressPoint == primaryCharPoint)
			g->setColor(primaryCharPoint, selectPressColor);
		else if (hoverPoint == primaryCharPoint)
			g->setColor(primaryCharPoint, selectHoverColor);
		else
			g->setColor(primaryCharPoint, selectColor);
	}
	if (image->getPixel(secondaryCharPoint).matches(PixelRepresentation::TRANSPARENT_CHAR)) {
		if (pressPoint == secondaryCharPoint)
			g->setColor(secondaryCharPoint, transSelectPressColor);
		else if (hoverPoint == secondaryCharPoint)
			g->setColor(secondaryCharPoint, transSelectHoverColor);
		else
			g->setColor(secondaryCharPoint, transSelectColor);
	}
	else {
		if (pressPoint == secondaryCharPoint)
			g->setColor(secondaryCharPoint, selectPressColor);
		else if (hoverPoint == secondaryCharPoint)
			g->setColor(secondaryCharPoint, selectHoverColor);
		else
			g->setColor(secondaryCharPoint, selectColor);
	}
}
void CharacterGrid::onMouseMoved(ControlSPtr sender, MouseEventArgs& e) {
	hoverPoint = -Point2I::ONE;
	if (e.newPosition >= 0 && e.newPosition < image->size()) {
		Pixel input = image->getPixel(e.newPosition);
		if ((input.matches(PixelRepresentation::TRANSPARENT_CHAR) && allowAttributes) || input.color == toColor(BLACK, GRAY)) {
			hoverPoint = e.newPosition;
		}
	}
}
void CharacterGrid::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left) || (e.isButtonPressed(MouseButtons::Right) && !singular)) {
		if (pressPoint == -Point2I::ONE) {
			secondaryPress = e.isButtonPressed(MouseButtons::Right);
			Pixel input = image->getPixel(e.newPosition);
			if ((input.matches(PixelRepresentation::TRANSPARENT_CHAR) && allowAttributes) || input.color == toColor(BLACK, GRAY)) {
				pressPoint = e.newPosition;
			}
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left) || (e.isButtonReleased(MouseButtons::Right) && !singular)) {
		if (pressPoint != -Point2I::ONE && secondaryPress == e.isButtonReleased(MouseButtons::Right)) {
			Pixel input = image->getPixel(pressPoint);
			bool secondary = e.isButtonReleased(MouseButtons::Right);
			Pixel& output = (secondary ? secondaryPixel : primaryPixel);
			output = PixelRepresentation::changePixelFromPart(output, input, PixelParts::Character);
			(secondary ? secondaryCharPoint : primaryCharPoint) = findCharPoint(output);
			pressPoint = -Point2I::ONE;

			_eventPixelChanged(shared_from_this(), PixelChangedEventArgs(output, secondary));
		}
	}
}

#pragma endregion
//=================================================================|
