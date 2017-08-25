/*=================================================================|
|* File:				Path.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Path.h"
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <windows.h>

using namespace PowerConsole::Helpers;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

DiskUsage::DiskUsage()
	: usedBytes(0UL), freeBytes(0UL), totalBytes(0UL) {}
DiskUsage::DiskUsage(unsigned long long usedBytes, unsigned long long freeBytes, unsigned long long totalBytes)
	: usedBytes(usedBytes), freeBytes(freeBytes), totalBytes(totalBytes) {}

//========== CONSTANTS ===========
#pragma region Constants

const std::string Path::COMPUTER_DIRECTORY = "Computer:";

#pragma endregion
//============ FILES =============
#pragma region Files

bool Path::fileExists(const std::string& path) {
	return (PathFileExistsA(path.c_str()) != 0) && (PathIsDirectoryA(path.c_str()) == 0);
}
bool Path::createFile(const std::string& path, bool overwrite) {
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, (unsigned int)FileShareModes::None, nullptr,
		(overwrite ? CREATE_ALWAYS : CREATE_NEW), (unsigned int)FileAttributes::Normal, nullptr);
	if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return true;
	}
	return false;
}
bool Path::createFile(const std::string& path, bool overwrite, FileAttributes attributes) {
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, (unsigned int)FileShareModes::None, nullptr,
		(overwrite ? CREATE_ALWAYS : CREATE_NEW), (unsigned int)attributes, nullptr);
	if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return true;
	}
	return false;
}
bool Path::copyFile(const std::string& path, const std::string& newPath, bool overwrite) {
	return CopyFileA(path.c_str(), newPath.c_str(), !overwrite) != 0;
}
bool Path::moveFile(const std::string& path, const std::string& newPath, bool overwrite) {
	return MoveFileExA(path.c_str(), newPath.c_str(), (overwrite ? MOVEFILE_REPLACE_EXISTING : 0)) != 0;
}
bool Path::deleteFile(const std::string& path, bool moveToRecycleBin) {
	if (moveToRecycleBin) {
		SHFILEOPSTRUCTA operation = { 0 };

		// http://stackoverflow.com/questions/11225667/i-cant-delete-file-using-shfileoperation
		// operation.pFrom is a list of files delimited by null characters.
		// The last file needs to end with 2 null characters in a row.
		char szFileName[MAX_PATH] = { 0 };
		strcpy_s(szFileName, path.c_str());
		szFileName[path.length() + 1] = '\0';

		operation.hwnd = NULL;
		operation.wFunc = FO_DELETE;
		operation.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_ALLOWUNDO;
		operation.pFrom = szFileName;

		return SHFileOperationA(&operation) == 0;
	}
	else {
		return DeleteFileA(path.c_str()) != 0;
	}
}
bool Path::renameFile(const std::string& path, const std::string& name) {
	std::string path2 = combine(getDirectory(path), name).c_str();
	return MoveFileA(path.c_str(), combine(getDirectory(path), name).c_str()) != 0;
}

#pragma endregion
//========= DIRECTORIES ==========
#pragma region Directories

bool Path::directoryExists(const std::string& path) {
	if (path == COMPUTER_DIRECTORY)
		return true;
	return (PathIsDirectoryA(path.c_str()) != 0);
}
bool Path::rootExists(const std::string& root) {
	return PathIsRootA(root.c_str()) != 0;
}
bool Path::createDirectory(const std::string& path) {
	return CreateDirectoryA(path.c_str(), nullptr) != 0;
}
bool Path::deleteDirectory(const std::string& path, bool moveToRecycleBin) {
	char szFileName[MAX_PATH] = { 0 };
	strcpy_s(szFileName, path.c_str());
	szFileName[path.length() + 1] = '\0';

	SHFILEOPSTRUCTA operation = { 0 };
	operation.hwnd = NULL;
	operation.wFunc = FO_DELETE;
	operation.fFlags = FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT;
	operation.pFrom = szFileName;

	if (moveToRecycleBin)
		operation.fFlags |= FOF_ALLOWUNDO;

	return SHFileOperationA(&operation) == 0;
}

#pragma endregion
//========== FILE INFO ===========
#pragma region File Info

unsigned long long Path::getFileSize(const std::string& path) {
	if (path == COMPUTER_DIRECTORY)
		return 0UL;
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, (unsigned int)FileShareModes::None, nullptr,
		OPEN_EXISTING, (unsigned int)FileAttributes::Normal, nullptr);
	if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE) {
		LARGE_INTEGER size;
		if (GetFileSizeEx(hFile, &size) == 0)
			size.QuadPart = 0L;
		CloseHandle(hFile);
		return (unsigned long long)size.QuadPart;
	}
	return 0UL;
}
FileAttributes Path::getFileAttributes(const std::string& path) {
	if (path == COMPUTER_DIRECTORY)
		return FileAttributes::Normal;
	return (FileAttributes)GetFileAttributesA(path.c_str());
}
FileTypes Path::getFileType(const std::string& path) {
	if (path == COMPUTER_DIRECTORY)
		return FileTypes::Computer;
	std::string fullPath = getFullPath(path);

	if (PathIsRootA(fullPath.c_str()) != 0) {
		return FileTypes::Drive;
	}

	FileAttributes attributes = getFileAttributes(path);
	if (attributes == FileAttributes::Invalid)
		return FileTypes::None;
	else if (getUIntFlag(attributes, FileAttributes::Directory))
		return FileTypes::Directory;
	else if (getExtension(fullPath) == ".lnk")
		return FileTypes::Shortcut;
	else
		return FileTypes::File;
}
DriveTypes Path::getDriveType(const std::string& path) {
	DriveTypes type = (DriveTypes)GetDriveTypeA(path.c_str());
	switch (type) {
	case DriveTypes::Local:
	case DriveTypes::Removable:
	case DriveTypes::Network:
	case DriveTypes::CDROM:
	case DriveTypes::RAM:		return type;
	default:					return DriveTypes::None;
	}
}
std::string Path::getFileVersionString(const std::string& path) {
	// http://stackoverflow.com/questions/940707/how-do-i-programatically-get-the-version-of-a-dll-or-exe-file
	std::string result = "";
	LPBYTE buffer = nullptr;
	DWORD verSize = GetFileVersionInfoSizeA(path.c_str(), nullptr);
	if (verSize != 0) {
		LPSTR verData = new char[verSize];
		UINT size = 0;

		if (GetFileVersionInfoA(path.c_str(), 0, verSize, verData) != 0) {
			if (VerQueryValueA(verData, "\\", (VOID FAR* FAR*)&buffer, &size) != 0) {
				if (size != 0) {
					VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)buffer;
					if (verInfo->dwSignature == 0xfeef04bd) {
						result += std::to_string((verInfo->dwFileVersionMS >> 16) & 0xFFFF) + ".";
						result += std::to_string((verInfo->dwFileVersionMS >>  0) & 0xFFFF) + ".";
						result += std::to_string((verInfo->dwFileVersionLS >> 16) & 0xFFFF) + ".";
						result += std::to_string((verInfo->dwFileVersionLS >>  0) & 0xFFFF);
					}
				}
			}
		}
		delete[] verData;
	}
	return result;
}
unsigned long long Path::getFileLastModified(const std::string& path) {
	FILETIME modified;
	HANDLE hFile = CreateFileA(path.c_str(), (DWORD)AccessRights::Read, (DWORD)FileShareModes::Read, nullptr, OPEN_EXISTING, 0, nullptr);
	GetFileTime(hFile, nullptr, nullptr, &modified);
	unsigned long long lastModified = ((unsigned long long)modified.dwHighDateTime << 32) | modified.dwLowDateTime;
	CloseHandle(hFile);
	return lastModified;
}
unsigned long long Path::getFileCreationDate(const std::string& path) {
	FILETIME creation;
	HANDLE hFile = CreateFileA(path.c_str(), (DWORD)AccessRights::Read, (DWORD)FileShareModes::Read, nullptr, OPEN_EXISTING, 0, nullptr);
	GetFileTime(hFile, &creation, nullptr, nullptr);
	unsigned long long creationDate = ((unsigned long long)creation.dwHighDateTime << 32) | creation.dwLowDateTime;
	CloseHandle(hFile);
	return creationDate;
}

#pragma endregion
//========== LOCATIONS ===========
#pragma region Locations

std::string Path::getCurrentDirectory() {
	char cPath[MAX_PATH] = { 0 };
	if (GetCurrentDirectoryA(MAX_PATH, cPath) != 0) {
		return cPath;
	}
	return "";
}
bool Path::setCurrentDirectory(const std::string& path) {
	return SetCurrentDirectoryA(path.c_str()) != 0;
}
std::string Path::GetUserDirectory() {
	char cUser[MAX_PATH] = { 0 };
	if (GetEnvironmentVariableA("USERPROFILE", cUser, MAX_PATH) != 0) {
		return cUser;
	}
	return "";
}
std::string Path::getExecutablePath() {
	char cExecutablePath[MAX_PATH] = { 0 };
	if (GetModuleFileNameA(GetModuleHandleA(nullptr), cExecutablePath, MAX_PATH) != 0) {
		return cExecutablePath;
	}
	return "";
}
std::string Path::getExecutableDirectory() {
	return getDirectory(getExecutablePath());
}
std::string Path::getTempPath() {
	char cPath[MAX_PATH] = { 0 };
	if (GetTempPathA(MAX_PATH, cPath)) {
		return cPath;
	}
	return "";
}
std::string Path::getTempFileName(const std::string& path, const std::string& prefix) {
	char cFileName[MAX_PATH] = { 0 };
	if (GetTempFileNameA(path.c_str(), prefix.c_str(), 0, cFileName)) {
		return cFileName;
	}
	return "";
}

#pragma endregion
//============ NAMING ============
#pragma region Naming

bool Path::isValidFileName(const std::string& name) {
	const std::string invalidChars = "<>:\"/\\|?*";
	for (int i = 0; i < (int)name.length(); i++) {
		unsigned char c = name[i];
		if (c < 0x20)
			return false;
		for (int j = 0; j < (int)invalidChars.length(); j++) {
			if (c == invalidChars[j])
				return false;
		}
	}
	return true;
}
bool Path::isValidPath(const std::string& path) {
	const std::string invalidChars = "<>\"|";
	for (int i = 0; i < (int)path.length(); i++) {
		unsigned char c = path[i];
		if (c < 0x20)
			return false;
		for (int j = 0; j < (int)invalidChars.length(); j++) {
			if (c == invalidChars[j])
				return false;
		}
	}
	return true;
}

#pragma endregion
//============ PATHS =============
#pragma region paths

bool Path::pathExists(const std::string& path) {
	if (path == COMPUTER_DIRECTORY)
		return true;
	return PathFileExistsA(path.c_str()) != 0;
}
std::string Path::combine(const std::string& path1, const std::string& path2) {
	char cPath[MAX_PATH] = { 0 };
	if (PathCombineA(cPath, path1.c_str(), path2.c_str()) != nullptr) {
		return cPath;
	}
	return "";
}
std::string Path::combine(const std::string& path1, const std::string& path2, const std::string& path3) {
	char cPath[MAX_PATH] = { 0 };
	if (PathCombineA(cPath, path1.c_str(), path2.c_str()) != nullptr) {
		return combine(cPath, path3);
	}
	return "";
}
std::string Path::combine(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4) {
	char cPath[MAX_PATH] = { 0 };
	if (PathCombineA(cPath, path1.c_str(), path2.c_str()) != nullptr) {
		return combine(cPath, path3, path4);
	}
	return "";
}
std::string Path::getFullPath(const std::string& path) {
	char cPath[MAX_PATH] = { 0 };
	if (GetFullPathNameA(path.c_str(), MAX_PATH, cPath, 0) != 0) {
		return cPath;
	}
	return "";
}
std::string Path::expandEnvironmentPath(const std::string& path) {
	char cPath[MAX_PATH] = { 0 };
	if (ExpandEnvironmentStringsA(path.c_str(), cPath, MAX_PATH) != 0) {
		return cPath;
	}
	return "";
}
std::string Path::getDirectory(const std::string& path) {
	if (path == COMPUTER_DIRECTORY)
		return "";
	std::string fullPath = getFullPath(path);
	if (!fullPath.empty()) {
		for (int i = path.length() - 1; i >= 0; i--) {
			if (path[i] == '\\' || path[i] == '/') {
				return getFullPath(path.substr(0, i + 1));
			}
		}
		return getCurrentDirectory();
	}
	return "";
}
std::string Path::getRoot(const std::string& path) {
	char cPath[MAX_PATH] = { 0 };
	path.copy(cPath, MAX_PATH);
	if (PathStripToRootA(cPath) != 0) {
		return cPath;
	}
	return "";
}
std::string Path::getFileName(const std::string& path) {
	return PathFindFileNameA(path.c_str());
}
std::string Path::getFileNameWithoutExtension(const std::string& path) {
	std::string fileName = PathFindFileNameA(path.c_str());
	char cFileName[MAX_PATH] = { 0 };
	fileName.copy(cFileName, MAX_PATH);
	PathRemoveExtensionA(cFileName);
	return cFileName;
}
std::string Path::getDriveName(const std::string& path) {
	DriveTypes type = getDriveType(path);
	std::string fixedPath = getFullPath(path);
	if (fixedPath.length() < 1)
		return "";
	switch (type) {
	case DriveTypes::Local:		return "Local Disk (" + fixedPath.substr(0, 2) + ")";
	case DriveTypes::Removable:	return "Removable Disk (" + fixedPath.substr(0, 2) + ")";
	case DriveTypes::Network:	return "Network Drive (" + fixedPath.substr(0, 2) + ")";
	case DriveTypes::CDROM:		return "CD Drive (" + fixedPath.substr(0, 2) + ")";
	case DriveTypes::RAM:		return "RAM Disk (" + fixedPath.substr(0, 2) + ")";
	default:					return "";
	}
}
std::string Path::getExtension(const std::string& path) {
	return PathFindExtensionA(path.c_str());
}
std::string Path::renameExtension(const std::string& path, const std::string& extension) {
	char cPath[MAX_PATH] = { 0 };
	path.copy(cPath, MAX_PATH);
	if (PathRenameExtensionA(cPath, extension.c_str()) != 0) {
		return cPath;
	}
	return "";
}
std::string Path::removeExtension(const std::string& path) {
	char cPath[MAX_PATH] = { 0 };
	path.copy(cPath, MAX_PATH);
	PathRemoveExtensionA(cPath);
	return cPath;
}
bool Path::hasExtension(const std::string& path) {
	return !std::string(PathFindExtensionA(path.c_str())).empty();
}
std::string Path::resolveShortcut(const std::string& path) {
	//static bool coInitialized = false;
	//HRESULT result;

	// This only needs to be done once in the application's lifetime.
	/*if (!coInitialized) {
		result = CoInitialize(nullptr);
		if (!SUCCEEDED(result))
			return ""; // Return an error
		else
			coInitialized = true;
	}*/


	IShellLinkW* ipShellLink = nullptr;
	IPersistFile* ipPersistFile = nullptr;
	std::string finalPath = "";
	wchar_t wLink[MAX_PATH] = { 0 };
	wchar_t wPath[MAX_PATH] = { 0 };

	CoInitialize(nullptr);

	if (FAILED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**)&ipShellLink)))
		goto Return;

	if (FAILED(ipShellLink->QueryInterface(IID_IPersistFile, (void**)&ipPersistFile)))
		goto ReleaseShellLink;

	MultiByteToWideChar(0, 0, path.c_str(), -1, wLink, MAX_PATH);
	if (FAILED(ipPersistFile->Load(wLink, 0)))
		goto ReleasePersistFile;

	if (FAILED(ipShellLink->GetPath(wPath, MAX_PATH, nullptr, 0)))
		goto ReleasePersistFile;

	finalPath = wstr_to_str(wPath);

