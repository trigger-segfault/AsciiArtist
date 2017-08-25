/*=================================================================|
|* File:				ClipboardStream.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CLIPBOARD_STREAM_H
#define CLIPBOARD_STREAM_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/IOStream.h>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* An input stream for reading data from the clipboard. */
class ClipboardInputStream : public InputStream {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The handle to the memory being read. */
	HGLOBAL hMem;
	/* The data of the memory. */
	unsigned char* data;
	/* The position of the pointer in the clipboard. */
	long long pointer;
	/* The size of the clipboard. */
	long long fileSize;
	/* The state of the clipboard. */
	short state;
	/* True if there is an error. */
	bool error;
	/* True if the clipboard is being read in big endian. */
	bool bigEndian;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default clipboard input stream. */
	ClipboardInputStream();
	/* Constructs a clipboard input stream with the specified settings. */
	ClipboardInputStream(bool bigEndian);
	/* Deconstructs the clipboard input stream. */
	~ClipboardInputStream();

	#pragma endregion
	//======== STREAM ACCESS =========
	#pragma region Stream Access

	/* Opens the clipboard. */
	bool open();
	/* Closes the clipboard. */
	bool close();
	/* Gets the Data of the specified format from the clipboard. */
	bool getData(unsigned int clipboardFormat);
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
	void setBigEndian(bool bigEndian);
	/* Gets the access rights for this stream. */
	AccessRights getAccessRights();
};
/* An output stream for writing data to the clipboard. */
class ClipboardOutputStream : public OutputStream {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The byte array of data for the clipboard. */
	std::vector<unsigned char> data;
	/* The position of the pointer in the clipboard. */
	long long pointer;
	/* The size of the clipboard. */
	long long fileSize;
	/* The state of the clipboard. */
	short state;
	/* True if there is an error. */
	bool error;
	/* True if the clipboard is being read in big endian. */
	bool bigEndian;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default clipboard output stream. */
	ClipboardOutputStream();
	/* Constructs a clipboard output stream with the specified settings. */
	ClipboardOutputStream(bool bigEndian);
	/* Deconstructs the clipboard output stream. */
	~ClipboardOutputStream();

	#pragma endregion
	//======== STREAM ACCESS =========
	#pragma region Stream Access

	/* Opens the clipboard. */
	bool open();
	/* Closes the clipboard. */
	bool close();
	/* Sets the Data of the specified format to from the clipboard. */
	bool setData(unsigned int clipboardFormat);
	/* Writes data to the output stream. */
	bool write(void* data, unsigned int size, unsigned int length, unsigned int offset = 0);
	/* Writes data to the output stream. */
	using OutputStream::write;

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
	void setBigEndian(bool bigEndian);
	/* Gets the access rights for this stream. */
	AccessRights getAccessRights();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
