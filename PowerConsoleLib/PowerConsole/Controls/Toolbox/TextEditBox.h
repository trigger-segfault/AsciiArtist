/*=================================================================|
|* File:				TextEditBox.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TEXT_EDIT_BOX_H
#define TEXT_EDIT_BOX_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Dialog.h>
#include <PowerConsole/Controls/Toolbox/Button.h>
#include <PowerConsole/Controls/Toolbox/Label.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Controls/Toolbox/TextBox.h>
#include <PowerConsole/Helpers/WordWrapper.h>

#undef MessageBox

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Shows a popup window with a message to the user. Gives options for different types of buttons and returns the pressed button as the result. */
class TextEditBox : public DialogBox {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The default width for text edit box in the show command. */
	static const int DEFAULT_WIDTH = 40;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Controls
	TextBoxSPtr textBox;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the text edit box. */
	TextEditBox(const std::string& title, DialogBoxButtons buttons, int width);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a text edit box with the specified settings. */
	static DialogBoxResults show(WindowSPtr owner, std::string& textResult, int width = DEFAULT_WIDTH);
	/* Shows a text edit box with the specified settings. */
	static DialogBoxResults show(WindowSPtr owner, std::string& textResult, const std::string& title, int width = DEFAULT_WIDTH);
	/* Shows a text edit box with the specified settings. */
	static DialogBoxResults show(WindowSPtr owner, std::string& textResult, const std::string& title, DialogBoxButtons buttons, int width = DEFAULT_WIDTH);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(TextEditBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
