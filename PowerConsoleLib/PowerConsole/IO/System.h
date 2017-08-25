/*=================================================================|
|* File:				System.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region OSVersion

struct OSVersion {
	//========== CONSTANTS ===========
	#pragma region Constants

	/* Used when you don't want to compare a version field. */
	static const unsigned int VERSION_DONT_CARE = 0xFFFFFFFF;

	/* The version number for Windows 2000. (5.0) */
	static const OSVersion WIN_2000;
	/* The version number for Windows XP. (5.1) */
	static const OSVersion WIN_XP;
	/* The version number for Windows Vista. (6.0) */
	static const OSVersion WIN_VISTA;
	/* The version number for Windows 7. (6.1) */
	static const OSVersion WIN_7;
	/* The version number for Windows 8. (6.2) */
	static const OSVersion WIN_8;
	/* The version number for Windows 8.1. (6.3) */
	static const OSVersion WIN_8_1;
	/* The version number for Windows 10. (10.0) */
	static const OSVersion WIN_10;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* The major version number of the OS. */
	unsigned int majorVersion;
	/* The minor version number of the OS. */
	unsigned int minorVersion;
	/* The service pack of the OS. */
	unsigned int servicePack;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default OS Version. */
	OSVersion();
	/* Constructs the OSVersion with the specified settings. */
	OSVersion(unsigned int majorVersion, unsigned int minorVersion = VERSION_DONT_CARE, unsigned int servicePack = VERSION_DONT_CARE);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Returns true if the version is the same. Fields with VERSION_DONT_CARE are not compared. */
	bool operator==(OSVersion version) const;
	/* Returns true if the version is not the same. Fields with VERSION_DONT_CARE are not compared. */
	bool operator!=(OSVersion version) const;
	/* Returns true if the version is greater. Fields with VERSION_DONT_CARE are not compared. */
	bool operator>(OSVersion version) const;
	/* Returns true if the version is lower. Fields with VERSION_DONT_CARE are not compared. */
	bool operator<(OSVersion version) const;
	/* Returns true if the version is greater or equal. Fields with VERSION_DONT_CARE are not compared. */
	bool operator>=(OSVersion version) const;
	/* Returns true if the version is lower or equal. Fields with VERSION_DONT_CARE are not compared. */
	bool operator<=(OSVersion version) const;

	#pragma endregion
};

#pragma endregion
//=================================================================|
#pragma region System

class System {

	//========== EXECUTION ===========
	#pragma region Execution
public:
	/* Runs a file with the specified shell command. */
	static void shellExecute(const std::string& operation, const std::string& file, const std::string& parameters = "", const std::string& startupDirectory = "");
	/* Executes a system command. */
	static void system(const std::string& command);
	/* Sleeps for the specified amount of milliseconds. */
	static void sleep(unsigned int milliseconds);

	#pragma endregion
	//========= INFORMATION ==========
	#pragma region Information

	/* Gets the version of the OS. */
	static OSVersion getOSVersion();

	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
