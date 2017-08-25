/*=================================================================|
|* File:				BlankTool.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef BLANK_TOOL_H
#define BLANK_TOOL_H

#include <string>
#include <AsciiArtist/Editing/Tool.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::Tools {
//=================================================================|
// CLASSES														   |
//=================================================================/

class BlankTool : public Tool {
	//========== CONSTANTS ===========
	#pragma region Constants


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members


	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tool. */
	BlankTool(CanvasSPtr canvas);

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
	void onMouseMoved(MouseEventArgs e) final;
	void onKey(KeyboardEventArgs e) final;
	void onKeyTyped(KeyTypedEventArgs e) final;
	void onTick(TickEventArgs e) final;
	void onPaint(PaintEventArgs e) final;

	#pragma endregion
};

DECLARE_SMART_PTRS(BlankTool);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
