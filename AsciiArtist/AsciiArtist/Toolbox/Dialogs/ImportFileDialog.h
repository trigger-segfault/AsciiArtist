/*=================================================================|
|* File:				ImportFileDialog.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef IMPORT_FILE_DIALOG_H
#define IMPORT_FILE_DIALOG_H

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

struct ImportFileSettings {
	/* The size of the image to import. */
	Point2I size;
	/* The position in the file to start reading at. */
	unsigned int startPosition;
	/* The number of bytes to skip after reading a character. */
	unsigned int bytesAfterCharacter;
	/* The number of bytes to skip after reading a color. */
	unsigned int bytesAfterColor;
	/* True if characters are read. */
	bool readCharacters;
	/* True if colors are read. */
	bool readColors;

	inline ImportFileSettings()
		: size(64, 32), startPosition(0),
		bytesAfterCharacter(0), bytesAfterColor(0),
		readCharacters(true), readColors(true) {}
};

class ImportFileDialog : public DialogBox {
	//========== CONSTANTS ===========
	#pragma region Constants
public:

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The import settings to return on success. */
	ImportFileSettings importResult;

	// Controls
	LabelSPtr labelSize;
	LabelSPtr labelX;
	LabelSPtr labelStartPosition;
	LabelSPtr labelBytesAfterChar;
	LabelSPtr labelBytesAfterColor;
	LabelSPtr labelReadChar;
	LabelSPtr labelReadColor;
	IntTextBoxSPtr textBoxWidth;
	IntTextBoxSPtr textBoxHeight;
	IntTextBoxSPtr textBoxStartPosition;
	IntTextBoxSPtr textBoxBytesAfterChar;
	IntTextBoxSPtr textBoxBytesAfterColor;
	YesNoToggleButtonSPtr toggleButtonChar;
	YesNoToggleButtonSPtr toggleButtonColor;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	ImportFileDialog(ImportFileSettings importResult);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a resize dialog box with the specified settings. */
	static bool show(WindowSPtr owner, ImportFileSettings& importResult);

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

DECLARE_SMART_PTRS(ImportFileDialog);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
