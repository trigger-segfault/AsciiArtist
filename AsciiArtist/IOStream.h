//================================================================
// IO STREAM HEADER:
//================================================================
#pragma once
#ifndef IO_STREAM_H
#define IO_STREAM_H

#include "stdafx.h"
#include "FileManager.h"

using namespace std;
using namespace PowerConsole::Windows;
//================================================================
// NAMESPACE:
//================================================================
/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of classes used for file IO. */
namespace IOStream {
//================================================================
// CLASSES:
//================================================================
/* The interface for all input streams. */
class InputStream {
	
	//========== OPERATORS ===========
public:
	// Unary Assignment
	/* Increments the pointer of the stream. */
	InputStream& operator++();
	/* Decrements the pointer of the stream. */
	InputStream& operator--();

	// Binary Assignment
	/* Sets the pointer of the stream. */
	InputStream& operator=(int rhs);
	/* Sets the pointer of the stream. */
	InputStream& operator=(long long rhs);
	/* Increments the pointer of the stream. */
	InputStream& operator+=(int rhs);
	/* Increments the pointer of the stream. */
	InputStream& operator+=(long long rhs);
	/* Decrements the pointer of the stream. */
	InputStream& operator-=(int rhs);
	/* Decrements the pointer of the stream. */
	InputStream& operator-=(long long rhs);

	//========== MANAGEMENT ==========

	// Basic
	/* Reads data from the input stream. */
	template<typename T> bool Read(T* data, unsigned int length = 1);
	/* Reads data from the input stream. */
	virtual bool Read(void* data, unsigned int size, unsigned int length, unsigned int offset = 0) = 0;

	// Type Specific
	/* Reads the next character from the input stream. */
	signed char ReadChar();
	/* Reads the next unsigned character from the input stream. */
	unsigned char ReadUChar();
	/* Reads the next short from the input stream. */
	signed short ReadShort();
	/* Reads the next unsigned short from the input stream. */
	unsigned short ReadUShort();
	/* Reads the next integer from the input stream. */
	signed int ReadInt();
	/* Reads the next unsigned integer from the input stream. */
	unsigned int ReadUInt();
	/* Reads the next long from the input stream. */
	signed long ReadLong();
	/* Reads the next unsigned long from the input stream. */
	unsigned long ReadULong();
	/* Reads the next long long from the input stream. */
	signed long long ReadLongLong();
	/* Reads the next unsigned long long from the input stream. */
	unsigned long long ReadULongLong();
	/* Reads the next float from the input stream. */
	float ReadFloat();
	/* Reads the next double from the input stream. */
	double ReadDouble();
	/* Reads the next long double from the input stream. */
	long double ReadLongDouble();

	// String
	/* Reads the next string from the input stream until a null character is reached. */
	string ReadString();
	/* Reads the next string of the specified length from the input stream. */
	string ReadString(unsigned int length);
	/* Reads the next utf string from the input stream. */
	string ReadUTFString();

	//========= INFORMATION ==========

	/* Returns true if the stream is open. */
	virtual bool IsOpen() = 0;
	/* Returns true if there is an error. */
	virtual bool Error() = 0;
	/* Returns true if the end of the file has been reached. */
	virtual bool Eof() = 0;
	/* Gets the position of the pointer. */
	virtual long long GetPointer() = 0;
	/* Sets the position of the pointer. */
	virtual bool SetPointer(long long pointer, bool relative = false) = 0;
	/* Gets the size of the stream data. */
	virtual long long Size() = 0;
	/* Returns true if the stream is being read as little endian. */
	virtual bool IsLittleEndian() = 0;
	/* Sets whether the stream should be read as little endian. */
	virtual void SetLittleEndian(bool littleEndian) = 0;
};
/* Reads data from the input stream. */
template<typename T> bool PowerConsole::IOStream::InputStream::Read(T* data, unsigned int length) {
	return Read(data, sizeof(T), length);
}
/* The interface for all output streams. */
class OutputStream {
		
	//========== OPERATORS ===========
public:
	// Unary Assignment
	/* Increments the pointer of the stream. */
	OutputStream& operator++();
	/* Decrements the pointer of the stream. */
	OutputStream& operator--();

