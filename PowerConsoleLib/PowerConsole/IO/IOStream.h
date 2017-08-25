/*=================================================================|
|* File:				IOStream.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef IO_STREAM_H
#define IO_STREAM_H

#include <string>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/IO/IOEnums.h>

namespace PowerConsole::IO {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The basic interface shared between both input and output streams. */
class BasicStream {
public:
	//========== OPERATORS ===========
	#pragma region Operators

	// Unary Assignment
	/* Increments the pointer of the stream. */
	BasicStream& operator++();
	/* Decrements the pointer of the stream. */
	BasicStream& operator--();

	// Binary Assignment
	/* Sets the pointer of the stream. */
	BasicStream& operator=(int rhs);
	/* Sets the pointer of the stream. */
	BasicStream& operator=(long long rhs);
	/* Increments the pointer of the stream. */
	BasicStream& operator+=(int rhs);
	/* Increments the pointer of the stream. */
	BasicStream& operator+=(long long rhs);
	/* Decrements the pointer of the stream. */
	BasicStream& operator-=(int rhs);
	/* Decrements the pointer of the stream. */
	BasicStream& operator-=(long long rhs);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Returns true if the stream is open. */
	virtual bool isOpen() = 0;
	/* Returns true if there is an error. */
	virtual bool hasError() = 0;
	/* Returns true if the end of the file has been reached. */
	virtual bool isEof() = 0;
	/* Gets the position of the pointer. */
	virtual long long getPointer() = 0;
	/* Sets the position of the pointer. */
	virtual bool setPointer(long long position, bool relative = false) = 0;
	/* Gets the size of the stream data. */
	virtual long long size() = 0;
	/* Returns true if the stream is being read in big endian. */
	virtual bool isBigEndian() = 0;
	/* Sets whether the stream should be read in big endian. */
	virtual void setBigEndian(bool bigEndian) = 0;
	/* Gets the access rights for this stream. */
	virtual AccessRights getAccessRights() = 0;

	#pragma endregion
};

/* The interface for all input streams. */
class InputStream : public BasicStream {
public:
	//=========== READING ============
	#pragma region Reading

	// Basic
	/* Reads data from the input stream. */
	template<typename T> bool read(T* data, unsigned int length = 1);
	/* Reads data from the input stream. */
	virtual bool read(void* data, unsigned int size, unsigned int length, unsigned int offset = 0) = 0;

	// Type Specific
	/* Reads the next character from the input stream. */
	signed char readChar();
	/* Reads the next unsigned character from the input stream. */
	unsigned char readUChar();
	/* Reads the next short from the input stream. */
	signed short readShort();
	/* Reads the next unsigned short from the input stream. */
	unsigned short readUShort();
	/* Reads the next integer from the input stream. */
	signed int readInt();
	/* Reads the next unsigned integer from the input stream. */
	unsigned int readUInt();
	/* Reads the next long from the input stream. */
	signed long readLong();
	/* Reads the next unsigned long from the input stream. */
	unsigned long readULong();
	/* Reads the next long long from the input stream. */
	signed long long readLongLong();
	/* Reads the next unsigned long long from the input stream. */
	unsigned long long readULongLong();
	/* Reads the next float from the input stream. */
	float readFloat();
	/* Reads the next double from the input stream. */
	double readDouble();
	/* Reads the next long double from the input stream. */
	long double readLongDouble();

	// String
	/* Reads the next string from the input stream until a null character is reached. */
	std::string readString();
	/* Reads the next string of the specified length from the input stream. */
	std::string readString(unsigned int length);
	/* Reads the next utf string from the input stream. */
	std::string readUTFString();

	#pragma endregion
};
/* Reads data from the input stream. */
template<typename T> bool InputStream::read(T* data, unsigned int length) {
	return read(data, sizeof(T), length);
}
/* The interface for all output streams. */
class OutputStream : public BasicStream {
public:
	//=========== WRITING ============
	#pragma region Writing

	// Basic
	/* Writes data to the output stream. */
	template<typename T> bool write(T* data, unsigned int length = 1);
	/* Writes data to the output stream. */
	virtual bool write(void* data, unsigned int size, unsigned int length, unsigned int offset = 0) = 0;

	// Type Specific
	/* Writes the character to the output stream. */
	void writeChar(signed char value);
	/* Writes the unsigned character to the output stream. */
	void writeUChar(unsigned char value);
	/* Writes the short to the output stream. */
	void writeShort(signed short value);
	/* Writes the unsigned short to the output stream. */
	void writeUShort(unsigned short value);
	/* Writes the integer to the output stream. */
	void writeInt(signed int value);
	/* Writes the unsigned integer to the output stream. */
	void writeUInt(unsigned int value);
	/* Writes the long to the output stream. */
	void writeLong(signed long value);
	/* Writes the unsigned long to the output stream. */
	void writeULong(unsigned long value);
	/* Writes the long long to the output stream. */
	void writeLongLong(signed long long value);
	/* Writes the unsigned long long to the output stream. */
	void writeULongLong(unsigned long long value);
	/* Writes the float to the output stream. */
	void writeFloat(float value);
	/* Writes the double to the output stream. */
	void writeDouble(double value);
	/* Writes the long double to the output stream. */
	void writeLongDouble(long double value);

	// String
	/* Writes the string to the output stream and terminates it with a null character. */
	void writeString(const std::string& text);
	/* Writes the string of the specified length to the output stream. */
	void writeString(const std::string& text, unsigned int length);
	/* Writes the utf string to the output stream. */
	void writeUTFString(const std::string& text);
	/* Writes text to the output stream without terminating it. */
	void writeText(const std::string& text);

	#pragma endregion
};
/* Writes data to the output stream. */
template<typename T> bool OutputStream::write(T* data, unsigned int length) {
	return write(data, sizeof(T), length);
}

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
