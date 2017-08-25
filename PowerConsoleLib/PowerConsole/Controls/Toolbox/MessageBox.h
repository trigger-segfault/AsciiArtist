/*=================================================================|
|* File:				MessageBox.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Helpers/WordWrapper.h>

#undef MessageBox

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Shows a popup window with a message to the user. Gives options for different types of buttons and returns the pressed button as the result. */
class MessageBox : public DialogBox {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The default maximum width for message boxes in the show command. */
	static const int DEFAULT_MAX_WIDTH = 60;
	/* The default minimum width for message boxes in the show command. */
	static const int DEFAULT_MIN_WIDTH = 32;
	/* The default minimum number of lines to use before expanding the width for message boxes in the show command. */
	static const int DEFAULT_MIN_LINES = 2;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Setup
	/* The message to show. */
	std::string message;
	/* The data for how to draw the message with wrapping. */
	PcHelp::WordWrapResult wordWrap;

	// Controls
	PanelSPtr panelMessage;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the message box. */
	MessageBox(const std::string& message, const std::string& title, DialogBoxButtons buttons, DialogBoxIcons icon,
		int maxWidth, int minWidth, int minLines);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a message box with the specified settings. */
	static DialogBoxResults show(WindowSPtr owner, const std::string& message,
		SystemSounds sound = SystemSounds::None, int maxWidth = DEFAULT_MAX_WIDTH, int minWidth = DEFAULT_MIN_WIDTH, int minLines = DEFAULT_MIN_LINES);
	/* Shows a message box with the specified settings. */
	static DialogBoxResults show(WindowSPtr owner, const std::string& message, const std::string& title,
		SystemSounds sound = SystemSounds::None, int maxWidth = DEFAULT_MAX_WIDTH, int minWidth = DEFAULT_MIN_WIDTH, int minLines = DEFAULT_MIN_LINES);
	/* Shows a message box with the specified settings. */
	static DialogBoxResults show(WindowSPtr owner, const std::string& message, const std::string& title, DialogBoxButtons buttons,
		SystemSounds sound = SystemSounds::None, int maxWidth = DEFAULT_MAX_WIDTH, int minWidth = DEFAULT_MIN_WIDTH, int minLines = DEFAULT_MIN_LINES);
	/* Shows a message box with the specified settings. */
	static DialogBoxResults show(WindowSPtr owner, const std::string& message, const std::string& title, DialogBoxIcons icon,
		SystemSounds sound = SystemSounds::None, int maxWidth = DEFAULT_MAX_WIDTH, int minWidth = DEFAULT_MIN_WIDTH, int minLines = DEFAULT_MIN_LINES);
	/* Shows a message box with the specified settings. */
	static DialogBoxResults show(WindowSPtr owner, const std::string& message, const std::string& title, DialogBoxButtons buttons, DialogBoxIcons icon,
		SystemSounds sound = SystemSounds::None, int maxWidth = DEFAULT_MAX_WIDTH, int minWidth = DEFAULT_MIN_WIDTH, int minLines = DEFAULT_MIN_LINES);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onMessagePaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(MessageBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
