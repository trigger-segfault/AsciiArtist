//================================================================
// IO STREAM CPP:
//================================================================
#include "stdafx.h"
#include "IOStream.h"

using namespace std;
using namespace PowerConsole::IOStream;
using namespace PowerConsole::Windows;
//================================================================
// CLASSES:
//================================================================
// InputStream:
//----------------------------------------------------------------
//========== OPERATORS ===========
InputStream& PowerConsole::IOStream::InputStream::operator++() {
	SetPointer(1L, true);
	return *this;
}
InputStream& PowerConsole::IOStream::InputStream::operator--() {
	SetPointer(-1L, true);
	return *this;
}
//--------------------------------
InputStream& PowerConsole::IOStream::InputStream::operator=(int rhs) {
	SetPointer((long long)rhs, false);
	return *this;
}
InputStream& PowerConsole::IOStream::InputStream::operator=(long long rhs) {
	SetPointer(rhs, false);
	return *this;
}
InputStream& PowerConsole::IOStream::InputStream::operator+=(int rhs) {
	SetPointer((long long)rhs, true);
	return *this;
}
InputStream& PowerConsole::IOStream::InputStream::operator+=(long long rhs) {
	SetPointer(rhs, true);
	return *this;
}
InputStream& PowerConsole::IOStream::InputStream::operator-=(int rhs) {
	SetPointer((long long)-rhs, true);
	return *this;
}
InputStream& PowerConsole::IOStream::InputStream::operator-=(long long rhs) {
	SetPointer(-rhs, true);
	return *this;
}
//========== MANAGEMENT ==========
signed char PowerConsole::IOStream::InputStream::ReadChar() {
	signed char value = '\0';
	Read(&value);
	return value;
}
unsigned char PowerConsole::IOStream::InputStream::ReadUChar() {
	unsigned char value = '\0';
	Read(&value);
	return value;
}
signed short PowerConsole::IOStream::InputStream::ReadShort() {
	signed short value = 0;
	Read(&value);
	return value;
}
unsigned short PowerConsole::IOStream::InputStream::ReadUShort() {
	unsigned short value = 0;
	Read(&value);
	return value;
}
signed int PowerConsole::IOStream::InputStream::ReadInt() {
	signed int value = 0;
	Read(&value);
	return value;
}
unsigned int PowerConsole::IOStream::InputStream::ReadUInt() {
	unsigned int value = 0U;
	Read(&value);
	return value;
}
signed long PowerConsole::IOStream::InputStream::ReadLong() {
	signed long value = 0;
	Read(&value);
	return value;
}
unsigned long PowerConsole::IOStream::InputStream::ReadULong() {
	unsigned long value = 0U;
	Read(&value);
	return value;
}
signed long long PowerConsole::IOStream::InputStream::ReadLongLong() {
	signed long long value = 0L;
	Read(&value);
	return value;
}
unsigned long long PowerConsole::IOStream::InputStream::ReadULongLong() {
	unsigned long long value = 0LU;
	Read(&value);
	return value;
}
float PowerConsole::IOStream::InputStream::ReadFloat() {
	float value = 0.0f;
	Read(&value);
	return value;
}
double PowerConsole::IOStream::InputStream::ReadDouble() {
	double value = 0.0f;
	Read(&value);
	return value;
}
long double PowerConsole::IOStream::InputStream::ReadLongDouble() {
	long double value = 0.0f;
	Read(&value);
	return value;
}
//--------------------------------
string PowerConsole::IOStream::InputStream::ReadString() {
	string text = "";
	signed char nextChar = ReadChar();
	do {
		text += nextChar;
		nextChar = ReadChar();
	} while (nextChar != '\0');

	return text;
}
string PowerConsole::IOStream::InputStream::ReadString(unsigned int length) {
	string text = "";
	for (int i = 0; i < (int)length; i++) {
		text += ReadChar();
	}
	return text;
}
string PowerConsole::IOStream::InputStream::ReadUTFString() {
	unsigned short length = ReadUShort();
	string text = "";
	for (int i = 0; i < (int)length; i++) {
		text += ReadChar();
	}
	return text;
}
//----------------------------------------------------------------
// OutputStream:
//----------------------------------------------------------------
//========== OPERATORS ===========
OutputStream& PowerConsole::IOStream::OutputStream::operator++() {
	SetPointer(1L, true);
	return *this;
}
OutputStream& PowerConsole::IOStream::OutputStream::operator--() {
	SetPointer(-1L, true);
	return *this;
}
//--------------------------------
OutputStream& PowerConsole::IOStream::OutputStream::operator=(int rhs) {
	SetPointer((long long)rhs, false);
	return *this;
}
OutputStream& PowerConsole::IOStream::OutputStream::operator=(long long rhs) {
	SetPointer(rhs, false);
	return *this;
}
OutputStream& PowerConsole::IOStream::OutputStream::operator+=(int rhs) {
	SetPointer((long long)rhs, true);
	return *this;
}
OutputStream& PowerConsole::IOStream::OutputStream::operator+=(long long rhs) {
	SetPointer(rhs, true);
	return *this;
}
OutputStream& PowerConsole::IOStream::OutputStream::operator-=(int rhs) {
	SetPointer((long long)-rhs, true);
	return *this;
}
OutputStream& PowerConsole::IOStream::OutputStream::operator-=(long long rhs) {
	SetPointer(-rhs, true);
	return *this;
}
//========== MANAGEMENT ==========
void PowerConsole::IOStream::OutputStream::WriteChar(signed char value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteUChar(unsigned char value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteShort(signed short value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteUShort(unsigned short value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteInt(signed int value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteUInt(unsigned int value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteLong(signed long value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteULong(unsigned long value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteLongLong(signed long long value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteULongLong(unsigned long long value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteFloat(float value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteDouble(double value) {
	Write(&value);
}
void PowerConsole::IOStream::OutputStream::WriteLongDouble(long double value) {
	Write(&value);
}
//--------------------------------
void PowerConsole::IOStream::OutputStream::WriteString(const string& text) {
	for (int i = 0; i < (int)text.length(); i++) {
		WriteChar(text[i]);
	}
	WriteChar('\0');
}
void PowerConsole::IOStream::OutputStream::WriteString(const string& text, unsigned int length) {
	for (int i = 0; i < (int)length; i++) {
		if (i < (int)text.length())
			WriteChar(text[i]);
		else
			WriteChar('\0');
	}
}
void PowerConsole::IOStream::OutputStream::WriteUTFString(const string& text) {
	WriteUShort((unsigned short)text.length());
	for (int i = 0; i < (int)text.length(); i++) {
		WriteChar(text[i]);
	}
}
//----------------------------------------------------------------
// FileInputStream:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::IOStream::FileInputStream::FileInputStream() {
	this->_hFile			= nullptr;
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= true;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::FileInputStream::FileInputStream(bool littleEndian) {
	this->_hFile			= nullptr;
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= littleEndian;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::FileInputStream::~FileInputStream() {
	if(_State == 1) {
		CloseHandle(_hFile);
	}
}
//========== MANAGEMENT ==========
bool PowerConsole::IOStream::FileInputStream::Open(const string& path) {
	return Open(path, FileShareNone, FileAttributeNormal);
}
bool PowerConsole::IOStream::FileInputStream::Open(const string& path, unsigned int shareMode, unsigned int attributes, unsigned int flags) {
	if (_State == 0) {
		_hFile = CreateFileA(path.c_str(), GENERIC_READ, shareMode, nullptr, OPEN_EXISTING, attributes | flags, nullptr);

		if (_hFile != nullptr && _hFile != INVALID_HANDLE_VALUE) {
			_State		= 1;
			_Error		= false;
			_Pointer	= 0L;

			unsigned long lowSize = 0, highSize = 0;
			lowSize		= GetFileSize(_hFile, &highSize);
			_Size		= (long long)(((unsigned long long)highSize << 32) + (unsigned long long)lowSize);
			return true;
		}
		_hFile		= nullptr;
	}
	return false;
}
bool PowerConsole::IOStream::FileInputStream::Close() {
	if (_State == 1) {
		if (CloseHandle(_hFile) != 0) {
			_hFile		= nullptr;
			_State		= 0;
			_Error		= false;
			_Pointer	= 0L;
			_Size		= 0L;
			return true;
		}
		_Error = true;
	}
	return false;
}
bool PowerConsole::IOStream::FileInputStream::Read(void* data, unsigned int size, unsigned int length, unsigned int offset) {
	if (_State == 1 && !_Error && size * length > 0) {

		unsigned long bytesRead = 0;
		if (_Pointer + (long long)(size * length) > _Size) {
			_Pointer	= _Size;
			_Error		= true;
		}
		else if (_LittleEndian) {
			if (ReadFile(_hFile, (unsigned char*)data + offset, (unsigned long)(size * length), &bytesRead, nullptr) != 0) {
				_Pointer	+= (long long)bytesRead;
			}
			else {
				_Error		= true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !_Error; i++) {
				for (int j = 0; j < (int)size && !_Error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (ReadFile(_hFile, (unsigned char*)data + pos, 1, &bytesRead, nullptr) != 0) {
						_Pointer++;
					}
					else {
						_Error		= true;
					}
				}
			}
		}
		return !_Error;
	}
	return false;
}
//========= INFORMATION ==========
bool PowerConsole::IOStream::FileInputStream::IsOpen() {
	return _State != 0;
}
bool PowerConsole::IOStream::FileInputStream::Error() {
	return _Error;
}
bool PowerConsole::IOStream::FileInputStream::Eof() {
	return _Pointer >= _Size;
}
long long PowerConsole::IOStream::FileInputStream::GetPointer() {
	return _Pointer;
}
bool PowerConsole::IOStream::FileInputStream::SetPointer(long long pointer, bool relative) {
	// Make sure the pointer position is valid
	unsigned long long maxPos = (unsigned long long)(numeric_limits<long long>::max)();
	bool validPos = false;
	if (relative && _Pointer + pointer >= 0L && (unsigned long long)(_Pointer + pointer) <= maxPos)
		validPos = true;
	if (!relative && pointer >= 0L && (unsigned long long)pointer <= maxPos)
		validPos = true;
	
	if (_State == 1 && !_Error && validPos) {
		long lowDistance			= (long)(pointer & 0xFFFFFFFF);
		long highDistance			= (long)((pointer & 0xFFFFFFFF00000000) >> 32);
		unsigned long moveMethod	= (relative ? FILE_CURRENT : FILE_BEGIN);
		if (SetFilePointer(_hFile, lowDistance, &highDistance, moveMethod) != INVALID_SET_FILE_POINTER) {
			if (relative)
				_Pointer += pointer;
			else
				_Pointer = pointer;
			return true;
		}
		_Error = true;
	}
	return false;
}
long long PowerConsole::IOStream::FileInputStream::Size() {
	return _Size;
}
bool PowerConsole::IOStream::FileInputStream::IsLittleEndian() {
	return _LittleEndian;
}
void PowerConsole::IOStream::FileInputStream::SetLittleEndian(bool littleEndian) {
	_LittleEndian = littleEndian;
}
//----------------------------------------------------------------
// FileOutputStream:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::IOStream::FileOutputStream::FileOutputStream() {
	this->_hFile			= nullptr;
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= true;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::FileOutputStream::FileOutputStream(bool littleEndian) {
	this->_hFile			= nullptr;
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= littleEndian;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::FileOutputStream::~FileOutputStream() {
	if (_State == 1) {
		CloseHandle(_hFile);
	}
}
//========== MANAGEMENT ==========
bool PowerConsole::IOStream::FileOutputStream::Open(const string& path) {
	return Open(path, FileShareNone, FileAttributeNormal);
}
bool PowerConsole::IOStream::FileOutputStream::Open(const string& path, unsigned int shareMode, unsigned int attributes, unsigned int flags) {
	if (_State == 0) {
		_hFile = CreateFileA(path.c_str(), GENERIC_WRITE, shareMode, nullptr, CREATE_ALWAYS, attributes | flags, nullptr);

		if (_hFile != nullptr && _hFile != INVALID_HANDLE_VALUE) {
			_State		= 1;
			_Error		= false;
			_Pointer	= 0L;

			unsigned long lowSize = 0, highSize = 0;
			lowSize		= GetFileSize(_hFile, &highSize);
			_Size		= (long long)(((unsigned long long)highSize << 32) + (unsigned long long)lowSize);
			return true;
		}
		_hFile = nullptr;
	}
	return false;
}
bool PowerConsole::IOStream::FileOutputStream::Close() {
	if (_State == 1) {
		if (CloseHandle(_hFile) != 0) {
			_hFile		= nullptr;
			_State		= 0;
			_Error		= false;
			_Pointer	= 0L;
			_Size		= 0L;
			return true;
		}
		_Error = true;
	}
	return false;
}
bool PowerConsole::IOStream::FileOutputStream::Write(void* data, unsigned int size, unsigned int length, unsigned int offset) {
	if (_State == 1 && !_Error && size * length > 0) {

		unsigned long bytesWritten = 0;
		if (_LittleEndian) {
			if (WriteFile(_hFile, (unsigned char*)data + offset, size * length, &bytesWritten, nullptr) != 0) {
				_Pointer	+= (long long)bytesWritten;
			}
			else {
				_Error		= true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !_Error; i++) {
				for (int j = 0; j < (int)size && !_Error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (WriteFile(_hFile, (unsigned char*)data + pos, 1, &bytesWritten, nullptr) != 0) {
						_Pointer++;
					}
					else {
						_Error		= true;
					}
				}
			}
		}
		if (_Pointer > _Size)
			_Size	= _Pointer;
		return !_Error;
	}
	return false;
}
//========= INFORMATION ==========
bool PowerConsole::IOStream::FileOutputStream::IsOpen() {
	return _State != 0;
}
bool PowerConsole::IOStream::FileOutputStream::Error() {
	return _Error;
}
long long PowerConsole::IOStream::FileOutputStream::GetPointer() {
	return _Pointer;
}
bool PowerConsole::IOStream::FileOutputStream::SetPointer(long long pointer, bool relative) {
	// Make sure the pointer position is valid
	unsigned long long maxPos = (unsigned long long)(numeric_limits<long long>::max)();
	bool validPos = false;
	if (relative && _Pointer + pointer >= 0L && (unsigned long long)(_Pointer + pointer) <= maxPos)
		validPos = true;
	if (!relative && pointer >= 0L && (unsigned long long)pointer <= maxPos)
		validPos = true;
	
	if (_State == 1 && !_Error && validPos) {
		long lowDistance			= (long)(pointer & 0xFFFFFFFF);
		long highDistance			= (long)((pointer & 0xFFFFFFFF00000000) >> 32);
		unsigned long moveMethod	= (relative ? FILE_CURRENT : FILE_BEGIN);
		if (SetFilePointer(_hFile, lowDistance, &highDistance, moveMethod) != INVALID_SET_FILE_POINTER) {
			if (relative)
				_Pointer += pointer;
			else
				_Pointer = pointer;
			if (_Pointer > _Size)
				_Size	= _Pointer;
			return true;
		}
		_Error = true;
	}
	return false;
}
long long PowerConsole::IOStream::FileOutputStream::Size() {
	return _Size;
}
bool PowerConsole::IOStream::FileOutputStream::IsLittleEndian() {
	return _LittleEndian;
}
void PowerConsole::IOStream::FileOutputStream::SetLittleEndian(bool littleEndian) {
	_LittleEndian = littleEndian;
}
//----------------------------------------------------------------
// ClipboardInputStream:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::IOStream::ClipboardInputStream::ClipboardInputStream() {
	this->_hMem				= nullptr;
	this->_Data				= nullptr;
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= true;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::ClipboardInputStream::ClipboardInputStream(bool littleEndian) {
	this->_hMem				= nullptr;
	this->_Data				= nullptr;
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= littleEndian;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::ClipboardInputStream::~ClipboardInputStream() {
	if (_State != 0) {
		if (_State == 2)
			GlobalUnlock(_hMem);
		CloseClipboard();
	}
}
//========== MANAGEMENT ==========
bool PowerConsole::IOStream::ClipboardInputStream::Open() {
	if (_State == 0) {
		if (OpenClipboard(GetConsoleWindow()) != 0) {
			_hMem		= nullptr;
			_Data		= nullptr;
			_State		= 1;
			_Error		= false;
			_Pointer	= 0L;
			_Size		= 0L;
			return true;
		}
	}
	return false;
}
bool PowerConsole::IOStream::ClipboardInputStream::Close() {
	if (_State != 0) {
		if (_State == 2) {
			if (GlobalUnlock(_hMem) != 0)
				_Error = true;
		}
		if (CloseClipboard() != 0) {
			_hMem		= nullptr;
			_Data		= nullptr;
			_State		= 0;
			_Error		= false;
			_Pointer	= 0L;
			_Size		= 0L;
			return true;
		}
		_Error = true;
	}
	return false;
}
bool PowerConsole::IOStream::ClipboardInputStream::GetData(unsigned int clipboardFormat) {
	if (_State != 0 && !_Error) {
		// Close any previous data
		if (_State == 2) {
			if (GlobalUnlock(_hMem) == 0)
				_Error = true;
		}
		// Get location of clipboard data
		_hMem = GetClipboardData(clipboardFormat);
		if (_hMem != nullptr) {
			// Get access to the memory
			_Data = (unsigned char*)GlobalLock(_hMem);
			if (_Data != nullptr) {
				_State		= 2;
				_Pointer	= 0L;
				_Size		= (long long)GlobalSize(_hMem);
				return true;
			}
		}
		_Error = true;
	}
	return false;
}
bool PowerConsole::IOStream::ClipboardInputStream::Read(void* data, unsigned int size, unsigned int length, unsigned int offset) {
	if (_State == 2 && !_Error && size * length > 0) {

		if (_Pointer + (long long)(size * length) > _Size) {
			_Pointer	= _Size;
			_Error		= true;
		}
		else if (_LittleEndian) {
			if (memcpy_s((unsigned char*)data + offset, size * length, _Data + _Pointer, size * length) == 0) {
				_Pointer	+= (long long)(size * length);
			}
			else {
				_Error = true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !_Error; i++) {
				for (int j = 0; j < (int)size && !_Error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (memcpy_s((unsigned char*)data + pos, 1, _Data + _Pointer, 1) == 0) {
						_Pointer++;
					}
					else {
						_Error		= true;
					}
				}
			}
		}
		return !_Error;
	}
	return false;
}
vector<string> PowerConsole::IOStream::ClipboardInputStream::ReadFileNames() {
	vector<string> names;

	if ((_State == 1 || _State == 2) && !_Error) {
		_hMem = GetClipboardData(CF_HDROP);
		if (_hMem != nullptr) {
			HDROP hDrop = (HDROP)GlobalLock(_hMem);
			if (hDrop != nullptr) {
				// Read off each file name
				unsigned int numFiles = DragQueryFileA(hDrop, -1, nullptr, 0);
				for (int i = 0; i < (int)numFiles; i++) {
					unsigned int length = DragQueryFileA(hDrop, 0, nullptr, 0);

					char* cFileName = new char[length + 1];
					if (DragQueryFileA(hDrop, 0, cFileName, length + 1) != 0 && length > 0) {
						names.push_back(cFileName);
					}
					else {
						_Error = true;
						break;
					}
				}
				DragFinish(hDrop);
				return names;
			}
		}
		_Error = true;
	}
	return names;
}
//========= INFORMATION ==========
bool PowerConsole::IOStream::ClipboardInputStream::IsFormatAvailable(unsigned int clipboardFormat) {
	return IsClipboardFormatAvailable(clipboardFormat) != 0;
}
bool PowerConsole::IOStream::ClipboardInputStream::IsFileFormatAvailable() {
	return IsClipboardFormatAvailable(CF_HDROP) != 0;
}
bool PowerConsole::IOStream::ClipboardInputStream::IsOpen() {
	return _State != 0;
}
bool PowerConsole::IOStream::ClipboardInputStream::Error() {
	return _Error;
}
bool PowerConsole::IOStream::ClipboardInputStream::Eof() {
	return _Pointer >= _Size;
}
long long PowerConsole::IOStream::ClipboardInputStream::GetPointer() {
	return _Pointer;
}
bool PowerConsole::IOStream::ClipboardInputStream::SetPointer(long long pointer, bool relative) {
	// Make sure the pointer position is valid
	unsigned long long maxPos = (unsigned long long)(numeric_limits<long long>::max)();
	bool validPos = false;
	if (relative && _Pointer + pointer >= 0L && (unsigned long long)(_Pointer + pointer) <= maxPos)
		validPos = true;
	if (!relative && pointer >= 0L && (unsigned long long)pointer <= maxPos)
		validPos = true;
	
	if (_State == 2 && !_Error && validPos) {
		if (relative)
			_Pointer += pointer;
		else
			_Pointer = pointer;
		if (_Pointer > _Size)
			_Pointer = _Size;
		return true;
	}
	return false;
}
long long PowerConsole::IOStream::ClipboardInputStream::Size() {
	return _Size;
}
bool PowerConsole::IOStream::ClipboardInputStream::IsLittleEndian() {
	return _LittleEndian;
}
void PowerConsole::IOStream::ClipboardInputStream::SetLittleEndian(bool littleEndian) {
	_LittleEndian = littleEndian;
}
//----------------------------------------------------------------
// ClipboardOutputStream:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::IOStream::ClipboardOutputStream::ClipboardOutputStream() {
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= true;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::ClipboardOutputStream::ClipboardOutputStream(bool littleEndian) {
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= littleEndian;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::ClipboardOutputStream::~ClipboardOutputStream() {
	if (_State == 1) {
		CloseClipboard();
	}
}
//========== MANAGEMENT ==========
bool PowerConsole::IOStream::ClipboardOutputStream::Open() {
	if (_State == 0) {
		if (OpenClipboard(GetConsoleWindow()) != 0) {
			if (EmptyClipboard() != 0) {
				_Data.clear();
				_State		= 1;
				_Error		= false;
				_Pointer	= 0L;
				_Size		= 0L;
				return true;
			}
		}
	}
	return false;
}
bool PowerConsole::IOStream::ClipboardOutputStream::Close() {
	if (_State == 1) {
		if (CloseClipboard() != 0) {
			_Data.clear();
			_State		= 0;
			_Error		= false;
			_Pointer	= 0L;
			_Size		= 0L;
			return true;
		}
		_Error = true;
	}
	return false;
}
bool PowerConsole::IOStream::ClipboardOutputStream::SetData(unsigned int clipboardFormat) {
	if (_State == 1 && !_Error) {
		
		// Allocate the memory for storing the clipboard data
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)_Size);
		if (hMem != nullptr) {

			// Get access to the memory
			unsigned char* data = (unsigned char*)GlobalLock(hMem);
			if (data != nullptr) {
				
				// Copy all the data, byte-by-byte so it's not put into little endian a second time
				for (int i = 0; i < (int)_Data.size(); i++) {
					if (memcpy_s(data + i, 1, _Data.data() + i, 1) != 0) {
						_Error = true;
						break;
					}
				}
				// Give back access
				if (GlobalUnlock(hMem) == 0) {
					// Finalize the clipboard data and set the specified format
					if (SetClipboardData(clipboardFormat, hMem) != nullptr) {
						// Clear the data for next format to be written
						_Data.clear();
						_Pointer	= 0L;
						_Size		= 0L;
						return true;
					}
				}
			}

		}
		_Error = true;
	}

	return !_Error;
}
bool PowerConsole::IOStream::ClipboardOutputStream::Write(void* data, unsigned int size, unsigned int length, unsigned int offset) {
	if (_State == 1 && !_Error && size * length > 0) {

		// Resize the byte array if needed
		if (_Pointer + (long long)(size * length) > _Size) {
			_Data.resize((unsigned int)(_Pointer + (long long)(size * length)));
			_Size = (long long)_Data.size();
		}

		if (_LittleEndian) {
			if (memcpy_s(_Data.data() + _Pointer, size * length, (unsigned char*)data + offset, size * length) == 0) {
				_Pointer += size * length;
			}
			else {
				_Error = true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !_Error; i++) {
				for (int j = 0; j < (int)size && !_Error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (memcpy_s(_Data.data() + _Pointer, 1, (unsigned char*)data + pos, 1) == 0) {
						_Pointer++;
					}
					else {
						_Error		= true;
					}
				}
			}
		}
		return true;
	}
	return false;
}
//========= INFORMATION ==========
bool PowerConsole::IOStream::ClipboardOutputStream::IsOpen() {
	return _State != 0;
}
bool PowerConsole::IOStream::ClipboardOutputStream::Error() {
	return _Error;
}
long long PowerConsole::IOStream::ClipboardOutputStream::GetPointer() {
	return _Pointer;
}
bool PowerConsole::IOStream::ClipboardOutputStream::SetPointer(long long pointer, bool relative) {
	// Make sure the pointer position is valid
	unsigned long long maxPos = (unsigned long long)(numeric_limits<long long>::max)();
	bool validPos = false;
	if (relative && _Pointer + pointer >= 0L && (unsigned long long)(_Pointer + pointer) <= maxPos)
		validPos = true;
	if (!relative && pointer >= 0L && (unsigned long long)pointer <= maxPos)
		validPos = true;
	
	if (_State == 1 && !_Error && validPos) {
		if (relative)
			_Pointer += pointer;
		else
			_Pointer = pointer;

		// Resize the byte array if needed
		if (_Pointer > _Size) {
			_Data.resize((unsigned int)_Pointer);
			_Size = (long long)_Data.size();
		}
		return true;
	}
	return false;
}
long long PowerConsole::IOStream::ClipboardOutputStream::Size() {
	return _Size;
}
bool PowerConsole::IOStream::ClipboardOutputStream::IsLittleEndian() {
	return _LittleEndian;
}
void PowerConsole::IOStream::ClipboardOutputStream::SetLittleEndian(bool littleEndian) {
	_LittleEndian = littleEndian;
}
//----------------------------------------------------------------
// ClipboardInputStream:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::IOStream::ResourceInputStream::ResourceInputStream() {
	this->_hMem				= nullptr;
	this->_Data				= nullptr;
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= true;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::ResourceInputStream::ResourceInputStream(bool littleEndian) {
	this->_hMem				= nullptr;
	this->_Data				= nullptr;
	this->_State			= 0;
	this->_Error			= false;
	this->_LittleEndian		= littleEndian;
	this->_Pointer			= 0L;
	this->_Size				= 0L;
}
PowerConsole::IOStream::ResourceInputStream::~ResourceInputStream() {
	if (_State == 1) {
		
	}
}
//========== MANAGEMENT ==========
bool PowerConsole::IOStream::ResourceInputStream::Open(const string& name, const string& type) {
	if (_State == 0) {
		// Locate the resource
		//MAKEINTRESOURCEA(RT_RCDATA)
		HRSRC hRes = FindResourceA(nullptr, name.c_str(), type.c_str());
		if (hRes != nullptr) {

			// Get the location the resource in memory
			_hMem = LoadResource(nullptr, hRes);
			if (_hMem != nullptr) {

				// Get access to the memory
				_Data = (unsigned char*)LockResource(_hMem);
				if (_Data != nullptr) {
					_State		= 1;
					_Error		= false;
					_Pointer	= 0L;
					_Size		= (long long)SizeofResource(nullptr, hRes);
					return true;
				}
			}
		}
		_Error = true;
	}
	return false;
}
bool PowerConsole::IOStream::ResourceInputStream::Open(const string& name, unsigned int typeId) {
	ostringstream oss;
	oss << "#" << typeId;
	return Open(name, oss.str());
}
bool PowerConsole::IOStream::ResourceInputStream::Open(unsigned int id, const string& type) {
	ostringstream oss;
	oss << "#" << id;
	return Open(oss.str(), type);
}
bool PowerConsole::IOStream::ResourceInputStream::Open(unsigned int id, unsigned int typeId) {
	ostringstream oss;
	oss << "#" << typeId;
	return Open(id, oss.str());
}
bool PowerConsole::IOStream::ResourceInputStream::Close() {
	if (_State == 1) {
		_hMem		= nullptr;
		_Data		= nullptr;
		_State		= 0;
		_Error		= false;
		_Pointer	= 0L;
		_Size		= 0L;
		return true;
	}
	return false;
}
bool PowerConsole::IOStream::ResourceInputStream::Read(void* data, unsigned int size, unsigned int length, unsigned int offset) {
	if (_State == 1 && !_Error && size * length > 0) {

		if (_Pointer + (long long)(size * length) > _Size) {
			_Pointer	= _Size;
			_Error		= true;
		}
		else if (_LittleEndian) {
			if (memcpy_s((unsigned char*)data + offset, size * length, _Data + _Pointer, size * length) == 0) {
				_Pointer	+= (long long)(size * length);
			}
			else {
				_Error = true;
			}
		}
		else {
			for (int i = 0; i < (int)length && !_Error; i++) {
				for (int j = 0; j < (int)size && !_Error; j++) {
					unsigned int pos = offset + (i * size) + (size - j - 1);
					if (memcpy_s((unsigned char*)data + pos, 1, _Data + _Pointer, 1) == 0) {
						_Pointer++;
					}
					else {
						_Error		= true;
					}
				}
			}
		}
		return !_Error;
	}
	return false;
}
//========= INFORMATION ==========
bool PowerConsole::IOStream::ResourceInputStream::IsOpen() {
	return _State != 0;
}
bool PowerConsole::IOStream::ResourceInputStream::Error() {
	return _Error;
}
bool PowerConsole::IOStream::ResourceInputStream::Eof() {
	return _Pointer >= _Size;
}
long long PowerConsole::IOStream::ResourceInputStream::GetPointer() {
	return _Pointer;
}
bool PowerConsole::IOStream::ResourceInputStream::SetPointer(long long pointer, bool relative) {
	// Make sure the pointer position is valid
	unsigned long long maxPos = (unsigned long long)(numeric_limits<long long>::max)();
	bool validPos = false;
	if (relative && _Pointer + pointer >= 0L && (unsigned long long)(_Pointer + pointer) <= maxPos)
		validPos = true;
	if (!relative && pointer >= 0L && (unsigned long long)pointer <= maxPos)
		validPos = true;
	
	if (_State == 1 && !_Error && validPos) {
		if (relative)
			_Pointer += pointer;
		else
			_Pointer = pointer;
		if (_Pointer > _Size)
			_Pointer = _Size;
		return true;
	}
	return false;
}
long long PowerConsole::IOStream::ResourceInputStream::Size() {
	return _Size;
}
bool PowerConsole::IOStream::ResourceInputStream::IsLittleEndian() {
	return _LittleEndian;
}
void PowerConsole::IOStream::ResourceInputStream::SetLittleEndian(bool littleEndian) {
	_LittleEndian = littleEndian;
}
//================================================================
// END IO STREAM CPP:
//================================================================