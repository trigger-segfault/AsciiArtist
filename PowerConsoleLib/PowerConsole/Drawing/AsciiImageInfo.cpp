/*=================================================================|
|* File:				AsciiImageInfo.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AsciiImageInfo.h"
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/IO/Clipboard.h>
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/IO/ResourceStream.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========== CONSTANTS ===========
#pragma region Constants

const unsigned short AsciiImageInfo::FILE_VERSION = 4;
const unsigned short AsciiImageInfo::SMALL_FILE_VERSION = 1;
const std::string AsciiImageInfo::FILE_SIGNATURE = "ASCIMG";
const std::string AsciiImageInfo::SMALL_FILE_SIGNATURE = "ASCSML";
const Point2I AsciiImageInfo::MAX_SIZE = Point2I(1024, 1024);
const unsigned int AsciiImageInfo::MAX_FRAMES = 1024;
const std::string AsciiImageInfo::CLIPBOARD_FORMAT_NAME = "Ascii Image";
const unsigned int AsciiImageInfo::CLIPBOARD_FORMAT = Clipboard::registerFormat(AsciiImageInfo::CLIPBOARD_FORMAT_NAME);

#pragma endregion
//========= INPUT/OUTPUT =========
#pragma region Input/Output

AsciiImageHeader AsciiImageInfo::createHeader(const AsciiImage& image, unsigned short version, bool smallFile) {
	AsciiImageHeader header = AsciiImageHeader();
	AsciiImageFrameHeader frame = AsciiImageFrameHeader();
	header.signature = (smallFile ? SMALL_FILE_SIGNATURE : FILE_SIGNATURE);
	header.version = version;

	header.bytesPerName = (unsigned short)image.getName().length();
	header.bytesPerAttribute = 2;
	if (!smallFile && version < 3)
		header.bytesPerAttribute = 1;

	header.headerOffset = 20;
	if (!smallFile && version < 3)
		header.headerOffset = 0;
	header.headerSize = 24 + (2 + header.bytesPerAttribute);
	if (!smallFile && version < 3)
		header.headerSize = 23;
	else if (smallFile)
		header.headerSize = 8;
	header.frameArrayOffset = header.headerOffset + header.headerSize;
	header.frameHeaderSize = 10 + header.bytesPerName;
	if (smallFile || version < 3)
		header.frameHeaderSize = 0;

	header.size = image.frameSize;
	header.format = image.format;
	header.frameCount = 1;
	header.animationSpeed = 800;
	header.background = image.background;

	header.frameSize = image.frameSize.x * image.frameSize.y * (2 + header.bytesPerAttribute);
	header.fileSize = header.frameArrayOffset + header.frameHeaderSize + header.frameSize;

	frame.frameOffset = header.frameArrayOffset;
	frame.frameSize = header.frameSize;
	frame.name = image.name;
	frame.size = image.frameSize;
	frame.frameSpeed = image.frameSpeed;
	frame.layers = 1;
	header.frameList.push_back(frame);

	return header;
}
AsciiImageHeader AsciiImageInfo::createHeader(const AsciiAnimation& image, unsigned short version, bool smallFile) {
	AsciiImageHeader header = AsciiImageHeader();
	AsciiImageFrameHeader frame = AsciiImageFrameHeader();
	header.signature = (smallFile ? SMALL_FILE_SIGNATURE : FILE_SIGNATURE);
	header.version = version;

	header.bytesPerName = 0;
	for (int i = 0; i < (int)image.frameCount(); i++)
		header.bytesPerName = std::max(header.bytesPerName, (unsigned short)image.getName(i).length());
	header.bytesPerAttribute = 2;
	if (!smallFile && version < 3)
		header.bytesPerAttribute = 1;

	header.headerOffset = 20;
	if (!smallFile && version < 3)
		header.headerOffset = 0;
	else if (smallFile)
		header.headerOffset = 8;
	header.headerSize = 24 + (2 + header.bytesPerAttribute);
	if (!smallFile && version < 3)
		header.headerSize = 23;
	else if (smallFile)
		header.headerSize = 8;
	header.frameArrayOffset = header.headerOffset + header.headerSize;
	header.frameHeaderSize = 10 + header.bytesPerName;
	if (smallFile || version < 3)
		header.frameHeaderSize = 0;

	header.size = image.size();
	header.format = image.getFormat();
	header.frameCount = image.frameCount();
	header.animationSpeed = image.getAnimationSpeed();
	header.background = image.getBackground();

	header.frameSize = image.size().x * image.size().y * (2 + header.bytesPerAttribute);
	header.fileSize = header.frameArrayOffset + header.frameHeaderSize * image.frameCount();

	for (int i = 0; i < (int)image.frameCount(); i++) {
		if (i == 0) {
			frame.frameOffset = header.frameArrayOffset;
		}
		else {
			AsciiImageFrameHeader& lastFrame = header.frameList[i - 1];
			frame.frameOffset = lastFrame.frameOffset + header.frameHeaderSize + (lastFrame.frameSize * lastFrame.layers);
		}

		if (version >= 4 && !smallFile)
			frame.frameSize = image[i].size().x * image[i].size().y * (2 + header.bytesPerAttribute);
		else
			frame.frameSize = header.frameSize;

		frame.name = image[i].name;
		frame.size = image[i].size();
		frame.frameSpeed = image[i].getFrameSpeed();
		frame.layers = 1;
		header.frameList.push_back(frame);
		header.fileSize += frame.frameSize * frame.layers;
	}

	return header;
}

bool AsciiImageInfo::isFileAsciiImage(const std::string& path) {
	AsciiImageHeader header = AsciiImageHeader();
	FileInputStream in = FileInputStream();
	if (in.open(path)) {
		bool success = loadHeader(&in, header, false);
		in.close();
		return success;
	}
	return false;
}

bool AsciiImageInfo::loadPixel(InputStream* in, const AsciiImageHeader& header, Pixel& pixel) {
	bool oldEndian = in->isBigEndian();
	in->setBigEndian(header.signature == FILE_SIGNATURE && header.version == 3);

	if (header.signature == SMALL_FILE_SIGNATURE || header.version >= 4) {
		if (header.bytesPerAttribute == 2) {
			in->read(&pixel);
		}
		else {
			pixel.character = in->readUChar();
			pixel.color = in->readUChar();

			unsigned long long attributes = (unsigned long long)pixel.attributes;
			in->read(&attributes, header.bytesPerAttribute, 1, 0);
		}
	}
	else {
		pixel.character = in->readUChar();
		pixel.color = in->readUChar();

		if (header.signature == FILE_SIGNATURE && header.version < 3) {
			unsigned char attributes = in->readUChar();
			pixel.attributes = PixelAttributes::None;
			if ((attributes & 0x01) == 0x01)
				pixel.attributes |= PixelAttributes::Char;
			if ((attributes & 0x02) == 0x02)
				pixel.attributes |= PixelAttributes::Color;
			if ((attributes & 0x04) == 0x04)
				pixel.attributes |= PixelAttributes::Invert;
		}
		else {
			unsigned long long attributes = (unsigned long long)PixelAttributes::Fill;
			if (header.bytesPerAttribute > 0 && header.bytesPerAttribute <= 8) {
				// Oh god it's so broken
				in->read(&attributes, header.bytesPerAttribute, 1, 8 - header.bytesPerAttribute);
			}
			pixel.attributes = (PixelAttributes)attributes;
		}
	}
	in->setBigEndian(oldEndian);
	return !in->hasError();
}
bool AsciiImageInfo::savePixel(OutputStream* out, const AsciiImageHeader& header, Pixel& pixel) {
	bool oldEndian = out->isBigEndian();
	out->setBigEndian(header.signature == FILE_SIGNATURE && header.version == 3);

	if (header.signature == SMALL_FILE_SIGNATURE || header.version >= 4) {
		if (header.bytesPerAttribute == 2) {
			out->write(&pixel);
		}
		else {
			out->writeUChar(pixel.character);
			out->writeUChar(pixel.color);

			unsigned long long attributes = (unsigned long long)pixel.attributes;
			out->write(&attributes, header.bytesPerAttribute, 1, 0);
		}
	}
	else {
		out->writeUChar(pixel.character);
		out->writeUChar(pixel.color);

		if (header.signature == FILE_SIGNATURE && header.version < 3) {
			unsigned char attributes = 0;
			if ((pixel.attributes & PixelAttributes::Char) != PixelAttributes::None)
				attributes |= 0x01;
			if ((pixel.attributes & PixelAttributes::Color) != PixelAttributes::None)
				attributes |= 0x02;
			if ((pixel.attributes & PixelAttributes::Invert) != PixelAttributes::None)
				attributes |= 0x04;
			out->writeUChar(attributes);
		}
		else if (header.bytesPerAttribute > 0 && header.bytesPerAttribute <= 8) {
			// Oh god it's so broken
			unsigned long long attributes = (unsigned long long)pixel.attributes;
			out->write(&attributes, header.bytesPerAttribute, 1, 8 - header.bytesPerAttribute);
		}
	}
	out->setBigEndian(oldEndian);
	return !out->hasError();
}
//--------------------------------
bool AsciiImageInfo::loadHeader(InputStream* in, AsciiImageHeader& header, bool loadFrameHeaders) {
	bool oldEndian = in->isBigEndian();

	header.fileStart = in->getPointer();

	unsigned int version = in->readUInt();
	if (version == 1 || version == 2) {
		// Old File Format
		in->setBigEndian(false);

		header.headerOffset = (unsigned int)(in->getPointer() - header.fileStart);
		header.signature = FILE_SIGNATURE;
		header.version = version;

		// Skip the old formatting
		in->readUInt();
		header.format = ImageFormats::Attributes | ImageFormats::Animation;

		header.frameCount = (unsigned short)in->readUInt();
		header.animationSpeed = in->readUInt();
		header.background = Pixel(' ', 0x00);
		if (version == 2)
			loadPixel(in, header, header.background);
		header.size.x = in->readUShort();
		header.size.y = in->readUShort();

		header.headerSize = (unsigned int)(in->getPointer() - header.fileStart - header.headerOffset);
		header.frameArrayOffset = (unsigned int)(in->getPointer() - header.fileStart);
		header.frameHeaderSize = 0;

		header.bytesPerName = 0;
		header.bytesPerAttribute = 1;

		header.frameSize = header.size.x * header.size.y * 3;
		header.fileSize = header.headerOffset + header.headerSize + header.frameCount * header.frameSize;

		/*if (header.Frames > 1) {
		header.Format |= ImageFormats::FormatAttributes;
		header.Format |= ImageFormats::FormatAnimation;
		}*/

		if (!(header.size >= Point2I::ONE && header.size <= MAX_SIZE)) {
			header.size = Point2I::ZERO;
			header.frameCount = 0;
		}
		else {

			for (int i = 0; i < header.frameCount && loadFrameHeaders; i++) {
				AsciiImageFrameHeader frame = AsciiImageFrameHeader();
				frame.frameOffset = header.frameArrayOffset + header.frameSize * i;
				in->setPointer(header.fileStart + frame.frameOffset, false);
				if (loadFrameHeader(in, header, frame)) {
					header.frameList.push_back(frame);
					//in->setPointer(header.frameSize, true);
				}
				else {
					return false;
				}
			}
		}

		in->setPointer(header.fileStart + header.frameArrayOffset, false);
	}
	else {
		// New File Format
		in->setBigEndian(true);
		in->setPointer(header.fileStart, false);

		// File Header
		header.signature = in->readString(6);
		header.version = in->readUShort();

		if (header.signature == FILE_SIGNATURE) {
			// Normal File Version
			if (header.version >= 4)
				in->setBigEndian(false);

			header.fileSize = in->readUInt();
			header.headerOffset = in->readUInt();
			header.frameArrayOffset = in->readUInt();

			in->setPointer(header.fileStart + header.headerOffset, false);

			// Header
			header.headerSize = in->readUInt();
			header.size.x = (int)in->readUShort();
			header.size.y = (int)in->readUShort();
			header.format = (ImageFormats)in->readUShort();
			header.frameCount = in->readUShort();
			header.animationSpeed = in->readUInt();
			header.frameHeaderSize = in->readUInt();
			header.bytesPerName = in->readUShort();
			header.bytesPerAttribute = in->readUShort();
			loadPixel(in, header, header.background);


			header.frameSize = header.size.x * header.size.y * (2 + header.bytesPerAttribute);

			if (!(header.size >= Point2I::ONE && header.size <= MAX_SIZE)) {
				header.size = Point2I::ZERO;
				header.frameCount = 0;
			}
			else {
				unsigned int framePos = header.frameArrayOffset;

				for (int i = 0; i < header.frameCount && loadFrameHeaders; i++) {
					AsciiImageFrameHeader frame = AsciiImageFrameHeader();
					if (i == 0) {
						frame.frameOffset = header.frameArrayOffset;
					}
					else {
						AsciiImageFrameHeader& lastFrame = header.frameList[i - 1];
						frame.frameOffset = lastFrame.frameOffset + header.frameHeaderSize +
							(lastFrame.frameSize * lastFrame.layers);
					}

					//in->setPointer(header.fileStart + framePos, false);
					in->setPointer(header.fileStart + frame.frameOffset, false);
					if (loadFrameHeader(in, header, frame)) {
						header.frameList.push_back(frame);
						//framePos += header.frameHeaderSize + header.frameSize * frame.layers;
					}
					else {
						return false;
					}
				}
			}

			in->setPointer(header.fileStart + header.frameArrayOffset, false);
		}
		else if (header.signature == SMALL_FILE_SIGNATURE) {
			// Lightweight File Version
			in->setBigEndian(false);

			header.headerOffset = (unsigned int)(in->getPointer() - header.fileStart);
			header.size.x = in->readUShort();
			header.size.y = in->readUShort();
			header.frameCount = in->readUShort();
			header.bytesPerAttribute = in->readUShort();

			header.animationSpeed = 800;
			header.bytesPerName = 0;
			header.format = ImageFormats::Basic;
			if (header.bytesPerAttribute > 0)
				header.format |= ImageFormats::Attributes;
			if (header.frameCount > 1)
				header.format |= ImageFormats::Animation;
			header.background = Pixel(' ', 0x00);

			header.headerSize = (unsigned int)(in->getPointer() - header.fileStart - header.headerOffset);
			header.frameArrayOffset = (unsigned int)(in->getPointer() - header.fileStart);
			header.frameHeaderSize = 0;

			header.frameSize = header.size.x * header.size.y * (2 + header.bytesPerAttribute);
			header.fileSize = header.headerOffset + header.headerSize + header.frameCount * header.frameSize;

			if (!(header.size >= Point2I::ONE && header.size <= MAX_SIZE)) {
				header.size = Point2I::ZERO;
				header.frameCount = 0;
			}
			else {
				for (int i = 0; i < header.frameCount && loadFrameHeaders; i++) {
					AsciiImageFrameHeader frame = AsciiImageFrameHeader();
					frame.frameOffset = header.frameArrayOffset + header.frameSize * i;
					in->setPointer(header.fileStart + frame.frameOffset, false);
					if (loadFrameHeader(in, header, frame)) {
						header.frameList.push_back(frame);
					}
					else {
						return false;
					}
				}
			}

			in->setPointer(header.fileStart + header.frameArrayOffset, false);
		}
	}
	in->setBigEndian(oldEndian);

	if (header.version < 1)
		return false;
	if (header.signature == FILE_SIGNATURE && header.version > FILE_VERSION)
		return false;
	if (header.signature == SMALL_FILE_SIGNATURE && header.version > SMALL_FILE_VERSION)
		return false;
	if (header.signature != FILE_SIGNATURE && header.signature != SMALL_FILE_SIGNATURE)
		return false;
	if (!(header.size >= Point2I::ONE && header.size <= MAX_SIZE))
		return false;
	if (header.frameCount < 1)
		return false;
	if (header.bytesPerAttribute > 8)
		return false;
	if (in->getPointer() - header.fileStart - header.headerOffset > header.headerSize)
		return false;

	return !in->hasError();
}
bool AsciiImageInfo::loadFrameHeader(InputStream* in, AsciiImageHeader& header, AsciiImageFrameHeader& frame) {
	//frame.frameOffset = (unsigned int)(in->getPointer() - header.fileStart);
	frame.frameSize = header.frameSize;
	if (header.signature == FILE_SIGNATURE && header.version >= 3) {
		frame.name = in->readString(header.bytesPerName);
		frame.size.x = (int)in->readUShort();
		frame.size.y = (int)in->readUShort();
		frame.frameSpeed = in->readUInt();
		frame.layers = in->readUShort();
		if (header.version >= 4)
			frame.frameSize = frame.size.x * frame.size.y * (2 + header.bytesPerAttribute);
	}
	else if (header.signature == SMALL_FILE_SIGNATURE && header.version >= 1) {
		frame.name = "";
		frame.size = header.size;
		frame.frameSpeed = 800;
		frame.layers = 1;
	}
	else {
		frame.name = "";
		frame.size = header.size;
		frame.frameSpeed = 800;
		frame.layers = 1;
	}

	if (!getFlag(header.format, ImageFormats::Multisize) && header.size != frame.size)
		return false;
	if (!(frame.size >= Point2I::ONE && frame.size <= MAX_SIZE))
		return false;
	if (frame.layers < 1)
		return false;

	return !in->hasError();
}
//--------------------------------
bool AsciiImageInfo::saveHeader(OutputStream* out, AsciiImageHeader& header, bool saveFrameHeaders) {
	if (header.version < 1)
		return false;
	if (header.signature == FILE_SIGNATURE && (header.version > FILE_VERSION || header.version == 1))
		return false;
	if (header.signature == SMALL_FILE_SIGNATURE && header.version > SMALL_FILE_VERSION)
		return false;
	if (header.signature != FILE_SIGNATURE && header.signature != SMALL_FILE_SIGNATURE)
		return false;
	if (!getFlag(header.format, ImageFormats::Animation) && header.frameCount > 1)
		return false;
	if (!(header.size >= Point2I::ONE && header.size <= MAX_SIZE))
		return false;
	if (header.frameCount < 1)
		return false;
	if (header.bytesPerAttribute > 8)
		return false;
	if (out->hasError())
		return false;

	bool oldEndian = out->isBigEndian();
	if (header.signature == FILE_SIGNATURE && header.version < 3) {
		// Old Version
		out->setBigEndian(false);

		out->writeUInt(header.version);
		out->writeUInt((unsigned int)header.format);
		out->writeUInt(header.frameCount);
		out->writeUInt(header.animationSpeed);
		if (header.version == 2)
			savePixel(out, header, header.background);
		out->writeUShort((unsigned short)header.size.x);
		out->writeUShort((unsigned short)header.size.y);
	}
	else if (header.signature == FILE_SIGNATURE && header.version >= 3) {
		// Normal Version
		out->setBigEndian(true);

		header.fileStart = 0;

		out->writeString(header.signature, 6);
		out->writeUShort(header.version);
		out->setBigEndian(header.version == 3);
		out->writeUInt(header.fileSize);
		out->writeUInt(header.headerOffset);
		out->writeUInt(header.frameArrayOffset);

		out->setPointer(header.fileStart + header.headerOffset);

		out->writeUInt(header.headerSize);
		out->writeUShort((unsigned short)header.size.x);
		out->writeUShort((unsigned short)header.size.y);
		out->writeUShort((unsigned short)header.format);
		out->writeUShort(header.frameCount);
		out->writeUInt(header.animationSpeed);
		out->writeUInt(header.frameHeaderSize);
		out->writeUShort(header.bytesPerName);
		out->writeUShort(header.bytesPerAttribute);
		savePixel(out, header, header.background);

		for (int i = 0; i < header.frameCount && saveFrameHeaders; i++) {
			out->setPointer(header.fileStart + header.frameList[i].frameOffset, false);

			if (!saveFrameHeader(out, header, header.frameList[i])) {
				return false;
			}
		}

		out->setPointer(header.fileStart + header.frameArrayOffset, false);
	}
	else if (header.signature == SMALL_FILE_SIGNATURE) {
		// Lightweight Version
		out->setBigEndian(true);

		out->writeString(header.signature, 6);
		out->writeUShort(header.version);
		out->setBigEndian(false);

		out->writeUShort((unsigned short)header.size.x);
		out->writeUShort((unsigned short)header.size.y);
		out->writeUShort(header.frameCount);
		out->writeUShort(header.bytesPerAttribute);
	}
	out->setBigEndian(oldEndian);

	return !out->hasError();
}
bool AsciiImageInfo::saveFrameHeader(OutputStream* out, AsciiImageHeader& header, AsciiImageFrameHeader& frame) {
	if (!getFlag(header.format, ImageFormats::Multisize) && header.size != frame.size)
		return false;
	if (!(frame.size >= Point2I::ONE && frame.size <= MAX_SIZE))
		return false;
	if (frame.layers < 1)
		return false;

	if (header.signature == FILE_SIGNATURE) {
		out->writeString(frame.name, header.bytesPerName);
		out->writeUShort((unsigned short)frame.size.x);
		out->writeUShort((unsigned short)frame.size.y);
		out->writeUInt(frame.frameSpeed);
		out->writeUShort(frame.layers);
	}
	return !out->hasError();
}

//=================================================================|
