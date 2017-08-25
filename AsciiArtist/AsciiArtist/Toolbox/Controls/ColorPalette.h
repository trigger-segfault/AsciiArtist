/*=================================================================|
|* File:				ColorPalette.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef COLOR_PALETTE_H
#define COLOR_PALETTE_H

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

class ColorPalette : public Control {
	//========== CONSTANTS ===========
	#pragma region Constants

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	// Settings
	/* True if only one color can be selected. */
	bool singular;
	/* True if attributes are allowed to be selected. */
	bool allowAttributes;

	/* The image of the color palette. */
	AsciiImageSPtr image;

	/* The primary pixel. */
	Pixel primaryPixel;
	/* The secondary pixel. */
	Pixel secondaryPixel;

	/* The position in the color palette being hovered over. */
	Point2I hoverPoint;

	// Events
	/* Called when the color has been changed with the color palette. */
	Event<PixelChangedEventArgs> _eventPixelChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the color palette display. */
	ColorPalette();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the primary or secondary pixel. */
	Pixel getDrawingPixel(bool secondary = false) const;
	/* Sets the primary or secondary pixel. */
	void setDrawingPixel(Pixel pixel, bool secondary = false);
	/* Gets if only one color can be selected. */
	bool isSingular() const;
	/* Sets if only one color can be selected. */
	void setSingular(bool singular);
	/* Gets if attributes are allowed to be selected. */
	bool getAllowAttributes() const;
	/* Sets if attributes are allowed to be selected. */
	void setAllowAttributes(bool allowAttributes);

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the color has been changed with the color palette. */
	Event<PixelChangedEventArgs>& eventPixelChanged();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMouseMoved(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(ColorPalette);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
