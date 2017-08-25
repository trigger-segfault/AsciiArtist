/*=================================================================|
|* File:				TextBox.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <PowerConsole/Controls/Control.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class TextBox : public Control {

	//========== CONSTANTS ===========
	#pragma region Constants

	/* The time in milliseconds it takes for the cursor to change states. */
	static const int CARET_FLASH_TIME = 560;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	// Settings
	/* The text used by the button. */
	std::string text;
	/* The alignment of the text. */
	PcGeo::HorizontalAlignments textAlignment;
	/* The max length of the textbox. */
	int maxLength;
	/* True if the text box automatically selects all on focus. */
	bool selectAllOnFocus;
	/* The color of the text. */
	unsigned char textColor;
	/* The color of selections. */
	unsigned char selectionColor;

	// Editing
	/* The index of the caret in the textbox. */
	int caretIndex;
	/* The starting point of the selection. */
	int selectionStart;
	/* The length of the selection. This value should always be greater than zero. */
	int selectionLength;
	/* The index that selection dragging was started at. -1 for no dragging. */
	int draggingIndex;

	// Visuals
	/* The starting view index of the textbox. */
	int scrollIndex;
	/* The timer used for caret flashing. */
	int caretTimer;

	// Events
	/* Called when the text has been changed. */
	Event<TextChangedEventArgs> _eventTextChanged;
	/* Called before text is input into the text box. */
	Event<TextInputEventArgs> _eventTextInput;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the textbox. */
	TextBox();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Settings
	/* Gets the text of the textbox. */
	std::string getText() const;
	/* Sets the text of the textbox. */
	void setText(const std::string& text);
	/* Gets the length of the text. */
	int getLength() const;
	/* Gets the alignment of the text. */
	PcGeo::HorizontalAlignments getTextAlignment() const;
	/* Gets the alignment of the text. */
	void setTextAlignment(PcGeo::HorizontalAlignments textAlignment);
	/* Gets the max length of the textbox. Use MINMAX_NONE for no max. */
	int getMaxLength() const;
	/* Sets the max length of the textbox. Use MINMAX_NONE for no max. */
	void setMaxLength(int maxLength);
	/* Gets if the text box automatically selects all on focus. */
	bool getSelectAllOnFocus() const;
	/* Gets if the text box automatically selects all on focus. */
	void setSelectAllOnFocus(bool selectAllOnFocus);
	/* Gets the color of the text. */
	unsigned char getTextColor() const;
	/* Sets the color of the text. */
	void setTextColor(unsigned char color);
	/* Gets the color of selections. */
	unsigned char getSelectionColor() const;
	/* Sets the color of selections. */
	void setSelectionColor(unsigned char color);

	// Editing
	/* Gets the index of the caret in the textbox. */
	int getCaretIndex() const;
	/* Sets the index of the caret in the textbox. */
	void setCaretIndex(int index);
	/* Returns true if the textbox has a selection. */
	bool hasSelection() const;
	/* Gets the selected text. */
	std::string getSelection() const;
	/* Gets the starting point of the selection. */
	int getSelectionStart() const;
	/* Gets the length of the selection. This value should always be greater than zero. */
	int getSelectionLength() const;

	#pragma endregion
	//=========== EDITING ============
	#pragma region Editing

	/* Sets the selection range. */
	void select(int start, int length);
	/* Selects everything in the textbox. */
	void selectAll();
	/* Appends text to the end of text box. */
	void appendText(const std::string& input);
	/* Inserts text into the text box. */
	void insertText(const std::string& input);
	/* Inserts text into the text box. */
	void insertText(int index, const std::string& input);
	/* Replaces text in the text box. */
	void replaceText(int index, int length, const std::string& input);
	/* Erases the current selection. */
	void eraseText();
	/* Erases text in the text box. */
	void eraseText(int index, int length);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const override;

	/* Gets the index in the text from the x position. */
	int getIndexFromPosition(int x) const;
	/* Gets the x position from the index in the text. */
	int getPositionFromIndex(int index) const;
	/* Gets the position of the text based on its alignment and scroll position. */
	int getTextAlignmentScrollPosition() const;
	/* True if the textbox can be scrolled left. */
	bool canScrollLeft() const;
	/* True if the textbox can be scrolled right. */
	bool canScrollRight() const;
	/* True if the textbox can be scrolled left or right. */
	bool canScroll() const;
	/* Updates the scroll index based on the caret index. */
	void updateScrollIndex();

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when the text has been changed. */
	Event<TextChangedEventArgs>& eventTextChanged();
	/* Called before text is input into the text box. */
	Event<TextInputEventArgs>& eventTextInput();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);
	void onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e);
	void onDoubleClick(ControlSPtr sender, MouseEventArgs& e);
	void onKeyTyped(ControlSPtr sender, KeyTypedEventArgs& e);
	void onTextInput(ControlSPtr sender, TextInputEventArgs& e);
	void onFocusGained(ControlSPtr sender, EventArgs& e);
	void onFocusLost(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//======= COMMAND HANDLERS =======
	#pragma region Command Handlers

	void onCopyCommand(ControlSPtr sender, EventArgs& e);
	void onCutCommand(ControlSPtr sender, EventArgs& e);
	void onPasteCommand(ControlSPtr sender, EventArgs& e);
	void onSelectAllCommand(ControlSPtr sender, EventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(TextBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
