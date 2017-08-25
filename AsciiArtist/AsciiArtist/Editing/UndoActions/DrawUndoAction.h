/*=================================================================|
|* File:				DrawUndoAction.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef DRAW_UNDO_ACTION_H
#define DRAW_UNDO_ACTION_H

#include <PowerConsole/Declarations.h>
#include <AsciiArtist/Editing/UndoAction.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::UndoActions {
//=================================================================|
// CLASSES														   |
//=================================================================/

class DrawUndoAction : public UndoAction {
	//=========== MEMBERS ============
	#pragma region Members

	/* The original state of the frame. */
	AsciiImageSPtr originalImage;
	/* The new state of the frame. */
	AsciiImageSPtr newImage;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the undo action. */
	DrawUndoAction(const std::string name, int frameIndex);

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

	/* Sets the original state of the frame. */
	void setOriginalImage(const AsciiImageFrame& image);
	/* Sets the new state of the frame. */
	void setNewImage(const AsciiImageFrame& image);

	#pragma endregion
};

DECLARE_SMART_PTRS(DrawUndoAction);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
