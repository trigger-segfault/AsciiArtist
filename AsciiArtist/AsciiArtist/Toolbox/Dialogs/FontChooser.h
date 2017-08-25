/*=================================================================|
|* File:				FontChooser.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FONT_CHOOSER_H
#define FONT_CHOOSER_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Controls/Toolbox/GroupBox.h>
#include <PowerConsole/Controls/Toolbox/ListBox.h>
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

class FontChooser : public DialogBox {

	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The resulting font. */
	//ConsoleFonts fontResult;
	/* The resulting font size. */
	//Point2I fontSizeResult;
	/* The resulting font weight. */
	//ConsoleFontWeights fontWeightResult;

	/* The resulting font. */
	ConsoleFont fontResult;

	// Controls
	GroupBoxSPtr groupBoxFont;
	GroupBoxSPtr groupBoxSize;
	GroupBoxSPtr groupBoxWeight;

	ListBoxSPtr listBoxFont;
	ListBoxSPtr listBoxSize;
	ListBoxSPtr listBoxWeight;

	IntTextBoxSPtr textBoxSize;

	ButtonSPtr buttonImport;
	ButtonSPtr buttonExport;
	ButtonSPtr buttonDefaults;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	FontChooser(ConsoleFont fontResult);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a dialog box with the specified settings. */
	static bool show(WindowSPtr owner, ConsoleFont& fontResult);

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers
protected:
	/* Checks if an action should be performed on a dialog button press. */
	/* Return false if the dialog box should not close. */
	bool buttonBehavior(DialogBoxResults buttonPressed) final;
	/* Gets the font size from the selection index. */
	Point2I getFontSizeFromIndex(int index) const;
	/* Gets the selection index from the font size. */
	int getIndexFromFontSize(Point2I fontSize) const;
	/* Updates changes to the font. */
	void updateFont();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onSizeGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onFontSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e);
	void onSizeSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e);
	void onWeightSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e);
	void onSizeValueChanged(ControlSPtr sender, IntChangedEventArgs& e);
	void onImportClicked(ControlSPtr sender, MouseEventArgs& e);
	void onExportClicked(ControlSPtr sender, MouseEventArgs& e);
	void onDefaultsClicked(ControlSPtr sender, MouseEventArgs& e);
	void onListBoxMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
