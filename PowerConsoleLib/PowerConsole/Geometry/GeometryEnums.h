/*=================================================================|
|* File:				GeometryEnums.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef GEOMETRY_ENUMS_H
#define GEOMETRY_ENUMS_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole::Geometry {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Alignment for horizontal directions. */
enum class HorizontalAlignments {
	/* Aligned to the left. */
	Left,
	/* Aligned in the center. */
	Center,
	/* Aligned to the right. */
	Right,
	/* Aligned from the left to the right. */
	Stretch
};
/* Alignment for vertical directions. */
enum class VerticalAlignments {
	/* Aligned to the top. */
	Top,
	/* Aligned in the center. */
	Center,
	/* Aligned to the bottom. */
	Bottom,
	/* Aligned from the top to the bottom. */
	Stretch
};

/* The x and y axis. */
enum class Orientations {
	/* The x axis. */
	Horizontal = 0,
	/* The x axis. */
	X = 0,
	/* The y axis. */
	Vertical = 1,
	/* The y axis. */
	Y = 1
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
