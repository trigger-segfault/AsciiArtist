/*=================================================================|
|* File:				System.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "System.h"
#include <PowerConsole/HiddenWinAPI.h>

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region OSVersion
#define dontCareCompare(version1, version2) ((version1) == (version2) || (version1) == VERSION_DONT_CARE || (version2) == VERSION_DONT_CARE)
//========== CONSTANTS ===========
#pragma region Constants

const OSVersion OSVersion::WIN_2000		= OSVersion(5,0);
const OSVersion OSVersion::WIN_XP		= OSVersion(5,1);
const OSVersion OSVersion::WIN_VISTA	= OSVersion(6,0);
const OSVersion OSVersion::WIN_7		= OSVersion(6,1);
const OSVersion OSVersion::WIN_8		= OSVersion(6,2);
const OSVersion OSVersion::WIN_8_1		= OSVersion(6,3);
const OSVersion OSVersion::WIN_10		= OSVersion(10,0);

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

OSVersion::OSVersion()
	: majorVersion(0), minorVersion(0), servicePack(0) {}
OSVersion::OSVersion(unsigned int majorVersion, unsigned int minorVersion, unsigned int servicePack)
	: majorVersion(majorVersion), minorVersion(minorVersion), servicePack(servicePack) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

bool OSVersion::operator==(OSVersion version) const {
	return
		dontCareCompare(majorVersion, version.majorVersion) &&
		dontCareCompare(minorVersion, version.minorVersion) &&
		dontCareCompare(servicePack, version.servicePack);
}
bool OSVersion::operator!=(OSVersion version) const {
	return
		!dontCareCompare(majorVersion, version.majorVersion) ||
		!dontCareCompare(minorVersion, version.minorVersion) ||
		!dontCareCompare(servicePack, version.servicePack);
}
bool OSVersion::operator>(OSVersion version) const {
	if (dontCareCompare(majorVersion, version.majorVersion)) {
		if (dontCareCompare(minorVersion, version.minorVersion))
			return servicePack > version.servicePack;
		else
			return minorVersion > version.minorVersion;
	}
	else {
		return majorVersion > version.majorVersion;
	}
}
bool OSVersion::operator<(OSVersion version) const {
	if (dontCareCompare(majorVersion, version.majorVersion)) {
		if (dontCareCompare(minorVersion, version.minorVersion))
			return servicePack < version.servicePack;
		else
			return minorVersion < version.minorVersion;
	}
	else {
		return majorVersion < version.majorVersion;
	}
}
bool OSVersion::operator>=(OSVersion version) const {
	if (dontCareCompare(majorVersion, version.majorVersion)) {
		if (dontCareCompare(minorVersion, version.minorVersion))
			return (servicePack > version.servicePack || dontCareCompare(servicePack, version.servicePack));
		else
			return minorVersion > version.minorVersion;
	}
	else {
		return majorVersion > version.majorVersion;
	}
}
bool OSVersion::operator<=(OSVersion version) const {
	if (dontCareCompare(majorVersion, version.majorVersion)) {
		if (dontCareCompare(minorVersion, version.minorVersion))
			return (servicePack < version.servicePack || dontCareCompare(servicePack, version.servicePack));
		else
			return minorVersion < version.minorVersion;
	}
	else {
		return majorVersion < version.majorVersion;
	}
}

#pragma endregion
//================================
#undef dontCareCompare
#pragma endregion
//=================================================================|
#pragma region System
//========== EXECUTION ===========
#pragma region Execution

void System::shellExecute(const std::string& operation, const std::string& file, const std::string& parameters, const std::string& startupDirectory) {
	ShellExecuteA(NULL, operation.c_str(), file.c_str(), parameters.c_str(), startupDirectory.c_str(), SW_SHOW);
}
void System::system(const std::string& command) {
	::system(("\"" + command + "\"").c_str());
}
void System::sleep(unsigned int milliseconds) {
	Sleep(milliseconds);
}

#pragma endregion
//========= INFORMATION ==========
#pragma region Information

OSVersion System::getOSVersion() {
	OSVERSIONINFOEXW osInfo = { sizeof(OSVERSIONINFOEXW) };
	if (RtlGetVersion(&osInfo) == STATUS_SUCCESS) {
		return OSVersion(osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.wServicePackMajor);
	}
	return OSVersion();
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
