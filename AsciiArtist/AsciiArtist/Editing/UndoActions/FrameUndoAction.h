/*=================================================================|
|* File:				FrameUndoAction.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FRAME_UNDO_ACTION_H
#define FRAME_UNDO_ACTION_H

#include <PowerConsole/Declarations.h>
#include <AsciiArtist/Editing/UndoAction.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::UndoActions {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The different types of frame undo actions. */
enum class FrameActionTypes {
	New,
	Duplicate,
	Delete,
	MoveDown,
	MoveUp,
	Rename
};

class FrameUndoAction : public UndoAction {
	//=========== MEMBERS ============
	#pragma region Members

	/* The action type for the frame undo action. */
	FrameActionTypes actionType;
	/* The original name of the frame for rename actions. */
	std::string originalName;
	/* The new name of the frame for rename actions. */
	std::string newName;
	/* The deleted frame. */
	AsciiImageSPtr deletedFrame;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the undo action. */
	FrameUndoAction(int frameIndex, FrameActionTypes actionType);

	#pragma endregion
	//============ ACTION ============
	#pragma region Action

	/* Undos this action. */
	void undo(CanvasSPtr canvas) final;
	/* Redos this action. */
	void redo(CanvasSPtr canvas) final;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	
	/* Sets the deleted frame to restore on undo. */
	void setDeletedFrame(const AsciiImageFrame& image);
	/* Sets the original name of the frame for rename actions. */
	void setOriginalName(const std::string& name);
	/* Sets the new name of the frame for rename actions. */
	void setNewName(const std::string& name);

	#pragma endregion
};

DECLARE_SMART_PTRS(FrameUndoAction);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
