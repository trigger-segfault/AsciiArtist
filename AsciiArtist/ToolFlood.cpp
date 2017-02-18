/*=================================================================|
|* File:				ToolFlood.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/15/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "ToolFlood.h"
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
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ToolFlood
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::AsciiArtistApp::ToolFlood::ToolFlood() : Tool(ToolTypes::Flood, "Flood Fill") {
	this->_secondary		= false;
	this->_startPoint		= Coord::Zero;
}
PowerConsole::AsciiArtistApp::ToolFlood::~ToolFlood() {
	
}
void PowerConsole::AsciiArtistApp::ToolFlood::Initialize(AsciiArtistManager* aa, ConsoleApp* app, AsciiAnimation* drawingBuffer) {
	Tool::Initialize(aa, app, drawingBuffer);
}
void PowerConsole::AsciiArtistApp::ToolFlood::Uninitialize() {
	Tool::Uninitialize();
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::AsciiArtistApp::ToolFlood::Update() {
	
	if (!_drawing) {
		bool pressed = false;
		if (_aa->GetCanvas()->IsMouseInsideCanvas()) {
			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				pressed = true;
				_secondary = false;
			}
			else if (_app->GetMouse()->IsButtonPressed(MouseButtons::Right)) {
				pressed = true;
				_secondary = true;
			}
		}

		if (pressed) {
			_drawing = true;
			_startPoint = _aa->GetCanvas()->GetImageMousePosition();
			Finalize();
		}
	}

	_aa->GetStatusBar()->SetSizeCoord(-Coord::One);
}
void PowerConsole::AsciiArtistApp::ToolFlood::Activate() {
	
}
void PowerConsole::AsciiArtistApp::ToolFlood::Finalize() {
	if (_drawing) {
		UndoAction* undo = new UndoAction("Flood Fill", Actions::Draw, _aa->GetCurrentFrame());
		if (_aa->IsUndoEnabled())
			undo->SetBeforeImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));

		unsigned short attributes = 0x0;
		if (_aa->IsCharacterDrawn())
			attributes |= PixelAttributes::AttributeChar;
		if (_aa->IsFColorDrawn())
			attributes |= PixelAttributes::AttributeFColor | PixelAttributes::AttributeFInvert;
		if (_aa->IsBColorDrawn())
			attributes |= PixelAttributes::AttributeBColor | PixelAttributes::AttributeBInvert;
		
		Pixel drawingPixel = (_secondary ? _aa->GetSecondaryPixel() : _aa->GetPrimaryPixel());
			_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).DrawFloodFill(_startPoint, drawingPixel, _aa->IsPixelSpecific(), attributes, PixelOptions::OptionReplace | PixelOptions::OptionCopySpecial);
			
		if (_aa->IsUndoEnabled())
			undo->SetResultImage(new AsciiAnimation(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame())));
		_aa->AddUndo(undo);
		
		_drawing = false;
	}
}
void PowerConsole::AsciiArtistApp::ToolFlood::Cancel() {
	if (_drawing) {
		_drawing = false;
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::AsciiArtistApp::ToolFlood::Draw(AsciiImageBase& g) {

}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|