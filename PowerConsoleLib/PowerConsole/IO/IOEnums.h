/*=================================================================|
|* File:				FileEnums.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef IO_ENUMS_H
#define IO_ENUMS_H

#define NOMINMAX
#include <windows.h>
#include <PowerConsole/Declarations.h>

namespace PowerConsole::IO {
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/

/* The different types a file can be. */
enum class FileTypes : unsigned char {
	/* There is no file. */
	None = 0,

	/* The file is a normal file. */
	File = 1,
	/* The file is a link to another file. */
	Shortcut = 2,
	/* The file is a folder. */
	Directory = 3,
	/* The file is a drive. */
	Drive = 4,
	/* The file is the computer. */
	Computer = 5
};
/* The types a drive can be. */
enum class DriveTypes : unsigned char {
	/* The file is not a drive. */
	None = 0,

	/* The drive is fixed such as a hard drive. */
	Local = DRIVE_FIXED,
	/* The drive is a CD drive. */
	CDROM = DRIVE_CDROM,
	/* The drive is removable such as a usb drive. */
	Removable = DRIVE_REMOVABLE,
	/* The drive is a network drive. */
	Network = DRIVE_REMOTE,
	/* The drive is RAM. */
	RAM = DRIVE_RAMDISK
};
/* The available file attributes. */
enum class FileAttributes : unsigned int {
	/* No file attributes. */
	None = 0,

	/* The file has no other attributes. */
	Normal = FILE_ATTRIBUTE_NORMAL,
	/* The file attributes could not be obtained. */
	Invalid = INVALID_FILE_ATTRIBUTES,

	/* The file is hidden. */
	Hidden = FILE_ATTRIBUTE_HIDDEN,
	/* The file cannot be written to. */
	ReadOnly = FILE_ATTRIBUTE_READONLY,
	/* The file is temporary. */
	Temporary = FILE_ATTRIBUTE_TEMPORARY,
	/* The file is archived. */
	Archive = FILE_ATTRIBUTE_ARCHIVE,
	/* The file is encrypted. */
	Encrypted = FILE_ATTRIBUTE_ENCRYPTED,
	/* The file is a system file. */
	System = FILE_ATTRIBUTE_SYSTEM,
	/* The file is a folder. */
	Directory = FILE_ATTRIBUTE_DIRECTORY
};
/* The available file share mode flags. */
enum class FileShareModes : unsigned char {
	/* Other programs will not have access to the file. */
	None = 0,
	/* Other programs will be able to delete the file. */
	Delete = FILE_SHARE_DELETE,
	/* Other programs will be able to read from the file. */
	Read = FILE_SHARE_READ,
	/* Other programs will be able to write to the file. */
	Write = FILE_SHARE_WRITE
};

/* The different types of input and output modes. */
enum class AccessRights : unsigned int {
	/* The IO device can read from this stream. */
	Read = GENERIC_READ,
	/* The IO device can write to this stream. */
	Write = GENERIC_WRITE,
	/* The IO device can read and write to this stream. */
	ReadWrite = GENERIC_READ | GENERIC_WRITE
};

DEFINE_ENUM_FLAG_OPERATORS(FileAttributes)
DEFINE_ENUM_FLAG_OPERATORS(FileShareModes)

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
