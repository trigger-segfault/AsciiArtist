/*=================================================================|
|* File:				PixelPickerDialog.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PIXEL_PICKER_DIALOG_H
#define PIXEL_PICKER_DIALOG_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Controls/Toolbox/GroupBox.h>
#include <AsciiArtist/Toolbox/Controls/CharacterGrid.h>
#include <AsciiArtist/Toolbox/Controls/ColorPalette.h>
#include <AsciiArtist/Toolbox/Controls/PixelDisplay.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiArtist::Toolbox::Controls;

namespace AsciiArtist::Toolbox::Dialogs {
//=================================================================|
// CLASSES														   |
//=================================================================/

class PixelPickerDialog : public DialogBox {
	//========== CONSTANTS ===========
	#pragma region Constants
public:

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The pixel to return on success. */
	Pixel pixelResult;
	/* True if attributes are allowed. */
	bool allowAttributes;

	// Controls
	GroupBoxSPtr groupBoxCharacter;
	GroupBoxSPtr groupBoxColor;
	GroupBoxSPtr groupBoxDisplay;

	CharacterGridSPtr characterGrid;
	ColorPaletteSPtr colorPalette;
	PixelDisplaySPtr pixelDisplay;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	PixelPickerDialog(Pixel pixelResult, bool allowAttributes);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a dialog box with the specified settings. */
	static bool show(WindowSPtr owner, Pixel& pixelResult, bool allowAttributes);

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers
protected:
	/* Checks if an action should be performed on a dialog button press. */
	/* Return false if the dialog box should not close. */
	bool buttonBehavior(DialogBoxResults buttonPressed) final;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onDisplayGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(PixelPickerDialog);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
