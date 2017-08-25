/*=================================================================|
|* File:				UndoAction.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef UNDO_ACTION_H
#define UNDO_ACTION_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <AsciiArtist/Toolbox/Controls/Canvas.h>

using namespace PowerConsole::Drawing;
using namespace AsciiArtist::Toolbox;
using namespace AsciiArtist::Toolbox::Controls;

namespace AsciiArtist::Editing {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* A abstract class for storing undo history and executing history navigation. */
class UndoAction {

	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The name of the action. */
	std::string name;
	/* The index of the modified frame. -1 for not relevant. */
	int frameIndex;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the undo action. */
	UndoAction(int frameIndex = -1);
	/* Constructs the undo action. */
	UndoAction(const std::string& name, int frameIndex = -1);

	#pragma endregion
	//============ ACTION ============
	#pragma region Action
	
	/* Undos this action. */
	virtual void undo(CanvasSPtr canvas) = 0;
	/* Redos this action. */
	virtual void redo(CanvasSPtr canvas) = 0;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the name of the action. */
	std::string getActionName() const;
	/* Gets the index of the modified frame. -1 for not relevant. */
	int getFrameIndex() const;

	#pragma endregion
};

DECLARE_SMART_PTRS(UndoAction);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
