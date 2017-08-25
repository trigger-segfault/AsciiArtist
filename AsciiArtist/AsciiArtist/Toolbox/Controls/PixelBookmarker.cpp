/*=================================================================|
|* File:				PixelBookmarker.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PixelBookmarker.h"
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

PixelBookmarker::PixelBookmarker()
 :	// Settings
	hoverIndex(-1),
	pressIndex(-1),
	secondaryPress(false),

	primaryPixel('A', 0x07),
	secondaryPixel(' ', 0x00),

	// Events
	_eventPixelChanged(EventDirections::FirstInFirstCalled) {

	for (int i = 0; i < 10; i++) {
		bookmarkedPixels[i] = Pixel(' ', 0x00);
	}

	setSizeSafe(Point2I(20, 2));

	eventInitialize().add(bindEvent(PixelBookmarker::onInitialize));
	eventPaint().add(bindEvent(PixelBookmarker::onPaint));
	eventMouseMoved().add(bindEvent(PixelBookmarker::onMouseMoved));
	eventMouseButton().add(bindEvent(PixelBookmarker::onMouseButton));
}
void PixelBookmarker::onInitialize(ControlSPtr sender, EventArgs& e) {
	/*for (int i = 0; i < 10; i++) {
		int realNumber = (i + 1) % 10;
		std::string realNumberStr = std::to_string(realNumber);
		commands().add(Command(realNumberStr + " Assign", Hotkey((Keys)((int)Keys::D0 + realNumber), Modifiers::Shift), false,
			bindEvent(PixelBookmarker::onAssignCommand, i)
		));
		commands().add(Command(realNumberStr + " Select", Hotkey((Keys)((int)Keys::D0 + realNumber)), false,
			bindEvent(PixelBookmarker::onSelectCommand, i)
		));
	}*/
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

Pixel PixelBookmarker::getDrawingPixel(bool secondary) const {
	return (secondary ? secondaryPixel : primaryPixel);
}
void PixelBookmarker::setDrawingPixel(Pixel pixel, bool secondary) {
	(secondary ? secondaryPixel : primaryPixel) = pixel;
}
Pixel PixelBookmarker::getBookmarkedPixel(int index) const {
	return bookmarkedPixels[index];
}
void PixelBookmarker::setBookmarkedPixel(int index, Pixel pixel) {
	bookmarkedPixels[index] = pixel;
}
void PixelBookmarker::selectBookmarkedPixel(int index) {
	setDrawingPixel(bookmarkedPixels[index]);
	_eventPixelChanged(shared_from_this(), PixelChangedEventArgs(bookmarkedPixels[index], false));
}
void PixelBookmarker::assignBookmarkedPixel(int index) {
	bookmarkedPixels[index] = getDrawingPixel();
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<PixelChangedEventArgs>& PixelBookmarker::eventPixelChanged() {
	return _eventPixelChanged;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void PixelBookmarker::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char normalColor = toColor(BLACK, GRAY);
	const unsigned char hoverColor = toColor(BLACK, WHITE);
	const unsigned char pressColor = toColor(BLACK, DARK_GRAY);
	unsigned char color = normalColor;

	for (int i = 0; i < 10; i++) {
		if (i == pressIndex)
			color = pressColor;
		else if (i == hoverIndex)
			color = hoverColor;
		else
			color = normalColor;
		Point2I point = Point2I(1 + (i % 5) * 4, i / 5);
		g->drawString(point, std::to_string((i + 1) % 10), Pixel(' ', color));
		g->setPixel(point + Point2I(1, 0), bookmarkedPixels[i]);
	}
}
void PixelBookmarker::onMouseMoved(ControlSPtr sender, MouseEventArgs& e) {
	hoverIndex = -1;
	for (int i = 0; i < 10; i++) {
		Rectangle2I bounds = Rectangle2I(1 + (i % 5) * 4, i / 5, 2, 1);
		if (bounds.containsPoint(e.newPosition)) {
			hoverIndex = i;
			break;
		}
	}
}
void PixelBookmarker::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (hoverIndex != -1 && pressIndex == -1 && (e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right))) {
		pressIndex = hoverIndex;
		secondaryPress = e.isButtonPressed(MouseButtons::Right);
		bool assign = getFlag(e.modifiers, Modifiers::Shift);
		if (assign) {
			bookmarkedPixels[hoverIndex] = getDrawingPixel(secondaryPress);
		}
		else {
			setDrawingPixel(bookmarkedPixels[hoverIndex], secondaryPress);
			_eventPixelChanged(shared_from_this(), PixelChangedEventArgs(bookmarkedPixels[hoverIndex], secondaryPress));
		}
	}
	else if (pressIndex != -1 && (e.isButtonReleased(MouseButtons::Left) || e.isButtonReleased(MouseButtons::Right))) {
		if (secondaryPress == e.isButtonReleased(MouseButtons::Right))
			pressIndex = -1;
	}
}

#pragma endregion
//======= COMMAND HANDLERS =======
#pragma region Command Handlers

void PixelBookmarker::onAssignCommand(ControlSPtr sender, EventArgs& e, int index) {
	bookmarkedPixels[index] = getDrawingPixel();
}
void PixelBookmarker::onSelectCommand(ControlSPtr sender, EventArgs& e, int index) {
	setDrawingPixel(bookmarkedPixels[index]);
	_eventPixelChanged(shared_from_this(), PixelChangedEventArgs(bookmarkedPixels[index], false));
}

#pragma endregion
//=================================================================|
