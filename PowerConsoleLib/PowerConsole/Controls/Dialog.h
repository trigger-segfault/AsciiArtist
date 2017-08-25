/*=================================================================|
|* File:				DialogNew.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef DIALOG_NEW_H
#define DIALOG_NEW_H

#include <PowerConsole/Controls/Window.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The types of modes the dialog can be in. */
enum class DialogModes {
	/* The dialog will be shown inside the current window. */
	InnerWindow,
	/* The dialog will resize and takeover the window. */
	EntireWindow
};

/* The base class for all dialogs that show from within the main window. */
class Dialog : public Window {

	//=========== MEMBERS ============
	#pragma region Members

	// Dialog
	/* The dialog mode to use for this dialog. */
	DialogModes dialogMode;
	/* The result of the dialog. Best used to tell if an action was sucessful or canceled. */
	bool dialogResult;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the dialog. */
	Dialog(DialogModes dialogMode);
	/* Initializes the dialog. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Draws the control to the graphics object. */
	void repaint(PcDraw::GraphicsSPtr g) final;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Control Tree

	/* Sets the owner of the window. */
	void setOwner(WindowSPtr owner) final;

	#pragma endregion
	//--------------------------------
	#pragma region Dialog

	/* Gets the dialog mode to use for this dialog. */
	DialogModes getDialogMode() const;
	/* Gets the result of the dialog. Best used to tell if an action was sucessful or canceled. */
	bool getDialogResult() const;
	/* Sets the result of the dialog. Best used to tell if an action was sucessful or canceled. */
	void setDialogResult(bool dialogResult);

	#pragma endregion
	//--------------------------------
	#pragma region Local Layout

	/* Gets the size of the control. */
	PcGeo::Point2I getSize() const final;
	/* Sets the size of the control. */
	void setSize(PcGeo::Point2I size) final;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onGlobalTick(ControlSPtr sender, TickEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
