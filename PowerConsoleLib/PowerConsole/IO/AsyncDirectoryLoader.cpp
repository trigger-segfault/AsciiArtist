/*=================================================================|
|* File:				AsyncDirectoryLoader.cpp				  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AsyncDirectoryLoader.h"
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/IO/Path.h>

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

AsyncDirectoryLoader::AsyncDirectoryLoader()
	: directory(),
	hDir(nullptr),
	folderEnd(0),
	shortcutEnd(0),
	firstTimeoutPeriod(400),
	timeoutPeriod(6),
	keepFileFunction(nullptr),
	fileAddedFunction(nullptr) {}

#pragma endregion
//=========== LOADING ============
#pragma region Loading

bool AsyncDirectoryLoader::load(const std::string& path) {
	// Prevent an error window from popping up if there is no drive inserted.
	SetErrorMode(SEM_FAILCRITICALERRORS);
	int startTime = clock();
	loading = true;
	folderEnd = 0;
	shortcutEnd = 0;
	hDir = nullptr;

	FileInfo newDir = FileInfo();
	if (newDir.load(path)) {
		directory.directoryFile = std::move(newDir);
		directory.fileList.clear();
	}
	else {
		loading = false;
		return false;
	}

	if (directory.directoryFile.fileType == FileTypes::Computer) {
		HANDLE hVolume;
		DWORD numRead;
		char cVolume[MAX_PATH] = { 0 };
		char cName[MAX_PATH] = { 0 };
		hVolume = FindFirstVolumeA(cVolume, MAX_PATH);
		if (hVolume == INVALID_HANDLE_VALUE) {
			insertParent(FileInfo("C:\\"));
			loading = false;
			return false;
		}

		std::vector<FileInfo> driveFiles;
		do {
			GetVolumePathNamesForVolumeNameA(cVolume, cName, MAX_PATH, &numRead);
			std::string drive = cName;
			FileInfo driveFile = FileInfo();
			if (driveFile.load(drive)) {
				driveFiles.push_back(std::move(driveFile));
			}
		} while (FindNextVolumeA(hVolume, cVolume, MAX_PATH) != 0);
		FindVolumeClose(hVolume);

		// Sort drives
		std::sort(driveFiles.begin(), driveFiles.end(), [](const FileInfo& fileA, const FileInfo& fileB) {
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
		for (FileInfo& file : driveFiles) {
			insertDrive(std::move(file));
		}
		loading = false;
		return true;
	}
	else if (directory.directoryFile.fileType == FileTypes::Shortcut && (directory.directoryFile.shortcutType == FileTypes::File ||
		directory.directoryFile.shortcutType == FileTypes::Shortcut || directory.directoryFile.shortcutType == FileTypes::None)) {
		FileInfo parentFile = FileInfo(directory.directoryFile.directory);
		parentFile.name = "Back to shortcut directory";
		parentFile.parentDirectory = true;
		insertParent(std::move(parentFile));

		FileInfo resolvedShortcutPath = FileInfo();
		if (resolvedShortcutPath.load(Path::getDirectory(directory.directoryFile.shortcutPath))) {
			resolvedShortcutPath.name = "Go to file location";
			insertFolder(std::move(resolvedShortcutPath));
		}

		FileInfo resolvedShortcutFile = FileInfo();
		if (resolvedShortcutFile.load(directory.directoryFile.shortcutPath)) {
			insertFile(std::move(resolvedShortcutFile));
		}
		loading = false;
		return true;
	}
	else if (directory.directoryFile.fileType != FileTypes::None) {
		hDir = FindFirstFileA(Path::combine(directory.directoryFile.path, "*").c_str(), &file);
		if (hDir == nullptr || hDir == INVALID_HANDLE_VALUE) {
			FileInfo computerFile = FileInfo(Path::COMPUTER_DIRECTORY);
			computerFile.parentDirectory = true;
			directory.fileList.push_back(std::move(computerFile));
			loading = false;
			return directory.directoryFile.fileType == FileTypes::Drive;
		}
		else if (directory.directoryFile.fileType == FileTypes::Drive) {
			FileInfo computerFile = FileInfo(Path::COMPUTER_DIRECTORY);
			computerFile.parentDirectory = true;
			insertParent(std::move(computerFile));
		}
		continueLoading(startTime, firstTimeoutPeriod);
		return true;
	}
	loading = false;
	return false;
}
bool AsyncDirectoryLoader::load(const FileInfo& file) {
	return load(file.path);
}
bool AsyncDirectoryLoader::refresh() {
	if (!directory.directoryFile.path.empty()) {
		return load(directory.directoryFile.path);
	}
	return false;
}
void AsyncDirectoryLoader::continueLoading() {
	continueLoading(clock(), timeoutPeriod);
}
int AsyncDirectoryLoader::insert(FileInfo&& file) {
	if (file.fileType == FileTypes::Directory || file.fileType == FileTypes::Drive) {
		int start = 0;
		if (!directory.fileList.empty() && directory.fileList[0].parentDirectory)
			start++;
		for (int i = start; i < folderEnd; i++) {
			if (_stricmp(file.name.c_str(), directory.fileList[i].name.c_str()) < 0) {
				if (insertFinal(i, std::move(file))) {
					folderEnd++;
					shortcutEnd++;
					return i;
				}
			}
		}
		if (insertFinal(folderEnd, std::move(file))) {
			folderEnd++;
			shortcutEnd++;
			return folderEnd - 1;
		}
	}
	else if (file.shortcutType == FileTypes::Directory || file.shortcutType == FileTypes::Drive) {
		for (int i = folderEnd; i < shortcutEnd; i++) {
			if (_stricmp(file.name.c_str(), directory.fileList[i].name.c_str()) < 0) {
				if (insertFinal(i, std::move(file))) {
					shortcutEnd++;
					return i;
				}
			}
		}
		if (insertFinal(shortcutEnd, std::move(file))) {
			shortcutEnd++;
			return shortcutEnd - 1;
		}
	}
	else if (file.fileType == FileTypes::File || file.shortcutType == FileTypes::File) {
		for (int i = shortcutEnd; i < (int)directory.fileList.size(); i++) {
			if (_stricmp(file.name.c_str(), directory.fileList[i].name.c_str()) < 0) {
				if (insertFinal(i, std::move(file))) {
					return i;
				}
			}
		}
		if (insertFinal((int)directory.fileList.size(), std::move(file))) {
			return (int)directory.fileList.size() - 1;
		}
	}
	return -1;
}
void AsyncDirectoryLoader::remove(int index) {
	if (index < shortcutEnd)
		shortcutEnd--;
	if (index < folderEnd)
		folderEnd--;
	directory.fileList.erase(directory.fileList.begin() + index);
}
void AsyncDirectoryLoader::continueLoading(int startTime, int timeoutPeriod) {
	do {
		if (clock() - startTime > timeoutPeriod)
			return;

		std::string fileName = file.cFileName;
		if (getFlag(file.dwFileAttributes, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED))
			continue;

		if (getFlag(file.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY)) {
			if (fileName != ".") {
				FileInfo newFile = FileInfo();
				if (newFile.load(Path::combine(directory.directoryFile.path, fileName))) {
					if (fileName == "..") {
						newFile.name = "..";
						newFile.parentDirectory = true;
						insertParent(std::move(newFile));
					}
					else {
						WIN32_FIND_DATAA subFile;
						HANDLE hSubDir = FindFirstFileA(Path::combine(newFile.path, "*").c_str(), &subFile);
						if (hSubDir != nullptr && hSubDir != INVALID_HANDLE_VALUE) {
							insertFolder(std::move(newFile));
						}
						FindClose(hSubDir);
					}
				}
			}
		}
		else if (fileName[0] != '.' && fileName[0] != '$') {
			FileInfo newFile = FileInfo();
			if (newFile.load(Path::combine(directory.directoryFile.path, fileName))) {
				if (newFile.fileType == FileTypes::Shortcut &&
					(newFile.shortcutType == FileTypes::Directory || newFile.shortcutType == FileTypes::Drive)) {
					insertShortcut(std::move(newFile));
				}
				else {
					insertFile(std::move(newFile));
				}
			}
		}
	} while (FindNextFileA(hDir, &file) != 0);
	loading = false;
	FindClose(hDir);
}

bool AsyncDirectoryLoader::insertFinal(int index, FileInfo&& file) {
	bool keepFile = true;
	if ((bool)keepFileFunction) {
		keepFile = keepFileFunction(file);
	}
	if (keepFile) {
		directory.fileList.insert(directory.fileList.begin() + index, std::move(file));
		if ((bool)fileAddedFunction) {
			fileAddedFunction(index, directory.fileList[index]);
		}
		return true;
	}
	return false;
}

void AsyncDirectoryLoader::insertParent(FileInfo&& file) {
	if (insertFinal(0, std::move(file))) {
		folderEnd++;
		shortcutEnd++;
	}
}
void AsyncDirectoryLoader::insertFolder(FileInfo&& file) {
	if (insertFinal(folderEnd, std::move(file))) {
		folderEnd++;
		shortcutEnd++;
	}
}
void AsyncDirectoryLoader::insertShortcut(FileInfo&& file) {
	if (insertFinal(shortcutEnd, std::move(file))) {
		shortcutEnd++;
	}
}
void AsyncDirectoryLoader::insertFile(FileInfo&& file) {
	insertFinal((int)directory.fileList.size(), std::move(file));
}
void AsyncDirectoryLoader::insertDrive(FileInfo&& file) {
	insertFinal((int)directory.fileList.size(), std::move(file));
}

#pragma endregion
//=========== LISTING ============
#pragma region Listing

int AsyncDirectoryLoader::getFirstTimeoutPeriod() const {
	return firstTimeoutPeriod;
}
void AsyncDirectoryLoader::setFirstTimeoutPeriod(int firstTimeoutPeriod) {
	this->firstTimeoutPeriod = firstTimeoutPeriod;
}
int AsyncDirectoryLoader::getTimeoutPeriod() const {
	return timeoutPeriod;
}
void AsyncDirectoryLoader::setTimeoutPeriod(int timeoutPeriod) {
	this->timeoutPeriod = timeoutPeriod;
}
bool AsyncDirectoryLoader::isLoading() const {
	return loading;
}
DirectoryInfo& AsyncDirectoryLoader::getDirectory() {
	return directory;
}
const DirectoryInfo& AsyncDirectoryLoader::getDirectory() const {
	return directory;
}
void AsyncDirectoryLoader::setKeepFileFunction(KeepFileFunction func) {
	keepFileFunction = func;
}
void AsyncDirectoryLoader::setFileAddedFunction(FileAddedFunction func) {
	fileAddedFunction = func;
}

#pragma endregion
//=================================================================|
