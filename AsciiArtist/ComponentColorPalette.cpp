/*=================================================================|
|* File:				ComponentColorPalette.cpp				  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/12/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentColorPalette.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "AsciiImage.h"

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

PowerConsole::Components::ComponentColorPalette::ComponentColorPalette() : Component() {
	this->_colorPaletteImage	= nullptr;
	this->_colorSelection		= Pixel();
	this->_colorIndex			= -1;
}
PowerConsole::Components::ComponentColorPalette::~ComponentColorPalette() {
	
}
void PowerConsole::Components::ComponentColorPalette::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentColorPalette::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentColorPalette::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentColorPalette::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

AsciiImage* PowerConsole::Components::ComponentColorPalette::GetColorPaletteImage() const {
	return _colorPaletteImage;
}
void PowerConsole::Components::ComponentColorPalette::SetColorPaletteImage(AsciiImage* image) {
	this->_colorPaletteImage = image;
}
Pixel PowerConsole::Components::ComponentColorPalette::GetNewColor() const {
	return _colorSelection;
}
	
#pragma endregion
//--------------------------------
#pragma region Events

bool PowerConsole::Components::ComponentColorPalette::IsPrimaryForegroundColorChanged() const {
	return (_colorIndex == 0);
}
bool PowerConsole::Components::ComponentColorPalette::IsPrimaryBackgroundColorChanged() const {
	return (_colorIndex == 1);
}
bool PowerConsole::Components::ComponentColorPalette::IsSecondaryForegroundColorChanged() const {
	return (_colorIndex == 2);
}
bool PowerConsole::Components::ComponentColorPalette::IsSecondaryBackgroundColorChanged() const {
	return (_colorIndex == 3);
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentColorPalette::Update() {
	
	Coord mousePos = _app->GetMouse()->GetPosition();
	_colorIndex = -1;
	_colorSelection = Pixel();
	bool background = false;

	if (SmallRect(_position, _colorPaletteImage->Size()).ContainsPoint(mousePos) &&
		SmallRect(_position, _size).ContainsPoint(mousePos)) {
		unsigned char c = _colorPaletteImage->GetChar(mousePos - _position);
		if (c == unsigned char(219) || c == unsigned char(176) || c == unsigned char(178)) {
			mousePos -= _position;

			Pixel selection = _colorPaletteImage->GetPixel(mousePos);
		
			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				_colorIndex = 0;
			}
			else if (_app->GetMouse()->IsButtonPressed(MouseButtons::Right)) {
				_colorIndex = 1;
				background = true;
			}
			if (_app->GetKeyboard()->IsKeyDown(Keys::LShiftKey) && _colorIndex != -1) {
				_colorIndex += 2;
			}

			if (selection.Char == unsigned char(219)) {
				_colorSelection.Color = (background ? selection.Color << 4 : selection.Color);
				_colorSelection.Attributes = (background ? PixelAttributes::AttributeBColor : PixelAttributes::AttributeFColor);
			}
			else if (selection.Char == unsigned char(178)) {
				_colorSelection.Color = 0x00;
				_colorSelection.Attributes = PixelAttributes::AttributeNone;
			}
			else if (selection.Char == unsigned char(176)) {
				_colorSelection.Color = 0x00;
				_colorSelection.Attributes = (background ? PixelAttributes::AttributeBInvert : PixelAttributes::AttributeFInvert);
			}
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentColorPalette::Draw(AsciiImageBase& g) {

	g.DrawImage(_position, Coord::Zero, _size, *_colorPaletteImage);

}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|