/*=================================================================|
|* File:				ComponentFileList.cpp					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/18/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentFileList.h"
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
#pragma region ComponentFileList
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentFileList::ComponentFileList() : Component() {
	this->_fileList					= nullptr;
	this->_fileListLength			= 0;
	this->_imageList				= nullptr;
	this->_imageListLength			= 0;
	this->_scrollBar				= nullptr;

	this->_selection				= 0;
	this->_hoverIndex				= -1;
	this->_selectionChanged			= false;
	this->_selectionClicked			= false;
	this->_selectionDoubleClicked	= false;

	this->_imageMode				= 0;
}
PowerConsole::Components::ComponentFileList::~ComponentFileList() {
	
}
void PowerConsole::Components::ComponentFileList::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentFileList::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentFileList::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentFileList::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

int PowerConsole::Components::ComponentFileList::GetSelection() const {
	return _selection;
}
void PowerConsole::Components::ComponentFileList::SetScrollBar(ComponentScrollBar* scrollBar) {
	this->_scrollBar = scrollBar;
}
void PowerConsole::Components::ComponentFileList::SetFileList(File* fileList, int length) {
	this->_fileList = fileList;
	this->_fileListLength = length;
}
void PowerConsole::Components::ComponentFileList::SetImageList(AsciiImageHeader* imageList, int length) {
	this->_imageList = imageList;
	this->_imageListLength = length;

	_selection = -1;
}
int PowerConsole::Components::ComponentFileList::GetImageMode() const {
	return _imageMode;
}
void PowerConsole::Components::ComponentFileList::SetImageMode(int mode) {
	this->_imageMode = mode;
}
	
#pragma endregion
//--------------------------------
#pragma region Events

bool PowerConsole::Components::ComponentFileList::IsSelectionChanged() const {
	return _selectionChanged;
}
bool PowerConsole::Components::ComponentFileList::IsSelectionClicked() const {
	return _selectionClicked;
}
bool PowerConsole::Components::ComponentFileList::IsSelectionDoubleClicked() const {
	return _selectionDoubleClicked;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentFileList::Update() {
	bool hovering = GetBounds().ContainsPoint(_app->GetMouse()->GetPosition());
	bool down = _app->GetMouse()->IsButtonDown(MouseButtons::Left);

	Coord mousePos = _app->GetMouse()->GetPosition();
	_hoverIndex = -1;
	_selectionChanged = false;
	_selectionClicked = false;
	_selectionDoubleClicked = false;

	for (int i = _scrollBar->GetListPosition().Y, y = 0; i < _fileListLength && y < _size.Y; i++, y++) {
		if (SmallRect(_position + Coord(1, y), Coord(_size.X - 1, 1)).ContainsPoint(mousePos)) {
			_hoverIndex = i;

			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				if (_selection != i) {
					_selection = i;
					_selectionChanged = true;
				}
				_selectionClicked = true;
			}
			if (_app->GetMouse()->IsButtonDoubleClicked(MouseButtons::Left)) {
				if (_selection != i) {
					_selection = i;
					_selectionChanged = true;
				}
				_selectionDoubleClicked = true;
			}
			break;
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentFileList::Draw(AsciiImageBase& g) {

	int namePos = 2;
	int nameSize = 37;
	int dimensionsPos = 41;
	int dimensionsSize = 12;
	int framesPos = 55;
	int framesSize = 10;
	int fileSizePos = 67;
	int fileSizeSize = 8;

	if (_imageMode != 0)
		nameSize = 63;

	int numFolders = _fileListLength - _imageListLength;

	for (int i = _scrollBar->GetListPosition().Y, y = 0; i < _fileListLength && y < _size.Y; i++, y++) {
		bool imageError = false;

		if (_fileList[i].Type != FileTypes::File && _fileList[i].Type != FileTypes::Link) {
			if (_fileList[i].IsParentDirectory) {
				g.SetPixel(_position + Coord(namePos, y), Pixel(17, 0x07));
				if (_fileList[i].Type == FileTypes::Computer)
					g.DrawString(_position + Coord(namePos + 2, y), "Computer", Pixel(' ', 0x07));
				else
					g.DrawString(_position + Coord(namePos + 2, y), "..", Pixel(' ', 0x07));
			}
			else {
				g.SetPixel(_position + Coord(namePos, y), Pixel(16, 0x07));
				string name = _fileList[i].Name;
				if (_fileList[i].Type == FileTypes::Drive)
					name = _fileList[i].NameProper;
				g.DrawString(_position + Coord(namePos + 2, y),
					((name.length() > nameSize - 2) ? (name.substr(0, nameSize - 2)) : name),
					Pixel(' ', 0x07));
			}
		}
		else {
			if (_imageMode == 0) {
				imageError = (_imageList[i - numFolders].Size == Coord::Zero);
			}

			// File Name
			g.DrawString(_position + Coord(namePos + 2, y),
					((_fileList[i].Name.length() > nameSize - 2) ? (_fileList[i].Name.substr(0, nameSize - 2)) : _fileList[i].Name),
					Pixel(' ', imageError ? 0x0C : 0x07));

			if (_imageMode == 0 && !imageError) {
				// Dimensions
				ostringstream oss1;
				oss1 << _imageList[i - numFolders].Size.X << "x" << _imageList[i - numFolders].Size.Y;
				g.DrawString(_position + Coord(dimensionsPos, y),
						((oss1.str().length() > dimensionsSize - 2) ? (oss1.str().substr(0, dimensionsSize - 2)) : oss1.str()),
						Pixel(' ', 0x07));

				// Frames
				ostringstream oss2;
				oss2 << _imageList[i - numFolders].Frames << " frames";
				g.DrawString(_position + Coord(framesPos, y),
						((oss2.str().length() > framesSize - 2) ? (oss2.str().substr(0, framesSize - 2)) : oss2.str()),
						Pixel(' ', 0x07));
			}
			
			// File Size
			ostringstream oss3;
			oss3 << (_fileList[i].FileSize / 0x400 + 1);
			g.DrawString(_position + Coord(fileSizePos + fileSizeSize - 4, y), oss3.str(), Pixel(' ', imageError ? 0x0C : 0x07), Alignment::Right);
			g.DrawString(_position + Coord(fileSizePos + fileSizeSize - 2, y), "kb", Pixel(' ', imageError ? 0x0C : 0x07));
		}

		if (i == _selection || i == _hoverIndex) {
			g.SetColor(_position + Coord(0, y), Coord(_size.X, 1), imageError ? 0xF4 : 0xF8);
		}
	}

	if (_imageMode != 0)
		g.SetPixel(Coord(64, 3), Coord(26, 1), Pixel(' ', 0x07));
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|