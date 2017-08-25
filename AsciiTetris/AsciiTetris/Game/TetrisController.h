/*=================================================================|
|* File:				TetrisController.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef TETRIS_CONTROLLER_H
#define TETRIS_CONTROLLER_H

#include <PowerConsole/Input/Keyboard.h>
#include <AsciiTetris/Game/TetrisWell.h>
#include <AsciiTetris/Game/Tetrominos/Tetromino.h>

using namespace PowerConsole::Input;
using namespace AsciiTetris::Game::Tetrominos;

namespace AsciiTetris::Game {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* The base controller for the tetromino. */
class TetrisController : public std::enable_shared_from_this<TetrisController> {

	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The reference to the well with all of the placed tetrominos. */
	TetrisWellWPtr well;
	/* The reference to the current tetromino. */
	TetrominoSPtr liveTetromino;
	/* The reference to the next tetromino. Only needed for the AI. */
	TetrominoSPtr nextTetromino;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tetris controller. */
	TetrisController(TetrisWellWPtr well);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Updates the controller. */
	virtual void update(const Keyboard& keyboard) = 0;
	/* Called after the last tetromio has landed and we need a new next tetromino. */
	virtual void setNextTetromino(TetrominoSPtr tetromino);
	/* Sets the next tetromino without affecting the live tetromino. */
	void setNextTetrominoSilent(TetrominoSPtr tetromino);

	#pragma endregion
};

DECLARE_SMART_PTRS(TetrisController);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
