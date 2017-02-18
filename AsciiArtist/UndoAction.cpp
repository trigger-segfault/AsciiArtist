/*=================================================================|
|* File:				UndoAction.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/16/2014								  *|
|*																  *|
|* Description: A structure for storing undo actions.			  *|
|=================================================================*/

#include "Stdafx.h"
#include "UndoAction.h"
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
#pragma region UndoAction
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::AsciiArtistApp::UndoAction::UndoAction() {
	this->_actionName			= "";
	this->_action				= Actions::None;
	this->_modifiedFrame		= -1;
	this->_beforeImage			= nullptr;
	this->_resultImage			= nullptr;
}
PowerConsole::AsciiArtistApp::UndoAction::UndoAction(const string& actionName, Actions action, int modifiedFrame) {
	this->_actionName			= actionName;
	this->_action				= action;
	this->_modifiedFrame		= modifiedFrame;
	this->_beforeImage			= nullptr;
	this->_resultImage			= nullptr;
}
PowerConsole::AsciiArtistApp::UndoAction::UndoAction(const string& actionName, Actions action, int modifiedFrame, AsciiAnimation* before, AsciiAnimation* result) {
	this->_actionName			= actionName;
	this->_action				= action;
	this->_modifiedFrame		= modifiedFrame;
	this->_beforeImage			= before;
	this->_resultImage			= result;
}
PowerConsole::AsciiArtistApp::UndoAction::~UndoAction() {
	if (this->_beforeImage != nullptr) {
		delete this->_beforeImage;
		this->_beforeImage		= nullptr;
	}
	if (this->_resultImage != nullptr) {
		delete this->_resultImage;
		this->_resultImage		= nullptr;
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Information

string PowerConsole::AsciiArtistApp::UndoAction::GetActionName() const {
	return _actionName;
}
void PowerConsole::AsciiArtistApp::UndoAction::SetActionName(const string& actionName) {
	this->_actionName = actionName;
}

Actions PowerConsole::AsciiArtistApp::UndoAction::GetAction() const {
	return _action;
}
void PowerConsole::AsciiArtistApp::UndoAction::SetAction(Actions action) {
	this->_action = action;
}

int PowerConsole::AsciiArtistApp::UndoAction::GetModifiedFrame() const {
	return _modifiedFrame;
}
void PowerConsole::AsciiArtistApp::UndoAction::SetModifiedFrame(int modifiedFrame) {
	this->_modifiedFrame = modifiedFrame;
}

AsciiAnimation* PowerConsole::AsciiArtistApp::UndoAction::GetBeforeImage() const {
	return _beforeImage;
}
void PowerConsole::AsciiArtistApp::UndoAction::SetBeforeImage(AsciiAnimation* beforeImage) {
	if (this->_beforeImage != nullptr) {
		delete this->_beforeImage;
	}
	this->_beforeImage = beforeImage;
}

AsciiAnimation* PowerConsole::AsciiArtistApp::UndoAction::GetResultImage() const {
	return _resultImage;
}
void PowerConsole::AsciiArtistApp::UndoAction::SetResultImage(AsciiAnimation* resultImage) {
	if (this->_resultImage != nullptr) {
		delete this->_resultImage;
	}
	this->_resultImage = resultImage;
}

#pragma endregion
//--------------------------------
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|