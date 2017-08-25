/*=================================================================|
|* File:				BlankUndoAction.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef _UNDO_ACTION_H
#define _UNDO_ACTION_H

#include <PowerConsole/Declarations.h>
#include <AsciiArtist/Editing/UndoAction.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::UndoActions {
//=================================================================|
// CLASSES														   |
//=================================================================/

class BlankUndoAction : public UndoAction {
	//=========== MEMBERS ============
	#pragma region Members



	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the undo action. */
	BlankUndoAction();

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



	#pragma endregion
};

DECLARE_SMART_PTRS(BlankUndoAction);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
