/*=================================================================|
|* File:				Point.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef COLOR_H
#define COLOR_H

#include <string>
#include <PowerConsole/Declarations.h>

namespace PowerConsole::Drawing {

DECLARE_STRUCT(ColorB);
DECLARE_STRUCT(ColorI);
DECLARE_STRUCT(ColorF);
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region ColorB

/* A structure for storing a byte-sized RGBA color. */
 struct ColorB {
	
	//=========== MEMBERS ============
	#pragma region Members

	/* The blue value of the color. */
	unsigned char b;
	/* The green value of the color. */
	unsigned char g;
	/* The red value of the color. */
	unsigned char r;
	/* The alpha value of the color. */
	unsigned char a;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default color with the values of R:0 G:0 B:0 A:255. */
	ColorB();
	/* Constructs a color with the specified RGB values. */
	ColorB(int red, int green, int blue, int alpha = 255);
	/* Constructs a color from the raw color. */
	ColorB(COLORREF rawColor);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	// To String
	/* Outputs a string representing this color in the format of [R:r G:g B:b A:a]. */
	std::string toString() const;

	// Boolean
	bool operator==(ColorB color) const;
	bool operator!=(ColorB color) const;

	// Conversion
	operator ColorI() const;
	explicit operator COLORREF() const;

	#pragma endregion
};

#pragma endregion
//=================================================================|
#pragma region ColorI

/* A structure for storing an integer-sized RGB color. */
struct ColorI {

	//=========== MEMBERS ============
	#pragma region Members

	/* The blue value of the color. */
	int b;
	/* The green value of the color. */
	int g;
	/* The red value of the color. */
	int r;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default color with the values of R:0 G:0 B:0. */
	ColorI();
	/* Constructs a color with the specified RGB values. */
	ColorI(int red, int green, int blue);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	// To String
	/* Outputs a string representing this color in the format of [R:r G:g B:b]. */
	std::string toString() const;

	// Binary Assignment
	ColorI& operator+=(ColorI color);
	ColorI& operator-=(ColorI color);
	ColorI& operator*=(ColorI color);
	ColorI& operator/=(ColorI color);
	ColorI& operator+=(int uniform);
	ColorI& operator-=(int uniform);
	ColorI& operator*=(int uniform);
	ColorI& operator/=(int uniform);

	// Binary
	ColorI operator+(ColorI color) const;
	ColorI operator-(ColorI color) const;
	ColorI operator*(ColorI color) const;
	ColorI operator/(ColorI color) const;
	ColorI operator+(int uniform) const;
	ColorI operator-(int uniform) const;
	ColorI operator*(int uniform) const;
	ColorI operator/(int uniform) const;

	// Boolean
	bool operator==(ColorI color) const;
	bool operator!=(ColorI color) const;
	bool operator==(int uniform) const;
	bool operator!=(int uniform) const;

	#pragma endregion
};

#pragma endregion
//=================================================================|
#pragma region ColorI

/* A structure for storing an float-sized RGB color. */
struct ColorF {

	//=========== MEMBERS ============
	#pragma region Members

	/* The blue value of the color. */
	float b;
	/* The green value of the color. */
	float g;
	/* The red value of the color. */
	float r;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default color with the values of R:0 G:0 B:0. */
	ColorF();
	/* Constructs a color with the specified RGB values. */
	ColorF(float red, float green, float blue);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	// To String
	/* Outputs a string representing this color in the format of [R:r G:g B:b]. */
	std::string toString() const;

	// Binary Assignment
	ColorF& operator+=(ColorF color);
	ColorF& operator-=(ColorF color);
	ColorF& operator*=(ColorF color);
	ColorF& operator/=(ColorF color);
	ColorF& operator+=(float uniform);
	ColorF& operator-=(float uniform);
	ColorF& operator*=(float uniform);
	ColorF& operator/=(float uniform);

	// Binary
	ColorF operator+(ColorF color) const;
	ColorF operator-(ColorF color) const;
	ColorF operator*(ColorF color) const;
	ColorF operator/(ColorF color) const;
	ColorF operator+(float uniform) const;
	ColorF operator-(float uniform) const;
	ColorF operator*(float uniform) const;
	ColorF operator/(float uniform) const;

	// Boolean
	bool operator==(ColorF color) const;
	bool operator!=(ColorF color) const;
	bool operator==(float uniform) const;
	bool operator!=(float uniform) const;

	#pragma endregion
};

#pragma endregion

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|