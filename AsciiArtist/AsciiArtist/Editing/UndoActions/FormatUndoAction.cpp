/*=================================================================|
|* File:				FormatUndoAction.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FormatUndoAction.h"

using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::UndoActions;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

FormatUndoAction::FormatUndoAction(ImageFormats originalFormat, ImageFormats newFormat)
 :	UndoAction("Format"),
	originalImage(nullptr),
	originalFormat(originalFormat),
	newFormat(newFormat) {}

#pragma endregion
//============ ACTION ============
#pragma region Action

void FormatUndoAction::undo(CanvasSPtr canvas) {
	if (originalImage != nullptr) {
		*canvas->getImage() = *originalImage;
	}
	else {
		canvas->getImage()->setFormat(originalFormat);
	}
}
void FormatUndoAction::redo(CanvasSPtr canvas) {
	canvas->getImage()->setFormat(newFormat);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void FormatUndoAction::setOriginalImage(const AsciiAnimation& image) {
	originalImage = std::make_shared<AsciiAnimation>(image);
}

#pragma endregion
//=================================================================|
