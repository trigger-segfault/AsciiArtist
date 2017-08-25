/*=================================================================|
|* File:				CircleTool.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CIRCLE_TOOL_H
#define CIRCLE_TOOL_H

#include <string>
#include <AsciiArtist/Editing/Tool.h>

namespace AsciiArtist::Editing::Tools {
//=================================================================|
// CLASSES														   |
//=================================================================/

class CircleTool : public Tool {

	//=========== MEMBERS ============
	#pragma region Members

	/* True if the secondary pixel is being drawn. */
	bool secondary;
	/* The starting point of the square. */
	Point2I startPoint;
	/* True if shift is down. */
	bool shift;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tool. */
	CircleTool(CanvasSPtr canvas);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the options used by the tool. */
	fixed_array<std::string> getUsedOptions() const final;
	/* Gets the selection size of teh tool. */
	Point2I getSelectionSize() const final;

	#pragma endregion
	//=========== EDITING ============
	#pragma region Editing

	/* Finishes drawing with the tool. */
	void finalize() final;
	/* Cancels drawing with the tool. */
	void cancel() final;

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers

	/* Draws changes made with the tool. */
	void drawChanges();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onMouseButton(MouseButtonEventArgs e) final;
	void onMouseMoved(MouseEventArgs e) final;
	void onKey(KeyboardEventArgs e) final;
	void onPaint(PaintEventArgs e) final;

	#pragma endregion
};

DECLARE_SMART_PTRS(CircleTool);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
