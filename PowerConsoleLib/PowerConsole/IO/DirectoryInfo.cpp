/*=================================================================|
|* File:				DirectoryInfo.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "DirectoryInfo.h"
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/IO/Path.h>

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

DirectoryInfo::DirectoryInfo()
	: directoryFile() { }
DirectoryInfo::DirectoryInfo(const std::string& path)
	: DirectoryInfo() {
	load(path);
}
DirectoryInfo::DirectoryInfo(const FileInfo& file)
	: DirectoryInfo() {
	load(file);
}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

FileInfo& DirectoryInfo::operator[](int index) {
	if (index < 0 || index >= (int)fileList.size()) {
		throw std::out_of_range("Index is not a valid file index in 'DirectoryInfo::operator[]'");
	}
	return fileList[index];
}

#pragma endregion
//=========== LOADING ============
#pragma region Loading

bool DirectoryInfo::load(const std::string& path) {
	// Prevent an error window from popping up if there is no drive inserted.
	SetErrorMode(SEM_FAILCRITICALERRORS);

	FileInfo newDir = FileInfo();
	if (newDir.load(path)) {
		directoryFile = newDir;
		fileList.clear();
	}
	else {
		return false;
	}

	if (directoryFile.fileType == FileTypes::Computer) {
		HANDLE hVolume;
		DWORD numRead;
		char cVolume[MAX_PATH] = { 0 };
		char cName[MAX_PATH] = { 0 };
		hVolume = FindFirstVolumeA(cVolume, MAX_PATH);
		if (hVolume == INVALID_HANDLE_VALUE) {
			fileList.push_back(FileInfo("C:\\"));
			return false;
		}

		do {
			GetVolumePathNamesForVolumeNameA(cVolume, cName, MAX_PATH, &numRead);
			std::string drive = cName;
			FileInfo driveFile = FileInfo();
			if (driveFile.load(drive)) {
				fileList.push_back(driveFile);
			}
		} while (FindNextVolumeA(hVolume, cVolume, MAX_PATH) != 0);
		FindVolumeClose(hVolume);

		// Sort drives
		std::sort(fileList.begin(), fileList.end(), [](const FileInfo& fileA, const FileInfo& fileB) {
			if (fileA.driveType == fileB.driveType) {
				return fileA.path.compare(fileB.path) < 0;
			}
			else {
				int orderA = 6, orderB = 6;
				switch (fileA.driveType) {
				case DriveTypes::Local: orderA = 1; break;
				case DriveTypes::CDROM: orderA = 2; break;
				case DriveTypes::Removable: orderA = 3; break;
				case DriveTypes::Network: orderA = 4; break;
				case DriveTypes::RAM: orderA = 5; break;
				}
				switch (fileB.driveType) {
				case DriveTypes::Local: orderB = 1; break;
				case DriveTypes::CDROM: orderB = 2; break;
				case DriveTypes::Removable: orderB = 3; break;
				case DriveTypes::Network: orderB = 4; break;
				case DriveTypes::RAM: orderB = 5; break;
				}
				return orderA < orderB;
			}
		});

		return true;
	}
	else if (directoryFile.fileType == FileTypes::Shortcut && (directoryFile.shortcutType == FileTypes::File ||
		directoryFile.shortcutType == FileTypes::Shortcut || directoryFile.shortcutType == FileTypes::None)) {
		FileInfo parentFile = FileInfo(directoryFile.directory);
		parentFile.name = "Back to shortcut directory";
		parentFile.parentDirectory = true;
		fileList.push_back(parentFile);

		FileInfo resolvedShortcutFile = FileInfo();
		if (resolvedShortcutFile.load(directoryFile.shortcutPath)) {
			fileList.push_back(resolvedShortcutFile);
		}

		return true;
	}
	else if (directoryFile.fileType != FileTypes::None) {
		std::vector<FileInfo> folders;
		std::vector<FileInfo> shortcuts;

		HANDLE hDir;
		WIN32_FIND_DATAA file;

		hDir = FindFirstFileA(Path::combine(directoryFile.path, "*").c_str(), &file);
		if (hDir == nullptr || hDir == INVALID_HANDLE_VALUE) {
			FileInfo computerFile = FileInfo(Path::COMPUTER_DIRECTORY);
			computerFile.parentDirectory = true;
			fileList.push_back(computerFile);
			return directoryFile.fileType == FileTypes::Drive;
		}
		do {
			std::string fileName = file.cFileName;
			if (getFlag(file.dwFileAttributes, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED))
				continue;

			if (getFlag(file.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY)) {
				if (fileName != ".") {
					FileInfo newFile = FileInfo();
					if (newFile.load(Path::combine(directoryFile.path, fileName))) {
						if (fileName == "..") {
							newFile.name = "..";
							newFile.parentDirectory = true;
							folders.insert(folders.begin(), std::move(newFile));
						}
						else {
							folders.push_back(std::move(newFile));
						}
					}
				}
			}
			else if (fileName[0] != '.' && fileName[0] != '$') {
				FileInfo newFile = FileInfo();
				if (newFile.load(Path::combine(directoryFile.path, fileName))) {
					if (newFile.fileType == FileTypes::Shortcut &&
						(newFile.shortcutType == FileTypes::Directory || newFile.shortcutType == FileTypes::Drive)) {
						shortcuts.push_back(std::move(newFile));
					}
					else {
						fileList.push_back(std::move(newFile));
					}
				}
			}
		} while (FindNextFileA(hDir, &file) != 0);
		FindClose(hDir);

		if (directoryFile.fileType == FileTypes::Drive) {
			FileInfo computerFile = FileInfo(Path::COMPUTER_DIRECTORY);
			computerFile.parentDirectory = true;
			folders.insert(folders.begin(), computerFile);
		}

		for (int i = (int)shortcuts.size() - 1; i >= 0; i--) {
			fileList.insert(fileList.begin(), std::move(shortcuts[i]));
		}
		for (int i = (int)folders.size() - 1; i >= 0; i--) {
			if (folders[i].fileType != FileTypes::Computer) {
				hDir = FindFirstFileA(Path::combine(folders[i].fileType == FileTypes::Shortcut ? folders[i].shortcutPath : folders[i].path, "*").c_str(), &file);
				if (hDir != nullptr && hDir != INVALID_HANDLE_VALUE) {
					fileList.insert(fileList.begin(), std::move(folders[i]));
				}
				FindClose(hDir);
			}
			else {
				fileList.insert(fileList.begin(), std::move(folders[i]));
			}
		}

		return true;
	}

	return false;
}
bool DirectoryInfo::load(const FileInfo& file) {
	return load(file.path);
}
bool DirectoryInfo::refresh() {
	if (!directoryFile.path.empty()) {
		return load(directoryFile.path);
	}
	return false;
}

#pragma endregion
//========== FILTERING ===========
#pragma region Filtering

void DirectoryInfo::removeFileExtensions(const std::string& extension, bool exclude) {
	for (int i = 0; i < (int)fileList.size(); i++) {
		if ((fileList[i].fileType == FileTypes::File || fileList[i].fileType == FileTypes::Shortcut)) {
			if (fileList[i].fileType == FileTypes::Shortcut) {
				if ((Path::getExtension(fileList[i].shortcutPath) == extension) != exclude) {
					fileList.erase(fileList.begin() + i);
					i--;
				}
			}
			else if ((fileList[i].extension == extension) != exclude) {
				fileList.erase(fileList.begin() + i);
				i--;
			}
		}
	}
}
void DirectoryInfo::removeFileExtensions(const std::vector<std::string>& extensions, bool exclude) {
	for (int i = 0; i < (int)fileList.size(); i++) {
		if ((fileList[i].fileType == FileTypes::File || fileList[i].fileType == FileTypes::Shortcut)) {
			bool safe = !exclude;
			for (const std::string& extension : extensions) {
				if (fileList[i].fileType == FileTypes::Shortcut) {
					if (fileList[i].shortcutType != FileTypes::File) {
						safe = true;
						break;
					}
					if (Path::getExtension(fileList[i].shortcutPath) == extension) {
						safe = exclude;
						break;
					}
				}
				else if (fileList[i].extension == extension) {
					safe = exclude;
					break;
				}
			}
			if (!safe) {
				fileList.erase(fileList.begin() + i);
				i--;
			}
		}
	}
}
void DirectoryInfo::removeFileTypes(FileTypes type, bool exclude) {
	for (int i = 0; i < (int)fileList.size(); i++) {
		if (fileList[i].fileType == FileTypes::Shortcut) {
			if ((fileList[i].shortcutType == type) != exclude) {
				fileList.erase(fileList.begin() + i);
				i--;
			}
		}
		else if ((fileList[i].fileType == type) != exclude) {
			fileList.erase(fileList.begin() + i);
			i--;
		}
	}
}
void DirectoryInfo::removeShortcutTypes(FileTypes type, bool exclude) {
	for (int i = 0; i < (int)fileList.size(); i++) {
		if (fileList[i].fileType == FileTypes::Shortcut && (fileList[i].shortcutType == type) != exclude) {
			fileList.erase(fileList.begin() + i);
			i--;
		}
	}
}
void DirectoryInfo::removeFileAttributes(FileAttributes attributes, bool exclude) {
	for (int i = 0; i < (int)fileList.size(); i++) {
		if (((fileList[i].attributes & attributes) != FileAttributes::None && !fileList[i].parentDirectory) != exclude) {
			fileList.erase(fileList.begin() + i);
			i--;
		}
	}
}
void DirectoryInfo::removeParentDirectory() {
	if (!fileList.empty()) {
		if (fileList[0].parentDirectory)
			fileList.erase(fileList.begin());
	}
}

#pragma endregion
//=========== LISTING ============
#pragma region Listing

int DirectoryInfo::fileCount() const {
	return (int)fileList.size();
}

#pragma endregion

//=================================================================|
