/*=================================================================|
|* File:				ScrollBars.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ScrollBars.h"
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ScrollBars::ScrollBars()
 :	// Dimensions
	scrollSize(Point2I::ZERO),
	thumbSize(Point2I::ZERO),
	viewportSize(Point2I::ZERO),
	objectSize(Point2I::ZERO),
	scrollRatio(Point2F::ZERO),

	// Scrolling
	gripPosition(0),
	dragging(ScrollBarDragging::None),
	hovering(ScrollBarHovering::None),

	// State
	horizontalActive(false),
	verticalActive(false),
	viewPosition(Point2I::ZERO),
	scrollPosition(Point2I::ZERO),
	arrowDelay(DEFAULT_ARROW_DELAY),
	arrowSpeed(DEFAULT_ARROW_SPEED),
	arrowHoldTime(0),
	heldArrow(ScrollBarHovering::None),

	// Settings
	horizontalScrollBarVisibility(ScrollBarVisibilities::Auto),
	verticalScrollBarVisibility(ScrollBarVisibilities::Auto),
	mouseWheelState(ScrollBarMouseWheelStates::Local),
	keepPositionOnResize(true),
	minThumbSize(Point2I::ONE),
	padding(Thickness::ZERO),
	increment(Point2I::ONE),

	// Events
	_eventScrolled(EventDirections::FirstInFirstCalled) {

	eventLayoutChanged().add(bindEvent(ScrollBars::onLayoutChanged));
	eventPaint().add(bindEvent(ScrollBars::onPaint));
	eventMouseButtonGlobal().add(bindEvent(ScrollBars::onMouseButtonGlobal));
	eventMouseMovedGlobal().add(bindEvent(ScrollBars::onMouseMovedGlobal));
	eventMouseWheelGlobal().add(bindEvent(ScrollBars::onMouseWheelGlobal));
	eventMouseWheel().add(bindEvent(ScrollBars::onMouseWheel));
	eventTick().add(bindEvent(ScrollBars::onTick));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Settings
ScrollBarVisibilities ScrollBars::getHorizontalScrollBarVisibility() const {
	return horizontalScrollBarVisibility;
}
void ScrollBars::setHorizontalScrollBarVisibility(ScrollBarVisibilities horizontalScrollBarVisibility) {
	this->horizontalScrollBarVisibility = horizontalScrollBarVisibility;
	updateScrollBars();
}
ScrollBarVisibilities ScrollBars::getVerticalScrollBarVisibility() const {
	return verticalScrollBarVisibility;
}
void ScrollBars::setVerticalScrollBarVisibility(ScrollBarVisibilities verticalScrollBarVisibility) {
	this->verticalScrollBarVisibility = verticalScrollBarVisibility;
	updateScrollBars();
}
ScrollBarMouseWheelStates ScrollBars::getMouseWheelState() const {
	return mouseWheelState;
}
void ScrollBars::setMouseWheelState(ScrollBarMouseWheelStates mouseWheelState) {
	this->mouseWheelState = mouseWheelState;
}
bool ScrollBars::isPositionKeptOnResize() const {
	return keepPositionOnResize;
}
void ScrollBars::setPositionKeptOnResize(bool keepPosition) {
	this->keepPositionOnResize = keepPosition;
}
Point2I ScrollBars::getMinThumbSize() const {
	return minThumbSize;
}
void ScrollBars::setMinThumbSize(Point2I minThumbSize) {
	this->minThumbSize = GMath::max(Point2I::ONE, minThumbSize);
	updateScrollBars();
}
Thickness ScrollBars::getPadding() const {
	return padding;
}
void ScrollBars::setPadding(Thickness padding) {
	this->padding = padding;
	updateScrollBars();
}
Point2I ScrollBars::getIncrement() const {
	return increment;
}
void ScrollBars::setIncrement(Point2I increment) {
	this->increment = increment;
}

// State
bool ScrollBars::isHorizontalActive() const {
	return horizontalActive;
}
bool ScrollBars::isVerticalActive() const {
	return verticalActive;
}
bool ScrollBars::canScrollLeft() const {
	return (viewPosition.x > 0);
}
bool ScrollBars::canScrollRight() const {
	return (viewPosition.x < objectSize.x - viewportSize.x);
}
bool ScrollBars::canScrollUp() const {
	return (viewPosition.y > 0);
}
bool ScrollBars::canScrollDown() const {
	return (viewPosition.y < objectSize.y - viewportSize.y);
}

#pragma endregion
//========== SCROLLING ===========
#pragma region Scrolling

Point2I ScrollBars::getViewportSize() const {
	return viewportSize;
}
Point2I ScrollBars::getObjectSize() const {
	return objectSize;
}
Point2I ScrollBars::getViewPosition() const {
	return viewPosition;
}
void ScrollBars::setViewPosition(Point2I position, bool relative) {
	Point2I oldViewPosition = viewPosition;
	Point2I oldScrollPosition = scrollPosition;

	if (horizontalActive) {
		viewPosition.x = GMath::clamp(position.x + (relative ? viewPosition.x : 0), 0, objectSize.x - viewportSize.x);
		if (viewPosition.x == objectSize.x - viewportSize.x)
			scrollPosition.x = scrollSize.x - thumbSize.x;
		else
			scrollPosition.x = (int)(viewPosition.x * scrollRatio.x);
	}
	if (verticalActive) {
		viewPosition.y = GMath::clamp(position.y + (relative ? viewPosition.y : 0), 0, objectSize.y - viewportSize.y);
		if (viewPosition.y == objectSize.y - viewportSize.y)
			scrollPosition.y = scrollSize.y - thumbSize.y;
		else
			scrollPosition.y = (int)(viewPosition.y * scrollRatio.y);
	}

	if (viewPosition != oldViewPosition || scrollPosition != oldScrollPosition) {
		_eventScrolled(shared_from_this(), ScrollEventArgs(viewPosition, oldViewPosition, scrollPosition, oldScrollPosition));
	}
}
Point2I ScrollBars::getScrollPosition() const {
	return scrollPosition;
}
void ScrollBars::setScrollPosition(Point2I position, bool relative) {
	Point2I oldViewPosition = viewPosition;
	Point2I oldScrollPosition = scrollPosition;

	if (horizontalActive) {
		scrollPosition.x = GMath::clamp(position.x + (relative ? scrollPosition.x : 0), 0, scrollSize.x - thumbSize.x);
		if (scrollPosition.x == 0)
			viewPosition.x = 0;
		else if (scrollPosition.y == scrollSize.x - thumbSize.x)
			viewPosition.x = objectSize.x - viewportSize.x;
		else
			viewPosition.x = (int)(scrollPosition.x / scrollRatio.x);
	}
	if (verticalActive) {
		scrollPosition.y = GMath::clamp(position.y + (relative ? scrollPosition.y : 0), 0, scrollSize.y - thumbSize.y);
		if (scrollPosition.y == 0)
			viewPosition.y = 0;
		else if (scrollPosition.y == scrollSize.y - thumbSize.y)
			viewPosition.y = objectSize.y - viewportSize.y;
		else
			viewPosition.y = (int)(scrollPosition.y / scrollRatio.y);
	}

	if (viewPosition != oldViewPosition || scrollPosition != oldScrollPosition) {
		_eventScrolled(shared_from_this(), ScrollEventArgs(viewPosition, oldViewPosition, scrollPosition, oldScrollPosition));
	}
}
void ScrollBars::resizeScrollBars(Point2I newObjectSize) {
	viewportSize = getActualSize() - padding.horizontalVertical();
	scrollSize = viewportSize - 4;
	objectSize = newObjectSize;
	//dragging = ScrollBarDragging::None;
	//hovering = ScrollBarHovering::None;
	verticalActive = false;
	horizontalActive = false;

	// Check if the scrollbars are required.
	if ((viewportSize.x < objectSize.x && horizontalScrollBarVisibility == ScrollBarVisibilities::Auto) ||
		horizontalScrollBarVisibility == ScrollBarVisibilities::Visible) {
		horizontalActive = true;
		viewportSize.y -= 2;

		// Check if the scrollbar makes the view size small enough to require the other scrollbar.
		if ((viewportSize.y < objectSize.y && verticalScrollBarVisibility == ScrollBarVisibilities::Auto) ||
			verticalScrollBarVisibility == ScrollBarVisibilities::Visible) {
			verticalActive = true;
			viewportSize.x -= 2;
			scrollSize -= 2;
		}
	}
	else if ((viewportSize.y < objectSize.y && verticalScrollBarVisibility == ScrollBarVisibilities::Auto) ||
		verticalScrollBarVisibility == ScrollBarVisibilities::Visible) {
		verticalActive = true;
		viewportSize.x -= 2;

		// Check if the scrollbar makes the view size small enough to require the other scrollbar.
		if ((viewportSize.x < objectSize.x && horizontalScrollBarVisibility == ScrollBarVisibilities::Auto) ||
			horizontalScrollBarVisibility == ScrollBarVisibilities::Visible) {
			horizontalActive = true;
			viewportSize.y -= 2;
			scrollSize -= 2;
		}
	}
	// Setup the horizontal scrollbar.
	if (horizontalActive) {
		thumbSize.x = (int)((float)scrollSize.x * (float)viewportSize.x / (float)objectSize.x);
		thumbSize.x = GMath::max(thumbSize.x, minThumbSize.x);

		// Set the scrollbar to child size ratio.
		scrollRatio.x = (float)(scrollSize.x - thumbSize.x) / (float)(objectSize.x - viewportSize.x);

		if (keepPositionOnResize) {
			viewPosition.x = GMath::min(viewPosition.x, objectSize.x - viewportSize.x);
			scrollPosition.x = (int)(viewPosition.x * scrollRatio.x);
		}
		else {
			viewPosition.x = 0;
			scrollPosition.x = 0;
		}
	}
	else {
		scrollSize.x = 0;
		thumbSize.x = 0;
		scrollRatio.x = 0.0;
		viewPosition.x = 0;
		scrollPosition.x = 0;
		if (dragging == ScrollBarDragging::Horizontal)
			dragging = ScrollBarDragging::None;
	}

	// Setup the vertical scrollbar.
	if (verticalActive) {
		thumbSize.y = (int)((float)scrollSize.y * (float)viewportSize.y / (float)objectSize.y);
		thumbSize.y = GMath::max(thumbSize.y, minThumbSize.y);

		// Set the scrollbar to child size ratio.
		scrollRatio.y = (float)(scrollSize.y - thumbSize.y) / (float)(objectSize.y - viewportSize.y);

		if (keepPositionOnResize) {
			viewPosition.y = GMath::min(viewPosition.y, objectSize.y - viewportSize.y);
			scrollPosition.y = (int)(viewPosition.y * scrollRatio.y);
		}
		else {
			viewPosition.y = 0;
			scrollPosition.y = 0;
		}
	}
	else {
		scrollSize.y = 0;
		thumbSize.y = 0;
		scrollRatio.y = 0.0;
		viewPosition.y = 0;
		scrollPosition.y = 0;
		if (dragging == ScrollBarDragging::Vertical)
			dragging = ScrollBarDragging::None;
	}
	if (dragging == ScrollBarDragging::Horizontal)
		gripPosition = GMath::min(gripPosition, thumbSize.x - 1);
	else if (dragging == ScrollBarDragging::Vertical)
		gripPosition = GMath::min(gripPosition, thumbSize.y - 1);
	onMouseMovedGlobal(shared_from_this(), MouseEventArgs(getMousePosition(), getMousePositionLast()));
}
void ScrollBars::updateScrollBars() {
	resizeScrollBars(objectSize);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

/* Called when view area has been scrolled. */
Event<ScrollEventArgs>& ScrollBars::eventScrolled() {
	return _eventScrolled;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ScrollBars::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		updateScrollBars();
	}
}
void ScrollBars::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	g->setExtraTranslation(padding.topLeft());
	g->setConnectGlobalBorders(true);

	// Color scheme
	unsigned char frameColor = toColor(BLACK, DARK_GRAY);
	unsigned char arrowColor = toColor(BLACK, GRAY);
	unsigned char arrowHoverColor = toColor(BLACK, WHITE);
	unsigned char arrowDisabledColor = frameColor;
	unsigned char thumbColor = frameColor;
	unsigned char thumbHoverColor = arrowColor;

	// Draw scrollbars
	if (horizontalActive) {
		// Draw the top horizontal line
		g->drawBorder(Rectangle2I(-1, viewportSize.y, getActualSize().x - padding.horizontal() + 2, 1), Pixel(' ', frameColor), false);

		// Draw the vertical lines separating the scrollbar from the arrow buttons
		g->drawBorder(Rectangle2I(1, viewportSize.y, 1, 3), Pixel(' ', frameColor), false);
		g->drawBorder(Rectangle2I(scrollSize.x + 2, viewportSize.y, 1, 3), Pixel(' ', frameColor), false);

		// Draw the top horizontal line
		/*g->setPixel(Point2I(0, viewportSize.y), Point2I(viewportSize.x, 1),
			Pixel(Borders::LINE_THIN_HORIZONTAL, frameColor));

		// Draw the vertical lines separating the scrollbar from the arrow buttons
		g->setPixel(Point2I(1, viewportSize.y + 1),
			Pixel(Borders::LINE_THIN_VERTICAL, frameColor));
		g->setPixel(Point2I(scrollSize.x + 2, viewportSize.y + 1),
			Pixel(Borders::LINE_THIN_VERTICAL, frameColor));

		// Connect the last vertical lines to the horizontal line
		g->setChar(Point2I(1, viewportSize.y),
			Borders::THREEWAY_THIN_BOTTOM_THIN_HORIZONTAL);
		g->setChar(Point2I(scrollSize.x + 2, viewportSize.y),
			Borders::THREEWAY_THIN_BOTTOM_THIN_HORIZONTAL);*/


		// Draw the arrows
		unsigned char finalArrowColor = arrowColor;
		if (!canScrollLeft())
			finalArrowColor = arrowDisabledColor;
		else if (hovering == ScrollBarHovering::LeftArrow || heldArrow == ScrollBarHovering::LeftArrow)
			finalArrowColor = arrowHoverColor;
		g->setPixel(Point2I(0, viewportSize.y + 1),
			Pixel(Characters::BIG_ARROW_LEFT, finalArrowColor));

		finalArrowColor = arrowColor;
		if (!canScrollRight())
			finalArrowColor = arrowDisabledColor;
		else if (hovering == ScrollBarHovering::RightArrow || heldArrow == ScrollBarHovering::RightArrow)
			finalArrowColor = arrowHoverColor;
		g->setPixel(Point2I(scrollSize.x + 3, viewportSize.y + 1),
			Pixel(Characters::BIG_ARROW_RIGHT, finalArrowColor));

		// Draw the bar
		unsigned char finalThumbColor = thumbColor;
		if (hovering == ScrollBarHovering::HorizontalThumb || dragging == ScrollBarDragging::Horizontal)
			finalThumbColor = thumbHoverColor;
		g->setPixel(Point2I(scrollPosition.x + 2, viewportSize.y + 1), Point2I(thumbSize.x, 1),
			Pixel(Characters::BLOCK_FULL, finalThumbColor));
	}
	if (verticalActive) {
		// Draw the left vertical line
		g->drawBorder(Rectangle2I(viewportSize.x, -1, 1, getActualSize().y - padding.vertical() + 2), Pixel(' ', frameColor), false);

		// Draw the horizontal lines separating the scrollbar from the arrow buttons
		g->drawBorder(Rectangle2I(viewportSize.x, 1, 3, 1), Pixel(' ', frameColor), false);
		g->drawBorder(Rectangle2I(viewportSize.x, scrollSize.y + 2, 3, 1), Pixel(' ', frameColor), false);

		// Draw the left vertical line
		/*g->setPixel(Point2I(viewportSize.x, 0), Point2I(1, viewportSize.y),
			Pixel(Borders::LINE_THIN_VERTICAL, frameColor));

		// Draw the horizontal lines separating the scrollbar from the arrow buttons
		g->setPixel(Point2I(viewportSize.x + 1, 1),
			Pixel(Borders::LINE_THIN_HORIZONTAL, frameColor));
		g->setPixel(Point2I(viewportSize.x + 1, scrollSize.y + 2),
			Pixel(Borders::LINE_THIN_HORIZONTAL, frameColor));

		// Connect the last horizontal lines to the vertical line
		g->setChar(Point2I(viewportSize.x, 1),
			Borders::THREEWAY_THIN_VERTICAL_THIN_RIGHT);
		g->setChar(Point2I(viewportSize.x, scrollSize.y + 2),
			Borders::THREEWAY_THIN_VERTICAL_THIN_RIGHT);*/

		// Draw the arrows
		unsigned char finalArrowColor = arrowColor;
		if (!canScrollUp())
			finalArrowColor = arrowDisabledColor;
		else if (hovering == ScrollBarHovering::UpArrow || heldArrow == ScrollBarHovering::UpArrow)
			finalArrowColor = arrowHoverColor;
		g->setPixel(Point2I(viewportSize.x + 1, 0),
			Pixel(Characters::BIG_ARROW_UP, finalArrowColor));

		finalArrowColor = arrowColor;
		if (!canScrollDown())
			finalArrowColor = arrowDisabledColor;
		else if (hovering == ScrollBarHovering::DownArrow || heldArrow == ScrollBarHovering::DownArrow)
			finalArrowColor = arrowHoverColor;
		g->setPixel(Point2I(viewportSize.x + 1, scrollSize.y + 3),
			Pixel(Characters::BIG_ARROW_DOWN, finalArrowColor));

		// Draw the thumb
		unsigned char finalThumbColor = thumbColor;
		if (hovering == ScrollBarHovering::VerticalThumb || dragging == ScrollBarDragging::Vertical)
			finalThumbColor = thumbHoverColor;
		g->setPixel(Point2I(viewportSize.x + 1, scrollPosition.y + 2), Point2I(1, thumbSize.y),
			Pixel(Characters::BLOCK_FULL, finalThumbColor));
	}

	// Draw the square connecting the two scrollbars
	/*if (verticalActive && horizontalActive) {
		g->setPixel(viewportSize,
			Pixel(Borders::FOURWAY_THIN_VERTICAL_THIN_HORIZONTAL, frameColor));
		g->setPixel(viewportSize + Point2I(1, 0),
			Pixel(Borders::LINE_THIN_HORIZONTAL, frameColor));
		g->setPixel(viewportSize + Point2I(0, 1),
			Pixel(Borders::LINE_THIN_VERTICAL, frameColor));
	}*/
	g->resetOptions();
}
void ScrollBars::onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e) {
	bool pressed = e.isButtonPressed(MouseButtons::Left);
	if (e.isButtonReleased(MouseButtons::Left)) {
		dragging = ScrollBarDragging::None;
		heldArrow = ScrollBarHovering::None;
		// Check for hovering again
		onMouseMovedGlobal(sender, MouseEventArgs(e.newPosition, e.previousPosition));
	}

	if (horizontalActive) {
		if (dragging == ScrollBarDragging::None) {
			if (hovering == ScrollBarHovering::LeftArrow) {
				if (pressed) {
					arrowHoldTime = clock();
					heldArrow = ScrollBarHovering::LeftArrow;
					if (canScrollLeft())
						setViewPosition(Point2I(-1, 0), true);
				}
			}
			else if (hovering == ScrollBarHovering::RightArrow) {
				if (pressed) {
					arrowHoldTime = clock();
					heldArrow = ScrollBarHovering::RightArrow;
					if (canScrollRight())
						setViewPosition(Point2I(1, 0), true);
				}
			}
			else if (hovering == ScrollBarHovering::HorizontalThumb) {
				if (pressed) {
					dragging = ScrollBarDragging::Horizontal;
					gripPosition = e.newPosition.x - scrollPosition.x - 2 - padding.left;
				}
			}
		}
	}
	if (verticalActive) {
		if (dragging == ScrollBarDragging::None) {
			if (hovering == ScrollBarHovering::UpArrow) {
				if (pressed) {
					arrowHoldTime = clock();
					heldArrow = ScrollBarHovering::UpArrow;
					if (canScrollUp())
						setViewPosition(Point2I(0, -1), true);
				}
			}
			else if (hovering == ScrollBarHovering::DownArrow) {
				if (pressed) {
					arrowHoldTime = clock();
					heldArrow = ScrollBarHovering::DownArrow;
					if (canScrollDown())
						setViewPosition(Point2I(0, 1), true);
				}
			}
			else if (hovering == ScrollBarHovering::VerticalThumb) {
				if (pressed) {
					dragging = ScrollBarDragging::Vertical;
					gripPosition = e.newPosition.y - scrollPosition.y - 2 - padding.top;
				}
			}
		}
	}
}
void ScrollBars::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	hovering = ScrollBarHovering::None;
	if (horizontalActive) {
		if (dragging == ScrollBarDragging::None) {
			if (e.newPosition == padding.topLeft() + Point2I(0, viewportSize.y + 1)) {
				hovering = ScrollBarHovering::LeftArrow;
			}
			else if (e.newPosition == padding.topLeft() + Point2I(scrollSize.x + 3, viewportSize.y + 1)) {
				hovering = ScrollBarHovering::RightArrow;
			}
			else if (Rectangle2I(padding.left + scrollPosition.x + 2, padding.top + viewportSize.y + 1, thumbSize.x, 1).containsPoint(e.newPosition)) {
				hovering = ScrollBarHovering::HorizontalThumb;
			}
		}
		else if (dragging == ScrollBarDragging::Horizontal) {
			setScrollPosition(Point2I(e.newPosition.x - gripPosition - 2 - padding.left, scrollPosition.y));
		}
	}
	if (verticalActive) {
		if (dragging == ScrollBarDragging::None) {
			if (e.newPosition == padding.topLeft() + Point2I(viewportSize.x + 1, 0)) {
				hovering = ScrollBarHovering::UpArrow;
			}
			else if (e.newPosition == padding.topLeft() + Point2I(viewportSize.x + 1, scrollSize.y + 3)) {
				hovering = ScrollBarHovering::DownArrow;
			}
			else if (Rectangle2I(padding.left + viewportSize.x + 1, padding.top + scrollPosition.y + 2, 1, thumbSize.y).containsPoint(e.newPosition)) {
				hovering = ScrollBarHovering::VerticalThumb;
			}
		}
		else if (dragging == ScrollBarDragging::Vertical) {
			setScrollPosition(Point2I(scrollPosition.x, e.newPosition.y - gripPosition - 2 - padding.top));
		}
	}
}
void ScrollBars::onMouseWheel(ControlSPtr sender, MouseWheelEventArgs& e) {
	Rectangle2I paddingBounds = getLocalVisibleArea().intersect(Rectangle2I(padding.topLeft(), getActualSize() - padding.horizontalVertical()));
	if (mouseWheelState != ScrollBarMouseWheelStates::None &&
		(mouseWheelState == ScrollBarMouseWheelStates::Global || paddingBounds.containsPoint(e.newPosition))) {
		if (horizontalActive) {
			if (e.isScrolledLeft() && canScrollLeft()) {
				setViewPosition(Point2I(-increment.x, 0), true);
			}
			else if (e.isScrolledRight() && canScrollRight()) {
				setViewPosition(Point2I(increment.x, 0), true);
			}
		}
		if (verticalActive) {
			if (e.isScrolledUp() && canScrollUp()) {
				setViewPosition(Point2I(0, -increment.y), true);
			}
			else if (e.isScrolledDown() && canScrollDown()) {
				setViewPosition(Point2I(0, increment.y), true);
			}
		}
	}
}
void ScrollBars::onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e) {
	if (mouseWheelState == ScrollBarMouseWheelStates::Global) {
		onMouseWheel(sender, e);
	}
}
void ScrollBars::onTick(ControlSPtr sender, TickEventArgs& e) {
	if (heldArrow != ScrollBarHovering::None && heldArrow != ScrollBarHovering::HorizontalThumb &&
		heldArrow != ScrollBarHovering::VerticalThumb) {
		int newTime = (int)clock() - arrowHoldTime;
		if (newTime >= arrowDelay + arrowSpeed) {
			if (heldArrow == ScrollBarHovering::LeftArrow && canScrollLeft()) {
				setViewPosition(Point2I(-1, 0), true);
			}
			else if (heldArrow == ScrollBarHovering::RightArrow && canScrollRight()) {
				setViewPosition(Point2I(1, 0), true);
			}
			else if (heldArrow == ScrollBarHovering::UpArrow && canScrollUp()) {
				setViewPosition(Point2I(0, -1), true);
			}
			else if (heldArrow == ScrollBarHovering::DownArrow && canScrollDown()) {
				setViewPosition(Point2I(0, 1), true);
			}
			arrowHoldTime = clock() - arrowDelay;
		}
	}
}

#pragma endregion
//=================================================================|
