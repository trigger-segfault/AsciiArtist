/*=================================================================|
|* File:				FileStream.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FileStream.h"

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= FILE ACCESS ==========
#pragma region File Access

bool FileStream::open(const std::string& path) {
	return open(path, FileShareModes::None, FileAttributes::Normal);
}
bool FileStream::open(const std::string& path, FileShareModes shareMode, FileAttributes attributes, unsigned int flags) {
	if (state == 0) {
		unsigned int creationDesposition = OPEN_ALWAYS;
		switch (getAccessRights()) {
		case AccessRights::Read:	creationDesposition = OPEN_EXISTING; break;
		case AccessRights::Write:	creationDesposition = CREATE_ALWAYS; break;
		default:					creationDesposition = OPEN_ALWAYS; break;
		}
		hFile = CreateFileA(
			path.c_str(),
			(unsigned int)getAccessRights(),
			(unsigned int)shareMode,
			nullptr,
			creationDesposition,
			(unsigned int)attributes | flags,
			nullptr
		);

		if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE) {
			state = 1;
			error = false;
			pointer = 0L;

			unsigned long lowSize = 0, highSize = 0;
			lowSize = GetFileSize(hFile, &highSize);
			fileSize = (long long)(((unsigned long long)highSize << 32) + (unsigned long long)lowSize);
			return true;
		}
		hFile = nullptr;
	}
	return false;
}
bool FileStream::close() {
	if (state == 1) {
		if (CloseHandle(hFile) != 0) {
			hFile = nullptr;
			state = 0;
			error = false;
			pointer = 0L;
			fileSize = 0L;
			return true;
		}
		error = true;
	}
	return false;
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool FileStream::isOpen() {
	return (state != 0);
}
bool FileStream::hasError() {
	return error;
}
bool FileStream::isEof() {
	return pointer >= fileSize;
}
long long FileStream::getPointer() {
	return pointer;
}
bool FileStream::setPointer(long long position, bool relative) {
	// Make sure the pointer position is valid
	unsigned long long maxPos = (unsigned long long)(std::numeric_limits<long long>::max)();
	bool validPos = false;
	if (relative && pointer + position >= 0L && (unsigned long long)(pointer + position) <= maxPos)
		validPos = true;
	if (!relative && position >= 0L && (unsigned long long)position <= maxPos)
		validPos = true;

	if (state == 1 && !error && validPos) {
		long lowDistance = (long)(position & 0xFFFFFFFF);
		long highDistance = (long)((position & 0xFFFFFFFF00000000) >> 32);
		unsigned long moveMethod = (relative ? FILE_CURRENT : FILE_BEGIN);
		if (SetFilePointer(hFile, lowDistance, &highDistance, moveMethod) != INVALID_SET_FILE_POINTER) {
			if (relative)
				pointer += position;
			else
				pointer = position;
			return true;
		}
		error = true;
	}
	return false;
}
long long FileStream::size() {
	return fileSize;
}
bool FileStream::isBigEndian() {
	return bigEndian;
}
void FileStream::setBigEndian(bool bigEndian) {
	this->bigEndian = bigEndian;
}

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

FileInputStream::FileInputStream() {
	this->hFile = nullptr;
	this->state = 0;
	this->error = false;
	this->bigEndian = false;
	this->pointer = 0L;
	this->fileSize = 0L;
}
FileInputStream::FileInputStream(bool bigEndian) {
	this->hFile = nullptr;
	this->state = 0;
	this->error = false;
	this->bigEndian = bigEndian;
	this->pointer = 0L;
	this->fileSize = 0L;
}
FileInputStream::~FileInputStream() {
	if (state == 1) {
		CloseHandle(hFile);
	}
}

FileOutputStream::FileOutputStream() {
	this->hFile = nullptr;
	this->state = 0;
	this->error = false;
	this->bigEndian = false;
	this->pointer = 0L;
	this->fileSize = 0L;
}
FileOutputStream::FileOutputStream(bool bigEndian) {
	this->hFile = nullptr;
	this->state = 0;
	this->error = false;
	this->bigEndian = bigEndian;
	this->pointer = 0L;
	this->fileSize = 0L;
}
FileOutputStream::~FileOutputStream() {
	if (state == 1) {
		CloseHandle(hFile);
	}
}

#pragma endregion
//========= FILE ACCESS ==========
#pragma region File Access

bool FileInputStream::read(void* rawData, unsigned int size, unsigned int length, unsigned int offset) {
	if (state == 1 && !error && size * length > 0) {

		unsigned long bytesRead = 0;
		if (pointer + (long long)(size * length) > fileSize) {
			pointer = fileSize;
			error = true;
		}
		else if (!bigEndian) {
			if (ReadFile(hFile, (unsigned char*)rawData + offset, (unsigned long)(size * length), &bytesRead, nullptr) != 0) {
				pointer += (long long)bytesRead;
			}
			else {
				error = true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !error; i++) {
				for (int j = 0; j < (int)size && !error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (ReadFile(hFile, (unsigned char*)rawData + pos, 1, &bytesRead, nullptr) != 0) {
						pointer++;
					}
					else {
						error = true;
					}
				}
			}
		}
		return !error;
	}
	return false;
}
bool FileOutputStream::write(void* rawData, unsigned int size, unsigned int length, unsigned int offset) {
	if (state == 1 && !error && size * length > 0) {

		unsigned long bytesWritten = 0;
		if (!bigEndian) {
			if (WriteFile(hFile, (unsigned char*)rawData + offset, size * length, &bytesWritten, nullptr) != 0) {
				pointer += (long long)bytesWritten;
			}
			else {
				error = true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !error; i++) {
				for (int j = 0; j < (int)size && !error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (WriteFile(hFile, (unsigned char*)rawData + pos, 1, &bytesWritten, nullptr) != 0) {
						pointer++;
					}
					else {
						error = true;
					}
				}
			}
		}
		if (pointer > fileSize)
			fileSize = pointer;
		return !error;
	}
	return false;
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool FileInputStream::isOpen() { return FileStream::isOpen(); }
bool FileInputStream::hasError() { return FileStream::hasError(); }
bool FileInputStream::isEof() { return FileStream::isEof(); }
long long FileInputStream::getPointer() { return FileStream::getPointer(); }
bool FileInputStream::setPointer(long long position, bool relative) { return FileStream::setPointer(position, relative); }
long long FileInputStream::size() { return FileStream::size(); }
bool FileInputStream::isBigEndian() { return FileStream::isBigEndian(); }
void FileInputStream::setBigEndian(bool bigEndian) { FileStream::setBigEndian(bigEndian); }
AccessRights FileInputStream::getAccessRights() {
	return AccessRights::Read;
}

bool FileOutputStream::isOpen() { return FileStream::isOpen(); }
bool FileOutputStream::hasError() { return FileStream::hasError(); }
bool FileOutputStream::isEof() { return FileStream::isEof(); }
long long FileOutputStream::getPointer() { return FileStream::getPointer(); }
bool FileOutputStream::setPointer(long long position, bool relative) { return FileStream::setPointer(position, relative); }
long long FileOutputStream::size() { return FileStream::size(); }
bool FileOutputStream::isBigEndian() { return FileStream::isBigEndian(); }
void FileOutputStream::setBigEndian(bool bigEndian) { FileStream::setBigEndian(bigEndian); }
AccessRights FileOutputStream::getAccessRights() {
	return AccessRights::Write;
}

//=================================================================|
