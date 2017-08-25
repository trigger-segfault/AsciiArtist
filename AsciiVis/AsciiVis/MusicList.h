/*=================================================================|
|* File:				HighscoreMenu.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef MUSIC_LIST_H
#define MUSIC_LIST_H

#include <PowerConsole/Controls/Control.h>
#include <AsciiVis/MusicLibrary.h>
#include <AsciiVis/Colors.h>
#include <memory>
#include <vector>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Geometry;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The base manager for the game. */
class MusicList : public Control {

	//========== CONSTANTS ===========
	#pragma region Constants

	static const int SCROLL_INCREMENT = 4;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The selection of the menu. */
	int selection;
	/* The current item being hovered over. */
	int hover;

	// Scrollbars
	/* The offset of the viewed object. */
	int viewPosition;
	/* The offset of the scrollbar. */
	int scrollPosition;
	/* The size of the scroll area. */
	int scrollSize;
	/* The size of the scroll thumb. */
	int thumbSize;
	/* The size of the viewport. */
	Point2I viewportSize;
	Point2I padding;
	/* The size of the viewed object. */
	int objectSize;
	/* The ratio between the scroll size and view size. */
	float scrollRatio;
	/* True if the scrollbar is active. */
	bool scrollbarActive;

	// Dragging
	bool hoverThumb;
	int gripPosition;
	bool dragging;

	Event<EventArgs> _eventMusicChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Contructs the tetris manager. */
	MusicList();
private:
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
public:
	void updatePlayIndex();

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers
private:
	/* Sets the offset of the viewed object. */
	void setViewPosition(int viewPosition, bool relative = false);
	/* Sets the position of the scrollbars. */
	void setScrollPosition(int scrollPosition, bool relative = false);
	/* Sets the dimensions of the scrollbars. */
	void resizeScrollBars(int objectSize);
	void updateScrollBars();
	/* Scrolls the selection into view. */
	void scrollToSelection();
	/* Returns true if the vertical scrollbar can be scrolled up. */
	bool canScrollUp() const;
	/* Returns true if the vertical scrollbar can be scrolled down. */
	bool canScrollDown() const;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
public:
	Event<EventArgs>& eventMusicChanged();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMouseMoved(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);
	void onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e);
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(MusicList);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
