/*=================================================================|
|* File:				PlayerController.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PlayerController.h"

using namespace AsciiTetris::Game::Controllers;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

PlayerController::PlayerController(TetrisWellWPtr well)
 :	TetrisController(well),
	
	dasTimer(-1),
	dominantRight(false) {}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PlayerController::update(const Keyboard& keyboard) {
	// Update rotations
	if (keyboard.isKeyPressed(Keys::Up) || keyboard.isKeyPressed(Keys::C)) {
		liveTetromino->rotate90();
	}
	else if (keyboard.isKeyPressed(Keys::X)) {
		liveTetromino->rotate180();
	}
	else if (keyboard.isKeyPressed(Keys::Z)) {
		liveTetromino->rotate270();
	}

	// Update movement
	bool hardMove = keyboard.isKeyDown(Keys::Shift);
	if (dasTimer > 0) {
		dasTimer--;
	}

	if (keyboard.isKeyPressed(Keys::Left)) {
		dominantRight = false;
		dasTimer = DAS_DELAY;
		liveTetromino->moveLeft(hardMove);
	}
	else if (keyboard.isKeyPressed(Keys::Right)) {
		dominantRight = true;
		dasTimer = DAS_DELAY;
		liveTetromino->moveRight(hardMove);
	}
	if (keyboard.isKeyReleased(Keys::Left)) {
		dominantRight = true;
		dasTimer = (keyboard.isKeyDown(Keys::Right) ? DAS_DELAY : -1);
	}
	else if (keyboard.isKeyReleased(Keys::Right)) {
		dominantRight = false;
		dasTimer = (keyboard.isKeyDown(Keys::Left) ? DAS_DELAY : -1);
	}
	if (dasTimer == 0) {
		dasTimer = DAS_REPEAT;
		if (!dominantRight)
			liveTetromino->moveLeft(hardMove);
		else
			liveTetromino->moveRight(hardMove);
	}

	// Update drop mode
	if (keyboard.isKeyPressed(Keys::Space)) {
		liveTetromino->setDropMode(DropModes::HardDrop);
	}
	else if (keyboard.isKeyDown(Keys::Down)) {
		liveTetromino->setDropMode(DropModes::SoftDrop);
	}
	else {
		liveTetromino->setDropMode(DropModes::Normal);
	}
}
void PlayerController::setNextTetromino(TetrominoSPtr tetromino) {
	TetrisController::setNextTetromino(tetromino);
}

#pragma endregion
//=================================================================|