	// Binary Assignment
	/* Sets the pointer of the stream. */
	OutputStream& operator=(int rhs);
	/* Sets the pointer of the stream. */
	OutputStream& operator=(long long rhs);
	/* Increments the pointer of the stream. */
	OutputStream& operator+=(int rhs);
	/* Increments the pointer of the stream. */
	OutputStream& operator+=(long long rhs);
	/* Decrements the pointer of the stream. */
	OutputStream& operator-=(int rhs);
	/* Decrements the pointer of the stream. */
	OutputStream& operator-=(long long rhs);

	//========== MANAGEMENT ==========

	// Basic
	/* Writes data to the output stream. */
	template<typename T> bool Write(T* data, unsigned int length = 1);
	/* Writes data to the output stream. */
	virtual bool Write(void* data, unsigned int size, unsigned int length, unsigned int offset = 0) = 0;
	
	// Type Specific
	/* Writes the character to the output stream. */
	void WriteChar(signed char value);
	/* Writes the unsigned character to the output stream. */
	void WriteUChar(unsigned char value);
	/* Writes the short to the output stream. */
	void WriteShort(signed short value);
	/* Writes the unsigned short to the output stream. */
	void WriteUShort(unsigned short value);
	/* Writes the integer to the output stream. */
	void WriteInt(signed int value);
	/* Writes the unsigned integer to the output stream. */
	void WriteUInt(unsigned int value);
	/* Writes the long to the output stream. */
	void WriteLong(signed long value);
	/* Writes the unsigned long to the output stream. */
	void WriteULong(unsigned long value);
	/* Writes the long long to the output stream. */
	void WriteLongLong(signed long long value);
	/* Writes the unsigned long long to the output stream. */
	void WriteULongLong(unsigned long long value);
	/* Writes the float to the output stream. */
	void WriteFloat(float value);
	/* Writes the double to the output stream. */
	void WriteDouble(double value);
	/* Writes the long double to the output stream. */
	void WriteLongDouble(long double value);

	// String
	/* Writes the string to the input stream and terminates it with a null character. */
	void WriteString(const string& text);
	/* Writes the string of the specified length to the output stream. */
	void WriteString(const string& text, unsigned int length);
	/* Writes the utf string to the output stream. */
	void WriteUTFString(const string& text);

	//========= INFORMATION ==========

	/* Returns true if the stream is open. */
	virtual bool IsOpen() = 0;
	/* Returns true if there is an error. */
	virtual bool Error() = 0;
	/* Gets the position of the pointer. */
	virtual long long GetPointer() = 0;
	/* Sets the position of the pointer. */
	virtual bool SetPointer(long long pointer, bool relative = false) = 0;
	/* Gets the size of the stream data. */
	virtual long long Size() = 0;
	/* Returns true if the stream is being read as little endian. */
	virtual bool IsLittleEndian() = 0;
	/* Sets whether the stream should be read as little endian. */
	virtual void SetLittleEndian(bool littleEndian) = 0;
};
/* Writes data to the output stream. */
template<typename T> bool PowerConsole::IOStream::OutputStream::Write(T* data, unsigned int length) {
	return Write(data, sizeof(T), length);
}
/* An input stream for reading files. */
class FileInputStream : public InputStream {

	//=========== MEMBERS ============
private:
	/* The handle to the file being read. */
	HANDLE _hFile;
	/* The position of the pointer in the file. */
	long long _Pointer;
	/* The size of the file. */
	long long _Size;
	/* The state of the file. */
	short _State;
	/* True if there is an error. */
	bool _Error;
	/* True if the file is being read as little endian. */
	bool _LittleEndian;
	
	//========= CONSTRUCTORS =========
public:
	/* Constructs the default file input stream. */
	FileInputStream();
	/* Constructs a file input stream with the specified settings. */
	FileInputStream(bool littleEndian);
	/* Deconstructs the file input stream. */
	~FileInputStream();

	//========== MANAGEMENT ==========
	
	/* Opens the file. */
	bool Open(const string& path);
	/* Opens the file with the specified settings. */
	bool Open(const string& path, unsigned int shareMode, unsigned int attributes, unsigned int flags = 0x0);
	/* Closes the file. */
	bool Close();
	/* Reads data from the input stream. */
	bool Read(void* data, unsigned int size, unsigned int length, unsigned int offset = 0);
	/* Reads data from the input stream. */
	using InputStream::Read;

	//========= INFORMATION ==========

