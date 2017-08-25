/*=================================================================|
|* File:				Parsing.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Parsing.h"

using namespace PowerConsole::Helpers;
//=================================================================|
// CLASSES														   |
//=================================================================/

Radixes Parsing::getRadixPrefix(const std::string& str) {
	if (str.length() >= 1 && str[0] == '0') {
		if (str.length() >= 2) {
			if (str[1] == 'X' || str[1] == 'x')
				return Radixes::RADIX_HEX;
			else if (str[1] == 'B' || str[1] == 'b')
				return Radixes::RADIX_BINARY;
		}
		return Radixes::RADIX_OCTAL;
	}
	return Radixes::RADIX_DECIMAL;
}
Radixes Parsing::getRadixPrefix(const std::string& str, int& prefixLength) {
	if (str.length() >= 1 && str[0] == '0') {
		if (str.length() >= 2) {
			if (str[1] == 'X' || str[1] == 'x') {
				prefixLength = 2;
				return Radixes::RADIX_HEX;
			}
			else if (str[1] == 'B' || str[1] == 'b') {
				prefixLength = 2;
				return Radixes::RADIX_BINARY;
			}
		}
		prefixLength = 1;
		return Radixes::RADIX_OCTAL;
	}
	prefixLength = 0;
	return Radixes::RADIX_DECIMAL;
}

//=================================================================|
