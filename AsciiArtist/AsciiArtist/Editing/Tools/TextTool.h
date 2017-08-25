/*=================================================================|
|* File:				TextTool.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TEXT_TOOL_H
#define TEXT_TOOL_H

#include <string>
#include <AsciiArtist/Editing/Tool.h>

using namespace AsciiArtist::Editing;

namespace AsciiArtist::Editing::Tools {
//=================================================================|
// CLASSES														   |
//=================================================================/

class TextTool : public Tool {
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The time in milliseconds it takes for the cursor to change states. */
	static const int CARET_FLASH_TIME = 560;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* True if the secondary pixel is used. */
	bool secondary;
	/* The start position of the text. */
	Point2I startPoint;

	// Editing
	/* The text used by the button. */
	std::string text;
	/* The index of the caret in the textbox. */
	int caretIndex;
	/* The starting point of the selection. */
	int selectionStart;
	/* The length of the selection. This value should always be greater than zero. */
	int selectionLength;
	/* The index that selection dragging was started at. -1 for no dragging. */
	int draggingIndex;

	// Visuals
	/* The timer used for caret flashing. */
	int caretTimer;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tool. */
	TextTool(CanvasSPtr canvas);

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
	/* Handles a selected character. Returns true if handled by the tool. */
	bool characterSelected(unsigned char character, bool transparent) final;

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers

	/* Gets the index in the text from the x position. */
	int getIndexFromPosition(int x) const;
	/* Gets the x position from the index in the text. */
	int getPositionFromIndex(int index) const;
	/* Draws changes made to the text. */
	void drawChanges();

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
	//=========== ACTIONS ============
	#pragma region Actions

	void onCopyCommand();
	void onCutCommand();
	void onDeleteCommand();
	void onPasteCommand();
	void onSelectAllCommand();

	#pragma endregion
};

DECLARE_SMART_PTRS(TextTool);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
