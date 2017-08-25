/*=================================================================|
|* File:				HighscoreMenu.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef HIGHSCORE_MENU_H
#define HIGHSCORE_MENU_H

#include <PowerConsole/Controls/Dialog.h>
#include <AsciiTetris/Resources/HighscoreList.h>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Drawing;
using namespace AsciiTetris::Resources;

namespace AsciiTetris::Menus {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The base manager for the game. */
class HighscoreMenu : public Dialog {

	//========== CONSTANTS ===========
	#pragma region Constants

	/* The height of the highscore list. */
	static const int LIST_HEIGHT = 35;
	/* The maximum length of a highscore name. */
	static const int MAX_NAME_LENGTH = 15;
	/* The time in milliseconds it takes for the cursor to change states. */
	static const int CARET_FLASH_TIME = 560;

	/* The speed at which the selection changes when holding shift. */
	static const int SHIFT_SPEED = 5;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The background image. */
	AsciiImageSPtr background;

	/* The game mode of the highscores. */
	GameMode gameMode;
	/* True if a highscore holders's name is being typed in. */
	bool addingHighscore;
	/* The selection of the menu. */
	int selection;

	// Visibility
	/* True if players are visible. */
	bool players;
	/* True if AIs are visible. */
	bool ais;

	// Scrollbars
	/* The offset of the viewed object. */
	int viewPosition;
	/* The offset of the scrollbar. */
	int scrollPosition;
	/* The size of the scroll area. */
	int scrollSize;
	/* The size of the scroll thumb. */
	int thumbSize;
	/* The size of the viewport. */
	int viewportSize;
	/* The size of the viewed object. */
	int objectSize;
	/* The ratio between the scroll size and view size. */
	float scrollRatio;
	/* True if the scrollbar is active. */
	bool scrollbarActive;

	// TextBox
	/* The index of the caret in the textbox. */
	int caretIndex;
	/* The timer used for caret flashing. */
	int caretTimer;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Contructs the tetris manager. */
	HighscoreMenu(GameMode gameMode, Highscore* highscoreToAdd);
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows the highscore menu. */
	static void show(WindowSPtr owner, GameMode gameMode);
	/* Shows the highscore menu. */
	static void show(WindowSPtr owner, GameMode gameMode, Highscore highscoreToAdd);

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers
private:
	/* Sets the offset of the viewed object. */
	void setViewPosition(int viewPosition, bool relative = false);
	/* Sets the position of the scrollbars. */
	void setScrollPosition(int scrollPosition, bool relative = false);
	/* Sets the dimensions of the scrollbars. */
	void resizeScrollBars(int objectSize);
	/* Scrolls the selection into view. */
	void scrollToSelection();
	/* Finishes typing in the name for the highscore holder. */
	void finishTyping();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onKey(ControlSPtr sender, KeyboardEventArgs& e);
	void onKeyTyped(ControlSPtr sender, KeyTypedEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
