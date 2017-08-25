/*=================================================================|
|* File:				GMath.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "GMath.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/
// class GMath
//========== CONSTANTS ===========
const float GMath::PI = (float)M_PI;
const float GMath::E = (float)M_E;

//============ BASIC =============
// Max
int GMath::max(int a, int b) {
	return (a > b ? a : b);
}
unsigned int GMath::max(unsigned int a, unsigned int b) {
	return (a > b ? a : b);
}
float GMath::max(float a, float b) {
	return (a > b ? a : b);
}

Point2I GMath::max(Point2I a, Point2I b) {
	return Point2I((a.x > b.x ? a.x : b.x), (a.y > b.y ? a.y : b.y));
}
Point2F GMath::max(Point2F a, Point2F b) {
	return Point2F((a.x > b.x ? a.x : b.x), (a.y > b.y ? a.y : b.y));
}

// Min
int GMath::min(int a, int b) {
	return (a < b ? a : b);
}
unsigned int GMath::min(unsigned int a, unsigned int b) {
	return (a < b ? a : b);
}
float GMath::min(float a, float b) {
	return (a < b ? a : b);
}

Point2I GMath::min(Point2I a, Point2I b) {
	return Point2I((a.x < b.x ? a.x : b.x), (a.y < b.y ? a.y : b.y));
}
Point2F GMath::min(Point2F a, Point2F b) {
	return Point2F((a.x < b.x ? a.x : b.x), (a.y < b.y ? a.y : b.y));
}

// Clamp
int GMath::clamp(int value, int min, int max) {
	//if (min > max)
	//	throw std::exception("Parameter 'min' is greater than 'max' in 'GMath::clamp(...)'");
	return GMath::max(min, GMath::min(max, value));
}
unsigned int GMath::clamp(unsigned int value, unsigned int min, unsigned int max) {
	//if (min > max)
	//	throw std::exception("Parameter 'min' is greater than 'max' in 'GMath::clamp(...)'");
	return GMath::max(min, GMath::min(max, value));
}
float GMath::clamp(float value, float min, float max) {
	//if (min > max)
	//	throw std::exception("Parameter 'min' is greater than 'max' in 'GMath::clamp(...)'");
	return GMath::max(min, GMath::min(max, value));
}

Point2I GMath::clamp(Point2I value, Point2I min, Point2I max) {
	//if (min.x > max.x)
	//	throw std::exception("Parameter 'min.x' is greater than 'max.x' in 'GMath::clamp(...)'");
	//else if (min.y > max.y)
	//	throw std::exception("Parameter 'min.y' is greater than 'max.y' in 'GMath::clamp(...)'");
	return GMath::max(min, GMath::min(max, value));
}
Point2F GMath::clamp(Point2F value, Point2F min, Point2F max) {
	//if (min.x > max.x)
	//	throw std::exception("Parameter 'min.x' is greater than 'max.x' in 'GMath::clamp(...)'");
	//else if (min.y > max.y)
	//	throw std::exception("Parameter 'min.y' is greater than 'max.y' in 'GMath::clamp(...)'");
	return GMath::max(min, GMath::min(max, value));
}

// Sign
int GMath::sign(int a) {
	return (a == 0 ? 0 : (a > 0 ? 1 : -1));
}
float GMath::sign(float a) {
	return (float)(a == 0 ? 0 : (a > 0 ? 1 : -1));
}

Point2I GMath::sign(Point2I a) {
	return Point2I((a.x == 0 ? 0 : (a.x > 0 ? 1 : -1)), (a.y == 0 ? 0 : (a.y > 0 ? 1 : -1)));
}
Point2F GMath::sign(Point2F a) {
	return Point2F((float)(a.x == 0 ? 0 : (a.x > 0 ? 1 : -1)), (float)(a.y == 0 ? 0 : (a.y > 0 ? 1 : -1)));
}

// Odd
int GMath::odd(int a) {
	return (a % 2 == 1 ? 1 : 0);
}
int GMath::odd(unsigned int a) {
	return (a % 2 == 1 ? 1 : 0);
}
Point2I GMath::odd(Point2I a) {
	return Point2I(a.x % 2 == 1 ? 1 : 0, a.y % 2 == 1 ? 1 : 0);
}

// Even
int GMath::even(int a) {
	return (a % 2 == 0 ? 1 : 0);
}
int GMath::even(unsigned int a) {
	return (a % 2 == 0 ? 1 : 0);
}
Point2I GMath::even(Point2I a) {
	return Point2I(a.x % 2 == 0 ? 1 : 0, a.y % 2 == 0 ? 1 : 0);
}

// Abs
int GMath::abs(int a) {
	return std::abs(a);
}
float GMath::abs(float a) {
	return std::abs(a);
}

Point2I GMath::abs(Point2I a) {
	return Point2I(std::abs(a.x), std::abs(a.y));
}
Point2F GMath::abs(Point2F a) {
	return Point2F(std::abs(a.x), std::abs(a.y));
}

// Wrap
int GMath::wrap(int a, int mod) {
	int value = a % mod;
	return (value < 0 ? (value + mod) : value);
}
float GMath::wrap(float a, float mod) {
	float value = std::fmod(a, mod);
	return (value < 0 ? (value + mod) : value);
}

Point2I GMath::wrap(Point2I a, int mod) {
	int x = a.x % mod;
	int y = a.y % mod;
	return Point2I((x < 0 ? (x + mod) : x), (y < 0 ? (y + mod) : y));
}
Point2I GMath::wrap(Point2I a, Point2I mod) {
	int x = a.x % mod.x;
	int y = a.y % mod.y;
	return Point2I((x < 0 ? (x + mod.x) : x), (y < 0 ? (y + mod.y) : y));
}
Point2F GMath::wrap(Point2F a, float mod) {
	float x = std::fmod(a.x, mod);
	float y = std::fmod(a.y, mod);
	return Point2F((x < 0 ? (x + mod) : x), (y < 0 ? (y + mod) : y));
}
Point2F GMath::wrap(Point2F a, Point2F mod) {
	float x = std::fmod(a.x, mod.x);
	float y = std::fmod(a.y, mod.y);
	return Point2F((x < 0 ? (x + mod.x) : x), (y < 0 ? (y + mod.y) : y));
}

// IsInt
bool GMath::isInt(float a) {
	return (a == 0.0f);
}

bool GMath::isInt(Point2F a) {
	return (a.x == 0.0f && a.y == 0.0f);
}

// Fmod
float GMath::fmod(float a, float mod) {
	return std::fmod(a, mod);
}

//=========== ROUNDING ===========

// Floor
float GMath::floor(float a) {
	return std::floor(a);
}

Point2F GMath::floor(Point2F a) {
	return Point2F(std::floor(a.x), std::floor(a.y));
}

// Ceil
float GMath::ceil(float a) {
	return std::ceil(a);
}

Point2F GMath::ceil(Point2F a) {
	return Point2F(std::ceil(a.x), std::ceil(a.y));
}

// Round
float GMath::round(float a) {
	return std::round(a);
}

Point2F GMath::round(Point2F a) {
	return Point2F(std::round(a.x), std::round(a.y));
}

// Trunc
float GMath::trunc(float a) {
	return std::trunc(a);
}

Point2F GMath::trunc(Point2F a) {
	return Point2F(std::trunc(a.x), std::trunc(a.y));
}

// Atrunc
float GMath::atrunc(float a) {
	return (a < 0.0f ? std::floor(a) : std::ceil(a));
}

Point2F GMath::atrunc(Point2F a) {
	return Point2F((a.x < 0.0f ? std::floor(a.x) : std::ceil(a.x)), (a.y < 0.0f ? std::floor(a.y) : std::ceil(a.y)));
}

//========== EXPONENTS ===========

// Pow
int GMath::pow(int a, int b) {
	return (int)std::pow(a, b);
}
unsigned int GMath::pow(unsigned int a, unsigned int b) {
	return (unsigned int)std::pow(a, b);
}
float GMath::pow(float a, float b) {
	return std::pow(a, b);
}

Point2I GMath::pow(Point2I a, int b) {
	return Point2I((int)std::pow(a.x, b), (int)std::pow(a.y, b));
}
Point2I GMath::pow(Point2I a, Point2I b) {
	return Point2I((int)std::pow(a.x, b.x), (int)std::pow(a.y, b.y));
}
Point2F GMath::pow(Point2F a, float b) {
	return Point2F(std::pow(a.x, b), std::pow(a.y, b));
}
Point2F GMath::pow(Point2F a, Point2F b) {
	return Point2F(std::pow(a.x, b.x), std::pow(a.y, b.y));
}

// Root
float GMath::root(float a, float b) {
	return std::pow(a, 1.0f / b);
}

Point2F GMath::root(Point2F a, float b) {
	return Point2F(std::pow(a.x, 1.0f / b), std::pow(a.y, 1.0f / b));
}
Point2F GMath::root(Point2F a, Point2F b) {
	return Point2F(std::pow(a.x, 1.0f / b.x), std::pow(a.y, 1.0f / b.y));
}

// Sqrt
float GMath::sqrt(float a) {
	return std::sqrt(a);
}

Point2F GMath::sqrt(Point2F a) {
	return Point2F(std::sqrt(a.x), std::sqrt(a.y));
}

//============ ANGLES ============

// GetDirection
float GMath::getDirection(Point2I a) {
	if (a.isZero())
		return 0;
	return GMath::modDirection(GMath::atan2((float)a.y, (float)a.x));
}
float GMath::getDirection(Point2F a) {
	if (a.isZero())
		return 0;
	return GMath::modDirection(atan2(a.y, a.x));
}

// Modulus Angle
float GMath::modDirection(float a) {
	return GMath::wrap(a, 360.0f);
}

//========= TRIGONOMETRY =========

// Trigonometric
float GMath::sin(float a) {
	return std::sin(a / 180.0f * (float)M_PI);
}
float GMath::cos(float a) {
	return std::cos(a / 180.0f * (float)M_PI);
}
float GMath::tan(float a) {
	return std::tan(a / 180.0f * (float)M_PI);
}

// Arc Trigonometric
float GMath::asin(float a) {
	return (std::asin(a) * 180.0f / (float)M_PI);
}
float GMath::acos(float a) {
	return (std::acos(a) * 180.0f / (float)M_PI);
}
float GMath::atan(float a) {
	return (std::atan(a) * 180.0f / (float)M_PI);
}
float GMath::atan2(float y, float x) {
	return (std::atan2(y, x) * 180.0f / (float)M_PI);
}

//=================================================================|