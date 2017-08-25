/*=================================================================|
|* File:				FrameSpeedUndoAction.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FRAME_SPEED_UNDO_ACTION_H
#define FRAME_SPEED_UNDO_ACTION_H

#include <PowerConsole/Declarations.h>
#include <AsciiArtist/Editing/UndoAction.h>
#include <PowerConsole/Helpers/fixed_array.h>

using namespace AsciiArtist::Editing;
using namespace PowerConsole::Helpers;

namespace AsciiArtist::Editing::UndoActions {
//=================================================================|
// CLASSES														   |
//=================================================================/

class FrameSpeedUndoAction : public UndoAction {
	//=========== MEMBERS ============
	#pragma region Members

	/* The original animation speed of the image. */
	unsigned int originalSpeed;
	/* The new animation speed of the image. */
	unsigned int newSpeed;
	/* The original state of the image. */
	fixed_array<unsigned int> originalFrameSpeeds;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the undo action. */
	FrameSpeedUndoAction(unsigned int newSpeed);
	/* Constructs the undo action. */
	FrameSpeedUndoAction(int frameIndex, unsigned int originalSpeed, unsigned int newSpeed);

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
	void setOriginalFrameSpeeds(fixed_array<unsigned int>&& originalFrameSpeeds);

	#pragma endregion
};

DECLARE_SMART_PTRS(FrameSpeedUndoAction);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
