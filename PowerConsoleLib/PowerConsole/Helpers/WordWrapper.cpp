/*=================================================================|
|* File:				WordWrapper.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "WordWrapper.h"

#undef max

using namespace PowerConsole::Helpers;
//=================================================================|
// CLASSES														   |
//=================================================================/

WordWrapResult WordWrapper::wrap(const std::string& text, int minLines, int minWidth, int maxWidth) {


	std::vector<int> lineStarts;
	std::vector<int> lineLengths;
	int wordEnd = -1;
	int wordStart = -1;
	int lineStart = 0;
	int width;
	for (width = minWidth; width <= maxWidth; width++) {
		for (int i = 0; i < (int)text.length(); i++) {
			if ((int)lineStarts.size() > minLines && width < maxWidth)
				break;
			char c = text[i];
			if (c == '\n') {
				// Force end line
				lineStarts.push_back(lineStart);
				lineLengths.push_back(wordEnd == -1 ? 0 : (wordStart != 0 ? i : wordEnd) - lineStart);
				wordEnd = -1;
				wordStart = -1;
				lineStart = i + 1;
				continue;
			}
			else if (c == ' ') {
				if (wordStart != -1) {
					wordEnd = i;
					wordStart = -1;
				}
			}
			else {
				if (wordStart == -1) {
					wordStart = i;
					// Push back the line start if we can
					if (wordEnd == -1)
						lineStart = i;
				}
			}
			if (i - lineStart == width) {
				lineStarts.push_back(lineStart);
				if (wordEnd == -1) {
					// Force end line
					lineLengths.push_back(i - lineStart);
					lineStart = i;
					wordStart = -1;
				}
				else {
					// Gracefully wrap line
					lineLengths.push_back(wordEnd - lineStart);
					lineStart = (wordStart != -1 ? wordStart : i);
					wordEnd = -1;
				}
			}
		}
		// We haven't reached our desired dimensions yet. Reset and try again.
		if ((int)lineStarts.size() + (wordStart != -1 ? 1 : 0) > minLines && width < maxWidth) {
			lineStarts.clear();
			lineLengths.clear();
			wordEnd = -1;
			wordStart = -1;
			lineStart = 0;
		}
		else {
			// Finish off the final line
			lineStarts.push_back(lineStart);
			if (wordStart != -1)
				lineLengths.push_back((int)text.length() - lineStart);
			else if (wordEnd != -1)
				lineLengths.push_back(wordEnd - lineStart);
			else
				lineLengths.push_back(0);

			WordWrapResult result;
			result.lineStarts = fixed_array<int>(lineStarts);
			result.lineLengths = fixed_array<int>(lineLengths);
			result.lines = (int)lineStarts.size();
			result.width = 0;
			for (int length : lineLengths) {
				result.width = std::max(result.width, length);
			}
			return result;
		}
	}
	return WordWrapResult();
}

//=================================================================|
