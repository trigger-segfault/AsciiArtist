/*=================================================================|
|* File:				ClipboardStream.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ClipboardStream.h"

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors
ClipboardInputStream::ClipboardInputStream() {
	this->hMem = nullptr;
	this->data = nullptr;
	this->state = 0;
	this->error = false;
	this->bigEndian = false;
	this->pointer = 0L;
	this->fileSize = 0L;
}
ClipboardInputStream::ClipboardInputStream(bool bigEndian) {
	this->hMem = nullptr;
	this->data = nullptr;
	this->state = 0;
	this->error = false;
	this->bigEndian = bigEndian;
	this->pointer = 0L;
	this->fileSize = 0L;
}
ClipboardInputStream::~ClipboardInputStream() {
	if (state != 0) {
		if (state == 2)
			GlobalUnlock(hMem);
		CloseClipboard();
	}
}

#pragma endregion
//======== STREAM ACCESS =========
#pragma region Stream Access

bool ClipboardInputStream::open() {
	if (state == 0) {
		if (OpenClipboard(GetConsoleWindow()) != 0) {
			hMem = nullptr;
			data = nullptr;
			state = 1;
			error = false;
			pointer = 0L;
			fileSize = 0L;
			return true;
		}
	}
	return false;
}
bool ClipboardInputStream::close() {
	if (state != 0) {
		if (state == 2) {
			if (GlobalUnlock(hMem) != 0)
				error = true;
		}
		if (CloseClipboard() != 0) {
			hMem = nullptr;
			data = nullptr;
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
bool ClipboardInputStream::getData(unsigned int clipboardFormat) {
	if (state != 0 && !error) {
		// Close any previous data
		if (state == 2) {
			if (GlobalUnlock(hMem) == 0)
				error = true;
		}
		// Get location of clipboard data
		hMem = GetClipboardData(clipboardFormat);
		if (hMem != nullptr) {
			// Get access to the memory
			data = (unsigned char*)GlobalLock(hMem);
			if (data != nullptr) {
				state = 2;
				pointer = 0L;
				fileSize = (long long)GlobalSize(hMem);
				return true;
			}
		}
		error = true;
	}
	return false;
}
bool ClipboardInputStream::read(void* rawData, unsigned int size, unsigned int length, unsigned int offset) {
	if (state == 2 && !error && size * length > 0) {

		if (pointer + (long long)(size * length) > fileSize) {
			pointer = size;
			error = true;
		}
		else if (!bigEndian) {
			if (memcpy_s((unsigned char*)rawData + offset, size * length, data + pointer, size * length) == 0) {
				pointer += (long long)(size * length);
			}
			else {
				error = true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !error; i++) {
				for (int j = 0; j < (int)size && !error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (memcpy_s((unsigned char*)rawData + pos, 1, data + pointer, 1) == 0) {
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

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool ClipboardInputStream::isOpen() {
	return (state != 0);
}
bool ClipboardInputStream::hasError() {
	return error;
}
bool ClipboardInputStream::isEof() {
	return pointer >= fileSize;
}
long long ClipboardInputStream::getPointer() {
	return pointer;
}
bool ClipboardInputStream::setPointer(long long position, bool relative) {
	// Make sure the pointer position is valid
	unsigned long long maxPos = (unsigned long long)(std::numeric_limits<long long>::max)();
	bool validPos = false;
	if (relative && pointer + position >= 0L && (unsigned long long)(pointer + position) <= maxPos)
		validPos = true;
	if (!relative && position >= 0L && (unsigned long long)position <= maxPos)
		validPos = true;

	if (state == 2 && !error && validPos) {
		if (relative)
			pointer += position;
		else
			pointer = position;
		if (pointer > fileSize)
			pointer = fileSize;
		return true;
	}
	return false;
}
long long ClipboardInputStream::size() {
	return fileSize;
}
bool ClipboardInputStream::isBigEndian() {
	return bigEndian;
}
void ClipboardInputStream::setBigEndian(bool bigEndian) {
	this->bigEndian = bigEndian;
}
AccessRights ClipboardInputStream::getAccessRights() {
	return AccessRights::Read;
}

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

ClipboardOutputStream::ClipboardOutputStream() {
	this->state = 0;
	this->error = false;
	this->bigEndian = false;
	this->pointer = 0L;
	this->fileSize = 0L;
}
ClipboardOutputStream::ClipboardOutputStream(bool bigEndian) {
	this->state = 0;
	this->error = false;
	this->bigEndian = bigEndian;
	this->pointer = 0L;
	this->fileSize = 0L;
}
ClipboardOutputStream::~ClipboardOutputStream() {
	if (state == 1) {
		CloseClipboard();
	}
}

#pragma endregion
//======== STREAM ACCESS =========
#pragma region Stream Access

bool ClipboardOutputStream::open() {
	if (state == 0) {
		if (OpenClipboard(GetConsoleWindow()) != 0) {
			if (EmptyClipboard() != 0) {
				data.clear();
				state = 1;
				error = false;
				pointer = 0L;
				fileSize = 0L;
				return true;
			}
		}
	}
	return false;
}
bool ClipboardOutputStream::close() {
	if (state == 1) {
		if (CloseClipboard() != 0) {
			data.clear();
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
bool ClipboardOutputStream::setData(unsigned int clipboardFormat) {
	if (state == 1 && !error) {

		// Allocate the memory for storing the clipboard data
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)fileSize);
		if (hMem != nullptr) {

			// Get access to the memory
			unsigned char* rawData = (unsigned char*)GlobalLock(hMem);
			if (rawData != nullptr) {

				// Copy all the data, byte-by-byte so it's not put into little endian a second time
				for (int i = 0; i < (int)data.size(); i++) {
					if (memcpy_s(rawData + i, 1, data.data() + i, 1) != 0) {
						error = true;
						break;
					}
				}
				// Give back access
				if (GlobalUnlock(hMem) == 0) {
					// Finalize the clipboard data and set the specified format
					if (SetClipboardData(clipboardFormat, hMem) != nullptr) {
						// Clear the data for next format to be written
						data.clear();
						pointer = 0L;
						fileSize = 0L;
						return true;
					}
				}
			}

		}
		error = true;
	}

	return !error;
}
bool ClipboardOutputStream::write(void* rawData, unsigned int size, unsigned int length, unsigned int offset) {
	if (state == 1 && !error && size * length > 0) {

		// Resize the byte array if needed
		if (pointer + (long long)(size * length) > fileSize) {
			data.resize((unsigned int)(pointer + (long long)(size * length)));
			fileSize = (long long)data.size();
		}

		if (!bigEndian) {
			if (memcpy_s(data.data() + pointer, size * length, (unsigned char*)rawData + offset, size * length) == 0) {
				pointer += size * length;
			}
			else {
				error = true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !error; i++) {
				for (int j = 0; j < (int)size && !error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (memcpy_s(data.data() + pointer, 1, (unsigned char*)rawData + pos, 1) == 0) {
						pointer++;
					}
					else {
						error = true;
					}
				}
			}
		}
		return true;
	}
	return false;
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool ClipboardOutputStream::isOpen() {
	return (state != 0);
}
bool ClipboardOutputStream::hasError() {
	return error;
}
bool ClipboardOutputStream::isEof() {
	return pointer >= fileSize;
}
long long ClipboardOutputStream::getPointer() {
	return pointer;
}
bool ClipboardOutputStream::setPointer(long long position, bool relative) {
	// Make sure the pointer position is valid
	unsigned long long maxPos = (unsigned long long)(std::numeric_limits<long long>::max)();
	bool validPos = false;
	if (relative && pointer + position >= 0L && (unsigned long long)(pointer + position) <= maxPos)
		validPos = true;
	if (!relative && position >= 0L && (unsigned long long)position <= maxPos)
		validPos = true;

	if (state == 1 && !error && validPos) {
		if (relative)
			pointer += position;
		else
			pointer = position;

		// Resize the byte array if needed
		if (pointer > fileSize) {
			data.resize((unsigned int)pointer);
			fileSize = (long long)data.size();
		}
		return true;
	}
	return false;
}
long long ClipboardOutputStream::size() {
	return fileSize;
}
bool ClipboardOutputStream::isBigEndian() {
	return bigEndian;
}
void ClipboardOutputStream::setBigEndian(bool bigEndian) {
	this->bigEndian = bigEndian;
}
AccessRights ClipboardOutputStream::getAccessRights() {
	return AccessRights::Write;
}

#pragma endregion
//=================================================================|
