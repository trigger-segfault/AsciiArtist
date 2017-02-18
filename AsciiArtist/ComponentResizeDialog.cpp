/*=================================================================|
|* File:				ComponentResizeDialog.cpp				  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/16/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentResizeDialog.h"
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

PowerConsole::Components::ComponentResizeDialog::ComponentResizeDialog() : ComponentWindow() {
	this->_id					= "ResizeDialog";
	this->_size					= Coord(29, 13);

	this->_backgroundImage		= nullptr;

	this->_widthTextBox			= nullptr;
	this->_heightTextBox		= nullptr;
	this->_resizeButton			= nullptr;
	this->_cancelButton			= nullptr;
	this->_resizeAllButton		= nullptr;

	this->_oldSize				= Coord::One;
	this->_newSize				= Coord::One;

	this->_resizeAll			= true;
	this->_resizeAllEnabled		= false;

	this->_resized				= false;
}
PowerConsole::Components::ComponentResizeDialog::~ComponentResizeDialog() {
	
}
void PowerConsole::Components::ComponentResizeDialog::Initialize(ConsoleApp* app, Frame* frame) {
	ComponentWindow::Initialize(app, frame);

	// Width
	this->_widthTextBox			= new ComponentTextBox();
	this->_widthTextBox->SetSize(Coord(6, 1));
	this->_widthTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_widthTextBox);
	
	// Height
	this->_heightTextBox			= new ComponentTextBox();
	this->_heightTextBox->SetSize(Coord(6, 1));
	this->_heightTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_heightTextBox);
	
	// Resize
	this->_resizeButton			= new ComponentButton();
	this->_resizeButton->SetSize(Coord(13, 1));
	this->_resizeButton->SetSpacing(Coord(3, 0));
	this->_resizeButton->SetText("Resize");
	AddComponent(this->_resizeButton);
	
	// Cancel
	this->_cancelButton			= new ComponentButton();
	this->_cancelButton->SetSize(Coord(13, 1));
	this->_cancelButton->SetSpacing(Coord(3, 0));
	this->_cancelButton->SetText("Cancel");
	AddComponent(this->_cancelButton);

	// Resize All
	this->_resizeAllButton		= new ComponentButton();
	this->_resizeAllButton->SetTogglable(true);
	this->_resizeAllButton->SetToggled(true);
	if (this->_resizeAllEnabled) {
		this->_resizeAllButton->SetSize(Coord(3, 1));
		this->_resizeAllButton->SetText("No");
		this->_resizeAllButton->SetToggled(false);
		this->_resizeAll		= false;
	}
	else {
		this->_resizeAllButton->SetSize(Coord::Zero);
		this->_resizeAllButton->SetText("");
	}
	AddComponent(this->_resizeAllButton);

	ostringstream ossX;
	ossX << this->_newSize.X;
	ostringstream ossY;
	ossY << this->_newSize.Y;
	this->_widthTextBox->SetText(ossX.str());
	this->_heightTextBox->SetText(ossY.str());
	this->_widthTextBox->SetTyping("true");

	_position = (_app->GetConsoleSize() - _size) / 2;

	_widthTextBox->SetPosition(_position + Coord(10, 3));
	_heightTextBox->SetPosition(_position + Coord(19, 3));
	_resizeAllButton->SetPosition(_position + Coord(15, 6));
	_resizeButton->SetPosition(_position + Coord(1, 11));
	_cancelButton->SetPosition(_position + Coord(15, 11));
}
void PowerConsole::Components::ComponentResizeDialog::Uninitialize() {
	ComponentWindow::Uninitialize();
}
void PowerConsole::Components::ComponentResizeDialog::OnEnter() {
	ComponentWindow::OnEnter();
}
void PowerConsole::Components::ComponentResizeDialog::OnLeave() {
	ComponentWindow::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

void PowerConsole::Components::ComponentResizeDialog::SetBackgroundImage(AsciiImage* backgroundImage) {
	this->_backgroundImage = backgroundImage;
}
Coord PowerConsole::Components::ComponentResizeDialog::GetNewSize() const {
	return _newSize;
}
void PowerConsole::Components::ComponentResizeDialog::SetOldSize(Coord oldSize) {
	this->_oldSize = oldSize;
	this->_newSize = oldSize;
}

bool PowerConsole::Components::ComponentResizeDialog::IsResizeAllSelected() const {
	return _resizeAll;
}
void PowerConsole::Components::ComponentResizeDialog::SetResizeAllEnabled(bool enabled) {
	this->_resizeAllEnabled = enabled;
}

#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentResizeDialog::IsResized() const {
	return (_closed && _resized);
}
bool PowerConsole::Components::ComponentResizeDialog::IsCanceled() const {
	return (_closed && !_resized);
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentResizeDialog::Update() {
	_position = (_app->GetConsoleSize() - _size) / 2;

	_widthTextBox->SetPosition(_position + Coord(10, 3));
	_heightTextBox->SetPosition(_position + Coord(19, 3));
	_resizeAllButton->SetPosition(_position + Coord(15, 6));
	_resizeButton->SetPosition(_position + Coord(1, 11));
	_cancelButton->SetPosition(_position + Coord(15, 11));

	ComponentWindow::Update();

	if (_app->GetKeyboard()->IsKeyPressed(Keys::Tab)) {
		if (_widthTextBox->IsTyping()) {
			_widthTextBox->SetTyping(false);
			_heightTextBox->SetTyping(true);
		}
		else if (_heightTextBox->IsTyping()) {
			_heightTextBox->SetTyping(false);
			_widthTextBox->SetTyping(true);
		}
	}

	if (_widthTextBox->IsFinishedTyping()) {
		Coord newSize = _newSize;
		bool valid = false;

		istringstream iss(_widthTextBox->GetText());
		iss >> newSize.X;
		valid = !iss.fail();

		if (valid) {
			if (newSize.X >= 1 && newSize.X <= AsciiImageInfo::MaxSize.X) {
				_newSize.X = newSize.X;
			}
			else {
				_newSize.X = max(1, min(AsciiImageInfo::MaxSize.X, newSize.X));
				valid = false;
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _newSize.X;
			_widthTextBox->SetText(oss.str());
		}
	}
	else if (_heightTextBox->IsFinishedTyping()) {
		Coord newSize = _newSize;
		bool valid = false;

		istringstream iss(_heightTextBox->GetText());
		iss >> newSize.Y;
		valid = !iss.fail();

		if (valid) {
			if (newSize.Y >= 1 && newSize.Y <= AsciiImageInfo::MaxSize.Y) {
				_newSize.Y = newSize.Y;
			}
			else {
				_newSize.Y = max(1, min(AsciiImageInfo::MaxSize.Y, newSize.Y));
				valid = false;
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _newSize.Y;
			_heightTextBox->SetText(oss.str());
		}
	}

	if (_resizeButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Enter)) {
		_resized = true;
		Close();
	}
	else if (_cancelButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Escape)) {
		_resized = false;
		Close();
	}
	else if (_resizeAllButton->IsButtonPressed()) {
		if (_resizeAllButton->IsToggled()) {
			_resizeAllButton->SetText("Yes");
			_resizeAll = true;
		}
		else {
			_resizeAllButton->SetText("No");
			_resizeAll = false;
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentResizeDialog::Draw(AsciiImageBase& g) {
	
	g.DrawImage(_position, *_backgroundImage);

	ostringstream oss;
	oss << _oldSize.X << "x" << _oldSize.Y;
	g.DrawString(_position + Coord(17, 8), oss.str(), Pixel(' ', 0x07));

	if (!_resizeAllEnabled) {
		g.DrawString(_position + Coord(15, 6), "Yes", Pixel(' ', 0x87));
	}

	ComponentWindow::Draw(g);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|