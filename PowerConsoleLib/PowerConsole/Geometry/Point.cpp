/*=================================================================|
|* File:				Point.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Point.h"
#define _USE_MATH_DEFINES
#include <math.h>

#undef max
#undef min

using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/
// struct Point2I
//========== CONSTANTS ===========
const Point2I Point2I::ZERO = Point2I(0, 0);
const Point2I Point2I::ONE = Point2I(1, 1);

//========= CONSTRUCTORS =========
Point2I::Point2I()
	: x(0), y(0) {}
Point2I::Point2I(int x, int y)
	: x(x), y(y) {}
Point2I::Point2I(int xy)
	: x(xy), y(xy) {}
Point2I::Point2I(const Point2I& point)
	: x(point.x), y(point.y) {}
Point2I::Point2I(const COORD& point)
	: x(point.X), y(point.Y) {}
Point2I::Point2I(const POINT& point)
	: x(point.x), y(point.y) {}
Point2I::Point2I(const SMALL_RECT& rect, bool asSize)
	: x(asSize ? rect.Right - rect.Left + 1 : rect.Left), y(asSize ? rect.Bottom - rect.Top + 1 : rect.Top) {}
Point2I::Point2I(const RECT& rect, bool asSize)
	: x(asSize ? rect.right - rect.left + 1 : rect.left), y(asSize ? rect.bottom - rect.top + 1 : rect.top) {}

//=========== GENERAL ============
std::string Point2I::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

//========== OPERATORS ===========
// Unary Assignment
Point2I& Point2I::operator++() { x++; y++; return *this; }
Point2I& Point2I::operator--() { x--; y--; return *this; }

// Binary Assignment
Point2I& Point2I::operator+=(int rhs) { x += rhs; y += rhs; return *this; }
Point2I& Point2I::operator+=(Point2I rhs) { x += rhs.x; y += rhs.y; return *this; }
Point2I& Point2I::operator-=(int rhs) { x -= rhs; y -= rhs; return *this; }
Point2I& Point2I::operator-=(Point2I rhs) { x -= rhs.x; y -= rhs.y; return *this; }
Point2I& Point2I::operator*=(int rhs) { x *= rhs; y *= rhs; return *this; }
Point2I& Point2I::operator*=(Point2I rhs) { x *= rhs.x; y *= rhs.y; return *this; }
Point2I& Point2I::operator/=(int rhs) { x /= rhs; y /= rhs; return *this; }
Point2I& Point2I::operator/=(Point2I rhs) { x /= rhs.x; y /= rhs.y; return *this; }
Point2I& Point2I::operator%=(int rhs) { x %= rhs; y %= rhs; return *this; }
Point2I& Point2I::operator%=(Point2I rhs) { x %= rhs.x; y %= rhs.y; return *this; }

// Unary
Point2I Point2I::operator+() const { return Point2I(+x, +y); }
Point2I Point2I::operator-() const { return Point2I(-x, -y); }

// Binary
Point2I Point2I::operator+(int rhs) const { return Point2I(x + rhs, y + rhs); }
Point2I Point2I::operator+(Point2I rhs) const { return Point2I(x + rhs.x, y + rhs.y); }
Point2I Point2I::operator-(int rhs) const { return Point2I(x - rhs, y - rhs); }
Point2I Point2I::operator-(Point2I rhs) const { return Point2I(x - rhs.x, y - rhs.y); }
Point2I Point2I::operator*(int rhs) const { return Point2I(x * rhs, y * rhs); }
Point2I Point2I::operator*(Point2I rhs) const { return Point2I(x * rhs.x, y * rhs.y); }
Point2I Point2I::operator/(int rhs) const { return Point2I(x / rhs, y / rhs); }
Point2I Point2I::operator/(Point2I rhs) const { return Point2I(x / rhs.x, y / rhs.y); }
Point2I Point2I::operator%(int rhs) const { return Point2I(x % rhs, y % rhs); }
Point2I Point2I::operator%(Point2I rhs) const { return Point2I(x % rhs.x, y % rhs.y); }

// Boolean
bool Point2I::operator==(int rhs) const { return (x == rhs && y == rhs); }
bool Point2I::operator==(Point2I rhs) const { return (x == rhs.x && y == rhs.y); }
bool Point2I::operator!=(int rhs) const { return (x != rhs || y != rhs); }
bool Point2I::operator!=(Point2I rhs) const { return (x != rhs.x || y != rhs.y); }
bool Point2I::operator>(int rhs) const { return (x > rhs && y > rhs); }
bool Point2I::operator>(Point2I rhs) const { return (x > rhs.x && y > rhs.y); }
bool Point2I::operator<(int rhs) const { return (x < rhs && y < rhs); }
bool Point2I::operator<(Point2I rhs) const { return (x < rhs.x && y < rhs.y); }
bool Point2I::operator>=(int rhs) const { return (x >= rhs && y >= rhs); }
bool Point2I::operator>=(Point2I rhs) const { return (x >= rhs.x && y >= rhs.y); }
bool Point2I::operator<=(int rhs) const { return (x <= rhs && y <= rhs); }
bool Point2I::operator<=(Point2I rhs) const { return (x <= rhs.x && y <= rhs.y); }

// Index
int& Point2I::operator[](int index) {
	if (index == 0)
		return x;
	else if (index == 1)
		return y;
	else
		throw std::out_of_range("Invalid index in 'Point2I::operator[]'");
}

//========== CONVERSION ==========
Point2I::operator Point2F() const {
	return Point2F((float)x, (float)y);
}
Point2I::operator COORD() const {
	return COORD{ (short)x, (short)y };
}
Point2I::operator POINT() const {
	return POINT{ x, y };
}

//========== PROPERTIES ==========
float Point2I::length() const {
	return std::sqrt(float(x*x + y*y));
}
float Point2I::direction() const {
	return std::atan2((float)y, (float)x) * 180.0f / (float)M_PI;
}
bool Point2I::isZero() const {
	return (x == 0 && y == 0);
}
Point2I Point2I::perpendicular() const {
	return Point2I(-y, x);
}
int Point2I::area() const {
	return x * y;
}

// struct Point2F
//========== CONSTANTS ===========
const Point2F Point2F::ZERO = Point2F(0.0f, 0.0f);
const Point2F Point2F::ONE = Point2F(1.0f, 1.0f);

//========= CONSTRUCTORS =========
Point2F::Point2F()
	: x(0.0f), y(0.0f) {}
Point2F::Point2F(float x, float y)
	: x(x), y(y) {}
Point2F::Point2F(float xy)
	: x(xy), y(xy) {}
Point2F::Point2F(const Point2F& point)
	: x(point.x), y(point.y) {}
/*Point2F::Point2F(const Point2I& point)
	: x((float)point.x), y((float)point.y) {}*/
