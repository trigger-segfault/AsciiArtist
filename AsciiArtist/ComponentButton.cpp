/*=================================================================|
|* File:				ComponentButton.cpp						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentButton.h"
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

PowerConsole::Components::ComponentButton::ComponentButton() : Component() {
	this->_icons			= nullptr;
	this->_text				= "";
	this->_spacing			= Coord::Zero;
	this->_togglable		= false;

	this->_state			= ButtonState::None;
	this->_toggleState		= false;
}
PowerConsole::Components::ComponentButton::~ComponentButton() {
	this->_icons			= nullptr;
}
void PowerConsole::Components::ComponentButton::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentButton::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentButton::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentButton::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

AsciiAnimation* PowerConsole::Components::ComponentButton::GetIcons() const {
	return _icons;
}
void PowerConsole::Components::ComponentButton::SetIcons(AsciiAnimation* icons) {
	this->_icons = icons;
}
string PowerConsole::Components::ComponentButton::GetText() const {
	return _text;
}
void PowerConsole::Components::ComponentButton::SetText(const string& text) {
	this->_text = text;
}
Coord PowerConsole::Components::ComponentButton::GetSpacing() const {
	return _spacing;
}
void PowerConsole::Components::ComponentButton::SetSpacing(Coord spacing) {
	this->_spacing = spacing;
}
bool PowerConsole::Components::ComponentButton::IsTogglable() const {
	return _togglable;
}
void PowerConsole::Components::ComponentButton::SetTogglable(bool togglable) {
	this->_togglable = togglable;
}
void PowerConsole::Components::ComponentButton::SetToggled(bool toggled) {
	this->_toggleState = toggled;
}
	
#pragma endregion
//--------------------------------
#pragma region Events

ButtonState PowerConsole::Components::ComponentButton::GetButtonState() const {
	return _state;
}
bool PowerConsole::Components::ComponentButton::IsButtonHovering() const {
	return (_state == ButtonState::Hovering);
}
bool PowerConsole::Components::ComponentButton::IsButtonPressed() const {
	return (_state == ButtonState::Pressed);
}
bool PowerConsole::Components::ComponentButton::IsButtonDown() const {
	return (_state == ButtonState::Pressed || _state == ButtonState::Down);
}
bool PowerConsole::Components::ComponentButton::IsButtonReleased() const {
	return (_state == ButtonState::Released);
}
bool PowerConsole::Components::ComponentButton::IsToggled() const {
	return _toggleState;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentButton::Update() {
	bool hovering = GetBounds().ContainsPoint(_app->GetMouse()->GetPosition());
	bool down = _app->GetMouse()->IsButtonDown(MouseButtons::Left);

	switch (_state) {
	case ButtonState::None:
	case ButtonState::Hovering:
	case ButtonState::Released:
		if (hovering) {
			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				_state = ButtonState::Pressed;
				if (_togglable)
					_toggleState = !_toggleState;
			}
			else {
				_state = ButtonState::Hovering;
			}
		}
		else {
			_state = ButtonState::None;
		}
		break;

	case ButtonState::Pressed:
	case ButtonState::Down:
		if (down)
			_state = ButtonState::Down;
		else
			_state = ButtonState::Released;
		break;
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentButton::Draw(AsciiImageBase& g) {

	if (_toggleState) {
		if (_icons != nullptr) {
			if (_icons->NumFrames() > 3)
				g.DrawImage(_position + _spacing, _icons->GetFrame(3));
			else if (_icons->NumFrames() > 2)
				g.DrawImage(_position + _spacing, _icons->GetFrame(2));
			else if (_icons->NumFrames() > 1)
				g.DrawImage(_position + _spacing, _icons->GetFrame(1));
			else
				g.DrawImage(_position + _spacing, _icons->GetFrame(0));
		}
		else {
			g.DrawString(_position + _spacing, _text, Pixel(' ', 0x9F));
		}
	}
	else {
		switch (_state) {
		case ButtonState::None:
			if (_icons != nullptr) {
				g.DrawImage(_position + _spacing, _icons->GetFrame(0));
			}
			else {
				g.DrawString(_position + _spacing, _text, Pixel(' ', 0x07));
			}
			break;

		case ButtonState::Hovering:
			if (_icons != nullptr) {
				if (_icons->NumFrames() > 1)
					g.DrawImage(_position + _spacing, _icons->GetFrame(1));
				else
					g.DrawImage(_position + _spacing, _icons->GetFrame(0));
			}
			else {
				g.DrawString(_position + _spacing, _text, Pixel(' ', 0x0F));
			}
			break;

		case ButtonState::Pressed:
		case ButtonState::Down:
		case ButtonState::Released:
			if (_icons != nullptr) {
				if (_icons->NumFrames() > 2)
					g.DrawImage(_position + _spacing, _icons->GetFrame(2));
				else if (_icons->NumFrames() > 1)
					g.DrawImage(_position + _spacing, _icons->GetFrame(1));
				else
					g.DrawImage(_position + _spacing, _icons->GetFrame(0));
			}
			else {
				g.DrawString(_position + _spacing, _text, Pixel(' ', 0x0F));
			}
			break;
		}
	}

}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|