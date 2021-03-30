/*=================================================================|
|* File:				TetrisWindow.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef TETRIS_WINDOW_H
#define TETRIS_WINDOW_H

#include <PowerConsole/Controls/MainWindow.h>
#include <AsciiTetris/Game/GameInstance.h>
#include <AsciiTetris/Resources/TetrominoShape.h>
#include <AsciiTetris/Resources/HighscoreList.h>
#include <AsciiTetris/Resources/AIList.h>
#include <AsciiTetris/UI/SelectionList.h>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <MP3/MP3.h>

using namespace PowerConsole::Controls;
using namespace AsciiTetris::Game;
using namespace AsciiTetris::Resources;
using namespace AsciiTetris::UI;

namespace AsciiTetris {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* The base manager for the game. */
class TetrisWindow : public MainWindow {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The frame rate for the game. */
	static const int FRAME_RATE = 1000 / 60;
	/* The animation speed for the selection timer flashing. */
	static const int ANIMATION_LENGTH = 400 / FRAME_RATE;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The current game instance being run. */
	GameInstanceSPtr game;

	/* The background image. */
	AsciiImageSPtr background;

	/* The block size setting for tetromino drawing. */
	Point2I blockSize;
	//int blockSize;

	/* The currently selected game mode. */
	GameMode gameMode;
	/* The currently selected AI. */
	AISettings ai;

	/* The index of the current game mode. */
	int gameModeIndex;
	/* The index of the current AI. -1 means it's the player. */
	int aiIndex;

	// Controls
	SelectionListSPtr selectionList;

	sfe::Mp3 mp3;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Contructs the tetris manager. */
	TetrisWindow();
private:
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//=========== RUNNING ============
	#pragma region Running
public:
	/* Starts a game instance. */
	void startGame();
	/* Reloads the AIs, highscores, and game modes. */
	void reload();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onGlobalTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onItemSelected(ControlSPtr sender, EventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
