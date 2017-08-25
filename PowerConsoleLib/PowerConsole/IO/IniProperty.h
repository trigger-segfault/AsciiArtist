/*=================================================================|
|* File:				IniProperty.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef INI_PROPERTY_H
#define INI_PROPERTY_H

#include <string>
#include <memory>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Geometry/Rectangle.h>
#include <PowerConsole/Helpers/ToString.h>

#define makeIniProperty(...) std::make_shared<IniProperty>(__VA_ARGS__)

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A class for storing a key and value of a property. */
class IniProperty {
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The default precision for floating-point numbers. */
	static const int DEFAULT_PRECISION = 6;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Ini Properties
	/* The name of the property. */
	std::string key;
	/* The comments for the property. */
	std::string comments;
	/* The value of the property. */
	std::string value;

	// Format
	/* True if the property uses quotes to format its value. */
	bool useQuotes;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the ini property. */
	IniProperty(const std::string& key, const std::string& value = "", const std::string& comments = "", bool useQuotes = false);

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
	/* Gets the value of the property. */
	std::string getValue() const;
	/* Sets the value of the property. */
	void setValue(const std::string& value);

	// Format
	/* Gets if the property uses quotes to format its value. */
	bool getUseQuotes() const;
	/* Sets if the property uses quotes to format its value. */
	void setUseQuotes(bool useQuotes);

	#pragma endregion
	//======== PROPERTY TYPES ========
	#pragma region Properties

	/* Gets the string of the value. */
	std::string getString() const;
	/* Sets the string of the value. */
	void setString(const std::string& value);
	/* Gets the integer of the value. */
	int getInt(PcHelp::Radixes radix = PcHelp::Radixes::RADIX_DECIMAL) const;
	/* Sets the integer of the value. */
	void setInt(int value, PcHelp::Radixes radix = PcHelp::Radixes::RADIX_DECIMAL);
	/* Gets the float of the value. */
	float getFloat() const;
	/* Sets the float of the value. */
	void setFloat(float value, int precision = DEFAULT_PRECISION);
	/* Gets the boolean of the value. */
	bool getBool(const std::string& trueStr = "true") const;
	/* Sets the boolean of the value. */
	void setBool(bool value, const std::string& trueStr = "true", const std::string& falseStr = "false");
	/* Gets the int-based point of the value. */
	PcGeo::Point2I getPoint2I() const;
	/* Sets the int-based point of the value. */
	void setPoint2I(PcGeo::Point2I value, const std::string& open = "(", const std::string& separator = ",", const std::string& close = ")");
	/* Gets the float-based point of the value. */
	PcGeo::Point2F getPoint2F() const;
	/* Sets the float-based point of the value. */
	void setPoint2F(PcGeo::Point2F value, const std::string& open = "(", const std::string& separator = ",", const std::string& close = ")", int precision = DEFAULT_PRECISION);

	#pragma endregion
	//========= INPUT/OUTPUT =========
	#pragma region Input/Output

	/* Writes the property to a string. */
	std::string writeProperty(bool strictFormatting, bool escape, const std::string& assignmentType, const std::string& commentType, int maxLineLength);

	#pragma endregion
};

typedef std::shared_ptr<IniProperty> IniPropertySPtr;

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
