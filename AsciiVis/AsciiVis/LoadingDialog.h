/*=================================================================|
|* File:				LoadingDialog.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef LOADING_DIALOG_H
#define LOADING_DIALOG_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Dialog.h>
#include <PowerConsole/Controls/Toolbox/Label.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

DECLARE_CLASS_AND_SMART_PTRS(LoadingDialog);

class LoadingDialog : public Dialog {
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	typedef std::function<void(LoadingDialogSPtr sender)> CallFunction;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The message to show. */
	std::string message;
	/* The function to call on load. */
	CallFunction callFunction;

	// Controls
	//LabelSPtr labelMessage;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	LoadingDialog(const std::string& message, CallFunction callFunction);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a resize dialog box with the specified settings. */
	static void show(WindowSPtr owner, const std::string& message, CallFunction callFunction);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the current message. */
	std::string getMessage() const;
	/* Sets the current message. */
	void setMessage(const std::string& message);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
