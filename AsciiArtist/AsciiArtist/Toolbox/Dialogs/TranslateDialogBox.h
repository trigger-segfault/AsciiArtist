/*=================================================================|
|* File:				TranslateDialogBox.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TRANSLATE_DIALOG_BOX_H
#define TRANSLATE_DIALOG_BOX_H

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

class TranslateDialogBox : public DialogBox {
	//========== CONSTANTS ===========
	#pragma region Constants
public:

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The distance to return on success. */
	Point2I distanceResult;
	/* True if all frames should be translated. */
	bool translateAllResult;

	// Controls
	LabelSPtr labelDistance;
	LabelSPtr labelComma;
	LabelSPtr labelTranslateAll;
	IntTextBoxSPtr textBoxX;
	IntTextBoxSPtr textBoxY;
	YesNoToggleButtonSPtr toggleButtonAll;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	TranslateDialogBox(Point2I distanceResult, bool translateAllResult);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a resize dialog box with the specified settings. */
	static bool show(WindowSPtr owner, Point2I& distanceResult, bool& translateAllResult);

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

DECLARE_SMART_PTRS(TranslateDialogBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
