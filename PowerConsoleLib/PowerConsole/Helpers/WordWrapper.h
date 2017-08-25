/*=================================================================|
|* File:				WordWrapper.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef WORD_WRAPPER_H
#define WORD_WRAPPER_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Helpers/fixed_array.h>

namespace PowerConsole::Helpers {
//=================================================================|
// CLASSES														   |
//=================================================================/

struct WordWrapResult {
	fixed_array<int> lineStarts;
	fixed_array<int> lineLengths;

	int width;
	int lines;
};

class WordWrapper {
public:
	static WordWrapResult wrap(const std::string& text, int minLines, int minWidth, int maxWidth);

};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
