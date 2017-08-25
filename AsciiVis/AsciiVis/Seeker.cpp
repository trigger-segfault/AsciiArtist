/*=================================================================|
|* File:				Seeker.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Seeker.h"
#include <AsciiVis/MusicLibrary.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Controls/Window.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Seeker::Seeker() :
	hover(false),
	dragging(false) {

	eventInitialize().add(bindEvent(Seeker::onInitialize));
	eventPaint().add(bindEvent(Seeker::onPaint));
	eventTick().add(bindEvent(Seeker::onTick));
	eventMouseMovedGlobal().add(bindEvent(Seeker::onMouseMovedGlobal));
	eventMouseButton().add(bindEvent(Seeker::onMouseButton));
}
void Seeker::onInitialize(ControlSPtr sender, EventArgs& e) {
	
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties


#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

int Seeker::getSeekerWidth() const {
	sf::Time dur = MusicLibrary::getMusic().getDuration();
	std::string str = "";
	bool showHours = (dur.asSeconds() / 60 / 60 >= 1);
	if (showHours) {
		str += std::to_string((int)(dur.asSeconds() / 60 / 60)) + ":";
	}
	str +=
		(((int)dur.asSeconds() / 60) % 60 < 10 && showHours ? "0" : "") +
		std::to_string(((int)dur.asSeconds() / 60) % 60) + ":" +
		((int)dur.asSeconds() % 60 < 10 ? "0" : "") +
		std::to_string((int)dur.asSeconds() % 60);
	return getActualSize().x - ((int)str.length() * 2 + 2);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Seeker::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	sf::Time times[2] = {
		MusicLibrary::getMusic().getPlayingOffset(),
		MusicLibrary::getMusic().getDuration()
	};
	std::string timeStrs[2] = { "", "" };
	for (int i = 0; i < 2; i++) {
		bool showHours = (times[i].asSeconds() / 60 / 60 >= 1);
		if (showHours) {
			timeStrs[i] += std::to_string((int)(times[i].asSeconds() / 60 / 60)) + ":";
		}
		timeStrs[i] +=
			(((int)times[i].asSeconds() / 60) % 60 < 10 && showHours ? "0" : "") +
			std::to_string(((int)times[i].asSeconds() / 60) % 60) + ":" +
			((int)times[i].asSeconds() % 60 < 10 ? "0" : "") +
			std::to_string((int)times[i].asSeconds() % 60);
	}

	int seekerWidth = getSeekerWidth();
	float floatPosition = MusicLibrary::getMusic().getPlayingOffset().asSeconds() / MusicLibrary::getMusic().getDuration().asSeconds();
	int seekerPosition = GMath::min(seekerWidth - 1, (int)(floatPosition * seekerWidth));

	g->drawRectangle(Rectangle2I(seekerWidth, 1), Pixel(Borders::LINE_THIN_HORIZONTAL, toColor(BLACK, LIGHT_NAVY)), true);
	g->setPixel(Point2I(seekerPosition, 0), Pixel(Characters::BLOCK_FULL, toColor(BLACK, (hover || dragging ? WHITE : LIGHT_LIGHT_NAVY))));

	std::string combinedTimes = timeStrs[0] + "/" + timeStrs[1];
	g->drawString(Point2I(getActualSize().x - 1, 0), combinedTimes, Pixel(' ', toColor(BLACK, LIGHT_LIGHT_NAVY)), HorizontalAlignments::Right);
}
void Seeker::onTick(ControlSPtr sender, TickEventArgs& e) {
	onMouseMovedGlobal(shared_from_this(), MouseEventArgs(getMousePosition(), getMousePositionLast()));
}
void Seeker::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	hover = false;
	int seekerWidth = getSeekerWidth();
	
	float floatPosition = MusicLibrary::getMusic().getPlayingOffset().asSeconds() / MusicLibrary::getMusic().getDuration().asSeconds();
	int seekerPosition = GMath::min(seekerWidth - 1, (int)(floatPosition * seekerWidth));
	if (e.newPosition.x == seekerPosition && e.newPosition.y == 0) {
		hover = true;
	}
	e.newPosition.x = GMath::clamp(e.newPosition.x, 0, seekerWidth - 1);
	if (dragging && e.newPosition.x != seekerPosition) {
		sf::Time offset = sf::seconds((float)e.newPosition.x / (float)seekerWidth * MusicLibrary::getMusic().getDuration().asSeconds());
		MusicLibrary::getMusic().setPlayingOffset(offset);
	}
}
void Seeker::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	int seekerWidth = getSeekerWidth();
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (e.newPosition.x >= 0 && e.newPosition.x < seekerWidth) {
			dragging = true;
			if (!hover)
				onMouseMovedGlobal(shared_from_this(), e);
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left)) {
		dragging = false;
	}
}

#pragma endregion
//=================================================================|
