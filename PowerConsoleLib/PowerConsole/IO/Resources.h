/*=================================================================|
|* File:				Resources.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The static class for managing resources. */
class Resources {
public:
	//========== RESOURCES ===========
	#pragma region Resources

	/* Returns true if the specified resource exists. */
	static bool resourceExists(const std::string& name, const std::string& type);
	/* Returns true if the specified resource exists. */
	static bool resourceExists(const std::string& name, unsigned int typeId);
	/* Returns true if the specified resource exists. */
	static bool resourceExists(unsigned int id, const std::string& type);
	/* Returns true if the specified resource exists. */
	static bool resourceExists(unsigned int id, unsigned int typeId);

	/* Loads text from a text file resource. */
	static std::string loadText(unsigned int id, const std::string& type);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
