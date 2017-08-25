/*=================================================================|
|* File:				Blank.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "StringHelpers.h"

using namespace PowerConsole::Helpers;
//=================================================================|
// FUNCTIONS													   |
//=================================================================/

std::string PcHelp::wstr_to_str(const std::wstring& ws) {
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.to_bytes(ws);
}
std::wstring PcHelp::str_to_wstr(const std::string& s) {
	return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(s);
}

std::string PcHelp::trimWhitespace(const std::string& str, const std::string& whitespace) {
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

// http://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-a-string-with-another-string
void PcHelp::replaceString(std::string& text, const std::string& search, const std::string& replace) {
	if (!search.empty()) {
		size_t pos = 0;
		while ((pos = text.find(search, pos)) != std::string::npos) {
			text.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}
}
std::string PcHelp::replaceString2(const std::string& text, const std::string& search, const std::string& replace) {
	std::string output = text;
	if (!search.empty()) {
		size_t pos = 0;
		while ((pos = text.find(search, pos)) != std::string::npos) {
			output.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}
	return output;
}
void PcHelp::replaceString(std::string& text, const std::regex& search, const std::string& replace) {
	text = std::regex_replace(text, search, replace);
}
std::string PcHelp::replaceString2(const std::string& text, const std::regex& search, const std::string& replace) {
	return std::regex_replace(text, search, replace);
}

void PcHelp::splitString(const std::string& text, std::vector<std::string>& output, const std::string& delimiter) {
	output.clear();

	std::string::size_type lastPos = 0;
	std::string::size_type pos = text.find(delimiter, 0);

	while (std::string::npos != pos) {
		output.push_back(text.substr(lastPos, pos - lastPos));
		lastPos = pos + delimiter.length();
		pos = text.find_first_of(delimiter, lastPos);
	}
	output.push_back(text.substr(lastPos, text.length() - lastPos));
}

void PcHelp::joinString(std::string& text, const std::vector<std::string>& input, const std::string& delimiter) {
	text.clear();
	for (int i = 0; i < (int)input.size(); i++) {
		text += input[i] + (i + 1 < (int)input.size() ? delimiter : "");
	}
}
std::string PcHelp::joinString2(const std::vector<std::string>& input, const std::string& delimiter) {
	std::string output = "";
	for (int i = 0; i < (int)input.size(); i++) {
		output += input[i] + (i + 1 < (int)input.size() ? delimiter : "");
	}
	return output;
}

int PcHelp::searchString(const std::string& text, const std::string& search) {
	return (int)text.find(search);
}
int PcHelp::searchString(const std::string& text, const std::regex& search) {
	std::smatch match;
	if (std::regex_search(text, match, search) && !match.empty())
		return (int)match.position(0);
	return -1;
}

bool PcHelp::compareString(const std::string& str1, const std::string& str2, bool caseSensitive) {
	if (!caseSensitive)
		return _stricmp(str1.c_str(), str2.c_str()) == 0;
	return str1 == str2;
}

std::string PcHelp::enquote(const std::string& text) {
	return "\"" + text + "\"";
}

//=================================================================|
