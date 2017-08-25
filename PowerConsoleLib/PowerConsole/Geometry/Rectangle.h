/*=================================================================|
|* File:				Rectangle.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Thickness.h>

namespace PowerConsole::Geometry {
//=================================================================|
// PREDEFINITIONS                                                  |
//=================================================================/
struct Rectangle2I;
struct Rectangle2F;

//=================================================================|
// CLASSES														   |
//=================================================================/
/* A two-dimensional integer-based rectangle. */
struct Rectangle2I {

	//========== CONSTANTS ===========

	/* An empty rectangle with a point and size of (0, 0). */
	static const Rectangle2I ZERO;

	//=========== MEMBERS ============

	/* The position of the rectangle. */
	Point2I point;
	/* The size of the rectangle. */
	Point2I size;

	//========= CONSTRUCTORS =========

	/* Constructs the default rectangle with a point and size of (0, 0). */
	Rectangle2I();
	/* Constructs a rectangle with a point of (0, 0) and the specified size. */
	Rectangle2I(int width, int height);
	/* Constructs a rectangle with a point of (0, 0) and the specified size. */
	Rectangle2I(Point2I size);
	/* Constructs a rectangle with the specified point and size. */
	Rectangle2I(int x, int y, int width, int height, bool asEnds = false);
	/* Constructs a rectangle with the specified point and size. */
	Rectangle2I(Point2I point, int width, int height, bool asEnds = false);
	/* Constructs a rectangle with the specified point and size. */
	Rectangle2I(int x, int y, Point2I size, bool asEnds = false);
	/* Constructs a rectangle with the specified point and size. */
	Rectangle2I(Point2I point, Point2I size, bool asEnds = false);
	/* Constructs a copy of the specified rectangle. */
	Rectangle2I(const Rectangle2I& rect);
	/* Constructs a copy of the specified rectangle. */
	Rectangle2I(const SMALL_RECT& rect);
	/* Constructs a copy of the specified rectangle. */
	Rectangle2I(const RECT& rect);


	//========== OPERATORS ===========

	// Binary Assignment
	Rectangle2I& operator+=(Point2I rhs);
	Rectangle2I& operator-=(Point2I rhs);

	//Rectangle2I& operator+=(Thickness rhs);
	//Rectangle2I& operator-=(Thickness rhs);

	// Binary
	Rectangle2I operator+(Point2I rhs) const;
	Rectangle2I operator-(Point2I rhs) const;

	//Rectangle2I operator+(Thickness rhs) const;
	//Rectangle2I operator-(Thickness rhs) const;

	// Boolean
	bool operator==(Rectangle2I rhs) const;
	bool operator!=(Rectangle2I rhs) const;

	//========== CONVERSION ==========

	operator Rectangle2F() const;
	operator SMALL_RECT() const;
	operator RECT() const;

	//========== PROPERTIES ==========

	/* Returns the x position of the rectangle. */
	int x() const;
	/* Sets the x position of the rectangle. */
	void x(int x);
	/* Returns the y position of the rectangle. */
	int y() const;
	/* Sets the y position of the rectangle. */
	void y(int y);
	/* Returns the width of the rectangle. */
	int width() const;
	/* Sets the width of the rectangle. */
	void width(int width);
	/* Returns the height of the rectangle. */
	int height() const;
	/* Sets the height of the rectangle. */
	void height(int height);

	/* Returns the left side of the rectangle. */
	int left() const;
	/* Sets the left side of the rectangle. */
	void left(int left);
	/* Returns the top side of the rectangle. */
	int top() const;
	/* Sets the top side of the rectangle. */
	void top(int top);
	/* Returns the right side of the rectangle. */
	int right() const;
	/* Sets the right side of the rectangle. */
	void right(int right);
	/* Returns the bottom side of the rectangle. */
	int bottom() const;
	/* Sets the bottom side of the rectangle. */
	void bottom(int bottom);
	/* Returns the top-left corner of the rectangle. */
	Point2I topLeft() const;
	/* Returns the top-left corner of the rectangle. */
	void topLeft(Point2I topLeft);
	/* Returns the top-right corner of the rectangle. */
	Point2I topRight() const;
	/* Returns the top-right corner of the rectangle. */
	void topRight(Point2I topRight);
	/* Returns the bottom-left corner of the rectangle. */
	Point2I bottomLeft() const;
	/* Returns the bottom-left corner of the rectangle. */
	void bottomLeft(Point2I bottomLeft);
	/* Returns the bottom-right corner of the rectangle. */
	Point2I bottomRight() const;
	/* Returns the bottom-right corner of the rectangle. */
	void bottomRight(Point2I bottomRight);

