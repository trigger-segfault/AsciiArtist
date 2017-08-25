/*=================================================================|
|* File:				SettingsUndoAction.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef _UNDO_ACTION_H
#define _UNDO_ACTION_H

#include <PowerConsole/Declarations.h>
#include <AsciiArtist/Editing/UndoAction.h>
#include <AsciiArtist/Editing/ImageSettings.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::UndoActions {
//=================================================================|
// CLASSES														   |
//=================================================================/

class SettingsUndoAction : public UndoAction {
	//=========== MEMBERS ============
	#pragma region Members

	/* The original state of the frame. */
	AsciiAnimationSPtr originalImage;
	/* The original settings of the ascii image. */
	ImageSettings originalSettings;
	/* The new settings of the ascii image. */
	ImageSettings newSettings;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the undo action. */
	SettingsUndoAction(ImageSettings originalSettings, ImageSettings newSettings);

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
	void setOriginalImage(const AsciiAnimation& image);

	#pragma endregion
};

DECLARE_SMART_PTRS(SettingsUndoAction);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
