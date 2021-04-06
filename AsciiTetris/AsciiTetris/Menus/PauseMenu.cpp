/*=================================================================|
|* File:				PauseMenu.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "PauseMenu.h"
#include <PowerConsole/PowerConsoleSettings.h>
#include <PowerConsole/Geometry/GMath.h>
#include <AsciiTetris/Menus/HighscoreMenu.h>
#include <AsciiTetris/Menus/ControlsMenu.h>
#include <AsciiTetris/Resources/SoundList.h>

using namespace PowerConsole;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace AsciiTetris;
using namespace AsciiTetris::Menus;
using namespace AsciiTetris::Resources;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

PauseMenu::PauseMenu(GameMode gameMode)
 :	Dialog(DialogModes::InnerWindow),

	result(PauseMenuResults::Resume),
	gameMode(gameMode) {

	eventInitialize().add(bindEvent(PauseMenu::onInitialize));
	eventLoad().add(bindEvent(PauseMenu::onLoad));
	eventTick().add(bindEvent(PauseMenu::onTick));
	eventPaint().add(bindEvent(PauseMenu::onPaint));
	eventKeyGlobal().add(bindEvent(PauseMenu::onKey));

	background = std::make_shared<AsciiImage>();
	background->loadResource(ASC_PAUSE_MENU);

	setSizeSafe(background->size());
}

void PauseMenu::onInitialize(ControlSPtr sender, EventArgs& e) {
	selectionList = makeControl(SelectionList);
	selectionList->setMargin(Thickness(0, 6, 0, 0));
	selectionList->addItem("Resume Game", CYAN);
	selectionList->addItem("Restart Game", MAGENTA);
	selectionList->addItem("Highscores", RED);
	selectionList->addItem("Controls", BLUE);
	selectionList->addItem("Quit Game", WHITE);
	selectionList->eventItemSelected().add(bindEvent(PauseMenu::onItemSelected));
	children().add(selectionList);
}
void PauseMenu::onLoad(ControlSPtr sender, EventArgs& e) {
	
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

PauseMenuResults PauseMenu::show(WindowSPtr owner, GameMode gameMode) {
	auto menu = makePrivateDialog(PauseMenu, gameMode);
	owner->showDialog(menu);
	return menu->result;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void PauseMenu::onTick(ControlSPtr sender, TickEventArgs& e) {
	
}
void PauseMenu::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	// Draw the background
	g->drawImage(Point2I::ZERO, *background);
}
void PauseMenu::onKey(ControlSPtr sender, KeyboardEventArgs& e) {
	if (e.isKeyPressed(Keys::Escape) || e.isKeyPressed(Keys::Backspace) || e.isHotkeyPressed(Keys::P)) {
		SoundList::playSound("MenuSelect");
		close();
	}
}
void PauseMenu::onItemSelected(ControlSPtr sender, EventArgs& e) {
	switch (selectionList->getSelectedIndex()) {
	case 0: // Resume Game
		result = PauseMenuResults::Resume;
		close();
		break;
	case 1: // Restart Game
		result = PauseMenuResults::Restart;
		close();
		break;
	case 2: // Highscores
		HighscoreMenu::show(getWindow(), gameMode);
		console().centerWindow();
		break;
	case 3: // Controls
		ControlsMenu::show(getWindow());
		break;
	case 4: // Quit Game
		result = PauseMenuResults::Quit;
		close();
		break;
	}
}

#pragma endregion
//=================================================================|
