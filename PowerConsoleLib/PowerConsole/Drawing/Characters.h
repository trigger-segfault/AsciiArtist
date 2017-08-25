/*=================================================================|
|* File:				Characters.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole::Drawing {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Characters {
public:

	static const unsigned char BIG_ARROW_LEFT = 17;
	static const unsigned char BIG_ARROW_UP = 30;
	static const unsigned char BIG_ARROW_RIGHT = 16;
	static const unsigned char BIG_ARROW_DOWN = 31;

	static const unsigned char SMALL_ARROW_LEFT = 27;
	static const unsigned char SMALL_ARROW_UP = 24;
	static const unsigned char SMALL_ARROW_RIGHT = 26;
	static const unsigned char SMALL_ARROW_DOWN = 25;
	static const unsigned char SMALL_ARROW_VERTICAL = 18;
	static const unsigned char SMALL_ARROW_HORIZONTAL = 29;
	
	static const unsigned char BLOCK_FULL = 219;
	static const unsigned char BLOCK_LEFT_HALF = 221;
	static const unsigned char BLOCK_TOP_HALF = 223;
	static const unsigned char BLOCK_RIGHT_HALF = 222;
	static const unsigned char BLOCK_BOTTOM_HALF = 220;

	static const unsigned char GRADIENT_LIGHT = 176;
	static const unsigned char GRADIENT_MEDIUM = 177;
	static const unsigned char GRADIENT_HEAVY = 178;


	static const unsigned char SUIT_HEART = 3;
	static const unsigned char SUIT_DIAMOND = 4;
	static const unsigned char SUIT_CLUB = 5;
	static const unsigned char SUIT_SPADE = 6;

	static const unsigned char SMILEY_OUTLINE = 1;
	static const unsigned char SMILEY_FILL = 2;

	static const unsigned char GENDER_MALE = 11;
	static const unsigned char GENDER_FEMALE = 12;
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
