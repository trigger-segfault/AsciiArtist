/*=================================================================|
|* File:				Helpers.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef HELPERS_H
#define HELPERS_H

#include <PowerConsole/Declarations.h>
#include <string>

namespace AsciiTetris::Helpers {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Creates a string based on the large number with commas and postfixes such as 'K' or 'M'. */
std::string largeIntToString(unsigned long long value, int maxLength);
/* Creates a string based on the large number with commas and postfixes such as 'K' or 'M'. */
std::string largeIntToString(int value, int maxLength);

/* Creates a string based on the number of ticks that have passed. */
std::string playTimeToString(int playTime);
/* Parses a playtime string. */
int parsePlayTime(const std::string& str);

/* Used for hash functions. */
template <class T> void hash_combine(std::size_t & s, const T & v) {
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
