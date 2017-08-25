/*=================================================================|
|* File:				TextBox.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TextBox.h"
#include <PowerConsole/Controls/Window.h>
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/IO/Clipboard.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

TextBox::TextBox() 
 :	// Settings
	text(""),
	textAlignment(HorizontalAlignments::Left),
	maxLength(MINMAX_NONE),
	selectAllOnFocus(false),
	textColor(toColor(BLACK, GRAY)),
	selectionColor(toColor(BLUE, WHITE)),
	
	// Editing
	caretIndex(0),
	selectionStart(0),
	selectionLength(0),
	draggingIndex(-1),

	// Visuals
	scrollIndex(0),
	caretTimer(0),
	
	// Events
	_eventTextChanged(EventDirections::FirstInFirstCalled),
	_eventTextInput(EventDirections::FirstInLastCalled) {

	eventTick().add(bindEvent(TextBox::onTick));
	eventPaint().add(bindEvent(TextBox::onPaint));
	eventLayoutChanged().add(bindEvent(TextBox::onLayoutChanged));
	eventMouseMovedGlobal().add(bindEvent(TextBox::onMouseMovedGlobal));
	eventMouseButton().add(bindEvent(TextBox::onMouseButton));
	eventMouseButtonGlobal().add(bindEvent(TextBox::onMouseButtonGlobal));
	eventDoubleClick().add(bindEvent(TextBox::onDoubleClick));
	eventKeyTyped().add(bindEvent(TextBox::onKeyTyped));
	eventFocusGained().add(bindEvent(TextBox::onFocusGained));
	eventFocusLost().add(bindEvent(TextBox::onFocusLost));
	eventTextInput().add(bindEvent(TextBox::onTextInput));

	commands().add(Command("Copy", Hotkey(Keys::C, Modifiers::Ctrl), true,
		bindEvent(TextBox::onCopyCommand)
	));
	commands().add(Command("Cut", Hotkey(Keys::X, Modifiers::Ctrl), true,
		bindEvent(TextBox::onCutCommand)
	));
	commands().add(Command("Paste", Hotkey(Keys::V, Modifiers::Ctrl), true,
		bindEvent(TextBox::onPasteCommand)
	));
	commands().add(Command("Select All", Hotkey(Keys::A, Modifiers::Ctrl), true,
		bindEvent(TextBox::onSelectAllCommand)
	));

	setSizeSafe(Point2I(10, 1));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Settings
std::string TextBox::getText() const {
	return text;
}
void TextBox::setText(const std::string& text) {
	this->text = text;
}
int TextBox::getLength() const {
	return (int)text.length();
}
HorizontalAlignments TextBox::getTextAlignment() const {
	return textAlignment;
}
void TextBox::setTextAlignment(HorizontalAlignments textAlignment) {
	this->textAlignment = textAlignment;
}
int TextBox::getMaxLength() const {
	return maxLength;
}
void TextBox::setMaxLength(int maxLength) {
	this->maxLength = GMath::max(0, maxLength);
}
bool TextBox::getSelectAllOnFocus() const {
	return selectAllOnFocus;
}
void TextBox::setSelectAllOnFocus(bool selectAllOnFocus) {
	this->selectAllOnFocus = selectAllOnFocus;
}
unsigned char TextBox::getTextColor() const {
	return textColor;
}
void TextBox::setTextColor(unsigned char color) {
	this->textColor = color;
}
unsigned char TextBox::getSelectionColor() const {
	return selectionColor;
}
void TextBox::setSelectionColor(unsigned char color) {
	this->selectionColor = color;
}

// Editing
int TextBox::getCaretIndex() const {
	return caretIndex;
}
void TextBox::setCaretIndex(int index) {
	this->caretIndex = GMath::clamp(index, 0, (int)text.length());
	this->selectionStart = caretIndex;
	this->selectionLength = 0;
	updateScrollIndex();
}
bool TextBox::hasSelection() const {
	return selectionLength > 0;
}
std::string TextBox::getSelection() const {
	return text.substr(selectionStart, selectionLength);
}
int TextBox::getSelectionStart() const {
	return selectionStart;
}
int TextBox::getSelectionLength() const {
	return selectionLength;
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void TextBox::select(int start, int length) {
	if (!hasFocus())
		focus();
	selectionStart = GMath::clamp(start, 0, (int)text.length());
	selectionLength = GMath::clamp(length, 0, (int)text.length() - selectionStart);
	caretIndex = selectionStart + selectionLength;
	updateScrollIndex();
}
void TextBox::selectAll() {
	if (!hasFocus())
		focus();
	selectionStart = 0;
	selectionLength = (int)text.length();
	caretIndex = selectionStart + selectionLength;
	updateScrollIndex();
}
void TextBox::appendText(const std::string& input) {
	replaceText((int)text.length(), 0, input);
}
void TextBox::insertText(const std::string& input) {
	replaceText(selectionStart, selectionLength, input);
}
void TextBox::insertText(int index, const std::string& input) {
	replaceText(index, 0, input);
}
void TextBox::replaceText(int index, int length, const std::string& input) {
	if (length > 0) {
		if ((int)input.length() + (int)text.length() - length > maxLength && maxLength != MINMAX_NONE) {
			int inputSubStrLength = maxLength - (int)text.length();
			text.replace(index, length, input.substr(0, inputSubStrLength));
			text = text.substr(0, maxLength);
			selectionStart = index + inputSubStrLength;
			if (getWindow() != nullptr)
				getWindow()->console().playSystemSound(SystemSounds::Alert);
		}
		else {
			text.replace(index, length, input);
			selectionStart = index + (int)input.length();
		}
		selectionLength = 0;
		caretIndex = selectionStart;
		draggingIndex = -1;
		caretTimer = 0;
		updateScrollIndex();
	}
	else if ((int)text.length() < maxLength || maxLength == MINMAX_NONE) {
		if ((int)input.length() + (int)text.length() > maxLength && maxLength != MINMAX_NONE) {
			int inputSubStrLength = maxLength - (int)text.length();
			text.insert(index, input.substr(0, inputSubStrLength));
			text = text.substr(0, maxLength);
			caretIndex = index + inputSubStrLength;
			if (getWindow() != nullptr)
				getWindow()->console().playSystemSound(SystemSounds::Alert);
		}
		else {
			text.insert(index, input);
			caretIndex = index + (int)input.length();
		}
		selectionStart = caretIndex;
		selectionLength = 0;
		draggingIndex = -1;
		caretTimer = 0;
		updateScrollIndex();
	}
	caretTimer = 0;
}
void TextBox::eraseText() {
	eraseText(selectionStart, selectionLength);
}
void TextBox::eraseText(int index, int length) {
	if (length > 0) {
		text.erase(index, length);
		selectionStart = index;
		selectionLength = 0;
		caretIndex = selectionStart;
		updateScrollIndex();
	}
	caretTimer = 0;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I TextBox::autoSizeBehavior() const {
	return Point2I(0, 1);
}

int TextBox::getIndexFromPosition(int x) const {
	return GMath::clamp(x - getTextAlignmentScrollPosition(), 0, (int)text.length());
}
int TextBox::getPositionFromIndex(int index) const {
	return getTextAlignmentScrollPosition() + index;
}
int TextBox::getTextAlignmentScrollPosition() const {
	if ((int)text.length() >= getActualSize().x) {
		return -scrollIndex;
	}
	else {
		switch (textAlignment) {
		case HorizontalAlignments::Center:
			return (getActualSize().x - (int)text.length()) / 2;
			break;
		case HorizontalAlignments::Right:
			return (getActualSize().x - (int)text.length()) - 1;
			break;
		default:
			return 0;
		}
	}
}
bool TextBox::canScrollLeft() const {
	return (scrollIndex > 0);
}
bool TextBox::canScrollRight() const {
	return (scrollIndex < (int)text.length() + 1 - getActualSize().x);
}
bool TextBox::canScroll() const {
	return (getActualSize().x < (int)text.length() + 1);
}
void TextBox::updateScrollIndex() {
	// The x position of the caret, not the index
	int caretPosition = getPositionFromIndex(caretIndex);
	if (canScroll()) {
		if (caretPosition < 0) {
			scrollIndex = caretIndex;
		}
		else if (caretPosition >= std::max(0, getActualSize().x)) {
			scrollIndex = caretIndex - std::max(0, getActualSize().x) + 1;
		}
		scrollIndex = GMath::min(scrollIndex, (int)text.length() + 1 - std::max(0, getActualSize().x));
	}
	else {
		scrollIndex = 0;
	}
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<TextChangedEventArgs>& TextBox::eventTextChanged() {
	return _eventTextChanged;
}
Event<TextInputEventArgs>& TextBox::eventTextInput() {
	return _eventTextInput;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void TextBox::onTick(ControlSPtr sender, TickEventArgs& e) {
	caretTimer += e.timeSinceLastTick;
	caretTimer %= CARET_FLASH_TIME * 2;
}
void TextBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	unsigned char color;
	unsigned char hoverColor = toColor(BLACK, WHITE);
	unsigned char disabledColor = toColor(BLACK, GRAY);

	// Draw the text
	color = (isEnabled() ? (!hasFocus() && isMouseInside() ? hoverColor : textColor) : disabledColor);
	if (canScroll()) {
		g->drawString(Point2I::ZERO, text.substr(scrollIndex, std::max(0, getActualSize().x)), Pixel(' ', color));
	}
	else {
		if (textAlignment == HorizontalAlignments::Right)
			g->drawString(Point2I(getPositionFromIndex(0) + 1, 0), text, Pixel(' ', color));
		else
			g->drawString(Point2I(getPositionFromIndex(0), 0), text, Pixel(' ', color));
	}
	
	if (hasFocus()) {
		// Draw the selection if there is any
		if (selectionLength > 0) {
			if (textAlignment == HorizontalAlignments::Right)
				g->setColor(Rectangle2I(getPositionFromIndex(selectionStart) + 1, 0, selectionLength, 1), selectionColor);
			else
				g->setColor(Rectangle2I(getPositionFromIndex(selectionStart), 0, selectionLength, 1), selectionColor);
		}

		// Draw the caret if it's state is 'show'
		if (caretTimer < CARET_FLASH_TIME) {
			g->setSpecial(Point2I(getPositionFromIndex(caretIndex), 0), PixelAttributes::Invert);
		}
	}
}
void TextBox::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		updateScrollIndex();
	}
}
void TextBox::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	// Scroll if mouse is outside bounds and dragging
	if (draggingIndex != -1) {
		if (e.newPosition.x < 0 && canScrollLeft())
			scrollIndex--;
		else if (e.newPosition.x >= getActualSize().x && canScrollRight())
			scrollIndex++;
	}

	int hoverIndex = getIndexFromPosition(e.newPosition.x);
	if (draggingIndex != -1) {
		if (hoverIndex >= draggingIndex) {
			selectionStart = draggingIndex;
			selectionLength = hoverIndex - draggingIndex;
			caretTimer = 0;
		}
		else if (hoverIndex < draggingIndex) {
			selectionStart = hoverIndex;
			selectionLength = draggingIndex - hoverIndex;
			caretTimer = 0;
		}
		caretIndex = hoverIndex;
	}
}
void TextBox::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.buttonChanged == MouseButtons::Left) {
		if (hasFocus()) {
			if (e.buttonState == PressedStates::Pressed) {
				if (e.newPosition.y == 0) {
					draggingIndex = getIndexFromPosition(e.newPosition.x);
					caretIndex = draggingIndex;
					selectionStart = caretIndex;
					selectionLength = 0;
					caretTimer = 0;
				}
			}
			else {
				draggingIndex = -1;
				caretTimer = 0;
			}
		}
		else if (e.buttonState == PressedStates::Pressed) {
			focus();
		}
	}
}
void TextBox::onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (!isMouseInside() && e.buttonState == PressedStates::Pressed) {
		unfocus();
	}
}
void TextBox::onDoubleClick(ControlSPtr sender, MouseEventArgs& e) {
	selectAll();
}
void TextBox::onKeyTyped(ControlSPtr sender, KeyTypedEventArgs& e) {
	std::string previousText = text;
	if (e.inputKey == Keys::Backspace) {
		if (selectionLength > 0) {
			text.erase(selectionStart, selectionLength);
			selectionLength = 0;
			caretIndex = selectionStart;
			draggingIndex = -1;
			caretTimer = 0;
			updateScrollIndex();
		}
		else if (caretIndex > 0) {
			text.erase(caretIndex - 1, 1);
			caretIndex--;
			selectionStart = caretIndex;
			draggingIndex = -1;
			caretTimer = 0;
			updateScrollIndex();
		}
		_eventTextChanged(shared_from_this(), TextChangedEventArgs(text, previousText));
	}
	else if (e.inputKey == Keys::Delete) {
		if (selectionLength > 0) {
			text.erase(selectionStart, selectionLength);
			selectionLength = 0;
			caretIndex = selectionStart;
			draggingIndex = -1;
			caretTimer = 0;
			updateScrollIndex();
		}
		else if (caretIndex < (int)text.length()) {
			text.erase(caretIndex, 1);
			draggingIndex = -1;
			caretTimer = 0;
			updateScrollIndex();
		}
		_eventTextChanged(shared_from_this(), TextChangedEventArgs(text, previousText));
	}
	else if (e.inputKey == Keys::Escape || e.inputKey == Keys::Enter) {
		unfocus();
	}
	else if (e.inputKey == Keys::Left) {
		if (caretIndex > 0) {
			if (getFlag(e.modifiers, Modifiers::Shift)) {
				if (caretIndex == selectionStart) {
					selectionStart--;
					selectionLength++;
				}
				else {
					selectionLength--;
				}
				caretIndex--;
			}
			else {
				caretIndex--;
				selectionStart = caretIndex;
				selectionLength = 0;
			}
			draggingIndex = -1;
			caretTimer = 0;
			updateScrollIndex();
		}
	}
	else if (e.inputKey == Keys::Right) {
		if (caretIndex < (int)text.length()) {
			if (getFlag(e.modifiers, Modifiers::Shift)) {
				if (caretIndex == selectionStart + selectionLength) {
					selectionLength++;
				}
				else {
					selectionStart++;
					selectionLength--;
				}
				caretIndex++;
			}
			else {
				caretIndex++;
				selectionStart = caretIndex;
				selectionLength = 0;
			}
			caretTimer = 0;
			updateScrollIndex();
		}
	}
	else if (e.inputKey == Keys::Home) {
		if (getFlag(e.modifiers, Modifiers::Shift)) {
			if (caretIndex == selectionStart)
				selectionLength += selectionStart;
			else
				selectionLength = selectionStart;
			selectionStart = 0;
			caretIndex = selectionStart;
		}
		else {
			caretIndex = 0;
			selectionStart = caretIndex;
			selectionLength = 0;
		}
		draggingIndex = -1;
		caretTimer = 0;
		updateScrollIndex();
	}
	else if (e.inputKey == Keys::End) {
		if (getFlag(e.modifiers, Modifiers::Shift)) {
			if (caretIndex != selectionStart + selectionLength)
				selectionStart += selectionLength;
			selectionLength = (int)text.length() - selectionStart;
			caretIndex = (int)text.length();
		}
		else {
			caretIndex = (int)text.length();
			selectionStart = caretIndex;
			selectionLength = 0;
		}
		draggingIndex = -1;
		caretTimer = 0;
		updateScrollIndex();
	}
	else if (e.inputKey != Keys::Tab && e.inputChar >= 0x20) {
		if (!getAnyFlag(e.modifiers, Modifiers::Alt | Modifiers::Ctrl | Modifiers::Win, Modifiers::None)) {
			_eventTextInput(shared_from_this(), TextInputEventArgs(text, std::string(1, e.inputChar), selectionStart, selectionLength));
			if (text != previousText) {
				_eventTextChanged(shared_from_this(), TextChangedEventArgs(text, previousText));
			}
		}
	}
}
void TextBox::onTextInput(ControlSPtr sender, TextInputEventArgs& e) {
	replaceText(e.index, e.length, e.input);
}
void TextBox::onFocusGained(ControlSPtr sender, EventArgs& e) {
	caretIndex = (int)text.length();
	selectionStart = caretIndex;
	selectionLength = 0;
	draggingIndex = -1;

	updateScrollIndex();
	caretTimer = 0;

	if (selectAllOnFocus) {
		caretIndex = (int)text.length();
		selectionStart = 0;
		selectionLength = caretIndex;
	}
}
void TextBox::onFocusLost(ControlSPtr sender, EventArgs& e) {
	caretIndex = 0;
	selectionStart = 0;
	selectionLength = 0;
	draggingIndex = -1;

	scrollIndex = 0;
	caretTimer = 0;
}

#pragma endregion
//======= COMMAND HANDLERS =======
#pragma region Command Handlers

void TextBox::onCopyCommand(ControlSPtr sender, EventArgs& e) {
	if (selectionLength > 0) {
		Clipboard::setText(text.substr(selectionStart, selectionLength));
	}
}
void TextBox::onCutCommand(ControlSPtr sender, EventArgs& e) {
	if (selectionLength > 0) {
		std::string previousText = text;
		Clipboard::setText(text.substr(selectionStart, selectionLength));
		eraseText();
		if (text != previousText) {
			_eventTextChanged(shared_from_this(), TextChangedEventArgs(text, previousText));
		}
	}
}
void TextBox::onPasteCommand(ControlSPtr sender, EventArgs& e) {
	if (Clipboard::containsText()) {
		std::string previousText = text;
		std::string clipboard = Clipboard::getText();
		_eventTextInput(shared_from_this(), TextInputEventArgs(text, clipboard, selectionStart, selectionLength));
		if (text != previousText) {
			_eventTextChanged(shared_from_this(), TextChangedEventArgs(text, previousText));
		}
	}
}
void TextBox::onSelectAllCommand(ControlSPtr sender, EventArgs& e) {
	selectAll();
}

#pragma endregion
//=================================================================|
