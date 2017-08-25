/*=================================================================|
|* File:				TetrominoBlock.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef TETROMINO_BLOCK_H
#define TETROMINO_BLOCK_H

#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <memory>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;

namespace AsciiTetris::Game::Tetrominos {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* A black in the Tetris Well. */
class TetrominoBlock {

	//=========== MEMBERS ============
	#pragma region Members

	/* The color of the tetromino block. */
	unsigned char color;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Creates a tetromino block with the specified color. */
	TetrominoBlock(unsigned char color);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the color of the tetromino block. */
	unsigned char getColor();

	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Draws the tetromino block. */
	static void draw(GraphicsSPtr g, Point2F position, Point2I blockSize, unsigned char color, Point2I offset = Point2I::ZERO);
	/* Draws the tetromino ghost block. */
	static void drawGhost(GraphicsSPtr g, Point2I position, Point2I blockSize);

	#pragma endregion
};

DECLARE_SMART_PTRS(TetrominoBlock);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
