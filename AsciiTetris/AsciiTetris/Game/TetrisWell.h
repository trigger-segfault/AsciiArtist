/*=================================================================|
|* File:				TetrisWell.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef TETRIS_WELL_H
#define TETRIS_WELL_H

#include <AsciiTetris/Helpers/PointList.h>
#include <AsciiTetris/Game/Tetrominos/TetrominoBlock.h>
#include <vector>
#include <memory>

using namespace AsciiTetris::Helpers;
using namespace AsciiTetris::Game::Tetrominos;

DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(AsciiTetris::Game::Tetrominos, Tetromino);

namespace AsciiTetris::Game {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* The grid containg all placed and currently placing tetrominos. */
class TetrisWell : public std::enable_shared_from_this<TetrisWell> {

	//========== CONSTANTS ===========
	#pragma region Constants

	/* The length of the row clear animation. */
	static const int ANIM_LENGTH = 500 / (1000 / 60);
	/* The length of the animation during a tetris or greater. */
	static const int TETRIS_ANIM_LENGTH = 800 / (1000 / 60);

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* The 1D grid for tetrominos. */
	fixed_array<TetrominoBlockSPtr> well;

	/* Used to save time wasted on checking empty rows. */
	fixed_array<bool> occupiedRows;

	/* The dimensions of the well grid. */
	Point2I dimensions;
	/* The size of each tetris block in the well. */
	Point2I blockSize;

	/* The list of cleared rows with the last tetromino placement. */
	std::vector<int> clearedRows;

	/* The timer for the row clear animation. */
	int clearAnimationTimer;

	/* True if special effects are enabled. */
	bool sfxEnabled;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Creates the Tetris Well with the specified dimensions and size. */
	TetrisWell(Point2I dimensions, Point2I blockSize, bool sfxEnabled);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* True if the row clear animation is present. */
	bool isAnimating() const;
	/* Checks if any row has been cleared. */
	bool isRowCleared() const;
	/* Checks if the row at the specified index is complete. */
	const std::vector<int>& getClearedRows() const;
	/* Gets the dimensions of the well. */
	Point2I getDimensions() const;
	/* Gets the size of each tetris block in the well. */
	Point2I getBlockSize() const;
	/* Gets the starting position for live tetrominos. */
	Point2I getStartPosition() const;
	/* Gets the block array. */
	const fixed_array<TetrominoBlockSPtr>& getWell() const;

	#pragma endregion
	//======== TETRIS CHECKS =========
	#pragma region Tetris Checks

	/* Returns true if the Tetromino is colliding with other blocks in the well. */
	bool isColliding(const PointList2I& tetromino) const;
	/* Returns the landing position of a tetromino. */
	int getLandingPosition(TetrominoSPtr tetromino) const;
	/* Returns the landing position of a tetromino. */
	int getLandingPosition(PointList2I& tetromino) const;

	/* Returns true if the specified row is full. Only needed by well. */
	bool isRowCleared(int row) const;
	/* Gets if the specified row is occupied. */
	bool isRowOccupied(int row) const;

	#pragma endregion
	//======= TETRIS FUNCTIONS =======
	#pragma region Tetris Functions

	/* Places the live tetromino. */
	/* Returns false if the tetromino failed to place, resulting in a game over. */
	bool placeTetromino(TetrominoSPtr tetromino);
	/* Removes the specified row. */
	void clearRows();
	/* Drops the rows starting at the given row index. */
	void dropRows();
	
	/* Updates the row clear animation. */
	void updateAnimation();

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Draws the well and all tetrominos inside. */
	void draw(GraphicsSPtr g, TetrominoSPtr tetromino, bool running, int tick);

	#pragma endregion
};

DECLARE_SMART_PTRS(TetrisWell);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
