/*=================================================================|
|* File:				ComponentImageSettingsDialog.cpp		  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/20/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentImageSettingsDialog.h"
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

PowerConsole::Components::ComponentImageSettingsDialog::ComponentImageSettingsDialog() : ComponentWindow() {
	this->_id					= "ImageSettingsDialog";
	this->_size					= Coord(42, 23);

	this->_backgroundImage		= nullptr;

	this->_attributesButton		= nullptr;
	this->_animationButton		= nullptr;
	this->_multisizeButton		= nullptr;
	this->_layeredButton		= nullptr;

	this->_version2Button		= nullptr;
	this->_version3Button		= nullptr;
	this->_version3SmallButton	= nullptr;

	this->_animSpeedTextBox		= nullptr;

	this->_saveButton			= nullptr;
	this->_cancelButton			= nullptr;

	this->_background			= Pixel();
	this->_format				= ImageFormats::FormatBasic;
	this->_version				= 3;
	this->_animSpeed			= 0;

	this->_primaryPixel			= Pixel();
	this->_secondaryPixel		= Pixel();

	this->_saved				= false;
}
PowerConsole::Components::ComponentImageSettingsDialog::~ComponentImageSettingsDialog() {
	
}
void PowerConsole::Components::ComponentImageSettingsDialog::Initialize(ConsoleApp* app, Frame* frame) {
	ComponentWindow::Initialize(app, frame);

	// Animation Speed
	this->_animSpeedTextBox = new ComponentTextBox();
	this->_animSpeedTextBox->SetSize(Coord(10, 1));
	this->_animSpeedTextBox->SetIndent(Coord(1, 0));
	AddComponent(this->_animSpeedTextBox);
	
	// Set Speed
	this->_saveButton = new ComponentButton();
	this->_saveButton->SetSize(Coord(13, 1));
	this->_saveButton->SetSpacing(Coord(4, 0));
	this->_saveButton->SetText("Save");
	AddComponent(this->_saveButton);
	
	// Cancel
	this->_cancelButton = new ComponentButton();
	this->_cancelButton->SetSize(Coord(13, 1));
	this->_cancelButton->SetSpacing(Coord(3, 0));
	this->_cancelButton->SetText("Cancel");
	AddComponent(this->_cancelButton);

	// Attributes
	this->_attributesButton = new ComponentButton();
	this->_attributesButton->SetTogglable(true);
	this->_attributesButton->SetToggled((_format & ImageFormats::FormatAttributes) != 0x0);
	this->_attributesButton->SetSize(Coord(10, 1));
	this->_attributesButton->SetText("Attributes");
	AddComponent(this->_attributesButton);
	
	// Animation
	this->_animationButton = new ComponentButton();
	this->_animationButton->SetTogglable(true);
	this->_animationButton->SetToggled((_format & ImageFormats::FormatAnimation) != 0x0);
	this->_animationButton->SetSize(Coord(10, 1));
	this->_animationButton->SetText("Animation");
	AddComponent(this->_animationButton);
	
	// Multisize
	this->_multisizeButton = new ComponentButton();
	this->_multisizeButton->SetTogglable(true);
	this->_multisizeButton->SetToggled((_format & ImageFormats::FormatMultiSize) != 0x0);
	this->_multisizeButton->SetSize(Coord(10, 1));
	this->_multisizeButton->SetText("Multisize");
	AddComponent(this->_multisizeButton);
	
	// Layered
	this->_layeredButton = new ComponentButton();
	this->_layeredButton->SetTogglable(true);
	this->_layeredButton->SetToggled((_format & ImageFormats::FormatLayered) != 0x0);
	//this->_layeredButton->SetSize(Coord(10, 1));
	this->_layeredButton->SetSize(Coord::Zero);
	//this->_layeredButton->SetText("Layered");
	this->_layeredButton->SetText("");
	AddComponent(this->_layeredButton);

	// Version 2
	this->_version2Button = new ComponentButton();
	this->_version2Button->SetTogglable(true);
	this->_version2Button->SetToggled(_version == 2);
	this->_version2Button->SetSize(Coord(15, 1));
	this->_version2Button->SetText("Version 2");
	AddComponent(this->_version2Button);
	
	// Version 3
	this->_version3Button = new ComponentButton();
	this->_version3Button->SetTogglable(true);
	this->_version3Button->SetToggled(_version == 3);
	this->_version3Button->SetSize(Coord(15, 1));
	this->_version3Button->SetText("Version 3");
	AddComponent(this->_version3Button);
	
	// Version 3 Small
	this->_version3SmallButton = new ComponentButton();
	this->_version3SmallButton->SetTogglable(true);
	this->_version3SmallButton->SetToggled(_version == 1);
	//this->_version3SmallButton->SetSize(Coord(15, 1));
	this->_version3SmallButton->SetSize(Coord::Zero);
	//this->_version3SmallButton->SetText("Version 3 Small");
	this->_version3SmallButton->SetText("");
	AddComponent(this->_version3SmallButton);

	ostringstream oss;
	oss << this->_animSpeed;
	ostringstream ossY;
	this->_animSpeedTextBox->SetText(oss.str());

	_position = (_app->GetConsoleSize() - _size) / 2;
	
	_attributesButton->SetPosition(_position + Coord(5, 6));
	_animationButton->SetPosition(_position + Coord(5, 7));
	_multisizeButton->SetPosition(_position + Coord(5, 8));
	_layeredButton->SetPosition(_position + Coord(5, 9));
	
	_version2Button->SetPosition(_position + Coord(24, 6));
	_version3Button->SetPosition(_position + Coord(24, 7));
	_version3SmallButton->SetPosition(_position + Coord(24, 8));

	_animSpeedTextBox->SetPosition(_position + Coord(16, 17));
	_saveButton->SetPosition(_position + Coord(14, 21));
	_cancelButton->SetPosition(_position + Coord(28, 21));
}
void PowerConsole::Components::ComponentImageSettingsDialog::Uninitialize() {
	ComponentWindow::Uninitialize();
}
void PowerConsole::Components::ComponentImageSettingsDialog::OnEnter() {
	ComponentWindow::OnEnter();
}
void PowerConsole::Components::ComponentImageSettingsDialog::OnLeave() {
	ComponentWindow::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

void PowerConsole::Components::ComponentImageSettingsDialog::SetBackgroundImage(AsciiImage* backgroundImage) {
	this->_backgroundImage = backgroundImage;
}
unsigned int PowerConsole::Components::ComponentImageSettingsDialog::GetAnimationSpeed() const {
	return _animSpeed;
}
void PowerConsole::Components::ComponentImageSettingsDialog::SetAnimationSpeed(unsigned int speed) {
	this->_animSpeed = speed;
}
Pixel PowerConsole::Components::ComponentImageSettingsDialog::GetBackground() const {
	return _background;
}
void PowerConsole::Components::ComponentImageSettingsDialog::SetBackground(Pixel background) {
	this->_background = background;
}
void PowerConsole::Components::ComponentImageSettingsDialog::SetPrimaryPixel(Pixel pixel) {
	this->_primaryPixel = pixel;
}
void PowerConsole::Components::ComponentImageSettingsDialog::SetSecondaryPixel(Pixel pixel) {
	this->_secondaryPixel = pixel;
}
unsigned short PowerConsole::Components::ComponentImageSettingsDialog::GetFormat() const {
	return _format;
}
void PowerConsole::Components::ComponentImageSettingsDialog::SetFormat(unsigned short format) {
	this->_format = format;
}
int PowerConsole::Components::ComponentImageSettingsDialog::GetVersion() const {
	return _version;
}
void PowerConsole::Components::ComponentImageSettingsDialog::SetVersion(int version) {
	this->_version = version;
}

#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentImageSettingsDialog::IsSaved() const {
	return (_closed && _saved);
}
bool PowerConsole::Components::ComponentImageSettingsDialog::IsCanceled() const {
	return (_closed && !_saved);
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentImageSettingsDialog::Update() {
	_position = (_app->GetConsoleSize() - _size) / 2;
	
	_attributesButton->SetPosition(_position + Coord(5, 6));
	_animationButton->SetPosition(_position + Coord(5, 7));
	_multisizeButton->SetPosition(_position + Coord(5, 8));
	_layeredButton->SetPosition(_position + Coord(5, 9));
	
	_version2Button->SetPosition(_position + Coord(24, 6));
	_version3Button->SetPosition(_position + Coord(24, 7));
	_version3SmallButton->SetPosition(_position + Coord(24, 8));

	_animSpeedTextBox->SetPosition(_position + Coord(16, 17));
	_saveButton->SetPosition(_position + Coord(14, 21));
	_cancelButton->SetPosition(_position + Coord(28, 21));

	ComponentWindow::Update();

	if (_attributesButton->IsButtonPressed()) {
		if (_attributesButton->IsToggled())
			_format |= ImageFormats::FormatAttributes;
		else
			_format &= ~ImageFormats::FormatAttributes;
	}
	else if (_animationButton->IsButtonPressed()) {
		if (_animationButton->IsToggled())
			_format |= ImageFormats::FormatAnimation;
		else
			_format &= ~ImageFormats::FormatAnimation;
	}
	else if (_multisizeButton->IsButtonPressed()) {
		if (_multisizeButton->IsToggled())
			_format |= ImageFormats::FormatMultiSize;
		else
			_format &= ~ImageFormats::FormatMultiSize;
	}
	else if (_layeredButton->IsButtonPressed()) {
		if (_layeredButton->IsToggled())
			_format |= ImageFormats::FormatLayered;
		else
			_format &= ~ImageFormats::FormatLayered;
	}

	if (_version2Button->IsButtonPressed()) {
		if (_version2Button->IsToggled()) {
			_version = 2;
			_version3Button->SetToggled(false);
			_version3SmallButton->SetToggled(false);
		}
		else {
			_version2Button->SetToggled(true);
		}
	}
	else if (_version3Button->IsButtonPressed()) {
		if (_version3Button->IsToggled()) {
			_version = 3;
			_version2Button->SetToggled(false);
			_version3SmallButton->SetToggled(false);
		}
		else {
			_version3Button->SetToggled(true);
		}
	}
	else if (_version3SmallButton->IsButtonPressed()) {
		if (_version3SmallButton->IsToggled()) {
			_version = 1;
			_version2Button->SetToggled(false);
			_version3Button->SetToggled(false);
		}
		else {
			_version3SmallButton->SetToggled(true);
		}
	}

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
				valid = false;
			}
		}
		if (!valid) {
			ostringstream oss;
			oss << _animSpeed;
			_animSpeedTextBox->SetText(oss.str());
		}
	}

	Coord mousePos = _app->GetMouse()->GetPosition() - _position;
	int pressed = (_app->GetMouse()->IsButtonPressed(MouseButtons::Left) ? 1 : (_app->GetMouse()->IsButtonPressed(MouseButtons::Right) ? 2 : 0));
	if (pressed != 0) {
		Pixel pixel = (pressed == 1 ? _primaryPixel : _secondaryPixel);
		if (mousePos == Coord(16, 13)) {
			_background = pixel;
		}
		else if (mousePos == Coord(23, 13)) {
			_background.Char = pixel.Char;
			_background.Attributes &= ~PixelAttributes::AttributeChar;
			_background.Attributes |= (pixel.Attributes & PixelAttributes::AttributeChar);
		}
		else if (mousePos == Coord(30, 13) || mousePos == Coord(31, 13)) {
			_background.Color = pixel.Color;
			_background.Attributes &= PixelAttributes::AttributeChar;
			_background.Attributes |= (pixel.Attributes & ~PixelAttributes::AttributeChar);
		}
	}

	if (_saveButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Enter)) {
		_saved = true;
		Close();
	}
	else if (_cancelButton->IsButtonPressed() || _app->GetKeyboard()->IsKeyPressed(Keys::Escape)) {
		_saved = false;
		Close();
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentImageSettingsDialog::Draw(AsciiImageBase& g) {
	
	g.DrawImage(_position, *_backgroundImage);

	g.SetPixel(_position + Coord(16, 13), _background);

	if ((_background.Attributes & PixelAttributes::AttributeChar) == 0x0) {
		g.SetPixel(_position + Coord(23, 13), Pixel(178, 0x03));
	}
	else {
		g.SetPixel(_position + Coord(23, 13), Pixel(_background.Char, 0x07));
	}
	
	if ((_background.Attributes & PixelAttributes::AttributeFInvert) != 0x0) {
		g.SetPixel(_position + Coord(30, 13), Pixel(176, 0x08));
	}
	else if ((_background.Attributes & PixelAttributes::AttributeFColor) == 0x0) {
		g.SetPixel(_position + Coord(30, 13), Pixel(178, 0x0F));
	}
	else {
		g.SetPixel(_position + Coord(30, 13), Pixel(219, _background.Color));
	}
	
	if ((_background.Attributes & PixelAttributes::AttributeBInvert) != 0x0) {
		g.SetPixel(_position + Coord(31, 13), Pixel(176, 0x08));
	}
	else if ((_background.Attributes & PixelAttributes::AttributeBColor) == 0x0) {
		g.SetPixel(_position + Coord(31, 13), Pixel(178, 0x0F));
	}
	else {
		g.SetPixel(_position + Coord(31, 13), Pixel(' ', _background.Color));
	}

	g.DrawString(_position + Coord(5, 9), "Layered", Pixel(' ', 0x08));
	g.DrawString(_position + Coord(24, 8), "Version 3 Small", Pixel(' ', 0x08));

	ComponentWindow::Draw(g);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|