Point2F::Point2F(const COORD& point)
	: x((float)point.X), y((float)point.Y) {}
Point2F::Point2F(const POINT& point)
	: x((float)point.x), y((float)point.y) {}

//=========== GENERAL ============
std::string Point2F::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

//========== OPERATORS ===========
// Unary Assignment
Point2F& Point2F::operator++() { x++; y++; return *this; }
Point2F& Point2F::operator--() { x--; y--; return *this; }

// Binary Assignment
Point2F& Point2F::operator+=(float rhs) { x += rhs; y += rhs; return *this; }
Point2F& Point2F::operator+=(Point2F rhs) { x += rhs.x; y += rhs.y; return *this; }
Point2F& Point2F::operator-=(float rhs) { x -= rhs; y -= rhs; return *this; }
Point2F& Point2F::operator-=(Point2F rhs) { x -= rhs.x; y -= rhs.y; return *this; }
Point2F& Point2F::operator*=(float rhs) { x *= rhs; y *= rhs; return *this; }
Point2F& Point2F::operator*=(Point2F rhs) { x *= rhs.x; y *= rhs.y; return *this; }
Point2F& Point2F::operator/=(float rhs) { x /= rhs; y /= rhs; return *this; }
Point2F& Point2F::operator/=(Point2F rhs) { x /= rhs.x; y /= rhs.y; return *this; }
Point2F& Point2F::operator%=(float rhs) { x = std::fmod(x, rhs); y = std::fmod(y, rhs); return *this; }
Point2F& Point2F::operator%=(Point2F rhs) { x = std::fmod(x, rhs.x); y = std::fmod(y, rhs.y); return *this; }

