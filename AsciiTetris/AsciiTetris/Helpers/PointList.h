/*=================================================================|
|* File:				PointLIst.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef POINT_LIST_H
#define POINT_LIST_H

#include <PowerConsole/Geometry/Rectangle.h>
#include <PowerConsole/Helpers/fixed_array.h>
#include <memory>
#include <vector>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;

namespace AsciiTetris::Helpers {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* A list of 2D integer points. */
class PointList2I {

	//=========== MEMBERS ============
	#pragma region Members

	/* The constant size list of points. */
	fixed_array<Point2I> points;
	/* The origin of rotation for the point list. */
	Point2F _origin;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	PointList2I();
	PointList2I(int size, Point2F origin);
	PointList2I(const std::vector<Point2I>& list, Point2F origin);
	PointList2I(const PointList2I& list);
	PointList2I(PointList2I&& list);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	// Assignment
	PointList2I& operator=(const PointList2I& list);

	// Index
	Point2I& operator[](int index);
	const Point2I& operator[](int index) const;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the number of points in the list. */
	int count() const;
	/* Gets the origin of rotation for the point list. */
	Point2F origin() const;
	/* Gets the center of the point list. */
	Point2F center() const;
	/* Gets the bounding box of the point list. */
	Rectangle2I bounds() const;

	#pragma endregion
	//======== TRANSFORMATION ========
	#pragma region Transformation

	// Accessor
	/* Returns a translated point list. */
	PointList2I translated(Point2I distance) const;
	/* Returns a translated point list. */
	PointList2I translated(int x, int y) const;
	/* Returns a rotated point list. */
	PointList2I rotated(int rotations) const;

	// Mutator
	/* Translates and returns the point list. */
	PointList2I& translate(Point2I distance);
	/* Translates and returns the point list. */
	PointList2I& translate(int x, int y);
	/* Rotates and returns the point list. */
	PointList2I& rotate(int rotations);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
