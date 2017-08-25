/*=================================================================|
|* File:				Tetromino.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Tetromino.h"
#include <AsciiTetris/Game/TetrisWell.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace AsciiTetris::Game::Tetrominos;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Tetromino::Tetromino(TetrisWellWPtr well, TetrominoShapeSPtr shape)
 :	well(well),
	baseShape(shape),
	currentShape(shape->getBlocks()),
	
	wellPosition(Point2I::ZERO),
	dropPosition(0.0f), 
	dropSpeed(2.0f / 60.0f),
	
	dropMode(DropModes::Normal),
	landed(false),

	dropScore(0),
	dropDistance(0.0f) {}

#pragma endregion
//=========== CONTROL ============
#pragma region Control

void Tetromino::activate(float dropSpeed) {
	auto well = this->well.lock();

	this->wellPosition = well->getStartPosition() - (Point2I)(baseShape->getOrigin() + 0.5f) - Point2I(0, 1);
	this->dropSpeed = dropSpeed;
}
void Tetromino::update() {
	if (landed)
		return;

	auto well = this->well.lock();

	if (dropMode == DropModes::HardDrop) {
		int distance = well->getLandingPosition(shared_from_this());
		wellPosition.y += distance;
		addHardDropScore((float)distance + 1.0f - GMath::fmod(dropPosition, 1.0f));
		dropPosition = 0.0f;
		landed = true;
		return;
	}
	else if (dropMode == DropModes::SoftDrop) {
		float distance = std::max(dropSpeed, 0.25f);
		dropPosition += distance;
		addSoftDropScore(distance);
	}
	else {
		dropPosition += dropSpeed;
		dropScore = 0;
	}

	if (dropPosition >= 1.0f) {
		int totalRowsDropped = (int)GMath::floor(dropPosition);

		PointList2I translatedShape = getTranslatedShape();
		for (int i = 0; i < totalRowsDropped; i++) {
			dropPosition--;
			if (well->isColliding(translatedShape.translate(0, 1))) {
				landed = true;
				break;
			}
			else {
				wellPosition.y++;
			}
		}
	}
}
void Tetromino::moveLeft(bool hardLeft) {
	auto well = this->well.lock();
	//PointList2I translatedShape = getTranslatedShape();

	if (hardLeft) {
		move(-well->getDimensions().x);
		// Move left until before we hit something.
		/*for (int i = -1; i >= -well->getDimensions().x; i--) {
			if (well->isColliding(translatedShape.translate(-1, 0))) {
				move(i + 1);
				break;
			}
		}*/
	}
	else {//if (!well->isColliding(translatedShape.translate(-1, 0))) {
		move(-1);
	}
}
void Tetromino::moveRight(bool hardRight) {
	auto well = this->well.lock();
	//PointList2I translatedShape = getTranslatedShape();

	if (hardRight) {
		move(well->getDimensions().x);
		// Move right until before we hit something.
		/*for (int i = 1; i <= well->getDimensions().x; i++) {
			if (well->isColliding(translatedShape.translate(1, 0))) {
				move(i - 1);
				break;
			}
		}*/
	}
	else {//if (!well->isColliding(translatedShape.translate(1, 0))) {
		move(1);
	}
}
void Tetromino::rotate90() {
	rotate(1);
}
void Tetromino::rotate180() {
	rotate(2);
}
void Tetromino::rotate270() {
	rotate(3);
}

void Tetromino::setDropMode(DropModes dropMode) {
	this->dropMode = dropMode;
}

void Tetromino::move(int distance) {
	auto well = this->well.lock();
	PointList2I translatedShape = getTranslatedShape();

	int sign = GMath::sign(distance);
	for (int i = 0; i < std::abs(distance); i++) {
		// Move until before we hit something.
		if (well->isColliding(translatedShape.translate(sign, 0))) {
			wellPosition.x += i * sign;
			return;
		}
	}
	wellPosition.x += distance;
}
void Tetromino::rotate(int rotations) {
	auto well = this->well.lock();
	PointList2I translatedShape = getTranslatedRotatedShape(rotations);

	// Check if we can rotate. If not, see if we can shift left or right one to rotate.
	if (!well->isColliding(translatedShape)) {
		currentShape.rotate(rotations);
	}
	else if (!well->isColliding(translatedShape.translate(1, 0))) {
		currentShape.rotate(rotations);
		move(1);
	}
	else if (!well->isColliding(translatedShape.translate(-2, 0))) { // Yes, -2 is right, we're translating the last translation
		currentShape.rotate(rotations);
		move(-1);
	}
}
void Tetromino::addSoftDropScore(float distance) {
	distance = std::max(0.0f, distance);
	dropDistance += distance;
	int softDistance = (int)GMath::floor(dropDistance);
	if (softDistance > 0) {
		dropDistance -= softDistance;
		dropScore += softDistance;
	}
}
void Tetromino::addHardDropScore(float distance) {
	distance = std::max(0.0f, distance);
	int hardDistance = (int)GMath::floor(distance);
	// Because drop score is only calculated for total rows crossed, add the remainder to soft drop
	if (distance > hardDistance)
		addSoftDropScore(distance - hardDistance);
	dropScore += hardDistance * 2;
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

const TetrominoShapeSPtr& Tetromino::getBaseShape() const {
	return baseShape;
}
const PointList2I& Tetromino::getCurrentShape() const {
	return currentShape;
}
Point2I Tetromino::getWellPosition() const {
	return wellPosition;
}
float Tetromino::getDropPosition() const {
	return dropPosition;
}
float Tetromino::getDropSpeed() const {
	return dropSpeed;
}
PointList2I Tetromino::getTranslatedShape() const {
	return currentShape.translated(wellPosition + Point2I(0, 1));
}
PointList2I Tetromino::getTranslatedShape(int offsetX, int offsetY) const {
	return currentShape.translated(wellPosition + Point2I(offsetX, offsetY + 1));
}
PointList2I Tetromino::getTranslatedRotatedShape(int rotations) const {
	return currentShape.rotated(rotations).translate(wellPosition + Point2I(0, 1));
}
PointList2I Tetromino::getTranslatedRotatedShape(int rotations, int offsetX, int offsetY) const {
	return currentShape.rotated(rotations).translate(wellPosition + Point2I(offsetX, offsetY + 1));
}
bool Tetromino::hasLanded() const {
	return landed;
}
int Tetromino::getDropScore() const {
	return dropScore;
}
unsigned char Tetromino::getColor() const {
	return baseShape->getColor();
}

#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void Tetromino::drawLive(GraphicsSPtr g, Point2I blockSize) {
	PointList2I translatedShape = getTranslatedShape();
	
	for (int i = 0; i < translatedShape.count(); i++) {
		Point2F block = translatedShape[i];
		block.y += dropPosition - (hasLanded() ? 0 : 1.0f - 1.0f / (float)blockSize.y);
		TetrominoBlock::draw(g, block, blockSize, getColor());
	}
}
void Tetromino::drawNext(GraphicsSPtr g, Point2I center, Point2I blockSize) {
	Point2I offset = center - (Point2I)GMath::floor(baseShape->getCenter() * blockSize);
	for (int i = 0; i < currentShape.count(); i++) {
		Point2I block = currentShape[i];
		TetrominoBlock::draw(g, block, blockSize, getColor(), offset);
	}
}

#pragma endregion
//=================================================================|
