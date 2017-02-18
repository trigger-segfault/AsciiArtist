/*=================================================================|
|* File:				Tool.cpp								  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/13/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "Tool.h"
#include "Component.h"
#include "AsciiArtistManager.h"
#include "ConsoleApp.h"

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
#pragma region Tool
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::AsciiArtistApp::Tool::Tool(ToolTypes type, const string& name) {
	// Containment
	this->_aa				= nullptr;
	this->_app				= nullptr;

	// Information
	this->_type				= type;
	this->_name				= name;

	// Drawing
	this->_drawingBuffer	= nullptr;
	this->_drawing			= false;
}
PowerConsole::AsciiArtistApp::Tool::~Tool() {
	
}
void PowerConsole::AsciiArtistApp::Tool::Initialize(AsciiArtistManager* aa, ConsoleApp* app, AsciiAnimation* drawingBuffer) {
	this->_aa				= aa;
	this->_app				= app;
	this->_drawingBuffer	= drawingBuffer;
}
void PowerConsole::AsciiArtistApp::Tool::Uninitialize() {
	this->_drawingBuffer	= nullptr;
	this->_app				= nullptr;
	this->_aa				= nullptr;
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Information

ToolTypes PowerConsole::AsciiArtistApp::Tool::GetToolType() const {
	return _type;
}
string PowerConsole::AsciiArtistApp::Tool::GetName() const {
	return _name;
}
bool PowerConsole::AsciiArtistApp::Tool::IsDrawing() const {
	return _drawing;
}

#pragma endregion
//--------------------------------
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|