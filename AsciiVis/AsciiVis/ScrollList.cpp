/*=================================================================|
|* File:				MusicList.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ScrollList.h"
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

ScrollList::ScrollList() :
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
	dragging(false) {

	eventInitialize().add(bindEvent(ScrollList::onInitialize));
	eventTick().add(bindEvent(ScrollList::onTick));
	eventPaint().add(bindEvent(ScrollList::onPaint));
	eventMouseButton().add(bindEvent(ScrollList::onMouseButton));
	eventMouseWheelGlobal().add(bindEvent(ScrollList::onMouseWheelGlobal));
	eventLayoutChanged().add(bindEvent(ScrollList::onLayoutChanged));
	eventContentAdded().add(bindEvent(ScrollList::onContentAdded));
	eventContentRemoved().add(bindEvent(ScrollList::onContentRemoved));
}

void ScrollList::onInitialize(ControlSPtr sender, EventArgs& e) {

}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void ScrollList::setViewPosition(int position, bool relative) {
	viewPosition = GMath::clamp(position + (relative ? viewPosition : 0), 0, objectSize - viewportSize.y);
	if (viewPosition == objectSize - viewportSize.y)
		scrollPosition = (scrollSize - thumbSize) * 2;
	else
		scrollPosition = (int)(viewPosition * scrollRatio * 2.0f);
}
void ScrollList::setScrollPosition(int position, bool relative) {
	scrollPosition = GMath::clamp(position, 0, (scrollSize - thumbSize) * 2);
	if (scrollPosition == 0)
		viewPosition = 0;
	else if (scrollPosition / 2 == scrollSize - thumbSize)
		viewPosition = objectSize - viewportSize.y;
	else
		viewPosition = (int)(scrollPosition / scrollRatio / 2);
}
void ScrollList::resizeScrollBars(int newObjectSize) {
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
	if (hasContent())
		updateContentLayout();
}
void ScrollList::updateScrollBars() {
	resizeScrollBars(hasContent() ? GMath::max(0, getContent()->getActualSize().y) : 0);
}
bool ScrollList::canScrollUp() const {
	return (viewPosition > 0);
}
bool ScrollList::canScrollDown() const {
	return (viewPosition < objectSize - viewportSize.y);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void ScrollList::updateContentLayout() {
	if (!hasContent())
		throw std::exception("'ScrollList::updateContentLayout()' should not be called when no content is set");
	//getContent()->setContainerLayout(Rectangle2I(-viewPosition, getActualSize()), Rectangle2I(viewportSize));
	Rectangle2I bounds = Rectangle2I(Point2I(1, 1) - Point2I(0, viewPosition), getActualSize() - Point2I(4, 2));
	if (getContent()->getSize().x != SIZE_AUTO || getContent()->getHorizontalAlignment() != HorizontalAlignments::Stretch)
		bounds.size.x = getContent()->getActualSize().x;
	if (getContent()->getSize().y != SIZE_AUTO || getContent()->getVerticalAlignment() != VerticalAlignments::Stretch)
		bounds.size.y = getContent()->getActualSize().y;
	getContent()->setContainerLayout(
		bounds,
		(getLocalVisibleArea().intersect(Rectangle2I(Point2I(1, 1), viewportSize)))
	);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ScrollList::onTick(ControlSPtr sender, TickEventArgs& e) {
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
void ScrollList::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

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
void ScrollList::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (hoverThumb) {
			dragging = true;
			float clientY = getWindow()->mouse().getClientPosition().y / getWindow()->console().getFontSize().y - getGlobalPosition().y;
			int scrollY = (int)GMath::floor(clientY * 2 - 2);
			gripPosition = scrollY - scrollPosition;
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left) && dragging) {
		dragging = false;
	}
}
void ScrollList::onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e) {
	if (scrollbarActive) {
		if (e.isScrolledUp() && canScrollUp()) {
			setViewPosition(-SCROLL_INCREMENT, true);
		}
		else if (e.isScrolledDown() && canScrollDown()) {
			setViewPosition(SCROLL_INCREMENT, true);
		}
	}
}
void ScrollList::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		updateScrollBars();
	}
}
void ScrollList::onContentLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		updateScrollBars();
	}
}
void ScrollList::onContentAdded(ControlSPtr sender, ContentModifiedEventArgs& e) {
	e.control->eventLayoutChanged().add(bindEvent(ScrollList::onContentLayoutChanged));
	updateScrollBars();
}
void ScrollList::onContentRemoved(ControlSPtr sender, ContentModifiedEventArgs& e) {
	e.control->eventLayoutChanged().remove(unbindEvent(ScrollList::onChildLayoutChanged));
	updateScrollBars();
}

#pragma endregion
//=================================================================|
