/*=================================================================|
|* File:				DirectoryInfo.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef DIRECTORY_INFO_H
#define DIRECTORY_INFO_H

#include <vector>
#include <string>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/FileInfo.h>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A class containing information about a specific directory. */
struct DirectoryInfo {

	//=========== MEMBERS ============
	#pragma region Members

	/* The file of this directory. */
	FileInfo directoryFile;
	/* The list of files in the directory. */
	std::vector<FileInfo> fileList;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default directory. */
	DirectoryInfo();
	/* Constructs and loads the specified directory. */
	DirectoryInfo(const std::string& path);
	/* Constructs and loads the specified directory. */
	DirectoryInfo(const FileInfo& file);

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Gets the file at the specified index in the directory. */
	FileInfo& operator[](int index);

	#pragma endregion
	//=========== LOADING ============
	#pragma region Loading

	/* Loads the directory info from the specified path. */
	bool load(const std::string& path);
	/* Loads the directory info from the specified file. */
	bool load(const FileInfo& file);
	/* Refreshes the current directory info. */
	bool refresh();

	#pragma endregion
	//========== FILTERING ===========
	#pragma region Filtering

	/* Removes all files with the specified file extension from the list. */
	void removeFileExtensions(const std::string& extension, bool exclude = false);
	/* Removes all files with the specified file extension from the list. */
	void removeFileExtensions(const std::vector<std::string>& extensions, bool exclude = false);
	/* Removes all files with the specified file types from the list. */
	void removeFileTypes(FileTypes type, bool exclude = false);
	/* Removes all shortcuts with the specified file types from the list. */
	void removeShortcutTypes(FileTypes type, bool exclude = false);
	/* Removes all files with the specified file attributes from the list. */
	void removeFileAttributes(FileAttributes attributes, bool exclude = false);
	/* Removes the parent directory from the list. */
	void removeParentDirectory();

	#pragma endregion
	//=========== LISTING ============
	#pragma region Listing

	/* Gets the number of files in the directory. */
	int fileCount() const;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
