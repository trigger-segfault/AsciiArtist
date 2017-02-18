//================================================================
// FILE MANAGER HEADER:
//================================================================
#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "stdafx.h"

using namespace std;
//================================================================
// NAMESPACE:
//================================================================
/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of Windows orientated classes and wrappers. */
namespace Windows {
//================================================================
// DEFINITIONS:
//================================================================
#undef CreateFile
#undef CopyFile
#undef MoveFile
#undef DeleteFile
#undef GetFileAttributes
#undef GetDriveType

//================================================================
// ENUMERATIONS:
//================================================================
/* The different types a file can be. */
enum class FileTypes : unsigned short {
	/* There is no file. */
	None		= 0,

	/* The file is a normal file. */
	File		= 1,
	/* The file is a link to another file. */
	Link		= 2,
	/* The file is a folder. */
	Directory	= 3,
	/* The file is a drive. */
	Drive		= 4,
	/* The file is the computer. */
	Computer	= 5
};
/* The types a drive can be. */
enum class DriveTypes : unsigned short {
	/* The file is not a drive. */
	None		= 0,

	/* The drive is fixed such as a hard drive. */
	Fixed		= DRIVE_FIXED,
	/* The drive is removable such as a usb drive. */
	Removable	= DRIVE_REMOVABLE,
	/* The drive is a network drive. */
	Remote		= DRIVE_REMOTE,
	/* The drive is a CD drive. */
	CDROM		= DRIVE_CDROM,
	/* The drive is RAM. */
	RAM			= DRIVE_RAMDISK
};
/* The available file attributes. */
enum FileAttributes : unsigned int {
	/* The file has no other attributes. */
	FileAttributeNormal		= FILE_ATTRIBUTE_NORMAL,
	/* The file attributes could not be obtained. */
	FileAttributeInvalid	= INVALID_FILE_ATTRIBUTES,

	/* The file is hidden. */
	FileAttributeHidden		= FILE_ATTRIBUTE_HIDDEN,
	/* The file cannot be written to. */
	FileAttributeReadOnly	= FILE_ATTRIBUTE_READONLY,
	/* The file is temporary. */
	FileAttributeTemporary	= FILE_ATTRIBUTE_TEMPORARY,
	/* The file is archived. */
	FileAttributeArchive	= FILE_ATTRIBUTE_ARCHIVE,
	/* The file is encrypted. */
	FileAttributeEncrypted	= FILE_ATTRIBUTE_ENCRYPTED,
	/* The file is a system file. */
	FileAttributeSystem		= FILE_ATTRIBUTE_SYSTEM,
	/* The file is a folder. */
	FileAttributeDirectory	= FILE_ATTRIBUTE_DIRECTORY
};
/* The available file share mode flags. */
enum FileShareModes : unsigned int {
	/* Other programs will not have access to the file. */
	FileShareNone			= 0x00000000,
	/* Other programs will be able to delete the file. */
	FileShareDelete			= FILE_SHARE_DELETE,
	/* Other programs will be able to read from the file. */
	FileShareRead			= FILE_SHARE_READ,
	/* Other programs will be able to write to the file. */
	FileShareWrite			= FILE_SHARE_WRITE
};
//================================================================
// CLASSES:
//================================================================
// File Directories:
//----------------------------------------------------------------
/* A class containing information about a specific file. */
class File {
	//=========== MEMBERS ============
public:
	// File Paths
	/* The full name of the file. */
	string Name;
	/* The short name of the file. */
	string NameShort;
	/* The proper name of the file. */
	string NameProper;
	/* The file extension. */
	string Extension;
	/* The directory containing this file. */
	string Directory;
	/* The full path of the file. */
	string Path;
	/* The path of the link. */
	string Link;
	
	// File Information
	/* The type of the file. */
	FileTypes Type;
	/* The type of the file that this file is linked to. */
	FileTypes LinkType;
	/* The type of the file if the file is a drive. */
	DriveTypes DriveType;
	/* The attributes of the file. */
	unsigned int Attributes;
	/* The size of the file. */
	unsigned long long FileSize;
	/* True if the file is the parent directory in a file list. */
	bool IsParentDirectory;

	//========= CONSTRUCTORS =========

	/* Constructs the default file. */
	File();
	/* Constructs and loads the specified file. */
	File(const string& path);
	
	//========== MANAGEMENT ==========

	/* Loads the specified file. */
	bool Load(const string& path);
	/* Refreshes the current file. */
	bool Refresh();
	
	//========= INFORMATION ==========

	/* Returns true if the file is hidden. */
	bool IsHidden();
	/* Returns true if the file is read only. */
	bool IsReadOnly();
	/* Returns true if the file is temporary. */
	bool IsTemporary();
	/* Returns true if the file is archived. */
	bool IsArchive();
	/* Returns true if the file is encrypted. */
	bool IsEncrypted();
	/* Returns true if the file is a system file. */
	bool IsSystem();
};
/* A class containing information about a specific directory. */
class Directory {
	//=========== MEMBERS ============
public:
	/* The file of this directory. */
	File DirectoryFile;
	/* The list of files in the directory. */
	vector<File> FileList;

	//========= CONSTRUCTORS =========

	/* Constructs the default directory. */
	Directory();
	/* Constructs and loads the specified directory. */
	Directory(const string& path);
	/* Constructs and loads the specified directory. */
	Directory(const File& file);

