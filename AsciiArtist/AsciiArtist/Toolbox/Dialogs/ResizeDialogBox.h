/*=================================================================|
|* File:				ResizeDialogBox.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef RESIZE_DIALOG_BOX_H
#define RESIZE_DIALOG_BOX_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Controls/Toolbox/NumericTextBox.h>
#include <PowerConsole/Controls/Toolbox/YesNoToggleButton.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiArtist::Toolbox::Dialogs {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ResizeDialogBox : public DialogBox {
	//========== CONSTANTS ===========
	#pragma region Constants
public:

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The size to return on success. */
	Point2I sizeResult;
	/* True if all frames should be resized. */
	bool resizeAllResult;
	/* True if a single frame can be resized. */
	bool canResizeSingle;

	// Controls
	LabelSPtr labelSize;
	LabelSPtr labelX;
	LabelSPtr labelResizeAll;
	LabelSPtr labelCurrentSize;
	IntTextBoxSPtr textBoxWidth;
	IntTextBoxSPtr textBoxHeight;
	YesNoToggleButtonSPtr toggleButtonAll;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	ResizeDialogBox(Point2I sizeResult, bool resizeAllResult, bool canResizeSingle);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a resize dialog box with the specified settings. */
	static bool show(WindowSPtr owner, Point2I& sizeResult, bool& resizeAllResult, bool canResizeSingle);

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
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(ResizeDialogBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
