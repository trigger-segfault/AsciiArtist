/*=================================================================|
|* File:				StringHelpers.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <string>
#include <regex>
#include <vector>
#include <PowerConsole/Declarations.h>

namespace PowerConsole::Helpers {
//=================================================================|
// FUNCTIONS													   |
//=================================================================/

/* Converts a wstring to a regular string. */
std::string wstr_to_str(const std::wstring& ws);
/* Converts a regular string to a wstring. */
std::wstring str_to_wstr(const std::string& s);

std::string trimWhitespace(const std::string& str, const std::string& whitespace = " \t");

void replaceString(std::string& text, const std::string& search, const std::string& replace);
void replaceString(std::string& text, const std::regex& search, const std::string& replace);
std::string replaceString2(const std::string& text, const std::string& search, const std::string& replace);
std::string replaceString2(const std::string& text, const std::regex& search, const std::string& replace);

void splitString(const std::string& text, std::vector<std::string>& output, const std::string& delimiter);

void joinString(std::string& text, const std::vector<std::string>& input, const std::string& delimiter);
std::string joinString2(const std::vector<std::string>& input, const std::string& delimiter);

int searchString(const std::string& text, const std::string& search);
int searchString(const std::string& text, const std::regex& search);

bool compareString(const std::string& str1, const std::string& str2, bool caseSensitive);

std::string enquote(const std::string& text);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
