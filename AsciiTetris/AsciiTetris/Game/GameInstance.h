/*=================================================================|
|* File:				GameInstance.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

#include <PowerConsole/Controls/Dialog.h>
#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <AsciiTetris/Menus/PauseMenu.h>
#include <AsciiTetris/UI/TextSlider.h>
#include <AsciiTetris/Game/TetrisWell.h>
#include <AsciiTetris/Game/Tetrominos/Tetromino.h>
#include <AsciiTetris/Game/TetrisController.h>
#include <AsciiTetris/Resources/TetrominoShape.h>
#include <AsciiTetris/Resources/TetrominoList.h>
#include <AsciiTetris/Resources/HighscoreList.h>
#include <AsciiTetris/Resources/AIList.h>
#include <SFML/Audio.hpp>
#include <memory>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;

using namespace AsciiTetris::Resources;
using namespace AsciiTetris::Menus;
using namespace AsciiTetris::UI;
using namespace AsciiTetris::Game::Tetrominos;

namespace AsciiTetris::Game {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum class GameInstanceResults {
	GameOver,
	Restart,
	Quit
};

/* The manager for running the current session of tetris. */
class GameInstance : public Dialog {

	//========== CONSTANTS ===========
	#pragma region Constants

	/* The level cap. */
	static const int MAX_LEVEL = 15;
	/* The number of lines required to level up. */
	static const int LINES_PER_LEVEL = 15;

	/* The score added for each amount of lines cleared. */
	static const int LINE_SCORE_CHART[];
	/* The score added for each combo. */
	static const int COMBO_SCORE = 50;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* The result when returning from the game instance. */
	GameInstanceResults result;

	/* True if the game is still running. */
	bool running;
	/* The current game mode. */
	GameMode gameMode;
	/* The highscore to save the stats to. */
	Highscore highscore;
	/* True if special effects are enabled. */
	bool sfxEnabled;
	/* True if the user can continue to the highscore menu after a gameover. */
	/* This is set to true after one GAME OVER message cycle. */
	bool gameOverInput;

	/* The number of lines left before leveling up. */
	int linesLeft;
	/* The current line combo. */
	int combo;
	/* The amount of lines that were cleared in one tetromino placement. Used for multi-tetris bonuses. */
	int linesCleared;
	/* The current drop speed of tetrominos. */
	float dropSpeed;

	/* The controller that gives the game input. */
	TetrisControllerSPtr controller;
	/* The contained tetris well. */
	TetrisWellSPtr well;
	/* The current tetromino being placed. */
	TetrominoSPtr liveTetromino;
	/* The tetromino to be used next. */
	TetrominoSPtr nextTetromino;

	// Control
	TextSliderSPtr textSlider;

	#pragma endregion
	//=========== RUNNING ============
	#pragma region Running
public:
	/* Runs a player-controlled game instance. */
	static GameInstanceResults runPlayerGame(WindowSPtr owner, Highscore& highscoreResult, GameMode gameMode);
	/* Runs an AI-controlled game instance. */
	static GameInstanceResults runAIGame(WindowSPtr owner, Highscore& highscoreResult, GameMode gameMode, AISettings ai);

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

private:
	/* Constructs a game instance. */
	GameInstance(GameMode gameMode, AISettings* ai);
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
public:
	/* Gets the current score. */
	long long getScore();
	/* Gets the current level. */
	int getLevel();
	/* Gets the current drop speed. */
	float getDropSpeed();
	/* Gets the current tetromino being dropped. */
	TetrominoSPtr getLiveTetromino();
	/* Gets the next tetromino to drop. */
	TetrominoSPtr getNextTetromino();
	/* Returns true if the game instance is running. */
	bool isRunning();

	#pragma endregion
	//=========== CONTROL ============
	#pragma region Control

	/* Tries to add the score to the total. Sets the infinity flag if it cannot go any higher. */
	void tryAddScore(int scoreToAdd);
	/* Calculates the drop speed based on the level. */
	void calculateDropSpeed();
	/* Randomly selects the next tetromino to use. Returns true if the tetromino is not colliding. */
	bool generateNextTetromino(bool allowPentominos);
	/* Generates a game over. */
	void gameOver();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onGlobalTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onTextSliderAnimationFinished(ControlSPtr sender, EventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(GameInstance);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
