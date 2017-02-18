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
// Pixel:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::Drawing::Pixel::Pixel() {
	this->Char			= ' ';
	this->Color			= 0x07;
	this->Attributes	= AttributeFill;
}
PowerConsole::Drawing::Pixel::Pixel(unsigned char character, unsigned char color) {
	this->Char			= character;
	this->Color			= color;
	this->Attributes	= AttributeFill;
}
PowerConsole::Drawing::Pixel::Pixel(unsigned char character, unsigned char color, unsigned short attributes) {
	this->Char			= character;
	this->Color			= color;
	this->Attributes	= attributes;
}
//========= INFORMATION ==========
bool PowerConsole::Drawing::Pixel::Matches(Pixel pixel, bool specific) {
	if (Attributes == pixel.Attributes) {
		if (Char == pixel.Char && Color == pixel.Color)
			return true;
		if (!specific) {
			// Check if the characters are matching because both foreground and
			// background colors are the same
			bool full1		= (GetFColor(Color) == GetBColor(Color) ||
							  Char == 0 || Char == 32 || Char == 255 || Char == 219);
			char fullColor1	= (Char == 219 ? GetFColor(Color) : GetBColor(Color));
			bool full2		= (GetFColor(pixel.Color) == GetBColor(pixel.Color) ||
							  pixel.Char == 0 || pixel.Char == 32 || pixel.Char == 255 || pixel.Char == 219);
			char fullColor2	= (pixel.Char == 219 ? GetFColor(pixel.Color) : GetBColor(pixel.Color));
			if (full1 && full2 && fullColor1 == fullColor2)
				return true;

			// Check if the characters are matching because they are inverse characters
			bool inverse = (Color == SwapColor(pixel.Color));
			if (inverse) {
				if ((Char == 7 && pixel.Char == 8) ||
					(Char == 8 && pixel.Char == 7))
					return true;
				if ((Char == 9 && pixel.Char == 10) ||
					(Char == 10 && pixel.Char == 9))
					return true;
				if ((Char == 220 && pixel.Char == 223) ||
					(Char == 223 && pixel.Char == 220))
					return true;
				if ((Char == 221 && pixel.Char == 222) ||
					(Char == 222 && pixel.Char == 221))
					return true;
			}
		}
	}
	return false;
}
/*void PowerConsole::Drawing::Pixel::SetFColor(unsigned char color) {

}
void PowerConsole::Drawing::Pixel::SetBColor(unsigned char color) {

}
void PowerConsole::Drawing::Pixel::SetFAttributes(unsigned char color) {

}
void PowerConsole::Drawing::Pixel::SetBAttributes(unsigned char color) {

}*/
//----------------------------------------------------------------
// AsciiImageBase:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::Drawing::AsciiImageBase::AsciiImageBase() {
	this->_Size			= Coord::One;
	this->_Format		= FormatBasic;
	this->_Background	= Pixel();
	this->_FrameSpeed	= 1;

	this->_Pixels		= new Pixel*[1];
	this->_Pixels[0]	= new Pixel[1];
	this->_Pixels[0][0]	= this->_Background;
}
PowerConsole::Drawing::AsciiImageBase::AsciiImageBase(Coord size, unsigned short format) {
	this->_Size			= size;
	this->_Format		= format;
	this->_Background	= Pixel();
	this->_FrameSpeed	= 1;

	this->_Pixels		= new Pixel*[this->_Size.X];
	for (int i = 0; i < this->_Size.X; i++) {
		this->_Pixels[i]	= new Pixel[this->_Size.Y];
		for (int j = 0; j < this->_Size.Y; j++) {
			this->_Pixels[i][j]	= this->_Background;
		}
	}
}
PowerConsole::Drawing::AsciiImageBase::AsciiImageBase(Coord size, unsigned short format, Pixel background) {
	this->_Size			= size;
	this->_Format		= format;
	this->_Background	= background;
	this->_FrameSpeed	= 1;

	this->_Pixels		= new Pixel*[this->_Size.X];
	for (int i = 0; i < this->_Size.X; i++) {
		this->_Pixels[i]	= new Pixel[this->_Size.Y];
		for (int j = 0; j < this->_Size.Y; j++) {
			this->_Pixels[i][j]	= this->_Background;
		}
	}
	
	// Make sure the image follows the format
	if (!GetFlag(this->_Format, FormatAttributes)) {
		this->_Background.Attributes = AttributeFill;
	}
}
PowerConsole::Drawing::AsciiImageBase::AsciiImageBase(unsigned short format, Pixel background, const AsciiImageBase& image) {
	this->_Size			= image._Size;
	this->_Format		= format;
	this->_Background	= background;
	this->_FrameSpeed	= image._FrameSpeed;

	this->_Pixels		= new Pixel*[this->_Size.X];
	for (int i = 0; i < this->_Size.X; i++) {
		this->_Pixels[i]	= new Pixel[this->_Size.Y];
		for (int j = 0; j < this->_Size.Y; j++) {
			this->_Pixels[i][j]	= image._Pixels[i][j];
		}
	}

	// Make sure the image follows the format
	if (!GetFlag(this->_Format, FormatAttributes)) {
		this->_Background.Attributes = AttributeFill;

		for (int i = 0; i < this->_Size.X; i++) {
			for (int j = 0; j < this->_Size.Y; j++) {
				_Pixels[i][j].Attributes = AttributeFill;
			}
		}
	}
}
PowerConsole::Drawing::AsciiImageBase::AsciiImageBase(unsigned short format, Pixel background, AsciiImageBase&& image) {
	this->_Size			= image._Size;
	this->_Format		= format;
	this->_Background	= background;
	this->_FrameSpeed	= image._FrameSpeed;
	
	this->_Pixels		= image._Pixels;
	image._Pixels		= nullptr;

	// Make sure the image follows the format
	if (!GetFlag(this->_Format, FormatAttributes)) {
		this->_Background.Attributes = AttributeFill;

		for (int i = 0; i < this->_Size.X; i++) {
			for (int j = 0; j < this->_Size.Y; j++) {
				_Pixels[i][j].Attributes = AttributeFill;
			}
		}
	}
}
PowerConsole::Drawing::AsciiImageBase::AsciiImageBase(Coord part, Coord partSize, unsigned short format, Pixel background, const AsciiImageBase& image) {
	FixRect(part, partSize);
	
	this->_Size			= partSize;
	this->_Format		= format;
	this->_Background	= background;
	this->_FrameSpeed	= image._FrameSpeed;

	this->_Pixels		= new Pixel*[this->_Size.X];
	for (int i = 0; i < this->_Size.X; i++) {
		this->_Pixels[i]	= new Pixel[this->_Size.Y];
		for (int j = 0; j < this->_Size.Y; j++) {
			if (Coord(i, j) + part >= Coord::Zero && Coord(i, j) + part < image._Size)
				this->_Pixels[i][j]	= image._Pixels[i + part.X][j + part.Y];
			else
				this->_Pixels[i][j]	= this->_Background;
		}
	}
	
	// Make sure the image follows the format
	if (!GetFlag(this->_Format, FormatAttributes)) {
		this->_Background.Attributes = AttributeFill;

		for (int i = 0; i < this->_Size.X; i++) {
			for (int j = 0; j < this->_Size.Y; j++) {
				_Pixels[i][j].Attributes = AttributeFill;
			}
		}
	}
}
PowerConsole::Drawing::AsciiImageBase::AsciiImageBase(const AsciiImageBase& image) {
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
PowerConsole::Drawing::AsciiImageBase::AsciiImageBase(AsciiImageBase&& image) {
	this->_Size			= image._Size;
	this->_Format		= image._Format;
	this->_Background	= image._Background;
	this->_FrameSpeed	= image._FrameSpeed;

	this->_Pixels		= image._Pixels;
	image._Pixels		= nullptr;
}
PowerConsole::Drawing::AsciiImageBase::~AsciiImageBase() {
	if (this->_Pixels != nullptr) {
		for (int i = 0; i < this->_Size.X; i++) {
			//cout << "Help1 ";
			delete[] _Pixels[i];
			//cout << "Help2 ";
			//this->_Pixels[i] = nullptr;
		}
		delete[] (this->_Pixels);
		this->_Pixels = nullptr;
	}
}
//========== OPERATORS ===========
AsciiImageBase& PowerConsole::Drawing::AsciiImageBase::operator=(const AsciiImageBase& image) {
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
	return *this;
}
AsciiImageBase& PowerConsole::Drawing::AsciiImageBase::operator=(AsciiImageBase&& image) {
	if (this != &image) {
		_Delete();
	
		this->_Size			= image._Size;
		this->_Format		= image._Format;
		this->_Background	= image._Background;
		this->_FrameSpeed	= image._FrameSpeed;
	
		this->_Pixels		= image._Pixels;
		image._Pixels		= nullptr;
	}
	return *this;
}
//========== MANAGEMENT ==========
void PowerConsole::Drawing::AsciiImageBase::_Delete() {
	if (_Pixels != nullptr) {
		for (int i = 0; i < _Size.X; i++) {
			delete[] _Pixels[i];
			_Pixels[i] = nullptr;
		}
		delete[] _Pixels;
		_Pixels = nullptr;
	}
}
void PowerConsole::Drawing::AsciiImageBase::_Resize(Coord size) {
	if (_Size != size && size >= Coord::One && size <= AsciiImageInfo::MaxSize) {
		Pixel** newPixels	= new Pixel*[size.X];
		for (int i = 0; i < size.X; i++) {
			newPixels[i]	= new Pixel[size.Y];
			for (int j = 0; j < size.Y; j++) {
				if (Coord(i, j) < _Size)
					newPixels[i][j]	= _Pixels[i][j];
				else
					newPixels[i][j]	= _Background;
			}
		}

		_Delete();
		_Size	= size;
		_Pixels	= newPixels;
	}
}
void PowerConsole::Drawing::AsciiImageBase::_SetFormat(unsigned short format) {
	_Format = format;

	// Make sure the image follows the attribute format
	if (!GetFlag(_Format, FormatAttributes)) {
		_Background.Attributes = AttributeFill;

		for (int i = 0; i < _Size.X; i++) {
			for (int j = 0; j < _Size.Y; j++) {
				_Pixels[i][j].Attributes = AttributeFill;
			}
		}
	}
}
bool PowerConsole::Drawing::AsciiImageBase::_Load(InputStream* in, const AsciiImageHeader& header, const AsciiImageFrameHeader& frame, bool isLayer) {

	if (in->IsOpen() && !in->Error()) {

		// Load the final layer to draw to
		Pixel** newPixels	= new Pixel*[frame.Size.X];
		for (int i = 0; i < frame.Size.X; i++) {
			newPixels[i]	= new Pixel[frame.Size.Y];
			for (int j = 0; j < frame.Size.Y; j++) {
				newPixels[i][j]	= header.Background;
			}
		}
		
		// Unlike all other 2D for loops, the IO loops must iterate in the order of y, x
		// This way the file will be saved and loaded row-by-row like most other image formats
		for (int j = 0; j < frame.Size.Y; j++) {
			for (int i = 0; i < frame.Size.X; i++) {
				AsciiImageInfo::LoadPixel(in, header, newPixels[i][j]);
			}
		}

		// Only overwrite the image once it has been successfully loaded
		if (!in->Error()) {
			_Delete();
			_Size		= frame.Size;
			_Format		= header.Format;
			_FrameSpeed	= frame.FrameSpeed;
			_Background	= header.Background;
			_Pixels		= newPixels;
			return true;
		}
	}
	return false;
}
bool PowerConsole::Drawing::AsciiImageBase::_Save(OutputStream* out, const AsciiImageHeader& header, const AsciiImageFrameHeader& frame, bool isLayer) {

	if (out->IsOpen() && !out->Error()) {

		// Unlike all other 2D for loops, the IO loops must iterate in the order of y, x
		// This way the file will be saved and loaded row-by-row like most other image formats
		for (int j = 0; j < _Size.Y; j++) {
			for (int i = 0; i < _Size.X; i++) {
				AsciiImageInfo::SavePixel(out, header, _Pixels[i][j]);
			}
		}

		return !out->Error();
	}
	return false;
}
//=========== DRAWING ============
void PowerConsole::Drawing::AsciiImageBase::Clear() {
	for (int i = 0; i < _Size.X; i++) {
		for (int j = 0; j < _Size.Y; j++) {
			_Pixels[i][j] = _Background;
		}
	}
}
void PowerConsole::Drawing::AsciiImageBase::Fill(Pixel pixel, unsigned short modAttributes, unsigned short modOptions) {
	SetPixel(Coord::Zero, _Size, pixel, modAttributes, modOptions);
}
//--------------------------------
void PowerConsole::Drawing::AsciiImageBase::SetChar(Coord point, unsigned char character, unsigned short modAttributes) {
	SetChar(point, Coord::One, character, modAttributes);
}
void PowerConsole::Drawing::AsciiImageBase::SetChar(Coord point, Coord size, unsigned char character, unsigned short modAttributes) {
	FixRect(point, size);
	
	for (int i = point.X; (i < point.X + size.X) && (i < _Size.X); i++) {
		for (int j = point.Y; (j < point.Y + size.Y) && (j < _Size.Y); j++) {
			if (i >= 0 && j >= 0) {
				if (GetFlag(modAttributes, AttributeChar))
					_Pixels[i][j].Char = character;
			}
		}
	}
}
void PowerConsole::Drawing::AsciiImageBase::SetColor(Coord point, unsigned char color, unsigned short modAttributes) {
	SetColor(point, Coord::One, color, modAttributes);
}
void PowerConsole::Drawing::AsciiImageBase::SetColor(Coord point, Coord size, unsigned char color, unsigned short modAttributes) {
	FixRect(point, size);
	
	for (int i = point.X; (i < point.X + size.X) && (i < _Size.X); i++) {
		for (int j = point.Y; (j < point.Y + size.Y) && (j < _Size.Y); j++) {
			if (i >= 0 && j >= 0) {
				if (GetFlag(modAttributes, AttributeFColor))
					_Pixels[i][j].Color = SetFColor(_Pixels[i][j].Color, color);
				if (GetFlag(modAttributes, AttributeBColor))
					_Pixels[i][j].Color = SetBColor(_Pixels[i][j].Color, color);
			}
		}
	}
}
void PowerConsole::Drawing::AsciiImageBase::SetAttributes(Coord point, unsigned short attributes, unsigned short modAttributes) {
	SetAttributes(point, Coord::One, attributes, modAttributes);
}
void PowerConsole::Drawing::AsciiImageBase::SetAttributes(Coord point, Coord size, unsigned short attributes, unsigned short modAttributes) {
	if (!GetFlag(_Format, FormatAttributes))
		return;
	FixRect(point, size);
	
	for (int i = point.X; (i < point.X + size.X) && (i < _Size.X); i++) {
		for (int j = point.Y; (j < point.Y + size.Y) && (j < _Size.Y); j++) {
			if (i >= 0 && j >= 0)
				_Pixels[i][j].Attributes = (_Pixels[i][j].Attributes & ~modAttributes) | (attributes & modAttributes);
		}
	}
}
void PowerConsole::Drawing::AsciiImageBase::SetSpecial(Coord point, unsigned short attributes) {
	SetSpecial(point, Coord::One, attributes);
}
void PowerConsole::Drawing::AsciiImageBase::SetSpecial(Coord point, Coord size, unsigned short attributes) {
	FixRect(point, size);
	
	for (int i = point.X; (i < point.X + size.X) && (i < _Size.X); i++) {
		for (int j = point.Y; (j < point.Y + size.Y) && (j < _Size.Y); j++) {
			if (i >= 0 && j >= 0) {
				// Invert
				if (GetFlag(attributes, AttributeFInvert))
					_Pixels[i][j].Color = InvertFColor(_Pixels[i][j].Color);
				if (GetFlag(attributes, AttributeBInvert))
					_Pixels[i][j].Color = InvertBColor(_Pixels[i][j].Color);

				// Lighten
				if (GetFlag(attributes, AttributeFLighten))
					_Pixels[i][j].Color = LightenFColor(_Pixels[i][j].Color);
				if (GetFlag(attributes, AttributeBLighten))
					_Pixels[i][j].Color = LightenBColor(_Pixels[i][j].Color);

				// Darken
				if (GetFlag(attributes, AttributeFDarken))
					_Pixels[i][j].Color = DarkenFColor(_Pixels[i][j].Color);
				if (GetFlag(attributes, AttributeBDarken))
					_Pixels[i][j].Color = DarkenBColor(_Pixels[i][j].Color);
			}
		}
	}
}
void PowerConsole::Drawing::AsciiImageBase::SetPixel(Coord point, Pixel pixel, unsigned short modAttributes, unsigned short modOptions) {
	SetPixel(point, Coord::One, pixel, modAttributes, modOptions);
}
void PowerConsole::Drawing::AsciiImageBase::SetPixel(Coord point, Coord size, Pixel pixel, unsigned short modAttributes, unsigned short modOptions) {
	FixRect(point, size);
	
	bool replace		= GetFlag(modOptions, OptionReplace);
	bool opposite		= GetFlag(modOptions, OptionOpposite);
	bool inverse		= GetFlag(modOptions, OptionInverse);
	bool noPixel		= GetFlag(modOptions, OptionNoPixel);
	bool noAttributes	= GetFlag(modOptions, OptionNoAttributes) || !GetFlag(_Format, FormatAttributes);
	bool copySpecial	= GetFlag(modOptions, OptionCopySpecial);

	for (int i = point.X; (i < point.X + size.X) && (i < _Size.X); i++) {
		for (int j = point.Y; (j < point.Y + size.Y) && (j < _Size.Y); j++) {
			if (i >= 0 && j >= 0) {

				//========== CHARACTER ===========

				// Apply the character to the destination
				if ((GetFlag(pixel.Attributes, AttributeChar) != opposite || replace) &&
					GetFlag(modAttributes, AttributeChar)) {
					if (!noPixel)
						_Pixels[i][j].Char = pixel.Char;
					if (!noAttributes) {
						if (inverse == GetFlag(pixel.Attributes, AttributeChar))
							_Pixels[i][j].Attributes &= ~AttributeChar;
						else
							_Pixels[i][j].Attributes |= AttributeChar;
					}
				}

				//============ COLOR =============

				// Apply the foreground color to the destination
				if ((GetFlag(pixel.Attributes, AttributeFColor) != opposite || replace) &&
					GetFlag(modAttributes, AttributeFColor)) {
					if (!noPixel)
						_Pixels[i][j].Color = SetFColor(_Pixels[i][j].Color, pixel.Color);
					if (!noAttributes) {
						if (inverse == GetFlag(pixel.Attributes, AttributeFColor))
							_Pixels[i][j].Attributes &= ~AttributeFColor;
						else
							_Pixels[i][j].Attributes |= AttributeFColor;
					}
				}
				// Apply the background color to the destination
				if ((GetFlag(pixel.Attributes, AttributeBColor) != opposite || replace) &&
					GetFlag(modAttributes, AttributeBColor)) {
					if (!noPixel)
						_Pixels[i][j].Color = SetBColor(_Pixels[i][j].Color, pixel.Color);
					if (!noAttributes) {
						if (inverse == GetFlag(pixel.Attributes, AttributeBColor))
							_Pixels[i][j].Attributes &= ~AttributeBColor;
						else
							_Pixels[i][j].Attributes |= AttributeBColor;
					}
				}
				
				//============ INVERT ============

				// Apply the foreground invert to the destination
				if ((GetFlag(pixel.Attributes, AttributeFInvert) != opposite || replace) &&
					GetFlag(modAttributes, AttributeFInvert)) {
					if (!noPixel && !copySpecial)
						_Pixels[i][j].Color = InvertFColor(_Pixels[i][j].Color);
					else if (!noAttributes && copySpecial) {
						if (inverse == GetFlag(pixel.Attributes, AttributeFInvert))
							_Pixels[i][j].Attributes &= ~AttributeFInvert;
						else
							_Pixels[i][j].Attributes |= AttributeFInvert;
					}
				}
				// Apply the background invert to the destination
				if ((GetFlag(pixel.Attributes, AttributeBInvert) != opposite || replace) &&
					GetFlag(modAttributes, AttributeBInvert)) {
					if (!noPixel && !copySpecial)
						_Pixels[i][j].Color = InvertBColor(_Pixels[i][j].Color);
					else if (!noAttributes && copySpecial) {
						if (inverse == GetFlag(pixel.Attributes, AttributeBInvert))
							_Pixels[i][j].Attributes &= ~AttributeBInvert;
						else
							_Pixels[i][j].Attributes |= AttributeBInvert;
					}
				}
				
				//=========== LIGHTEN ============

				// Apply the foreground lighten to the destination
				if ((GetFlag(pixel.Attributes, AttributeFLighten) != opposite || replace) &&
					GetFlag(modAttributes, AttributeFLighten)) {
					if (!noPixel && !copySpecial)
						_Pixels[i][j].Color = LightenFColor(_Pixels[i][j].Color);
					else if (!noAttributes && copySpecial) {
						if (inverse == GetFlag(pixel.Attributes, AttributeFLighten))
							_Pixels[i][j].Attributes &= ~AttributeFLighten;
						else
							_Pixels[i][j].Attributes |= AttributeFLighten;
					}
				}
				// Apply the background lighten to the destination
				if ((GetFlag(pixel.Attributes, AttributeBLighten) != opposite || replace) &&
					GetFlag(modAttributes, AttributeBLighten)) {
					if (!noPixel && !copySpecial)
						_Pixels[i][j].Color = LightenBColor(_Pixels[i][j].Color);
					else if (!noAttributes && copySpecial) {
						if (inverse == GetFlag(pixel.Attributes, AttributeBLighten))
							_Pixels[i][j].Attributes &= ~AttributeBLighten;
						else
							_Pixels[i][j].Attributes |= AttributeBLighten;
					}
				}
				
				//============ DARKEN ============

				// Apply the foreground darken to the destination
				if ((GetFlag(pixel.Attributes, AttributeFDarken) != opposite || replace) &&
					GetFlag(modAttributes, AttributeFDarken)) {
					if (!noPixel && !copySpecial)
						_Pixels[i][j].Color = DarkenFColor(_Pixels[i][j].Color);
					else if (!noAttributes && copySpecial) {
						if (inverse == GetFlag(pixel.Attributes, AttributeFDarken))
							_Pixels[i][j].Attributes &= ~AttributeFDarken;
						else
							_Pixels[i][j].Attributes |= AttributeFDarken;
					}
				}
				// Apply the background darken to the destination
				if ((GetFlag(pixel.Attributes, AttributeBDarken) != opposite || replace) &&
					GetFlag(modAttributes, AttributeBDarken)) {
					if (!noPixel && !copySpecial)
						_Pixels[i][j].Color = DarkenBColor(_Pixels[i][j].Color);
					else if (!noAttributes && copySpecial) {
						if (inverse == GetFlag(pixel.Attributes, AttributeBDarken))
							_Pixels[i][j].Attributes &= ~AttributeBDarken;
						else
							_Pixels[i][j].Attributes |= AttributeBDarken;
					}
				}

			}
		}
	}
}
//--------------------------------
void PowerConsole::Drawing::AsciiImageBase::DrawLine(Coord point, Coord size, Pixel pixel, bool asEnd, unsigned short modAttributes, unsigned short modOptions) {
	if (asEnd)
		size -= point - Coord(1, 1);

	// Bresenham's algorithm
	//http://www.brackeen.com/vga/shapes.html#shapes3.0

	// Absolute distance
	Coord absd = Coord(abs(size.X), abs(size.Y));
	// Sign of the distance
	Coord inc = Coord(size.X == 0 ? 0 : (size.X > 0 ? 1 : -1),
					  size.Y == 0 ? 0 : (size.Y > 0 ? 1 : -1));

	SetPixel(point, pixel, modAttributes, modOptions);

	if (absd.X >= absd.Y) {
		int D = 2 * absd.Y - absd.X;
		for (Coord p = point + Coord(inc.X, 0); p.X != point.X + size.X + inc.X; p.X += inc.X) {
			if (D > 0) {
				p.Y += inc.Y;
				D += 2 * absd.Y - 2 * absd.X;
			}
			else {
				D += 2 * absd.Y;
			}
			SetPixel(p, pixel, modAttributes, modOptions);
		}
	}
	else {
		int D = 2 * absd.X - absd.Y;
		for (Coord p = point + Coord(0, inc.Y); p.Y != point.Y + size.Y + inc.Y; p.Y += inc.Y) {
			if (D > 0) {
				p.X += inc.X;
				D += 2 * absd.X - 2 * absd.Y;
			}
			else {
				D += 2 * absd.X;
			}
			SetPixel(p, pixel, modAttributes, modOptions);
		}
	}
}
void PowerConsole::Drawing::AsciiImageBase::DrawRect(Coord point, Coord size, Pixel pixel, bool fill, bool asEnd, unsigned short modAttributes, unsigned short modOptions) {
	if (asEnd)
		size -= point - Coord::One;
	FixRect(point, size);
	
	if (fill) {
		SetPixel(point, size, pixel, modAttributes, modOptions);
	}
	else {
		SetPixel(point, Coord(size.X, 1), pixel, modAttributes, modOptions);
		SetPixel(point, Coord(1, size.Y), pixel, modAttributes, modOptions);
		SetPixel(point + Coord(0, size.Y - 1), Coord(size.X, 1), pixel, modAttributes, modOptions);
		SetPixel(point + Coord(size.X - 1, 0), Coord(1, size.Y), pixel, modAttributes, modOptions);
	}
}
void PowerConsole::Drawing::AsciiImageBase::DrawEllipse(Coord point, Coord size, Pixel pixel, bool fill, bool asOrigin, unsigned short modAttributes, unsigned short modOptions) {
	if (asOrigin) {
		point -= size;
		size *= 2;
	}
	FixRect(point, size);
	
	// Bresenham's algorithm
	//http://enchantia.com/graphapp/doc/tech/ellipses.html

	// e(x,y) = b^2*x^2 + a^2*y^2 - a^2*b^2
	Point2D center = Point2D(size) / 2.0 + Point2D(point);
	Point2D radius = Point2D(size) / 2.0;
	Coord radius2 = radius * radius;
	int crit1 = int(-radius2.X / 4 - fmod(radius.Y, 2) - radius2.Y);
	int crit2 = int(-radius2.Y / 4 - fmod(radius.X, 2) - radius2.X);
	int crit3 = int(-radius2.Y / 4 - fmod(radius.X, 2));
	int t = int(-radius2.X * radius.Y);
	Coord dt = Coord(0, int(-2 * radius2.X * radius.Y));
	Coord d2t = Coord(radius2.Y, radius2.X) * 2;
	unsigned int width = 1;

	if (radius.Y == 0 && fill)
		SetPixel(Coord(center - Point2D(radius.X, 0)), Coord(int(2 * radius.X + 1), 1), pixel, modAttributes, modOptions);

	for (Coord p = Coord(0, (int)radius.Y); p.Y >= 0 && p.X <= radius.X; ) {
		if (fill) {
			if (t + radius2.Y * p.X <= crit1 || t + radius2.X * p.Y <= crit3) { // e(x+1,y-1/2) <= 0  ||  e(x+1/2,y) <= 0
				p.X++; dt.X += d2t.X; t += dt.X;
				width += 2;
			}
			else if (t - radius2.X * p.Y > crit2) { // e(x+1/2,y-1) > 0
				SetPixel(center - p, Coord(width, 1), pixel, modAttributes, modOptions);
				if (p.Y != 0)
					SetPixel(center + Coord(-p.X, p.Y), Coord(width, 1), pixel, modAttributes, modOptions);
				p.Y--; dt.Y += d2t.Y; t += dt.Y;
			}
			else {
				SetPixel(center - p, Coord(width, 1), pixel, modAttributes, modOptions);
				if (p.Y != 0)
					SetPixel(center + Coord(-p.X, p.Y), Coord(width, 1), pixel, modAttributes, modOptions);
				p.X++; dt.X += d2t.X; t += dt.X;
				p.Y--; dt.Y += d2t.Y; t += dt.Y;
				width += 2;
			}
		}
		else {
			SetPixel(center + p, pixel, modAttributes, modOptions);
			if (p.X != 0 || p.Y != 0)
				SetPixel(center - p, pixel, modAttributes, modOptions);
			if (p.X != 0 && p.Y != 0) {
				SetPixel(center + Coord(p.X, -p.Y), pixel, modAttributes, modOptions);
				SetPixel(center + Coord(-p.X, p.Y), pixel, modAttributes, modOptions);
			}
			if (t + radius2.Y * p.X <= crit1 || t + radius2.X * p.Y <= crit3) { // e(x+1,y-1/2) <= 0  ||  e(x+1/2,y) <= 0
				p.X++; dt.X += d2t.X; t += dt.X;
			}
			else if (t - radius2.X * p.Y > crit2) { // e(x+1/2,y-1) > 0
				p.Y--; dt.Y += d2t.Y; t += dt.Y;
			}
			else {
				p.X++; dt.X += d2t.X; t += dt.X;
				p.Y--; dt.Y += d2t.Y; t += dt.Y;
			}
		}
	}
}
void PowerConsole::Drawing::AsciiImageBase::DrawFloodFill(Coord point, Pixel pixel, bool specific, unsigned short modAttributes, unsigned short modOptions) {
	// East West algorithm
	//http://en.wikipedia.org/wiki/Flood_fill

	if (!(point >= Coord::Zero && point < _Size))
		return;
	Pixel target = _Pixels[point.X][point.Y];
	if (pixel.Matches(target, false))
		return;

	SetPixel(point, pixel, modAttributes, modOptions);
	if (_Pixels[point.X][point.Y].Matches(target)) {
		_Pixels[point.X][point.Y] = target;
		return;
	}
	_Pixels[point.X][point.Y] = target;

	queue<Coord> nodes;
	nodes.push(point);
	while (!nodes.empty()) {
		point = nodes.front();
		if (_Pixels[point.X][point.Y].Matches(target, specific)) {
			int width = 1;
			// Travel as far left as possible
			for (; point.X - 1 >= 0 && _Pixels[point.X - 1][point.Y].Matches(target, specific); point.X--, width++);
			// Travel as far right as possible
			for (; point.X + width < _Size.X && _Pixels[point.X + width][point.Y].Matches(target, specific); width++);
			SetPixel(point, Coord(width, 1), pixel, modAttributes, modOptions);

			// The continue makes sure that only one node is added for every
			// range of pixels in a row
			bool northcontinue = false, southcontinue = false;
			for (int i = 0; i < width; ++i) {
				if (point.Y - 1 >= 0) {
					if (_Pixels[point.X + i][point.Y - 1].Matches(target, specific)) {
						if (!northcontinue) {
							nodes.push(point + Coord(i, -1));
							northcontinue = true;
						}
					}
					else {
						northcontinue = false;
					}
				}
				if (point.Y + 1 < _Size.Y) {
					if (_Pixels[point.X + i][point.Y + 1].Matches(target, specific)) {
						if (!southcontinue) {
							nodes.push(point + Coord(i, 1));
							southcontinue = true;
						}
					}
					else {
						southcontinue = false;
					}
				}
			}

		}
		nodes.pop();
	}
}
void PowerConsole::Drawing::AsciiImageBase::DrawReplace(Coord point, Pixel pixel, bool specific, unsigned short modAttributes, unsigned short modOptions) {
	if (!(point >= Coord::Zero && point < _Size))
		return;
	Pixel target = _Pixels[point.X][point.Y];
	DrawReplace(target, pixel, specific, modAttributes, modOptions);
}
void PowerConsole::Drawing::AsciiImageBase::DrawReplace(Pixel target, Pixel pixel, bool specific, unsigned short modAttributes, unsigned short modOptions) {
	if (target.Matches(pixel, false))
		return;

	for (int i = 0; i < _Size.X; i++) {
		for (int j = 0; j < _Size.Y; j++) {
			if(_Pixels[i][j].Matches(target, specific))
				SetPixel(Coord(i, j), pixel, modAttributes, modOptions);
		}
	}
}
void PowerConsole::Drawing::AsciiImageBase::DrawString(Coord point, string text, Pixel pixel, Alignment alignment, unsigned short modAttributes, unsigned short modOptions) {
	if (alignment == Alignment::Center)
		point.X -= text.length() / 2;
	else if (alignment == Alignment::Right)
		point.X -= text.length() - 1;

	for (int i = 0; i < (int)text.length(); i++) {
		SetPixel(point + Coord(i, 0), Pixel(text[i], pixel.Color, pixel.Attributes), modAttributes, modOptions);
	}
}
void PowerConsole::Drawing::AsciiImageBase::DrawImage(Coord point, const AsciiImageBase& image, unsigned short modAttributes, unsigned short modOptions) {
	DrawImage(point, Coord::Zero, image._Size, image, modAttributes, modOptions);
}
void PowerConsole::Drawing::AsciiImageBase::DrawImage(Coord point, Coord part, Coord partSize, const AsciiImageBase& image, unsigned short modAttributes, unsigned short modOptions) {
	FixRect(part, partSize);

	for (int i = 0; (i < partSize.X) && (i + part.X < image._Size.X) && (i + point.X < _Size.X); i++) {
		for (int j = 0; (j < partSize.Y) && (j + part.Y < image._Size.Y) && (j + point.Y < _Size.Y); j++) {
			if (Coord(i, j) + point >= Coord::Zero && Coord(i, j) + part >= Coord::Zero)
				SetPixel(Coord(i, j) + point, image._Pixels[i + part.X][j + part.Y], modAttributes, modOptions);
		}
	}
}
//--------------------------------
/*void PowerConsole::Drawing::AsciiImageBase::DrawBorderLine(Coord point, Coord size, Pixel pixel, bool thick, bool priority, bool specific, bool asEnd, unsigned short modAttributes, unsigned short modOptions) {

}
void PowerConsole::Drawing::AsciiImageBase::DrawBorderRect(Coord point, Coord size, Pixel pixel, bool thick, bool priority, bool specific, bool asEnd, unsigned short modAttributes, unsigned short modOptions) {

}*/
//--------------------------------
void PowerConsole::Drawing::AsciiImageBase::Translate(Coord distance) {
	Pixel** newPixels	= new Pixel*[_Size.X];
	for (int i = 0; i < _Size.X; i++) {
		newPixels[i]	= new Pixel[_Size.Y];
		for (int j = 0; j < _Size.Y; j++) {
			Coord newPoint = Coord(i, j) - distance;
			
			if (newPoint >= Coord::Zero && newPoint < _Size)
				newPixels[i][j] = _Pixels[newPoint.X][newPoint.Y];
			else
				newPixels[i][j] = _Background;
		}
	}

	_Delete();
	_Pixels	= newPixels;
}
void PowerConsole::Drawing::AsciiImageBase::Rotate(int rotations) {
	rotations = rotations % 4 + (rotations < 0 ? 4 : 0);
	Pixel** newPixels	= new Pixel*[_Size.X];
	for (int i = 0; i < _Size.X; i++) {
		newPixels[i]	= new Pixel[_Size.Y];
		for (int j = 0; j < _Size.Y; j++) {
			Coord newPoint = Coord(i, j);
			for (int k = 0; k < rotations % 4; k++)
				Coord(newPoint.Y, (k % 2 == 0 ? _Size.X : _Size.Y) - newPoint.X - 1);

			if (newPoint >= Coord::Zero && newPoint < _Size)
				newPixels[i][j] = _Pixels[newPoint.X][newPoint.Y];
			else
				newPixels[i][j] = _Background;
		}
	}

	_Delete();
	_Pixels	= newPixels;
}
void PowerConsole::Drawing::AsciiImageBase::Flip(bool vertical) {
	Pixel** newPixels	= new Pixel*[_Size.X];
	for (int i = 0; i < _Size.X; i++) {
		newPixels[i]	= new Pixel[_Size.Y];
		for (int j = 0; j < _Size.Y; j++) {
			Coord newPoint = Coord(i, j);
			if (vertical)
				newPoint.Y = _Size.Y - newPoint.Y - 1;
			else
				newPoint.X = _Size.X - newPoint.X - 1;
			
			if (newPoint >= Coord::Zero && newPoint < _Size)
				newPixels[i][j] = _Pixels[newPoint.X][newPoint.Y];
			else
				newPixels[i][j] = _Background;
		}
	}

	_Delete();
	_Pixels	= newPixels;
}
//========= INFORMATION ==========
unsigned char PowerConsole::Drawing::AsciiImageBase::GetChar(Coord point) const {
	if (point >= Coord::Zero && point < _Size)
		return _Pixels[point.X][point.Y].Char;
	return '\0';
}
unsigned char PowerConsole::Drawing::AsciiImageBase::GetColor(Coord point) const {
	if (point >= Coord::Zero && point < _Size)
		return _Pixels[point.X][point.Y].Color;
	return 0x00;
}
unsigned short PowerConsole::Drawing::AsciiImageBase::GetAttributes(Coord point) const {
	if (point >= Coord::Zero && point < _Size)
		return _Pixels[point.X][point.Y].Attributes;
	return AttributeNone;
}
bool PowerConsole::Drawing::AsciiImageBase::GetAttributes(Coord point, unsigned short attributes) const {
	if (point >= Coord::Zero && point < _Size)
		return GetFlag(_Pixels[point.X][point.Y].Attributes, attributes);
	return false;
}
Pixel PowerConsole::Drawing::AsciiImageBase::GetPixel(Coord point) const {
	if (point >= Coord::Zero && point < _Size)
		return _Pixels[point.X][point.Y];
	return Pixel();
}
//--------------------------------
Coord PowerConsole::Drawing::AsciiImageBase::Size() const {
	return _Size;
}
Pixel PowerConsole::Drawing::AsciiImageBase::GetBackground() const {
	return _Background;
}
unsigned short PowerConsole::Drawing::AsciiImageBase::GetFormat() const {
	return _Format;
}
unsigned int PowerConsole::Drawing::AsciiImageBase::GetFrameSpeed() const {
	return _FrameSpeed;
}
Pixel** PowerConsole::Drawing::AsciiImageBase::GetPixels() const {
	return _Pixels;
}
//================================================================
// END ASCII IMAGE BASE CPP:
//================================================================