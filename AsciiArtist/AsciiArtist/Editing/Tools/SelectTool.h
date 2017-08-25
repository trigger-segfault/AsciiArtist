/*=================================================================|
|* File:				SelectTool.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SELECT_TOOL_H
#define SELECT_TOOL_H

#include <string>
#include <AsciiArtist/Editing/Tool.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::Tools {
//=================================================================|
// CLASSES														   |
//=================================================================/

class SelectTool : public Tool {
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The time in milliseconds it takes for the selection bounds to change states. */
	static const int BOUNDS_FLASH_TIME = 560;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* True if something has been selected. */
	//bool hasSelection;
	/* The currently selected image. */
	AsciiImageSPtr selection;
	/* The starting point of the selection OR the original location of the selection. */
	Point2I startPoint;
	/* The current position of the selection after dragging. */
	Point2I selectionPoint;
	/* The grip position of the selection when dragging. */
	Point2I gripPoint;
	/* True if the selection is currently being dragged. */
	bool dragging;
	/* True if the selection was copied and shouldn't draw a shadow. */
	bool copied;
	/* True if shift is down. */
	bool shift;

	/* The timer for highlighting the selection bounds. */
	int boundsTimer;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tool. */
	SelectTool(CanvasSPtr canvas);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the options used by the tool. */
	fixed_array<std::string> getUsedOptions() const final;
	/* Gets the selection size of teh tool. */
	Point2I getSelectionSize() const final;
	/* Returns true if the tool currently has a selection. */
	bool hasSelection() const;
	/* Gets the boundaries of the current selection. */
	Rectangle2I getSelectionBounds() const;

	#pragma endregion
	//=========== ACTIONS ============
	#pragma region Actions

	/* Copies the specified selection. */
	void copy();
	/* Cuts the current selection. */
	void cut();
	/* Deletes the current selection. */
	void remove();
	/* Selects the entire image. */
	void selectAll();
	/* Pastes the image in the clipboard. */
	void paste();

	#pragma endregion
	//=========== EDITING ============
	#pragma region Editing

	/* Finishes drawing with the tool. */
	void finalize() final;
	/* Finishes drawing a copied selection. */
	void finalizeCopy();
	/* Finishes drawing a deleted selection. */
	void finalizeDelete();
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
	void onTick(TickEventArgs e) final;
	void onPaint(PaintEventArgs e) final;

	#pragma endregion
};

DECLARE_SMART_PTRS(SelectTool);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
