/*=================================================================|
|* File:				NowPlaying.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "NowPlaying.h"
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

NowPlaying::NowPlaying() :
	timerTitle(0),
	timerArtist(0),
	timerAlbum(0),
	waitingForMetadata(true) {

	eventInitialize().add(bindEvent(NowPlaying::onInitialize));
	eventPaint().add(bindEvent(NowPlaying::onPaint));
	eventTick().add(bindEvent(NowPlaying::onTick));

	setSizeSafe(Point2I(SIZE_AUTO));
}
void NowPlaying::onInitialize(ControlSPtr sender, EventArgs& e) {
	setHorizontalAlignment(HorizontalAlignments::Right);
	setMargin(Thickness(0, 1, 1, 0));
	updatePlayIndex();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void NowPlaying::updatePlayIndex() {
	MusicDataCSPtr data = MusicLibrary::getMusicData();
	if (data == nullptr) {
		setVisible(false);
		return;
	}
	setVisible(true);

	title = data->title;
	artist = data->artist;
	album = data->album;

	scrollLength = std::max(
		(int)title.length(),
	std::max(
		(int)artist.length() + 1,
		(int)album.length() + 1
	));

	int height = 1;
	if (!artist.empty())
		height++;
	if (!album.empty())
		height++;
	setSize(Point2I(std::min(scrollLength, MAX_WIDTH) + 4, height + 2));

	scrollLength += SCROLL_SEPARATION;

	timerTitle = 0;
	timerArtist = 0;
	timerAlbum = 0;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void NowPlaying::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	g->drawRectangle(Rectangle2I(getActualSize()), Pixel(' ', toColor(DARK_BLUE, DARK_BLUE)), true);
	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', toColor(DARK_BLUE, LIGHT_NAVY)), false);

	unsigned char color = toColor(DARK_BLUE, LIGHT_LIGHT_NAVY);

	for (int i = 0; i < 3; i++) {
		std::string str = title;
		int timer = timerTitle;
		int offset = 0;
		if (i == 1) {
			offset = 1;
			if (!artist.empty()) {
				str = artist; timer = timerArtist;
			}
			else if (!album.empty()) {
				str = album; timer = timerAlbum;
			}
			else
				break;
		}
		else if (i == 2) {
			offset = 1;
			if (!artist.empty() && !album.empty()) {
				str = album; timer = timerAlbum;
			}
			else
				break;
		}

		int width = getActualSize().x - 4 - offset;
		int length = (int)str.length();

		int scrollPosition = (timer - SCROLL_DELAY) / SCROLL_SPEED;
		if (length <= width)
			scrollPosition = 0;
		int start = GMath::clamp(scrollPosition, 0, length);
		std::string firstStr = str.substr(GMath::clamp(scrollPosition, 0, length), GMath::min(width, length - start));
		g->drawString(Point2I(2 + offset, 1 + i), firstStr, Pixel(' ', color));

		int secondScrollPosition = GMath::max(0, -scrollPosition + length + SCROLL_SEPARATION);
		if (scrollPosition > 0 && secondScrollPosition < width) {
			int secondLength = width - secondScrollPosition;
			std::string secondStr = str.substr(0, secondLength);
			g->drawString(Point2I(2 + offset + secondScrollPosition, 1 + i), secondStr, Pixel(' ', color));
		}
	}
}
void NowPlaying::onTick(ControlSPtr sender, TickEventArgs& e) {

	timerTitle += e.timeSinceLastTick;
	timerArtist += e.timeSinceLastTick;
	timerAlbum += e.timeSinceLastTick;
	const int total = SCROLL_DELAY + SCROLL_SPEED * scrollLength;
	if (timerTitle >= total) {
		timerTitle %= total;
		timerArtist %= total;
		timerAlbum %= total;
	}
}

#pragma endregion
//=================================================================|
