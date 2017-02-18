//================================================================
// ASCII IMAGE BASE INFO CPP:
//================================================================

#include "Stdafx.h"
#include "AsciiImage.h"

using namespace std;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::IOStream;

//================================================================
// DEFINITIONS:
//================================================================
/* Returns true if the flag is applied to the attributes. */
#define GetFlag(attributes, flag)	(((attributes) & (flag)) == (flag))

/* Gets the foreground color of the specified color. */
#define	GetFColor(color)	((color) & 0x0F)
/* Gets the background color of the specified color. */
#define	GetBColor(color)	(((color) & 0xF0) >> 4)
/* Swaps the foreground and background colors of the specified color. */
#define	SwapColor(color)	((((color) & 0x0F) << 4) | (((color) & 0xF0) >> 4))

/* Sets the foreground color of the specified color. */
#define	SetFColor(color, fcolor)	(((color) & 0xF0) | ((fcolor) & 0x0F))
/* Sets the background color of the specified color. */
#define	SetBColor(color, bcolor)	(((color) & 0x0F) | ((bcolor) & 0xF0))

/* Inverts the specified color. */
#define InvertColor(color)			((unsigned char)0xFF - (color))
/* Inverts the foreground color of the specified color. */
#define InvertFColor(color)			SetFColor(color, (unsigned char)0x0F - ((color) & 0x0F))
/* Inverts the background color of the specified color. */
#define InvertBColor(color)			SetBColor(color, (unsigned char)0xF0 - ((color) & 0xF0))

/* Lightens the specified color. */
#define LightenColor(color)			((color) | 0x88)
/* Lightens the foreground color of the specified color. */
#define LightenFColor(color)		SetFColor(color, (color) | 0x08)
/* Lightens the background color of the specified color. */
#define LightenBColor(color)		SetBColor(color, (color) | 0x80)

/* Darkens the specified color. */
#define DarkenColor(color)			((color) & 0x77)
/* Darkens the foreground color of the specified color. */
#define DarkenFColor(color)			SetFColor(color, (color) & 0x07)
/* Darkens the background color of the specified color. */
#define DarkenBColor(color)			SetBColor(color, (color) & 0x70)

/* Fixes the point and size so the size is always positive. */
#define FixRect(point, size)		(point += Coord(size.X < 0 ? size.X + 1 : 0, size.Y < 0 ? size.Y + 1 : 0)); \
									(size *= Coord(size.X < 0 ? -1 : 1, size.Y < 0 ? -1 : 1))

