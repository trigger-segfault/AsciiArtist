/*=================================================================|
|* File:				ComponentImportFileDialog.cpp			  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/24/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentImportFileDialog.h"
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
#pragma region ComponentImportFileDialog
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentImportFileDialog::ComponentImportFileDialog() : ComponentWindow() {
	this->_id					= "ImportFileDialog";
	this->_size					= Coord(43, 21);

	this->_backgroundImage		= nullptr;

	this->_widthTextBox			= nullptr;
	this->_heightTextBox		= nullptr;
	this->_startPositionTextBox	= nullptr;
	this->_charSpacingTextBox	= nullptr;
	this->_colorSpacingTextBox	= nullptr;

	this->_readCharButton		= nullptr;
	this->_readColorButton		= nullptr;

	this->_importButton			= nullptr;
	this->_cancelButton			= nullptr;

	this->_imageSize			= Coord::Zero;
	this->_startPosition		= 0;
	this->_charSpacing			= 0;
	this->_colorSpacing			= 0;

	this->_imported				= false;
}
PowerConsole::Components::ComponentImportFileDialog::~ComponentImportFileDialog() {
	
}
void PowerConsole::Components::ComponentImportFileDialog::Initialize(ConsoleApp* app, Frame* frame) {
	ComponentWindow::Initialize(app, frame);

	// Width
	this->_widthTextBox				= new ComponentTextBox();
	this->_widthTextBox->SetSize(Coord(6, 1));
	this->_widthTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_widthTextBox);
	
	// Height
	this->_heightTextBox			= new ComponentTextBox();
	this->_heightTextBox->SetSize(Coord(6, 1));
	this->_heightTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_heightTextBox);
	
	// Start Position
	this->_startPositionTextBox		= new ComponentTextBox();
	this->_startPositionTextBox->SetSize(Coord(15, 1));
	this->_startPositionTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_startPositionTextBox);
	
	// Character Spacing
	this->_charSpacingTextBox		= new ComponentTextBox();
	this->_charSpacingTextBox->SetSize(Coord(15, 1));
	this->_charSpacingTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_charSpacingTextBox);
	
	// Color Spacing
	this->_colorSpacingTextBox		= new ComponentTextBox();
	this->_colorSpacingTextBox->SetSize(Coord(15, 1));
	this->_colorSpacingTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_colorSpacingTextBox);
	
	// Read Char
	this->_readCharButton = new ComponentButton();
	this->_readCharButton->SetTogglable(true);
	this->_readCharButton->SetToggled(_charImported);
	this->_readCharButton->SetSize(Coord(3, 1));
	this->_readCharButton->SetSpacing(Coord(0, 0));
	this->_readCharButton->SetText(_charImported ? "Yes" : "No");
	AddComponent(this->_readCharButton);
	
	// Read Color
	this->_readColorButton = new ComponentButton();
	this->_readColorButton->SetTogglable(true);
	this->_readColorButton->SetToggled(_colorImported);
	this->_readColorButton->SetSize(Coord(3, 1));
	this->_readColorButton->SetSpacing(Coord(0, 0));
	this->_readColorButton->SetText(_colorImported ? "Yes" : "No");
	AddComponent(this->_readColorButton);

	// Import
	this->_importButton = new ComponentButton();
	this->_importButton->SetSize(Coord(13, 1));
	this->_importButton->SetSpacing(Coord(3, 0));
	this->_importButton->SetText("Import");
	AddComponent(this->_importButton);
	
	// Cancel
	this->_cancelButton = new ComponentButton();
	this->_cancelButton->SetSize(Coord(13, 1));
	this->_cancelButton->SetSpacing(Coord(3, 0));
	this->_cancelButton->SetText("Cancel");
	AddComponent(this->_cancelButton);
	
	ostringstream ossX;
	ossX << this->_imageSize.X;
	ostringstream ossY;
	ossY << this->_imageSize.Y;
	this->_widthTextBox->SetText(ossX.str());
	this->_heightTextBox->SetText(ossY.str());
	this->_widthTextBox->SetTyping("true");

	ostringstream ossPos;
	ossPos << this->_startPosition;
	ostringstream ossChar;
	ossChar << this->_charSpacing;
	ostringstream ossColor;
	ossColor << this->_colorSpacing;
	this->_startPositionTextBox->SetText(ossPos.str());
	this->_charSpacingTextBox->SetText(ossChar.str());
	this->_colorSpacingTextBox->SetText(ossColor.str());

	Coord pos2 = (_app->GetConsoleSize() - _size) / 2;

	_position = (_app->GetConsoleSize() - _size) / 2;

	_widthTextBox->SetPosition(_position + Coord(4, 7));
	_heightTextBox->SetPosition(_position + Coord(13, 7));
	_startPositionTextBox->SetPosition(_position + Coord(24, 7));
	_charSpacingTextBox->SetPosition(_position + Coord(4, 12));
	_colorSpacingTextBox->SetPosition(_position + Coord(24, 12));
	
	_readCharButton->SetPosition(_position + Coord(14, 15));
	_readColorButton->SetPosition(_position + Coord(35, 15));

	_importButton->SetPosition(_position + Coord(15, 19));
	_cancelButton->SetPosition(_position + Coord(29, 19));

	pos2 = 2;
}
void PowerConsole::Components::ComponentImportFileDialog::Uninitialize() {
	ComponentWindow::Uninitialize();
}
void PowerConsole::Components::ComponentImportFileDialog::OnEnter() {
	ComponentWindow::OnEnter();
}
void PowerConsole::Components::ComponentImportFileDialog::OnLeave() {
	ComponentWindow::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

void PowerConsole::Components::ComponentImportFileDialog::SetBackgroundImage(AsciiImage* backgroundImage) {
	this->_backgroundImage = backgroundImage;
}
Coord PowerConsole::Components::ComponentImportFileDialog::GetImageSize() const {
	return _imageSize;
}
void PowerConsole::Components::ComponentImportFileDialog::SetImageSize(Coord size) {
	this->_imageSize = size;
	this->_imageSize = size;
}
unsigned int PowerConsole::Components::ComponentImportFileDialog::GetStartPosition() const {
	return _startPosition;
}
void PowerConsole::Components::ComponentImportFileDialog::SetStartPosition(unsigned int position) {
	this->_startPosition = position;
}
unsigned int PowerConsole::Components::ComponentImportFileDialog::GetCharSpacing() const {
	return _charSpacing;
}
void PowerConsole::Components::ComponentImportFileDialog::SetCharSpacing(unsigned int spacing) {
	this->_charSpacing = spacing;
}
unsigned int PowerConsole::Components::ComponentImportFileDialog::GetColorSpacing() const {
	return _colorSpacing;
}
void PowerConsole::Components::ComponentImportFileDialog::SetColorSpacing(unsigned int spacing) {
	this->_colorSpacing = spacing;
}
bool PowerConsole::Components::ComponentImportFileDialog::IsCharImported() const {
	return _charImported;
}
void PowerConsole::Components::ComponentImportFileDialog::SetCharImported(bool imported) {
	this->_charImported = imported;
}
bool PowerConsole::Components::ComponentImportFileDialog::IsColorImported() const {
	return _colorImported;
}
void PowerConsole::Components::ComponentImportFileDialog::SetColorImported(bool imported) {
	this->_colorImported = imported;
}

#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentImportFileDialog::IsImported() const {
	return (_closed && _imported);
}
bool PowerConsole::Components::ComponentImportFileDialog::IsCanceled() const {
	return (_closed && !_imported);
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentImportFileDialog::Update() {
	_position = (_app->GetConsoleSize() - _size) / 2;

	_widthTextBox->SetPosition(_position + Coord(4, 7));
	_heightTextBox->SetPosition(_position + Coord(13, 7));
	_startPositionTextBox->SetPosition(_position + Coord(24, 7));
	_charSpacingTextBox->SetPosition(_position + Coord(4, 12));
	_colorSpacingTextBox->SetPosition(_position + Coord(24, 12));
	
	_readCharButton->SetPosition(_position + Coord(14, 15));
	_readColorButton->SetPosition(_position + Coord(35, 15));

	_importButton->SetPosition(_position + Coord(15, 19));
	_cancelButton->SetPosition(_position + Coord(29, 19));

	ComponentWindow::Update();


	
	if (_app->GetKeyboard()->IsKeyPressed(Keys::Tab)) {
		if (_widthTextBox->IsTyping()) {
			_widthTextBox->SetTyping(false);
			_heightTextBox->SetTyping(true);
		}
		else if (_heightTextBox->IsTyping()) {
			_heightTextBox->SetTyping(false);
			_startPositionTextBox->SetTyping(true);
		}
		else if (_startPositionTextBox->IsTyping()) {
			_startPositionTextBox->SetTyping(false);
			_charSpacingTextBox->SetTyping(true);
		}
		else if (_charSpacingTextBox->IsTyping()) {
			_charSpacingTextBox->SetTyping(false);
			_colorSpacingTextBox->SetTyping(true);
		}
		else if (_colorSpacingTextBox->IsTyping()) {
			_colorSpacingTextBox->SetTyping(false);
			_widthTextBox->SetTyping(true);
		}
	}
	
	if (_widthTextBox->IsFinishedTyping()) {
		Coord newSize = _imageSize;
		bool valid = false;

		istringstream iss(_widthTextBox->GetText());
		iss >> newSize.X;
		valid = !iss.fail();

		if (valid) {
			if (newSize.X >= 1 && newSize.X <= AsciiImageInfo::MaxSize.X) {
				_imageSize.X = newSize.X;
			}
			else {
				_imageSize.X = max(1, min(AsciiImageInfo::MaxSize.X, newSize.X));
				valid = false;
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _imageSize.X;
			_widthTextBox->SetText(oss.str());
		}
	}
	else if (_heightTextBox->IsFinishedTyping()) {
		Coord newSize = _imageSize;
		bool valid = false;

		istringstream iss(_heightTextBox->GetText());
		iss >> newSize.Y;
		valid = !iss.fail();

		if (valid) {
			if (newSize.Y >= 1 && newSize.Y <= AsciiImageInfo::MaxSize.Y) {
				_imageSize.Y = newSize.Y;
			}
			else {
				_imageSize.Y = max(1, min(AsciiImageInfo::MaxSize.Y, newSize.Y));
				valid = false;
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _imageSize.Y;
			_heightTextBox->SetText(oss.str());
		}
	}
	else if (_startPositionTextBox->IsFinishedTyping()) {
		int newPos = (int)_startPosition;
		bool valid = false;

		istringstream iss(_startPositionTextBox->GetText());
		iss >> newPos;
		valid = !iss.fail();

		if (valid) {
			if (newPos >= 0 && newPos <= 999999999) {
				_startPosition = (unsigned int)newPos;
			}
			else {
				_startPosition = (unsigned int)max(0, min(999999999, newPos));
				valid = false;
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _startPosition;
			_startPositionTextBox->SetText(oss.str());
		}
	}
	else if (_charSpacingTextBox->IsFinishedTyping()) {
		int newSpacing = (int)_charSpacing;
		bool valid = false;

		istringstream iss(_charSpacingTextBox->GetText());
		iss >> newSpacing;
		valid = !iss.fail();

		if (valid) {
			if (newSpacing >= 0 && newSpacing <= 99999) {
				_charSpacing = (unsigned int)newSpacing;
			}
			else {
				_charSpacing = (unsigned int)max(0, min(99999, newSpacing));
				valid = false;
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _charSpacing;
			_charSpacingTextBox->SetText(oss.str());
		}
	}
	else if (_colorSpacingTextBox->IsFinishedTyping()) {
		int newSpacing = (int)_colorSpacing;
		bool valid = false;

		istringstream iss(_colorSpacingTextBox->GetText());
		iss >> newSpacing;
		valid = !iss.fail();

		if (valid) {
			if (newSpacing >= 0 && newSpacing <= 99999) {
				_colorSpacing = (unsigned int)newSpacing;
			}
			else {
				_colorSpacing = (unsigned int)max(0, min(99999, newSpacing));
				valid = false;
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _colorSpacing;
			_colorSpacingTextBox->SetText(oss.str());
		}
	}

	if (_readCharButton->IsButtonPressed()) {
		_charImported = !_charImported;
		if (_readCharButton->IsToggled()) {
			_readCharButton->SetText("Yes");
		}
		else {
			_readCharButton->SetText("No");
		}
	}
	else if (_readColorButton->IsButtonPressed()) {
		_colorImported = !_colorImported;
		if (_readColorButton->IsToggled()) {
			_readColorButton->SetText("Yes");
		}
		else {
			_readColorButton->SetText("No");
		}
	}

	if (_importButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Enter)) {
		_imported = true;
		Close();
	}
	else if (_cancelButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Escape)) {
		_imported = false;
		Close();
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentImportFileDialog::Draw(AsciiImageBase& g) {
	
	g.DrawImage(_position, *_backgroundImage);

	ComponentWindow::Draw(g);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|