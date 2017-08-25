/*=================================================================|
|* File:				Thickness.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Thickness.h"

using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const Thickness Thickness::ZERO = Thickness(0);
const Thickness Thickness::ONE = Thickness(1);

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

Thickness::Thickness()
	: left(0), top(0), right(0), bottom(0) {}
Thickness::Thickness(int uniform)
	: left(uniform), top(uniform), right(uniform), bottom(uniform) {}
Thickness::Thickness(int horizontal, int vertical)
	: left(horizontal), top(vertical), right(horizontal), bottom(vertical) {}
Thickness::Thickness(int left, int top, int right, int bottom)
	: left(left), top(top), right(right), bottom(bottom) {}
Thickness::Thickness(Point2I horizontalVertical)
	: left(horizontalVertical.x), top(horizontalVertical.y), right(horizontalVertical.x), bottom(horizontalVertical.y) {}
Thickness::Thickness(Point2I topLeft, Point2I bottomRight)
	: left(topLeft.x), top(topLeft.y), right(bottomRight.x), bottom(bottomRight.y) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

Thickness Thickness::operator+() const {
	return Thickness(+left, +top, +right, +bottom);
}
Thickness Thickness::operator-() const {
	return Thickness(-left, -top, -right, -bottom);
}

bool Thickness::operator==(int uniform) const {
	return (left == uniform && top == uniform &&
			right == uniform && bottom == uniform);
}
bool Thickness::operator==(Point2I horizontalVertical) const {
	return (left == horizontalVertical.x && top == horizontalVertical.y &&
			right == horizontalVertical.x && bottom == horizontalVertical.y);
}
bool Thickness::operator==(Thickness thickness) const {
	return (left == thickness.left && top == thickness.top &&
			right == thickness.right && bottom == thickness.bottom);
}
bool Thickness::operator!=(int uniform) const {
	return (left != uniform || top != uniform ||
			right != uniform || bottom != uniform);
}
bool Thickness::operator!=(Point2I horizontalVertical) const {
	return (left != horizontalVertical.x || top != horizontalVertical.y ||
			right != horizontalVertical.x || bottom != horizontalVertical.y);
}
bool Thickness::operator!=(Thickness thickness) const {
	return (left != thickness.left || top != thickness.top ||
			right != thickness.right || bottom != thickness.bottom);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

int Thickness::horizontal() const {
	return (left + right);
}
int Thickness::vertical() const {
	return (top + bottom);
}
Point2I Thickness::horizontalVertical() const {
	return Point2I(left + right, top + bottom);
}
Point2I Thickness::topLeft() const {
	return Point2I(left, top);
}
Point2I Thickness::topRight() const {
	return Point2I(right, top);
}
Point2I Thickness::bottomLeft() const {
	return Point2I(left, bottom);
}
Point2I Thickness::bottomRight() const {
	return Point2I(right, bottom);
}

#pragma endregion
//=================================================================|