// Unary
Point2F Point2F::operator+() const { return Point2F(+x, +y); }
Point2F Point2F::operator-() const { return Point2F(-x, -y); }

// Binary
Point2F Point2F::operator+(float rhs) const { return Point2F(x + rhs, y + rhs); }
Point2F Point2F::operator+(Point2F rhs) const { return Point2F(x + rhs.x, y + rhs.y); }
Point2F Point2F::operator-(float rhs) const { return Point2F(x - rhs, y - rhs); }
Point2F Point2F::operator-(Point2F rhs) const { return Point2F(x - rhs.x, y - rhs.y); }
Point2F Point2F::operator*(float rhs) const { return Point2F(x * rhs, y * rhs); }
Point2F Point2F::operator*(Point2F rhs) const { return Point2F(x * rhs.x, y * rhs.y); }
Point2F Point2F::operator/(float rhs) const { return Point2F(x / rhs, y / rhs); }
Point2F Point2F::operator/(Point2F rhs) const { return Point2F(x / rhs.x, y / rhs.y); }
Point2F Point2F::operator%(float rhs) const { return Point2F(std::fmod(x, rhs), std::fmod(y, rhs)); }
Point2F Point2F::operator%(Point2F rhs) const { return Point2F(std::fmod(x, rhs.x), std::fmod(y, rhs.y)); }

// Boolean
bool Point2F::operator==(float rhs) const { return (x == rhs && y == rhs); }
bool Point2F::operator==(Point2F rhs) const { return (x == rhs.x && y == rhs.y); }
bool Point2F::operator!=(float rhs) const { return (x != rhs || y != rhs); }
bool Point2F::operator!=(Point2F rhs) const { return (x != rhs.x || y != rhs.y); }
bool Point2F::operator>(float rhs) const { return (x > rhs && y > rhs); }
bool Point2F::operator>(Point2F rhs) const { return (x > rhs.x && y > rhs.y); }
bool Point2F::operator<(float rhs) const { return (x < rhs && y < rhs); }
bool Point2F::operator<(Point2F rhs) const { return (x < rhs.x && y < rhs.y); }
bool Point2F::operator>=(float rhs) const { return (x >= rhs && y >= rhs); }
bool Point2F::operator>=(Point2F rhs) const { return (x >= rhs.x && y >= rhs.y); }
bool Point2F::operator<=(float rhs) const { return (x <= rhs && y <= rhs); }
bool Point2F::operator<=(Point2F rhs) const { return (x <= rhs.x && y <= rhs.y); }

// Index
float& Point2F::operator[](int index) {
	if (index == 0)
		return x;
	else if (index == 1)
		return y;
	else
		throw std::out_of_range("Invalid index in 'Point2F::operator[](int index)'");
}

//========== CONVERSION ==========
Point2F::operator Point2I() const {
	return Point2I((int)x, (int)y);
}
Point2F::operator COORD() const {
	return COORD{ (short)x, (short)y };
}
Point2F::operator POINT() const {
	return POINT{ (int)x, (int)y };
}

//========== PROPERTIES ==========
float Point2F::length() const {
	return std::sqrt(x*x + y*y);
}
float Point2F::direction() const {
	return std::atan2(y, x) * 180.0f / (float)M_PI;
}
bool Point2F::isZero() const {
	return (x == 0.0f && y == 0.0f);
}
Point2F Point2F::perpendicular() const {
	return Point2F(-y, x);
}

//=================================================================|