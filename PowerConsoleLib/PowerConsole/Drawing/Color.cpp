/*=================================================================|
|* File:				Color.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Color.h"
#include <PowerConsole/Geometry/GMath.h>
#include <Windows.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region ColorB
//========= CONSTRUCTORS =========
#pragma region Constructors

ColorB::ColorB() : r(0), g(0), b(0), a(255) {}
ColorB::ColorB(int red, int green, int blue, int alpha) :
	r((unsigned char)GMath::clamp(red, 0, 255)),
	g((unsigned char)GMath::clamp(green, 0, 255)),
	b((unsigned char)GMath::clamp(blue, 0, 255)),
	a((unsigned char)GMath::clamp(alpha, 0, 255)) {}
ColorB::ColorB(COLORREF rawColor) :
	r((unsigned char)(rawColor & 0x0000FF)),
	g((unsigned char)((rawColor & 0x00FF00) >> 8)),
	b((unsigned char)((rawColor & 0xFF0000) >> 16)),
	a(255) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

// To String
std::string ColorB::toString() const { return "[R:" + std::to_string(r) + " G:" + std::to_string(g) + " B:" + std::to_string(b) + " A:" + std::to_string(a) + "]"; }

// Boolean
bool ColorB::operator==(ColorB color) const {
	return (r == color.r && g == color.g && b == color.b && a == color.a);
}
bool ColorB::operator!=(ColorB color) const {
	return (r != color.r || g != color.g || b != color.b || a != color.a);
}

// Conversion
ColorB::operator ColorI() const {
	return ColorI(r, g, b);
}
ColorB::operator COLORREF() const {
	return RGB(r, g, b);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma region ColorI
//========= CONSTRUCTORS =========
#pragma region Constructors

ColorI::ColorI() : r(0), g(0), b(0) {}
ColorI::ColorI(int red, int green, int blue) :
	r(red), g(green), b(blue) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

// To String
std::string ColorI::toString() const { return "[R:" + std::to_string(r) + " G:" + std::to_string(g) + " B:" + std::to_string(b) + "]"; }

// Binary Assignment
ColorI& ColorI::operator+=(ColorI color) { r += color.r; g += color.g; b += color.b; return *this; }
ColorI& ColorI::operator-=(ColorI color) { r -= color.r; g -= color.g; b -= color.b; return *this; }
ColorI& ColorI::operator*=(ColorI color) { r *= color.r; g *= color.g; b *= color.b; return *this; }
ColorI& ColorI::operator/=(ColorI color) { r /= color.r; g /= color.g; b /= color.b; return *this; }
ColorI& ColorI::operator+=(int uniform) { r += uniform; g += uniform; b += uniform; return *this; }
ColorI& ColorI::operator-=(int uniform) { r -= uniform; g -= uniform; b -= uniform; return *this; }
ColorI& ColorI::operator*=(int uniform) { r *= uniform; g *= uniform; b *= uniform; return *this; }
ColorI& ColorI::operator/=(int uniform) { r /= uniform; g /= uniform; b /= uniform; return *this; }

// Binary
ColorI ColorI::operator+(ColorI color) const { return ColorI(r + color.r, g + color.g, b + color.b); }
ColorI ColorI::operator-(ColorI color) const { return ColorI(r - color.r, g - color.g, b - color.b); }
ColorI ColorI::operator*(ColorI color) const { return ColorI(r * color.r, g * color.g, b * color.b); }
ColorI ColorI::operator/(ColorI color) const { return ColorI(r / color.r, g / color.g, b / color.b); }
ColorI ColorI::operator+(int uniform) const { return ColorI(r + uniform, g + uniform, b + uniform); }
ColorI ColorI::operator-(int uniform) const { return ColorI(r - uniform, g - uniform, b - uniform); }
ColorI ColorI::operator*(int uniform) const { return ColorI(r * uniform, g * uniform, b * uniform); }
ColorI ColorI::operator/(int uniform) const { return ColorI(r / uniform, g / uniform, b / uniform); }

// Boolean
bool ColorI::operator==(ColorI color) const { return (r == color.r && g == color.g && b == color.b); }
bool ColorI::operator!=(ColorI color) const { return (r != color.r || g != color.g || b != color.b); }
bool ColorI::operator==(int uniform) const { return (r == uniform && g == uniform && b == uniform); }
bool ColorI::operator!=(int uniform) const { return (r != uniform || g != uniform || b != uniform); }

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma region ColorF
//========= CONSTRUCTORS =========
#pragma region Constructors

ColorF::ColorF() : r(0.0f), g(0.0f), b(0.0f) {}
ColorF::ColorF(float red, float green, float blue) :
	r(red), g(green), b(blue) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

// To String
std::string ColorF::toString() const { return "[R:" + std::to_string(r) + " G:" + std::to_string(g) + " B:" + std::to_string(b) + "]"; }

// Binary Assignment
ColorF& ColorF::operator+=(ColorF color) { r += color.r; g += color.g; b += color.b; return *this; }
ColorF& ColorF::operator-=(ColorF color) { r -= color.r; g -= color.g; b -= color.b; return *this; }
ColorF& ColorF::operator*=(ColorF color) { r *= color.r; g *= color.g; b *= color.b; return *this; }
ColorF& ColorF::operator/=(ColorF color) { r /= color.r; g /= color.g; b /= color.b; return *this; }
ColorF& ColorF::operator+=(float uniform) { r += uniform; g += uniform; b += uniform; return *this; }
ColorF& ColorF::operator-=(float uniform) { r -= uniform; g -= uniform; b -= uniform; return *this; }
ColorF& ColorF::operator*=(float uniform) { r *= uniform; g *= uniform; b *= uniform; return *this; }
ColorF& ColorF::operator/=(float uniform) { r /= uniform; g /= uniform; b /= uniform; return *this; }

// Binary
ColorF ColorF::operator+(ColorF color) const { return ColorF(r + color.r, g + color.g, b + color.b); }
ColorF ColorF::operator-(ColorF color) const { return ColorF(r - color.r, g - color.g, b - color.b); }
ColorF ColorF::operator*(ColorF color) const { return ColorF(r * color.r, g * color.g, b * color.b); }
ColorF ColorF::operator/(ColorF color) const { return ColorF(r / color.r, g / color.g, b / color.b); }
ColorF ColorF::operator+(float uniform) const { return ColorF(r + uniform, g + uniform, b + uniform); }
ColorF ColorF::operator-(float uniform) const { return ColorF(r - uniform, g - uniform, b - uniform); }
ColorF ColorF::operator*(float uniform) const { return ColorF(r * uniform, g * uniform, b * uniform); }
ColorF ColorF::operator/(float uniform) const { return ColorF(r / uniform, g / uniform, b / uniform); }

// Boolean
bool ColorF::operator==(ColorF color) const { return (r == color.r && g == color.g && b == color.b); }
bool ColorF::operator!=(ColorF color) const { return (r != color.r || g != color.g || b != color.b); }
bool ColorF::operator==(float uniform) const { return (r == uniform && g == uniform && b == uniform); }
bool ColorF::operator!=(float uniform) const { return (r != uniform || g != uniform || b != uniform); }

#pragma endregion
//================================
#pragma endregion
//=================================================================|