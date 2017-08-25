/*=================================================================|
|* File:				TetrominoBlock.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TetrominoBlock.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Drawing/Graphics.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace AsciiTetris::Game::Tetrominos;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

TetrominoBlock::TetrominoBlock(unsigned char color)
 :	color(color) {}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

unsigned char TetrominoBlock::getColor() {
	return color;
}

#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void TetrominoBlock::draw(GraphicsSPtr g, Point2F position, Point2I blockSize, unsigned char color, Point2I offset) {
	//color = swapColor(color);
	if (blockSize > Point2I::ONE) {
		g->setPixel(Rectangle2I(offset + (Point2I)GMath::floor(position * (Point2F)blockSize) + 1, blockSize - 2), Pixel(' ', color));
		g->drawBorder(Rectangle2I(offset + (Point2I)GMath::floor(position * (Point2F)blockSize), blockSize), Pixel(' ', color), true, DisabledBorders(true));
	}
	else {
		g->setPixel(Rectangle2I(offset + (Point2I)(position * (Point2F)blockSize), blockSize), Pixel(254, color));
	}
}
void TetrominoBlock::drawGhost(GraphicsSPtr g, Point2I position, Point2I blockSize) {
	unsigned char color = toColor(BLACK, DARK_GRAY);
	//color = swapColor(color);
	draw(g, position, blockSize, color);
}

#pragma endregion
//=================================================================|
