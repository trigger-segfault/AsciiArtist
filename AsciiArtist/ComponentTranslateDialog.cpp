/*=================================================================|
|* File:				ComponentTranslateDialog.cpp			  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/19/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentTranslateDialog.h"
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

PowerConsole::Components::ComponentTranslateDialog::ComponentTranslateDialog() : ComponentWindow() {
	this->_id					= "TranslateDialog";
	this->_size					= Coord(29, 11);

	this->_backgroundImage		= nullptr;

	this->_xTextBox				= nullptr;
	this->_yTextBox				= nullptr;
	this->_translateButton		= nullptr;
	this->_cancelButton			= nullptr;
	this->_translateAllButton	= nullptr;

	this->_distance				= Coord::Zero;

	this->_translateAll			= false;

	this->_translated			= false;
}
PowerConsole::Components::ComponentTranslateDialog::~ComponentTranslateDialog() {
	
}
void PowerConsole::Components::ComponentTranslateDialog::Initialize(ConsoleApp* app, Frame* frame) {
	ComponentWindow::Initialize(app, frame);

	// Width
	this->_xTextBox = new ComponentTextBox();
	this->_xTextBox->SetSize(Coord(6, 1));
	this->_xTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_xTextBox);
	
	// Height
	this->_yTextBox = new ComponentTextBox();
	this->_yTextBox->SetSize(Coord(6, 1));
	this->_yTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_yTextBox);
	
	// Translate
	this->_translateButton = new ComponentButton();
	this->_translateButton->SetSize(Coord(13, 1));
	this->_translateButton->SetSpacing(Coord(4, 0));
	this->_translateButton->SetText("Shift");
	AddComponent(this->_translateButton);
	
	// Cancel
	this->_cancelButton = new ComponentButton();
	this->_cancelButton->SetSize(Coord(13, 1));
	this->_cancelButton->SetSpacing(Coord(3, 0));
	this->_cancelButton->SetText("Cancel");
	AddComponent(this->_cancelButton);

	// Resize All
	this->_translateAllButton = new ComponentButton();
	this->_translateAllButton->SetTogglable(true);
	this->_translateAllButton->SetToggled(false);
	this->_translateAllButton->SetSize(Coord(3, 1));
	this->_translateAllButton->SetText("No");
	AddComponent(this->_translateAllButton);

	ostringstream ossX;
	ossX << this->_distance.X;
	ostringstream ossY;
	ossY << this->_distance.Y;
	this->_xTextBox->SetText(ossX.str());
	this->_yTextBox->SetText(ossY.str());
	this->_xTextBox->SetTyping("true");

	_position = (_app->GetConsoleSize() - _size) / 2;

	_xTextBox->SetPosition(_position + Coord(10, 3));
	_yTextBox->SetPosition(_position + Coord(19, 3));
	_translateAllButton->SetPosition(_position + Coord(14, 6));
	_translateButton->SetPosition(_position + Coord(1, 9));
	_cancelButton->SetPosition(_position + Coord(15, 9));
}
void PowerConsole::Components::ComponentTranslateDialog::Uninitialize() {
	ComponentWindow::Uninitialize();
}
void PowerConsole::Components::ComponentTranslateDialog::OnEnter() {
	ComponentWindow::OnEnter();
}
void PowerConsole::Components::ComponentTranslateDialog::OnLeave() {
	ComponentWindow::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

void PowerConsole::Components::ComponentTranslateDialog::SetBackgroundImage(AsciiImage* backgroundImage) {
	this->_backgroundImage = backgroundImage;
}
Coord PowerConsole::Components::ComponentTranslateDialog::GetDistance() const {
	return _distance;
}

bool PowerConsole::Components::ComponentTranslateDialog::IsTranslateAllSelected() const {
	return _translateAll;
}

#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentTranslateDialog::IsTranslated() const {
	return (_closed && _translated);
}
bool PowerConsole::Components::ComponentTranslateDialog::IsCanceled() const {
	return (_closed && !_translated);
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentTranslateDialog::Update() {
	_position = (_app->GetConsoleSize() - _size) / 2;

	_xTextBox->SetPosition(_position + Coord(10, 3));
	_yTextBox->SetPosition(_position + Coord(19, 3));
	_translateAllButton->SetPosition(_position + Coord(14, 6));
	_translateButton->SetPosition(_position + Coord(1, 9));
	_cancelButton->SetPosition(_position + Coord(15, 9));

	ComponentWindow::Update();
	
	if (_app->GetKeyboard()->IsKeyPressed(Keys::Tab)) {
		if (_xTextBox->IsTyping()) {
			_xTextBox->SetTyping(false);
			_yTextBox->SetTyping(true);
		}
		else if (_yTextBox->IsTyping()) {
			_yTextBox->SetTyping(false);
			_xTextBox->SetTyping(true);
		}
	}

	if (_xTextBox->IsFinishedTyping()) {
		Coord newDistance = _distance;
		bool valid = false;

		istringstream iss(_xTextBox->GetText());
		iss >> newDistance.X;
		valid = !iss.fail();

		if (valid) {
			_distance.X = newDistance.X;
		}
		if (!valid) {
			ostringstream oss;
			oss << _distance.X;
			_xTextBox->SetText(oss.str());
		}
	}
	else if (_yTextBox->IsFinishedTyping()) {
		Coord newDistance = _distance;
		bool valid = false;

		istringstream iss(_yTextBox->GetText());
		iss >> newDistance.Y;
		valid = !iss.fail();

		if (valid) {
			_distance.Y = newDistance.Y;
		}
		if (!valid) {
			ostringstream oss;
			oss << _distance.Y;
			_yTextBox->SetText(oss.str());
		}
	}

	if (_translateButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Enter)) {
		_translated = true;
		Close();
	}
	else if (_cancelButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Escape)) {
		_translated = false;
		Close();
	}
	else if (_translateAllButton->IsButtonPressed()) {
		if (_translateAllButton->IsToggled()) {
			_translateAllButton->SetText("Yes");
			_translateAll = true;
		}
		else {
			_translateAllButton->SetText("No");
			_translateAll = false;
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentTranslateDialog::Draw(AsciiImageBase& g) {
	
	g.DrawImage(_position, *_backgroundImage);

	/*if (!_translateAllEnabled) {
		g.DrawString(_position + Coord(14, 6), "No", Pixel(' ', 0x08));
	}*/

	ComponentWindow::Draw(g);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|