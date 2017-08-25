/*=================================================================|
|* File:				PaletteEditor.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PALETTE_EDITOR_H
#define PALETTE_EDITOR_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Controls/Toolbox/GroupBox.h>
#include <PowerConsole/Controls/Toolbox/NumericTextBox.h>
#include <PowerConsole/Helpers/WordWrapper.h>

using namespace PowerConsole;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiArtist::Toolbox::Dialogs {
//=================================================================|
// CLASSES														   |
//=================================================================/

class PaletteEditor : public DialogBox {

	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The palette to return on OK. */
	ConsolePalette paletteResult;
	/* The currently selected color being edited. */
	unsigned char selection;
	Point2I palettePostions[ConsolePalette::NUM_COLORS];

	// Controls
	GroupBoxSPtr groupBoxColor;
	GroupBoxSPtr groupBoxPalette;
	GroupBoxSPtr groupBoxInfo;

	IntTextBoxSPtr textBoxR;
	IntTextBoxSPtr textBoxG;
	IntTextBoxSPtr textBoxB;

	ButtonSPtr buttonImport;
	ButtonSPtr buttonExport;
	ButtonSPtr buttonDefaults;

	LabelSPtr labelRed;
	LabelSPtr labelGrn;
	LabelSPtr labelBlue;

	LabelSPtr labelColorName;
	LabelSPtr labelColorNameValue;
	LabelSPtr labelColorCode;
	LabelSPtr labelColorCodeValue;
	LabelSPtr labelInvertColor;
	LabelSPtr labelLightDarkColor;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	PaletteEditor(ConsolePalette paletteResult);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a dialog box with the specified settings. */
	static bool show(WindowSPtr owner, ConsolePalette& paletteResult);

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
	void onColorGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onPaletteGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onInfoGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onValueChanged(ControlSPtr sender, IntChangedEventArgs& e);
	void onImportClicked(ControlSPtr sender, MouseEventArgs& e);
	void onExportClicked(ControlSPtr sender, MouseEventArgs& e);
	void onDefaultsClicked(ControlSPtr sender, MouseEventArgs& e);
	void onPaletteClicked(ControlSPtr sender, MouseEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
