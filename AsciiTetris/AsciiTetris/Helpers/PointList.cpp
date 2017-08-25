/*=================================================================|
|* File:				PointList.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PointList.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace AsciiTetris::Helpers;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

PointList2I::PointList2I()
 :	points(),
	_origin(Point2F::ZERO) {}
PointList2I::PointList2I(int size, Point2F origin) {
	if (size <= 0)
		throw std::exception("PointList2I cannot construct a musicList of size 0 or less");
	Point2F mod = origin % 1.0f;
	if (mod != 0.0f && mod != 0.5f)
		throw std::exception("Invalid origin in 'PointList2I::PointList2I(int size, Point2F origin)'. Both coordinates must end in '.0' or '.5'");
	this->points = fixed_array<Point2I>(size);
	this->_origin = origin;
}
PointList2I::PointList2I(const std::vector<Point2I>& list, Point2F origin) {
	if (list.empty())
		throw std::exception("PointList2I cannot construct from a musicList of size 0");
	Point2F mod = origin % 1.0f;
	if (mod != 0.0f && mod != 0.5f)
		throw std::exception("Invalid origin in 'PointList2I::PointList2I(const vector<Point2I>& musicList, Point2F origin)'. Both coordinates must end in '.0' or '.5'");
	this->points = fixed_array<Point2I>(list.size());
	this->_origin = origin - 0.5f;
	for (int i = 0; i < (int)list.size(); i++)
		this->points[i] = list[i];
}
PointList2I::PointList2I(const PointList2I& list) {
	this->points = list.points;
	this->_origin = list._origin;
}
PointList2I::PointList2I(PointList2I&& list) {
	this->points = std::move(list.points);
	this->_origin = list._origin;
}

#pragma endregion
//========== OPERATORS ===========

// Assignment
PointList2I& PointList2I::operator=(const PointList2I& list) {
	this->points = list.points;
	this->_origin = list._origin;
	return *this;
}

// Index
Point2I& PointList2I::operator[](int index) {
	if (index < 0 || index >= (int)points.size())
		throw std::exception("Index out of bounds in 'PointList2I::operator[](int index)'");
	return points[index];
}
const Point2I& PointList2I::operator[](int index) const  {
	if (index < 0 || index >= (int)points.size())
		throw std::exception("Index out of bounds in 'PointList2I::operator[](int index)'");
	return points[index];
}


//======== TRANSFORMATION ========
// Accessor
PointList2I PointList2I::translated(Point2I distance) const {
	return PointList2I(*this).translate(distance);
}
PointList2I PointList2I::translated(int x, int y) const {
	return translated(Point2I(x, y));
}
PointList2I PointList2I::rotated(int rotations) const {
	return PointList2I(*this).rotate(rotations);
}

// Mutator
PointList2I& PointList2I::translate(Point2I distance) {
	for (int i = 0; i < (int)points.size(); i++)
		points[i] += distance;
	return *this;
}
PointList2I& PointList2I::translate(int x, int y) {
	return translate(Point2I(x, y));
}
PointList2I& PointList2I::rotate(int rotations) {
	rotations = GMath::wrap(rotations, 4);
	for (int i = 0; i < (int)points.size(); i++) {
		Point2F pointf = points[i];
		pointf -= _origin;
		for (int j = 0; j < rotations; j++)
			pointf = pointf.perpendicular();
		pointf += _origin;
		// Round just to be safe from floating point errors. I'm probably just being paranoid...
		points[i] = (Point2I)GMath::round(pointf);
	}
	return *this;
}

//========== PROPERTIES ==========
int PointList2I::count() const {
	return (int)points.size();
}
Point2F PointList2I::origin() const {
	return _origin;
}
Point2F PointList2I::center() const {
	Rectangle2I bounds = this->bounds();
	return (Point2F)bounds.point + (Point2F)bounds.size / 2.0f;
}
Rectangle2I PointList2I::bounds() const {
	Rectangle2I bounds = Rectangle2I(points[0], Point2I::ONE);
	for (int i = 1; i < (int)points.size(); i++) {
		bounds.topLeft(GMath::min(points[i], bounds.topLeft()));
		bounds.bottomRight(GMath::max(points[i], bounds.bottomRight()));
	}
	return bounds;
}

//=================================================================|
