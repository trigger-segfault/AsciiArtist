/*=================================================================|
|* File:				Resources.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Resources.h"

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== RESOURCES ===========
#pragma region Resources

bool Resources::resourceExists(const std::string& name, const std::string& type) {
	return FindResourceA(nullptr, name.c_str(), type.c_str()) != nullptr;
}
bool Resources::resourceExists(const std::string& name, unsigned int typeId) {
	return FindResourceA(nullptr, name.c_str(), MAKEINTRESOURCEA(typeId)) != nullptr;
}
bool Resources::resourceExists(unsigned int id, const std::string& type) {
	return FindResourceA(nullptr, MAKEINTRESOURCEA(id), type.c_str()) != nullptr;
}
bool Resources::resourceExists(unsigned int id, unsigned int typeId) {
	return FindResourceA(nullptr, MAKEINTRESOURCEA(id), MAKEINTRESOURCEA(typeId)) != nullptr;
}

std::string Resources::loadText(unsigned int id, const std::string& type) {
	HRSRC hRsrc = FindResourceA(nullptr, MAKEINTRESOURCEA(id), type.c_str());
	HGLOBAL rcData = LoadResource(nullptr, hRsrc);
	DWORD size = SizeofResource(nullptr, hRsrc);
	std::string text = (const char*)LockResource(rcData);
	return text;
}

#pragma endregion
//=================================================================|
