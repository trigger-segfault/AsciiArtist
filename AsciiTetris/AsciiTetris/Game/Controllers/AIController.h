/*=================================================================|
|* File:				AIController.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H

#include <AsciiTetris/Game/TetrisController.h>
#include <AsciiTetris/Resources/AIList.h>
#include <queue>

using namespace AsciiTetris::Game;
using namespace AsciiTetris::Resources;

namespace AsciiTetris::Game::Controllers {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum class AIActions {
	Rotate90,
	Rotate180,
	Rotate270,
	MoveLeft,
	MoveHardLeft,
	MoveRight,
	MoveHardRight,
	SoftDrop,
	HardDrop,
	Wait
};

/* The controller run by artificial intelligence. */
class AIController : public TetrisController {

	//=========== MEMBERS ============
	#pragma region Members

	/* The AI settings. */
	AISettings ai;

	/* The queued actions to perform. */
	std::queue<AIActions> actions;
	
	/* The timer for performing actions. */
	int actionTimer;
	/* The timer for thinking. */
	int thinkTimer;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the AI controller. */
	AIController(TetrisWellWPtr well);
	/* Constructs the AI controller. */
	AIController(TetrisWellWPtr well, const AISettings& ai);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Calculates the score based on the final position of the tetromino. */
	int calculateScore(const PointList2I& shape) const;

	/* Determines which actions to take. */
	void determineActions();
	/* Performs the next action in the queue. */
	void doNextAction();

	/* Updates the AI controller. */
	void update(const Keyboard& keyboard) final;
	/* Called after the last tetromio has landed and we need a new next tetromino. */
	void setNextTetromino(TetrominoSPtr tetromino) final;

	#pragma endregion
};

DECLARE_SMART_PTRS(AIController);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
