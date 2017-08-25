/*=================================================================|
|* File:				UndoAction.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "UndoAction.h"

using namespace AsciiArtist::Editing;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

UndoAction::UndoAction(int frameIndex)
 :	name(""),
	frameIndex(frameIndex) {}
UndoAction::UndoAction(const std::string& name, int frameIndex)
 :	name(name),
	frameIndex(frameIndex) {}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string UndoAction::getActionName() const {
	return name;
}
int UndoAction::getFrameIndex() const {
	return frameIndex;
}

#pragma endregion
//=================================================================|
