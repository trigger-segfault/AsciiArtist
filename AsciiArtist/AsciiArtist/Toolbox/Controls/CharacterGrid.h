/*=================================================================|
|* File:				CharacterGrid.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CHARACTER_GRID_H
#define CHARACTER_GRID_H

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

class CharacterGrid : public Control {
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
	/* The primary pixel position in the character grid. */
	Point2I primaryCharPoint;
	/* The secondary pixel position in the character grid. */
	Point2I secondaryCharPoint;

	/* The position in the character grid being hovered over. */
	Point2I hoverPoint;
	/* The position in the character grid that's being pressed. */
	Point2I pressPoint;
	/* True if the pressed character is pressed with the right mouse button. */
	bool secondaryPress;

	// Events
	/* Called when the color has been changed with the character grid. */
	Event<PixelChangedEventArgs> _eventPixelChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the character grid. */
	CharacterGrid();
private:
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
public:
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
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const override;

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers
private:
	/* Finds the position of the character in the character grid. */
	Point2I findCharPoint(Pixel pixel) const;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMouseMoved(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(CharacterGrid);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
