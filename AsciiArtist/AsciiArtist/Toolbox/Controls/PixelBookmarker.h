/*=================================================================|
|* File:				PixelBookmarker.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PIXEL_BOOKMARKER_H
#define PIXEL_BOOKMARKER_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>
#include <AsciiArtist/Toolbox/AsciiArtistEventArgs.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;

namespace AsciiArtist::Toolbox::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

class PixelBookmarker : public Control {

	//=========== MEMBERS ============
	#pragma region Members

	/* The pixel being hovered over. */
	int hoverIndex;
	/* The current index being pressed. */
	int pressIndex;
	/* True if the secondary mouse is being pressed. */
	bool secondaryPress;

	// Pixels
	/* The primary pixel. */
	Pixel primaryPixel;
	/* The secondary pixel. */
	Pixel secondaryPixel;
	/* The 10 bookmarked pixels. */
	Pixel bookmarkedPixels[10];

	// Events
	/* Called when the color has been changed with color swapping. */
	Event<PixelChangedEventArgs> _eventPixelChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the pixel bookmarker. */
	PixelBookmarker();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the primary or secondary pixel. */
	Pixel getDrawingPixel(bool secondary = false) const;
	/* Sets the primary or secondary pixel. */
	void setDrawingPixel(Pixel pixel, bool secondary = false);
	/* Gets the bookmarked pixel at the specified index. */
	Pixel getBookmarkedPixel(int index) const;
	/* Sets the bookmarked pixel at the specified index. */
	void setBookmarkedPixel(int index, Pixel pixel);
	/* Selects the bookmarked pixel at the specified index. */
	void selectBookmarkedPixel(int index);
	/* Assigns the bookmarked pixel at the specified index. */
	void assignBookmarkedPixel(int index);

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the color has been changed with color swapping. */
	Event<PixelChangedEventArgs>& eventPixelChanged();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMouseMoved(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);

	#pragma endregion
	//======= COMMAND HANDLERS =======
	#pragma region Command Handlers

	void onAssignCommand(ControlSPtr sender, EventArgs& e, int index);
	void onSelectCommand(ControlSPtr sender, EventArgs& e, int index);

	#pragma endregion
};

DECLARE_SMART_PTRS(PixelBookmarker);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
