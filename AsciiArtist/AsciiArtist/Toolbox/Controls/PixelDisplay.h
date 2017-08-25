/*=================================================================|
|* File:				PixelDisplay.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PIXEL_DISPLAY_H
#define PIXEL_DISPLAY_H

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

class PixelDisplay : public Control {
	
	//=========== MEMBERS ============
	#pragma region Members

	// Settings
	/* True if only one pixel is displayed. */
	bool singular;
	/* True if the display allows color swapping with the middle mouse button. */
	bool allowInput;
	/* True if the colors and characters are displayed before the final pixels. */
	bool leftToRight;
	/* True if the character should be shown. */
	bool showCharacter;

	/* The primary pixel. */
	Pixel primaryPixel;
	/* The secondary pixel. */
	Pixel secondaryPixel;

	// Events
	/* Called when the color has been changed with color swapping. */
	Event<PixelChangedEventArgs> _eventPixelChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the pixel display. */
	PixelDisplay();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the primary or secondary pixel. */
	Pixel getDrawingPixel(bool secondary = false) const;
	/* Sets the primary or secondary pixel. */
	void setDrawingPixel(Pixel pixel, bool secondary = false);
	/* Gets if only one pixel can be displayed. */
	bool isSingular() const;
	/* Sets if only one pixel can be displayed. */
	void setSingular(bool singular);
	/* Gets if the display allows color swapping with the middle mouse button. */
	bool getAllowInput() const;
	/* Sets if the display allows color swapping with the middle mouse button. */
	void setAllowInput(bool allowInput);
	/* Gets if the colors and characters are displayed before the final pixels. */
	bool isLeftToRight() const;
	/* Sets if the colors and characters are displayed before the final pixels. */
	void setLeftToRight(bool leftToRight);
	/* Gets if the character should be shown. */
	bool getShowCharacter() const;
	/* Sets if the character should be shown. */
	void setShowCharacter(bool showCharacter);

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the color has been changed with color swapping. */
	Event<PixelChangedEventArgs>& eventPixelChanged();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(PixelDisplay);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