	/* Returns the absolute width of the rectangle. */
	int absWidth() const;
	/* Returns the absolute height of the rectangle. */
	int absHeight() const;
	/* Returns the abolute size of the rectangle. */
	Point2I absSize() const;

	/* Returns the absolute left side of the rectangle. */
	int absLeft() const;
	/* Returns the top side of the rectangle. */
	int absTop() const;
	/* Returns the right side of the rectangle. */
	int absRight() const;
	/* Returns the bottom side of the rectangle. */
	int absBottom() const;
	/* Returns the top-left corner of the rectangle. */
	Point2I absTopLeft() const;
	/* Returns the top-right corner of the rectangle. */
	Point2I absTopRight() const;
	/* Returns the bottom-left corner of the rectangle. */
	Point2I absBottomLeft() const;
	/* Returns the bottom-right corner of the rectangle. */
	Point2I absBottomRight() const;

	/* Returns this rect with the point guaranteed to be in the top-left corner. */
	Rectangle2I absRect() const;

	/* The area of the rectangle. */
	int area() const;
	/* The perimeter of the rectangle. */
	int perimeter() const;
	/* The center of the rectangle. */
	Point2I center() const;

	/* Gets the pixel boundaries of the rectangle. */
	Rectangle2I pixelBounds() const;

	//======== TRANSFORMATION ========

	// Accessor
	/* Returns a translated rectangle. */
	Rectangle2I translated(Point2I distance) const;
	/* Returns a scaled rectangle. */
	Rectangle2I scaled(Point2F scale) const;
	/* Returns an inflated rectangle. */
	Rectangle2I inflated(Thickness amount) const;

	// Mutator
	/* Translates and returns the rectanglet. */
	Rectangle2I& translate(Point2I distance);
	/* Scales and returns the rectangle. */
	Rectangle2I& scale(Point2F scale);
	/* Inflates and returns the rectangle. */
	Rectangle2I& inflate(Thickness amount);

	//========== COLLISION ===========

	/* Returns true if the rectangle contains the specified point. */
	bool containsPoint(Point2I point) const;
	/* Returns true if the rectangle contains the specified rectangle. */
	bool containsRect(Rectangle2I rect) const;

	/* Returns the intersection between these rectangles. */
	Rectangle2I& intersect(Rectangle2I rect);
	/* Returns the intersection between these rectangles. */
	Rectangle2I intersection(Rectangle2I rect) const;
	/* Returns a new rectangle that contains both rectangles. */
	Rectangle2I unionRect(Rectangle2I rect) const;
};

/* A two-dimensional float-based rectangle. */
struct Rectangle2F {

	//========== CONSTANTS ===========

	/* An empty rectangle with a point and size of (0.0f, 0.0f). */
	static const Rectangle2F ZERO;

	//=========== MEMBERS ============

	/* The position of the rectangle. */
	Point2F point;
	/* The size of the rectangle. */
	Point2F size;

	//========= CONSTRUCTORS =========

