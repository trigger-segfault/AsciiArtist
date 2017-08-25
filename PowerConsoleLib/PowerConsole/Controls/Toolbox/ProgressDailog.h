/*=================================================================|
|* File:				ProgressDialog.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Dialog.h>
#include <PowerConsole/Controls/Toolbox/Label.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

DECLARE_CLASS_AND_SMART_PTRS(ProgressDialog);

class ProgressDialog : public Dialog {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The default width of the progress dialog. */
	static const int DEFAULT_WIDTH = 48;
	typedef std::function<void(ProgressDialogSPtr sender)> CallFunction;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The current completion percentage. */
	int percentage;
	/* True if the action message is shown. */
	bool showActionMessage;
	/* True if the action can be canceled. */
	bool allowCancel;
	/* The function to call on load. */
	CallFunction callFunction;

	// Controls
	LabelSPtr labelTitle;
	LabelSPtr labelPercentage;
	LabelSPtr labelActionMessage;
	LabelSPtr labelPressEscape;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
private:
	/* Constructs the dialog. */
	ProgressDialog(bool showActionMessage, bool allowCancel, int width, CallFunction callFunction);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a dialog with the specified settings. */
	static void show(WindowSPtr owner, const std::string& title, bool showActionMessage, bool allowCancel, CallFunction callFunction);
	/* Shows a dialog with the specified settings. */
	static void show(WindowSPtr owner, const std::string& title, bool showActionMessage, bool allowCancel, int width, CallFunction callFunction);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
public:
	/* Gets the title of the dialog. */
	std::string getTitle() const;
	/* Sets the title of the dialog. */
	void setTitle(const std::string& title);
	/* Gets the current completion percentage. */
	int getPercentage() const;
	/* Sets the current completion percentage. */
	void setPercentage(int percentage);
	/* Gets the current action message. */
	std::string getActionMessage() const;
	/* Sets the current action message. */
	void setActionMessage(const std::string& message);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
public:
	/* Updates the progress dialog and checks for the escape key. Returns true if the operation was canceled. */
	bool update();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
