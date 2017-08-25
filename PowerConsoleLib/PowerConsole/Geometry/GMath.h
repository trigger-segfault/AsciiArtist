/*=================================================================|
|* File:				GMath.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef GMATH_H
#define GMATH_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Point.h>

#undef max
#undef min

namespace PowerConsole::Geometry {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* A static math class to encapsulate all needed mathematic functions and constants. */
class GMath {
public:
	//========== CONSTANTS ===========

	/* The floating-point value for pi. */
	static const float PI;
	/* The floating-point value for Euler's number. */
	static const float E;

	//============ BASIC =============

	// Max
	/* Returns the larger of the two specified numbers. */
	static int max(int a, int b);
	/* Returns the larger of the two specified numbers. */
	static unsigned int max(unsigned int a, unsigned int b);
	/* Returns the larger of the two specified numbers. */
	static float max(float a, float b);

	/* Returns the larger coordinates of the two specified points. */
	static Point2I max(Point2I a, Point2I b);
	/* Returns the larger coordinates of the two specified points. */
	static Point2F max(Point2F a, Point2F b);

	// Min
	/* Returns the smaller of the two specified numbers. */
	static int min(int a, int b);
	/* Returns the smaller of the two specified numbers. */
	static unsigned int min(unsigned int a, unsigned int b);
	/* Returns the smaller of the two specified numbers. */
	static float min(float a, float b);

	/* Returns the smaller coordinates of the two specified points. */
	static Point2I min(Point2I a, Point2I b);
	/* Returns the smaller coordinates of the two specified points. */
	static Point2F min(Point2F a, Point2F b);

	// Clamp
	/* Restricts the value to be within the specified range. */
	static int clamp(int value, int min, int max);
	/* Restricts the value to be within the specified range. */
	static unsigned int clamp(unsigned int value, unsigned int min, unsigned int max);
	/* Restricts the value to be within the specified range. */
	static float clamp(float value, float min, float max);

	/* Restricts the point to be within the specified range. */
	static Point2I clamp(Point2I value, Point2I min, Point2I max);
	/* Restricts the point to be within the specified range. */
	static Point2F clamp(Point2F value, Point2F min, Point2F max);

	// Sign
	/* Returns a value indicating the sign of the specified number. */
	static int sign(int a);
	/* Returns a value indicating the sign of the specified number. */
	static float sign(float a);

	/* Returns the coordinates indicating the sign of the specified point. */
	static Point2I sign(Point2I a);
	/* Returns the coordinates indicating the sign of the specified point. */
	static Point2F sign(Point2F a);

	// Odd
	/* Returns one if the number is odd and zero if the number is even. */
	static int odd(int a);
	/* Returns one if the number is odd and zero if the number is even. */
	static int odd(unsigned int a);
	/* Returns the coordinates indicating the oddness of the specified point where one is odd and zero is even. */
	static Point2I odd(Point2I a);

	// Even
	/* Returns one if the number is even and zero if the number is odd. */
	static int even(int a);
	/* Returns one if the number is even and zero if the number is odd. */
	static int even(unsigned int a);
	/* Returns the coordinates indicating the oddness of the specified point where one is even and zero is odd. */
	static Point2I even(Point2I a);

	// Abs
	/* Returns the absolute value of the specified number. */
	static int abs(int a);
	/* Returns the absolute value of the specified number. */
	static float abs(float a);

	/** Returns the absolute coordinates of the specified point. </summary> */
	static Point2I abs(Point2I a);
	/** Returns the absolute coordinates of the specified point. </summary> */
	static Point2F abs(Point2F a);

	// Wrap (modulus that also works as wrapping for negative numbers)
	/* Returns the positive modulus of the specified number. */
	static int wrap(int a, int mod);
	/* Returns the positive modulus of the specified number. */
	static float wrap(float a, float mod);

