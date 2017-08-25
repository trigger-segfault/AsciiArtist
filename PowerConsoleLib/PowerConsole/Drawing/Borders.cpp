/*=================================================================|
|* File:				Borders.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Borders.h"
#include <PowerConsole/Helpers/Flags.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

BorderConnection::BorderConnection(unsigned char character, int left, int top, int right, int bottom)
 :	character(character),
	left((BorderThickness)left), top((BorderThickness)top),
	right((BorderThickness)right), bottom((BorderThickness)bottom) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

BorderConnection::operator unsigned char() const {
	return character;
}

#pragma endregion
//=========== TESTING ============
#pragma region Testing

bool BorderConnection::matches(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom) {
	return	(this->left == left && this->right == right) &&
			(this->top == top && this->bottom == bottom);
}

#pragma endregion

#define borderCase(character, left, top, right, bottom) case character: return BorderConnection(character, left, top, right, bottom)

BorderConnection Borders::getBorderConnection(unsigned char character) {
	switch (character) {
		#pragma region Thin

		// Horizontal/Vertical
		borderCase(LINE_THIN_HORIZONTAL, 1, 0, 1, 0);
		borderCase(LINE_THIN_VERTICAL, 0, 1, 0, 1);

		// Corners
		borderCase(CORNER_THIN_TOP_THIN_LEFT, 1, 1, 0, 0);
		borderCase(CORNER_THIN_TOP_THIN_RIGHT, 0, 1, 1, 0);
		borderCase(CORNER_THIN_BOTTOM_THIN_RIGHT, 0, 0, 1, 1);
		borderCase(CORNER_THIN_BOTTOM_THIN_LEFT, 1, 0, 0, 1);

		// Threeways
		borderCase(THREEWAY_THIN_VERTICAL_THIN_LEFT, 1, 1, 0, 1);
		borderCase(THREEWAY_THIN_TOP_THIN_HORIZONTAL, 1, 1, 1, 0);
		borderCase(THREEWAY_THIN_VERTICAL_THIN_RIGHT, 0, 1, 1, 1);
		borderCase(THREEWAY_THIN_BOTTOM_THIN_HORIZONTAL, 1, 0, 1, 1);

		// Fourway
		borderCase(FOURWAY_THIN_VERTICAL_THIN_HORIZONTAL, 1, 1, 1, 1);

		#pragma endregion
		//--------------------------------
		#pragma region Thick

		// Horizontal/Vertical
		borderCase(LINE_THICK_HORIZONTAL, 2, 0, 2, 0);
		borderCase(LINE_THICK_VERTICAL, 0, 2, 0, 2);

		// Corners
		borderCase(CORNER_THICK_TOP_THICK_LEFT, 2, 2, 0, 0);
		borderCase(CORNER_THICK_TOP_THICK_RIGHT, 0, 2, 2, 0);
		borderCase(CORNER_THICK_BOTTOM_THICK_RIGHT, 0, 0, 2, 2);
		borderCase(CORNER_THICK_BOTTOM_THICK_LEFT, 2, 0, 0, 2);

		// Threeways
		borderCase(THREEWAY_THICK_VERTICAL_THICK_LEFT, 2, 2, 0, 2);
		borderCase(THREEWAY_THICK_TOP_THICK_HORIZONTAL, 2, 2, 2, 0);
		borderCase(THREEWAY_THICK_VERTICAL_THICK_RIGHT, 0, 2, 2, 2);
		borderCase(THREEWAY_THICK_BOTTOM_THICK_HORIZONTAL, 2, 0, 2, 2);

		// Fourway
		borderCase(FOURWAY_THICK_VERTICAL_THICK_HORIZONTAL, 2, 2, 2, 2);

		#pragma endregion
		//--------------------------------
		#pragma region Thin Horizontal, Thick Vertical

		// Corners
		borderCase(CORNER_THICK_TOP_THIN_RIGHT, 1, 2, 0, 0);
		borderCase(CORNER_THICK_TOP_THIN_LEFT, 0, 2, 1, 0);
		borderCase(CORNER_THICK_BOTTOM_THIN_RIGHT, 0, 0, 1, 2);
		borderCase(CORNER_THICK_BOTTOM_THIN_LEFT, 1, 0, 0, 2);

		// Threeways
		borderCase(THREEWAY_THICK_VERTICAL_THIN_LEFT, 1, 2, 0, 2);
		borderCase(THREEWAY_THICK_TOP_THIN_HORIZONTAL, 1, 2, 1, 0);
		borderCase(THREEWAY_THICK_VERTICAL_THIN_RIGHT, 0, 2, 1, 2);
		borderCase(THREEWAY_THICK_BOTTOM_THIN_HORIZONTAL, 1, 0, 1, 2);

		// Fourways
		borderCase(FOURWAY_THICK_VERTICAL_THIN_HORIZONTAL, 1, 2, 1, 2);

		#pragma endregion
		//--------------------------------
		#pragma region Thick Horizontal, Thin Vertical

		// Corners
		borderCase(CORNER_THIN_TOP_THICK_RIGHT, 2, 1, 0, 0);
		borderCase(CORNER_THIN_TOP_THICK_LEFT, 0, 1, 2, 0);
		borderCase(CORNER_THIN_BOTTOM_THICK_RIGHT, 0, 0, 2, 1);
		borderCase(CORNER_THIN_BOTTOM_THICK_LEFT, 2, 0, 0, 1);

		// Threeways
		borderCase(THREEWAY_THIN_VERTICAL_THICK_LEFT, 2, 1, 0, 1);
		borderCase(THREEWAY_THIN_TOP_THICK_HORIZONTAL, 2, 1, 2, 0);
		borderCase(THREEWAY_THIN_VERTICAL_THICK_RIGHT, 0, 1, 2, 1);
		borderCase(THREEWAY_THIN_BOTTOM_THICK_HORIZONTAL, 2, 0, 2, 1);

		// Fourways
		borderCase(FOURWAY_THIN_VERTICAL_THICK_HORIZONTAL, 2, 1, 2, 1);

		#pragma endregion
		//--------------------------------
		#pragma region Default

	default: return BorderConnection('\0', 0, 0, 0, 0);

		#pragma endregion
	}
}

unsigned char Borders::getBestConnection(Point2I point, AsciiImageFrame& image, ConnectionPriorities priorities) {
	return getBestConnection(
		image.getChar(point + Point2I(-1, 0)),
		image.getChar(point + Point2I(0, -1)),
		image.getChar(point + Point2I(1, 0)),
		image.getChar(point + Point2I(0, 1)),
		false, priorities
	);
}


unsigned char Borders::getBestConnection(unsigned char left, unsigned char top, unsigned char right, unsigned char bottom, bool asThickness, ConnectionPriorities priorities) {
	// Convert the characters into thicknesses
	if (!asThickness) {
		left = getBorderConnection(left).right;
		top = getBorderConnection(top).bottom;
		right = getBorderConnection(right).left;
		bottom = getBorderConnection(bottom).top;
	}
	else {
		if (left   < 0 || left   > 2) left   = 0;
		if (top    < 0 || top    > 2) top    = 0;
		if (right  < 0 || right  > 2) right  = 0;
		if (bottom < 0 || bottom > 2) bottom = 0;
	}
	
	// Check if a character that fits these criteria exists.
	if ((left == 0 || right == 0 || left == right) && (top == 0 || bottom == 0 || top == bottom)) {
		for (unsigned char i = RANGE_START; i <= RANGE_END; i++) {
			if (getBorderConnection(i).matches(left, top, right, bottom)) {
				return i;
			}
		}
	}
	else if (getFlag(priorities, ConnectionPriorities::DontReturnBadMatch)) {
		return '\0';
	}
	else {
		bool dontReturnBadMatch = getFlag(priorities, ConnectionPriorities::DontReturnBadMatch);
		bool thinBeforeThick = getFlag(priorities, ConnectionPriorities::ThinBeforeThick);
		bool dontMatchThickness = getFlag(priorities, ConnectionPriorities::DontMatchThickness);
		bool connectionCountFirst = getFlag(priorities, ConnectionPriorities::ConnectionCountFirst);
		bool allowOpenConnections = getFlag(priorities, ConnectionPriorities::AllowOpenConnections);
		bool dontMatchThicknessIfNoSolution = getFlag(priorities, ConnectionPriorities::DontMatchThicknessIfNoSolution);

		int totalConnections =	(left != 0 ? 1 : 0) + (right != 0 ? 1 : 0) +
								(top != 0 ? 1 : 0) + (bottom != 0 ? 1 : 0);
		int totalThinConnections =	(left == 1 ? 1 : 0) + (right == 1 ? 1 : 0) +
									(top == 1 ? 1 : 0) + (bottom == 1 ? 1 : 0);
		int totalThickConnections = (left == 2 ? 1 : 0) + (right == 2 ? 1 : 0) +
									(top == 2 ? 1 : 0) + (bottom == 2 ? 1 : 0);
		bool thicknessPriority = (totalThinConnections > totalThickConnections && thinBeforeThick) ||
								 (totalThinConnections < totalThickConnections && !thinBeforeThick);

		// We think we're superior to everyone and don't want a bad match
		if (dontReturnBadMatch) {
			return '\0';
		}

		// Lets find the best possible match
		if (totalConnections == 1) {
			//      
			//   +--
			//      
			return '\0';
		}
		else if (totalConnections == 2) {
			//      
			// --+==
			//      
			return '\0';
		}
		else if (totalConnections == 3) {
			//   |  
			// --+==
			//      
			unsigned char thickness = (totalThinConnections == 2 ? BORDER_THIN : BORDER_THICK);
			if (left == thickness) {
				if (top == thickness)
					return (thickness == BORDER_THIN ? CORNER_THIN_TOP_THIN_LEFT : CORNER_THICK_TOP_THICK_LEFT);
				else // bottom == thickness
					return (thickness == BORDER_THIN ? CORNER_THIN_BOTTOM_THIN_LEFT : CORNER_THICK_BOTTOM_THICK_LEFT);
			}
			else { // right == thickness
				if (top == thickness)
					return (thickness == BORDER_THIN ? CORNER_THIN_TOP_THIN_RIGHT : CORNER_THICK_TOP_THICK_RIGHT);
				else // bottom == thickness
					return (thickness == BORDER_THIN ? CORNER_THIN_BOTTOM_THIN_RIGHT : CORNER_THICK_BOTTOM_THICK_RIGHT);
			}
		}
		else { // totalConnections == 4
			//   |     |  
			// --+== --+==
			//   |     =  
			if (totalThinConnections == 3 || totalThickConnections == 3) {
				//   |  
				// --+==
				//   |  
				unsigned char thickness = (totalThinConnections == 3 ? BORDER_THIN : BORDER_THICK);
				if (left != right) {
					if (left != thickness)
						return (thickness == BORDER_THIN ? THREEWAY_THIN_VERTICAL_THIN_RIGHT : THREEWAY_THICK_VERTICAL_THICK_RIGHT);
					else
						return (thickness == BORDER_THIN ? THREEWAY_THIN_VERTICAL_THIN_LEFT : THREEWAY_THICK_VERTICAL_THICK_LEFT);
				}
				else { // top != bottom
					if (top != thickness)
						return (thickness == BORDER_THIN ? THREEWAY_THIN_BOTTOM_THIN_HORIZONTAL : THREEWAY_THICK_BOTTOM_THICK_HORIZONTAL);
					else
						return (thickness == BORDER_THIN ? THREEWAY_THIN_TOP_THIN_HORIZONTAL : THREEWAY_THICK_TOP_THICK_HORIZONTAL);
				}
			}
			else {
				//   |  
				// --+==
				//   =  
				unsigned char thickness = (thinBeforeThick ? BORDER_THIN : BORDER_THICK);
				if (left == thickness) {
					if (top == thickness)
						return (thickness == BORDER_THIN ? CORNER_THIN_TOP_THIN_LEFT : CORNER_THICK_TOP_THICK_LEFT);
					else // bottom == thickness
						return (thickness == BORDER_THIN ? CORNER_THIN_BOTTOM_THIN_LEFT : CORNER_THICK_BOTTOM_THICK_LEFT);
				}
				else { // right == thickness
					if (top == thickness)
						return (thickness == BORDER_THIN ? CORNER_THIN_TOP_THIN_RIGHT : CORNER_THICK_TOP_THICK_RIGHT);
					else // bottom == thickness
						return (thickness == BORDER_THIN ? CORNER_THIN_BOTTOM_THIN_RIGHT : CORNER_THICK_BOTTOM_THICK_RIGHT);
				}
			}
		}
	}
	return '\0';
}

//=================================================================|
