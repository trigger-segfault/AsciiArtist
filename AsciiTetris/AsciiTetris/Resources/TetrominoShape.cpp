/*=================================================================|
|* File:				TetrominoShape.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TetrominoShape.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace AsciiTetris::Resources;
using namespace PowerConsole::Drawing;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

TetrominoShape::TetrominoShape(const std::string& name, unsigned char color, bool warning, Point2F origin, const std::string& data) {
	this->name = name;
	this->color = toColor(darkenFColor(color), color);
	this->warning = warning;
	this->placeCount = 0;
	std::vector<Point2I> blockPositions;
	Point2I scanPoint = Point2I::ZERO;
	for (int i = 0; i < (int)data.length(); i++) {
		if (data[i] == '|') {
			if (i + 1 < (int)data.length() && data[i + 1] == '|') {
				scanPoint.x = 0;
				scanPoint.y++;
			}
		}
		else if (data[i] == '`' || data[i] == '#') {
			if (data[i] == '#')
				blockPositions.push_back(scanPoint);
			scanPoint.x++;
		}
		else {
			throw std::exception("Invalid character in TetrominoData data string.");
		}
	}
	this->blocks = PointList2I(blockPositions, origin);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

const PointList2I& TetrominoShape::getBlocks() const {
	return blocks;
}
Point2I TetrominoShape::getBlockAt(int index) const {
	if (index < 0 || index >= blocks.count())
		throw std::exception("Invalid index in 'TetrominoData::getBlockAt(int index)'.");
	return blocks[index];
}
int TetrominoShape::getNumBlocks() const {
	return blocks.count();
}
Point2F TetrominoShape::getOrigin() const {
	return blocks.origin();
}
Point2F TetrominoShape::getCenter() const {
	return blocks.center();
}
Rectangle2I TetrominoShape::getBounds() const {
	return blocks.bounds();
}
unsigned char TetrominoShape::getColor() const {
	return color;
}
std::string TetrominoShape::getName() const {
	return name;
}
bool TetrominoShape::hasWarning() const {
	return warning;
}
int TetrominoShape::getPlaceCount() const {
	return placeCount;
}
void TetrominoShape::incrementPlaceCount() {
	placeCount++;
}
void TetrominoShape::clearPlaceCount() {
	placeCount = 0;
}

#pragma endregion
//=================================================================|
