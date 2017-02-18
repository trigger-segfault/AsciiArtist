//================================================================
// ASCII IMAGE BASE CPP:
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
// AsciiImage:
//----------------------------------------------------------------
//========== CONSTANTS ===========

//========= CONSTRUCTORS =========
PowerConsole::Drawing::AsciiImage::AsciiImage() : AsciiImageBase() {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(Coord size, unsigned short format) : AsciiImageBase(size, format) {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(Coord size, unsigned short format, Pixel background) : AsciiImageBase(size, format, background) {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(unsigned short format, Pixel background, const AsciiImageBase& image) : AsciiImageBase(format, background, image) {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(unsigned short format, Pixel background, AsciiImageBase&& image) : AsciiImageBase(format, background, move(image)) {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(Coord part, Coord partSize, unsigned short format, Pixel background, const AsciiImageBase& image) : AsciiImageBase(part, partSize, format, background, image) {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(const AsciiImageBase& image) : AsciiImageBase(image) {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(AsciiImageBase&& image) : AsciiImageBase(move(image)) {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(const AsciiImage& image) : AsciiImageBase(image) {

}
PowerConsole::Drawing::AsciiImage::AsciiImage(AsciiImage&& image) : AsciiImageBase(move(image)) {

}
PowerConsole::Drawing::AsciiImage::~AsciiImage() {

}
//========== OPERATORS ===========
AsciiImage& PowerConsole::Drawing::AsciiImage::operator=(const AsciiImageBase& image) {
	/*if (this != &image) {
		_Delete();
	
		this->_Size			= image._Size;
		this->_Format		= image._Format;
		this->_Background	= image._Background;
		this->_FrameSpeed	= image._FrameSpeed;

		this->_Pixels		= new Pixel*[this->_Size.X];
		for (int i = 0; i < this->_Size.X; i++) {
			this->_Pixels[i]	= new Pixel[this->_Size.Y];
			for (int j = 0; j < this->_Size.Y; j++) {
				this->_Pixels[i][j]	= image._Pixels[i][j];
			}
		}
	}*/
	if (this != &image) {
		AsciiImageBase::operator=(image);
	}
	return *this;
}
AsciiImage& PowerConsole::Drawing::AsciiImage::operator=(AsciiImageBase&& image) {
	/*if (this != &image) {
		_Delete();
	
		this->_Size			= image._Size;
		this->_Format		= image._Format;
		this->_Background	= image._Background;
		this->_FrameSpeed	= image._FrameSpeed;
	
		this->_Pixels		= image._Pixels;
		image._Pixels		= nullptr;
	}*/
	if (this != &image) {
		AsciiImageBase::operator=(move(image));
	}
	return *this;
}
AsciiImage& PowerConsole::Drawing::AsciiImage::operator=(const AsciiImage& image) {
	if (this != &image) {
		_Delete();
	
		this->_Size			= image._Size;
		this->_Format		= image._Format;
		this->_Background	= image._Background;
		this->_FrameSpeed	= image._FrameSpeed;

		this->_Pixels		= new Pixel*[this->_Size.X];
		for (int i = 0; i < this->_Size.X; i++) {
			this->_Pixels[i]	= new Pixel[this->_Size.Y];
			for (int j = 0; j < this->_Size.Y; j++) {
				this->_Pixels[i][j]	= image._Pixels[i][j];
			}
		}
	}
	/*if (this != &image) {
		AsciiImageBase::operator=(image);
	}*/
	return *this;
}
AsciiImage& PowerConsole::Drawing::AsciiImage::operator=(AsciiImage&& image) {
	if (this != &image) {
		_Delete();
	
		this->_Size			= image._Size;
		this->_Format		= image._Format;
		this->_Background	= image._Background;
		this->_FrameSpeed	= image._FrameSpeed;
	
		this->_Pixels		= image._Pixels;
		image._Pixels		= nullptr;
	}
	/*if (this != &image) {
		AsciiImageBase::operator=(move(image));
	}*/
	return *this;
}
//========== MANAGEMENT ==========
bool PowerConsole::Drawing::AsciiImage::Load(InputStream* in) {
	AsciiImageHeader header = AsciiImageHeader();
	if (AsciiImageInfo::LoadHeader(in, header, true)) {
		in->SetPointer(header.FileStart + header.FrameArrayOffset + header.FrameHeaderSize, false);
		bool success = _Load(in, header, header.FrameList[0]);
		if (!success) {
			*this = AsciiImage();
		}
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImage::LoadFile(const string& path) {
	FileInputStream in = FileInputStream();
	if (in.Open(path)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImage::LoadResource(const string& name, const string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.Open(name, type)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImage::LoadResource(const string& name, unsigned int typeId) {
	ResourceInputStream in = ResourceInputStream();
	if (in.Open(name, typeId)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImage::LoadResource(unsigned int id, const string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.Open(id, type)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImage::LoadResource(unsigned int id, unsigned int typeId) {
	ResourceInputStream in = ResourceInputStream();
	if (in.Open(id, typeId)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImage::LoadClipboard() {
	ClipboardInputStream in = ClipboardInputStream();
	if (in.IsFormatAvailable(AsciiImageInfo::ClipboardFormat)) {
		if (in.Open()) {
			bool success = false;
			if (in.GetData(AsciiImageInfo::ClipboardFormat)) {
				success = Load(&in);
			}
			in.Close();
			return success;
		}
	}
	else if (in.IsFileFormatAvailable()) {
		if (in.Open()) {
			vector<string> files = in.ReadFileNames();
			in.Close();
			if (!files.empty()) {
				return LoadFile(files[0]) && !in.Error();
			}
		}
	}
	return false;
}
//--------------------------------
bool PowerConsole::Drawing::AsciiImage::Save(OutputStream* out, unsigned short version, bool smallFile) {
	AsciiImageHeader header = AsciiImageInfo::CreateHeader(*this, version, smallFile);

	if (AsciiImageInfo::SaveHeader(out, header, true)) {
		out->SetPointer(header.FileStart + header.FrameArrayOffset + header.FrameHeaderSize, false);
		return _Save(out, header, header.FrameList[0]);
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImage::SaveFile(const string& path, unsigned short version, bool smallFile) {
	FileOutputStream out = FileOutputStream();
	if (out.Open(path)) {
		bool success = Save(&out, version, smallFile);
		out.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImage::SaveClipboard(unsigned short version, bool smallFile) {
	ClipboardOutputStream out = ClipboardOutputStream();
	if (out.Open()) {
		bool success = false;
		if (Save(&out, version, smallFile)) {
			success = out.SetData(AsciiImageInfo::ClipboardFormat);
		}
		out.Close();
		return success;
	}
	return false;
}
//=========== DRAWING ============
void PowerConsole::Drawing::AsciiImage::Resize(Coord size) {
	_Resize(size);
}
void PowerConsole::Drawing::AsciiImage::RotateAndResize(int rotations) {
	Coord oldSize = _Size;
	_Resize(Coord(max(_Size.X, _Size.Y), max(_Size.X, _Size.Y)));
	rotations = rotations % 4 + (rotations < 0 ? 4 : 0);
	Rotate(rotations);
	if (rotations == 1 && oldSize.Y < oldSize.X) {
		Translate(Coord(oldSize.X - oldSize.Y, 0));
		_Resize(oldSize);
	}
	else if (rotations == 2 && oldSize.X < oldSize.Y) {
		Translate(Coord(oldSize.Y - oldSize.X, 0));
		_Resize(oldSize);
	}
	else if (rotations == 2 && oldSize.Y < oldSize.X) {
		Translate(Coord(0, oldSize.X - oldSize.Y));
		_Resize(oldSize);
	}
	else if (rotations == 3 && oldSize.X < oldSize.Y) {
		Translate(Coord(0, oldSize.Y - oldSize.X));
		_Resize(oldSize);
	}
}
//========= INFORMATION ==========
void PowerConsole::Drawing::AsciiImage::SetBackground(Pixel background) {
	_Background = background;
}
void PowerConsole::Drawing::AsciiImage::SetFormat(unsigned short format) {
	_SetFormat(format);
}
void PowerConsole::Drawing::AsciiImage::SetFrameSpeed(unsigned int frameSpeed) {
	_FrameSpeed = frameSpeed;
}
//----------------------------------------------------------------
// AsciiAnimation:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation() {
	this->_AnimationSpeed	= 800;
	this->_Frames.push_back(AsciiImage(Coord(1, 1), FormatAnimation));
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(Coord size, unsigned short format, unsigned int frames) {
	this->_AnimationSpeed	= 800;
	this->_Frames.push_back(AsciiImage(size, format));

	for (int i = 1; i < (int)frames; i++) {
		this->_Frames.push_back(AsciiImage(size, format));
	}
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(Coord size, unsigned short format, Pixel background, unsigned int frames) {
	this->_AnimationSpeed	= 800;
	this->_Frames.push_back(AsciiImage(size, format, background));

	for (int i = 1; i < (int)frames; i++) {
		this->_Frames.push_back(AsciiImage(size, format, background));
	}
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(unsigned short format, Pixel background, const AsciiImageBase& image) {
	this->_AnimationSpeed	= 800;
	this->_Frames.push_back(AsciiImage(format, background, image));
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(unsigned short format, Pixel background, AsciiImageBase&& image) {
	this->_AnimationSpeed	= 800;
	this->_Frames.push_back(AsciiImage(format, background, move(image)));
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(Coord part, Coord partSize, unsigned short format, Pixel background, const AsciiImageBase& image) {
	this->_AnimationSpeed	= 800;
	this->_Frames.push_back(AsciiImage(part, partSize, format, background, image));
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(unsigned short format, Pixel background, const AsciiAnimation& image) {
	this->_AnimationSpeed	= image._AnimationSpeed;
	
	for (int i = 0; i < (int)image._Frames.size(); i++) {
		this->_Frames.push_back(AsciiImage(format, background, image._Frames[i]));
	}
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(unsigned short format, Pixel background, AsciiAnimation&& image) {
	this->_AnimationSpeed	= image._AnimationSpeed;

	for (int i = 0; i < (int)image._Frames.size(); i++) {
		this->_Frames.push_back(AsciiImage(format, background, move(image._Frames[i])));
	}
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(Coord part, Coord partSize, unsigned short format, Pixel background, const AsciiAnimation& image) {
	this->_AnimationSpeed	= image._AnimationSpeed;

	for (int i = 0; i < (int)image._Frames.size(); i++) {
		this->_Frames.push_back(AsciiImage(part, partSize, format, background, image._Frames[i]));
	}
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(const AsciiImageBase& image) {
	this->_AnimationSpeed	= 800;
	this->_Frames.push_back(AsciiImage(image._Format, image._Background, image));
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(AsciiImageBase&& image) {
	this->_AnimationSpeed	= 800;
	this->_Frames.push_back(AsciiImage(image._Format, image._Background, move(image)));
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(const AsciiAnimation& image) {
	this->_AnimationSpeed	= image._AnimationSpeed;

	for (int i = 0; i < (int)image._Frames.size(); i++) {
		this->_Frames.push_back(AsciiImage(image._Frames[i]._Format, image._Frames[i]._Background, image._Frames[i]));
	}
}
PowerConsole::Drawing::AsciiAnimation::AsciiAnimation(AsciiAnimation&& image) {
	this->_AnimationSpeed	= image._AnimationSpeed;

	for (int i = 0; i < (int)image._Frames.size(); i++) {
		this->_Frames.push_back(AsciiImage(image._Frames[i]._Format, image._Frames[i]._Background, move(image._Frames[i])));
	}
}
PowerConsole::Drawing::AsciiAnimation::~AsciiAnimation() {

}
//========== OPERATORS ===========
AsciiAnimation& PowerConsole::Drawing::AsciiAnimation::operator=(const AsciiImageBase& image) {
	this->_AnimationSpeed	= 800;
	this->_Frames.clear();
	this->_Frames.push_back(AsciiImage(image));
	return *this;
}
AsciiAnimation& PowerConsole::Drawing::AsciiAnimation::operator=(AsciiImageBase&& image) {
	this->_AnimationSpeed	= 800;
	this->_Frames.clear();
	this->_Frames.push_back(AsciiImage(move(image)));
	return *this;
}
AsciiAnimation& PowerConsole::Drawing::AsciiAnimation::operator=(const AsciiAnimation& image) {
	this->_AnimationSpeed	= image._AnimationSpeed;
	this->_Frames.clear();

	for (int i = 0; i < (int)image._Frames.size(); i++) {
		this->_Frames.push_back(image._Frames[i]);
	}
	return *this;
}
AsciiAnimation& PowerConsole::Drawing::AsciiAnimation::operator=(AsciiAnimation&& image) {
	this->_AnimationSpeed	= image._AnimationSpeed;
	this->_Frames.clear();

	for (int i = 0; i < (int)image._Frames.size(); i++) {
		this->_Frames.push_back(move(image._Frames[i]));
	}
	return *this;
}
//--------------------------------
AsciiImageBase& PowerConsole::Drawing::AsciiAnimation::operator[](int index) const {
	return (AsciiImageBase&)_Frames[index];
}
//========== MANAGEMENT ==========
bool PowerConsole::Drawing::AsciiAnimation::Load(InputStream* in) {
	AsciiImageHeader header = AsciiImageHeader();
	if (AsciiImageInfo::LoadHeader(in, header, true)) {
		bool success = true;
		_Frames.clear();
		_AnimationSpeed = header.AnimationSpeed;
		for (int i = 0; i < (int)header.Frames && success; i++) {
			in->SetPointer(header.FileStart + header.FrameArrayOffset + header.FrameHeaderSize +
				(header.FrameHeaderSize + header.FrameSize * header.FrameList[i].Layers) * i, false);
			AsciiImage frame = AsciiImage();
			success = frame._Load(in, header, header.FrameList[i]);
			_Frames.push_back(frame);
		}
		if (!success) {
			*this = AsciiAnimation();
		}
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiAnimation::LoadFile(const string& path) {
	FileInputStream in = FileInputStream();
	if (in.Open(path)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiAnimation::LoadResource(const string& name, const string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.Open(name, type)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiAnimation::LoadResource(const string& name, unsigned int typeId) {
	ResourceInputStream in = ResourceInputStream();
	if (in.Open(name, typeId)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiAnimation::LoadResource(unsigned int id, const string& type) {
	ResourceInputStream in = ResourceInputStream();
	if (in.Open(id, type)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiAnimation::LoadResource(unsigned int id, unsigned int typeId) {
	ResourceInputStream in = ResourceInputStream();
	if (in.Open(id, typeId)) {
		bool success = Load(&in);
		in.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiAnimation::LoadClipboard() {
	ClipboardInputStream in = ClipboardInputStream();
	if (in.IsFormatAvailable(AsciiImageInfo::ClipboardFormat)) {
		if (in.Open()) {
			bool success = false;
			if (in.GetData(AsciiImageInfo::ClipboardFormat)) {
				success = Load(&in);
			}
			in.Close();
			return success;
		}
	}
	else if (in.IsFileFormatAvailable()) {
		if (in.Open()) {
			vector<string> files = in.ReadFileNames();
			in.Close();
			if (files.size() == 1) {
				return LoadFile(files[0]) && !in.Error();
			}
		}
	}
	return false;
}
//--------------------------------
bool PowerConsole::Drawing::AsciiAnimation::Save(OutputStream* out, unsigned short version, bool smallFile) {
	AsciiImageHeader header = AsciiImageInfo::CreateHeader(*this, version, smallFile);

	if (AsciiImageInfo::SaveHeader(out, header, true)) {
		bool success = true;
		for (int i = 0; i < (int)_Frames.size() && success; i++) {
			out->SetPointer(header.FileStart + header.FrameArrayOffset + header.FrameHeaderSize +
				(header.FrameHeaderSize + header.FrameSize * header.FrameList[i].Layers) * i, false);
			success = _Frames[i]._Save(out, header, header.FrameList[i]);
		}
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiAnimation::SaveFile(const string& path, unsigned short version, bool smallFile) {
	FileOutputStream out = FileOutputStream();
	if (out.Open(path)) {
		bool success = Save(&out, version, smallFile);
		out.Close();
		return success;
	}
	return false;
}
bool PowerConsole::Drawing::AsciiAnimation::SaveClipboard(unsigned short version, bool smallFile) {
	ClipboardOutputStream out = ClipboardOutputStream();
	if (out.Open()) {
		bool success = false;
		if (Save(&out, version, smallFile)) {
			success = out.SetData(AsciiImageInfo::ClipboardFormat);
		}
		out.Close();
		return success;
	}
	return false;
}
//============ FRAMES ============
AsciiImageBase& PowerConsole::Drawing::AsciiAnimation::GetFrame(int index) const {
	return (AsciiImageBase&)_Frames[index];
}
void PowerConsole::Drawing::AsciiAnimation::AddFrame() {
	_Frames.push_back(AsciiImage(_Frames.back()._Size, _Frames[0]._Format, _Frames[0]._Background));
}
void PowerConsole::Drawing::AsciiAnimation::AddFrame(int index) {
	if (index == (int)_Frames.size())
		_Frames.push_back(AsciiImage(_Frames.back()._Size, _Frames[0]._Format, _Frames[0]._Background));
	else
		_Frames.insert(_Frames.begin() + index, AsciiImage(_Frames[max(0, index)]._Size, _Frames[0]._Format, _Frames[0]._Background));
}
void PowerConsole::Drawing::AsciiAnimation::AddFrame(int index, const AsciiImageBase& image) {
	if (GetFlag(_Frames[0]._Format, FormatMultiSize)) {
		_Frames.insert(_Frames.begin() + index, AsciiImage(_Frames[0]._Format, _Frames[0]._Background, image));
	}
	else {
		_Frames.insert(_Frames.begin() + index, AsciiImage(Coord::Zero, _Frames[0]._Size, _Frames[0]._Format, _Frames[0]._Background, image));
	}
}
void PowerConsole::Drawing::AsciiAnimation::AddFrame(int index, Coord part, Coord partSize, const AsciiImageBase& image) {
	if (GetFlag(_Frames[0]._Format, FormatMultiSize)) {
		_Frames.insert(_Frames.begin() + index, AsciiImage(part, partSize, _Frames[0]._Format, _Frames[0]._Background, image));
	}
	else {
		_Frames.insert(_Frames.begin() + index, AsciiImage(part, _Frames[0]._Size, _Frames[0]._Format, _Frames[0]._Background, image));
	}
}
void PowerConsole::Drawing::AsciiAnimation::SetFrame(int index, const AsciiImageBase& image) {
	if (GetFlag(_Frames[0]._Format, FormatMultiSize)) {
		_Frames[index] = AsciiImage(_Frames[0]._Format, _Frames[0]._Background, image);
	}
	else {
		_Frames[index] = AsciiImage(Coord::Zero, _Frames[0]._Size, _Frames[0]._Format, _Frames[0]._Background, image);
	}
}
void PowerConsole::Drawing::AsciiAnimation::SetFrame(int index, Coord part, Coord partSize, const AsciiImageBase& image) {
	if (GetFlag(_Frames[0]._Format, FormatMultiSize)) {
		_Frames[index] = AsciiImage(part, partSize, _Frames[0]._Format, _Frames[0]._Background, image);
	}
	else {
		_Frames[index] = AsciiImage(part, _Frames[0]._Size, _Frames[0]._Format, _Frames[0]._Background, image);
	}
}
void PowerConsole::Drawing::AsciiAnimation::RemoveFrame() {
	_Frames.pop_back();
}
void PowerConsole::Drawing::AsciiAnimation::RemoveFrame(int index) {
	_Frames.erase(_Frames.begin() + index);
}
void PowerConsole::Drawing::AsciiAnimation::SwapFrame(int index1, int index2) {
	AsciiImage frame = move(_Frames[index1]);
	_Frames[index1] = move(_Frames[index2]);
	_Frames[index2] = move(frame);
}
void PowerConsole::Drawing::AsciiAnimation::MoveFrame(int indexOld, int indexNew, bool relative) {
	AsciiImage frame = move(_Frames[indexOld]);
	_Frames.erase(_Frames.begin() + indexOld);
	_Frames.insert(_Frames.begin() + indexNew + (relative ? indexOld : 0), move(frame));
}
void PowerConsole::Drawing::AsciiAnimation::Clear() {
	while (_Frames.size() > 1) {
		_Frames.pop_back();
	}
	_Frames[0].Clear();
}
//=========== DRAWING ============
void PowerConsole::Drawing::AsciiAnimation::Translate(Coord distance, int index) {
	if (index != -1) {
		_Frames[index].Translate(distance);
	}
	else {
		for (int i = 0; i < (int)_Frames.size(); i++) {
			_Frames[i].Translate(distance);
		}
	}
}
void PowerConsole::Drawing::AsciiAnimation::Rotate(int rotations, int index) {
	if (index != -1) {
		_Frames[index].Rotate(rotations);
	}
	else {
		for (int i = 0; i < (int)_Frames.size(); i++) {
			_Frames[i].Rotate(rotations);
		}
	}
}
void PowerConsole::Drawing::AsciiAnimation::Flip(bool vertical, int index) {
	if (index != -1) {
		_Frames[index].Flip(vertical);
	}
	else {
		for (int i = 0; i < (int)_Frames.size(); i++) {
			_Frames[i].Flip(vertical);
		}
	}
}
void PowerConsole::Drawing::AsciiAnimation::Resize(Coord size, int index) {
	if (index != -1 && GetFlag(_Frames[0]._Format, FormatMultiSize)) {
		_Frames[index].Resize(size);
	}
	else {
		for (int i = 0; i < (int)_Frames.size(); i++) {
			_Frames[i].Resize(size);
		}
	}
}
void PowerConsole::Drawing::AsciiAnimation::RotateAndResize(int rotations, int index) {
	if (index != -1 && GetFlag(_Frames[0]._Format, FormatMultiSize)) {
		_Frames[index].RotateAndResize(rotations);
	}
	else {
		for (int i = 0; i < (int)_Frames.size(); i++) {
			_Frames[i].RotateAndResize(rotations);
		}
	}
}
//========= INFORMATION ==========
Coord PowerConsole::Drawing::AsciiAnimation::Size() const {
	Coord maxSize = _Frames[0]._Size;
	for (int i = 1; i < (int)_Frames.size(); i++) {
		maxSize.X = max(maxSize.X, _Frames[i]._Size.X);
		maxSize.Y = max(maxSize.Y, _Frames[i]._Size.Y);
	}
	return maxSize;
}
Pixel PowerConsole::Drawing::AsciiAnimation::GetBackground() const {
	return _Frames[0]._Background;
}
void PowerConsole::Drawing::AsciiAnimation::SetBackground(Pixel background) {
	for (int i = 0; i < (int)_Frames.size(); i++) {
		_Frames[i]._Background = background;
	}
}
unsigned int PowerConsole::Drawing::AsciiAnimation::NumFrames() const {
	return _Frames.size();
}
unsigned short PowerConsole::Drawing::AsciiAnimation::GetFormat() const {
	return _Frames[0]._Format;
}
void PowerConsole::Drawing::AsciiAnimation::SetFormat(unsigned short format) {
	if (!GetFlag(format, FormatAnimation)) {
		while ((int)_Frames.size() > 1) {
			_Frames.pop_back();
		}
		_Frames[0]._SetFormat(format);
	}
	else {
		Coord size = Size();
		for (int i = 0; i < (int)_Frames.size(); i++) {
			_Frames[i]._SetFormat(format);
			if (!GetFlag(format, FormatMultiSize))
				_Frames[i].Resize(size);
		}
	}
}
unsigned int PowerConsole::Drawing::AsciiAnimation::GetAnimationSpeed() const {
	return _AnimationSpeed;
}
void PowerConsole::Drawing::AsciiAnimation::SetAnimationSpeed(unsigned int animationSpeed) {
	_AnimationSpeed = animationSpeed;
}
void PowerConsole::Drawing::AsciiAnimation::SetFrameSpeed(unsigned int frameSpeed, int index) {
	if (index != -1) {
		_Frames[index]._FrameSpeed = frameSpeed;
	}
	else {
		for (int i = 0; i < (int)_Frames.size(); i++) {
			_Frames[i]._FrameSpeed = frameSpeed;
		}
	}
}
//================================================================
// END ASCII IMAGE CPP:
//================================================================