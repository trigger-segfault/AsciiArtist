/*=================================================================|
|* File:				ComponentAnimationSpeedDialog.cpp		  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/19/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentAnimationSpeedDialog.h"
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
#pragma region ComponentResizeWindow
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentAnimationSpeedDialog::ComponentAnimationSpeedDialog() : ComponentWindow() {
	this->_id					= "AnimationSpeedDialog";
	this->_size					= Coord(29, 11);

	this->_backgroundImage		= nullptr;

	this->_animSpeedTextBox		= nullptr;
	this->_setButton			= nullptr;
	this->_cancelButton			= nullptr;

	this->_animSpeed			= 0;

	this->_speedSet				= false;
}
PowerConsole::Components::ComponentAnimationSpeedDialog::~ComponentAnimationSpeedDialog() {
	
}
void PowerConsole::Components::ComponentAnimationSpeedDialog::Initialize(ConsoleApp* app, Frame* frame) {
	ComponentWindow::Initialize(app, frame);

	// Animation Speed
	this->_animSpeedTextBox			= new ComponentTextBox();
	this->_animSpeedTextBox->SetSize(Coord(10, 1));
	this->_animSpeedTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_animSpeedTextBox);
	
	// Set Speed
	this->_setButton			= new ComponentButton();
	this->_setButton->SetSize(Coord(13, 1));
	this->_setButton->SetSpacing(Coord(5, 0));
	this->_setButton->SetText("Set");
	AddComponent(this->_setButton);
	
	// Cancel
	this->_cancelButton			= new ComponentButton();
	this->_cancelButton->SetSize(Coord(13, 1));
	this->_cancelButton->SetSpacing(Coord(3, 0));
	this->_cancelButton->SetText("Cancel");
	AddComponent(this->_cancelButton);

	ostringstream oss;
	oss << this->_animSpeed;
	ostringstream ossY;
	this->_animSpeedTextBox->SetText(oss.str());
	this->_animSpeedTextBox->SetTyping(true);

	_position = (_app->GetConsoleSize() - _size) / 2;

	_animSpeedTextBox->SetPosition(_position + Coord(15, 3));
	_setButton->SetPosition(_position + Coord(1, 9));
	_cancelButton->SetPosition(_position + Coord(15, 9));
}
void PowerConsole::Components::ComponentAnimationSpeedDialog::Uninitialize() {
	ComponentWindow::Uninitialize();
}
void PowerConsole::Components::ComponentAnimationSpeedDialog::OnEnter() {
	ComponentWindow::OnEnter();
}
void PowerConsole::Components::ComponentAnimationSpeedDialog::OnLeave() {
	ComponentWindow::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

void PowerConsole::Components::ComponentAnimationSpeedDialog::SetBackgroundImage(AsciiImage* backgroundImage) {
	this->_backgroundImage = backgroundImage;
}
unsigned int PowerConsole::Components::ComponentAnimationSpeedDialog::GetAnimationSpeed() const {
	return _animSpeed;
}
void PowerConsole::Components::ComponentAnimationSpeedDialog::SetAnimationSpeed(unsigned int speed) {
	this->_animSpeed = speed;
}


#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentAnimationSpeedDialog::IsSpeedSet() const {
	return (_closed && _speedSet);
}
bool PowerConsole::Components::ComponentAnimationSpeedDialog::IsCanceled() const {
	return (_closed && !_speedSet);
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentAnimationSpeedDialog::Update() {
	_position = (_app->GetConsoleSize() - _size) / 2;

	_animSpeedTextBox->SetPosition(_position + Coord(15, 3));
	_setButton->SetPosition(_position + Coord(1, 9));
	_cancelButton->SetPosition(_position + Coord(15, 9));

	ComponentWindow::Update();


	if (_animSpeedTextBox->IsFinishedTyping()) {
		int newSpeed = _animSpeed;
		bool valid = false;

		istringstream iss(_animSpeedTextBox->GetText());
		iss >> newSpeed;
		valid = !iss.fail();

		if (valid) {
			if (newSpeed >= 1 && newSpeed <= 9999999) {
				_animSpeed = newSpeed;
			}
			else {
				_animSpeed = max(1, min(9999999, newSpeed));
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _animSpeed;
			_animSpeedTextBox->SetText(oss.str());
		}
	}

	if (_setButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Enter)) {
		_speedSet = true;
		Close();
	}
	else if (_cancelButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Escape)) {
		_speedSet = false;
		Close();
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentAnimationSpeedDialog::Draw(AsciiImageBase& g) {
	
	g.DrawImage(_position, *_backgroundImage);

	ComponentWindow::Draw(g);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|