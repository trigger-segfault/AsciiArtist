/*=================================================================|
|* File:				ComponentCharacterGrid.cpp				  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentCharacterGrid.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "AsciiImage.h"
#include "AsciiArtistManager.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Components;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ComponentButton
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentCharacterGrid::ComponentCharacterGrid() : Component() {
	this->_aa					= nullptr;

	this->_charList				= new Coord[257];
	this->_charGridImage		= nullptr;
	this->_primaryCharacter		= Pixel('A', 0x00, PixelAttributes::AttributeChar);
	this->_secondaryCharacter	= Pixel(' ', 0x00, PixelAttributes::AttributeChar);
	this->_primarySelection		= -Coord::One;
	this->_secondarySelection	= -Coord::One;
	this->_primaryChanged		= false;
	this->_secondaryChanged		= false;
	this->_hoverPosition		= -Coord::One;
}
PowerConsole::Components::ComponentCharacterGrid::~ComponentCharacterGrid() {
	delete[] this->_charList;
	this->_charList				= nullptr;
}
void PowerConsole::Components::ComponentCharacterGrid::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentCharacterGrid::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentCharacterGrid::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentCharacterGrid::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

void PowerConsole::Components::ComponentCharacterGrid::SetAsciiArtistManager(AsciiArtistManager* aa) {
	this->_aa = aa;
}

#pragma endregion
//--------------------------------
#pragma region Visual

AsciiImage* PowerConsole::Components::ComponentCharacterGrid::GetCharacterGridImage() const {
	return _charGridImage;
}
void PowerConsole::Components::ComponentCharacterGrid::SetCharacterGridImage(AsciiImage* image) {
	this->_charGridImage = image;


	for (int i = 0; i < 257; i++) {
		_charList[i] = -Coord::One;
	}

	for (int x = 0; x < image->Size().X; x++) {
		for (int y = 0; y < image->Size().Y; y++) {
			if (image->GetColor(Coord(x, y)) == 0x07) {
				if (_charList[image->GetChar(Coord(x, y))] == -Coord::One) {
					_charList[image->GetChar(Coord(x, y))] = Coord(x, y);
				}
			}
			else if (image->GetColor(Coord(x, y)) == 0x03) {
				if (_charList[256] == -Coord::One)
					_charList[256] = Coord(x, y);
			}
		}
	}
	
	if (_charList[0] == -Coord::One) {
		_charList[0] = _charList[' '];
	}
	if (_charList[255] == -Coord::One) {
		_charList[255] = _charList[' '];
	}

	if ((_primaryCharacter.Attributes & PixelAttributes::AttributeChar) == 0x0)
		_primarySelection	= _charList[256];
	else
		_primarySelection	= _charList[_primaryCharacter.Char];
	if ((_secondaryCharacter.Attributes & PixelAttributes::AttributeChar) == 0x0)
		_secondarySelection	= _charList[256];
	else
		_secondarySelection	= _charList[_secondaryCharacter.Char];
}
Pixel PowerConsole::Components::ComponentCharacterGrid::GetPrimaryCharacter() const {
	return _primaryCharacter;
}
void PowerConsole::Components::ComponentCharacterGrid::SetPrimaryCharacter(Pixel character) {
	this->_primaryCharacter = character;
	_primarySelection = _charList[character.Char];
	if ((character.Attributes & PixelAttributes::AttributeChar) == 0x0) {
		_primaryCharacter.Char = ' ';
		_primaryCharacter.Color = 0x00;
		_primarySelection = _charList[256];
	}
}
Pixel PowerConsole::Components::ComponentCharacterGrid::GetSecondaryCharacter() const {
	return _secondaryCharacter;
}
void PowerConsole::Components::ComponentCharacterGrid::SetSecondaryCharacter(Pixel character) {
	this->_secondaryCharacter = character;
	_secondarySelection = _charList[character.Char];
	if ((character.Attributes & PixelAttributes::AttributeChar) == 0x0) {
		_secondaryCharacter.Char = ' ';
		_secondaryCharacter.Color = 0x00;
		_secondarySelection = _charList[256];
	}
}
	
#pragma endregion
//--------------------------------
#pragma region Events

bool PowerConsole::Components::ComponentCharacterGrid::IsPrimaryCharacterChanged() const {
	return _primaryChanged;
}
bool PowerConsole::Components::ComponentCharacterGrid::IsSecondaryCharacterChanged() const {
	return _secondaryChanged;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentCharacterGrid::Update() {
	
	Coord mousePos = _app->GetMouse()->GetPosition();
	_primaryChanged = false;
	_secondaryChanged = false;
	_hoverPosition = -Coord::One;

	if (SmallRect(_position, _charGridImage->Size()).ContainsPoint(mousePos) &&
		SmallRect(_position, _size).ContainsPoint(mousePos)) {
		mousePos -= _position;
		if (_charGridImage->GetColor(mousePos) == 0x07 || _charGridImage->GetColor(mousePos) == 0x03) {
			_hoverPosition = mousePos;

			//unsigned char hoverChar = _charGridImage->GetChar(mousePos);
			bool hoverTransparent = (_charGridImage->GetColor(mousePos) == 0x03);
			Pixel hoverChar = Pixel(_charGridImage->GetChar(mousePos), 0x00, PixelAttributes::AttributeChar);
			if (_charGridImage->GetColor(mousePos) == 0x03) {
				hoverChar.Char = ' ';
				hoverChar.Color = 0x00;
				hoverChar.Attributes = PixelAttributes::AttributeNone;
			}
			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				if (!_aa->GetCurrentTool()->IsDrawing()) {
					_primaryChanged = (hoverChar != _primaryCharacter);
					_primaryCharacter = hoverChar;
					_primarySelection = mousePos;
					if (_charGridImage->GetColor(mousePos) == 0x03)
						_primaryCharacter.Attributes = PixelAttributes::AttributeNone;
				}
				else if (_aa->GetCurrentToolType() == ToolTypes::Text && _charGridImage->GetColor(mousePos) == 0x07) {
					_aa->GetTextTool()->AddCharacter(hoverChar.Char);
				}
			}
			else if (_app->GetMouse()->IsButtonPressed(MouseButtons::Right)) {
				if (!_aa->GetCurrentTool()->IsDrawing()) {
					_secondaryChanged = (hoverChar != _secondaryCharacter);
					_secondaryCharacter = hoverChar;
					_secondarySelection = mousePos;
					if (_charGridImage->GetColor(mousePos) == 0x03)
						_secondaryCharacter.Attributes = PixelAttributes::AttributeNone;
				}
			}
		}
	}
	if (!_aa->GetCurrentTool()->IsDrawing()) {
		Coord newSelection = _primarySelection;

		if (_app->GetKeyboard()->IsKeyPressed(Keys::A))
			newSelection -= Coord(1, 0);
		else if (_app->GetKeyboard()->IsKeyPressed(Keys::D))
			newSelection += Coord(1, 0);
		else if (_app->GetKeyboard()->IsKeyPressed(Keys::W))
			newSelection -= Coord(0, 1);
		else if (_app->GetKeyboard()->IsKeyPressed(Keys::S))
			newSelection += Coord(0, 1);

		if (newSelection != _primarySelection) {
			Pixel pixel = _charGridImage->GetPixel(newSelection);
			if (pixel.Color != 0x07 && pixel.Color != 0x03) {
				newSelection += (newSelection - _primarySelection);
				pixel = _charGridImage->GetPixel(newSelection);
			}

			if ((pixel.Color == 0x07 || pixel.Color == 0x03) && newSelection >= Coord::Zero && newSelection < _charGridImage->Size()) {

				if (pixel.Color == 0x03) {
					_primaryCharacter.Char = ' ';
					_primaryCharacter.Attributes = PixelAttributes::AttributeNone;
				}
				else {
					_primaryCharacter.Char = pixel.Char;
					_primaryCharacter.Attributes = PixelAttributes::AttributeChar;
				}
				_primaryChanged = true;
				_primarySelection = newSelection;
			}
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentCharacterGrid::Draw(AsciiImageBase& g) {

	g.DrawImage(_position, Coord::Zero, _size, *_charGridImage);

	if (SmallRect(Coord::Zero, _size).ContainsPoint(_hoverPosition)) {
		if (_charGridImage->GetColor(_hoverPosition) == 0x03)
			g.SetColor(_position + _hoverPosition, 0x0B);
		else
			g.SetColor(_position + _hoverPosition, 0x0F);
	}
	if (SmallRect(Coord::Zero, _size).ContainsPoint(_primarySelection)) {
		if ((_primaryCharacter.Attributes & PixelAttributes::AttributeChar) == 0x0)
			g.SetColor(_position + _primarySelection, 0x1B);
		else
			g.SetColor(_position + _primarySelection, 0x9F);
	}
	if (SmallRect(Coord::Zero, _size).ContainsPoint(_secondarySelection)) {
		if ((_secondaryCharacter.Attributes & PixelAttributes::AttributeChar) == 0x0)
			g.SetColor(_position + _secondarySelection, 0x1B);
		else
			g.SetColor(_position + _secondarySelection, 0x9F);
	}

}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|