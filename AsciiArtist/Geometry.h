//================================================================
// GEOMETRY HEADER:
//================================================================
#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "stdafx.h"

using namespace std;
//================================================================
// NAMESPACE:
//================================================================
/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of geometric classes. */
namespace Geometry {
//================================================================
// PREDEFINITIONS:
//================================================================
class Coord;
class Point2D;
class SmallRect;
class Rectangle2D;
//================================================================
// CLASSES:
//================================================================
/* The basic class for storing integer precision coordinates. */
class Coord {
public:
	
	//========== CONSTANTS ===========
	
	/* A point positioned at (0, 0). */
	static const Coord Zero;
	/* A point positioned at (1, 1). */
	static const Coord One;

	//=========== MEMBERS ============

	/* The x coordinate of the point. */
	int X;
	/* The y coordinate of the point. */
	int Y;

	//========= CONSTRUCTORS =========

	/* Constructs the default point centered at (0, 0). */
	Coord();
	/* Constructs a point with the specified x and y coordinates. */
	Coord(int x, int y);
	/* Constructs a point with the specified x and y coordinates. */
	Coord(int xy);
	/* Constructs a copy of the specified point. */
	Coord(const Coord& coord);
	/* Constructs a copy of the specified point. */
	Coord(const COORD& coord);
	/* Constructs a copy of the specified point. */
	Coord(const POINT& point);
	/* Constructs a copy of the specified point or size of the rectangle. */
	Coord(const SMALL_RECT& rect, bool asSize);
	/* Constructs a copy of the specified point or size of the rectangle. */
	Coord(const RECT& rect, bool asSize);
	
	//========== OPERATORS ===========
	
	// Unary Assignment
	Coord& operator++();
	Coord& operator--();

	// Binary Assignment
	Coord& operator+=(int rhs);
	Coord& operator+=(Coord rhs);
	Coord& operator-=(int rhs);
	Coord& operator-=(Coord rhs);
	Coord& operator*=(int rhs);
	Coord& operator*=(Coord rhs);
	Coord& operator/=(int rhs);
	Coord& operator/=(Coord rhs);
	Coord& operator%=(int rhs);
	Coord& operator%=(Coord rhs);

	// Unary
	Coord operator+() const;
	Coord operator-() const;
	
	// Binary
	Coord operator+(int rhs) const;
	Coord operator+(Coord rhs) const;
	Coord operator-(int rhs) const;
	Coord operator-(Coord rhs) const;
	Coord operator*(int rhs) const;
	Coord operator*(Coord rhs) const;
	Coord operator/(int rhs) const;
	Coord operator/(Coord rhs) const;
	Coord operator%(int rhs) const;
	Coord operator%(Coord rhs) const;

	// Boolean
	bool operator==(Coord rhs) const;
	bool operator!=(Coord rhs) const;
	bool operator>(Coord rhs) const;
	bool operator<(Coord rhs) const;
	bool operator>=(Coord rhs) const;
	bool operator<=(Coord rhs) const;
	
	//========== CONVERSION ==========
	
	operator COORD() const;
	operator Point2D() const;
	operator POINT() const;
	
	//========= INFORMATION ==========

	/* The length of the point. */
	double Length() const;
	/* The direction of the point. */
	double Direction() const;
};
/* The basic class for storing double precision coordinates. */
class Point2D {
public:
	//========== CONSTANTS ===========
	
	/* A point positioned at (0.0, 0.0). */
	static const Point2D Zero;
	/* A point positioned at (1.0, 1.0). */
	static const Point2D One;

	//=========== MEMBERS ============

	/* The x coordinate of the point. */
	double X;
	/* The y coordinate of the point. */
	double Y;

	//========= CONSTRUCTORS =========

	/* Constructs the default point centered at (0, 0). */
	Point2D();
	/* Constructs a point with the specified x and y coordinates. */
	Point2D(double x, double y);
	/* Constructs a point with the specified x and y coordinates. */
	Point2D(double xy);
	/* Constructs a copy of the specified point. */
	Point2D(const Point2D& point);
	/* Constructs a copy of the specified point. */
	Point2D(const COORD& coord);
	/* Constructs a copy of the specified point. */
	Point2D(const POINT& point);
	/* Constructs a copy of the specified point or size of the rectangle. */
	Point2D(const SMALL_RECT& rect, bool asSize);
	/* Constructs a copy of the specified point or size of the rectangle. */
	Point2D(const RECT& rect, bool asSize);
	
