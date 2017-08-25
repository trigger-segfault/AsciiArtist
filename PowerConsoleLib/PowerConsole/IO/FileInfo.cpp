/*=================================================================|
|* File:				FileInfo.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FileInfo.h"
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/IO/Path.h>

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

FileInfo::FileInfo()
	: name(""), extension(""), directory(""), path(""), shortcutPath(""),
	fileType(FileTypes::None), shortcutType(FileTypes::None),
	driveType(DriveTypes::None), attributes(FileAttributes::Invalid),
	fileSize(0UL), usedSize(0UL), lastModified(0UL), creationDate(0UL),
	parentDirectory(false), details(nullptr) {}
FileInfo::FileInfo(const std::string& path)
	: FileInfo() {
	load(path);
}

#pragma endregion
//=========== LOADING ============
#pragma region Loading

bool FileInfo::load(const std::string& filePath) {
	bool error = false;
	details = nullptr;

	if (filePath != Path::COMPUTER_DIRECTORY) {
		path = Path::getFullPath(filePath);
	}
	else {
		path = filePath;
	}
	fileType = Path::getFileType(path);
	shortcutType = FileTypes::None;
	driveType = DriveTypes::None;
	parentDirectory = false;

	if (filePath != Path::COMPUTER_DIRECTORY) {
		name = Path::getFileName(path);
	}
	else {
		name = "Computer";
	}
	extension = "";
	directory = "";
	shortcutPath = "";
	attributes = FileAttributes::Normal;
	fileSize = 0UL;
	usedSize = 0UL;

	if (fileType == FileTypes::File || fileType == FileTypes::Directory || fileType == FileTypes::Shortcut) {
		extension = Path::getExtension(path);
		directory = Path::getDirectory(path);
		attributes = Path::getFileAttributes(path);
		fileSize = Path::getFileSize(path);
		if (fileType == FileTypes::Shortcut) {
			shortcutPath = Path::resolveShortcut(path);
			shortcutType = Path::getFileType(shortcutPath);
			name = Path::removeExtension(name);
			if (shortcutPath.empty() || shortcutType == FileTypes::None)
				error = true;
		}
		FILETIME modified;
		FILETIME creation;
		HANDLE hFile = CreateFileA(path.c_str(), (DWORD)AccessRights::Read, (DWORD)FileShareModes::Read, nullptr, OPEN_EXISTING, 0, nullptr);
		GetFileTime(hFile, &creation, nullptr, &modified);
		creationDate = ((unsigned long long)creation.dwHighDateTime << 32) | creation.dwLowDateTime;
		lastModified = ((unsigned long long)modified.dwHighDateTime << 32) | modified.dwLowDateTime;
		CloseHandle(hFile);
		if (directory.empty() || attributes == FileAttributes::Invalid)
			error = true;
	}
	else if (fileType == FileTypes::Drive) {
		name = Path::getDriveName(path);
		driveType = Path::getDriveType(path);

		DiskUsage usage = Path::getDiskUsage(path);
		fileSize = usage.totalBytes;
		usedSize = usage.usedBytes;

		if (driveType == DriveTypes::None)
			error = true;
	}
	else if (fileType == FileTypes::Computer) {

	}
	else {
		error = true;
	}
	if (name.empty() || path.empty()) {
		error = true;
	}

	if (error) {
		attributes = FileAttributes::Invalid;
	}
	return !error;
}
bool FileInfo::refresh() {
	if (!path.empty()) {
		return load(path);
	}
	return false;
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

bool FileInfo::operator!() const {
	return isInvalid();
}
FileInfo::operator bool() const {
	return !isInvalid();
}

#pragma endregion
//========== ATTRIBUTES ==========
#pragma region Attributes

bool FileInfo::isInvalid() const {
	return getFlag(attributes, FileAttributes::Invalid);
}
bool FileInfo::isHidden() const {
	return getFlag(attributes, FileAttributes::Hidden);
}
bool FileInfo::isReadOnly() const {
	return getFlag(attributes, FileAttributes::ReadOnly);
}
bool FileInfo::isTemporary() const {
	return getFlag(attributes, FileAttributes::Temporary);
}
bool FileInfo::isArchive() const {
	return getFlag(attributes, FileAttributes::Archive);
}
bool FileInfo::isEncrypted() const {
	return getFlag(attributes, FileAttributes::Encrypted);
}
bool FileInfo::isSystem() const {
	return getFlag(attributes, FileAttributes::System);
}

#pragma endregion
//=================================================================|
