/*=================================================================|
|* File:				IniSection.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "IniSection.h"
#include <PowerConsole/IO/IniDocument.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::IO;

#define getOrReturnDefault(valueGetter) auto property = get(key); \
	if (property != nullptr) return property->valueGetter; \
	return defaultValue

//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

IniSection::IniSection(const std::string& key, const std::string& comments) {
	this->key		= key;
	this->comments	= comments;

	if (!comments.empty()) {
		if (comments.back() != '\n')
			this->comments.append(1, '\n');
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Ini Properties
std::string IniSection::getKey() const {
	return key;
}
std::string IniSection::getComments() const {
	return comments;
}
void IniSection::setComments(const std::string& comments) {
	this->comments = comments;
	if (!comments.empty()) {
		if (comments.back() != '\n')
			this->comments.append(1, '\n');
	}
}
std::vector<IniPropertySPtr>& IniSection::getProperties() {
	return properties;
}
const std::vector<IniPropertySPtr>& IniSection::getProperties() const {
	return properties;
}

#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

IniPropertySPtr IniSection::get(const std::string& key) {
	auto it = std::find_if(properties.begin(), properties.end(), [key](IniPropertySPtr property) {
		return _stricmp(property->getKey().c_str(), key.c_str()) == 0;
	});
	if (it != properties.end())
		return *it;
	return nullptr;
}
const IniPropertySPtr IniSection::get(const std::string& key) const {
	auto it = std::find_if(properties.begin(), properties.end(), [key](IniPropertySPtr property) {
		return _stricmp(property->getKey().c_str(), key.c_str()) == 0;
	});
	if (it != properties.end())
		return *it;
	return nullptr;
}
IniPropertySPtr IniSection::add(IniPropertySPtr property) {
	auto it = std::find(properties.begin(), properties.end(), property);
	if (it == properties.end()) {
		properties.push_back(property);
		return property;
	}
	return nullptr;
}
IniPropertySPtr IniSection::add(const std::string& key, const std::string& value, const std::string& comments, bool useQuotes) {
	IniPropertySPtr property = get(key);
	if (property == nullptr) {
		property = makeIniProperty(key, value, comments, useQuotes);
		properties.push_back(property);
		return property;
	}
	else {
		return property;
	}
}
void IniSection::remove(IniPropertySPtr property) {
	auto it = std::find(properties.begin(), properties.end(), property);
	if (it != properties.end())
		properties.erase(it);
}
void IniSection::remove(const std::string& key) {
	auto it = std::find_if(properties.begin(), properties.end(), [key](IniPropertySPtr property) {
		return _stricmp(property->getKey().c_str(), key.c_str()) == 0;
	});
	if (it != properties.end())
		properties.erase(it);
}
bool IniSection::contains(IniPropertySPtr property) const {
	return std::find(properties.begin(), properties.end(), property) != properties.end();
}
bool IniSection::contains(const std::string& key) const {
	return std::find_if(properties.begin(), properties.end(), [key](IniPropertySPtr property) {
		return _stricmp(property->getKey().c_str(), key.c_str()) == 0;
	}) != properties.end();
}
void IniSection::IniSection::clear() {
	properties.clear();
}
int IniSection::count() const {
	return (int)properties.size();
}

#pragma endregion
//======== PROPERTY TYPES ========
#pragma region Properties

std::string IniSection::getPropertyComments(const std::string& key, const std::string& defaultComments) {
	auto property = get(key);
	if (property != nullptr) return property->getComments();
	return defaultComments;
}
void IniSection::setPropertyComments(const std::string& key, const std::string& comments) {
	add(key)->setComments(comments);
}
std::string IniSection::getValue(const std::string& key, const std::string& defaultValue) const {
	getOrReturnDefault(getValue());
}
void IniSection::setValue(const std::string& key, const std::string& value) {
	add(key)->setValue(value);
}
std::string IniSection::getString(const std::string& key, const std::string& defaultValue) const {
	getOrReturnDefault(getString());
}
void IniSection::setString(const std::string& key, const std::string& value) {
	add(key)->setString(value);
}
int IniSection::getInt(const std::string& key, PcHelp::Radixes radix, int defaultValue) const {
	getOrReturnDefault(getInt(radix));
}
void IniSection::setInt(const std::string& key, int value, PcHelp::Radixes radix) {
	add(key)->setInt(value, radix);
}
float IniSection::getFloat(const std::string& key, float defaultValue) const {
	getOrReturnDefault(getFloat());
}
void IniSection::setFloat(const std::string& key, float value, int precision) {
	add(key)->setFloat(value, precision);
}
bool IniSection::getBool(const std::string& key, const std::string& trueStr, bool defaultValue) const {
	getOrReturnDefault(getBool(trueStr));
}
void IniSection::setBool(const std::string& key, bool value, const std::string& trueStr, const std::string& falseStr) {
	add(key)->setBool(value, trueStr, falseStr);
}
Point2I IniSection::getPoint2I(const std::string& key, Point2I defaultValue) const {
	getOrReturnDefault(getPoint2I());
}
void IniSection::setPoint2I(const std::string& key, Point2I value, const std::string& open, const std::string& separator, const std::string& close) {
	add(key)->setPoint2I(value, open, separator, close);
}
Point2F IniSection::getPoint2F(const std::string& key, Point2F defaultValue) const {
	getOrReturnDefault(getPoint2F());
}
void IniSection::setPoint2F(const std::string& key, Point2F value, const std::string& open, const std::string& separator, const std::string& close, int precision) {
	add(key)->setPoint2F(value, open, separator, close, precision);
}

#pragma endregion
//========= INPUT/OUTPUT =========
#pragma region Input/Output

std::string IniSection::writeSection(bool strictFormatting, bool escape, const std::string& assignmentType, const std::string& commentType, int maxLineLength) {
	std::string text = "";
	if (!comments.empty())
		text += IniDocument::formatComments(comments, commentType);
	if (!key.empty())
		text += IniDocument::formatSection(key, strictFormatting, escape, maxLineLength) + "\n";

	for (int i = 0; i < (int)properties.size(); i++) {
		text += properties[i]->writeProperty(strictFormatting, escape, assignmentType, commentType, maxLineLength);
	}
	return text;
}

#pragma endregion
//=================================================================|
