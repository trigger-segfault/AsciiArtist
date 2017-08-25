/*=================================================================|
|* File:				Rectangle.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Rectangle.h"

#undef max
#undef min

using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region struct Rectangle2I
//========== CONSTANTS ===========
const Rectangle2I Rectangle2I::ZERO = Rectangle2I(0, 0, 0, 0);

//========= CONSTRUCTORS =========
Rectangle2I::Rectangle2I()
	: point(Point2I::ZERO), size(Point2I::ZERO) {}
Rectangle2I::Rectangle2I(int width, int height)
	: point(Point2I::ZERO), size(width, height) {}
Rectangle2I::Rectangle2I(Point2I size)
	: point(Point2I::ZERO), size(size) {}
Rectangle2I::Rectangle2I(int x, int y, int width, int height, bool asEnds)
	: point(x, y), size(width - (asEnds ? x - 1 : 0), height - (asEnds ? y - 1 : 0)) {}
Rectangle2I::Rectangle2I(Point2I point, int width, int height, bool asEnds)
	: point(point), size(width - (asEnds ? point.x - 1 : 0), height - (asEnds ? point.y - 1 : 0)) {}
Rectangle2I::Rectangle2I(int x, int y, Point2I size, bool asEnds)
	: point(x, y), size(size.x - (asEnds ? x - 1 : 0), size.y - (asEnds ? y - 1 : 0)) {}
Rectangle2I::Rectangle2I(Point2I point, Point2I size, bool asEnds)
	: point(point), size(size.x - (asEnds ? point.x - 1 : 0), size.y - (asEnds ? point.y - 1 : 0)) {}
Rectangle2I::Rectangle2I(const Rectangle2I& rect)
	: point(rect.point), size(rect.size) {}
Rectangle2I::Rectangle2I(const SMALL_RECT& rect)
	: point(rect.Left, rect.Top), size(rect.Right - rect.Left + 1, rect.Bottom - rect.Top + 1) {}
Rectangle2I::Rectangle2I(const RECT& rect)
	: point(rect.left, rect.top), size(rect.right - rect.left + 1, rect.bottom - rect.top + 1) {}

//========== OPERATORS ===========

// Binary Assignment
Rectangle2I& Rectangle2I::operator+=(Point2I rhs) { point += rhs; return *this; }
Rectangle2I& Rectangle2I::operator-=(Point2I rhs) { point -= rhs; return *this; }

//Rectangle2I& Rectangle2I::operator+=(Thickness rhs) { point += rhs.topLeft(); size -= rhs.bottomRight(); return *this; }
//Rectangle2I& Rectangle2I::operator-=(Thickness rhs) { point -= rhs.topLeft(); size += rhs.bottomRight(); return *this; }

// Binary
Rectangle2I Rectangle2I::operator+(Point2I rhs) const { return Rectangle2I(point + rhs, size); }
Rectangle2I Rectangle2I::operator-(Point2I rhs) const { return Rectangle2I(point - rhs, size); }

//Rectangle2I Rectangle2I::operator+(Thickness rhs) const { return Rectangle2I(point + rhs.topLeft(), size - rhs.bottomRight()); }
//Rectangle2I Rectangle2I::operator-(Thickness rhs) const { return Rectangle2I(point - rhs.topLeft(), size + rhs.bottomRight()); }

// Boolean
bool Rectangle2I::operator==(Rectangle2I rhs) const { return (point == rhs.point && size == rhs.size); }
bool Rectangle2I::operator!=(Rectangle2I rhs) const { return (point != rhs.point || size != rhs.size); }

//========== CONVERSION ==========
Rectangle2I::operator Rectangle2F() const {
	return Rectangle2F((Point2F)point, (Point2F)size);
}
Rectangle2I::operator SMALL_RECT() const {
	return SMALL_RECT{ (short)left(), (short)top(), (short)right(), (short)bottom() };
}
Rectangle2I::operator RECT() const {
	return RECT{ left(), top(), right(), bottom() };
}

//========== PROPERTIES ==========
int Rectangle2I::x() const {
	return point.x;
}
void Rectangle2I::x(int x) {
	point.x = x;
}
int Rectangle2I::y() const {
	return point.y;
}
void Rectangle2I::y(int y) {
	point.y = y;
}
int Rectangle2I::width() const {
	return size.x;
}
void Rectangle2I::width(int width) {
	size.x = width;
}
int Rectangle2I::height() const {
	return size.y;
}
void Rectangle2I::height(int height) {
	size.y = height;
}
int Rectangle2I::absWidth() const {
	return std::abs(size.x);
}
int Rectangle2I::absHeight() const {
	return std::abs(size.y);
}

int Rectangle2I::left() const {
	return point.x;
}
void Rectangle2I::left(int left) {
	size.x -= left - point.x;
	point.x = left;
}
int Rectangle2I::top() const {
	return point.y;
}
void Rectangle2I::top(int top) {
	size.y -= top - point.y;
	point.y = top;
}
int Rectangle2I::right() const {
	return point.x + size.x - 1;
}
void Rectangle2I::right(int right) {
	size.x = right + 1 - point.x;
}
int Rectangle2I::bottom() const {
	return point.y + size.y - 1;
}
void Rectangle2I::bottom(int bottom) {
	size.y = bottom + 1 - point.y;
}
Point2I Rectangle2I::topLeft() const {
	return point;
}
void Rectangle2I::topLeft(Point2I topLeft) {
	size -= topLeft - point;
	point = topLeft;
}
Point2I Rectangle2I::topRight() const {
	return point + Point2I(size.x - 1, 0);
}
void Rectangle2I::topRight(Point2I topRight) {
	size.x = topRight.x + 1 - point.x;
	size.y -= topRight.y - point.y;
	point.y = topRight.y;
}
Point2I Rectangle2I::bottomLeft() const {
	return point + Point2I(0, size.y - 1);
}
void Rectangle2I::bottomLeft(Point2I bottomLeft) {
	size.x -= bottomLeft.x - point.x;
	point.x = bottomLeft.x;
	size.y = bottomLeft.y + 1 - point.y;
}
Point2I Rectangle2I::bottomRight() const {
	return point + size - Point2I::ONE;
}
void Rectangle2I::bottomRight(Point2I bottomRight) {
	size = bottomRight + Point2I::ONE - point;
}


int Rectangle2I::absLeft() const {
	return point.x + (size.x < 0 ? size.x + 1 : 0);
}
int Rectangle2I::absTop() const {
	return point.y + (size.y < 0 ? size.y + 1 : 0);
}
int Rectangle2I::absRight() const {
	return point.x + (size.x < 0 ? 0 : size.x - 1);
}
int Rectangle2I::absBottom() const {
	return point.y + (size.y < 0 ? 0 : size.y - 1);
}
Point2I Rectangle2I::absTopLeft() const {
	return Point2I(absLeft(), absTop());
}
Point2I Rectangle2I::absTopRight() const {
	return Point2I(absRight(), absTop());
}
Point2I Rectangle2I::absBottomLeft() const {
	return Point2I(absLeft(), absBottom());
}
Point2I Rectangle2I::absBottomRight() const {
	return Point2I(absRight(), absBottom());
}

Point2I Rectangle2I::absSize() const {
	return Point2I(absWidth(), absHeight());
}
Rectangle2I Rectangle2I::absRect() const {
	return Rectangle2I(absTopLeft(), absSize());
}

int Rectangle2I::area() const {
	return std::abs(size.x * size.y);
}
int Rectangle2I::perimeter() const {
	return (std::abs(size.x) + std::abs(size.y)) * 2;
}
Point2I Rectangle2I::center() const {
	return topLeft() + absSize() / 2;
}

Rectangle2I Rectangle2I::pixelBounds() const {
	Point2I offset = Point2I(size.x >= 0 ? 1 : -1, size.y >= 0 ? 1 : -1);
	return Rectangle2I(point, size + offset).absRect();
}

//======== TRANSFORMATION ========
// Accessor
Rectangle2I Rectangle2I::translated(Point2I distance) const {
	return Rectangle2I(*this).translate(distance);
}
Rectangle2I Rectangle2I::scaled(Point2F scale) const {
	return Rectangle2I(*this).scale(scale);
}
Rectangle2I Rectangle2I::inflated(Thickness amount) const {
	return Rectangle2I(point - amount.topLeft(), size + amount.topLeft() + amount.bottomRight());
}

// Mutator
Rectangle2I& Rectangle2I::translate(Point2I distance) {
	point += distance;
	return *this;
}
Rectangle2I& Rectangle2I::scale(Point2F scale) {
	size = (Point2I)((Point2F)size * scale);
	return *this;
}
Rectangle2I& Rectangle2I::inflate(Thickness amount) {
	point -= amount.topLeft();
	size += amount.topLeft() + amount.bottomRight();
	return *this;
}

//========== COLLISION ===========
bool Rectangle2I::containsPoint(Point2I point) const {
	return (point >= topLeft() && point <= bottomRight());
}
bool Rectangle2I::containsRect(Rectangle2I rect) const {
	return (rect.topLeft() >= topLeft() && rect.bottomRight() <= bottomRight());
}

Rectangle2I& Rectangle2I::intersect(Rectangle2I rect) {
	Rectangle2I newRect = Rectangle2I(
		std::max(left(), rect.left()), std::max(top(), rect.top()),
		std::min(right(), rect.right()), std::min(bottom(), rect.bottom()),
		true
	);
	if (newRect.size >= Point2I::ZERO)
		*this = newRect;
	else
		*this = Rectangle2I::ZERO;
	return *this;
}
Rectangle2I Rectangle2I::intersection(Rectangle2I rect) const {
	Rectangle2I newRect = Rectangle2I(
		std::max(left(), rect.left()), std::max(top(), rect.top()),
		std::min(right(), rect.right()), std::min(bottom(), rect.bottom()),
		true
	);
	if (newRect.size >= Point2I::ZERO)
		return newRect;
	return Rectangle2I::ZERO;
}
Rectangle2I Rectangle2I::unionRect(Rectangle2I rect) const {
	return Rectangle2I(
		std::min(left(), rect.left()), std::min(top(), rect.top()),
		std::max(right(), rect.right()), std::max(bottom(), rect.bottom()),
		true
	);
}

#pragma endregion

#pragma region struct Rectangle2F
//========== CONSTANTS ===========
const Rectangle2F Rectangle2F::ZERO = Rectangle2F(0.0f, 0.0f, 0.0f, 0.0f);

//========= CONSTRUCTORS =========
Rectangle2F::Rectangle2F()
	: point(Point2F::ZERO), size(Point2F::ZERO) {}
Rectangle2F::Rectangle2F(float width, float height)
	: point(Point2F::ZERO), size(width, height) {}
Rectangle2F::Rectangle2F(Point2F size)
	: point(Point2F::ZERO), size(size) {}
Rectangle2F::Rectangle2F(float x, float y, float width, float height)
	: point(x, y), size(width, height) {}
Rectangle2F::Rectangle2F(Point2F point, float width, float height)
	: point(point), size(width, height) {}
Rectangle2F::Rectangle2F(float x, float y, Point2F size)
	: point(x, y), size(size) {}
Rectangle2F::Rectangle2F(Point2F point, Point2F size)
	: point(point), size(size) {}
Rectangle2F::Rectangle2F(const Rectangle2F& rect)
	: point(rect.point), size(rect.size) {}
Rectangle2F::Rectangle2F(const Rectangle2I& rect)
	: point(rect.point), size(rect.size) {}
Rectangle2F::Rectangle2F(const SMALL_RECT& rect)
	: point((float)rect.Left, (float)rect.Top), size((float)(rect.Right - rect.Left + 1), (float)(rect.Bottom - rect.Top + 1)) {}
Rectangle2F::Rectangle2F(const RECT& rect)
	: point((float)rect.left, (float)rect.top), size((float)(rect.right - rect.left + 1), (float)(rect.bottom - rect.top + 1)) {}

//========== OPERATORS ===========
// Binary Assignment
Rectangle2F& Rectangle2F::operator+=(Rectangle2F rhs) { point += rhs.point; size += rhs.size; return *this; }
Rectangle2F& Rectangle2F::operator-=(Rectangle2F rhs) { point -= rhs.point; size -= rhs.size; return *this; }
Rectangle2F& Rectangle2F::operator*=(Rectangle2F rhs) { point *= rhs.point; size *= rhs.size; return *this; }
Rectangle2F& Rectangle2F::operator/=(Rectangle2F rhs) { point /= rhs.point; size /= rhs.size; return *this; }

// Binary
Rectangle2F Rectangle2F::operator+(Rectangle2F rhs) const { return Rectangle2F(point + rhs.point, size + rhs.size); }
Rectangle2F Rectangle2F::operator-(Rectangle2F rhs) const { return Rectangle2F(point - rhs.point, size - rhs.size); }
Rectangle2F Rectangle2F::operator*(Rectangle2F rhs) const { return Rectangle2F(point * rhs.point, size * rhs.size); }
Rectangle2F Rectangle2F::operator/(Rectangle2F rhs) const { return Rectangle2F(point / rhs.point, size / rhs.size); }

// Boolean
bool Rectangle2F::operator==(Rectangle2F rhs) const { return (point == rhs.point && size == rhs.size); }
bool Rectangle2F::operator!=(Rectangle2F rhs) const { return (point != rhs.point || size != rhs.size); }

//========== CONVERSION ==========
Rectangle2F::operator Rectangle2I() const {
	return Rectangle2I((Point2I)point, (Point2I)size);
}
Rectangle2F::operator SMALL_RECT() const {
	return SMALL_RECT{ (short)left(), (short)top(), (short)right(), (short)bottom() };
}
Rectangle2F::operator RECT() const {
	return RECT{ (int)left(), (int)top(), (int)right(), (int)bottom() };
}

//========== PROPERTIES ==========
float Rectangle2F::x() const {
	return point.x;
}
float Rectangle2F::y() const {
	return point.y;
}
float Rectangle2F::width() const {
	return size.x;
}
float Rectangle2F::height() const {
	return size.y;
}
float Rectangle2F::absWidth() const {
	return std::abs(size.x);
}
float Rectangle2F::absHeight() const {
	return std::abs(size.y);
}

float Rectangle2F::left() const {
	if (size.x < 0)
		return point.x + size.x;
	else
		return point.x;
}
float Rectangle2F::top() const {
	if (size.y < 0)
		return point.y + size.y;
	else
		return point.y;
}
float Rectangle2F::right() const {
	return left() + absWidth();
}
float Rectangle2F::bottom() const {
	return top() + absHeight();
}
Point2F Rectangle2F::topLeft() const {
	return Point2F(left(), top());
}
Point2F Rectangle2F::topRight() const {
	return Point2F(right(), top());
}
Point2F Rectangle2F::bottomLeft() const {
	return Point2F(left(), bottom());
}
Point2F Rectangle2F::bottomRight() const {
	return Point2F(right(), bottom());
}

Point2F Rectangle2F::absSize() const {
	return Point2F(absWidth(), absHeight());
}
Rectangle2F Rectangle2F::absRect() const {
	return Rectangle2F(topLeft(), absSize());
}

float Rectangle2F::area() const {
	return std::abs(size.x * size.y);
}
float Rectangle2F::perimeter() const {
	return (std::abs(size.x) + std::abs(size.y)) * 2.0f;
}
Point2F Rectangle2F::center() const {
	return topLeft() + absSize() / 2.0f;
}

//======== TRANSFORMATION ========
// Accessor
Rectangle2F Rectangle2F::translated(Point2F distance) const {
	return Rectangle2F(*this).translate(distance);
}
Rectangle2F Rectangle2F::translated(float x, float y) const {
	return translated(Point2F(x, y));
}
Rectangle2F Rectangle2F::scaled(Point2F scale) const {
	return Rectangle2F(*this).scale(scale);
}
Rectangle2F Rectangle2F::scaled(float x, float y) const {
	return scaled(Point2F(x, y));
}

// Mutator
/* Translates and returns the rectanglet. */
Rectangle2F& Rectangle2F::translate(Point2F distance) {
	point += distance;
	return *this;
}
/* Translates and returns the rectangle. */
Rectangle2F& Rectangle2F::translate(float x, float y) {
	return translate(Point2F(x, y));
}
/* Scales and returns the rectangle. */
Rectangle2F& Rectangle2F::scale(Point2F scale) {
	size *= scale;
	return *this;
}
/* Scales and returns the rectangle. */
Rectangle2F& Rectangle2F::scale(float x, float y) {
	return scale(Point2F(x, y));
}

//========== COLLISION ===========
bool Rectangle2F::containsPoint(Point2F point) const {
	return (point >= topLeft() && point <= bottomRight());
}
bool Rectangle2F::containsRect(Rectangle2F rect) const {
	return (rect.topLeft() >= topLeft() && rect.bottomRight() <= bottomRight());
}

#pragma endregion
//=================================================================|