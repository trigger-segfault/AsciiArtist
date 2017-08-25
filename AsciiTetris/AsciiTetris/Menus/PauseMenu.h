/*=================================================================|
|* File:				PauseMenu.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <PowerConsole/Controls/Dialog.h>
#include <AsciiTetris/Resources/GameModeList.h>
#include <AsciiTetris/UI/SelectionList.h>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Drawing;
using namespace AsciiTetris::Resources;
using namespace AsciiTetris::UI;

namespace AsciiTetris::Menus {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum class PauseMenuResults {
	Resume,
	Restart,
	Quit
};

/* The base manager for the game. */
class PauseMenu : public Dialog {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The background image. */
	AsciiImageSPtr background;

	/* The result of the pause menu. */
	PauseMenuResults result;
	/* The current game mode. */
	GameMode gameMode;

	// Controls
	SelectionListSPtr selectionList;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Contructs the tetris manager. */
	PauseMenu(GameMode gameMode);
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows the pause menu. */
	static PauseMenuResults show(WindowSPtr owner, GameMode gameMode);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onKey(ControlSPtr sender, KeyboardEventArgs& e);
	void onItemSelected(ControlSPtr sender, EventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
