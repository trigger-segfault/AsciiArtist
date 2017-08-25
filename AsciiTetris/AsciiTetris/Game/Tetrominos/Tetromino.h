/*=================================================================|
|* File:				Tetromino.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef TETROMINO_H
#define TETROMINO_H

#include <PowerConsole/Drawing/Graphics.h>
#include <AsciiTetris/Resources/TetrominoShape.h>
#include <memory>

using namespace PowerConsole::Drawing;
using namespace AsciiTetris::Resources;

DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(AsciiTetris::Game, TetrisWell);
using namespace AsciiTetris::Game;

namespace AsciiTetris::Game::Tetrominos {
//=================================================================|
// ENUMERATIONS                                                    |
//=================================================================/
/* The different ways a tetromino can be dropped. */
enum class DropModes {
	Normal,
	SoftDrop,
	HardDrop
};

//=================================================================|
// CLASSES														   |
//=================================================================/
/* A tetromino usually consisting of four tetromino blocks. */
class Tetromino : public std::enable_shared_from_this<Tetromino> {

	//=========== MEMBERS ============
	#pragma region Members

	/* The reference to the well with all of the placed tetrominos. */
	TetrisWellWPtr well;

	/* The structure of the tetromino. */
	TetrominoShapeSPtr baseShape;
	/* The current shape of the tetromino. */
	PointList2I currentShape;

	/* The current integer-based position in the well. */
	Point2I wellPosition;
	/* The floating-point position of the falling Tetromino in the current cell. */
	float dropPosition;

	/* The mode used to determine the speed of dropping.*/
	DropModes dropMode;

	/* True if the tetromino has landed in the well. */
	bool landed;
	/* The drop speed of the tetromino. One unit equals one block. */
	float dropSpeed;

	/* The score accumulated from using and landing with soft drop. */
	/* Score resets if you stop using soft drop. */
	int dropScore;
	/* The distance passed while soft dropping. */
	float dropDistance;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Creates a tetromino block with the specified color. */
	Tetromino(TetrisWellWPtr well, TetrominoShapeSPtr shape);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the base shape of the tetromino. */
	const TetrominoShapeSPtr& getBaseShape() const;
	/* Gets the current shape of the tetromino. */
	const PointList2I& getCurrentShape() const;
	/* Gets the position of the tetromino in the well. */
	Point2I getWellPosition() const;
	/* Gets the landing position in the well. */
	float getDropPosition() const;
	/* Gets the drop speed of the tetromino. */
	float getDropSpeed() const;
	/* Gets the translated shape. */
	PointList2I getTranslatedShape() const;
	/* Gets the translated shape with the specified offset. */
	PointList2I getTranslatedShape(int offsetX, int offsetY) const;
	/* Gets the translated rotated shape. */
	PointList2I getTranslatedRotatedShape(int rotations) const;
	/* Gets the translated rotated shape with the specified offset. */
	PointList2I getTranslatedRotatedShape(int rotations, int offsetX, int offsetY) const;
	/* Gets if the tetromino has landed. */
	bool hasLanded() const;
	/* Gets the accumulated drop score of the tetromino. */
	int getDropScore() const;
	/* Gets the color of the tetromino. */
	unsigned char getColor() const;

	#pragma endregion
	//=========== CONTROL ============
	#pragma region Control

	/* Activates the tetromino and sets the drop speed. */
	void activate(float dropSpeed);
	/* Updates the tetromino. */
	void update();
	/* Moves the tetromino to the left. */
	void moveLeft(bool hardLeft);
	/* Moves the tetromino to the right. */
	void moveRight(bool hardRight);
	/* Rotates the tetromino 90 degrees. */
	void rotate90();
	/* Rotates the tetromino 180 degrees. */
	void rotate180();
	/* Rotates the tetromino 270 degrees. */
	void rotate270();
	/* Sets the drop mode of the tetromino. */
	void setDropMode(DropModes dropMode);

	/* Moves the tetromino without checks. */
	void move(int distance);
	/* Rotates the tetromino the specified amount of rotations. */
	void rotate(int rotations);
private:
	/* Adds soft drop score based on distance. */
	void addSoftDropScore(float distance);
	/* Adds hard drop score based on distance. */
	void addHardDropScore(float distance);

	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing
public:
	/* Draws the live tetromino. */
	void drawLive(GraphicsSPtr g, Point2I blockSize);
	/* Draws the next tetromino. */
	void drawNext(GraphicsSPtr g, Point2I center, Point2I blockSize);

	#pragma endregion
};

DECLARE_SMART_PTRS(Tetromino);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
