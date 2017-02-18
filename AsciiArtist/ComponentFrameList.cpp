/*=================================================================|
|* File:				ComponentFrameList.cpp					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/13/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentFrameList.h"
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
#pragma region ComponentFrameList
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentFrameList::ComponentFrameList() : Component() {
	this->_numFrames		= 1;
	this->_scrollBar		= nullptr;

	this->_selection		= 0;
	this->_hoverIndex		= -1;
	this->_selectionChanged	= false;
	this->_shiftHovering	= false;
}
PowerConsole::Components::ComponentFrameList::~ComponentFrameList() {
	
}
void PowerConsole::Components::ComponentFrameList::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentFrameList::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentFrameList::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentFrameList::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

int PowerConsole::Components::ComponentFrameList::GetNumFrames() const {
	return _numFrames;
}
void PowerConsole::Components::ComponentFrameList::SetNumFrames(unsigned int numFrames) {
	this->_numFrames = numFrames;
}
void PowerConsole::Components::ComponentFrameList::SetScrollBar(ComponentScrollBar* scrollBar) {
	this->_scrollBar = scrollBar;
}

int PowerConsole::Components::ComponentFrameList::GetCurrentFrame() const {
	return _selection;
}
void PowerConsole::Components::ComponentFrameList::SetCurrentFrame(int selection) {
	this->_selection = max(0, min(_numFrames - 1, selection));
}
int PowerConsole::Components::ComponentFrameList::GetHoverFrame() const {
	return _hoverIndex;
}
	
#pragma endregion
//--------------------------------
#pragma region Events

bool PowerConsole::Components::ComponentFrameList::IsFrameChanged() const {
	return _selectionChanged;
}
bool PowerConsole::Components::ComponentFrameList::IsShiftHovered() const {
	return _shiftHovering;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentFrameList::Update() {
	bool hovering = GetBounds().ContainsPoint(_app->GetMouse()->GetPosition());
	bool down = _app->GetMouse()->IsButtonDown(MouseButtons::Left);

	Coord mousePos = _app->GetMouse()->GetPosition();
	_hoverIndex = -1;
	_selectionChanged = false;
	_shiftHovering = false;

	for (int i = _scrollBar->GetListPosition().Y, y = 0; i < _numFrames && y < _size.Y; i++, y++) {
		if (SmallRect(_position + Coord(1, y), Coord(_size.X - 1, 1)).ContainsPoint(mousePos)) {
			_hoverIndex = i;

			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				if (_selection != i) {
					_selection = i;
					_selectionChanged = true;
				}
			}
			else if (_app->GetKeyboard()->IsKeyDown(Keys::LShiftKey)) {
				if (_selection != i) {
					_shiftHovering = true;
				}
			}
			break;
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentFrameList::Draw(AsciiImageBase& g) {

	for (int i = _scrollBar->GetListPosition().Y, y = 0; i < _numFrames && y < _size.Y; i++, y++) {

		ostringstream oss;
		oss << "frame " << (i + 1);

		if (i == _selection) {
			g.DrawString(_position + Coord(1, y), oss.str(), Pixel(' ', 0x9F));
		}
		else if (i == _hoverIndex) {
			if (_shiftHovering)
				g.DrawString(_position + Coord(1, y), oss.str(), Pixel(' ', 0x4F));
			else
				g.DrawString(_position + Coord(1, y), oss.str(), Pixel(' ', 0x0F));
		}
		else {
			g.DrawString(_position + Coord(1, y), oss.str(), Pixel(' ', 0x07));
		}
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|