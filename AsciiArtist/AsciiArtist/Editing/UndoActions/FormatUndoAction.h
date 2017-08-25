/*=================================================================|
|* File:				FormatUndoAction.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FORMAT_UNDO_ACTION_H
#define FORMAT_UNDO_ACTION_H

#include <PowerConsole/Declarations.h>
#include <AsciiArtist/Editing/UndoAction.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::UndoActions {
//=================================================================|
// CLASSES														   |
//=================================================================/

class FormatUndoAction : public UndoAction {
	//=========== MEMBERS ============
	#pragma region Members

	/* The original state of the frame. */
	AsciiAnimationSPtr originalImage;
	/* The original format of the image. */
	ImageFormats originalFormat;
	/* The new format of the image. */
	ImageFormats newFormat;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the undo action. */
	FormatUndoAction(ImageFormats originalFormat, ImageFormats newFormat);

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

DECLARE_SMART_PTRS(FormatUndoAction);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
