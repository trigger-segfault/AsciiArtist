/*=================================================================|
|* File:				Path.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PATH_H
#define PATH_H

#include <string>
#include <PowerConsole/IO/IOEnums.h>

// Reference:
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb773559(v=vs.85).aspx

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The amount of total bytes, used bytes, and free bytes in the drive. */
struct DiskUsage {
	/* The number of used bytes in the drive. */
	unsigned long long usedBytes;
	/* The number of free bytes in the drive. */
	unsigned long long freeBytes;
	/* The total number of bytes in the drive. */
	unsigned long long totalBytes;

	/* Constructs the default drive usage. */
	DiskUsage();
	/* Constructs the drive usage. */
	DiskUsage(unsigned long long usedBytes, unsigned long long freeBytes, unsigned long long totalBytes);
};

class Path {
public:
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The path used to represent the computer directory. */
	static const std::string COMPUTER_DIRECTORY;

	#pragma endregion
	//============ FILES =============
	#pragma region Files

	/* Returns true if a file exists at the specified path. */
	static bool fileExists(const std::string& path);
	/* Creates a file at the specified location. */
	static bool createFile(const std::string& path, bool overwrite);
	/* Creates a file at the specified location. */
	static bool createFile(const std::string& path, bool overwrite, FileAttributes attributes);
	/* Copies the specified file to the new location. */
	static bool copyFile(const std::string& path, const std::string& newPath, bool overwrite);
	/* Moves the specified file to the new location. */
	static bool moveFile(const std::string& path, const std::string& newPath, bool overwrite);
	/* Deletes the specified file. */
	static bool deleteFile(const std::string& path, bool moveToRecycleBin = false);
	/* Renames the specified file. */
	static bool renameFile(const std::string& path, const std::string& name);

	#pragma endregion
	//========= DIRECTORIES ==========
	#pragma region Directories

	/* Returns true if a directory exists at the specified path. */
	static bool directoryExists(const std::string& path);
	/* Returns true if the specified root exists. */
	static bool rootExists(const std::string& root);
	/* Creates a directory at the specified path. */
	static bool createDirectory(const std::string& path);
	/* Deltes the specified directory. */
	static bool deleteDirectory(const std::string& path, bool moveToRecycleBin = false);

	#pragma endregion
	//========== FILE INFO ===========
	#pragma region File Info

	static unsigned long long getFileSize(const std::string& path);
	/* Gets the attributes of the specified file. */
	static FileAttributes getFileAttributes(const std::string& path);
	/* Gets the type of the specified file. */
	static FileTypes getFileType(const std::string& path);
	/* Gets the type of the specified drive. */
	static DriveTypes getDriveType(const std::string& path);
	/* Gets the file version of the dll or executable. */
	static std::string getFileVersionString(const std::string& path);
	/* Gets the time that the file was last modified at. */
	static unsigned long long getFileLastModified(const std::string& path);
	/* Gets the time that the file was created at. */
	static unsigned long long getFileCreationDate(const std::string& path);

	#pragma endregion
	//========== LOCATIONS ===========
	#pragma region Locations

	/* Gets the current directory of the program. */
	static std::string getCurrentDirectory();
	/* Sets the current directory of the program. */
	static bool setCurrentDirectory(const std::string& path);
	/* Gets the current user profile directory */
	static std::string GetUserDirectory();
	/* Gets the path of the executable. */
	static std::string getExecutablePath();
	/* Gets the directory of the executable. */
	static std::string getExecutableDirectory();
	/* Gets the temporary file path. */
	static std::string getTempPath();
	/* Gets the name of a temporary file. */
	static std::string getTempFileName(const std::string& path, const std::string& prefix = "");

	#pragma endregion
	//============ NAMING ============
	#pragma region Naming

	/* Returns true if the name follows correct syntax for file names. */
	static bool isValidFileName(const std::string& name);
	/* Returns true if the path follows correct syntax for paths. */
	static bool isValidPath(const std::string& path);

	#pragma endregion
	//============ PATHS =============
	#pragma region paths

	/* Returns true if the specified path exists. */
	static bool pathExists(const std::string& path);
	/* Combines the specified paths into one path. Returns an empty string on failure */
	static std::string combine(const std::string& path1, const std::string& path2);
	/* Combines the specified paths into one path. Returns an empty string on failure */
	static std::string combine(const std::string& path1, const std::string& path2, const std::string& path3);
	/* Combines the specified paths into one path. Returns an empty string on failure */
	static std::string combine(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4);
	/* Gets the full path of the specified relative path. Returns an empty string on failure. */
	static std::string getFullPath(const std::string& path);
	/* Gets the full path with environment strings resolved to their respective directories. */
	static std::string expandEnvironmentPath(const std::string& path);
	/* Gets the parent directory of the specified file. */
	static std::string getDirectory(const std::string& path);
	/* Gets the root directory of the specified path. */
	static std::string getRoot(const std::string& path);
	/* Gets the name of the specified file. */
	static std::string getFileName(const std::string& path);
	/* Gets the name of the specified file with out an extension. */
	static std::string getFileNameWithoutExtension(const std::string& path);
	/* Gets the name of the specified drive. */
	static std::string getDriveName(const std::string& path);
	/* Gets the name extension of the specified file. */
	static std::string getExtension(const std::string& path);
	/* Returns the path with the new extension. */
	static std::string renameExtension(const std::string& path, const std::string& extension);
	/* Returns the path without any extension. */
	static std::string removeExtension(const std::string& path);
	/* Returns true if the file has an extension. */
	static bool hasExtension(const std::string& path);
	/* Resolves the path of the specified shortcut. Returns an empty string on failure */
	static std::string resolveShortcut(const std::string& path);
	/* Gets the file size as a string. */
	static std::string getFileSizeString(unsigned long long size);
	/* Gets the file size as a string. */
	static std::string getFileSizeString(const std::string& path);
	/* Gets a string of the file time. */
	static std::string getFileTimeString(unsigned long long fileTime, bool showDate = true, bool showFullYear = true, bool showTime = true, bool showAMPM = true);
	/* Gets the data usage of the drive. */
	static DiskUsage getDiskUsage(const std::string& path);
	/* Gets the known folder path. */
	static std::string getSpecialFolderPath(GUID folderId);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
