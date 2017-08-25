/*=================================================================|
|* File:				FileInfo.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <memory>
#include <string>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/IOEnums.h>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A class containing information about a specific file. */
struct FileInfo {

	//=========== MEMBERS ============
	#pragma region Members

	// File Paths
	/* The full name of the file. */
	std::string name;
	/* The extension of the file. */
	std::string extension;
	/* The directory containing this file. */
	std::string directory;
	/* The full path of the file. */
	std::string path;
	/* The path of the shortcut. */
	std::string shortcutPath;

	// File Information
	/* The type of the file. */
	FileTypes fileType;
	/* The type of the file that this file is linked to. */
	FileTypes shortcutType;
	/* The type of the file if the file is a drive. */
	DriveTypes driveType;
	/* The attributes of the file. */
	FileAttributes attributes;
	/* The size of the file. This is also the total bytes for drives. */
	unsigned long long fileSize;
	/* The used space for drives. */
	unsigned long long usedSize;
	/* The time the file was last modified at. */
	unsigned long long lastModified;
	/* The time the file was created at. */
	unsigned long long creationDate;
	/* True if the file is the parent directory in a file list. */
	bool parentDirectory;

	/* Used to store extra information about the file. */
	std::shared_ptr<void> details;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default file info. */
	FileInfo();
	/* Constructs and loads the specified file info. */
	FileInfo(const std::string& path);

	#pragma endregion
	//=========== LOADING ============
	#pragma region Loading

	/* Loads the specified file info. */
	bool load(const std::string& path);
	/* Refreshes the current file info. */
	bool refresh();

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Converts the file info into a boolean based on its validity. */
	bool operator!() const;
	/* Converts the file info into a boolean based on its validity. */
	operator bool() const;

	#pragma endregion
	//========== ATTRIBUTES ==========
	#pragma region Attributes

	/* Returns true if the file is invalid. */
	bool isInvalid() const;
	/* Returns true if the file is hidden. */
	bool isHidden() const;
	/* Returns true if the file is read only. */
	bool isReadOnly() const;
	/* Returns true if the file is temporary. */
	bool isTemporary() const;
	/* Returns true if the file is archived. */
	bool isArchive() const;
	/* Returns true if the file is encrypted. */
	bool isEncrypted() const;
	/* Returns true if the file is a system file. */
	bool isSystem() const;

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