	/* Returns true if the stream is open. */
	bool IsOpen();
	/* Returns true if there is an error. */
	bool Error();
	/* Returns true if the end of the file has been reached. */
	bool Eof();
	/* Gets the position of the pointer. */
	long long GetPointer();
	/* Sets the position of the pointer. */
	bool SetPointer(long long pointer, bool relative = false);
	/* Gets the size of the stream data. */
	long long Size();
	/* Returns true if the stream is being read as little endian. */
	bool IsLittleEndian();
	/* Sets whether the stream should be read as little endian. */
	void SetLittleEndian(bool littleEndian);
};
/* An output stream for writing files. */
class FileOutputStream : public OutputStream {

	//=========== MEMBERS ============
private:
	/* The handle to the file being read. */
	HANDLE _hFile;
	/* The position of the pointer in the file. */
	long long _Pointer;
	/* The size of the file. */
	long long _Size;
	/* The state of the file. */
	short _State;
	/* True if there is an error. */
	bool _Error;
	/* True if the file is being read as little endian. */
	bool _LittleEndian;
	
	//========= CONSTRUCTORS =========
public:
	/* Constructs the default file output stream. */
	FileOutputStream();
	/* Constructs a file output stream with the specified settings. */
	FileOutputStream(bool littleEndian);
	/* Deconstructs the file output stream. */
	~FileOutputStream();

	//========== MANAGEMENT ==========
	
	/* Opens the file. */
	bool Open(const string& path);
	/* Opens the file with the specified settings. */
	bool Open(const string& path, unsigned int shareMode, unsigned int attributes, unsigned int flags = 0x0);
	/* Closes the file. */
	bool Close();
	/* Writes data to the output stream. */
	bool Write(void* data, unsigned int size, unsigned int length, unsigned int offset = 0);
	/* Writes data to the output stream. */
	using OutputStream::Write;

	//========= INFORMATION ==========

	/* Returns true if the stream is open. */
	bool IsOpen();
	/* Returns true if there is an error. */
	bool Error();
	/* Gets the position of the pointer. */
	long long GetPointer();
	/* Sets the position of the pointer. */
	bool SetPointer(long long pointer, bool relative = false);
	/* Gets the size of the stream data. */
	long long Size();
	/* Returns true if the stream is being read as little endian. */
	bool IsLittleEndian();
	/* Sets whether the stream should be read as little endian. */
	void SetLittleEndian(bool littleEndian);
};
/* An input stream for reading data from the clipboard. */
class ClipboardInputStream : public InputStream {

	//=========== MEMBERS ============
private:
	/* The handle to the memory being read. */
	HGLOBAL _hMem;
	/* The data of the memory. */
	unsigned char* _Data;
	/* The position of the pointer in the clipboard. */
	long long _Pointer;
	/* The size of the clipboard. */
	long long _Size;
	/* The state of the clipboard. */
	short _State;
	/* True if there is an error. */
	bool _Error;
	/* True if the clipboard is being read as little endian. */
	bool _LittleEndian;
	
	//========= CONSTRUCTORS =========
public:
	/* Constructs the default clipboard input stream. */
	ClipboardInputStream();
	/* Constructs a clipboard input stream with the specified settings. */
	ClipboardInputStream(bool littleEndian);
	/* Deconstructs the clipboard input stream. */
	~ClipboardInputStream();

	//========== MANAGEMENT ==========
	
	/* Opens the clipboard. */
	bool Open();
	/* Closes the clipboard. */
	bool Close();
	/* Gets the Data of the specified format from the clipboard. */
	bool GetData(unsigned int clipboardFormat);
	/* Reads data from the input stream. */
	bool Read(void* data, unsigned int size, unsigned int length, unsigned int offset = 0);
	/* Reads data from the input stream. */
	using InputStream::Read;
	/* Reads the file names from the clipboard. */
	vector<string> ReadFileNames();

	//========= INFORMATION ==========

	/* Returns true if the specified format is available. */
	bool IsFormatAvailable(unsigned int clipboardFormat);
	/* Returns true if the drop file format is available. */
	bool IsFileFormatAvailable();
	/* Returns true if the stream is open. */
	bool IsOpen();
	/* Returns true if there is an error. */
	bool Error();
	/* Returns true if the end of the file has been reached. */
	bool Eof();
	/* Gets the position of the pointer. */
	long long GetPointer();
	/* Sets the position of the pointer. */
	bool SetPointer(long long pointer, bool relative = false);
	/* Gets the size of the stream data. */
	long long Size();
	/* Returns true if the stream is being read as little endian. */
	bool IsLittleEndian();
	/* Sets whether the stream should be read as little endian. */
	void SetLittleEndian(bool littleEndian);
};
/* An output stream for writing data to the clipboard. */
class ClipboardOutputStream : public OutputStream {

