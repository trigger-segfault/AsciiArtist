/*=================================================================|
|* File:				Borders.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef BORDERS_H
#define BORDERS_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiImageFrame.h>

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum BorderThickness : unsigned char {
	BORDER_NONE = 0,
	BORDER_THIN = 1,
	BORDER_THICK = 2,
	BORDER_BOTH = 3
};

struct BorderConnection {

	//=========== MEMBERS ============
	#pragma region Members

	/* The character that represents this type of border. */
	unsigned char character;
	/* The left border thickness. */
	BorderThickness left;
	/* The top border thickness. */
	BorderThickness top;
	/* The right border thickness. */
	BorderThickness right;
	/* The bottom border thickness. */
	BorderThickness bottom;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs a border thickness. */
	BorderConnection(unsigned char character, int left, int top, int right, int bottom);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	operator unsigned char() const;

	#pragma endregion
	//=========== TESTING ============
	#pragma region Testing

	/* Returns true if this border matches the border thickness in each direction. */
	bool matches(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom);

	#pragma endregion
};

enum class ConnectionPriorities : unsigned short {
	ThickBeforeThin = 0x0000,
	ThinBeforeThick = 0x0001,
	ConnectionCountFirst = 0x0002,
	DontMatchThickness = 0x0004,
	DontMatchThicknessIfNoSolution = 0x0008,
	AllowOpenConnections = 0x0010,
	DontReturnBadMatch = 0x0020
};

DEFINE_ENUM_FLAG_OPERATORS(ConnectionPriorities)

class Borders {
public:
	//========== CONSTANTS ===========
	#pragma region Constants
	//--------------------------------
	#pragma region Thin

	// Horizontal/Vertical
	/* 196: (1, 0, 1, 0) */
	static const unsigned char LINE_THIN_HORIZONTAL = 196;
	/* 179: (0, 1, 0, 1) */
	static const unsigned char LINE_THIN_VERTICAL = 179;

	// Corners
	/* 217: (1, 1, 0, 0) */
	static const unsigned char CORNER_THIN_TOP_THIN_LEFT = 217;
	/* 192: (0, 1, 1, 0) */
	static const unsigned char CORNER_THIN_TOP_THIN_RIGHT = 192;
	/* 218: (0, 0, 1, 1) */
	static const unsigned char CORNER_THIN_BOTTOM_THIN_RIGHT = 218;
	/* 191: (1, 0, 0, 1) */
	static const unsigned char CORNER_THIN_BOTTOM_THIN_LEFT = 191;

	// Threeways
	/* 180: (1, 1, 0, 1) */
	static const unsigned char THREEWAY_THIN_VERTICAL_THIN_LEFT = 180;
	/* 193: (1, 1, 1, 0) */
	static const unsigned char THREEWAY_THIN_TOP_THIN_HORIZONTAL = 193;
	/* 195: (0, 1, 1, 1) */
	static const unsigned char THREEWAY_THIN_VERTICAL_THIN_RIGHT = 195;
	/* 194: (1, 0, 1, 1) */
	static const unsigned char THREEWAY_THIN_BOTTOM_THIN_HORIZONTAL = 194;

	// Fourway
	/* 197: (1, 1, 1, 1) */
	static const unsigned char FOURWAY_THIN_VERTICAL_THIN_HORIZONTAL = 197;

	#pragma endregion
	//--------------------------------
	#pragma region Thick

	// Horizontal/Vertical
	/* 205: (2, 0, 2, 0) */
	static const unsigned char LINE_THICK_HORIZONTAL = 205;
	/* 186: (0, 2, 0, 2) */
	static const unsigned char LINE_THICK_VERTICAL = 186;

	// Corners
	/* 188: (2, 2, 0, 0) */
	static const unsigned char CORNER_THICK_TOP_THICK_LEFT = 188;
	/* 200: (0, 2, 2, 0) */
	static const unsigned char CORNER_THICK_TOP_THICK_RIGHT = 200;
	/* 201: (0, 0, 2, 2) */
	static const unsigned char CORNER_THICK_BOTTOM_THICK_RIGHT = 201;
	/* 187: (2, 0, 0, 2) */
	static const unsigned char CORNER_THICK_BOTTOM_THICK_LEFT = 187;

