/*=================================================================|
|* File:				IniDocument.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef INI_DOCUMENT_H
#define INI_DOCUMENT_H

#include <string>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/IniSection.h>

#define makeIniDocument(...) std::make_shared<IniDocument>(__VA_ARGS__)

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

class IniDocument {

	//========== CONSTANTS ===========
	#pragma region Constants
public:

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Format
	/* The string used for assignments. */
	std::string assignmentType;
	/* The string used to start comments. */
	std::string commentType;
	/* The maximum line length before the line is continued. */
	int maxLineLength;
	/* Requires that all formatted lines use escape characters even when unnecessary. */
	bool strictFormatting;

	// Settings
	/* True if comments are retained upon loading. */
	bool keepComments;
	/* True if duplicate sections are allowed. */
	bool allowDuplicates;
	/* True if characters are escaped. */
	bool escapeEnabled;
	/* The line spacing between sections. */
	int sectionSpacing;

	// Ini Properties
	/* The list of sections containing properties. */
	std::vector<IniSectionSPtr> sections;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the ini document. */
	IniDocument();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Format
	/* Gets the string used for assignments. */
	std::string getAssignmentType() const;
	/* Sets the string used for assignments. */
	void setAssignmentType(const std::string& assignmentType);
	/* Gets the string used to start comments. */
	std::string getCommentType() const;
	/* Sets the string used to start comments. */
	void setCommentType(const std::string& commentType);
	/* Gets the maximum line length before the line is continued. */
	int getMaxLineLength() const;
	/* Sets the maximum line length before the line is continued. */
	void setMaxLineLength(int maxLineLength);
	/* Gets if formatted lines use of escape characters even when unnecessary. */
	bool getStrictFormatting() const;
	/* Sets if formatted lines use of escape characters even when unnecessary. */
	void setStrictFormatting(bool strictFormatting);

	// Settings
	/* Gets if comments are retained upon loading. */
	bool getKeepComments() const;
	/* Sets if comments are retained upon loading. */
	void setKeepComments(bool keepComments);
	/* Gets if duplicate sections are allowed. */
	bool getAllowDuplicates() const;
	/* Sets if duplicate sections are allowed. */
	void setAllowDuplicates(bool allowDuplicates);
	/* Gets if characters are escaped. */
	bool isEscapeEnabled() const;
	/* Sets if characters are escaped. */
	void setEscapeEnabled(bool escapeEnabled);
	/* Gets the line spacing between sections. */
	int getSectionSpacing() const;
	/* Sets the line spacing between sections. */
	void setSectionSpacing(int sectionSpacing);

	// Ini Properties
	/* Gets the global section. */
	IniSectionSPtr getGlobalSection();
	/* Gets the global section. */
	const IniSectionSPtr getGlobalSection() const;
	/* Gets the list of sections containing properties. */
	std::vector<IniSectionSPtr>& getSections();
	/* Gets the list of sections containing properties. */
	const std::vector<IniSectionSPtr>& getSections() const;

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	// Sections
	/* Gets the section of the document. */
	IniSectionSPtr get(const std::string& key);
	/* Gets the section of the document. */
	const IniSectionSPtr get(const std::string& key) const;
	/* Adds a section to the document. */
	IniSectionSPtr add(IniSectionSPtr section);
	/* Adds a section to the document. */
	IniSectionSPtr add(const std::string& key, const std::string& comments = "");
	/* Removes a section from the document. */
	void remove(IniSectionSPtr section);
	/* Removes a section from the document. */
	void remove(const std::string& key);
	/* Returns true if the document contains the specified section. */
	bool contains(IniSectionSPtr section) const;
	/* Returns true if the document contains the specified section. */
	bool contains(const std::string& key) const;
	/* Clears all properties from the section. */
	void clear();
	/* Gets the number of sections in the document. */
	int count() const;

	#pragma endregion
	//========= INPUT/OUTPUT =========
	#pragma region Input/Output

	/* Loads the ini document from text. */
	void loadFromText(const std::string& text);
	/* Loads the ini document. */
	bool load(const std::string& path);
	/* Saves the ini document. */
	bool save(const std::string& path);

	#pragma endregion
	//=========== PARSING ============
	#pragma region Parsing
private:
	/* Parses the text to create the list of properties */
	void readDocument(std::string text);

	/* Parses and returns the text with escape characters. */
	static std::string parseEscape(std::string text);
	/* Parses and returns the section. */
	static std::string parseSection(std::string section, bool escape);
	/* Parses and returns the property. */
	static std::string parseProperty(std::string property);
	/* Parses and returns the key. */
	static std::string parseKey(std::string key, bool escape);
	/* Parses and returns the value. */
	static std::string parseValue(std::string value, bool escape, bool& useQuotes);

	#pragma endregion
	//========== FORMATTING ==========
	#pragma region Formatting
public:
	/* Writes the document to a string. */
	std::string writeDocument();
private:
	/* Format and returns the text with escape characters. */
	static std::string formatEscape(std::string text, bool escape, bool special, bool quotes, bool spaces);
	/* Formats and returns the section header. */
	static std::string formatSection(const std::string& section, bool strictFormatting, bool escape, int maxLineLength);
	/* Formats and returns the property. */
	static std::string formatProperty(const std::string& key, const std::string& value, bool strictFormatting, bool escape, const std::string& assignmentType, int maxLineLength, bool useQuotes);
	/* Formats and returns the key. */
	static std::string formatKey(const std::string& key, bool strictFormatting, bool escape);
	/* Formats and returns the value. */
	static std::string formatValue(std::string value, bool strictFormatting, bool escape, bool useQuotes);
	/* Formats and returns the comments with comment indicators. */
	static std::string formatComments(std::string comments, const std::string& commentType);
	/* Continues the line if it reaches the max length. */
	static std::string continueLine(std::string text, int maxLineLength);

	#pragma endregion
	//=========== FRIENDS ============
	#pragma region Friends

	friend IniProperty;
	friend IniSection;

	#pragma endregion
};

typedef std::shared_ptr<IniDocument> IniDocumentSPtr;

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
