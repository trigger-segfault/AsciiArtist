/*=================================================================|
|* File:				ComponentList.cpp						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentList.h"
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

PowerConsole::Components::ComponentList::ComponentList() : Component() {
	this->_selection			= -1;
	this->_hoverIndex			= -1;
	this->_requiresSelection	= false;
	this->_selectionChanged		= false;
}
PowerConsole::Components::ComponentList::~ComponentList() {
	
}
void PowerConsole::Components::ComponentList::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentList::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentList::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentList::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

bool PowerConsole::Components::ComponentList::IsSelectionRequired() const {
	return _requiresSelection;
}
void PowerConsole::Components::ComponentList::SetSelectionRequired(bool requiresSelection) {
	this->_requiresSelection = requiresSelection;
	if (requiresSelection && _selection == -1)
		_selection = 0;
}
int PowerConsole::Components::ComponentList::GetSelection() const {
	return _selection;
}
void PowerConsole::Components::ComponentList::SetSelection(int selection) {
	this->_selection = max(_requiresSelection ? 0 : -1, min(_listEntries.size() - 1, selection));
}
void PowerConsole::Components::ComponentList::AddEntry(const string& entry) {
	_listEntries.push_back(entry);
}
	
#pragma endregion
//--------------------------------
#pragma region Events

bool PowerConsole::Components::ComponentList::IsSelectionChanged() const {
	return _selectionChanged;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentList::Update() {
	bool hovering = GetBounds().ContainsPoint(_app->GetMouse()->GetPosition());
	bool down = _app->GetMouse()->IsButtonDown(MouseButtons::Left);

	Coord mousePos = _app->GetMouse()->GetPosition();
	_hoverIndex = -1;
	_selectionChanged = false;

	for (int i = 0; i < (int)_listEntries.size(); i++) {
		if (SmallRect(_position + Coord(0, i), Coord(_size.X, 1)).ContainsPoint(mousePos)) {
			_hoverIndex = i;

			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				if (_selection != i) {
					_selection = i;
					_selectionChanged = true;
				}
				else if (!_requiresSelection) {
					_selection = -1;
					_selectionChanged = true;
				}
			}
			break;
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentList::Draw(AsciiImageBase& g) {

	for (int i = 0; i < (int)_listEntries.size(); i++) {
		if (i == _selection)
			g.DrawString(_position + Coord(0, i), _listEntries[i], Pixel(' ', 0x9F));
		else if (i == _hoverIndex)
			g.DrawString(_position + Coord(0, i), _listEntries[i], Pixel(' ', 0x0F));
		else
			g.DrawString(_position + Coord(0, i), _listEntries[i], Pixel(' ', 0x07));
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|