/*=================================================================|
|* File:				TextSlider.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TextSlider.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace AsciiTetris::UI;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const float TextSlider::ZOOM_DISTANCE = 0.37f;
const float TextSlider::SLIDE_DISTANCE = 1.0f - (ZOOM_DISTANCE * 2);

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

TextSlider::TextSlider()
 :	text(""),
	timer(-1),
	
	// Events
	_eventAnimationFinished(EventDirections::FirstInFirstCalled) {

	eventInitialize().add(bindEvent(TextSlider::onInitialize));
	eventTick().add(bindEvent(TextSlider::onTick));
	eventPaint().add(bindEvent(TextSlider::onPaint));
}
void TextSlider::onInitialize(ControlSPtr sender, EventArgs& e) {
	setSize(Point2I(SIZE_AUTO));
	setHorizontalAlignment(HorizontalAlignments::Stretch);
	setVerticalAlignment(VerticalAlignments::Stretch);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string TextSlider::getText() const {
	return text;
}
bool TextSlider::isAnimating() const {
	return timer != -1;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void TextSlider::start(const std::string& text) {
	this->text = text;
	timer = 0;
}
void TextSlider::restart() {
	timer = 0;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<EventArgs>& TextSlider::eventAnimationFinished() {
	return _eventAnimationFinished;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void TextSlider::onTick(ControlSPtr sender, TickEventArgs& e) {
	if (timer != -1) {
		timer++;
		if (timer >= ZOOM_ANIM_LENGTH * 2 + SLIDE_ANIM_LENGTH + EXTRA_ANIM_DELAY) {
			timer = -1;
			_eventAnimationFinished(shared_from_this());
		}
	}
}
void TextSlider::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	if (timer != -1) {
		int y = getActualSize().y / 2 - 1;
		for (int i = 0; i < 3; i++) {
			int timerOffset = timer - EXTRA_ANIM_DELAY * i / 2;
			int mode = (timerOffset < ZOOM_ANIM_LENGTH ? 0 : (timerOffset < ZOOM_ANIM_LENGTH + SLIDE_ANIM_LENGTH ? 1 : 2));

			int x;
			if (mode == 0) {
				float ratio = (float)timerOffset / (float)ZOOM_ANIM_LENGTH;
				x = (int)(ratio * (ZOOM_DISTANCE * getActualSize().x + (int)text.length())) - (int)text.length();
			}
			else if (mode == 1) {
				float ratio = (float)(timerOffset - ZOOM_ANIM_LENGTH) / (float)SLIDE_ANIM_LENGTH;
				int startX = (int)(ZOOM_DISTANCE * getActualSize().x);
				x = startX + (int)(ratio * SLIDE_DISTANCE * getActualSize().x);
			}
			else {
				float ratio = (float)(timerOffset - ZOOM_ANIM_LENGTH - SLIDE_ANIM_LENGTH) / (float)ZOOM_ANIM_LENGTH;
				int startX = (int)((SLIDE_DISTANCE + ZOOM_DISTANCE) * getActualSize().x);
				x = startX + (int)(ratio * (ZOOM_DISTANCE * getActualSize().x + (int)text.length()));
			}
			g->drawString(Point2I(x, y + i), text, Pixel(' ', toColor(BLACK, DARK_GRAY), PixelAttributes::Char | PixelAttributes::FColor), HorizontalAlignments::Center);
		}
	}
}

#pragma endregion
//=================================================================|
