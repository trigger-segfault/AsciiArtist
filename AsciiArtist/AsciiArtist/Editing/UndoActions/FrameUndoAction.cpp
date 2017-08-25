/*=================================================================|
|* File:				FrameUndoAction.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FrameUndoAction.h"

using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::UndoActions;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

FrameUndoAction::FrameUndoAction(int frameIndex, FrameActionTypes actionType)
 :	UndoAction(frameIndex),
	actionType(actionType),
	deletedFrame(nullptr),
	originalName(""),
	newName("") {
	switch (actionType) {
	case FrameActionTypes::New:			name = "New frame"; break;
	case FrameActionTypes::Duplicate:	name = "Duplicate frame"; break;
	case FrameActionTypes::Delete:		name = "Delete frame"; break;
	case FrameActionTypes::MoveDown:	name = "Move frame down"; break;
	case FrameActionTypes::MoveUp:		name = "Move frame Up"; break;
	case FrameActionTypes::Rename:		name = "Rename frame"; break;
	}
}

#pragma endregion
//============ ACTION ============
#pragma region Action

void FrameUndoAction::undo(CanvasSPtr canvas) {
	switch (actionType) {
	case FrameActionTypes::New:
	case FrameActionTypes::Duplicate:
		canvas->removeFrameUndoAction(frameIndex + 1);
		break;
	case FrameActionTypes::Delete:
		canvas->insertFrameUndoAction(frameIndex + 1, *deletedFrame);
		break;
	case FrameActionTypes::MoveDown:
		canvas->moveFrameUndoAction(frameIndex + 1, frameIndex);
		break;
	case FrameActionTypes::MoveUp:
		canvas->moveFrameUndoAction(frameIndex - 1, frameIndex);
		break;
	case FrameActionTypes::Rename:
		canvas->renameFrameUndoAction(frameIndex, originalName);
	}
	canvas->setFrameIndex(frameIndex);
}
void FrameUndoAction::redo(CanvasSPtr canvas) {
	switch (actionType) {
	case FrameActionTypes::New:
		canvas->insertFrameUndoAction(frameIndex + 1);
		break;
	case FrameActionTypes::Duplicate:
		canvas->insertFrameUndoAction(frameIndex + 1, canvas->getImage()->at(frameIndex));
		break;
	case FrameActionTypes::Delete:
		canvas->removeFrameUndoAction(frameIndex);
		break;
	case FrameActionTypes::MoveDown:
		canvas->moveFrameUndoAction(frameIndex, frameIndex + 1);
		break;
	case FrameActionTypes::MoveUp:
		canvas->moveFrameUndoAction(frameIndex, frameIndex - 1);
		break;
	case FrameActionTypes::Rename:
		canvas->renameFrameUndoAction(frameIndex, newName);
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void FrameUndoAction::setDeletedFrame(const AsciiImageFrame& image) {
	deletedFrame = std::make_shared<AsciiImage>(image);
}
void FrameUndoAction::setOriginalName(const std::string& name) {
	originalName = name;
}
void FrameUndoAction::setNewName(const std::string& name) {
	newName = name;
}

#pragma endregion
//=================================================================|
