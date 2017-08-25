/*=================================================================|
|* File:				AIController.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AIController.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace AsciiTetris::Game::Controllers;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

AIController::AIController(TetrisWellWPtr well)
 :	AIController(well, AISettings()) {}
AIController::AIController(TetrisWellWPtr well, const AISettings& ai)
 :	TetrisController(well),
	
	ai(ai),
	actionTimer(-1),
	thinkTimer(0) {}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

int AIController::calculateScore(const PointList2I& shape) const {
	auto well = this->well.lock();
	int score = 0;
	const fixed_array<TetrominoBlockSPtr>& grid = well->getWell();
	Point2I dimensions = well->getDimensions();

	#define getIndex(_x, _y) grid[xyToIndex((_x), (_y), dimensions.x)]
	#define getDroppedIndex(_x, _y) (((_y) - rowsDropped[(_y)] >= 0) ? grid[xyToIndex((_x), (_y) - rowsDropped[(_y)], dimensions.x)] : nullptr)

	#pragma region Lines Cleared

	// Calculate the lines cleared and rows to drop
	std::vector<int> clearedRows;
	fixed_array<int> rowsDropped = fixed_array<int>(dimensions.y, 0);
	for (int i = 0; i < shape.count(); i++) {
		// Determine if this row as already been flagged for checking. If not, flag it.
		for (int j = 0; j <= (int)clearedRows.size(); j++) {
			if (j == (int)clearedRows.size()) {
				if (shape[i].y >= 0)
					clearedRows.push_back(shape[i].y);
				break;
			}
			else if (clearedRows[j] == shape[i].y) {
				break;
			}
		}
	}
	for (int i = 0; i < (int)clearedRows.size(); i++) {
		for (int x = 0; x < dimensions.x; x++) {
			if (getIndex(x, clearedRows[i]) == nullptr) {
				clearedRows.erase(clearedRows.begin() + i);
				i--;
				break;
			}
		}
	}
	std::sort(clearedRows.begin(), clearedRows.end());
	if (!clearedRows.empty()) {
		for (int row = clearedRows[clearedRows.size() - 1] - 1; row >= 0; row--) {
			// Skip unoccupied rows.
			if (!well->isRowOccupied(row) && row < clearedRows[0]) {
				// No need to check any more rows above. They're empty from here on out.
				break;
			}

			// Find how many rows this row will be dropped by.
			int drops = 0;
			for (int j = 0; j < (int)clearedRows.size(); j++) {
				if (row < clearedRows[j]) {
					drops = (int)clearedRows.size() - j;
					break;
				}
			}
			rowsDropped[row + drops] = drops;
		}
	}
	int linesCleared = (int)clearedRows.size();

	#pragma endregion

	#pragma region Edges

	// Calculate edges
	#define calculateEdge(px, py) \
		if (point.x + px < 0 || point.x + px >= dimensions.x || point.y + py >= dimensions.y) \
			edges++; \
		else if (point.y + py >= 0 && getDroppedIndex(point.x + px, point.y + py) != nullptr) \
			edges++
	int edges = 0;
	for (int i = 0; i < shape.count(); i++) {
		Point2I point = shape[i];
		calculateEdge(1, 0);
		calculateEdge(0, 1);
		calculateEdge(-1, 0);
		calculateEdge(0, -1);
	}

	#pragma endregion

	#pragma region Height

	// Calculate height
	int height = 0;
	for (int i = 0; i < shape.count(); i++) {
		height += dimensions.y - shape[i].y - 1;
	}
	height -= linesCleared;

	#pragma endregion

	#pragma region Covered

	int covered = 0;
	int alreadyCovered = 0;
	std::vector<Point2I> columnsToCheck;
	for (int i = 0; i < shape.count(); i++) {
		bool cleared = false;
		for (int k = 0; k < (int)clearedRows.size(); k++) {
			if (shape[i].y == clearedRows[k]) {
				cleared = true;
				break;
			}
		}
		if (cleared)
			continue;
		for (int j = 0; j <= (int)columnsToCheck.size(); j++) {
			if (j == (int)columnsToCheck.size()) {
				columnsToCheck.push_back(shape[i]);
				break;
			}
			else if (columnsToCheck[j].x == shape[i].x) {
				columnsToCheck[j].y = std::max(columnsToCheck[j].y, shape[i].y);
				break;
			}
		}
	}
	for (int i = 0; i < (int)columnsToCheck.size(); i++) {
		int x = columnsToCheck[i].x;
		bool stage2 = false;
		for (int y = std::max(0, columnsToCheck[i].y + 1); y < dimensions.y; y++) {
			if (getDroppedIndex(x, y) == nullptr)
			//if (grid[xyToIndex(x, y, dimensions.x)] == nullptr)
				(stage2 ? alreadyCovered : covered)++;
			else
				stage2 = true;
		}
	}

	#pragma endregion

	// Add up the score
	score += edges * ai.edgeWeight;
	score += linesCleared * ai.lineClearWeight;
	score += height * ai.heightWeight;
	score += covered * ai.coveredWeight;
	score += alreadyCovered * ai.alreadyCoveredWeight;

	return score;
}

void AIController::determineActions() {
	auto well = this->well.lock();
	Point2I dimensions = well->getDimensions();

	int score = std::numeric_limits<int>().min();
	int rotations = 0;
	int distance = 0;
	int dropDistance = 0;
	bool hardMove = false;
	PointList2I finalShape;

	for (int r = 0; r < 4; r++) {
		bool leftHit = false;
		bool rightHit = false;
		for (int h = 1; h <= dimensions.x + 1; h++) {
			if ((h % 2 == 0 && rightHit) || (h % 2 == 1 && leftHit))
				continue;

			int hdistance = (h % 2 == 0 ? h / 2 : -h / 2);
			int ydistance = (int)std::floor(
				liveTetromino->getDropPosition() +
				liveTetromino->getDropSpeed() * std::abs((r + hdistance) * ai.actionSpeed)
			);
			PointList2I translatedShape = liveTetromino->getTranslatedRotatedShape(r, hdistance, ydistance);

			if (well->isColliding(translatedShape)) {
				if (h % 2 == 0 || h == 1)
					rightHit = true;
				if (h % 2 == 1)
					leftHit = true;
				continue;
			}
			// Check if we can do a hard move
			bool hard = false;
			if (hdistance != 0) {
				hard = well->isColliding(translatedShape.translated(GMath::sign(hdistance), 0));
			}

			int drop = well->getLandingPosition(translatedShape);

			int newScore = calculateScore(translatedShape);
			if (newScore > score) {
				score = newScore;
				rotations = r;
				distance = hdistance;
				dropDistance = drop;
				hardMove = hard;
			}
		}
	}

	switch (rotations) {
	case 1: actions.push(AIActions::Rotate90); break;
	case 2: actions.push(AIActions::Rotate180); break;
	case 3: actions.push(AIActions::Rotate270); break;
	}

	if (distance > 0) {
		if (hardMove)
			actions.push(AIActions::MoveHardRight);
		for (int i = 0; i < distance && !hardMove; i++)
			actions.push(AIActions::MoveRight);
	}
	else if (distance < 0) {
		if (hardMove)
			actions.push(AIActions::MoveHardLeft);
		for (int i = 0; i > distance && !hardMove; i--)
			actions.push(AIActions::MoveLeft);
	}
	actions.push(AIActions::HardDrop);
	
}
void AIController::doNextAction() {
	AIActions action = actions.front();
	actions.pop();
	switch (action) {
	case AIActions::Rotate90:		liveTetromino->rotate90(); break;
	case AIActions::Rotate180:		liveTetromino->rotate180(); break;
	case AIActions::Rotate270:		liveTetromino->rotate270(); break;
	case AIActions::MoveLeft:		liveTetromino->moveLeft(false); break;
	case AIActions::MoveHardLeft:	liveTetromino->moveLeft(true); break;
	case AIActions::MoveRight:		liveTetromino->moveRight(false); break;
	case AIActions::MoveHardRight:	liveTetromino->moveRight(true); break;
	case AIActions::SoftDrop:		liveTetromino->setDropMode(DropModes::SoftDrop); break;
	case AIActions::HardDrop:		liveTetromino->setDropMode(DropModes::HardDrop); break;
	case AIActions::Wait:			/* Do Nothing */ break;
	}
}

void AIController::update(const Keyboard& keyboard) {

	if (thinkTimer > 0) {
		thinkTimer--;
		return;
	}

	if (actions.empty()) {
		determineActions();
		// The first action starts right after thinking is done
		actionTimer = 0;
	}

	if (actionTimer > 0) {
		actionTimer--;
	}
	// The while is here if actionSpeed is set to zero
	while (actionTimer == 0 && !actions.empty()) {
		doNextAction();
		// Restart the action timer
		actionTimer = ai.actionSpeed;
	}

	if (actions.empty()) {
		// Restart the think timer for the next tetromino
		thinkTimer = ai.thinkSpeed;
	}
}
void AIController::setNextTetromino(TetrominoSPtr tetromino) {
	TetrisController::setNextTetromino(tetromino);
}

#pragma endregion
//=================================================================|
