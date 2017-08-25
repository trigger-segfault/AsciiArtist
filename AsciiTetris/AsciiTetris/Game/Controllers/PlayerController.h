/*=================================================================|
|* File:				PlayerController.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <AsciiTetris/Game/TetrisController.h>

using namespace AsciiTetris::Game;

namespace AsciiTetris::Game::Controllers {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* The controller run by user input. */
class PlayerController : public TetrisController {

	//========== CONSTANTS ===========
	#pragma region Constants

	/* The first delay before delayed auto shifting activates. */
	static const int DAS_DELAY = 12;
	/* The delay between delayed auto shifting repetitions. */
	static const int DAS_REPEAT = 4;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* The timer used for delayed auto shifting the tetromino left and right. */
	int dasTimer;
	/* True if the dominant direction is right. */
	bool dominantRight;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the player controller. */
	PlayerController(TetrisWellWPtr well);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Updates the player controller. */
	void update(const Keyboard& keyboard) final;
	/* Called after the last tetromio has landed and we need a new next tetromino. */
	void setNextTetromino(TetrominoSPtr tetromino) final;

	#pragma endregion
};

DECLARE_SMART_PTRS(PlayerController);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
