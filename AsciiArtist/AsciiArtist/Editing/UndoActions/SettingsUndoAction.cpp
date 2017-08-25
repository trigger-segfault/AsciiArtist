/*=================================================================|
|* File:				SettingsUndoAction.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "SettingsUndoAction.h"

using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::UndoActions;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

SettingsUndoAction::SettingsUndoAction(ImageSettings originalSettings, ImageSettings newSettings)
 :	UndoAction("Settings"),
	originalImage(nullptr),
	originalSettings(originalSettings),
	newSettings(newSettings) {}

#pragma endregion
//============ ACTION ============
#pragma region Action

void SettingsUndoAction::undo(CanvasSPtr canvas) {
	if (originalImage != nullptr) {
		*canvas->getImage() = *originalImage;
	}
	else {
		canvas->getImage()->setFormat(originalSettings.format);
		canvas->getImage()->setBackground(originalSettings.background);
	}
	canvas->setFileVersion(originalSettings.fileVersion, originalSettings.smallFile);
}
void SettingsUndoAction::redo(CanvasSPtr canvas) {
	canvas->getImage()->setFormat(newSettings.format);
	canvas->getImage()->setBackground(newSettings.background);
	canvas->setFileVersion(newSettings.fileVersion, newSettings.smallFile);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void SettingsUndoAction::setOriginalImage(const AsciiAnimation& image) {
	originalImage = std::make_shared<AsciiAnimation>(image);
}

#pragma endregion
//=================================================================|
