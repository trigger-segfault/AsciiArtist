/*=================================================================|
|* File:				FileStream.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/IOStream.h>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The abstract class for handling file streams. */
class FileStream : public BasicStream {

	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The handle to the file being read. */
	HANDLE hFile;
	/* The position of the pointer in the file. */
	long long pointer;
	/* The size of the file. */
	long long fileSize;
	/* The state of the file. */
	short state;
	/* True if there is an error. */
	bool error;
	/* True if the file is being read in big endian. */
	bool bigEndian;

	#pragma endregion
	//======== STREAM ACCESS =========
	#pragma region Stream Access
public:
	/* Opens the file. */
	bool open(const std::string& path);
	/* Opens the file with the specified settings. */
	bool open(const std::string& path, FileShareModes shareMode, FileAttributes attributes, unsigned int flags = 0x0);
	/* Closes the file. */
	bool close();

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
	virtual AccessRights getAccessRights() = 0;

	#pragma endregion
};

/* An input stream for reading files. */
class FileInputStream : public FileStream, public InputStream {
public:
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default file input stream. */
	FileInputStream();
	/* Constructs a file input stream with the specified settings. */
	FileInputStream(bool bigEndian);
	/* Deconstructs the file input stream. */
	~FileInputStream();

	#pragma endregion
	//======== STREAM ACCESS =========
	#pragma region Stream Access

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

	#pragma endregion
};
/* An output stream for writing files. */
class FileOutputStream : public FileStream, public OutputStream {
public:
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the default file output stream. */
	FileOutputStream();
	/* Constructs a file output stream with the specified settings. */
	FileOutputStream(bool bigEndian);
	/* Deconstructs the file output stream. */
	~FileOutputStream();

	#pragma endregion
	//======== STREAM ACCESS =========
	#pragma region Stream Access

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
