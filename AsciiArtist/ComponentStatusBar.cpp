/*=================================================================|
|* File:				ComponentStatusBar.cpp					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentStatusBar.h"
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
#pragma region ComponentStatusBar
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentStatusBar::ComponentStatusBar() : Component() {
	this->_message			= "";
	this->_urgentMessage	= false;
	this->_messageTimer		= 0;
	this->_pointCoord		= -Coord::One;
	this->_sizeCoord		= -Coord::One;
}
PowerConsole::Components::ComponentStatusBar::~ComponentStatusBar() {
	
}
void PowerConsole::Components::ComponentStatusBar::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentStatusBar::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentStatusBar::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentStatusBar::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

void PowerConsole::Components::ComponentStatusBar::SetMessage(const string& message) {
	this->_message = message;
	_urgentMessage = false;
	_messageTimer = clock();
}
void PowerConsole::Components::ComponentStatusBar::SetUrgentMessage(const string& message) {
	this->_message = message;
	_urgentMessage = true;
	_messageTimer = clock();
}
void PowerConsole::Components::ComponentStatusBar::SetPointCoord(Coord point) {
	this->_pointCoord = point;
}
void PowerConsole::Components::ComponentStatusBar::SetSizeCoord(Coord size) {
	this->_sizeCoord = size;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentStatusBar::Update() {
	unsigned int time = clock();
	if (time - _messageTimer >= 5000) {
		_message = "";
		_urgentMessage = false;
		_messageTimer = 0;
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentStatusBar::Draw(AsciiImageBase& g) {
	if (_pointCoord != -Coord::One) {
		ostringstream oss;
		oss << _pointCoord.X << ", " << _pointCoord.Y;
		
		g.DrawString(Coord(10, _app->GetConsoleSize().Y - 2), oss.str(), Pixel(' ', 0x07));
	}
	if (_sizeCoord != -Coord::One) {
		ostringstream oss;
		oss << _sizeCoord.X << ", " << _sizeCoord.Y;

		g.DrawString(Coord(31, _app->GetConsoleSize().Y - 2), oss.str(), Pixel(' ', 0x07));
	}

	if (!_message.empty()) {
		if (_app->GetConsoleSize().X - 45 - 1 < (int)_message.length()) {
			g.DrawString(Coord(45, _app->GetConsoleSize().Y - 2), _message.substr(0, _app->GetConsoleSize().X - 45 - 1), Pixel(' ', _urgentMessage ? 0x0C : 0x07));
		}
		else {
			g.DrawString(Coord(45, _app->GetConsoleSize().Y - 2), _message, Pixel(' ', _urgentMessage ? 0x0C : 0x07));
		}
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|