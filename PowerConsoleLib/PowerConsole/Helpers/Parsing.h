/*=================================================================|
|* File:				Parsing.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PARSING_H
#define PARSING_H

#include <cmath>
#include <string>
#include <limits>
#include <exception>
#include <PowerConsole/Declarations.h>

namespace PowerConsole::Helpers {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum Radixes : unsigned short {
	RADIX_ANY = 0,
	RADIX_BINARY = 2,
	RADIX_OCTAL = 8,
	RADIX_DECIMAL = 10,
	RADIX_HEX = 16
};

class Parsing {
public:
	/* Gets the base of the number string based on the prefix. */
	static Radixes getRadixPrefix(const std::string& str);
	/* Gets the base of the number string based on the prefix and also sets the length of the prefix. */
	static Radixes getRadixPrefix(const std::string& str, int& prefixLength);

	/* Parses a point. */
	template<typename T> static T parsePointType(const std::string& str) {
		std::string edit = str;
		if (str.empty())
			throw std::invalid_argument("Input string is empty");

		if (str.front() == '(' || str.front() == '[' || str.front() == '{')
			edit = edit.substr(1, edit.length() - 1);
		if (str.back() == ')' || str.back() == ']' || str.back() == '}')
			edit = edit.substr(0, edit.length() - 1);
		auto it = std::find(edit.begin(), edit.end(), ',');
		if (it == edit.end())
			it = std::find(edit.begin(), edit.end(), 'x');
		if (it == edit.end())
			it = std::find(edit.begin(), edit.end(), 'X');
		if (it != edit.end()) {
			int position = std::distance(edit.begin(), it);
			return T(
				parseAnyType<T::type>(edit.substr(0, position)),
				parseAnyType<T::type>(edit.substr(position + 1))
			);
		}
		return T();
	}

	/* Parses an integer or floating type. */
	template<typename T> static T parseAnyType(const std::string& str) {
		if (std::numeric_limits<T>().is_integer)
			return parseIntType<T>(str);
		else
			return parseFloatType<T>(str);
	}

	/* Parses an integer type value. */
	template<typename T> static T parseIntType(const std::string& str, unsigned short radix = 10) {
		if (str.empty())
			throw std::invalid_argument("Input string is empty");
		if (radix == 1)
			throw std::invalid_argument("Numeric base cannot be one");
		if (radix > 36)
			throw std::invalid_argument("Numeric base cannot be greater than 36");
		if ((unsigned int)std::numeric_limits<T>().max() < ((unsigned int)radix + 1U))
			throw std::overflow_error("Single digit is larger than numeric base");

		int prefixLength = 0;
		if (radix == RADIX_ANY)
			radix = (unsigned short)getRadixPrefix(str, prefixLength);

		T value = 0;
		T digit;
		bool negative = str[prefixLength] == '-';

		if (negative && !std::numeric_limits<T>().is_signed)
			throw std::invalid_argument("Negative symbol with unsigned number");
		if (negative && str.length() == 1)
			throw std::invalid_argument("Negative symbol but no digits");

		for (int i = prefixLength + (negative ? 1 : 0); i < (int)str.length(); i++) {
			char c = str[i];
			if (c >= '0' && c <= '9')
				digit = (T)(c - '0');
			else if (c >= 'A' && c <= 'Z')
				digit = (T)(10 + c - 'A');
			else if (c >= 'a' && c <= 'z')
				digit = (T)(10 + c - 'a');
			else if (c == '-')
				throw std::invalid_argument("Negative symbol must be first character in string");
			else
				throw std::invalid_argument("Invalid character");

			if (digit >= radix)
				throw std::invalid_argument("Digit '" + std::string(1, c) + "' is greater than base");
			else if (!negative && (std::numeric_limits<T>().max() - digit) / (T)radix < value)
				throw std::overflow_error("Number is larger than the numeric type");
			else if (negative && (std::numeric_limits<T>().min() + digit) / (T)radix > value)
				throw std::overflow_error("Negative number is smaller than the numeric type");

			value *= (T)radix;
			if (negative)
				value -= digit;
			else
				value += digit;
		}
		return value;
	}

	/* Parses a floating type value. */
	template<typename T> static T parseFloatType(const std::string& str) {
		if (str.empty())
			throw std::invalid_argument("Input string is empty");

		T value = 0;
		T evalue = 0;
		T digit;
		bool e = false;
		bool hasDigits = false;
		bool negative = str[0] == '-'; // Also used for 'e'
		bool decimal = false; // Also used for 'e'
		int decimalPlace = 0; // Also used for 'e'

		for (int i = (negative ? 1 : 0); i < (int)str.length(); i++) {
			char c = str[i];
			if (c >= '0' && c <= '9') {
				digit = (T)(c - '0');
				hasDigits = true;
			}
			else if (c == '.') {
				if (decimal)
					throw std::invalid_argument("Decimal place symbol already in string");
				decimal = true;
				continue;
			}
			else if (c == 'E' || c == 'e') {
				if (e)
					throw std::invalid_argument("'e' symbol already in string");
				else if (!hasDigits)
					throw std::invalid_argument("No digits before 'e' symbol");

				e = true;
				negative = false;
				decimal = false;
				decimalPlace = 0;

				if (str[i + 1] == '-') {
					negative = true;
					i++;
				}
				continue;
			}
			else if (c == '-')
				throw std::invalid_argument("Negative symbol must be first character in string");
			else
				throw std::invalid_argument("Invalid character");

			#define evalue_or_value (e ? evalue : value)
			if (decimal) {
				decimalPlace++;
				digit /= (T)10 * (T)decimalPlace;
				if (negative)
					evalue_or_value -= digit;
				else
					evalue_or_value += digit;
			}
			else {
				evalue_or_value *= (T)10;
				if (negative)
					evalue_or_value -= digit;
				else
					evalue_or_value += digit;
			}
		}
		if (e) {
			if (!hasDigits)
				throw std::invalid_argument("No digits after 'e' symbol");
			value *= (T)std::pow((T)10, evalue);
		}
		else if (!hasDigits) {
			throw std::invalid_argument("No digits in number");
		}
		return value;
	}
};



//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
