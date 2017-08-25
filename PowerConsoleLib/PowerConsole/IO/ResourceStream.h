/*=================================================================|
|* File:				ResourceStream.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef RESOURCE_STREAM_H
#define RESOURCE_STREAM_H

#include <PowerConsole/IO/IOStream.h>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* An input stream for reading data from resources. */
class ResourceInputStream : public InputStream {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The handle to the memory being read. */
	HGLOBAL hMem;
	/* The data of the memory. */
	unsigned char* data;
	/* The position of the pointer in the resource. */
	long long pointer;
	/* The size of the resource. */
	long long fileSize;
	/* The state of the resource. */
	short state;
	/* True if there is an error. */
	bool error;
	/* True if the resource is being read in big endian. */
	bool bigEndian;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default resource input stream. */
	ResourceInputStream();
	/* Constructs a resource input stream with the specified settings. */
	ResourceInputStream(bool bigEndian);
	/* Deconstructs the resource input stream. */
	~ResourceInputStream();

	#pragma endregion
	//======== STREAM ACCESS =========
	#pragma region Stream Access

	/* Opens the resource. */
	bool open(const std::string& name, const std::string& type);
	/* Opens the resource. */
	bool open(const std::string& name, unsigned int typeId);
	/* Opens the resource. */
	bool open(unsigned int id, const std::string& type);
	/* Opens the resource. */
	bool open(unsigned int id, unsigned int typeId);
	/* Closes the resource. */
	bool close();
	/* Reads data from the input stream. */
	bool read(void* data, unsigned int size, unsigned int length, unsigned int offset = 0);
	/* Reads data from the input stream. */
	using InputStream::read;
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Returns true if the stream is open. */
	bool isOpen();
	/* Returns true if there is an error. */
	bool hasError();
	/* Returns true if the end of the file has been reached. */
	bool isEof();
	/* Gets the position of the pointer. */
	long long getPointer();
	/* Sets the position of the pointer. */
	bool setPointer(long long position, bool relative = false);
	/* Gets the size of the stream data. */
	long long size();
	/* Returns true if the stream is being read in big endian. */
	bool isBigEndian();
	/* Sets whether the stream should be read in big endian. */
	void setBigEndian(bool littleEndian);
	/* Gets the access rights for this stream. */
	AccessRights getAccessRights();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
