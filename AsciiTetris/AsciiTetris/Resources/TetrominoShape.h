/*=================================================================|
|* File:				TetrominoShape.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef TETROMINO_SHAPE_H
#define TETROMINO_SHAPE_H

#include <AsciiTetris/Helpers/PointList.h>
#include <vector>
#include <memory>
#include <string>

using namespace AsciiTetris::Helpers;

namespace AsciiTetris::Resources {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* A tetromino usually consisting of four tetromino blocks. */
class TetrominoShape {

	//=========== MEMBERS ============
	#pragma region Members

	/* The list of blocks in the tetromino. */
	PointList2I blocks;
	/* The color of the tetromino. */
	unsigned char color;
	/* The name of the tetromino. */
	std::string name;
	/* True if a warning sound is played when the tetromino appears. */
	bool warning;
	/* The amount of times the shape has been placed. */
	int placeCount;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Creates a tetromino data structure with information. */
	TetrominoShape(const std::string& name, unsigned char color, bool warning, Point2F origin, const std::string& data);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the list of blocks in the tetromino with optional rotations. */
	const PointList2I& getBlocks() const;
	/* Gets the block at the specified index with optional rotations. */
	Point2I getBlockAt(int index) const;
	/* Gets the number of blocks in the tetromino. */
	int getNumBlocks() const;
	/* Gets the origin of the Tetromino used for rotation. */
	Point2F getOrigin() const;
	/* Gets the center of the Tetromino used for drawing in the next box. */
	Point2F getCenter() const;
	/* Gets the bounding box of the tetromino. */
	Rectangle2I getBounds() const;
	/* Gets the color of the tetromino. */
	unsigned char getColor() const;
	/* Gets the name of the tetromino. */
	std::string getName() const;
	/* Gets if a warning sound plays when the tetromino appears. */
	bool hasWarning() const;
	/* Gets the amount of times the shape has been placed. */
	int getPlaceCount() const;
	/* Increments the amount of times the shape has been placed. */
	void incrementPlaceCount();
	/* Clears the amount of times the shape has been placed. */
	void clearPlaceCount();

	#pragma endregion
};

DECLARE_SMART_PTRS(TetrominoShape);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
