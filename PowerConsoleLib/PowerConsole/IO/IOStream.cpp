/*=================================================================|
|* File:				IOStream.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "IOStream.h"

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== OPERATORS ===========
#pragma region Operators

BasicStream& BasicStream::operator++() {
	setPointer(1L, true);
	return *this;
}
BasicStream& BasicStream::operator--() {
	setPointer(-1L, true);
	return *this;
}

BasicStream& BasicStream::operator=(int rhs) {
	setPointer((long long)rhs, false);
	return *this;
}
BasicStream& BasicStream::operator=(long long rhs) {
	setPointer(rhs, false);
	return *this;
}
BasicStream& BasicStream::operator+=(int rhs) {
	setPointer((long long)rhs, true);
	return *this;
}
BasicStream& BasicStream::operator+=(long long rhs) {
	setPointer(rhs, true);
	return *this;
}
BasicStream& BasicStream::operator-=(int rhs) {
	setPointer((long long)-rhs, true);
	return *this;
}
BasicStream& BasicStream::operator-=(long long rhs) {
	setPointer(-rhs, true);
	return *this;
}

#pragma endregion
//=========== READING ============
#pragma region Reading

signed char InputStream::readChar() {
	signed char value = '\0';
	read(&value);
	return value;
}
unsigned char InputStream::readUChar() {
	unsigned char value = '\0';
	read(&value);
	return value;
}
signed short InputStream::readShort() {
	signed short value = 0;
	read(&value);
	return value;
}
unsigned short InputStream::readUShort() {
	unsigned short value = 0;
	read(&value);
	return value;
}
signed int InputStream::readInt() {
	signed int value = 0;
	read(&value);
	return value;
}
unsigned int InputStream::readUInt() {
	unsigned int value = 0U;
	read(&value);
	return value;
}
signed long InputStream::readLong() {
	signed long value = 0;
	read(&value);
	return value;
}
unsigned long InputStream::readULong() {
	unsigned long value = 0U;
	read(&value);
	return value;
}
signed long long InputStream::readLongLong() {
	signed long long value = 0L;
	read(&value);
	return value;
}
unsigned long long InputStream::readULongLong() {
	unsigned long long value = 0LU;
	read(&value);
	return value;
}
float InputStream::readFloat() {
	float value = 0.0f;
	read(&value);
	return value;
}
double InputStream::readDouble() {
	double value = 0.0f;
	read(&value);
	return value;
}
long double InputStream::readLongDouble() {
	long double value = 0.0f;
	read(&value);
	return value;
}

std::string InputStream::readString() {
	std::string text = "";
	signed char nextChar = readChar();
	do {
		text += nextChar;
		nextChar = readChar();
	} while (nextChar != '\0' && !isEof() && !hasError());

	return text;
}
std::string InputStream::readString(unsigned int length) {
	std::string text = "";
	for (int i = 0; i < (int)length && !isEof() && !hasError(); i++) {
		char ch = readChar();
		if (ch == '\0') {
			setPointer(length - i - 1, true);
			break;
		}
		else {
			text += ch;
		}
	}
	return text;
}
std::string InputStream::readUTFString() {
	unsigned short length = readUShort();
	std::string text = "";
	for (int i = 0; i < (int)length && !isEof() && !hasError(); i++) {
		char ch = readChar();
		if (ch != '\0')
			text += ch;
	}
	return text;
}

#pragma endregion
//=========== WRITING ============
#pragma region Writing

void OutputStream::writeChar(signed char value) {
	write(&value);
}
void OutputStream::writeUChar(unsigned char value) {
	write(&value);
}
void OutputStream::writeShort(signed short value) {
	write(&value);
}
void OutputStream::writeUShort(unsigned short value) {
	write(&value);
}
void OutputStream::writeInt(signed int value) {
	write(&value);
}
void OutputStream::writeUInt(unsigned int value) {
	write(&value);
}
void OutputStream::writeLong(signed long value) {
	write(&value);
}
void OutputStream::writeULong(unsigned long value) {
	write(&value);
}
void OutputStream::writeLongLong(signed long long value) {
	write(&value);
}
void OutputStream::writeULongLong(unsigned long long value) {
	write(&value);
}
void OutputStream::writeFloat(float value) {
	write(&value);
}
void OutputStream::writeDouble(double value) {
	write(&value);
}
void OutputStream::writeLongDouble(long double value) {
	write(&value);
}

void OutputStream::writeString(const std::string& text) {
	for (int i = 0; i < (int)text.length(); i++) {
		writeChar(text[i]);
	}
	writeChar('\0');
}
void OutputStream::writeString(const std::string& text, unsigned int length) {
	for (int i = 0; i < (int)length; i++) {
		if (i < (int)text.length())
			writeChar(text[i]);
		else
			writeChar('\0');
	}
}
void OutputStream::writeUTFString(const std::string& text) {
	writeUShort((unsigned short)text.length());
	for (int i = 0; i < (int)text.length(); i++) {
		writeChar(text[i]);
	}
}
void OutputStream::writeText(const std::string& text) {
	for (int i = 0; i < (int)text.length(); i++) {
		writeChar(text[i]);
	}
}

#pragma endregion
//=================================================================|