	//========== OPERATORS ===========

	/* Gets the file at the specified index in the directory. */
	File& operator[](int index);
	
	//========== MANAGEMENT ==========
	
	/* Loads the specified directory. */
	bool Load(const string& path);
	/* Loads the specified directory. */
	bool Load(const File& file);
	/* Refreshes the current directory. */
	bool Refresh();

	/* Removes all files with the specified file extension from the list. */
	void RemoveFileExtensions(const string& extension, bool exclude = false);
	/* Removes all files with the specified file types from the list. */
	void RemoveFileTypes(FileTypes type, bool exclude = false);
	/* Removes all files with the specified file attributes from the list. */
	void RemoveFileAttributes(unsigned int attribute, bool exclude = false);
	/* Removes the parent directory from the list. */
	void RemoveParentDirectory();
	
	//========= INFORMATION ==========

	/* Gets the number of files in the directory. */
	unsigned int NumFiles();
};
/* A static class for managing files. */
class FileManager {

	//========== CONSTANTS ===========
public:
	/* The path used to represent the computer directory. */
	const static string ComputerDirectory;

	//=========== MEMBERS ============
private:
	/* The structure to store the data about the file manager class . */
	struct _FileManagerMembers {

		/* The directory of the executable. */
		string ExecutablePath;
	};
	/* The collection of data in the file manager class. */
	static _FileManagerMembers _Data;
	
	//========= CONSTRUCTORS =========
public:
	/* Initializes and sets up the file manager. */
	static void Initialize();
	/* Uninitializes and cleans up the file manager. */
	static void Uninitialize();

	//========== MANAGEMENT ==========

	/* Creates a file at the specified location. */
	static bool CreateFile(const string& path, bool overwrite, bool folder);
	/* Creates a file at the specified location. */
	static bool CreateFile(const string& path, bool overwrite, unsigned int attributes);
	/* Copies the file to the new location. */
	static bool CopyFile(const string& path, const string& newPath, bool overwrite);
	/* Moves the file to the new location. */
	static bool MoveFile(const string& path, const string& newPath, bool overwrite);
	/* Deletes the file at the specified location. */
	static bool DeleteFile(const string& path);
	/* Renames the file at the specified location. */
	static bool RenameFile(const string& path, const string& name);
	
	//========= INFORMATION ==========
	
	/* Gets the path of the executable. */
	static string GetExecutablePath();
	/* Gets the directory of the executable. */
	static string GetExecutableDirectory();
	/* Gets the name of the executable. */
	static string GetExecutableName(bool includeExtension = true);

	/* Gets the working directory of the program. */
	static string GetWorkingDirectory();
	/* Sets the working directory of the program. */
	static bool SetWorkingDirectory(const string& path);

	/* Gets the size of the specified file. */
	static unsigned long long GetFileSize(const string& path);
	/* Gets the attributes of the specified file. */
	static unsigned int GetFileAttributes(const string& path);
	/* Gets the type of the specified file. */
	static FileTypes GetFileType(const string& path);
	/* Gets the type of the specified drive. */
	static DriveTypes GetDriveType(const string& path);

	/* Returns true if the name follows correct syntax for file names. */
	static bool IsValidFileName(const string& name);
	/* Returns true if the path follows correct syntax for file paths. */
	static bool IsValidFilePath(const string& path);
	/* Returns true if the specified file exists. */
	static bool FileExists(const string& path);
	/* Returns true if the specified directory exists. */
	static bool DirectoryExists(const string& path);
	/* Returns true if the specified path exists. */
	static bool PathExists(const string& path);

	/* Appends the specified path to the current path. */
	static string AppendPath(const string& path, const string& newPath);
	/* Fixes the path and fills in environment variables and shortcuts. */
	static string FixPath(const string& path);

	/* Gets the extension of the specified file. */
	static string GetFileExtension(const string& path);
	/* Gets the name of the specified file. */
	static string GetFileName(const string& path, bool includeExtension = true);
	/* Gets the name of the specified drive. */
	static string GetDriveName(const string& path, bool proper = false);
	/* Gets the directory of the specified file. */
	static string GetDirectory(const string& path);
	/* Gets the parent directory of the specified file. */
	static string GetParentDirectory(const string& path);
	/* Gets the link location of the specified file. */
	static string GetLinkLocation(const string& path);
	/* Gets the root directory of the specified path. */
	static string GetRoot(const string& path);
	/* Gets the current user profile directory */
	static string GetUserDirectory();

	// Clipboard
	/* Registers the specified clipboard format and returns it's id. */
	static unsigned int CreateClipboardFormat(const string& formatName);
	/* Returns true if the specified clipboard format is availabled. */
	static bool IsClipboardFormatAvailable(unsigned int clipboardFormat);
	
	// Resources
	/* Returns true if the specified resource exists. */
	static bool ResourceExists(const string& name, const string& type);
	/* Returns true if the specified resource exists. */
	static bool ResourceExists(const string& name, unsigned int typeId);
	/* Returns true if the specified resource exists. */
	static bool ResourceExists(unsigned int id, const string& type);
	/* Returns true if the specified resource exists. */
	static bool ResourceExists(unsigned int id, unsigned int typeId);

};
//================================================================
// END FILE MANAGER HEADER:
//================================================================
}}
#endif