//================================================================
// CLASSES:
//================================================================
// AsciiImageInfo:
//----------------------------------------------------------------
//========== CONSTANTS ===========
const unsigned short PowerConsole::Drawing::AsciiImageInfo::FileVersion			= 3;
const unsigned short PowerConsole::Drawing::AsciiImageInfo::SmallFileVersion	= 1;
const string PowerConsole::Drawing::AsciiImageInfo::FileSignature				= "ASCIMG";
const string PowerConsole::Drawing::AsciiImageInfo::SmallFileSignature			= "ASCSML";
const Coord PowerConsole::Drawing::AsciiImageInfo::MaxSize						= Coord(1024, 1024);
const unsigned int PowerConsole::Drawing::AsciiImageInfo::MaxFrames				= 2048;
const string PowerConsole::Drawing::AsciiImageInfo::ClipboardFormatName			= "CF_ASCII_IMAGE";
const unsigned int PowerConsole::Drawing::AsciiImageInfo::ClipboardFormat		= RegisterClipboardFormatA("CF_ASCII_IMAGE");
//========== MANAGEMENT ==========
AsciiImageHeader PowerConsole::Drawing::AsciiImageInfo::CreateHeader(const AsciiImage& image, unsigned short version, bool smallFile) {
	AsciiImageHeader header = AsciiImageHeader();
	AsciiImageFrameHeader frame = AsciiImageFrameHeader();
	header.Signature		= (smallFile ? SmallFileSignature : FileSignature);
	header.Version			= version;
	
	header.BytesPerName		= 0;
	header.BytesPerAttribute	= (GetFlag(image._Format, FormatAttributes) ? 2 : 0);
	if (!smallFile && version < 3)
		header.BytesPerAttribute	= 1;

	header.HeaderOffset		= 20;
	if (!smallFile && version < 3)
		header.HeaderOffset		= 0;
	header.HeaderSize		= 24 + (2 + header.BytesPerAttribute);
	if (!smallFile && version < 3)
		header.HeaderSize	= 23;
	header.FrameArrayOffset	= header.HeaderOffset + header.HeaderSize;
	header.FrameHeaderSize	= 10 + header.BytesPerName;
	if (!smallFile && version < 3)
		header.FrameHeaderSize	= 0;

	header.Size				= image._Size;
	header.Format			= image._Format;
	header.Frames			= 1;
	header.AnimationSpeed	= 1;
	header.Background		= image._Background;
	
	header.FrameSize		= image._Size.X * image._Size.Y * (2 + header.BytesPerAttribute);
	header.FileSize			= header.FrameArrayOffset + header.FrameHeaderSize + header.FrameSize;
	
	frame.FrameOffset		= header.FrameArrayOffset;
	frame.Name				= "";
	frame.Size				= image._Size;
	frame.FrameSpeed		= image._FrameSpeed;
	frame.Layers			= 1;
	header.FrameList.push_back(frame);

	return header;
}
AsciiImageHeader PowerConsole::Drawing::AsciiImageInfo::CreateHeader(const AsciiAnimation& image, unsigned short version, bool smallFile) {
	AsciiImageHeader header = AsciiImageHeader();
	AsciiImageFrameHeader frame = AsciiImageFrameHeader();
	header.Signature		= (smallFile ? SmallFileSignature : FileSignature);
	header.Version			= version;
	
	header.BytesPerName		= 0;
	header.BytesPerAttribute	= (GetFlag(image.GetFormat(), FormatAttributes) ? 2 : 0);
	if (!smallFile && version < 3)
		header.BytesPerAttribute	= 1;

	header.HeaderOffset		= 20;
	if (!smallFile && version < 3)
		header.HeaderOffset		= 0;
	header.HeaderSize		= 24 + (2 + header.BytesPerAttribute);
	if (!smallFile && version < 3)
		header.HeaderSize	= 23;
	header.FrameArrayOffset	= header.HeaderOffset + header.HeaderSize;
	header.FrameHeaderSize	= 10 + header.BytesPerName;
	if (!smallFile && version < 3)
		header.FrameHeaderSize	= 0;

	header.Size				= image.Size();
	header.Format			= image.GetFormat();
	header.Frames			= image.NumFrames();
	header.AnimationSpeed	= image.GetAnimationSpeed();
	header.Background		= image.GetBackground();
	
	header.FrameSize		= image.Size().X * image.Size().Y * (2 + header.BytesPerAttribute);
	header.FileSize			= header.FrameArrayOffset + (header.FrameHeaderSize + header.FrameSize) * header.Frames;

	for (int i = 0; i < (int)image.NumFrames(); i++) {
		frame.FrameOffset		= header.FrameArrayOffset + (header.FrameHeaderSize + header.FrameSize) * i;
		frame.Name				= "";
		frame.Size				= image._Frames[i].Size();
		frame.FrameSpeed		= image._Frames[i].GetFrameSpeed();
		frame.Layers			= 1;
		header.FrameList.push_back(frame);
	}

	return header;
}
//--------------------------------
bool PowerConsole::Drawing::AsciiImageInfo::IsFileAsciiImage(const string& path) {
	AsciiImageHeader header = AsciiImageHeader();
	FileInputStream in = FileInputStream();
	if (in.Open(path)) {
		bool success = LoadHeader(&in, header, false);
		in.Close();
		return success;
	}
	return false;
}
//--------------------------------
bool PowerConsole::Drawing::AsciiImageInfo::LoadPixel(InputStream* in, const AsciiImageHeader& header, Pixel& pixel) {
	bool oldEndian = in->IsLittleEndian();
	in->SetLittleEndian(header.Signature == FileSignature && header.Version < 3);

	pixel.Char	= in->ReadUChar();
	pixel.Color	= in->ReadUChar();

	if (header.Signature == FileSignature && header.Version < 3) {
		unsigned char attributes	= in->ReadUChar();
		pixel.Attributes	= AttributeNone;
		if ((attributes & 0x01) == 0x01)
			pixel.Attributes	|= AttributeChar;
		if ((attributes & 0x02) == 0x02)
			pixel.Attributes	|= AttributeColor;
		if ((attributes & 0x04) == 0x04)
			pixel.Attributes	|= AttributeInvert;
	}
	else {
		unsigned long long attributes	= AttributeFill;
		if (header.BytesPerAttribute > 0 && header.BytesPerAttribute <= 8)
			in->Read(&attributes, header.BytesPerAttribute, 1, 8 - header.BytesPerAttribute);
		pixel.Attributes	= (unsigned short)attributes;
	}
	in->SetLittleEndian(oldEndian);
	return !in->Error();
}
bool PowerConsole::Drawing::AsciiImageInfo::SavePixel(OutputStream* out, const AsciiImageHeader& header, Pixel& pixel) {
	bool oldEndian = out->IsLittleEndian();
	out->SetLittleEndian(header.Signature == FileSignature && header.Version < 3);

	out->WriteUChar(pixel.Char);
	out->WriteUChar(pixel.Color);

	if (header.Signature == FileSignature && header.Version < 3) {
		unsigned char attributes = 0;
		if ((pixel.Attributes & AttributeChar) != 0)
			attributes |= 0x01;
		if ((pixel.Attributes & AttributeColor) != 0)
			attributes |= 0x02;
		if ((pixel.Attributes & AttributeInvert) != 0)
			attributes |= 0x04;
		out->WriteUChar(attributes);
	}
	else if (header.BytesPerAttribute > 0 && header.BytesPerAttribute <= 8) {
		unsigned long long attributes = pixel.Attributes;
		out->Write(&attributes, header.BytesPerAttribute, 1, 8 - header.Background.Attributes);
	}
	out->SetLittleEndian(oldEndian);
	return !out->Error();
}
//--------------------------------
bool PowerConsole::Drawing::AsciiImageInfo::LoadHeader(InputStream* in, AsciiImageHeader& header, bool loadFrameHeaders) {
	bool oldEndian = in->IsLittleEndian();

	header.FileStart		= in->GetPointer();

	unsigned int version	= in->ReadUInt();
	if (version == 1 || version == 2) {
		// Old File Format
		in->SetLittleEndian(true);
		
		header.HeaderOffset		= (unsigned int)(in->GetPointer() - header.FileStart);
		header.Signature		= FileSignature;
		header.Version			= version;
		
		// Skip the old formatting
		in->ReadUInt();
		header.Format			= (unsigned short)(ImageFormats::FormatAttributes | ImageFormats::FormatAnimation);

		header.Frames			= (unsigned short)in->ReadUInt();
		header.AnimationSpeed	= in->ReadUInt();
		header.Background		= Pixel();
		if (version == 2)
			LoadPixel(in, header, header.Background);
		header.Size.X			= in->ReadUShort();
		header.Size.Y			= in->ReadUShort();

		header.HeaderSize		= (unsigned int)(in->GetPointer() - header.FileStart - header.HeaderOffset);
		header.FrameArrayOffset	= (unsigned int)(in->GetPointer() - header.FileStart);
		header.FrameHeaderSize	= 0;

		header.BytesPerName		= 0;
		header.BytesPerAttribute	= 1;
		
		header.FrameSize		= header.Size.X * header.Size.Y * 3;
		header.FileSize			= header.HeaderOffset + header.HeaderSize + header.Frames * header.FrameSize;
		
		/*if (header.Frames > 1) {
			header.Format |= ImageFormats::FormatAttributes;
			header.Format |= ImageFormats::FormatAnimation;
		}*/

		if (!(header.Size >= Coord::One && header.Size <= MaxSize)) {
			header.Size = Coord::Zero;
			header.Frames = 0;
		}
		else {

			for (int i = 0; i < header.Frames && loadFrameHeaders; i++) {
				AsciiImageFrameHeader frame = AsciiImageFrameHeader();
				LoadFrameHeader(in, header, frame);
				header.FrameList.push_back(frame);
				in->SetPointer(header.FrameSize, true);
			}
		}

		
		in->SetPointer(header.FileStart + header.FrameArrayOffset, false);
	}
	else {
		// New File Format
		in->SetLittleEndian(false);
		in->SetPointer(header.FileStart, false);

		// File Header
		header.Signature		= in->ReadString(6);
		header.Version			= in->ReadUShort();

		if (header.Signature == FileSignature) {
			// Normal File Version

			header.FileSize			= in->ReadUInt();
			header.HeaderOffset		= in->ReadUInt();
			header.FrameArrayOffset	= in->ReadUInt();

			in->SetPointer(header.FileStart + header.HeaderOffset, false);
			
			// Header
			header.HeaderSize		= in->ReadUInt();
			header.Size.X			= (int)in->ReadUShort();
			header.Size.Y			= (int)in->ReadUShort();
			header.Format			= in->ReadUShort();
			header.Frames			= in->ReadUShort();
			header.AnimationSpeed	= in->ReadUInt();
			header.FrameHeaderSize	= in->ReadUInt();
			header.BytesPerName		= in->ReadUShort();
			header.BytesPerAttribute	= in->ReadUShort();
			LoadPixel(in, header, header.Background);
			
			
			header.FrameSize		= header.Size.X * header.Size.Y * (2 + header.BytesPerAttribute);
			
			if (!(header.Size >= Coord::One && header.Size <= MaxSize)) {
				header.Size = Coord::Zero;
				header.Frames = 0;
			}
			else {
				unsigned int framePos	= header.FrameArrayOffset;

				for (int i = 0; i < header.Frames && loadFrameHeaders; i++) {
					in->SetPointer(header.FileStart + framePos, false);

					AsciiImageFrameHeader frame = AsciiImageFrameHeader();
					if (LoadFrameHeader(in, header, frame)) {
						header.FrameList.push_back(frame);

						framePos += header.FrameHeaderSize + header.FrameSize * frame.Layers;
					}
					else {
						return false;
					}
				}
			}

			in->SetPointer(header.FileStart + header.FrameArrayOffset, false);
		}
		else if (header.Signature == SmallFileSignature) {
			// Lightweight File Version

			header.HeaderOffset		= (unsigned int)(in->GetPointer() - header.FileStart);
			header.Size.X			= in->ReadUShort();
			header.Size.Y			= in->ReadUShort();
			header.Frames			= in->ReadUShort();
			header.BytesPerAttribute	= in->ReadUShort();
			
			header.AnimationSpeed	= 1;
			header.BytesPerName		= 0;
			header.Format			= FormatBasic;
			if (header.BytesPerAttribute > 0)
				header.Format		= FormatAttributes;
			if (header.Frames > 1)
				header.Format		|= FormatAnimation;
			header.Background		= Pixel();
			
			header.HeaderSize		= (unsigned int)(in->GetPointer() - header.FileStart - header.HeaderOffset);
			header.FrameArrayOffset	= (unsigned int)(in->GetPointer() - header.FileStart);
			header.FrameHeaderSize	= 0;

			header.FrameSize		= header.Size.X * header.Size.Y * (2 + header.BytesPerAttribute);
			header.FileSize			= header.HeaderOffset + header.HeaderSize + header.Frames * header.FrameSize;

			if (!(header.Size >= Coord::One && header.Size <= MaxSize)) {
				header.Size = Coord::Zero;
				header.Frames = 0;
			}
			else {
				for (int i = 0; i < header.Frames && loadFrameHeaders; i++) {
					AsciiImageFrameHeader frame = AsciiImageFrameHeader();
					if (LoadFrameHeader(in, header, frame)) {
						header.FrameList.push_back(frame);
						in->SetPointer(header.FrameSize, true);
					}
					else {
						return false;
					}
				}
			}
		
			in->SetPointer(header.FileStart + header.FrameArrayOffset, false);
		}
	}
	in->SetLittleEndian(oldEndian);

	if (header.Version < 1)
		return false;
	if (header.Signature == FileSignature && header.Version > FileVersion)
		return false;
	if (header.Signature == SmallFileSignature && header.Version > SmallFileVersion)
		return false;
	if (header.Signature != FileSignature && header.Signature != SmallFileSignature)
		return false;
	if (!(header.Size >= Coord::One && header.Size <= MaxSize))
		return false;
	if (header.Frames < 1)
		return false;
	if (header.BytesPerAttribute > 8)
		return false;
	if (in->GetPointer() - header.FileStart - header.HeaderOffset > header.HeaderSize)
		return false;
	if (in->Error())
		return false;

	return true;
}
bool PowerConsole::Drawing::AsciiImageInfo::LoadFrameHeader(InputStream* in, AsciiImageHeader& header, AsciiImageFrameHeader& frame) {
	if (header.Signature == FileSignature && header.Version >= 3) {
		frame.FrameOffset	= (unsigned int)(in->GetPointer() - header.FileStart);
		frame.Name			= in->ReadString(header.BytesPerName);
		frame.Size.X		= (int)in->ReadUShort();
		frame.Size.Y		= (int)in->ReadUShort();
		frame.FrameSpeed	= in->ReadUInt();
		frame.Layers		= in->ReadUShort();
	}
	else {
		frame.FrameOffset	= (unsigned int)(in->GetPointer() - header.FileStart);
		frame.Name			= "";
		frame.Size			= header.Size;
		frame.FrameSpeed	= 1;
		frame.Layers		= 1;
	}

	if (!GetFlag(header.Format, FormatMultiSize) && header.Size != frame.Size)
		return false;
	if (!(frame.Size >= Coord(1, 1) && frame.Size <= MaxSize))
		return false;
	if (frame.Layers < 1)
		return false;

	return !in->Error();
}
//--------------------------------
bool PowerConsole::Drawing::AsciiImageInfo::SaveHeader(OutputStream* out, AsciiImageHeader& header, bool saveFrameHeaders) {
	if (header.Version < 1)
		return false;
	if (header.Signature == FileSignature && header.Version > FileVersion)
		return false;
	if (header.Signature == SmallFileSignature && header.Version > SmallFileVersion)
		return false;
	if (header.Signature != FileSignature && header.Signature != SmallFileSignature)
		return false;
	if (!GetFlag(header.Format, FormatAnimation) && header.Frames > 1)
		return false;
	if (!(header.Size >= Coord::One && header.Size <= MaxSize))
		return false;
	if (header.Frames < 1)
		return false;
	if (header.BytesPerAttribute > 8)
		return false;
	if (out->Error())
		return false;

	bool oldEndian = out->IsLittleEndian();
	if (header.Signature == FileSignature && header.Version < 3) {
		// Old Version
		out->SetLittleEndian(true);

		out->WriteUInt(header.Version);
		out->WriteUInt(header.Format);
		out->WriteUInt(header.Frames);
		out->WriteUInt(header.AnimationSpeed);
		if (header.Version == 2)
			SavePixel(out, header, header.Background);
		out->WriteUShort((unsigned short)header.Size.X);
		out->WriteUShort((unsigned short)header.Size.Y);
	}
	else if (header.Signature == FileSignature && header.Version >= 3) {
		// Normal Version
		out->SetLittleEndian(false);
		
		header.FileStart = 0;

		out->WriteString(header.Signature, 6);
		out->WriteUShort(header.Version);
		out->WriteUInt(header.FileSize);
		out->WriteUInt(header.HeaderOffset);
		out->WriteUInt(header.FrameArrayOffset);

		out->SetPointer(header.FileStart + header.HeaderOffset);

		out->WriteUInt(header.HeaderSize);
		out->WriteUShort((unsigned short)header.Size.X);
		out->WriteUShort((unsigned short)header.Size.Y);
		out->WriteUShort(header.Format);
		out->WriteUShort(header.Frames);
		out->WriteUInt(header.AnimationSpeed);
		out->WriteUInt(header.FrameHeaderSize);
		out->WriteUShort(header.BytesPerName);
		out->WriteUShort(header.BytesPerAttribute);
		SavePixel(out, header, header.Background);

		for (int i = 0; i < header.Frames && saveFrameHeaders; i++) {
			out->SetPointer(header.FileStart + header.FrameArrayOffset +
				(header.FrameHeaderSize + header.FrameSize * header.FrameList[i].Layers) * i);

			if (!SaveFrameHeader(out, header, header.FrameList[i])) {
				return false;
			}
		}

		out->SetPointer(header.FileStart + header.FrameArrayOffset, false);
	}
	else if (header.Signature == SmallFileSignature) {
		// Lightweight Version
		out->SetLittleEndian(false);
		
		out->WriteString(header.Signature, 6);
		out->WriteUShort(header.Version);

		out->WriteUShort((unsigned short)header.Size.X);
		out->WriteUShort((unsigned short)header.Size.Y);
		out->WriteUShort(header.Frames);
		out->WriteUShort(header.BytesPerAttribute);
	}
	out->SetLittleEndian(oldEndian);

	return !out->Error();
}
bool PowerConsole::Drawing::AsciiImageInfo::SaveFrameHeader(OutputStream* out, AsciiImageHeader& header, AsciiImageFrameHeader& frame) {
	if (!GetFlag(header.Format, FormatMultiSize) && header.Size != frame.Size)
		return false;
	if (!(frame.Size >= Coord::One && frame.Size <= MaxSize))
		return false;
	if (frame.Layers < 1)
		return false;

	if (header.Signature == FileSignature && header.Version >= 3) {
		out->WriteUShort((unsigned short)frame.Size.X);
		out->WriteUShort((unsigned short)frame.Size.Y);
		out->WriteUInt(frame.FrameSpeed);
		out->WriteUShort(frame.Layers);
	}
	return !out->Error();
}
//========= INFORMATION ==========
PowerConsole::Drawing::AsciiImageHeader::AsciiImageHeader() {
	// File Information
	this->Signature			= "";
	this->Version			= 3;
	this->FileSize			= 0;
	this->FileStart			= 0;
	
	// Header Information
	this->HeaderSize		= 0;
	this->HeaderOffset		= 0;
	this->FrameHeaderSize	= 0;
	this->FrameArrayOffset	= 0;
	this->FrameSize			= 0;

	// Data Sizes
	this->BytesPerAttribute	= 2;
	this->BytesPerName		= 0;

	// Image Information
	this->Format			= ImageFormats::FormatBasic;
	this->Frames			= 1;
	this->Size				= Coord::One;
	this->Background		= Pixel();
	this->AnimationSpeed	= 800;
}
PowerConsole::Drawing::AsciiImageFrameHeader::AsciiImageFrameHeader() {
	// File Information
	this->FrameOffset		= 0;

	// Frame Information
	this->Name				= "";
	this->Size				= Coord::One;
	this->FrameSpeed		= 1;
	this->Layers			= 1;
}
//================================================================
// END ASCII IMAGE INFO CPP:
//================================================================