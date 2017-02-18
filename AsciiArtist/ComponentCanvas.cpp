/*=================================================================|
|* File:				ComponentCanvas.cpp						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/14/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentCanvas.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "AsciiImage.h"
#include "AsciiArtistManager.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::AsciiArtistApp;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Components;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ComponentCanvas
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentCanvas::ComponentCanvas() : Component() {
	this->_aa				= nullptr;
	this->_image			= nullptr;
	this->_scrollBar		= nullptr;
	this->_frameIndex		= 0;
	this->_hoverFrame		= -1;
	this->_insideCanvas		= false;
	this->_mouseImagePos	= Coord::Zero;
}
PowerConsole::Components::ComponentCanvas::~ComponentCanvas() {
	
}
void PowerConsole::Components::ComponentCanvas::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentCanvas::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentCanvas::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentCanvas::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

void PowerConsole::Components::ComponentCanvas::SetScrollBar(ComponentScrollBar* scrollBar) {
	this->_scrollBar = scrollBar;
}
void PowerConsole::Components::ComponentCanvas::SetImage(AsciiAnimation* image) {
	this->_image = image;
}
void PowerConsole::Components::ComponentCanvas::SetFrame(int index) {
	this->_frameIndex = index;
}
void PowerConsole::Components::ComponentCanvas::SetHoverFrame(int hoverFrame) {
	this->_hoverFrame = hoverFrame;
}
Coord PowerConsole::Components::ComponentCanvas::GetDrawSize() const {
	Coord size = _image->GetFrame(_frameIndex).Size() - _scrollBar->GetListPosition();
	if (_size.X < size.X)
		size.X = _size.X;
	if (_size.Y < size.Y)
		size.Y = _size.Y;
	return size;
}
Coord PowerConsole::Components::ComponentCanvas::GetImagePosition() const {
	return _scrollBar->GetListPosition();
}
SmallRect PowerConsole::Components::ComponentCanvas::GetImageBounds() const {
	return SmallRect(_scrollBar->GetListPosition(), _size);
}
Coord PowerConsole::Components::ComponentCanvas::GetImageMousePosition() const {
	return _mouseImagePos;
}
void PowerConsole::Components::ComponentCanvas::SetAsciiArtistManager(AsciiArtistManager* aa) {
	this->_aa = aa;
}
	
#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentCanvas::IsMouseInsideCanvas() const {
	return _insideCanvas;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentCanvas::Update() {
	_hoverFrame = -1;
	_insideCanvas = false;

	Coord mousePos = _app->GetMouse()->GetPosition();
	_mouseImagePos = mousePos - _position + _scrollBar->GetListPosition();

	if (SmallRect(_position, _size).ContainsPoint(mousePos) &&
		SmallRect(_position, _image->GetFrame(_frameIndex).Size()).ContainsPoint(mousePos)) {
		_insideCanvas = true;

		_aa->GetStatusBar()->SetPointCoord(_mouseImagePos);
	}
	else {
		_aa->GetStatusBar()->SetPointCoord(-Coord::One);
	}

	_aa->GetCurrentTool()->Update();
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentCanvas::Draw(AsciiImageBase& g) {
	
	if (_hoverFrame == -1) {
		_aa->GetCurrentTool()->Draw(g);
	}

	Coord canvasSize = _size;
	if (_image->GetFrame(_frameIndex).Size().X < canvasSize.X)
		canvasSize.X = _image->GetFrame(_frameIndex).Size().X;
	if (_image->GetFrame(_frameIndex).Size().Y < canvasSize.Y)
		canvasSize.Y = _image->GetFrame(_frameIndex).Size().Y;

	Pixel background = _image->GetBackground();
	background.Attributes |= PixelAttributes::AttributeFill;
	g.SetPixel(_position, canvasSize, background, PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

	if (!_aa->GetCurrentTool()->IsDrawing() || _hoverFrame != -1) {
		g.DrawImage(_position, _scrollBar->GetListPosition(), _size, _image->GetFrame(_hoverFrame != -1 ? _hoverFrame : _frameIndex));
	}
	else {
		g.DrawImage(_position, _scrollBar->GetListPosition(), GetDrawSize(), _aa->GetDrawingBuffer()->GetFrame(4));
	}

	if (_size.X > _image->GetFrame(_frameIndex).Size().X) {
		g.SetPixel(_position + Coord(_image->GetFrame(_frameIndex).Size().X, 0), Coord(1, min(_size.Y, _image->GetFrame(_frameIndex).Size().Y)), Pixel(179, 0x08));
		if (g.GetChar(_position + Coord(_image->GetFrame(_frameIndex).Size().X, -1)) == unsigned char(205))
			g.SetChar(_position + Coord(_image->GetFrame(_frameIndex).Size().X, -1), 209);
		else if (g.GetChar(_position + Coord(_image->GetFrame(_frameIndex).Size().X, -1)) == unsigned char(207))
			g.SetChar(_position + Coord(_image->GetFrame(_frameIndex).Size().X, -1), 216);
		if (_size.Y <= _image->GetFrame(_frameIndex).Size().Y) {
			if (g.GetChar(_position + Coord(_image->GetFrame(_frameIndex).Size().X, _size.Y)) == unsigned char(205))
				g.SetChar(_position + Coord(_image->GetFrame(_frameIndex).Size().X, _size.Y), 207);
			else if (g.GetChar(_position + Coord(_image->GetFrame(_frameIndex).Size().X, _size.Y)) == unsigned char(209))
				g.SetChar(_position + Coord(_image->GetFrame(_frameIndex).Size().X, _size.Y), 216);
		}
	}
	if (_size.Y > _image->GetFrame(_frameIndex).Size().Y) {
		g.SetPixel(_position + Coord(0, _image->GetFrame(_frameIndex).Size().Y), Coord(min(_size.X, _image->GetFrame(_frameIndex).Size().X), 1), Pixel(196, 0x08));
		if (g.GetChar(_position + Coord(-1, _image->GetFrame(_frameIndex).Size().Y)) == unsigned char(186))
			g.SetChar(_position + Coord(-1, _image->GetFrame(_frameIndex).Size().Y), 199);
		if (g.GetChar(_position + Coord(-1, _image->GetFrame(_frameIndex).Size().Y)) == unsigned char(182))
			g.SetChar(_position + Coord(-1, _image->GetFrame(_frameIndex).Size().Y), 215);
		if (_size.X <= _image->GetFrame(_frameIndex).Size().X) {
			if (g.GetChar(_position + Coord(_size.X, _image->GetFrame(_frameIndex).Size().Y)) == unsigned char(186))
				g.SetChar(_position + Coord(_size.X, _image->GetFrame(_frameIndex).Size().Y), 182);
			else if (g.GetChar(_position + Coord(_size.X, _image->GetFrame(_frameIndex).Size().Y)) == unsigned char(199))
				g.SetChar(_position + Coord(_size.X, _image->GetFrame(_frameIndex).Size().Y), 215);
		}
	}
	if (_size.X > _image->GetFrame(_frameIndex).Size().X && _size.Y > _image->GetFrame(_frameIndex).Size().Y) {
		g.SetPixel(_position + _image->GetFrame(_frameIndex).Size(), Pixel(217, 0x08));
	}

	if (_insideCanvas && _aa->IsCursorVisible()) {
		g.SetSpecial(_position + _mouseImagePos - _scrollBar->GetListPosition(), PixelAttributes::AttributeInvert);
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|