	//========== OPERATORS ===========
	
	// Unary Assignment
	Point2D& operator++();
	Point2D& operator--();
	
	// Binary Assignment
	Point2D& operator+=(double rhs);
	Point2D& operator+=(Point2D rhs);
	Point2D& operator-=(double rhs);
	Point2D& operator-=(Point2D rhs);
	Point2D& operator*=(double rhs);
	Point2D& operator*=(Point2D rhs);
	Point2D& operator/=(double rhs);
	Point2D& operator/=(Point2D rhs);
	Point2D& operator%=(double rhs);
	Point2D& operator%=(Point2D rhs);

	// Unary
	Point2D operator+() const;
	Point2D operator-() const;

	// Binary
	Point2D operator+(double rhs) const;
	Point2D operator+(Point2D rhs) const;
	Point2D operator-(double rhs) const;
	Point2D operator-(Point2D rhs) const;
	Point2D operator*(double rhs) const;
	Point2D operator*(Point2D rhs) const;
	Point2D operator/(double rhs) const;
	Point2D operator/(Point2D rhs) const;
	Point2D operator%(double rhs) const;
	Point2D operator%(Point2D rhs) const;

	// Boolean
	bool operator==(Point2D rhs) const;
	bool operator!=(Point2D rhs) const;
	bool operator>(Point2D rhs) const;
	bool operator<(Point2D rhs) const;
	bool operator>=(Point2D rhs) const;
	bool operator<=(Point2D rhs) const;
	
	//========== CONVERSION ==========
	
	operator Coord() const;
	operator COORD() const;
	operator POINT() const;

	//========= INFORMATION ==========

	/* The length of the point. */
	double Length() const;
	/* The direction of the point. */
	double Direction() const;
};
/* The basic class for storing an integer precision rectangle. */
class SmallRect {
public:
	//=========== MEMBERS ============

	/* The position of the rectangle. */
	Coord Point;
	/* The size of the rectangle. */
	Coord Size;

	//========= CONSTRUCTORS =========

	/* Constructs the default empty rectangle. */
	SmallRect();
	/* Constructs a rectangle with the specified position and size. */
	SmallRect(int x, int y, int width, int height);
	/* Constructs a rectangle with the specified position and size. */
	SmallRect(Coord point, int width, int height);
	/* Constructs a rectangle with the specified position and size. */
	SmallRect(int x, int y, Coord size);
	/* Constructs a rectangle with the specified position and size. */
	SmallRect(Coord point, Coord size);
	/* Constructs a copy of the specified rectangle. */
	//SmallRect(const Rectangle2D& rect);
	/* Constructs a copy of the specified rectangle. */
	SmallRect(const SMALL_RECT& rect);
	/* Constructs a copy of the specified rectangle. */
	SmallRect(const RECT& rect);
	
	//========== OPERATORS ===========

	// Binary
	SmallRect operator+(int rhs) const;
	SmallRect operator+(Coord rhs) const;
	SmallRect operator-(int rhs) const;
	SmallRect operator-(Coord rhs) const;
	SmallRect operator*(int rhs) const;
	SmallRect operator*(Coord rhs) const;
	SmallRect operator/(int rhs) const;
	SmallRect operator/(Coord rhs) const;
	SmallRect operator%(int rhs) const;
	SmallRect operator%(Coord rhs) const;

	// Boolean
	bool operator==(SmallRect rhs) const;
	bool operator!=(SmallRect rhs) const;
	
	//========== CONVERSION ==========
	
	//operator Rectangle2D() const;
	operator SMALL_RECT() const;
	operator RECT() const;

	//========= INFORMATION ==========

	/* The area of the rectangle. */
	int Area() const;
	/* The perimeter of the rectangle. */
	int Perimeter() const;

	//========== COLLISION ===========

	/* Returns true if the rectangle contains the specified point. */
	bool ContainsPoint(Coord coord) const;
	/* Returns true if the rectangle contains the specified rectangle. */
	bool ContainsRect(SmallRect rect) const;
};
//================================================================
// END GEOMETRY HEADER:
//================================================================
}}
#endif