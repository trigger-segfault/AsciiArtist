/*=================================================================|
|* File:				ScrollViewer.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SCROLL_VIEWER_H
#define SCROLL_VIEWER_H

#include <PowerConsole/Controls/Toolbox/ScrollBars.h>
#include <PowerConsole/Controls/ContentControl.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/


class ScrollViewer : public ContentControl {

	//========= ENUMERATIONS =========
	#pragma region Enumerations

	enum class ScrollBarDragging {
		None,
		Horizontal,
		Vertical
	};
	enum class ScrollBarHovering {
		None,
		LeftArrow,
		HorizontalThumb,
		RightArrow,
		UpArrow,
		VerticalThumb,
		DownArrow
	};

	#pragma endregion
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The default time to wait before arrow presses can be repeated. */
	static const int DEFAULT_ARROW_DELAY = 500;
	/* The default speed to repeat arrow presses at. */
	static const int DEFAULT_ARROW_SPEED = 40;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	// Dimensions
	/* The size of the scroll area. */
	PcGeo::Point2I scrollSize;
	/* The size of the scroll thumb. */
	PcGeo::Point2I thumbSize;
	/* The size of the viewport. */
	PcGeo::Point2I viewportSize;
	/* The size of the viewed object. */
	PcGeo::Point2I objectSize;
	/* The offset for the object size. */
	PcGeo::Point2I objectOffset;
	/* The ratio between the scroll size and view size. */
	PcGeo::Point2F scrollRatio;

	// Scrolling
	/* The grip position used when dragging. */
	int gripPosition;
	/* True if the scrollbar is being dragged. */
	ScrollBarDragging dragging;
	/* The hovering state of the scrollbar. */
	ScrollBarHovering hovering;
	/* The time to wait before arrow presses can be repeated. */
	int arrowDelay;
	/* The speed to repeat arrow presses at. */
	int arrowSpeed;
	/* The time an arrow button has been held for. */
	int arrowHoldTime;
	/* The held down arrow. */
	ScrollBarHovering heldArrow;

	// State
	/* True if the horizontal scrollbar is active. */
	bool horizontalActive;
	/* True if the vertical scrollbar is active. */
	bool verticalActive;
	/* The offset of the viewed object. */
	PcGeo::Point2I viewPosition;
	/* The offset of the scrollbar. */
	PcGeo::Point2I scrollPosition;

	// Settings
	/* The visibility setting of the horizontal scrollbar. */
	ScrollBarVisibilities horizontalScrollBarVisibility;
	/* The visibility setting of the vertical scrollbar. */
	ScrollBarVisibilities verticalScrollBarVisibility;
	/* The state of the mouse wheel for the scrollbar. */
	ScrollBarMouseWheelStates mouseWheelState;
	/* True if the scroll bar keeps its position after being resized. */
	bool keepPositionOnResize;
	/* The minimum size of the scroll thumb. */
	PcGeo::Point2I minThumbSize;
	/* The padding of the scrollbars. */
	PcGeo::Thickness padding;
	/* The increment to scroll by with the mouse wheel. */
	PcGeo::Point2I increment;

	// Events
	/* Called when view area has been scrolled. */
	Event<ScrollEventArgs> _eventScrolled;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	ScrollViewer();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Settings
	/* Gets the visibility setting of the horizontal scrollbar. */
	ScrollBarVisibilities getHorizontalScrollBarVisibility() const;
	/* Sets the visibility setting of the horizontal scrollbar. */
	void setHorizontalScrollBarVisibility(ScrollBarVisibilities horizontalScrollBarVisibility);
	/* Gets the visibility setting of the vertical scrollbar. */
	ScrollBarVisibilities getVerticalScrollBarVisibility() const;
	/* Sets the visibility setting of the vertical scrollbar. */
	void setVerticalScrollBarVisibility(ScrollBarVisibilities verticalScrollBarVisibility);
	/* Gets the state of the mouse wheel for the scrollbar. */
	ScrollBarMouseWheelStates getMouseWheelState() const;
	/* Sets the state of the mouse wheel for the scrollbar. */
	void setMouseWheelState(ScrollBarMouseWheelStates mouseWheelState);
	/* Gets if the scroll bar keeps its position after being resized. */
	bool isPositionKeptOnResize() const;
	/* Gets if the scroll bar keeps its position after being resized. */
	void setPositionKeptOnResize(bool keepPosition);
	/* Get the minimum size of the scroll thumb. */
	PcGeo::Point2I getMinThumbSize() const;
	/* Set the minimum size of the scroll thumb. */
	void setMinThumbSize(PcGeo::Point2I minThumbSize);
	/* Gets the padding of the scrollbars. */
	PcGeo::Thickness getPadding() const;
	/* Sets the padding of the scrollbars. */
	void setPadding(PcGeo::Thickness padding);
	/* Gets the increment to scroll by with the mouse wheel. */
	PcGeo::Point2I getIncrement() const;
	/* Sets the increment to scroll by with the mouse wheel. */
	void setIncrement(PcGeo::Point2I increment);
	/* Gets the offset for the object size. */
	PcGeo::Point2I getObjectOffset() const;
	/* Sets the offset for the object size. */
	void setObjectOffset(PcGeo::Point2I objectOffset);

	// State
	/* Gets if if the horizontal scrollbar is active. */
	bool isHorizontalActive() const;
	/* Gets if if the vertical scrollbar is active. */
	bool isVerticalActive() const;
	/* Returns true if the horizontal scrollbar can be scrolled left. */
	bool canScrollLeft() const;
	/* Returns true if the horizontal scrollbar can be scrolled right. */
	bool canScrollRight() const;
	/* Returns true if the vertical scrollbar can be scrolled up. */
	bool canScrollUp() const;
	/* Returns true if the vertical scrollbar can be scrolled down. */
	bool canScrollDown() const;

	#pragma endregion
	//========== SCROLLING ===========
	#pragma region Scrolling

	/* Gets the final viewport size of the scroll area. */
	PcGeo::Point2I getViewportSize() const;
	/* Gets the object size of the scroll area. */
	PcGeo::Point2I getObjectSize() const;
	/* Gets the offset of the viewed object. */
	PcGeo::Point2I getViewPosition() const;
	/* Sets the offset of the viewed object. */
	void setViewPosition(PcGeo::Point2I viewPosition, bool relative = false);
	/* Gets the position of the scrollbars. */
	PcGeo::Point2I getScrollPosition() const;
	/* Sets the position of the scrollbars. */
	void setScrollPosition(PcGeo::Point2I scrollPosition, bool relative = false);
	/* Sets the dimensions of the scrollbars. */
	void resizeScrollBars(PcGeo::Point2I objectSize);
private:
	/* Updates the dimensions of the scrollbar after a property has been changed. */
	void updateScrollBars();

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating
public:
	/* Sets the layout of the specified child. */
	void updateContentLayout() final;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when view area has been scrolled. */
	Event<ScrollEventArgs>& eventScrolled();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onContentAdded(ControlSPtr sender, ContentModifiedEventArgs& e);
	void onContentRemoved(ControlSPtr sender, ContentModifiedEventArgs& e);
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onContentLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e);
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);
	void onMouseWheel(ControlSPtr sender, MouseWheelEventArgs& e);
	void onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e);
	void onTick(ControlSPtr sender, TickEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(ScrollViewer);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