	// Threeways
	/* 185: (2, 2, 0, 2) */
	static const unsigned char THREEWAY_THICK_VERTICAL_THICK_LEFT = 185;
	/* 202: (2, 2, 2, 0) */
	static const unsigned char THREEWAY_THICK_TOP_THICK_HORIZONTAL = 202;
	/* 204: (0, 2, 2, 2) */
	static const unsigned char THREEWAY_THICK_VERTICAL_THICK_RIGHT = 204;
	/* 203: (2, 0, 2, 2) */
	static const unsigned char THREEWAY_THICK_BOTTOM_THICK_HORIZONTAL = 203;

	// Fourway
	/* 206: (2, 2, 2, 2) */
	static const unsigned char FOURWAY_THICK_VERTICAL_THICK_HORIZONTAL = 206;

	#pragma endregion
	//--------------------------------
	#pragma region Thin Horizontal, Thick Vertical

	// Corners
	/* 189: (1, 2, 0, 0) */
	static const unsigned char CORNER_THICK_TOP_THIN_RIGHT = 189;
	/* 211: (0, 2, 1, 0) */
	static const unsigned char CORNER_THICK_TOP_THIN_LEFT = 211;
	/* 214: (0, 0, 1, 2) */
	static const unsigned char CORNER_THICK_BOTTOM_THIN_RIGHT = 214;
	/* 183: (1, 0, 0, 2) */
	static const unsigned char CORNER_THICK_BOTTOM_THIN_LEFT = 183;

	// Threeways
	/* 182: (1, 2, 0, 2) */
	static const unsigned char THREEWAY_THICK_VERTICAL_THIN_LEFT = 182;
	/* 208: (1, 2, 1, 0) */
	static const unsigned char THREEWAY_THICK_TOP_THIN_HORIZONTAL = 208;
	/* 199: (0, 2, 1, 2) */
	static const unsigned char THREEWAY_THICK_VERTICAL_THIN_RIGHT = 199;
	/* 210: (1, 0, 1, 2) */
	static const unsigned char THREEWAY_THICK_BOTTOM_THIN_HORIZONTAL = 210;

	// Fourway
	/* 215: (1, 2, 1, 2) */
	static const unsigned char FOURWAY_THICK_VERTICAL_THIN_HORIZONTAL = 215;

	#pragma endregion
	//--------------------------------
	#pragma region Thick Horizontal, Thin Vertical

	// Corners
	/* 190: (2, 1, 0, 0) */
	static const unsigned char CORNER_THIN_TOP_THICK_RIGHT = 190;
	/* 212 (0, 1, 2, 0) */
	static const unsigned char CORNER_THIN_TOP_THICK_LEFT = 212;
	/* 213: (0, 0, 2, 1) */
	static const unsigned char CORNER_THIN_BOTTOM_THICK_RIGHT = 213;
	/* 184: (2, 0, 0, 1) */
	static const unsigned char CORNER_THIN_BOTTOM_THICK_LEFT = 184;

	// Threeways
	/* 181: (2, 1, 0, 1) */
	static const unsigned char THREEWAY_THIN_VERTICAL_THICK_LEFT = 181;
	/* 207: (2, 1, 2, 0) */
	static const unsigned char THREEWAY_THIN_TOP_THICK_HORIZONTAL = 207;
	/* 198: (0, 1, 2, 1) */
	static const unsigned char THREEWAY_THIN_VERTICAL_THICK_RIGHT = 198;
	/* 209: (2, 0, 2, 1) */
	static const unsigned char THREEWAY_THIN_BOTTOM_THICK_HORIZONTAL = 209;

	// Fourway
	/* 216: (2, 1, 2, 1) */
	static const unsigned char FOURWAY_THIN_VERTICAL_THICK_HORIZONTAL = 216;

	#pragma endregion
	//--------------------------------
	#pragma region Ranges

	/* The starting range of border characters for an unsigned char. */
	static const unsigned char RANGE_START = 179;
	/* The ending range of border characters for an unsigned char. This character code is included. */
	static const unsigned char RANGE_END = 218;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONNECTIONS ==========
	#pragma region Connections

	static unsigned char getBestConnection(PcGeo::Point2I point, AsciiImageFrame& image, ConnectionPriorities priorities);
	static unsigned char getBestConnection(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom, bool asThickness, ConnectionPriorities priorities);
	/* Gets the border connection details from the specified character. */
	static BorderConnection getBorderConnection(unsigned char character);

	#pragma endregion
};



//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
