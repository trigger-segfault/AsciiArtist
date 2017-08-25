/*=================================================================|
|* File:				AsyncDirectoryLoader.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASYNC_DIRECTORY_LOADER_H
#define ASYNC_DIRECTORY_LOADER_H

#include <PowerConsole/IO/DirectoryInfo.h>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Asynchornously loads a directory info structure. */
class AsyncDirectoryLoader {

	//=========== MEMBERS ============
	#pragma region Members
public:
	typedef std::function<bool(const FileInfo&)> KeepFileFunction;
	typedef std::function<void(int,FileInfo&)> FileAddedFunction;

private:
	/* The directory info structure being loaded. */
	DirectoryInfo directory;

	/* The handle for enumerating files in a folder. */
	HANDLE hDir;
	/* The file data that was loaded from the directory. */
	WIN32_FIND_DATAA file;
	/* The ending position of the folders in this directory enumeration. */
	int folderEnd;
	/* The ending position of the shortcuts in this directory enumeration. */
	int shortcutEnd;
	/* The timeout period to use on the first load function. */
	int firstTimeoutPeriod;
	/* The timeout period to use when continue loading is called. */
	int timeoutPeriod;
	/* True if the directory is still loading. */
	bool loading;
	/* The function to determine if a file should be kept in the list. */
	KeepFileFunction keepFileFunction;
	/* The function called when a file is added to the list. */
	FileAddedFunction fileAddedFunction;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default async directory loader. */
	AsyncDirectoryLoader();

	#pragma endregion
	//=========== LOADING ============
	#pragma region Loading

	/* Loads the directory info from the specified path. */
	bool load(const std::string& path);
	/* Loads the directory info from the specified file. */
	bool load(const FileInfo& file);
	/* Refreshes the current directory info. */
	bool refresh();
	/* Continues loading the directory. */
	void continueLoading();
	/* Inserts a file into the list. */
	int insert(FileInfo&& file);
	/* Removes the file at the specified index. */
	void remove(int index);
private:
	/* Continues loading with the specified settings. */
	void continueLoading(int startTime, int timeoutPeriod);
	/* Inserts a file at the specified position. */
	bool insertFinal(int index, FileInfo&& file);
	/* Inserts a parent folder. */
	void insertParent(FileInfo&& file);
	/* Inserts a folder. */
	void insertFolder(FileInfo&& file);
	/* Inserts a folder shortcut. */
	void insertShortcut(FileInfo&& file);
	/* Inserts a file. */
	void insertFile(FileInfo&& file);
	/* Inserts a drive. */
	void insertDrive(FileInfo&& file);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
public:
	/* Gets the timeout period to use on the first load function. */
	int getFirstTimeoutPeriod() const;
	/* Sets the timeout period to use on the first load function. */
	void setFirstTimeoutPeriod(int firstTimeoutPeriod);
	/* Gets the timeout period to use when continue loading is called. */
	int getTimeoutPeriod() const;
	/* Sets the timeout period to use when continue loading is called. */
	void setTimeoutPeriod(int timeoutPeriod);
	/* Returns true if the directory is still being loaded. */
	bool isLoading() const;
	/* Gets the directory info loaded by this class. */
	DirectoryInfo& getDirectory();
	/* Gets the directory info loaded by this class. */
	const DirectoryInfo& getDirectory() const;
	/* Sets the function to determine if a file should be kept in the list. */
	void setKeepFileFunction(KeepFileFunction func);
	/* Sets the function called when a file is added to the list. */
	void setFileAddedFunction(FileAddedFunction func);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
