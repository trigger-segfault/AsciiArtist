/*=================================================================|
|* File:				ToolLine.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/15/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "ToolLine.h"
#include "Component.h"
#include "ConsoleApp.h"
#include "AsciiArtistManager.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;
using namespace PowerConsole::AsciiArtistApp;

//=================================================================|
// DEFINITIONS													   |
//=================================================================/

#define FixRect(point, size)		(point += Coord(size.X < 0 ? size.X + 1 : 0, size.Y < 0 ? size.Y + 1 : 0)); \
									(size *= Coord(size.X < 0 ? -1 : 1, size.Y < 0 ? -1 : 1))

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ToolLine
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::AsciiArtistApp::ToolLine::ToolLine() : Tool(ToolTypes::Line, "Line") {
	this->_secondary		= false;
	this->_startPoint		= Coord::Zero;
}
PowerConsole::AsciiArtistApp::ToolLine::~ToolLine() {
	
}
void PowerConsole::AsciiArtistApp::ToolLine::Initialize(AsciiArtistManager* aa, ConsoleApp* app, AsciiAnimation* drawingBuffer) {
	Tool::Initialize(aa, app, drawingBuffer);
}
void PowerConsole::AsciiArtistApp::ToolLine::Uninitialize() {
	Tool::Uninitialize();
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::AsciiArtistApp::ToolLine::Update() {
	
	if (!_drawing) {
		bool pressed = false;
		if (_aa->GetCanvas()->IsMouseInsideCanvas()) {
			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				pressed = true;
				_secondary = false;
			}
			else if (_app->GetMouse()->IsButtonPressed(MouseButtons::Right)) {
				pressed = true;
				_secondary = true;
			}
		}

		if (pressed) {
			_drawing = true;
			_startPoint = _aa->GetCanvas()->GetImageMousePosition();

			_drawingBuffer->GetFrame(0).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(1).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(2).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(3).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		}
	}
	else {
		if ((_app->GetMouse()->IsButtonPressed(MouseButtons::Left) && _secondary) ||
			(_app->GetMouse()->IsButtonPressed(MouseButtons::Right) && !_secondary)) {
			Cancel();
		}
		else if ((_app->GetMouse()->IsButtonReleased(MouseButtons::Left) && !_secondary) ||
				(_app->GetMouse()->IsButtonReleased(MouseButtons::Right) && _secondary)) {
			Finalize();
		}
		else {
			_drawingBuffer->GetFrame(0).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(1).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(2).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(3).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

			Coord endPoint = _aa->GetCanvas()->GetImageMousePosition();
			
			// Pixel Fill
			Pixel drawingPixel = (_secondary ? _aa->GetSecondaryPixel() : _aa->GetPrimaryPixel());
			unsigned short attributes = 0x0;
			if (_aa->IsCharacterDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeChar) != 0x0)
				attributes |= PixelAttributes::AttributeChar;
			if (_aa->IsFColorDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeFColor) != 0x0)
				attributes |= PixelAttributes::AttributeFColor;
			if (_aa->IsBColorDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeBColor) != 0x0)
				attributes |= PixelAttributes::AttributeBColor;

			_drawingBuffer->GetFrame(0).DrawLine(_startPoint, endPoint - _startPoint, drawingPixel, false, attributes);
			

			// Transparency Add
			drawingPixel = (_secondary ? _aa->GetSecondaryPixel() : _aa->GetPrimaryPixel());
			attributes = 0x0;
			if (_aa->IsCharacterDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeChar) == 0x0)
				attributes |= PixelAttributes::AttributeChar;
			if (_aa->IsFColorDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeFColor) == 0x0)
				attributes |= PixelAttributes::AttributeFColor;
			if (_aa->IsBColorDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeBColor) == 0x0)
				attributes |= PixelAttributes::AttributeBColor;

			drawingPixel.Char = _aa->GetImage()->GetBackground().Char;
			drawingPixel.Color = _aa->GetImage()->GetBackground().Color;
			drawingPixel.Attributes = attributes;
			_drawingBuffer->GetFrame(1).DrawLine(_startPoint, endPoint - _startPoint, drawingPixel, false, attributes);
			

			// Invert Add
			drawingPixel = (_secondary ? _aa->GetSecondaryPixel() : _aa->GetPrimaryPixel());
			attributes = 0x0;
			if (_aa->IsFColorDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeFInvert) != 0x0)
				attributes |= PixelAttributes::AttributeFInvert;
			if (_aa->IsBColorDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeBInvert) != 0x0)
				attributes |= PixelAttributes::AttributeBInvert;
			
			drawingPixel.Char = _aa->GetImage()->GetBackground().Char;
			drawingPixel.Color = _aa->GetImage()->GetBackground().Color;
			drawingPixel.Attributes = attributes;
			_drawingBuffer->GetFrame(2).DrawLine(_startPoint, endPoint - _startPoint, drawingPixel, false, attributes, PixelOptions::OptionCopySpecial);
			

			// Invert Remove
			drawingPixel = (_secondary ? _aa->GetSecondaryPixel() : _aa->GetPrimaryPixel());
			attributes = 0x0;
			if (_aa->IsFColorDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeFInvert) == 0x0)
				attributes |= PixelAttributes::AttributeFInvert;
			if (_aa->IsBColorDrawn() && (drawingPixel.Attributes & PixelAttributes::AttributeBInvert) == 0x0)
				attributes |= PixelAttributes::AttributeBInvert;

			drawingPixel.Char = _aa->GetImage()->GetBackground().Char;
			drawingPixel.Color = _aa->GetImage()->GetBackground().Color;
			drawingPixel.Attributes = attributes;
			_drawingBuffer->GetFrame(3).DrawLine(_startPoint, endPoint - _startPoint, drawingPixel, false, attributes, PixelOptions::OptionCopySpecial);
		}
	}

	if (_drawing) {
		Coord startPoint = _startPoint;
		Coord endPoint = _aa->GetCanvas()->GetImageMousePosition();
		if (endPoint.X < _startPoint.X)
			endPoint.X -= 1;
		else
			endPoint.X += 1;
		if (endPoint.Y < _startPoint.Y)
			endPoint.Y -= 1;
		else
			endPoint.Y += 1;
		Coord size = endPoint - startPoint;
		FixRect(startPoint, size);

		_aa->GetStatusBar()->SetSizeCoord(size);
	}
	else {
		_aa->GetStatusBar()->SetSizeCoord(-Coord::One);
	}
}
void PowerConsole::AsciiArtistApp::ToolLine::Activate() {
	
}
void PowerConsole::AsciiArtistApp::ToolLine::Finalize() {
	if (_drawing) {
		UndoAction* undo = new UndoAction("Line", Actions::Draw, _aa->GetCurrentFrame());
		if (_aa->IsUndoEnabled())
			undo->SetBeforeImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));

		_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).DrawImage(Coord::Zero, _drawingBuffer->GetFrame(1), PixelAttributes::AttributeFill, PixelOptions::OptionInverse);
		_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).DrawImage(Coord::Zero, _drawingBuffer->GetFrame(2), PixelAttributes::AttributeInvert, PixelOptions::OptionCopySpecial);
		_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).DrawImage(Coord::Zero, _drawingBuffer->GetFrame(3), PixelAttributes::AttributeInvert, PixelOptions::OptionCopySpecial | PixelOptions::OptionInverse);
		_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).DrawImage(Coord::Zero, _drawingBuffer->GetFrame(0), PixelAttributes::AttributeFill, PixelOptions::OptionCopySpecial);

		if (_aa->IsUndoEnabled())
			undo->SetResultImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));
		_aa->AddUndo(undo);
		
		_drawing = false;
	}
}
void PowerConsole::AsciiArtistApp::ToolLine::Cancel() {
	if (_drawing) {
		_drawing = false;
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::AsciiArtistApp::ToolLine::Draw(AsciiImageBase& g) {

	if (_drawing) {
		_drawingBuffer->GetFrame(4).DrawImage(Coord::Zero, _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

		_drawingBuffer->GetFrame(4).DrawImage(Coord::Zero,
			_drawingBuffer->GetFrame(1), PixelAttributes::AttributeFill, PixelOptions::OptionInverse);
		_drawingBuffer->GetFrame(4).DrawImage(Coord::Zero,
			_drawingBuffer->GetFrame(2), PixelAttributes::AttributeInvert, PixelOptions::OptionCopySpecial);
		_drawingBuffer->GetFrame(4).DrawImage(Coord::Zero,
			_drawingBuffer->GetFrame(3), PixelAttributes::AttributeInvert, PixelOptions::OptionCopySpecial | PixelOptions::OptionInverse);
		_drawingBuffer->GetFrame(4).DrawImage(Coord::Zero,
			_drawingBuffer->GetFrame(0), PixelAttributes::AttributeFill);
	}
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|