	//=========== MEMBERS ============
private:
	/* The byte array of data for the clipboard. */
	vector<unsigned char> _Data;
	/* The position of the pointer in the clipboard. */
	long long _Pointer;
	/* The size of the clipboard. */
	long long _Size;
	/* The state of the clipboard. */
	short _State;
	/* True if there is an error. */
	bool _Error;
	/* True if the clipboard is being read as little endian. */
	bool _LittleEndian;
	
	//========= CONSTRUCTORS =========
public:
	/* Constructs the default clipboard output stream. */
	ClipboardOutputStream();
	/* Constructs a clipboard output stream with the specified settings. */
	ClipboardOutputStream(bool littleEndian);
	/* Deconstructs the clipboard output stream. */
	~ClipboardOutputStream();

	//========== MANAGEMENT ==========
	
	/* Opens the clipboard. */
	bool Open();
	/* Closes the clipboard. */
	bool Close();
	/* Sets the Data of the specified format to from the clipboard. */
	bool SetData(unsigned int clipboardFormat);
	/* Writes data to the output stream. */
	bool Write(void* data, unsigned int size, unsigned int length, unsigned int offset = 0);
	/* Writes data to the output stream. */
	using OutputStream::Write;

	//========= INFORMATION ==========

	/* Returns true if the stream is open. */
	bool IsOpen();
	/* Returns true if there is an error. */
	bool Error();
	/* Gets the position of the pointer. */
	long long GetPointer();
	/* Sets the position of the pointer. */
	bool SetPointer(long long pointer, bool relative = false);
	/* Gets the size of the stream data. */
	long long Size();
	/* Returns true if the stream is being read as little endian. */
	bool IsLittleEndian();
	/* Sets whether the stream should be read as little endian. */
	void SetLittleEndian(bool littleEndian);
};
/* An input stream for reading data from resources. */
class ResourceInputStream : public InputStream {

	//=========== MEMBERS ============
private:
	/* The handle to the memory being read. */
	HGLOBAL _hMem;
	/* The data of the memory. */
	unsigned char* _Data;
	/* The position of the pointer in the resource. */
	long long _Pointer;
	/* The size of the resource. */
	long long _Size;
	/* The state of the resource. */
	short _State;
	/* True if there is an error. */
	bool _Error;
	/* True if the resource is being read as little endian. */
	bool _LittleEndian;
	
	//========= CONSTRUCTORS =========
public:
	/* Constructs the default resource input stream. */
	ResourceInputStream();
	/* Constructs a resource input stream with the specified settings. */
	ResourceInputStream(bool littleEndian);
	/* Deconstructs the resource input stream. */
	~ResourceInputStream();

	//========== MANAGEMENT ==========
	
	/* Opens the resource. */
	bool Open(const string& name, const string& type);
	/* Opens the resource. */
	bool Open(const string& name, unsigned int typeId);
	/* Opens the resource. */
	bool Open(unsigned int id, const string& type);
	/* Opens the resource. */
	bool Open(unsigned int id, unsigned int typeId);
	/* Closes the resource. */
	bool Close();
	/* Reads data from the input stream. */
	bool Read(void* data, unsigned int size, unsigned int length, unsigned int offset = 0);
	/* Reads data from the input stream. */
	using InputStream::Read;

	//========= INFORMATION ==========

	/* Returns true if the specified resource exists. */
	bool ResourceExists(const string& name);
	/* Returns true if the specified resource exists. */
	bool ResourceExists(int id);
	/* Returns true if the stream is open. */
	bool IsOpen();
	/* Returns true if there is an error. */
	bool Error();
	/* Returns true if the end of the file has been reached. */
	bool Eof();
	/* Gets the position of the pointer. */
	long long GetPointer();
	/* Sets the position of the pointer. */
	bool SetPointer(long long pointer, bool relative = false);
	/* Gets the size of the stream data. */
	long long Size();
	/* Returns true if the stream is being read as little endian. */
	bool IsLittleEndian();
	/* Sets whether the stream should be read as little endian. */
	void SetLittleEndian(bool littleEndian);
};
//================================================================
// END IO STREAM HEADER:
//================================================================
}}
#endif