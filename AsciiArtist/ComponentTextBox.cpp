/*=================================================================|
|* File:				ComponentTextBox.cpp					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/16/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentTextBox.h"
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
#pragma region ComponentTextBox
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentTextBox::ComponentTextBox() : Component() {
	this->_text				= "";
	this->_textTyping		= "";
	this->_typing			= false;
	this->_cursorPos		= 0;
	this->_indent			= Coord::Zero;
	this->_hovering			= false;
	this->_finishedTyping	= false;

	this->_cursorTimer		= 0;
	this->_cursorState		= true;
}
PowerConsole::Components::ComponentTextBox::~ComponentTextBox() {
	
}
void PowerConsole::Components::ComponentTextBox::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentTextBox::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentTextBox::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentTextBox::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual
	
string PowerConsole::Components::ComponentTextBox::GetText() const {
	return _text;
}
void PowerConsole::Components::ComponentTextBox::SetText(const string& text) {
	this->_text = text;
	if (_typing) {
		_textTyping = text;
		_cursorPos = (int)text.length();
		_cursorTimer = clock();
		_cursorState = true;
	}
}
	
Coord PowerConsole::Components::ComponentTextBox::GetIndent() const {
	return _indent;
}
void PowerConsole::Components::ComponentTextBox::SetIndent(Coord indent) {
	this->_indent = indent;
}

bool PowerConsole::Components::ComponentTextBox::IsTyping() const {
	return _typing;
}
void PowerConsole::Components::ComponentTextBox::SetTyping(bool typing) {
	if (this->_typing != typing) {
		this->_typing = typing;
		if (typing) {
			_textTyping = _text;
			_cursorPos = _textTyping.length();
			_cursorTimer = clock();
			_cursorState = true;
		}
		else {
			_text = _textTyping;
			_textTyping = "";
			_finishedTyping = true;
		}
	}
}

#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentTextBox::IsFinishedTyping() const {
	return _finishedTyping;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentTextBox::Update() {

	Coord mousePos = _app->GetMouse()->GetPosition();
	bool pressed = _app->GetMouse()->IsButtonPressed(MouseButtons::Left);
	bool inside = SmallRect(_position, _size).ContainsPoint(mousePos);
	bool hovering = false;

	_finishedTyping = false;

	if (!_typing) {
		if (inside) {
			hovering = true;
			if (pressed) {
				_typing = true;
				_textTyping = _text;
				_cursorPos = _textTyping.length();
				_cursorTimer = clock();
				_cursorState = true;
			}
		}
	}
	else {
		unsigned int time = clock();
		if (time - _cursorTimer >= 560) {
			_cursorTimer = time;
			_cursorState = !_cursorState;
		}

		if (SmallRect(_position + _indent, Coord(_textTyping.length() + 1, 1)).ContainsPoint(mousePos) && pressed) {
			_cursorPos = mousePos.X - _indent.X - _position.X;
			_cursorTimer = clock();
			_cursorState = true;
		}
		if (_app->GetKeyboard()->IsKeyTyped(Keys::Left)) {
			if (_cursorPos > 0) {
				_cursorTimer = clock();
				_cursorState = true;

				_cursorPos--;
			}
		}
		else if (_app->GetKeyboard()->IsKeyTyped(Keys::Right)) {
			if (_cursorPos < (int)_textTyping.length()) {
				_cursorTimer = clock();
				_cursorState = true;

				_cursorPos++;
			}
		}
		else if (_app->GetKeyboard()->IsKeyTyped(Keys::Back)) {
			if (_cursorPos > 0) {
				_cursorTimer = clock();
				_cursorState = true;

				_textTyping.erase(_cursorPos - 1, 1);
				_cursorPos--;
			}
		}
		else if (_app->GetKeyboard()->IsKeyTyped(Keys::Enter) || (!inside && pressed)) {
			_typing = false;
			_text = _textTyping;
			_textTyping = "";
			_finishedTyping = true;
		}
		else if (_app->GetKeyboard()->IsKeyTyped(Keys::Escape)) {
			_typing = false;
			_textTyping = "";
			_finishedTyping = true;
		}
		else if (_app->GetKeyboard()->IsCharTyped()) {
			if ((int)_textTyping.length() < _size.X - (_indent.X * 2)) {
				_cursorTimer = clock();
				_cursorState = true;
				unsigned char c = (unsigned char)_app->GetKeyboard()->GetCharTyped();

				if (c >= 32 && c <= 126) {
					if (_cursorPos == (int)_textTyping.length()) {
						_textTyping += c;
					}
					else {
						_textTyping.insert(_cursorPos, 1, c);
					}
				}
				_cursorPos++;
			}
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentTextBox::Draw(AsciiImageBase& g) {

	g.DrawString(_position + _indent, (_typing ? _textTyping : _text), Pixel(' ', (_hovering ? 0x0F : 0x07)));

	if (_typing && _cursorState) {
		if ((int)_textTyping.length() >= _size.X - (_indent.X * 2)) {
			g.SetSpecial(_position + _indent + Coord(_textTyping.length() - 1, 0), PixelAttributes::AttributeInvert);
		}
		else {
			g.SetSpecial(_position + _indent + Coord(_cursorPos, 0), PixelAttributes::AttributeInvert);
		}
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|