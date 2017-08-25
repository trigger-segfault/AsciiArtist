/*=================================================================|
|* File:				IniDocument.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "IniDocument.h"
#include <PowerConsole/Helpers/StringHelpers.h>
#include <fstream>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

IniDocument::IniDocument() {
	// Format
	this->assignmentType	= "=";
	this->commentType		= "; ";
	this->maxLineLength		= -1;
	this->strictFormatting	= false;

	// Settings
	this->keepComments		= false;
	this->allowDuplicates	= false;
	this->escapeEnabled		= false;
	this->sectionSpacing	= 0;

	// Add global section
	this->sections.push_back(makeIniSection(""));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Format
std::string IniDocument::getAssignmentType() const {
	return assignmentType;
}
void IniDocument::setAssignmentType(const std::string& assignmentType) {
	this->assignmentType = assignmentType;
}
std::string IniDocument::getCommentType() const {
	return commentType;
}
void IniDocument::setCommentType(const std::string& commentType) {
	this->commentType = commentType;
}
int IniDocument::getMaxLineLength() const {
	return maxLineLength;
}
void IniDocument::setMaxLineLength(int maxLineLength) {
	this->maxLineLength = maxLineLength;
}
bool IniDocument::getStrictFormatting() const {
	return strictFormatting;
}
void IniDocument::setStrictFormatting(bool strictFormatting) {
	this->strictFormatting = strictFormatting;
}

// Settings
bool IniDocument::getKeepComments() const {
	return keepComments;
}
void IniDocument::setKeepComments(bool keepComments) {
	this->keepComments = keepComments;
}
bool IniDocument::getAllowDuplicates() const {
	return allowDuplicates;
}
void IniDocument::setAllowDuplicates(bool allowDuplicates) {
	this->allowDuplicates = allowDuplicates;
}
bool IniDocument::isEscapeEnabled() const {
	return escapeEnabled;
}
void IniDocument::setEscapeEnabled(bool escapeEnabled) {
	this->escapeEnabled = escapeEnabled;
}
int IniDocument::getSectionSpacing() const {
	return sectionSpacing;
}
void IniDocument::setSectionSpacing(int sectionSpacing) {
	this->sectionSpacing = sectionSpacing;
}

// Ini Properties
IniSectionSPtr IniDocument::getGlobalSection() {
	return sections[0];
}
const IniSectionSPtr IniDocument::getGlobalSection() const {
	return sections[0];
}
std::vector<IniSectionSPtr>& IniDocument::getSections() {
	return sections;
}
const std::vector<IniSectionSPtr>& IniDocument::getSections() const {
	return sections;
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

// Sections
IniSectionSPtr IniDocument::get(const std::string& key) {
	auto it = std::find_if(sections.begin(), sections.end(), [key](IniSectionSPtr section) {
		return compareString(section->getKey(), key, false);
	});
	if (it != sections.end())
		return *it;
	return makeIniSection("");
}
const IniSectionSPtr IniDocument::get(const std::string& key) const {
	auto it = std::find_if(sections.begin(), sections.end(), [key](IniSectionSPtr section) {
		return compareString(section->getKey(), key, false);
	});
	if (it != sections.end())
		return *it;
	return makeIniSection("");
}
IniSectionSPtr IniDocument::add(IniSectionSPtr section) {
	if (allowDuplicates) {
		sections.push_back(section);
		return section;
	}
	auto it = std::find(sections.begin(), sections.end(), section);
	if (it == sections.end()) {
		sections.push_back(section);
		return section;
	}
	return nullptr;
}
IniSectionSPtr IniDocument::add(const std::string& key, const std::string& comments) {
	IniSectionSPtr section;
	if (allowDuplicates) {
		section = makeIniSection(key, comments);
		sections.push_back(section);
		return section;
	}
	section = get(key);
	if (!compareString(section->getKey(), key, false)) {
		section = makeIniSection(key, comments);
		sections.push_back(section);
		return section;
	}
	else {
		return section;
	}
}
void IniDocument::remove(IniSectionSPtr section) {
	auto it = std::find(sections.begin(), sections.end(), section);
	if (it != sections.end())
		sections.erase(it);
}
void IniDocument::remove(const std::string& key) {
	auto it = std::find_if(sections.begin(), sections.end(), [key](IniSectionSPtr section) {
		return compareString(section->getKey(), key, false);
	});
	if (it != sections.end())
		sections.erase(it);
}
bool IniDocument::contains(IniSectionSPtr section) const {
	return std::find(sections.begin(), sections.end(), section) != sections.end();
}
bool IniDocument::contains(const std::string& key) const {
	return std::find_if(sections.begin(), sections.end(), [key](IniSectionSPtr section) {
		return compareString(section->getKey(), key, false);
	}) != sections.end();
}
void IniDocument::clear() {
	sections.erase(sections.begin() + 1, sections.end());
	sections[0]->clear();
}
int IniDocument::count() const {
	return (int)sections.size();
}

#pragma endregion
//========= INPUT/OUTPUT =========
#pragma region Input/Output

void IniDocument::loadFromText(const std::string& text) {
	readDocument(text);
}
bool IniDocument::load(const std::string& path) {
	std::ifstream in;
	in.open(path);
	if (in.is_open()) {
		bool error = false;
		std::string text;
		text.assign((std::istreambuf_iterator<char>(in)),
					(std::istreambuf_iterator<char>()));
		error = in.bad() || in.fail();
		in.close();
		readDocument(text);
		return !error;
	}
	return false;
}
bool IniDocument::save(const std::string& path) {
	std::ofstream out;
	out.open(path);
	if (out.is_open()) {
		bool error = false;
		out << writeDocument();
		error = out.bad() || out.fail();
		out.close();
		return !error;
	}
	return false;
}

#pragma endregion
//=========== PARSING ============
#pragma region Parsing

void IniDocument::readDocument(std::string text) {
	// The lines of the text
	std::vector<std::string> lines;
	// The current section being edited. (Set to global section by default)
	IniSectionSPtr section = makeIniSection("");
	// The current description for the next section or property
	std::string comment = "";

	sections.clear();
	sections.push_back(section);

	// Formating Order:
	// Replace '\r' with '\n'
	// Remove all empty lines
	// Remove '\n' if line ends with '\'
	// Split up lines
	// Remove lines beginning with ';', '#', or '!'
	// Split lines at "=", " = ", ":", or " : " and not "\\=" or "\\:"
	// Remove quotes around values
	// Implement all escape characters:
	// '\\', '\0', '\a', '\b', '\t', '\r', '\n', '\;', '\#', '\!', '\=', '\:', '\"', '\ ', '\x####'

	// Replace all carriage returns with new lines
	replaceString(text, "\r\n", "\n");

	// Remove all empty lines
	if (!keepComments)
		replaceString(text, std::regex("\n+"), "\n");

	// Remove all commented lines
	if (!keepComments) {
		replaceString(text, std::regex("\n[\\;|\\#|\\!].*"), "");
		replaceString(text, std::regex("^[\\;|\\#|\\!].*\n"), "");
	}

	// Remove new line if line ends with '\'
	if (!keepComments) {
		replaceString(text, "\\\n", "");
		// Split up the lines
		splitString(text, lines, "\n");
	}
	else {
		// Split up the lines
		splitString(text, lines, "\n");
		for (int i = 0; i < (int)lines.size(); i++) {
			if (!lines[i].empty()) {
				if (searchString(lines[i], std::regex("^[^\\;\\#\\!].*\\\\$")) == 0) {
					lines[i] = lines[i].substr(0, lines[i].length() - 1);
					if (i + 1 < (int)lines.size()) {
						lines[i] += lines[i + 1];
						lines.erase(lines.begin() + i);
					}
				}
			}
		}
	}

	// Iterate through all the lines
	for (int i = 0; i < (int)lines.size(); i++) {
		std::string& line = lines[i];
		std::smatch match;
		if (line.empty()) { // Empty line comment
			// If the comment is a blank line
			comment += "\n";
		}
		else if (searchString(line, std::regex("^[\\;\\#\\!].*")) == 0) { // Comment
			// If the comment is spaced or not spaced
			if (searchString(line, std::regex("^[\\;\\#\\!] .*")) == 0)
				comment += line.substr(2, line.length() - 2) + "\n";
			else
				comment += line.substr(1, line.length() - 1) + "\n";
		}
		else if (searchString(line, std::regex("^\\[.*\\]$")) == 0) { // Section
			section = add(parseSection(line, escapeEnabled), comment);
			comment = "";
		}
		else { // Property
			std::vector<std::string> parts;
			// Split the line into [key, value]
			splitString(parseProperty(line), parts, "\n");
			if (parts.size() == 2) {
				bool useQuotes = false;
				std::string key		= parseKey(parts[0], escapeEnabled);
				std::string value	= parseValue(parts[1], escapeEnabled, useQuotes);
				section->add(key, value, comment, useQuotes);
				comment = "";
			}
			else {
				// Not a valid line
			}
		}
	}
}

std::string IniDocument::parseEscape(std::string text) {
	replaceString(text, "\\\\", "\\");
	replaceString(text, "\\0", "\0");
	replaceString(text, "\\a", "\a");
	replaceString(text, "\\b", "\b");
	replaceString(text, "\\t", "\t");
	replaceString(text, "\\r", "\r");
	replaceString(text, "\\n", "\n");
	replaceString(text, "\\\"", "\"");
	replaceString(text, "\\;", ";");
	replaceString(text, "\\#", "#");
	replaceString(text, "\\!", "!");
	replaceString(text, "\\=", "=");
	replaceString(text, "\\:", ":");
	replaceString(text, "\\ ", " ");
	return text;
}
std::string IniDocument::parseSection(std::string section, bool escape) {
	section = section.substr(1, section.length() - 2);
	if (escape)
		section = parseEscape(section);
	return section;
}
std::string IniDocument::parseProperty(std::string property) {
	// Separate the key and value by splitting it with '\n'
	// Replace ' ', '=', and ':' with non-matchable characters
	replaceString(property, "\\ ", "\\s");
	replaceString(property, "\\=", "\\e");
	replaceString(property, "\\:", "\\c");

	//replaceString(property, std::regex(" \\= | \\: |\\=|\\:"), "\n");
	int firstIndex = -1;
	int firstIndex1 = searchString(property, "=");
	int firstIndex2 = searchString(property, ":");
	if (firstIndex1 != -1 && (firstIndex1 < firstIndex2 || firstIndex2 == -1))
		firstIndex = firstIndex1;
	else if (firstIndex2 != -1 && (firstIndex2 < firstIndex1 || firstIndex1 == -1))
		firstIndex = firstIndex2;
	if (firstIndex != -1)
		property[firstIndex] = '\n';

	replaceString(property, "\\s", "\\ ");
	replaceString(property, "\\e", "\\=");
	replaceString(property, "\\c", "\\:");
	return property;
}
std::string IniDocument::parseKey(std::string key, bool escape) {
	if (escape)
		return parseEscape(key);
	return key;
}
std::string IniDocument::parseValue(std::string value, bool escape, bool& useQuotes) {
	// Remove quotes if the value was quoted
	std::smatch match;
	useQuotes = (searchString(value, std::regex("^\\\".*\\\"$")) == 0);
	if (useQuotes) {
		value = value.substr(1, value.length() - 2);
	}
	if (escape)
		value = parseEscape(value);
	return value;
}

#pragma endregion
//========== FORMATTING ==========
#pragma region Formatting

std::string IniDocument::writeDocument() {
	std::string text = "";

	// Write each of the sections
	for (int i = 0; i < (int)sections.size(); i++) {
		text += sections[i]->writeSection(strictFormatting, escapeEnabled , assignmentType, commentType, maxLineLength);
		if (!text.empty() && i + 1 < (int)sections.size() && sectionSpacing > 0)
			text += std::string(sectionSpacing, '\n');
	}

	// Final Formatting
	// Not needed since std::ofstream seems to do this automatically
	//replaceString(text, "\n", "\r\n");

	return text;
}

std::string IniDocument::formatEscape(std::string text, bool escape, bool special, bool quotes, bool spaces) {
	replaceString(text, "\\", "\\\\");
	if (escape) {
		replaceString(text, "\0", "\\0");
		replaceString(text, "\a", "\\a");
		replaceString(text, "\b", "\\b");
		replaceString(text, "\t", "\\t");
		replaceString(text, "\r", "\\r");
		replaceString(text, "\n", "\\n");
	}
	if (quotes) {
		replaceString(text, "\"", "\\\"");
	}
	if (special) {
		replaceString(text, ";", "\\;");
		replaceString(text, "#", "\\#");
		replaceString(text, "!", "\\!");
		replaceString(text, "=", "\\=");
		replaceString(text, ":", "\\:");
	}
	if (spaces) {
		replaceString(text, " ", "\\ ");
	}
	return text;
}
std::string IniDocument::formatSection(const std::string& section, bool strictFormatting, bool escape, int maxLineLength) {
	if (section.empty())
		return "";
	std::string text = section;
	if (escape)
		text = formatEscape(section, true, strictFormatting, strictFormatting, strictFormatting);
	text = "[" + text + "]";
	return continueLine(text, maxLineLength);
}
std::string IniDocument::formatProperty(const std::string& key, const std::string& value, bool strictFormatting, bool escape, const std::string& assignmentType, int maxLineLength, bool useQuotes) {
	std::string text = "";
	text += formatKey(key, strictFormatting, escape);
	text += assignmentType;
	text += formatValue(value, strictFormatting, escape, useQuotes);
	return continueLine(text, maxLineLength);
}
std::string IniDocument::formatKey(const std::string& key, bool strictFormatting, bool escape) {
	if (escape)
		return formatEscape(key, escape, true, strictFormatting, true);
	return key;
}
std::string IniDocument::formatValue(std::string value, bool strictFormatting, bool escape, bool useQuotes) {
	if (escape)
		value = formatEscape(value, true, strictFormatting, true, strictFormatting);
	if (useQuotes)
		return "\"" + value + "\"";
	return value;
}
std::string IniDocument::formatComments(std::string comments, const std::string& commentType) {
	std::vector<std::string> lines;
	splitString(comments, lines, "\n");
	for (int i = 0; i < (int)lines.size(); i++) {
		// Don't comment empty lines
		if (!lines[i].empty())
			lines[i] = commentType + lines[i];
	}
	return joinString2(lines, "\n");
}
std::string IniDocument::continueLine(std::string text, int maxLineLength) {
	std::vector<std::string> lines;
	while ((int)text.length() > maxLineLength && maxLineLength != -1) {
		lines.push_back(text.substr(0, maxLineLength + 1));
		text = text.substr(maxLineLength + 1, (int)text.length() - maxLineLength + 1);
	}
	if (!lines.empty())
		return joinString2(lines, "\n") + text;
	return text;
}

#pragma endregion
//=================================================================|
