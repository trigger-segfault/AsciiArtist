/*=================================================================|
|* File:				EyedropperTool.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef EYEDROPPER_TOOL_H
#define EYEDROPPER_TOOL_H

#include <string>
#include <AsciiArtist/Editing/Tool.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::Tools {
//=================================================================|
// CLASSES														   |
//=================================================================/

class EyedropperTool : public Tool {
	//========== CONSTANTS ===========
	#pragma region Constants


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* The index of the previous tool before switching to the eyedropper. */
	int previousToolIndex;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tool. */
	EyedropperTool(CanvasSPtr canvas);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the options used by the tool. */
	fixed_array<std::string> getUsedOptions() const final;
	/* Sets the index of the previous tool before switching to the eyedropper. */
	void setPreviousToolIndex(int index);


	#pragma endregion
	//=========== EDITING ============
	#pragma region Editing

	/* Finishes drawing with the tool. */
	void finalize() final;
	/* Cancels drawing with the tool. */
	void cancel() final;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onMouseButton(MouseButtonEventArgs e) final;

	#pragma endregion
};

DECLARE_SMART_PTRS(EyedropperTool);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