	/* Constructs the default rectangle with a point and size of (0.0f, 0.0f). */
	Rectangle2F();
	/* Constructs a rectangle with a point of (0.0f, 0.0f) and the specified size. */
	Rectangle2F(float width, float height);
	/* Constructs a rectangle with a point of (0.0f, 0.0f) and the specified size. */
	Rectangle2F(Point2F size);
	/* Constructs a rectangle with the specified point and size. */
	Rectangle2F(float x, float y, float width, float height);
	/* Constructs a rectangle with the specified point and size. */
	Rectangle2F(Point2F point, float width, float height);
	/* Constructs a rectangle with the specified point and size. */
	Rectangle2F(float x, float y, Point2F size);
	/* Constructs a rectangle with the specified point and size. */
	Rectangle2F(Point2F point, Point2F size);
	/* Constructs a copy of the specified rectangle. */
	Rectangle2F(const Rectangle2F& rect);
	/* Constructs a copy of the specified rectangle. */
	Rectangle2F(const Rectangle2I& rect);
	/* Constructs a copy of the specified rectangle. */
	Rectangle2F(const SMALL_RECT& rect);
	/* Constructs a copy of the specified rectangle. */
	Rectangle2F(const RECT& rect);

	//========== OPERATORS ===========

	// Binary Assignment
	Rectangle2F& operator+=(Rectangle2F rhs);
	Rectangle2F& operator-=(Rectangle2F rhs);
	Rectangle2F& operator*=(Rectangle2F rhs);
	Rectangle2F& operator/=(Rectangle2F rhs);

	// Binary
	Rectangle2F operator+(Rectangle2F rhs) const;
	Rectangle2F operator-(Rectangle2F rhs) const;
	Rectangle2F operator*(Rectangle2F rhs) const;
	Rectangle2F operator/(Rectangle2F rhs) const;

	// Boolean
	bool operator==(Rectangle2F rhs) const;
	bool operator!=(Rectangle2F rhs) const;

	//========== CONVERSION ==========

	explicit operator Rectangle2I() const;
	explicit operator SMALL_RECT() const;
	explicit operator RECT() const;

	//========== PROPERTIES ==========

	/* Returns the x position of the rectangle. */
	float x() const;
	/* Returns the y position of the rectangle. */
	float y() const;
	/* Returns the width of the rectangle. */
	float width() const;
	/* Returns the height of the rectangle. */
	float height() const;
	/* Returns the absolute width of the rectangle. */
	float absWidth() const;
	/* Returns the absolute height of the rectangle. */
	float absHeight() const;

	/* Returns the left side of the rectangle. */
	float left() const;
	/* Returns the right side of the rectangle. */
	float right() const;
	/* Returns the top side of the rectangle. */
	float top() const;
	/* Returns the bottom side of the rectangle. */
	float bottom() const;
	/* Returns the top-left corner of the rectangle. */
	Point2F topLeft() const;
	/* Returns the top-right corner of the rectangle. */
	Point2F topRight() const;
	/* Returns the bottom-left corner of the rectangle. */
	Point2F bottomLeft() const;
	/* Returns the bottom-right corner of the rectangle. */
	Point2F bottomRight() const;

	/* Returns the abolute size of the rectangle. */
	Point2F absSize() const;
	/* Returns this rect with the point guaranteed to be in the top-left corner. */
	Rectangle2F absRect() const;

	/* The area of the rectangle. */
	float area() const;
	/* The perimeter of the rectangle. */
	float perimeter() const;
	/* The center of the rectangle. */
	Point2F center() const;

	//======== TRANSFORMATION ========

	// Accessor
	/* Returns a translated rectangle. */
	Rectangle2F translated(Point2F distance) const;
	/* Returns a translated rectangle. */
	Rectangle2F translated(float x, float y) const;
	/* Returns a scaled rectangle. */
	Rectangle2F scaled(Point2F scale) const;
	/* Returns a scaled rectangle. */
	Rectangle2F scaled(float x, float y) const;

	// Mutator
	/* Translates and returns the rectanglet. */
	Rectangle2F& translate(Point2F distance);
	/* Translates and returns the rectangle. */
	Rectangle2F& translate(float x, float y);
	/* Scales and returns the rectangle. */
	Rectangle2F& scale(Point2F scale);
	/* Scales and returns the rectangle. */
	Rectangle2F& scale(float x, float y);

	//========== COLLISION ===========

	/* Returns true if the rectangle contains the specified point. */
	bool containsPoint(Point2F point) const;
	/* Returns true if the rectangle contains the specified rectangle. */
	bool containsRect(Rectangle2F rect) const;
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
