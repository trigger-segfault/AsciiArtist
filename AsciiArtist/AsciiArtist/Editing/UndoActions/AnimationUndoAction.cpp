/*=================================================================|
|* File:				AnimationUndoAction.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AnimationUndoAction.h"

using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::UndoActions;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Constructors

AnimationUndoAction::AnimationUndoAction(const std::string name)
 :	UndoAction(name),
	originalImage(nullptr),
	newImage(nullptr) {}

#pragma endregion
//============ ACTION ============
#pragma region Action

void AnimationUndoAction::undo(CanvasSPtr canvas) {
	*canvas->getImage() = *originalImage;
}
void AnimationUndoAction::redo(CanvasSPtr canvas) {
	*canvas->getImage() = *newImage;
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void AnimationUndoAction::setOriginalImage(const AsciiAnimation& image) {
	originalImage = std::make_shared<AsciiAnimation>(image);
}
/* Sets the new state of the frame. */
void AnimationUndoAction::setNewImage(const AsciiAnimation& image) {
	newImage = std::make_shared<AsciiAnimation>(image);
}

#pragma endregion
//=================================================================|
