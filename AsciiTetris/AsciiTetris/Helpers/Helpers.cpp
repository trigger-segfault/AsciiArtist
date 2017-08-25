/*=================================================================|
|* File:				Helpers.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Helpers.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Helpers/Parsing.h>

using namespace PowerConsole::Helpers;
using namespace PowerConsole::Geometry;

using namespace AsciiTetris::Helpers;
//=================================================================|
// CLASSES														   |
//=================================================================/

std::string AsciiTetris::Helpers::largeIntToString(unsigned long long value, int maxLength) {
	if (maxLength < 4)
		throw std::invalid_argument("Max length cannot be less than 4.");

	std::string valueStr = std::to_string(value);
	int digits = (int)std::log10(value == 0 ? 1 : value) + 1;

	// Insert the comma delimiters
	for (int i = 0; i < (digits - 1) / 3; i++) {
		valueStr.insert(digits - (i + 1) * 3, ",");
	}

	if ((int)valueStr.length() > maxLength) {
		int topDigits = ((digits - 1) % 3) + 1;
		int extraThousandsPlaces = (maxLength - (topDigits + 1)) / 4;
		int newLength = (extraThousandsPlaces * 4) + topDigits;
		int unitPrefix = GMath::clamp((digits - topDigits) / 3 - extraThousandsPlaces, 0, 6);

		const std::string unitPrefixes[7] = { "", "K", "M", "B", "T", "P", "E" };

		valueStr = valueStr.substr(0, newLength);
		valueStr += unitPrefixes[unitPrefix];
	}
	return valueStr;
}
std::string AsciiTetris::Helpers::largeIntToString(int value, int maxLength) {
	return largeIntToString((unsigned long long)value, maxLength);
}

std::string AsciiTetris::Helpers::playTimeToString(int playTime) {
	int sec = playTime / 60;
	int min = sec / 60;
	int hour = min / 60;
	min %= 60;
	sec %= 60;

	return
		(hour < 10 ? "0" : "") + std::to_string(hour) + ":" +
		(min  < 10 ? "0" : "") + std::to_string(min)  + ":" +
		(sec  < 10 ? "0" : "") + std::to_string(sec);
}
int AsciiTetris::Helpers::parsePlayTime(const std::string& str) {
	std::vector<std::string> split;
	splitString(str, split, ":");
	if (split.size() == 3) {
		try {
			int playTime = 0;
			playTime += Parsing::parseIntType<int>(split[0]) * 60 * 60 * 60;
			playTime += Parsing::parseIntType<int>(split[1]) * 60 * 60;
			playTime += Parsing::parseIntType<int>(split[2]) * 60;
			return playTime;
		}
		catch (...) {}
	}
	return 0;
}

//=================================================================|
