/*=================================================================|
|* File:				ControlsMenu.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef CONTROLS_MENU_H
#define CONTROLS_MENU_H

#include <PowerConsole/Controls/Dialog.h>
#include <vector>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Drawing;

namespace AsciiTetris::Menus {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The base manager for the game. */
class ControlsMenu : public Dialog {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The background image. */
	AsciiImageSPtr background;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Contructs the tetris manager. */
	ControlsMenu();
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows the controls menu. */
	static void show(WindowSPtr owner);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onKey(ControlSPtr sender, KeyboardEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
