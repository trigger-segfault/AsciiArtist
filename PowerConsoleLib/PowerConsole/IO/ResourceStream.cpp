/*=================================================================|
|* File:				ResourceStream.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ResourceStream.h"

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

ResourceInputStream::ResourceInputStream() {
	this->hMem = nullptr;
	this->data = nullptr;
	this->state = 0;
	this->error = false;
	this->bigEndian = false;
	this->pointer = 0L;
	this->fileSize = 0L;
}
ResourceInputStream::ResourceInputStream(bool bigEndian) {
	this->hMem = nullptr;
	this->data = nullptr;
	this->state = 0;
	this->error = false;
	this->bigEndian = bigEndian;
	this->pointer = 0L;
	this->fileSize = 0L;
}
ResourceInputStream::~ResourceInputStream() {
	if (state == 1) {
		// Nothing to close
	}
}

#pragma endregion
//======== STREAM ACCESS =========
#pragma region Stream Access

bool ResourceInputStream::open(const std::string& name, const std::string& type) {
	if (state == 0) {
		// Locate the resource
		
		HRSRC hRes = FindResourceA(nullptr, name.c_str(), type.c_str());
		if (hRes != nullptr) {

			// Get the location the resource in memory
			hMem = LoadResource(nullptr, hRes);
			if (hMem != nullptr) {

				// Get access to the memory
				data = (unsigned char*)LockResource(hMem);
				if (data != nullptr) {
					state = 1;
					error = false;
					pointer = 0L;
					fileSize = (long long)SizeofResource(nullptr, hRes);
					return true;
				}
			}
		}
		error = true;
	}
	return false;
}
bool ResourceInputStream::open(const std::string& name, unsigned int typeId) {
	return open(name, "#" + std::to_string(typeId));
}
bool ResourceInputStream::open(unsigned int id, const std::string& type) {
	return open("#" + std::to_string(id), type);
}
bool ResourceInputStream::open(unsigned int id, unsigned int typeId) {
	return open("#" + std::to_string(id), "#" + std::to_string(typeId));
}
bool ResourceInputStream::close() {
	if (state == 1) {
		hMem = nullptr;
		data = nullptr;
		state = 0;
		error = false;
		pointer = 0L;
		fileSize = 0L;
		return true;
	}
	return false;
}
bool ResourceInputStream::read(void* rawData, unsigned int size, unsigned int length, unsigned int offset) {
	if (state == 1 && !error && size * length > 0) {

		if (pointer + (long long)(size * length) > fileSize) {
			pointer = fileSize;
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

bool ResourceInputStream::isOpen() {
	return (state != 0);
}
bool ResourceInputStream::hasError() {
	return error;
}
bool ResourceInputStream::isEof() {
	return (pointer >= fileSize);
}
long long ResourceInputStream::getPointer() {
	return pointer;
}
bool ResourceInputStream::setPointer(long long position, bool relative) {
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
		if (pointer > fileSize)
			pointer = fileSize;
		return true;
	}
	return false;
}
long long ResourceInputStream::size() {
	return fileSize;
}
bool ResourceInputStream::isBigEndian() {
	return bigEndian;
}
void ResourceInputStream::setBigEndian(bool bigEndian) {
	this->bigEndian = bigEndian;
}
AccessRights ResourceInputStream::getAccessRights() {
	return AccessRights::Read;
}

#pragma endregion
//=================================================================|
