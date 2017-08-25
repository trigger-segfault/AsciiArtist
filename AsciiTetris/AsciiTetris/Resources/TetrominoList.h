/*=================================================================|
|* File:				TetrominoList.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef TETROMINO_LIST_H
#define TETROMINO_LIST_H

#include <AsciiTetris/Resources/TetrominoShape.h>
#include <PowerConsole/IO/IniDocument.h>
#include <memory>
#include <vector>

using namespace PowerConsole::IO;

namespace AsciiTetris::Resources {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* The resource list for tetrominos. */
class TetrominoList {

	//=========== MEMBERS ============
	#pragma region Tetrominos

	/* The list of defined 4-block tetrominos. */
	static std::vector<TetrominoShapeSPtr> tetrominos;
	/* The list of defined 5-block tetrominos known as pentominos. */
	static std::vector<TetrominoShapeSPtr> pentominos;
	/* The list of defined custom tetrominos. */
	static std::vector<TetrominoShapeSPtr> custom;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Initializes the resources database. */
	//TetrominoList();

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the full list of tetrominos. */
	static const std::vector<TetrominoShapeSPtr>& getTetrominos();
	/* Gets the full list of pentominos. */
	static const std::vector<TetrominoShapeSPtr>& getPentominos();
	/* Gets the full list of custom tetrominos. */
	static const std::vector<TetrominoShapeSPtr>& getCustoms();
	/* Gets the tetromino at the specified index in the list. */
	static TetrominoShapeSPtr getTetromino(int index);
	/* Gets the pentomino at the specified index in the list. */
	static TetrominoShapeSPtr getPentomino(int index);
	/* Gets the custom tetromino at the specified index in the list. */
	static TetrominoShapeSPtr getCustom(int index);
	/* Gets the number of tetrominos. */
	static int getTetrominoCount();
	/* Gets the number of pentominos. */
	static int getPentominoCount();
	/* Gets the number of custom tetrominos. */
	static int getCustomCount();
	/* Gets the index of the tetromino in the list. */
	static int indexOfTetromino(TetrominoShapeSPtr tetromino);
	/* Gets the index of the pentomino in the list. */
	static int indexOfPentomino(TetrominoShapeSPtr pentomino);
	/* Gets the index of the custom tetromino in the list. */
	static int indexOfCustom(TetrominoShapeSPtr custom);

	#pragma endregion
	//=========== CREATION ===========
	#pragma region Creation
private:
	/* Adds a tetromino to the list. */
	static void addTetromino(const std::string& name, unsigned char color, Point2F origin, const std::string& data);
	/* Adds a pentomino to the list. */
	static void addPentomino(const std::string& name, unsigned char color, Point2F origin, const std::string& data);
	/* Adds a custom tetromino to the list. */
	static void addCustom(IniSectionSPtr section);
public:
	/* Creates the list of tetrominos. */
	static void createTetrominos();
	/* Creates the list of pentominos. */
	static void createPentominos();
	/* Loads the list of custom tetrominos. */
	static void loadCustom();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
