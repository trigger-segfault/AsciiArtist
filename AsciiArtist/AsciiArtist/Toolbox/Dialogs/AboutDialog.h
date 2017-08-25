/*=================================================================|
|* File:				AboutDialog.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Controls/Toolbox/GroupBox.h>
#include <PowerConsole/Controls/Toolbox/Label.h>
#include <PowerConsole/Controls/Toolbox/Image.h>
#include <PowerConsole/Helpers/WordWrapper.h>

using namespace PowerConsole::Helpers;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiArtist::Toolbox::Dialogs {
//=================================================================|
// CLASSES														   |
//=================================================================/

class AboutDialog : public DialogBox {
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The description to show in the window. */
	static const std::string ABOUT_DESCRIPTION;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	// Drawing
	/* The data for how to draw the description with wrapping. */
	WordWrapResult wordWrap;

	// Controls
	GroupBoxSPtr groupBoxLogo;
	GroupBoxSPtr groupBoxDetails;
	GroupBoxSPtr groupBoxDescription;

	LabelSPtr labelVersion;
	LabelSPtr labelBuild;
	LabelSPtr labelAuthor;
	ImageSPtr imageLogo;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	AboutDialog();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a dialog box with the specified settings. */
	static void show(WindowSPtr owner);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onDescriptionPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
