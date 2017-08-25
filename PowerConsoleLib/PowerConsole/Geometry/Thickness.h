/*=================================================================|
|* File:				Thickness.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef THICKNESS_H
#define THICKNESS_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Point.h>

namespace PowerConsole::Geometry {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The thickness for a frame. */
struct Thickness {
	//========== CONSTANTS ===========
	#pragma region Constants

	/* A uniform thickness of 0. */
	static const Thickness ZERO;
	/* A uniform thickness of 1. */
	static const Thickness ONE;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* The thickness for the left side. */
	int left;
	/* The thickness for the top side. */
	int top;
	/* The thickness for the right side. */
	int right;
	/* The thickness for the bottom side. */
	int bottom;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs a uniform thickness of 0. */
	Thickness();
	/* Constructs the specified uniform thickness for all sides. */
	Thickness(int uniform);
	/* Constructs the specified uniform thickness for horizontal and vertical sides. */
	Thickness(int horizontal, int vertical);
	/* Constructs the specified thickness for each side. */
	Thickness(int left, int top, int right, int bottom);
	/* Constructs the specified uniform thickness for horizontal and vertical sides. */
	Thickness(Point2I horizontalVertical);
	/* Constructs the a thickness with the specified top-left and bottom-right. */
	Thickness(Point2I topLeft, Point2I bottomRight);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Returns the positive thickness. */
	Thickness operator+() const;
	/* Returns the negative thickness. */
	Thickness operator-() const;


	/* Returns true if the thickness is equal to the uniform value. */
	bool operator==(int uniform) const;
	/* Returns true if the thickness is equal to the uniform horizontal and vertical values. */
	bool operator==(Point2I horizontalVertical) const;
	/* Returns true if the thickness is equal to the specified thickness. */
	bool operator==(Thickness thickness) const;
	/* Returns true if the thickness is not equal to the uniform value. */
	bool operator!=(int uniform) const;
	/* Returns true if the thickness is not equal to the uniform horizontal and vertical values. */
	bool operator!=(Point2I horizontalVertical) const;
	/* Returns true if the thickness is equal to the specified thickness. */
	bool operator!=(Thickness thickness) const;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the horizontal thickness. */
	int horizontal() const;
	/* Gets the vertical thickness. */
	int vertical() const;
	/* Gets the point for both horizontal and vertical thickness. */
	Point2I horizontalVertical() const;
	/* Gets the top left thickness. */
	Point2I topLeft() const;
	/* Gets the top right thickness. */
	Point2I topRight() const;
	/* Gets the bottom left thickness. */
	Point2I bottomLeft() const;
	/* Gets the bottom right thickness. */
	Point2I bottomRight() const;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
