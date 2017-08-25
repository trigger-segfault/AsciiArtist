/*=================================================================|
|* File:				SingleUndoAction.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "DrawUndoAction.h"

using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::UndoActions;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

DrawUndoAction::DrawUndoAction(const std::string name, int frameIndex)
 :	UndoAction(name, frameIndex),
	originalImage(nullptr),
	newImage(nullptr) {}

#pragma endregion
//============ ACTION ============
#pragma region Action

void DrawUndoAction::undo(CanvasSPtr canvas) {
	canvas->getImage()->setFrame(frameIndex, *originalImage);
	canvas->setFrameIndex(frameIndex);
}
void DrawUndoAction::redo(CanvasSPtr canvas) {
	canvas->getImage()->setFrame(frameIndex, *newImage);
	canvas->setFrameIndex(frameIndex);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void DrawUndoAction::setOriginalImage(const AsciiImageFrame& image) {
	originalImage = std::make_shared<AsciiImage>(image);
}
/* Sets the new state of the frame. */
void DrawUndoAction::setNewImage(const AsciiImageFrame& image) {
	newImage = std::make_shared<AsciiImage>(image);
}

#pragma endregion
//=================================================================|
