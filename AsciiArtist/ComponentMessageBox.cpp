/*=================================================================|
|* File:				ComponentMessageBox.cpp					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/19/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentMessageBox.h"
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
#pragma region ComponentOverwriteWindow
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentMessageBox::ComponentMessageBox() : ComponentWindow() {
	this->_id					= "MessageBox";
	this->_size					= Coord(29, 11);

	this->_backgroundImage		= nullptr;

	this->_okButton				= nullptr;
	this->_cancelButton			= nullptr;

	this->_line1				= "";
	this->_line2				= "";

	this->_ok					= false;
}
PowerConsole::Components::ComponentMessageBox::~ComponentMessageBox() {
	
}
void PowerConsole::Components::ComponentMessageBox::Initialize(ConsoleApp* app, Frame* frame) {
	ComponentWindow::Initialize(app, frame);
	
	// Ok
	this->_okButton			= new ComponentButton();
	this->_okButton->SetSize(Coord(13, 1));
	this->_okButton->SetSpacing(Coord(5, 0));
	this->_okButton->SetText("OK");
	AddComponent(this->_okButton);
	
	// Cancel
	this->_cancelButton			= new ComponentButton();
	this->_cancelButton->SetSize(Coord(13, 1));
	this->_cancelButton->SetSpacing(Coord(3, 0));
	this->_cancelButton->SetText("Cancel");
	AddComponent(this->_cancelButton);
	
	_position = (_app->GetConsoleSize() - _size) / 2;

	_okButton->SetPosition(_position + Coord(1, 9));
	_cancelButton->SetPosition(_position + Coord(15, 9));
}
void PowerConsole::Components::ComponentMessageBox::Uninitialize() {
	ComponentWindow::Uninitialize();
}
void PowerConsole::Components::ComponentMessageBox::OnEnter() {
	ComponentWindow::OnEnter();
}
void PowerConsole::Components::ComponentMessageBox::OnLeave() {
	ComponentWindow::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

void PowerConsole::Components::ComponentMessageBox::SetBackgroundImage(AsciiImage* backgroundImage) {
	this->_backgroundImage = backgroundImage;
}
void PowerConsole::Components::ComponentMessageBox::SetLine1(const string& text) {
	this->_line1 = text;
}
void PowerConsole::Components::ComponentMessageBox::SetLine2(const string& text) {
	this->_line2 = text;
}

#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentMessageBox::IsAccepted() const {
	return (_closed && _ok);
}
bool PowerConsole::Components::ComponentMessageBox::IsCanceled() const {
	return (_closed && !_ok);
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentMessageBox::Update() {
	_position = (_app->GetConsoleSize() - _size) / 2;

	_okButton->SetPosition(_position + Coord(1, 9));
	_cancelButton->SetPosition(_position + Coord(15, 9));

	ComponentWindow::Update();


	if (_okButton->IsButtonPressed()) {
		_ok = true;
		Close();
	}
	else if (_cancelButton->IsButtonPressed()) {
		_ok = false;
		Close();
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentMessageBox::Draw(AsciiImageBase& g) {
	
	g.DrawImage(_position, *_backgroundImage);
	
	if (_line2.empty()) {
		g.DrawString(_position + Coord(3, 4), _line1, Pixel(' ', 0x07));
	}
	else {
		g.DrawString(_position + Coord(3, 3), _line1, Pixel(' ', 0x07));
		g.DrawString(_position + Coord(3, 5), _line2, Pixel(' ', 0x07));
	}
	

	ComponentWindow::Draw(g);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|