/*=================================================================|
|* File:				AnimationUndoAction.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ANIMATION_UNDO_ACTION_H
#define ANIMATION_UNDO_ACTION_H

#include <PowerConsole/Declarations.h>
#include <AsciiArtist/Editing/UndoAction.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::UndoActions {
//=================================================================|
// CLASSES														   |
//=================================================================/

class AnimationUndoAction : public UndoAction {
	//=========== MEMBERS ============
	#pragma region Members

	/* The original state of the image. */
	AsciiAnimationSPtr originalImage;
	/* The new state of the image. */
	AsciiAnimationSPtr newImage;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the undo action. */
	AnimationUndoAction(const std::string name);

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

	/* Sets the original state of the image. */
	void setOriginalImage(const AsciiAnimation& image);
	/* Sets the new state of the image. */
	void setNewImage(const AsciiAnimation& image);

	#pragma endregion
};

DECLARE_SMART_PTRS(AnimationUndoAction);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
