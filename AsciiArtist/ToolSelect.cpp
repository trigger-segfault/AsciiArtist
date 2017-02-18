/*=================================================================|
|* File:				ToolSelect.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/15/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "ToolSelect.h"
#include "Component.h"
#include "ConsoleApp.h"
#include "AsciiArtistManager.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;
using namespace PowerConsole::AsciiArtistApp;

//=================================================================|
// DEFINITIONS													   |
//=================================================================/

#define FixRect(point, size)		(point += Coord(size.X < 0 ? size.X + 1 : 0, size.Y < 0 ? size.Y + 1 : 0)); \
									(size *= Coord(size.X < 0 ? -1 : 1, size.Y < 0 ? -1 : 1))

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ToolSelect
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::AsciiArtistApp::ToolSelect::ToolSelect() : Tool(ToolTypes::Select, "Select") {
	// Selection
	this->_hasSelection		= false;
	this->_selection		= nullptr;

	// Dragging
	this->_startPoint		= Coord::Zero;
	this->_dragPoint		= Coord::Zero;
	this->_gripPoint		= Coord::Zero;
	this->_dragging			= false;
	this->_copied			= false;

	// Visual
	this->_boxTimer			= 0U;
	this->_boxState			= false;
}
PowerConsole::AsciiArtistApp::ToolSelect::~ToolSelect() {
	
}
void PowerConsole::AsciiArtistApp::ToolSelect::Initialize(AsciiArtistManager* aa, ConsoleApp* app, AsciiAnimation* drawingBuffer) {
	Tool::Initialize(aa, app, drawingBuffer);
}
void PowerConsole::AsciiArtistApp::ToolSelect::Uninitialize() {
	Tool::Uninitialize();
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::AsciiArtistApp::ToolSelect::Update() {
	
	if (!_drawing) {
		bool pressed = false;
		if (_aa->GetCanvas()->IsMouseInsideCanvas()) {
			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				pressed = true;
			}
		}

		if (pressed) {
			_drawing = true;
			_startPoint = _aa->GetCanvas()->GetImageMousePosition();
			
			_drawingBuffer->GetFrame(0).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(1).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(2).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		}
	}
	else if (!_hasSelection) {
		Coord endPoint = _aa->GetCanvas()->GetImageMousePosition();
		if (endPoint.X < _startPoint.X)
			endPoint.X -= 1;
		else
			endPoint.X += 1;
		if (endPoint.Y < _startPoint.Y)
			endPoint.Y -= 1;
		else
			endPoint.Y += 1;
		
		Coord size = endPoint - _startPoint;
		Coord startPoint = _startPoint;

		FixRect(startPoint, size);

		if (_app->GetMouse()->IsButtonPressed(MouseButtons::Right)) {
			_hasSelection = false;
			Cancel();
		}
		else if (_app->GetMouse()->IsButtonReleased(MouseButtons::Left)) {
			if (_aa->GetCanvas()->GetImageMousePosition() - _startPoint != Coord::Zero) {
				_hasSelection = true;
				_startPoint = startPoint;
				_dragPoint = startPoint;
				_selection = new AsciiImage(startPoint, size, ImageFormats::FormatAttributes, _aa->GetImage()->GetBackground(), _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()));
				_boxTimer = clock();
				_boxState = false;
			}
			else {
				_hasSelection = false;
				Cancel();
			}
		}
		else {
			// Draw the selection box outline
			_drawingBuffer->GetFrame(2).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(2).DrawRect(startPoint, size, Pixel(' ', 0x00, PixelAttributes::AttributeInvert), false, false,
				PixelAttributes::AttributeInvert, PixelOptions::OptionCopySpecial);
		}
	}
	else if (!_dragging) {
		Coord mousePos = _aa->GetCanvas()->GetImageMousePosition();
		if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
			if (SmallRect(_dragPoint, _selection->Size()).ContainsPoint(mousePos)) {
				if (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey)) {
					FinalizeCopy();
					_gripPoint = mousePos - _dragPoint;
					_dragging = true;
				}
				else {
					_gripPoint = mousePos - _dragPoint;
					_dragging = true;
				}
			}
			else if (SmallRect(_aa->GetCanvas()->GetPosition(), _aa->GetCanvas()->GetSize()).ContainsPoint(_app->GetMouse()->GetPosition())) {
				Finalize();
			}
			
		}
		else if (_app->GetMouse()->IsButtonPressed(MouseButtons::Right)) {
			if (SmallRect(_aa->GetCanvas()->GetPosition(), _aa->GetCanvas()->GetSize()).ContainsPoint(_app->GetMouse()->GetPosition())) {
				Finalize();
			}
		}
	}
	else {
		Coord mousePos = _aa->GetCanvas()->GetImageMousePosition();
		if (_app->GetMouse()->IsButtonReleased(MouseButtons::Left)) {
			_dragPoint = mousePos - _gripPoint;
			_dragging = false;
		}
		else {
			_dragPoint = mousePos - _gripPoint;
		}
	}

	if (_hasSelection) {
		unsigned int time = clock();
		if (time - _boxTimer >= 560) {
			_boxTimer += 560;
			_boxState = !_boxState;
		}

		_drawingBuffer->GetFrame(0).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		_drawingBuffer->GetFrame(1).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		_drawingBuffer->GetFrame(2).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

		unsigned short attributes = 0x0;
		if (_aa->IsCharacterDrawn())
			attributes |= PixelAttributes::AttributeChar;
		if (_aa->IsFColorDrawn())
			attributes |= PixelAttributes::AttributeFColor | PixelAttributes::AttributeFInvert;
		if (_aa->IsBColorDrawn())
			attributes |= PixelAttributes::AttributeBColor | PixelAttributes::AttributeBInvert;
		
		Pixel background = _aa->GetImage()->GetBackground();
		background.Attributes |= PixelAttributes::AttributeFill;
		
		if (!_copied) {
			_drawingBuffer->GetFrame(0).DrawImage(_startPoint, *_selection, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			_drawingBuffer->GetFrame(0).SetPixel(_startPoint, _selection->Size(), background, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		}
		_drawingBuffer->GetFrame(1).DrawImage(_dragPoint, *_selection, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

		int xoffset1 = _boxState ? 0 : 1;
		int yoffset1 = (xoffset1 + _selection->Size().X) % 2 == 0 ? 0 : 1;
		int xoffset2 = (yoffset1 + _selection->Size().Y) % 2 == 0 ? 0 : 1;
		int yoffset2 = (xoffset2 + _selection->Size().X) % 2 == 0 ? 0 : 1;

		for (int i = xoffset1; i < _selection->Size().X; i += 2) {
			_drawingBuffer->GetFrame(2).SetAttributes(_dragPoint + Coord(i, 0), PixelAttributes::AttributeInvert);
		}
		for (int i = yoffset1 + 1; i + 1 < _selection->Size().Y; i += 2) {
			_drawingBuffer->GetFrame(2).SetAttributes(_dragPoint + Coord(_selection->Size().X - 1, i), PixelAttributes::AttributeInvert, PixelAttributes::AttributeAll | PixelOptions::OptionCopySpecial);
		}
		for (int i = xoffset2; i < _selection->Size().X && _selection->Size().X > 1; i += 2) {
			_drawingBuffer->GetFrame(2).SetAttributes(_dragPoint + _selection->Size() - Coord(1 + i, 1), PixelAttributes::AttributeInvert, PixelAttributes::AttributeAll | PixelOptions::OptionCopySpecial);
		}
		for (int i = yoffset2 + 1; i + 1 < _selection->Size().Y && _selection->Size().Y > 1; i += 2) {
			_drawingBuffer->GetFrame(2).SetAttributes(_dragPoint + Coord(0, _selection->Size().Y - 1 - i), PixelAttributes::AttributeInvert, PixelAttributes::AttributeAll | PixelOptions::OptionCopySpecial);
		}
	}

	if (_drawing) {
		if (_hasSelection) {
			_aa->GetStatusBar()->SetSizeCoord(_selection->Size());
		}
		else {
			Coord startPoint = _startPoint;
			Coord endPoint = _aa->GetCanvas()->GetImageMousePosition();
			if (endPoint.X < _startPoint.X)
				endPoint.X -= 1;
			else
				endPoint.X += 1;
			if (endPoint.Y < _startPoint.Y)
				endPoint.Y -= 1;
			else
				endPoint.Y += 1;
			Coord size = endPoint - startPoint;
			FixRect(startPoint, size);

			_aa->GetStatusBar()->SetSizeCoord(size);
		}
	}
	else {
		_aa->GetStatusBar()->SetSizeCoord(-Coord::One);
	}
}
void PowerConsole::AsciiArtistApp::ToolSelect::Activate() {
	
}
void PowerConsole::AsciiArtistApp::ToolSelect::Finalize() {
	if (_drawing && _hasSelection) {
		UndoAction* undo = new UndoAction("Selection", Actions::Draw, _aa->GetCurrentFrame());
		if (_aa->IsUndoEnabled())
			undo->SetBeforeImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));

		unsigned short attributes = 0x0;
		if (_aa->IsCharacterDrawn())
			attributes |= PixelAttributes::AttributeChar;
		if (_aa->IsFColorDrawn())
			attributes |= PixelAttributes::AttributeFColor | PixelAttributes::AttributeFInvert;
		if (_aa->IsBColorDrawn())
			attributes |= PixelAttributes::AttributeBColor | PixelAttributes::AttributeBInvert;
		
		if (!_copied) {
			Pixel background = _aa->GetImage()->GetBackground();
			_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).SetPixel(_startPoint, _selection->Size(), background, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		}
		_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).DrawImage(_dragPoint, *_selection, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

		if (_selection != nullptr) {
			delete _selection;
			_selection = nullptr;
		}
		
		if (_aa->IsUndoEnabled())
		undo->SetResultImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));
		_aa->AddUndo(undo);
		
		_drawing = false;
		_hasSelection = false;
		_dragging = false;
		_copied = false;
	}
}
void PowerConsole::AsciiArtistApp::ToolSelect::FinalizeCopy() {
	if (_drawing && _hasSelection) {
		UndoAction* undo = new UndoAction("Selection", Actions::Draw, _aa->GetCurrentFrame());
		if (_aa->IsUndoEnabled())
			undo->SetBeforeImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));

		unsigned short attributes = 0x0;
		if (_aa->IsCharacterDrawn())
			attributes |= PixelAttributes::AttributeChar;
		if (_aa->IsFColorDrawn())
			attributes |= PixelAttributes::AttributeFColor | PixelAttributes::AttributeFInvert;
		if (_aa->IsBColorDrawn())
			attributes |= PixelAttributes::AttributeBColor | PixelAttributes::AttributeBInvert;
		
		if (!_copied) {
			Pixel background = _aa->GetImage()->GetBackground();
			_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).SetPixel(_startPoint, _selection->Size(), background, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		}
		_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).DrawImage(_dragPoint, *_selection, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);

		if (_aa->IsUndoEnabled())
			undo->SetResultImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));
		_aa->AddUndo(undo);
		
		_copied = true;
	}
}
void PowerConsole::AsciiArtistApp::ToolSelect::FinalizeDelete() {
	if (_drawing && _hasSelection) {
		UndoAction* undo = new UndoAction("Selection", Actions::Draw, _aa->GetCurrentFrame());
		if (_aa->IsUndoEnabled())
			undo->SetBeforeImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));

		unsigned short attributes = 0x0;
		if (_aa->IsCharacterDrawn())
			attributes |= PixelAttributes::AttributeChar;
		if (_aa->IsFColorDrawn())
			attributes |= PixelAttributes::AttributeFColor | PixelAttributes::AttributeFInvert;
		if (_aa->IsBColorDrawn())
			attributes |= PixelAttributes::AttributeBColor | PixelAttributes::AttributeBInvert;
		
		if (!_copied) {
			Pixel background = _aa->GetImage()->GetBackground();
			_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).SetPixel(_startPoint, _selection->Size(), background, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		}
		
		if (_selection != nullptr) {
			delete _selection;
			_selection = nullptr;
		}
		
		if (_aa->IsUndoEnabled())
			undo->SetResultImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));
		_aa->AddUndo(undo);
		
		_drawing = false;
		_hasSelection = false;
		_dragging = false;
		_copied = false;
	}
}
void PowerConsole::AsciiArtistApp::ToolSelect::Cancel() {
	if (_drawing) {
		_drawing = false;
		_hasSelection = false;
		_dragging = false;
		_copied = false;
		if (_selection != nullptr) {
			delete _selection;
			_selection = nullptr;
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::AsciiArtistApp::ToolSelect::Draw(AsciiImageBase& g) {

	if (_drawing) {
		_drawingBuffer->GetFrame(4).DrawImage(Coord::Zero, _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		
		unsigned short attributes = 0x0;
		if (_aa->IsCharacterDrawn())
			attributes |= PixelAttributes::AttributeChar;
		if (_aa->IsFColorDrawn())
			attributes |= PixelAttributes::AttributeFColor | PixelAttributes::AttributeFInvert;
		if (_aa->IsBColorDrawn())
			attributes |= PixelAttributes::AttributeBColor | PixelAttributes::AttributeBInvert;

		if (_hasSelection) {
			if (!_copied) {
				Pixel background = _aa->GetImage()->GetBackground();
				_drawingBuffer->GetFrame(4).SetPixel(_startPoint, _selection->Size(), background, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			}

			_drawingBuffer->GetFrame(4).DrawImage(_dragPoint, *_selection, attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		}
		_drawingBuffer->GetFrame(4).DrawImage(Coord::Zero,
			_drawingBuffer->GetFrame(2), PixelAttributes::AttributeAll, PixelOptions::OptionCopySpecial);
	}
}

#pragma endregion
//=========== ACTIONS ============
#pragma region Actions

void PowerConsole::AsciiArtistApp::ToolSelect::Copy() {
	if (_hasSelection) {
		if (_selection->SaveClipboard()) {
			_aa->SetMessage("Image Copied");
		}
		else {
			_aa->SetUrgentMessage("Error Copying to Clipboard");
		}
	}
	else {
		_aa->SetMessage("Nothing Selected");
	}
}
void PowerConsole::AsciiArtistApp::ToolSelect::Cut() {
	if (_hasSelection) {
		if (_selection->SaveClipboard()) {
			FinalizeDelete();
			_aa->SetMessage("Image Cut");
		}
		else {
			_aa->SetUrgentMessage("Error Copying to Clipboard");
		}
	}
	else {
		_aa->SetMessage("Nothing Selected");
	}
}
void PowerConsole::AsciiArtistApp::ToolSelect::Delete() {
	if (_hasSelection) {
		FinalizeDelete();
		_aa->SetMessage("Image Deleted");
	}
	else {
		_aa->SetMessage("Nothing Selected");
	}
}
void PowerConsole::AsciiArtistApp::ToolSelect::Paste() {
	AsciiImage* _newSelection = new AsciiImage();
	if (_newSelection->LoadClipboard()) {
		if (_drawing) {
			if (_hasSelection) {
				Finalize();
			}
			else {
				Cancel();
			}
		}
		
		_selection = _newSelection;
		_startPoint = Coord::Zero;
		_dragPoint = Coord::Zero;
		_hasSelection = true;
		_drawing = true;
		_copied = true;
		_boxTimer = clock();
		_boxState = false;
		
		_drawingBuffer->GetFrame(0).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		_drawingBuffer->GetFrame(1).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		_drawingBuffer->GetFrame(2).Fill(Pixel(' ', 0x00, PixelAttributes::AttributeNone), PixelAttributes::AttributeAll, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
		
		_aa->SetMessage("Image Pasted");
	}
	else {
		delete _newSelection;
		_newSelection = nullptr;

		_aa->SetMessage("Clipboard Empty");
	}
}
void PowerConsole::AsciiArtistApp::ToolSelect::SelectAll() {
	if (_drawing) {
		if (_hasSelection) {
			Finalize();
		}
		else {
			Cancel();
		}
	}
	_selection = new AsciiImage(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()));
	_startPoint = Coord::Zero;
	_dragPoint = Coord::Zero;
	_hasSelection = true;
	_drawing = true;
	_copied = false;
	_boxTimer = clock();
	_boxState = false;
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|