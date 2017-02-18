/*=================================================================|
|* File:				ToolText.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/19/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "ToolText.h"
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
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ToolText
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::AsciiArtistApp::ToolText::ToolText() : Tool(ToolTypes::Text, "Text") {
	this->_secondary		= false;
	this->_startPoint		= Coord::Zero;

	this->_text				= "";
	this->_cursorPos		= 0;

	this->_cursorTimer		= 0;
	this->_cursorState		= true;
}
PowerConsole::AsciiArtistApp::ToolText::~ToolText() {
	
}
void PowerConsole::AsciiArtistApp::ToolText::Initialize(AsciiArtistManager* aa, ConsoleApp* app, AsciiAnimation* drawingBuffer) {
	Tool::Initialize(aa, app, drawingBuffer);
}
void PowerConsole::AsciiArtistApp::ToolText::Uninitialize() {
	Tool::Uninitialize();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Information


#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::AsciiArtistApp::ToolText::Update() {
	
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
			_cursorPos = 0;
			_cursorState = true;
			_cursorTimer = clock();
		}
	}
	else {
		unsigned int time = clock();
		if (time - _cursorTimer >= 560) {
			_cursorTimer = time;
			_cursorState = !_cursorState;
		}
		
		Coord mousePos = _aa->GetCanvas()->GetImageMousePosition();
		bool pressed = _app->GetMouse()->IsButtonPressed(MouseButtons::Left);

		if (SmallRect(_startPoint, Coord(_text.length() + 1, 1)).ContainsPoint(mousePos) && pressed) {
			_cursorPos = mousePos.X - _startPoint.X;
			_cursorTimer = clock();
			_cursorState = true;
		}
		else if (SmallRect(_aa->GetCanvas()->GetPosition(), _aa->GetCanvas()->GetSize()).ContainsPoint(_app->GetMouse()->GetPosition()) && pressed) {
			Finalize();
		}
		else if (_app->GetKeyboard()->IsKeyPressed(Keys::Enter)) {
			Finalize();
		}
		else if (_app->GetKeyboard()->IsKeyTyped(Keys::Left)) {
			if (_cursorPos > 0) {
				_cursorTimer = clock();
				_cursorState = true;

				_cursorPos--;
			}
		}
		else if (_app->GetKeyboard()->IsKeyTyped(Keys::Right)) {
			if (_cursorPos < (int)_text.length()) {
				_cursorTimer = clock();
				_cursorState = true;

				_cursorPos++;
			}
		}
		else if (_app->GetKeyboard()->IsKeyTyped(Keys::Back)) {
			if (_cursorPos > 0) {
				_cursorTimer = clock();
				_cursorState = true;

				_text.erase(_cursorPos - 1, 1);
				_cursorPos--;
			}
		}
		else if (_app->GetKeyboard()->IsCharTyped()) {
			if ((int)_text.length() < _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).Size().X - _startPoint.X) {
				_cursorTimer = clock();
				_cursorState = true;
				unsigned char c = (unsigned char)_app->GetKeyboard()->GetCharTyped();

				if (c >= 32 && c <= 126) {
					if (_cursorPos == (int)_text.length()) {
						_text += c;
					}
					else {
						_text.insert(_cursorPos, 1, c);
					}
				}
				_cursorPos++;
			}
		}
	}

	_aa->GetStatusBar()->SetSizeCoord(-Coord::One);
}
void PowerConsole::AsciiArtistApp::ToolText::Activate() {
	
}
void PowerConsole::AsciiArtistApp::ToolText::Finalize() {
	if (_drawing) {
		if (_text.empty()) {
			Cancel();
			return;
		}
		UndoAction* undo = new UndoAction("Text", Actions::Draw, _aa->GetCurrentFrame());
		if (_aa->IsUndoEnabled())
			undo->SetBeforeImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));
		
		Pixel drawingPixel = (_secondary ? _aa->GetSecondaryPixel() : _aa->GetPrimaryPixel());
		_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).DrawString(_startPoint, _text, drawingPixel, Alignment::Left, PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		
		if (_aa->IsUndoEnabled())
			undo->SetResultImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));
		_aa->AddUndo(undo);
		
		_drawing = false;
		_cursorPos = 0;
		_cursorState = true;
		_text = "";
	}
}
void PowerConsole::AsciiArtistApp::ToolText::Cancel() {
	if (_drawing) {
		_drawing = false;
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::AsciiArtistApp::ToolText::Draw(AsciiImageBase& g) {
	if (_drawing) {
		_drawingBuffer->GetFrame(4).DrawImage(Coord::Zero, _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

		Pixel drawingPixel = (_secondary ? _aa->GetSecondaryPixel() : _aa->GetPrimaryPixel());
		_drawingBuffer->GetFrame(4).DrawString(_startPoint, _text, drawingPixel, Alignment::Left,
			PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

		if (_cursorState) {
			if ((int)_text.length() >= _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).Size().X - _startPoint.X) {
				_drawingBuffer->GetFrame(4).SetSpecial(_startPoint + Coord(_text.length() - 1, 0), PixelAttributes::AttributeInvert);
			}
			else {
				_drawingBuffer->GetFrame(4).SetSpecial(_startPoint + Coord(_cursorPos, 0), PixelAttributes::AttributeInvert);
			}
		}
	}
}

#pragma endregion
//=========== ACTIONS ============
#pragma region Actions

void PowerConsole::AsciiArtistApp::ToolText::AddCharacter(unsigned char character) {
	if ((int)_text.length() < _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).Size().X - _startPoint.X) {
		_cursorTimer = clock();
		_cursorState = true;

		if (_cursorPos == (int)_text.length()) {
			_text += character;
		}
		else {
			_text.insert(_cursorPos, 1, character);
		}
		_cursorPos++;
	}
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|