	/* Returns the positive modulus coordinates of the specified point. */
	static Point2I wrap(Point2I a, int mod);
	/* Returns the positive modulus coordinates of the specified point. */
	static Point2I wrap(Point2I a, Point2I mod);
	/* Returns the positive modulus coordinates of the specified point. */
	static Point2F wrap(Point2F a, float mod);
	/* Returns the positive modulus coordinates of the specified point. */
	static Point2F wrap(Point2F a, Point2F mod);

	// IsInt
	/* Returns true if the specified number is an integer. */
	static bool isInt(float a);

	/* Returns true if the specified coordinates are integers. */
	static bool isInt(Point2F a);

	// Fmod
	/* Returns the modulus of a floating point value. */
	static float fmod(float a, float mod);

	//=========== ROUNDING ===========

	// Floor
	/* Returns the largest integer less than or equal to the specified number. */
	static float floor(float a);

	/* Returns the largest point less than or equal to the specified point. */
	static Point2F floor(Point2F a);

	// Ceil
	/* Returns the smallest integer greater than or equal to the specified number. */
	static float ceil(float a);

	/* Returns the smallest point greater than or equal to the specified point. */
	static Point2F ceil(Point2F a);

	// Round
	/* Rounds the specified number to the nearest integral value. */
	static float round(float a);

	/* Rounds the specified point to the nearest integral coordinates. */
	static Point2F round(Point2F a);

	// Trunc
	/* Rounds the specified value to the nearest integer towards zero. */
	static float trunc(float a);

	/* Rounds the specified point to the nearest point towards zero. */
	static Point2F trunc(Point2F a);

	// Atrunc
	/* Rounds the specified value to the farthest integer from zero. */
	static float atrunc(float a);

	/* Rounds the specified point to the farthest point from zero. */
	static Point2F atrunc(Point2F a);

	//========== EXPONENTS ===========

	// Pow
	/* Returns the specified number raised to the specified power. */
	static int pow(int a, int b);
	/* Returns the specified number raised to the specified power. */
	static unsigned int pow(unsigned int a, unsigned int b);
	/* Returns the specified number raised to the specified power. */
	static float pow(float a, float b);

	/* Returns the specified point raised to the specified power. */
	static Point2I pow(Point2I a, int b);
	/* Returns the specified point raised to the specified power. */
	static Point2I pow(Point2I a, Point2I b);
	/* Returns the specified point raised to the specified power. */
	static Point2F pow(Point2F a, float b);
	/* Returns the specified point raised to the specified power. */
	static Point2F pow(Point2F a, Point2F b);

	// Root
	/* Returns a specified number lowered to the specified root. */
	static float root(float a, float b);

	/* Returns a specified point lowered to the specified root. */
	static Point2F root(Point2F a, float b);
	/* Returns a specified point lowered to the specified root. */
	static Point2F root(Point2F a, Point2F b);

	// Sqrt
	/* Returns the square root of a specified number. */
	static float sqrt(float a);

	/* Returns the square root of a specified vector. */
	static Point2F sqrt(Point2F a);

	//============ ANGLES ============

	// GetDirection
	/* Gets the direction of the point. */
	static float getDirection(Point2I a);
	/* Gets the direction of the point. */
	static float getDirection(Point2F a);

	// Modulus Angle
	/* Returns the absolute angle as (0, 360]. */
	static float modDirection(float a);

	//========= TRIGONOMETRY =========

	// Trigonometric
	/* Returns the sine of the specified angle. */
	static float sin(float a);
	/* Returns the cosine of the specified angle. */
	static float cos(float a);
	/* Returns the tangent of the specified angle. */
	static float tan(float a);

	// Arc Trigonometric
	/* Returns the angle whose sine is the specified number. */
	static float asin(float a);
	/* Returns the angle whose cosine is the specified number. */
	static float acos(float a);
	/* Returns the angle whose tangent is the specified number. */
	static float atan(float a);
	/* Returns the angle whose tangent is the quotient of two specified numbers. */
	static float atan2(float y, float x);
};
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
