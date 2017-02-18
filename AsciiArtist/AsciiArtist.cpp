/*=================================================================|
|* File:				AsciiArtist.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "AsciiArtist.h"
#include "Frame.h"
#include "FrameCanvas.h"
#include "FrameFileDialog.h"
#include "ConsoleApp.h"
#include "Component.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::AsciiArtistApp;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region AsciiArtist
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::AsciiArtistApp::AsciiArtist::AsciiArtist(int argc, char** argv) : ConsoleApp(argc, argv) {
	this->_aa				= nullptr;
}
PowerConsole::AsciiArtistApp::AsciiArtist::~AsciiArtist() {
	
}
void PowerConsole::AsciiArtistApp::AsciiArtist::Initialize() {
	ConsoleApp::Initialize();

	this->_aa				= new AsciiArtistManager();
	this->_aa->Initialize(this);

	Frame* frameCanvas = new FrameCanvas(this->_aa);
	AddFrame(frameCanvas);
	Frame* frameFileDialog = new FrameFileDialog(this->_aa);
	AddFrame(frameFileDialog);
	_members.FrameCurrent	= frameCanvas;

}
void PowerConsole::AsciiArtistApp::AsciiArtist::Uninitialize() {
	this->_aa				= nullptr;



	ConsoleApp::Uninitialize();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::AsciiArtistApp::AsciiArtist::Update() {
	ConsoleApp::Update();
	
	//_aa->GetCurrentTool()->Update();

}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::AsciiArtistApp::AsciiArtist::Draw() {
	ConsoleApp::Draw();

	//_aa->GetCurrentTool()->Draw(*_members.Graphics);

}

#pragma endregion
//========== COMPONENTS ==========
#pragma region Components

	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|