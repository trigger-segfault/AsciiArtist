/*=================================================================|
|* File:				TetrisController.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TetrisController.h"

using namespace AsciiTetris::Game;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

TetrisController::TetrisController(TetrisWellWPtr well)
 :	well(well),
	liveTetromino(nullptr),
	nextTetromino(nullptr) {}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void TetrisController::setNextTetromino(TetrominoSPtr tetromino) {
	liveTetromino = nextTetromino;
	nextTetromino = tetromino;
}
void TetrisController::setNextTetrominoSilent(TetrominoSPtr tetromino) {
	nextTetromino = tetromino;
}

#pragma endregion
//=================================================================|
