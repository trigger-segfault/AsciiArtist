/*=================================================================|
|* File:				TetrisWell.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TetrisWell.h"
#include <AsciiTetris/Game/Tetrominos/Tetromino.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Graphics.h>
#include <PowerConsole/PowerConsoleSettings.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace AsciiTetris::Game;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

TetrisWell::TetrisWell(Point2I dimensions, Point2I blockSize, bool sfxEnabled) {
	this->dimensions = dimensions;
	this->blockSize = blockSize;
	this->well = fixed_array<TetrominoBlockSPtr>(dimensions.x * dimensions.y, nullptr);

	this->occupiedRows = fixed_array<bool>(dimensions.y, false);

	this->clearAnimationTimer = 0;
	this->sfxEnabled = sfxEnabled;
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool TetrisWell::isAnimating() const {
	return clearAnimationTimer > 0;
}
bool TetrisWell::isRowCleared() const  {
	return !clearedRows.empty();
}
const std::vector<int>& TetrisWell::getClearedRows() const {
	return clearedRows;
}
Point2I TetrisWell::getDimensions() const {
	return dimensions;
}
Point2I TetrisWell::getBlockSize() const {
	return blockSize;
}
Point2I TetrisWell::getStartPosition() const {
	//return Point2I((int)GMath::ceil((float)dimensions.x / 2.0f), 0);
	return Point2I(dimensions.x / 2, 0);
}
const fixed_array<TetrominoBlockSPtr>& TetrisWell::getWell() const {
	return well;
}

#pragma endregion
//======== TETRIS CHECKS =========
#pragma region Tetris Checks

bool TetrisWell::isColliding(const PointList2I& tetromino) const {
	for (int i = 0; i < tetromino.count(); i++) {
		// First check if we're colliding with the well walls.
		// We don't want to be checking out of bounds indecies in the array.
		if (tetromino[i].x < 0 || tetromino[i].x >= dimensions.x || tetromino[i].y >= dimensions.y)
			return true;

		// Make sure to not bother checking above the well. That's allowed territory.
		if (tetromino[i].y >= 0 && well[pointToIndex(tetromino[i], dimensions.x)] != nullptr)
			return true;
	}
	return false;
}
int TetrisWell::getLandingPosition(TetrominoSPtr tetromino) const {
	PointList2I shape = tetromino->getTranslatedShape();

	for (int y = tetromino->getWellPosition().y + 1; y <= dimensions.y; y++) {
		if (isColliding(shape.translate(0, 1)))
			return y - tetromino->getWellPosition().y - 1;
	}
	return 0;
}
int TetrisWell::getLandingPosition(PointList2I& tetromino) const {
	int startY = tetromino[0].y;
	for (int y = startY + 1; y <= dimensions.y; y++) {
		if (isColliding(tetromino.translate(0, 1))) {
			tetromino.translate(0, -1);
			return y - startY - 1;
		}
	}
	return 0;
}
bool TetrisWell::isRowCleared(int row) const {
	for (int x = 0; x < dimensions.x; x++) {
		if (well[xyToIndex(x, row, dimensions.x)] == nullptr)
			return false;
	}
	return true;
}
bool TetrisWell::isRowOccupied(int row) const {
	return occupiedRows[row];
}

#pragma endregion
//======= TETRIS FUNCTIONS =======
#pragma region Tetris Functions

bool TetrisWell::placeTetromino(TetrominoSPtr tetromino) {
	PointList2I translatedShape = tetromino->getTranslatedShape(0, 0);
	std::vector<int> rowsToCheck;

	// Check if we should trigger a game over
	if (isColliding(translatedShape))
		return false;

	// Place new tetromino blocks in the well.
	for (int i = 0; i < translatedShape.count(); i++) {
		bool rowWasNotOccupied = false;

		// We landed outside the bounds. That's a game over.
		if (translatedShape[i].y < 0)
			return false;

		// Set the empty row to occupied
		if (!occupiedRows[translatedShape[i].y]) {
			occupiedRows[translatedShape[i].y] = true;

			// We keep this so we know not to add this row to the row check.
			// This will be ignored if the dimensions.x value is small enough
			//   for a row to be cleared with a single tetromino.
			rowWasNotOccupied = true;
		}

		if (!rowWasNotOccupied || dimensions.x == translatedShape.bounds().size.x) {
			// Determine if this row as already been flagged for checking. If not, flag it.
			for (int j = 0; j <= (int)rowsToCheck.size(); j++) {
				if (j == (int)rowsToCheck.size()) {
					rowsToCheck.push_back(translatedShape[i].y);
					break;
				}
				else if (rowsToCheck[j] == translatedShape[i].y) {
					break;
				}
			}
		}

		if (well[pointToIndex(translatedShape[i], dimensions.x)] == nullptr) {
			// Add the tetromino block.
			well[pointToIndex(translatedShape[i], dimensions.x)] = std::make_shared<TetrominoBlock>(tetromino->getColor());
		}
		else {
			// This shouldn't happen.
			throw std::exception("Error in 'TetrisWell::placeTetromino(...)'. Well block already exists");
		}
	}

	// Now check for cleared rows.
	clearedRows.clear();
	for (int i = 0; i < (int)rowsToCheck.size(); i++) {
		if (isRowCleared(rowsToCheck[i])) {
			// Let the well know that this row is no longer occupied.
			// This may change after the rows are dropped.
			occupiedRows[rowsToCheck[i]] = false;

			// Keep track of this row because it was cleared.
			clearedRows.push_back(rowsToCheck[i]);
		}
	}
	std::sort(clearedRows.begin(), clearedRows.end());

	if (!clearedRows.empty()) {
		if (sfxEnabled)
			clearAnimationTimer = (clearedRows.size() >= 4 ? TETRIS_ANIM_LENGTH : ANIM_LENGTH);
		else
			clearAnimationTimer = 2;
	}

	// Actually clear the rows. (May move this till after the animation finishes)
	clearRows();
	return true;
}

void TetrisWell::clearRows() {
	for (int i = 0; i < (int)clearedRows.size(); i++) {
		for (int x = 0; x < dimensions.x; x++) {
			// No messy cleanup. Thank you smart pointers! ^_^
			well[xyToIndex(x, clearedRows[i], dimensions.x)] = nullptr;
		}
	}
}

void TetrisWell::dropRows() {
	// Drop every row starting above
	for (int row = clearedRows[clearedRows.size() - 1] - 1; row >= 0; row--) {
		// Skip unoccupied rows.
		if (!occupiedRows[row]) {
			if (row < clearedRows[0]) {
			//if (i < clearedRows[clearedRows.size() - 1]) {
				// No need to check any more rows above. They're empty from here on out.
				break;
			}
			else {
				// Skip this row because it's one of the cleared rows.
				continue;
			}
		}

		// Find how many rows this row needs to be dropped by.
		int rowsToDrop = 0;
		for (int j = 0; j < (int)clearedRows.size(); j++) {
			if (row < clearedRows[j]) {
				rowsToDrop = (int)clearedRows.size() - j;
				break;
			}
		}
		// Drop the rows
		for (int x = 0; x < dimensions.x; x++) {
			well[xyToIndex(x, row + rowsToDrop, dimensions.x)] = well[xyToIndex(x, row, dimensions.x)];
			well[xyToIndex(x, row, dimensions.x)] = nullptr;
		}

		occupiedRows[row + rowsToDrop] = occupiedRows[row];
		occupiedRows[row] = false;
	}
	// Clear the saved rows because we don't need them anymore.
	clearedRows.clear();
}

void TetrisWell::updateAnimation() {
	// Decrement the animation timer.
	if (clearAnimationTimer > 0) {
		clearAnimationTimer--;
		if (clearAnimationTimer == 0) {
			// Do something about it.
			dropRows();
		}
	}
}

#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void TetrisWell::draw(GraphicsSPtr g, TetrominoSPtr tetromino, bool running, int tick) {

	// Draw well walls.
	/*unsigned char wallColor = toColor(BLACK, DARK_BLUE);
	if (!running)
		wallColor = toColor(BLACK, DARK_RED);
	g->drawBorder(Rectangle2I(0, 0, dimensions * blockSize + 2), Pixel(' ', wallColor), true);*/


	// Draw grid. (optional)
	const unsigned char gridColor = toColor(BLACK, GRAY);

	// Draw blocks in the well and the grid.
	for (int y = 0; y < dimensions.y; y++) {
		// Check if we can skip this row.

		if (PowerConsoleSettings::isDebugging() && occupiedRows[y])
			g->setPixel(Rectangle2I(Point2I(dimensions.x, y) * blockSize, 1, blockSize.y), Pixel('O', toColor(BLACK, DARK_GRAY)));

		if (!occupiedRows[y])
			continue;

		for (int x = 0; x < dimensions.x; x++) {
			int index = xyToIndex(x, y, dimensions.x);
			if (well[index] != nullptr)
				TetrominoBlock::draw(g, Point2I(x, y), blockSize, well[index]->getColor());
		}
	}
	std::string clearNames[5] = { "Single", "Double", "Triple", "Tetris", "Quintuple" };

	if (clearAnimationTimer > 0) {
		const unsigned char hues[7] = {
			RED, WHITE, YELLOW, GREEN, CYAN, BLUE, MAGENTA
		};
		const unsigned char hue = hues[(tick / 3) % 7];
		const unsigned char clearColor = toColor(DARK_GRAY, hue);
		const unsigned char clearDashColor = toColor(DARK_GRAY, hue);
		// Draw clear animations.
		for (int i = 0; i < (int)clearedRows.size(); i++) {
			g->setPixel(Rectangle2I(0, clearedRows[i] * blockSize.y, Point2I(dimensions.x, 1) * blockSize), Pixel(' ', clearColor));
			for (int x = 0; x < (dimensions.x * blockSize.x + 5) / 3; x++) {
				for (int y = 0; y < blockSize.y; y++) {
					int offset = x * 3 + (tick / 2) % 3 - y;
					if (offset < 0 || offset >= dimensions.x * blockSize.x)
						continue;
					g->setPixel(Point2I(offset, clearedRows[i] * blockSize.y + y), Pixel('-', clearDashColor));
				}
			}
			
			g->drawString(Point2I(0, clearedRows[i] * blockSize.y + (blockSize.y - 1) / 2), clearNames[clearedRows.size() - 1], Pixel(' ', clearColor), HorizontalAlignments::Center, dimensions.x * blockSize.x);
		}
	}
	else {
		g->startRegion(Point2I::ZERO, Rectangle2I(dimensions * blockSize));
		// Calculate the position of the live tetromino ghost.
		PointList2I translatedShape = tetromino->getTranslatedShape(0, 1);
		while (!isColliding(translatedShape)) {
			translatedShape.translate(0, 1);
		}

		// Translate up one so we're now where the live tetromino will land.
		translatedShape.translate(0, -1);

		// Draw the live tetromino ghost.
		if (running) {
			for (int i = 0; i < translatedShape.count(); i++) {
				TetrominoBlock::drawGhost(g, translatedShape[i], blockSize);
			}
		}

		// Draw the live tetromino.
		tetromino->drawLive(g, blockSize);
		g->endRegion();
	}
	//g->endRegion();
}

#pragma endregion
//=================================================================|
