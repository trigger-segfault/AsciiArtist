/*=================================================================|
|* File:				MusicList.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "MusicList.h"
#include <PowerConsole/PowerConsoleSettings.h>
#include <PowerConsole/Controls/Window.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

MusicList::MusicList() :
	selection(MusicLibrary::getPlayIndex()),
	hover(-1),

	// Scrolling
	scrollSize(0),
	thumbSize(0),
	viewportSize(Point2I::ZERO),
	padding(Point2I(1, 1)),
	objectSize(0),
	scrollRatio(0.0f),
	viewPosition(0),
	scrollPosition(0),
	scrollbarActive(false),
	
	hoverThumb(false),
	gripPosition(0),
	dragging(false),
	
	_eventMusicChanged(EventDirections::FirstInFirstCalled) {

	eventInitialize().add(bindEvent(MusicList::onInitialize));
	eventTick().add(bindEvent(MusicList::onTick));
	eventPaint().add(bindEvent(MusicList::onPaint));
	eventMouseMoved().add(bindEvent(MusicList::onMouseMoved));
	eventMouseButton().add(bindEvent(MusicList::onMouseButton));
	eventMouseWheelGlobal().add(bindEvent(MusicList::onMouseWheelGlobal));
	eventLayoutChanged().add(bindEvent(MusicList::onLayoutChanged));
}

void MusicList::onInitialize(ControlSPtr sender, EventArgs& e) {

	updateScrollBars();
	scrollToSelection();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void MusicList::updatePlayIndex() {
	if (MusicLibrary::getCurrentPlaylist() == MusicLibrary::getPlayingPlaylist() && MusicLibrary::getPlayIndex() >= 0) {
		selection = MusicLibrary::getPlayIndex() + MusicLibrary::getFolderCount() + (MusicLibrary::hasParent() ? 1 : 0);
		//scrollToSelection();
	}
	else {
		selection = -1;
	}
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void MusicList::setViewPosition(int position, bool relative) {
	viewPosition = GMath::clamp(position + (relative ? viewPosition : 0), 0, objectSize - viewportSize.y);
	if (viewPosition == objectSize - viewportSize.y)
		scrollPosition = (scrollSize - thumbSize) * 2;
	else
		scrollPosition = (int)(viewPosition * scrollRatio * 2.0f);
}
void MusicList::setScrollPosition(int position, bool relative) {
	scrollPosition = GMath::clamp(position, 0, (scrollSize - thumbSize) * 2);
	if (scrollPosition == 0)
		viewPosition = 0;
	else if (scrollPosition / 2 == scrollSize - thumbSize)
		viewPosition = objectSize - viewportSize.y;
	else
		viewPosition = (int)(scrollPosition / scrollRatio / 2);
}
void MusicList::resizeScrollBars(int newObjectSize) {
	viewportSize = Point2I(getActualSize().x - 4, getActualSize().y - 2);
	scrollSize = viewportSize.y;
	objectSize = newObjectSize;

	scrollbarActive = (viewportSize < objectSize);

	if (scrollbarActive) {
		thumbSize = (int)((float)scrollSize * (float)viewportSize.y / (float)objectSize);
		thumbSize = GMath::max(thumbSize, 1);

		// Set the scrollbar to child size ratio.
		scrollRatio = (float)(scrollSize - thumbSize) / (float)(objectSize - viewportSize.y);

		viewPosition = GMath::min(viewPosition, objectSize - viewportSize.y);
		scrollPosition = (int)(viewPosition * scrollRatio);
	}
	else {
		thumbSize = scrollSize;
	}
}
void MusicList::updateScrollBars() {
	resizeScrollBars(MusicLibrary::getTotalCount() + 2);
	//scrollToSelection();
}
void MusicList::scrollToSelection() {
	int y = selection - viewPosition;
	int offset = 1;
	if (y - offset < 0) {
		setViewPosition(GMath::min(y - offset, 0), true);
		onMouseMoved(shared_from_this(), MouseEventArgs(getMousePosition(), getMousePositionLast()));
	}
	else if (y + offset + 1 >= viewportSize.y) {
		setViewPosition(y + offset - viewportSize.y + 1, true);
		onMouseMoved(shared_from_this(), MouseEventArgs(getMousePosition(), getMousePositionLast()));
	}
}
bool MusicList::canScrollUp() const {
	return (viewPosition > 0);
}
bool MusicList::canScrollDown() const {
	return (viewPosition < objectSize - viewportSize.y);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<EventArgs>& MusicList::eventMusicChanged() {
	return _eventMusicChanged;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void MusicList::onTick(ControlSPtr sender, TickEventArgs& e) {
	hoverThumb = false;
	Point2F client = getWindow()->mouse().getClientPosition() / getWindow()->console().getFontSize() - getGlobalPosition();
	Point2I scrollPos = (Point2I)GMath::floor(Point2F(client.x, client.y * 2 - 2));
	if (scrollPos.y >= scrollPosition && scrollPos.y < scrollPosition + thumbSize * 2 && scrollPos.x == getActualSize().x - 2) {
		hoverThumb = true;
	}
	if (dragging) {
		setScrollPosition(scrollPos.y - gripPosition);
	}
}
void MusicList::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char textColor = toColor(DARK_NAVY, LIGHT_LIGHT_NAVY);
	const unsigned char hoverColor = toColor(DARK_NAVY, WHITE);
	const unsigned char selectColor = toColor(WHITE, LIGHT_NAVY);

	g->startRegion(padding, Rectangle2I(viewportSize));

	// Draw highscores
	for (int i = viewPosition; i < viewPosition + viewportSize.y && i < objectSize - 2; i++) {
		unsigned char color = (i == hover ? hoverColor : textColor);
		if (i == selection)
			color = selectColor;

		int y = i - viewPosition;

		std::string title = ""; //MusicLibrary::getSongs()[i]->title;

		if (i == 0 && MusicLibrary::hasParent()) {
			// Parent
			title = std::string(1, Characters::BIG_ARROW_LEFT) + "..";
		}
		else if (i < MusicLibrary::getFolderCount() + (MusicLibrary::hasParent() ? 1 : 0)) {
			// Folder
			int index = i - (MusicLibrary::hasParent() ? 1 : 0);
			title = std::string(1, Characters::BIG_ARROW_RIGHT) + MusicLibrary::getCurrentPlaylist()->folders[index]->name;
		}
		else {
			// Song
			int index = i - (MusicLibrary::getFolderCount() + (MusicLibrary::hasParent() ? 1 : 0));
			title = " " + MusicLibrary::getCurrentPlaylist()->songs[index]->title;
		}
		title = title.substr(0, std::min((int)title.size(), viewportSize.x - 0));

		g->drawString(Point2I(0, y), title, Pixel(' ', textColor));

		g->setColor(Rectangle2I(0, y, viewportSize.x, 1), color);
	}

	g->endRegion();

	// Draw outlines
	g->drawBorder(Rectangle2I(1, getActualSize().y + 1), Pixel(' ', toColor(DARK_NAVY, LIGHT_NAVY)), false);
	g->drawBorder(Rectangle2I(getActualSize().x, 1), Pixel(' ', toColor(DARK_NAVY, LIGHT_NAVY)), false);
	g->drawBorder(Rectangle2I(getActualSize().x - 3, 0, 3, getActualSize().y), Pixel(' ', toColor(DARK_NAVY, LIGHT_NAVY)), false);

	unsigned char barColor = toColor(DARK_NAVY, LIGHT_NAVY);
	if (hoverThumb || dragging)
		barColor = toColor(DARK_NAVY, LIGHT_LIGHT_NAVY);

	// Draw the scroll bar thumb
	Point2I scrollPt = Point2I(getActualSize().x - 2, 1);
	g->setPixel(scrollPt + Point2I(0, scrollPosition / 2),
		Pixel(scrollPosition % 2 == 0 ? Characters::BLOCK_FULL : Characters::BLOCK_BOTTOM_HALF, barColor)
	);
	if (thumbSize > 2) {
		g->setPixel(Rectangle2I(scrollPt + Point2I(0, scrollPosition / 2 + 1), 1, thumbSize - 2 + (scrollPosition % 2)), Pixel(Characters::BLOCK_FULL, barColor));
	}
	if (thumbSize > 1) {
		g->setPixel(scrollPt + Point2I(0, scrollPosition / 2 + thumbSize - 1 + (scrollPosition % 2)),
			Pixel(scrollPosition % 2 == 0 ? Characters::BLOCK_FULL : Characters::BLOCK_TOP_HALF, barColor)
		);
	}
}
void MusicList::onMouseMoved(ControlSPtr sender, MouseEventArgs& e) {
	hover = -1;
	Point2I mouse = e.newPosition - padding;
	if (mouse >= Point2I::ZERO && mouse < viewportSize) {
		hover = mouse.y + viewPosition;
	}
}
void MusicList::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (hoverThumb) {
			dragging = true;
			float clientY = getWindow()->mouse().getClientPosition().y / getWindow()->console().getFontSize().y - getGlobalPosition().y;
			int scrollY = (int)GMath::floor(clientY * 2 - 2);
			gripPosition = scrollY - scrollPosition;
		}
		else if (hover != -1 && hover < MusicLibrary::getTotalCount()) {
			bool evnt = (hover != selection);
			if (hover == 0 && MusicLibrary::hasParent()) {
				// Parent
				MusicLibrary::gotoParentPlaylist();
				updateScrollBars();
				setScrollPosition(0);
				updatePlayIndex();
			}
			else if (hover < MusicLibrary::getFolderCount() + (MusicLibrary::hasParent() ? 1 : 0)) {
				// Folder
				int index = hover - (MusicLibrary::hasParent() ? 1 : 0);
				MusicLibrary::gotoSubPlaylist(index);
				updateScrollBars();
				setScrollPosition(0);
				updatePlayIndex();
			}
			else {
				// Song
				int index = hover - (MusicLibrary::getFolderCount() + (MusicLibrary::hasParent() ? 1 : 0));
				MusicLibrary::setForceStopped(false);
				MusicLibrary::setPlayIndex(index);
				updatePlayIndex();
				if (evnt) {
					_eventMusicChanged(shared_from_this());
				}
			}
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left) && dragging) {
		dragging = false;
	}
}
void MusicList::onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e) {
	if (scrollbarActive) {
		if (e.isScrolledUp() && canScrollUp()) {
			setViewPosition(-SCROLL_INCREMENT, true);
			onMouseMoved(shared_from_this(), e);
		}
		else if (e.isScrolledDown() && canScrollDown()) {
			setViewPosition(SCROLL_INCREMENT, true);
			onMouseMoved(shared_from_this(), e);
		}
	}
}
void MusicList::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		updateScrollBars();
		onMouseMoved(shared_from_this(), MouseEventArgs(getMousePosition(), getMousePositionLast()));
	}
}

#pragma endregion
//=================================================================|
