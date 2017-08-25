/*=================================================================|
|* File:				Point.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef POINT_H
#define POINT_H

#define NOMINMAX
#include <windows.h>
#include <string>
#include <PowerConsole/Declarations.h>

namespace PowerConsole::Geometry {
//=================================================================|
// PREDEFINITIONS                                                  |
//=================================================================/
struct Point2I;
struct Point2F;

//=================================================================|
// CLASSES														   |
//=================================================================/
/* A two-dimensional integer-based point. */
struct Point2I {

	//========== CONSTANTS ===========

	/* A point positioned at (0, 0). */
	static const Point2I ZERO;
	/* A point positioned at (1, 1). */
	static const Point2I ONE;

	typedef int type;

	//=========== MEMBERS ============

	/* The x coordinate of the point. */
	int x;
	/* The y coordinate of the point. */
	int y;

	//========= CONSTRUCTORS =========

	/* Constructs the default point centered at (0, 0). */
	Point2I();
	/* Constructs a point with the specified x and y coordinates. */
	Point2I(int x, int y);
	/* Constructs a point with the same coordinates for both x and y. */
	Point2I(int xy);
	/* Constructs a copy of the specified point. */
	Point2I(const Point2I& point);
	/* Constructs a copy of the specified point. */
	Point2I(const COORD& point);
	/* Constructs a copy of the specified point. */
	Point2I(const POINT& point);
	/* Constructs a copy of the point or size of the rectangle. */
	Point2I(const SMALL_RECT& rect, bool asSize);
	/* Constructs a copy of the point or size of the rectangle. */
	Point2I(const RECT& rect, bool asSize);

	//=========== GENERAL ============

	/* Outputs a string representing this point as (x, y). */
	std::string toString();

	//========== OPERATORS ===========

	// Unary Assignment
	Point2I& operator++();
	Point2I& operator--();

	// Binary Assignment
	Point2I& operator+=(int rhs);
	Point2I& operator+=(Point2I rhs);
	Point2I& operator-=(int rhs);
	Point2I& operator-=(Point2I rhs);
	Point2I& operator*=(int rhs);
	Point2I& operator*=(Point2I rhs);
	Point2I& operator/=(int rhs);
	Point2I& operator/=(Point2I rhs);
	Point2I& operator%=(int rhs);
	Point2I& operator%=(Point2I rhs);

	// Unary
	Point2I operator+() const;
	Point2I operator-() const;

	// Binary
	Point2I operator+(int rhs) const;
	Point2I operator+(Point2I rhs) const;
	Point2I operator-(int rhs) const;
	Point2I operator-(Point2I rhs) const;
	Point2I operator*(int rhs) const;
	Point2I operator*(Point2I rhs) const;
	Point2I operator/(int rhs) const;
	Point2I operator/(Point2I rhs) const;
	Point2I operator%(int rhs) const;
	Point2I operator%(Point2I rhs) const;

	// Boolean
	bool operator==(int rhs) const;
	bool operator==(Point2I rhs) const;
	bool operator!=(int rhs) const;
	bool operator!=(Point2I rhs) const;
	bool operator>(int rhs) const;
	bool operator>(Point2I rhs) const;
	bool operator<(int rhs) const;
	bool operator<(Point2I rhs) const;
	bool operator>=(int rhs) const;
	bool operator>=(Point2I rhs) const;
	bool operator<=(int rhs) const;
	bool operator<=(Point2I rhs) const;

	// Index
	int& operator[](int index);

	//========== CONVERSION ==========

	operator Point2F() const;
	operator COORD() const;
	operator POINT() const;

	//========== PROPERTIES ==========

	/* The length of the point. */
	float length() const;
	/* The direction of the point. */
	float direction() const;
	/* Returns true if the point is positioned at (0, 0). */
	bool isZero() const;
	/* Returns a new point that is perpendicular to this one. */
	Point2I perpendicular() const;
	/* The area of the size. */
	int area() const;
};

/* A two-dimensional float-based point. */
struct Point2F {

	//========== CONSTANTS ===========

	/* A point positioned at (0.0f, 0.0f). */
	static const Point2F ZERO;
	/* A point positioned at (1.0f, 1.0f). */
	static const Point2F ONE;

	typedef float type;

	//=========== MEMBERS ============

	/* The x coordinate of the point. */
	float x;
	/* The y coordinate of the point. */
	float y;

	//========= CONSTRUCTORS =========

	/* Constructs the default point centered at (0.0f, 0.0f). */
	Point2F();
	/* Constructs a point with the specified x and y coordinates. */
	Point2F(float x, float y);
	/* Constructs a point with the same coordinates for both x and y. */
	Point2F(float xy);
	/* Constructs a copy of the specified point. */
	Point2F(const Point2F& point);
	/* Constructs a copy of the specified point. */
	//Point2F(const Point2I& point);
	/* Constructs a copy of the specified point. */
	Point2F(const COORD& point);
	/* Constructs a copy of the specified point. */
	Point2F(const POINT& point);

	//=========== GENERAL ============

	/* Outputs a string representing this point as (x, y). */
	std::string toString();

	//========== OPERATORS ===========

	// Unary Assignment
	Point2F& operator++();
	Point2F& operator--();

	// Binary Assignment
	Point2F& operator+=(float rhs);
	Point2F& operator+=(Point2F rhs);
	Point2F& operator-=(float rhs);
	Point2F& operator-=(Point2F rhs);
	Point2F& operator*=(float rhs);
	Point2F& operator*=(Point2F rhs);
	Point2F& operator/=(float rhs);
	Point2F& operator/=(Point2F rhs);
	Point2F& operator%=(float rhs);
	Point2F& operator%=(Point2F rhs);

	// Unary
	Point2F operator+() const;
	Point2F operator-() const;

	// Binary
	Point2F operator+(float rhs) const;
	Point2F operator+(Point2F rhs) const;
	Point2F operator-(float rhs) const;
	Point2F operator-(Point2F rhs) const;
	Point2F operator*(float rhs) const;
	Point2F operator*(Point2F rhs) const;
	Point2F operator/(float rhs) const;
	Point2F operator/(Point2F rhs) const;
	Point2F operator%(float rhs) const;
	Point2F operator%(Point2F rhs) const;

	// Boolean
	bool operator==(float rhs) const;
	bool operator==(Point2F rhs) const;
	bool operator!=(float rhs) const;
	bool operator!=(Point2F rhs) const;
	bool operator>(float rhs) const;
	bool operator>(Point2F rhs) const;
	bool operator<(float rhs) const;
	bool operator<(Point2F rhs) const;
	bool operator>=(float rhs) const;
	bool operator>=(Point2F rhs) const;
	bool operator<=(float rhs) const;
	bool operator<=(Point2F rhs) const;

	// Index
	float& operator[](int index);

	//========== CONVERSION ==========

	explicit operator Point2I() const;
	explicit operator COORD() const;
	explicit operator POINT() const;

	//========== PROPERTIES ==========

	/* The length of the point. */
	float length() const;
	/* The direction of the point. */
	float direction() const;
	/* Returns true if the point is positioned at (0.0f, 0.0f). */
	bool isZero() const;
	/* Returns a new point that is perpendicular to this one. */
	Point2F perpendicular() const;
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
