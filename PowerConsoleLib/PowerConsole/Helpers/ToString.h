/*=================================================================|
|* File:				ToString.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TO_STRING_H
#define TO_STRING_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Helpers/Parsing.h>
#include <limits>
#include <sstream>

namespace PowerConsole::Helpers {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ToString {
public:
	template<typename T> static std::string anyTypeToString(T value) {
		if (std::numeric_limits<T>().is_integer)
			return intTypeToString(value);
		else
			return floatTypeToString(value);
	}
	template<typename T> static std::string intTypeToString(T value, unsigned short radix = 10) {
		if (radix < 2)
			throw std::invalid_argument("Radix cannot be one or zero");
		if (radix > 36)
			throw std::invalid_argument("Radix cannot be greater than 36");
		bool negative = value < (T)0;
		if (negative)
			value *= -1;
		std::string str = "";
		while (value != (T)0) {
			T digit = value % (int)radix;
			char digitChar = digit + (digit <= (T)9 ? '0' : 'A' - 10);
			str.insert(0, 1, digitChar);
			value /= (int)radix;
		}
		if (str.empty())
			str = "0";
		if (negative)
			str.insert(0, "-");
		return str;
	}
	template<typename T> static std::string floatTypeToString(T value, unsigned int precision = 10) {
		std::ostringstream oss;
		oss.precision(precision);
		oss << value;
		return oss.str();
	}
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
