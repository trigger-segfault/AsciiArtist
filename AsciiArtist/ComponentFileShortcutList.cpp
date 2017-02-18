/*=================================================================|
|* File:				ComponentFileShortcutList.cpp			  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/18/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentFileShortcutList.h"
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
#pragma region ComponentFileShortcutList
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentFileShortcutList::ComponentFileShortcutList() : Component() {
	this->_fileList					= nullptr;
	this->_fileListLength			= 0;

	this->_selection				= 0;
	this->_hoverIndex				= -1;
	this->_selectionClicked			= false;
}
PowerConsole::Components::ComponentFileShortcutList::~ComponentFileShortcutList() {
	
}
void PowerConsole::Components::ComponentFileShortcutList::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentFileShortcutList::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentFileShortcutList::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentFileShortcutList::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

File& PowerConsole::Components::ComponentFileShortcutList::GetFileSelection() const {
	return _fileList[_selection];
}
void PowerConsole::Components::ComponentFileShortcutList::SetFileList(File* fileList, int length) {
	this->_fileList = fileList;
	this->_fileListLength = length;
	this->_hoverIndex = -1;
}
	
#pragma endregion
//--------------------------------
#pragma region Events

bool PowerConsole::Components::ComponentFileShortcutList::IsSelectionClicked() const {
	return _selectionClicked;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentFileShortcutList::Update() {
	bool hovering = GetBounds().ContainsPoint(_app->GetMouse()->GetPosition());
	bool down = _app->GetMouse()->IsButtonDown(MouseButtons::Left);

	Coord mousePos = _app->GetMouse()->GetPosition();
	_selection = -1;
	_hoverIndex = -1;
	_selectionClicked = false;

	for (int i = 0; i < _fileListLength; i++) {
		if (SmallRect(_position + Coord(1, i), Coord(_size.X - 1, 1)).ContainsPoint(mousePos)) {
			_hoverIndex = i;

			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				_selection = i;
				_selectionClicked = true;
			}
			break;
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentFileShortcutList::Draw(AsciiImageBase& g) {

	for (int i = 0; i < _fileListLength; i++) {
		if (_fileList[i].Type != FileTypes::File) {
			g.SetPixel(_position + Coord(1, i), Pixel(16, 0x07));
			string name = _fileList[i].Name;
			if (_fileList[i].Type == FileTypes::Drive)
				name = _fileList[i].NameProper;
			g.DrawString(_position + Coord(3, i),
				((name.length() > _size.X - 4) ? (name.substr(0, _size.X - 4)) : name),
				Pixel(' ', 0x07));
		}
		else {
			string name = _fileList[i].Name;
			g.DrawString(_position + Coord(1, i),
				((name.length() > _size.X - 2) ? (name.substr(0, _size.X - 2)) : name),
				Pixel(' ', 0x07));
		}

		if (i == _hoverIndex) {
			g.SetColor(_position + Coord(0, i), Coord(_size.X, 1), 0xF8);
		}
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|