/*=================================================================|
|* File:				ImageSettingsDialog.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef IMAGE_SETTINGS_DIALOG_H
#define IMAGE_SETTINGS_DIALOG_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Controls/Toolbox/NumericTextBox.h>
#include <PowerConsole/Controls/Toolbox/ToggleButton.h>
#include <PowerConsole/Controls/Toolbox/ListBox.h>
#include <PowerConsole/Controls/Toolbox/GroupBox.h>
#include <AsciiArtist/Editing/ImageSettings.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiArtist::Editing;

namespace AsciiArtist::Toolbox::Dialogs {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ImageSettingsDialog : public DialogBox {
	//========== CONSTANTS ===========
	#pragma region Constants
public:

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The image settings to return on success. */
	ImageSettings settingsResult;

	// Controls
	LabelSPtr labelDistance;
	LabelSPtr labelComma;
	LabelSPtr labelTranslateAll;
	IntTextBoxSPtr textBoxX;
	IntTextBoxSPtr textBoxY;
	ToggleButtonSPtr toggleButtonYes;
	ToggleButtonSPtr toggleButtonNo;

	GroupBoxSPtr groupBoxFormat;
	GroupBoxSPtr groupBoxVersion;
	GroupBoxSPtr groupBoxBackground;

	ToggleButtonSPtr toggleButtonAttributes;
	ToggleButtonSPtr toggleButtonAnimation;
	ToggleButtonSPtr toggleButtonMultisize;

	ListBoxSPtr listBoxVersion;

	ToggleButtonSPtr toggleButtonVersion2;
	ToggleButtonSPtr toggleButtonVersion3;
	ToggleButtonSPtr toggleButtonSmallVersion1;

	ButtonSPtr buttonBackground;
	LabelSPtr labelCurrent;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	ImageSettingsDialog(ImageSettings settingsResult);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a resize dialog box with the specified settings. */
	static bool show(WindowSPtr owner, ImageSettings& settingsResult);

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
	void onBackgroundGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onToggleButtonClick(ControlSPtr sender, MouseEventArgs& e);
	void onVersionChanged(ControlSPtr sender, SelectionChangedEventArgs& e);
	void onChangeBackgroundClick(ControlSPtr sender, MouseEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(ImageSettingsDialog);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
