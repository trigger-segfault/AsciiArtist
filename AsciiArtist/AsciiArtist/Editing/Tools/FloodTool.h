/*=================================================================|
|* File:				FloodTool.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FLOOD_TOOL_H
#define FLOOD_TOOL_H

#include <string>
#include <AsciiArtist/Editing/Tool.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::Tools {
//=================================================================|
// CLASSES														   |
//=================================================================/

class FloodTool : public Tool {
	
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tool. */
	FloodTool(CanvasSPtr canvas);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the options used by the tool. */
	fixed_array<std::string> getUsedOptions() const final;

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

DECLARE_SMART_PTRS(FloodTool);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