ReleasePersistFile:
	ipPersistFile->Release();
ReleaseShellLink:
	ipShellLink->Release();
Return:
	CoUninitialize();
	return finalPath;

	/*IShellLinkW* psl = nullptr;
	std::string finalPath = "";

	result = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
	if (SUCCEEDED(result)) {
		IPersistFile* ppf;
		result = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (SUCCEEDED(result)) {
			wchar_t wLink[MAX_PATH] = { 0 };
			MultiByteToWideChar(0, 0, path.c_str(), -1, wLink, MAX_PATH);

			result = ppf->Load(wLink, 0);
			if (SUCCEEDED(result)) {
				wchar_t wPath[MAX_PATH] = { 0 };
				char cPath[MAX_PATH] = { 0 };
				psl->GetPath(wPath, MAX_PATH, nullptr, 0); //SLGP_UNCPRIORITY

				WideCharToMultiByte(0, 0, wPath, -1, cPath, MAX_PATH, nullptr, nullptr);
				finalPath = cPath;
			}
		}
		psl->Release();
	}
Return:
	return finalPath;*/
}
std::string Path::getFileSizeString(unsigned long long size) {
	if (size < 1024)
		return std::to_string(size) + " B ";
	else if ((size /= 1024) < 1024)
		return std::to_string(size) + " KB";
	else if ((size /= 1024) < 1024)
		return std::to_string(size) + " MB";
	else if ((size /= 1024) < 1024)
		return std::to_string(size) + " GB";
	else if ((size /= 1024) < 1024)
		return std::to_string(size) + " TB";
	else
		return std::to_string(size / 1024) + " PB";
	
}
std::string Path::getFileSizeString(const std::string& path) {
	return getFileSizeString(getFileSize(path));
}
std::string Path::getFileTimeString(unsigned long long time, bool showDate, bool showFullYear, bool showTime, bool showAMPM) {
	FILETIME fileTime;
	SYSTEMTIME systemTime, localTime;
	fileTime.dwLowDateTime = (DWORD)(time & 0xFFFFFFFF);
	fileTime.dwHighDateTime = (DWORD)((time >> 32) & 0xFFFFFFFF);
	FileTimeToSystemTime(&fileTime, &systemTime);
	SystemTimeToTzSpecificLocalTime(nullptr, &systemTime, &localTime);

	std::string ampm = (showAMPM ? (localTime.wHour < 12 ? "AM" : "PM") : "");
	if (showAMPM)
		localTime.wHour = (localTime.wHour == 0 ? 12 : localTime.wHour % 12);
	if (!showFullYear)
		localTime.wYear %= 100;
	
	return	(showDate ? std::to_string(localTime.wMonth) + "/" +
						std::to_string(localTime.wDay) + "/" +
						std::to_string(localTime.wYear) + " " : "") +
			(showTime ? std::to_string(localTime.wHour) + ":" +
						(localTime.wMinute < 10 ? "0" : "") +
						std::to_string(localTime.wMinute) +
						ampm : "");
}
DiskUsage Path::getDiskUsage(const std::string& path) {
	ULARGE_INTEGER totalBytes;
	ULARGE_INTEGER freeBytes;
	if (GetDiskFreeSpaceExA(path.c_str(), nullptr, &totalBytes, &freeBytes) != 0) {
		return DiskUsage(totalBytes.QuadPart - freeBytes.QuadPart, freeBytes.QuadPart, totalBytes.QuadPart);
	}
	return DiskUsage();
}
std::string Path::getSpecialFolderPath(GUID folderId) {
	wchar_t* wstr;
	if (SHGetKnownFolderPath(folderId, KNOWN_FOLDER_FLAG::KF_FLAG_DEFAULT, nullptr, &wstr) == S_OK) {
		std::string path = wstr_to_str(std::wstring(wstr));
		CoTaskMemFree(wstr);
		return path;
	}
	return "";
}

#pragma endregion

//=================================================================|
