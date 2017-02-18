//================================================================
// FILE MANAGER CPP:
//================================================================
#include "stdafx.h"
#include "FileManager.h"

using namespace std;
using namespace PowerConsole::Windows;
//================================================================
// DEFINITIONS:
//================================================================
/* Returns true if the flag is applied to the attributes. */
#define GetFlag(attributes, flag)	(((attributes) & (flag)) == (flag))

//================================================================
// CLASSES:
//================================================================
// File:
//----------------------------------------------------------------
//========== MANAGEMENT ==========
bool PowerConsole::Windows::File::Load(const string& path) {
	Path			= FileManager::FixPath(path);
	Type			= FileManager::GetFileType(path);
	LinkType		= FileTypes::None;
	DriveType		= DriveTypes::None;
	IsParentDirectory	= false;
	
	Name			= FileManager::GetFileName(Path, true);
	NameShort		= FileManager::GetFileName(Path, Type == FileTypes::Directory);
	NameProper		= FileManager::GetFileName(Path, Type == FileTypes::Directory);
	Extension		= "";
	Directory		= "";
	Link			= "";
	Attributes		= FileAttributeNormal;
	FileSize		= 0LU;

	if (Type == FileTypes::File || Type == FileTypes::Directory || Type == FileTypes::Link) {
		Extension		= FileManager::GetFileExtension(Path);
		Directory		= FileManager::GetDirectory(Path);
		Attributes		= FileManager::GetFileAttributes(Path);
		FileSize		= FileManager::GetFileSize(Path);
		if (Type == FileTypes::Link) {
			Link		= FileManager::GetLinkLocation(Path);
			LinkType	= FileManager::GetFileType(Link);
			if (Link.empty() || LinkType == FileTypes::None)
				return false;
		}
		if (Directory.empty() || Attributes == FileAttributeInvalid)
			return false;
	}
	else if (Type == FileTypes::Drive) {
		Name		= FileManager::GetDriveName(Path, false);
		NameShort	= FileManager::GetDriveName(Path, false);
		NameProper	= FileManager::GetDriveName(Path, true);
		DriveType	= FileManager::GetDriveType(Path);

		if (DriveType == DriveTypes::None)
			return false;
	}
	else if (Type == FileTypes::Computer) {

	}
	else {
		return false;
	}
	if (Name.empty() || Path.empty())
		return false;

	return true;
}
//========= CONSTRUCTORS =========
PowerConsole::Windows::File::File() {
	this->Name			= "";
	this->NameShort		= "";
	this->NameProper	= "";
	this->Path			= "";

	this->Extension		= "";
	this->Directory		= "";
	this->Path			= "";
	this->Link			= "";

	this->Type			= FileTypes::None;
	this->LinkType		= FileTypes::None;
	this->DriveType		= DriveTypes::None;
	this->Attributes	= FileAttributeInvalid;
	this->FileSize		= 0LU;
	this->IsParentDirectory	= false;
}
PowerConsole::Windows::File::File(const string& path) {
	Load(path);
}
//========== MANAGEMENT ==========
bool PowerConsole::Windows::File::Refresh() {
	return Load(Path);
}
//========= INFORMATION ==========
bool PowerConsole::Windows::File::IsHidden() {
	return GetFlag(Attributes, FileAttributeHidden);
}
bool PowerConsole::Windows::File::IsReadOnly() {
	return GetFlag(Attributes, FileAttributeReadOnly);
}
bool PowerConsole::Windows::File::IsTemporary() {
	return GetFlag(Attributes, FileAttributeTemporary);
}
bool PowerConsole::Windows::File::IsArchive() {
	return GetFlag(Attributes, FileAttributeArchive);
}
bool PowerConsole::Windows::File::IsEncrypted() {
	return GetFlag(Attributes, FileAttributeEncrypted);
}
bool PowerConsole::Windows::File::IsSystem() {
	return GetFlag(Attributes, FileAttributeSystem);
}
//----------------------------------------------------------------
// Directory:
//----------------------------------------------------------------
//========== MANAGEMENT ==========
bool PowerConsole::Windows::Directory::Load(const string& path) {
	File newDir = File();
	if (newDir.Load(path)) {
		DirectoryFile = newDir;
		FileList.clear();
	}
	else {
		return false;
	}

	if (DirectoryFile.Type == FileTypes::Computer) {
		HANDLE hVolume;
		DWORD numRead;
		char cVolume[MAX_PATH];
		char cName[MAX_PATH];
		hVolume = FindFirstVolumeA(cVolume, MAX_PATH);
		if (hVolume == INVALID_HANDLE_VALUE) {
			FileList.push_back(File("C:\\"));
			return false;
		}
			
		do {
			GetVolumePathNamesForVolumeNameA(cVolume, cName, MAX_PATH, &numRead);
			string drive = cName;
			File driveFile = File();
			if (driveFile.Load(drive)) {
				FileList.push_back(driveFile);
			}
		} while(FindNextVolumeA(hVolume, cVolume, MAX_PATH) != 0);
		FindVolumeClose(hVolume);

		return true;
	}
	else if (DirectoryFile.Type != FileTypes::None) {
		vector<File> folders;

		HANDLE hDir;
		WIN32_FIND_DATAA file;
		
		hDir = FindFirstFileA(FileManager::AppendPath(DirectoryFile.Path, "*").c_str(), &file);
		if (hDir == nullptr || hDir == INVALID_HANDLE_VALUE) {
			File computerFile = File(FileManager::ComputerDirectory);
			computerFile.IsParentDirectory = true;
			FileList.push_back(computerFile);
			return DirectoryFile.Type == FileTypes::Drive;
		}
		do {
			string fileName = file.cFileName;
			if (GetFlag(file.dwFileAttributes, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED))
				continue;

			if (GetFlag(file.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY)) {
				if (fileName != ".") {
					File newFile = File();
					if (newFile.Load(FileManager::AppendPath(DirectoryFile.Path, fileName))) {
						if (fileName == "..") {
							newFile.IsParentDirectory = true;
							folders.insert(folders.begin(), newFile);
						}
						else {
							folders.push_back(newFile);
						}
					}
				}
			}
			else if (fileName[0] != '.' && fileName[0] != '$') {
				File newFile = File();
				if (newFile.Load(FileManager::AppendPath(DirectoryFile.Path, fileName)))
					FileList.push_back(newFile);
			}
		} while(FindNextFileA(hDir, &file) != 0);
		FindClose(hDir);
		
		if (DirectoryFile.Type == FileTypes::Drive) {
			File computerFile = File(FileManager::ComputerDirectory);
			computerFile.IsParentDirectory = true;
			folders.insert(folders.begin(), computerFile);
		}

		for (int i = (int)folders.size() - 1; i >= 0; i--){
			if (folders[i].Type != FileTypes::Computer) {
				hDir = FindFirstFileA(FileManager::AppendPath(folders[i].Path, "*").c_str(), &file);
				if (hDir != nullptr && hDir != INVALID_HANDLE_VALUE) {
					FileList.insert(FileList.begin(), folders[i]);
				}
				FindClose(hDir);
			}
			else {
				FileList.insert(FileList.begin(), folders[i]);
			}
		}

		return true;
	}

	return false;
}
bool PowerConsole::Windows::Directory::Load(const File& file) {
	return Load(file.Path);
}
//========= CONSTRUCTORS =========
PowerConsole::Windows::Directory::Directory() {
	this->DirectoryFile	= File();
}
PowerConsole::Windows::Directory::Directory(const string& path) {
	Load(path);
}
PowerConsole::Windows::Directory::Directory(const File& file) {
	Load(file);
}
//========== OPERATORS ===========
File& PowerConsole::Windows::Directory::operator[](int index) {
	return FileList[index];
}
//========== MANAGEMENT ==========
bool PowerConsole::Windows::Directory::Refresh() {
	return Load(DirectoryFile.Path);
}
//--------------------------------
void PowerConsole::Windows::Directory::RemoveFileExtensions(const string& extension, bool exclude) {
	for (int i = 0; i < (int)FileList.size(); i++) {
		if ((FileList[i].Type == FileTypes::File || FileList[i].Type == FileTypes::Link)) {
			if ((FileList[i].Extension == extension) != exclude) {
				FileList.erase(FileList.begin() + i);
				i--;
			}
		}
	}
}
void PowerConsole::Windows::Directory::RemoveFileTypes(FileTypes type, bool exclude) {
	for (int i = 0; i < (int)FileList.size(); i++) {
		if ((FileList[i].Type == type) != exclude) {
			FileList.erase(FileList.begin() + i);
			i--;
		}
	}
}
void PowerConsole::Windows::Directory::RemoveFileAttributes(unsigned int attributes, bool exclude) {
	for (int i = 0; i < (int)FileList.size(); i++) {
		if (((FileList[i].Attributes & attributes) != 0) != exclude) {
			FileList.erase(FileList.begin() + i);
			i--;
		}
	}
}
void PowerConsole::Windows::Directory::RemoveParentDirectory() {
	if (!FileList.empty()) {
		if (FileList[0].NameProper == ".." || FileList[0].Path == FileManager::ComputerDirectory)
			FileList.erase(FileList.begin());
	}
}
//========= INFORMATION ==========
unsigned int PowerConsole::Windows::Directory::NumFiles() {
	return FileList.size();
}
//----------------------------------------------------------------
// FileManager:
//----------------------------------------------------------------
//=========== MEMBERS ============
const string PowerConsole::Windows::FileManager::ComputerDirectory = "Computer:";
PowerConsole::Windows::FileManager::_FileManagerMembers PowerConsole::Windows::FileManager::_Data;
//========= CONSTRUCTORS =========
void PowerConsole::Windows::FileManager::Initialize() {
	static bool initialized = false;
	if (initialized)
		return;
	initialized = true;

	_Data.ExecutablePath = "";
	char cExecutablePath[MAX_PATH];
	if (GetModuleFileNameA(GetModuleHandleA(nullptr), cExecutablePath, MAX_PATH) != 0) {
		_Data.ExecutablePath = cExecutablePath;
	}

	CoInitialize(nullptr);
}
void PowerConsole::Windows::FileManager::Uninitialize() {

}
//========== MANAGEMENT ==========
bool PowerConsole::Windows::FileManager::CreateFile(const string& path, bool overwrite, bool folder) {
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, FileShareNone, nullptr,
		(overwrite ? CREATE_ALWAYS : CREATE_NEW), (folder ? FileAttributeDirectory : FileAttributeNormal), nullptr);
	if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return true;
	}
	return false;
}
bool PowerConsole::Windows::FileManager::CreateFile(const string& path, bool overwrite, unsigned int attributes) {
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, FileShareNone, nullptr,
		(overwrite ? CREATE_ALWAYS : CREATE_NEW), attributes, nullptr);
	if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return true;
	}
	return false;
}
bool PowerConsole::Windows::FileManager::CopyFile(const string& path, const string& newPath, bool overwrite) {
	return CopyFileA(path.c_str(), newPath.c_str(), !overwrite) != 0;
}
bool PowerConsole::Windows::FileManager::MoveFile(const string& path, const string& newPath, bool overwrite) {
	return MoveFileExA(path.c_str(), newPath.c_str(), (overwrite ? MOVEFILE_REPLACE_EXISTING : 0)) != 0;
}
bool PowerConsole::Windows::FileManager::DeleteFile(const string& path) {
	return DeleteFileA(path.c_str()) != 0;
}
bool PowerConsole::Windows::FileManager::RenameFile(const string& path, const string& name) {
	return MoveFileA(path.c_str(), AppendPath(GetDirectory(path), name).c_str()) != 0;
}
//========= INFORMATION ==========
string PowerConsole::Windows::FileManager::GetExecutablePath() {
	return _Data.ExecutablePath;
}
string PowerConsole::Windows::FileManager::GetExecutableDirectory() {
	return GetDirectory(_Data.ExecutablePath);
}
string PowerConsole::Windows::FileManager::GetExecutableName(bool includeExtension) {
	return GetFileName(_Data.ExecutablePath, includeExtension);
}
//--------------------------------
string PowerConsole::Windows::FileManager::GetWorkingDirectory() {
	char cPath[MAX_PATH];
	if (GetCurrentDirectoryA(MAX_PATH, cPath) != 0) {
		return cPath;
	}
	return "";
}
bool PowerConsole::Windows::FileManager::SetWorkingDirectory(const string& path) {
	return SetCurrentDirectoryA(path.c_str()) != 0;
}
//--------------------------------
unsigned long long PowerConsole::Windows::FileManager::GetFileSize(const string& path) {
	if (path == ComputerDirectory)
		return 0LU;
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, FileShareNone, nullptr,
		OPEN_EXISTING, FileAttributeNormal, nullptr);
	if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE) {
		LARGE_INTEGER size;
		if (GetFileSizeEx(hFile, &size) == 0)
			size.QuadPart = 0L;
		CloseHandle(hFile);
		return (unsigned long long)size.QuadPart;
	}
	return 0L;
}
unsigned int PowerConsole::Windows::FileManager::GetFileAttributes(const string& path) {
	if (path == ComputerDirectory)
		return FileAttributeNormal;
	return (unsigned int)GetFileAttributesA(path.c_str());
}
FileTypes PowerConsole::Windows::FileManager::GetFileType(const string& path) {
	if (path == ComputerDirectory)
		return FileTypes::Computer;
	string fixedPath = FixPath(path);

	if (fixedPath.length() == 3 || fixedPath.length() == 2) {
		if (fixedPath[1] == ':')
			return FileTypes::Drive;
	}
	unsigned int attributes = GetFileAttributes(path);
	if (attributes == FileAttributeInvalid)
		return FileTypes::None;
	else if (GetFlag(attributes, FileAttributeDirectory))
		return FileTypes::Directory;
	else if (GetFileExtension(fixedPath) == "lnk")
		return FileTypes::Link;
	else
		return FileTypes::File;
}
DriveTypes PowerConsole::Windows::FileManager::GetDriveType(const string& path) {
	DriveTypes type = (DriveTypes)GetDriveTypeA(path.c_str());
	switch (type) {
	case DriveTypes::Fixed:
	case DriveTypes::Remote:
	case DriveTypes::Removable:
	case DriveTypes::CDROM:
	case DriveTypes::RAM:		return type;
	default:					return DriveTypes::None;
	}
}
//--------------------------------
bool PowerConsole::Windows::FileManager::IsValidFileName(const string& name) {
	const string invalidChars = "<>:\"/\\|?*";
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
bool PowerConsole::Windows::FileManager::IsValidFilePath(const string& path) {
	if (path.empty())
		return false;

	return true;
}
bool PowerConsole::Windows::FileManager::FileExists(const string& path) {
	unsigned int attributes = GetFileAttributes(path);
	return (attributes != FileAttributeInvalid &&
			!GetFlag(attributes, FileAttributeDirectory));
}
bool PowerConsole::Windows::FileManager::DirectoryExists(const string& path) {
	if (path == ComputerDirectory)
		return true;
	unsigned int attributes = GetFileAttributes(path);
	return (attributes != FileAttributeInvalid &&
			GetFlag(attributes, FileAttributeDirectory));
}
bool PowerConsole::Windows::FileManager::PathExists(const string& path) {
	if (path == ComputerDirectory)
		return true;
	unsigned int attributes = GetFileAttributes(path);
	return attributes != FileAttributeInvalid;
}
//--------------------------------
string PowerConsole::Windows::FileManager::AppendPath(const string& path, const string& newPath) {
	if (path == ComputerDirectory) {
		if (newPath.front() == '\\')
			return newPath.substr(1, newPath.length() - 1);
		else
			return newPath;
	}
	if (path.empty() || newPath.empty())
		return path + newPath;
	if (path.back() == '\\' && newPath.front() == '\\')
		return path.substr(0, path.length() - 1) + newPath;
	if (path.back() != '\\' && newPath.front() != '\\')
		return path + "\\" + newPath;
	return path + newPath;
}
string PowerConsole::Windows::FileManager::FixPath(const string& path) {
	if (path == ComputerDirectory)
		return path;
	if (path.empty())
		return GetWorkingDirectory();
	char *cPath = new char[MAX_PATH];
	if (GetFullPathNameA(path.c_str(), MAX_PATH, cPath, 0) != 0) {
		return cPath;
	}
	return "";
}
//--------------------------------
string PowerConsole::Windows::FileManager::GetFileExtension(const string& path) {
	if (path == ComputerDirectory)
		return "";
	for (int i = path.length() - 1; i >= 0; i--) {
		if (path[i] == '\\')
			return "";
		if (path[i] == '.') {
			string extension = path.substr(i + 1, path.length() - i);
			for (int j = 0; j < (int)extension.length(); j++) {
				unsigned char c = extension[j];
				if (c >= 65 && c <= 90)
					extension[j] += 32;
			}
			return extension;
		}
	}
	return "";
}
string PowerConsole::Windows::FileManager::GetFileName(const string& path, bool includeExtension) {
	if (path == ComputerDirectory)
		return "Computer";
	int extension = -1;
	for (int i = path.length() - 1; i >= 0; i--) {
		if (path[i] == '\\') {
			if (i < (int)path.length() - 1) {
				if (includeExtension || extension == -1)
					return path.substr(i + 1, path.length() - i);
				else
					return path.substr(i + 1, extension - i - 1);
			}
			return "";
		}
		if (path[i] == '.' && extension == -1)
			extension = i;
	}
	if (includeExtension || extension == -1)
		return path;
	else
		return path.substr(0, extension);
}
string PowerConsole::Windows::FileManager::GetDriveName(const string& path, bool proper) {
	DriveTypes type = GetDriveType(path);
	string fixedPath = FixPath(path);
	if (fixedPath.length() < 1)
		return "";
	if (!proper)
		return fixedPath.substr(0, 2) + "\\";
	switch (type) {
	case DriveTypes::Fixed:		return "Local Disk (" + fixedPath.substr(0, 2) + ")";
	case DriveTypes::Remote:	return "Network Drive (" + fixedPath.substr(0, 2) + ")";
	case DriveTypes::Removable:	return "Removable Disk (" + fixedPath.substr(0, 2) + ")";
	case DriveTypes::CDROM:		return "CD Drive (" + fixedPath.substr(0, 2) + ")";
	case DriveTypes::RAM:		return "RAM Disk (" + fixedPath.substr(0, 2) + ")";
	default:					return "";
	}
}
string PowerConsole::Windows::FileManager::GetDirectory(const string& path) {
	if (path == ComputerDirectory)
		return "";
	for (int i = path.length() - 1; i >= 0; i--) {
		if (path[i] == '\\') {
			if (i < (int)path.length() - 1)
				return FixPath(path.substr(0, i));
			else
				return "";
		}
	}
	return GetWorkingDirectory();
}
string PowerConsole::Windows::FileManager::GetParentDirectory(const string& path) {
	if (path == ComputerDirectory)
		return "";
	return FixPath(AppendPath(GetDirectory(path), ".."));
}
string PowerConsole::Windows::FileManager::GetLinkLocation(const string& path) {
	/*HRESULT result = CoInitialize(nullptr);
	if (!SUCCEEDED(result))
		return "";*/
	
	IShellLinkW* psl = nullptr;
	string finalPath = "";
	
	HRESULT result = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
	if (SUCCEEDED(result)) {
		IPersistFile* ppf;
		result = psl->QueryInterface(IID_IPersistFile, (LPVOID *) &ppf);
		if (SUCCEEDED(result)) {
			wchar_t wLink[MAX_PATH];
			MultiByteToWideChar(0, 0, path.c_str(), -1, wLink, MAX_PATH);

			result = ppf->Load(wLink, 0);
			if (SUCCEEDED(result)) {
				wchar_t wPath[MAX_PATH];
				char cPath[MAX_PATH];
				psl->GetPath(wPath, MAX_PATH, nullptr, 0); //SLGP_UNCPRIORITY

				WideCharToMultiByte(0, 0, wPath, -1, cPath, MAX_PATH, nullptr, nullptr);
				finalPath = cPath;
			}
		}
		psl->Release();
	}
	
	return finalPath;
}
string PowerConsole::Windows::FileManager::GetRoot(const string& path) {
	if (path == ComputerDirectory)
		return "";
	string fixedPath = FixPath(path);
	for (int i = 0; i < (int)fixedPath.length(); i++) {
		if (fixedPath[i] == '\\')
			return fixedPath.substr(0, i + 1);
	}
	return "";
}
string PowerConsole::Windows::FileManager::GetUserDirectory() {
	char cUser[MAX_PATH];
	if (GetEnvironmentVariableA("USERPROFILE", cUser, MAX_PATH) == 0) {
		return "";
	}
	return string(cUser);
}
//--------------------------------
unsigned int PowerConsole::Windows::FileManager::CreateClipboardFormat(const string& formatName) {
	return RegisterClipboardFormatA(formatName.c_str());
}
bool PowerConsole::Windows::FileManager::IsClipboardFormatAvailable(unsigned int clipboardFormat) {
	return ::IsClipboardFormatAvailable(clipboardFormat) != 0;
}
//--------------------------------
bool PowerConsole::Windows::FileManager::ResourceExists(const string& name, const string& type) {
	return FindResourceA(nullptr, name.c_str(), type.c_str()) != nullptr;
}
bool PowerConsole::Windows::FileManager::ResourceExists(const string& name, unsigned int typeId) {
	return FindResourceA(nullptr, name.c_str(), MAKEINTRESOURCEA(typeId)) != nullptr;
}
bool PowerConsole::Windows::FileManager::ResourceExists(unsigned int id, const string& type) {
	return FindResourceA(nullptr, MAKEINTRESOURCEA(id), type.c_str()) != nullptr;
}
bool PowerConsole::Windows::FileManager::ResourceExists(unsigned int id, unsigned int typeId) {
	return FindResourceA(nullptr, MAKEINTRESOURCEA(id), MAKEINTRESOURCEA(typeId)) != nullptr;
}
//================================================================
// END FILE MANAGER CPP:
//================================================================