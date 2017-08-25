/*=================================================================|
|* File:				DialogBox.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Dialog.h>
#include <PowerConsole/Controls/Toolbox/Button.h>
#include <PowerConsole/Controls/Toolbox/Image.h>
#include <PowerConsole/Controls/Toolbox/Label.h>
#include <PowerConsole/Controls/Toolbox/Panel.h>

#undef MessageBox
#undef DialogBox

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The types of results that can return from a message box. */
enum class DialogBoxResults {
	/* Cancel was pressed. This is the default result. */
	Cancel,
	/* OK was pressed. */
	OK,
	/* No was pressed. */
	No,
	/* Yes was pressed. */
	Yes
};

/* Specifies the buttons that are displayed on a message box. */
enum class DialogBoxButtons {
	/* The message box displays an OK button. */
	OK,
	/* The message box displays OK and Cancel buttons. */
	OKCancel,
	/* The message box displays Yes and No buttons. */
	YesNo,
	/* The message box displays Yes, No, and Cancel buttons. */
	YesNoCancel
};

/* Specifies the icon that is displayed by a message box. */
enum class DialogBoxIcons {
	/* No icon is shown. */
	None,
	/* A blue icon with an 'i' in it is shown. */
	Information,
	/* A blue icon with a question mark in it is shown. */
	Question,
	/* A yellow icon with an exclamation mark in it is shown. */
	Warning,
	/* A red icon with an 'X' in it is shown. */
	Error
};

/* Shows a popup window with a message to the user. Gives options for different types of buttons and returns the pressed button as the result. */
class DialogBox : public Dialog {

	//========== CONSTANTS ===========
	#pragma region Constants
public:

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The title of the message box. */
	std::string title;
	/* The buttons to use. */
	DialogBoxButtons buttonSetup;
	/* The icon to show. */
	DialogBoxIcons iconType;

	// Controls
	LabelSPtr labelTitle;
	ButtonSPtr buttons[3];
	ImageSPtr imageIcon;

	// Result
	/* The result of the message box. */
	DialogBoxResults dialogBoxResult;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
protected:
	/* Constructs the message box. */
	DialogBox(const std::string& title, DialogBoxButtons buttons, DialogBoxIcons icon);
private:
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
public:
	/* Gets the button setup of the dialog box. */
	DialogBoxButtons getButtonSetup() const;
	/* Gets the icon type of the dialog box. */
	DialogBoxIcons getIconType() const;
	/* Returns true if the dialog box has an icon. */
	bool hasIcon() const;
	/* Gets the title of the dialog box. */
	std::string getTitle() const;
	/* Sets the title of the dialog box. */
	void setTitle(const std::string& title);
	/* Gets the dialog box result. */
	DialogBoxResults getDialogBoxResult() const;
	/* Sets the dialog box result. */
	void setDialogBoxResult(DialogBoxResults result);

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers
protected:
	/* Gets the number of buttons based on the button setup enum. */
	int numButtons() const;
	/* Gets the message box result from the button. */
	DialogBoxResults getDialogBoxResultFromButton(ControlSPtr button) const;
	/* Gets the minimum size of the dialog box. */
	PcGeo::Point2I getMinSize() const;
	/* Checks if an action should be performed on a dialog button press. */
	/* Return false if the dialog box should not close. */
	virtual bool buttonBehavior(DialogBoxResults buttonPressed);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onButtonClick(ControlSPtr sender, MouseEventArgs& e);
	void onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(DialogBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
