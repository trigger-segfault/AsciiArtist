/*=================================================================|
|* File:				ControlsMenu.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resources.h"
#include "ControlsMenu.h"
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

ControlsMenu::ControlsMenu()
	: Dialog(DialogModes::InnerWindow) {
	eventInitialize().add(bindEvent(ControlsMenu::onInitialize));
	eventLoad().add(bindEvent(ControlsMenu::onLoad));
	eventPaint().add(bindEvent(ControlsMenu::onPaint));
	eventKeyGlobal().add(bindEvent(ControlsMenu::onKey));

	background = std::make_shared<AsciiImage>();
	background->loadResource(ASC_CONTROLS_MENU);

	setSizeSafe(background->size());
}

void ControlsMenu::onInitialize(ControlSPtr sender, EventArgs& e) {

}
void ControlsMenu::onLoad(ControlSPtr sender, EventArgs& e) {
	
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

void ControlsMenu::show(WindowSPtr owner) {
	auto menu = makePrivateDialog(ControlsMenu);
	owner->showDialog(menu);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ControlsMenu::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	// Draw the background
	g->drawImage(Point2I::ZERO, *background);
}
void ControlsMenu::onKey(ControlSPtr sender, KeyboardEventArgs& e) {
	if (e.isKeyPressed(Keys::Enter) || e.isKeyPressed(Keys::Space) ||
		e.isKeyPressed(Keys::Backspace) || e.isKeyPressed(Keys::Escape)) {
		SoundList::playSound("MenuSelect");
		close();
	}
}

#pragma endregion
//=================================================================|
