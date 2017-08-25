/*=================================================================|
|* File:				IniSection.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef INI_SECTION_H
#define INI_SECTION_H

#include <string>
#include <map>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/IniProperty.h>

#define makeIniSection(...) std::make_shared<IniSection>(__VA_ARGS__)

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

class IniSection {
	//========== CONSTANTS ===========
	#pragma region Constants
public:

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Ini Properties
	/* The name of the section. */
	std::string key;
	/* The description of the section. */
	std::string comments;
	/* The list of properties in the section. */
	std::vector<IniPropertySPtr> properties;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the ini section. */
	IniSection(const std::string& key, const std::string& comments = "");

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Ini Properties
	/* Gets the name of the property. */
	std::string getKey() const;
	/* Gets the comments for the property. */
	std::string getComments() const;
	/* Sets the comments for the property. */
	void setComments(const std::string& comments);
	/* Gets the list of properties in the section. */
	std::vector<IniPropertySPtr>& getProperties();
	/* Gets the list of properties in the section. */
	const std::vector<IniPropertySPtr>& getProperties() const;

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the property of the section. */
	IniPropertySPtr get(const std::string& key);
	/* Gets the property of the section. */
	const IniPropertySPtr get(const std::string& key) const;
	/* Adds a property to the section. */
	IniPropertySPtr add(IniPropertySPtr property);
	/* Adds a property to the section. */
	IniPropertySPtr add(const std::string& key, const std::string& value = "", const std::string& comments = "", bool useQuotes = false);
	/* Removes a property from the section. */
	void remove(IniPropertySPtr property);
	/* Removes a property from the section. */
	void remove(const std::string& key);
	/* Returns true if the section contains the specified property. */
	bool contains(IniPropertySPtr property) const;
	/* Returns true if the section contains the specified property. */
	bool contains(const std::string& key) const;
	/* Clears all properties from the section. */
	void clear();
	/* Gets the number of properties in the section. */
	int count() const;

	#pragma endregion
	//======== PROPERTY TYPES ========
	#pragma region Properties

	/* Gets the comments for the property. */
	std::string getPropertyComments(const std::string& key, const std::string& defaultComments = "");
	/* Sets the string of the value. */
	void setPropertyComments(const std::string& key, const std::string& comments);
	/* Gets the value of the property. */
	std::string getValue(const std::string& key, const std::string& defaultValue = "") const;
	/* Sets the value of the property. */
	void setValue(const std::string& key, const std::string& value);
	/* Gets the string of the value. */
	std::string getString(const std::string& key, const std::string& defaultValue = "") const;
	/* Sets the string of the value. */
	void setString(const std::string& key, const std::string& value);
	/* Gets the integer of the value. */
	int getInt(const std::string& key, PcHelp::Radixes radix = PcHelp::Radixes::RADIX_DECIMAL, int defaultValue = 0) const;
	/* Sets the integer of the value. */
	void setInt(const std::string& key, int value, PcHelp::Radixes radix = PcHelp::Radixes::RADIX_DECIMAL);
	/* Gets the float of the value. */
	float getFloat(const std::string& key, float defaultValue = 0.0f) const;
	/* Sets the float of the value. */
	void setFloat(const std::string& key, float value, int precision = IniProperty::DEFAULT_PRECISION);
	/* Gets the boolean of the value. */
	bool getBool(const std::string& key, const std::string& trueStr = "true", bool defaultValue = false) const;
	/* Sets the boolean of the value. */
	void setBool(const std::string& key, bool value, const std::string& trueStr = "true", const std::string& falseStr = "false");
	/* Gets the int-based point of the value. */
	PcGeo::Point2I getPoint2I(const std::string& key, PcGeo::Point2I defaultValue = PcGeo::Point2I::ZERO) const;
	/* Sets the int-based point of the value. */
	void setPoint2I(const std::string& key, PcGeo::Point2I value, const std::string& open = "(", const std::string& separator = ",", const std::string& close = ")");
	/* Gets the float-based point of the value. */
	PcGeo::Point2F getPoint2F(const std::string& key, PcGeo::Point2F defaultValue = PcGeo::Point2F::ZERO) const;
	/* Sets the float-based point of the value. */
	void setPoint2F(const std::string& key, PcGeo::Point2F value, const std::string& open = "(", const std::string& separator = ",", const std::string& close = ")", int precision = IniProperty::DEFAULT_PRECISION);

	#pragma endregion
	//========= INPUT/OUTPUT =========
	#pragma region Input/Output

	/* Writes the section to a string. */
	std::string writeSection(bool strictFormatting, bool escape, const std::string& assignmentType, const std::string& commentType, int maxLineLength);

	#pragma endregion
};

typedef std::shared_ptr<IniSection> IniSectionSPtr;

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
