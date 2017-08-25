/*=================================================================|
|* File:				IniProperty.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "IniProperty.h"
#include <PowerConsole/IO/IniDocument.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

IniProperty::IniProperty(const std::string& key, const std::string& value, const std::string& comments, bool useQuotes) {
	this->key		= key;
	this->value		= value;
	this->comments	= comments;
	this->useQuotes	= useQuotes;

	if (!comments.empty()) {
		if (comments.back() != '\n')
			this->comments.append(1, '\n');
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Ini Properties
std::string IniProperty::getKey() const {
	return key;
}
std::string IniProperty::getComments() const {
	return comments;
}
void IniProperty::setComments(const std::string& comments) {
	this->comments = comments;
	if (!comments.empty()) {
		if (comments.back() != '\n')
			this->comments.append(1, '\n');
	}
}
std::string IniProperty::getValue() const {
	return value;
}
void IniProperty::setValue(const std::string& value) {
	this->value = value;
}

// Format
bool IniProperty::getUseQuotes() const {
	return useQuotes;
}
void IniProperty::setUseQuotes(bool useQuotes) {
	this->useQuotes = useQuotes;
}

#pragma endregion
//======== PROPERTY TYPES ========
#pragma region Properties

std::string IniProperty::getString() const {
	return value;
}
void IniProperty::setString(const std::string& value) {
	this->value = value;
}
int IniProperty::getInt(Radixes radix) const {
	try {
		return Parsing::parseIntType<int>(value, radix);
	}
	catch (...) {
		return 0;
	}
}
void IniProperty::setInt(int value, Radixes radix) {
	this->value = ToString::intTypeToString(value, radix);
}
float IniProperty::getFloat() const {
	try {
		return Parsing::parseFloatType<float>(value);
	}
	catch (...) {
		return 0.0f;
	}
}
void IniProperty::setFloat(float value, int precision) {
	this->value = ToString::floatTypeToString(value, precision);
}
bool IniProperty::getBool(const std::string& trueStr) const {
	return (_stricmp(value.c_str(), "true") == 0 || _stricmp(value.c_str(), "yes") == 0 || value == "1" ||
			(!trueStr.empty() && _stricmp(value.c_str(), trueStr.c_str()) == 0));
}
void IniProperty::setBool(bool value, const std::string& trueStr, const std::string& falseStr) {
	this->value = (value ? trueStr : falseStr);
}
Point2I IniProperty::getPoint2I() const {
	try {
		return Parsing::parsePointType<Point2I>(value);
	}
	catch (...) {
		return Point2I::ZERO;
	}
}
void IniProperty::setPoint2I(Point2I value, const std::string& open, const std::string& separator, const std::string& close) {
	this->value = open + ToString::intTypeToString(value.x) + separator + ToString::intTypeToString(value.y) + close;
}
Point2F IniProperty::getPoint2F() const {
	try {
		return Parsing::parsePointType<Point2F>(value);
	}
	catch (...) {
		return Point2I::ZERO;
	}
}
void IniProperty::setPoint2F(Point2F value, const std::string& open, const std::string& separator, const std::string& close, int precision) {
	this->value = open + ToString::floatTypeToString(value.x, precision) + separator + ToString::floatTypeToString(value.y, precision) + close;
}

#pragma endregion
//========= INPUT/OUTPUT =========
#pragma region Input/Output

std::string IniProperty::writeProperty(bool strictFormatting, bool escape, const std::string& assignmentType, const std::string& commentType, int maxLineLength) {
	std::string text = "";
	if (!comments.empty())
		text += IniDocument::formatComments(comments, commentType);
	text += IniDocument::formatProperty(key, value, strictFormatting, escape, assignmentType, maxLineLength, useQuotes) + "\n";
	return text;
}

#pragma endregion
//=================================================================|
