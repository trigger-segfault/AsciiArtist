/*=================================================================|
|* File:				TextTool.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TextTool.h"
#include <AsciiArtist/Editing/UndoActions/DrawUndoAction.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/IO/Clipboard.h>
#include <AsciiArtist/Settings.h>

using namespace AsciiArtist;
using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::Tools;
using namespace AsciiArtist::Editing::UndoActions;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

TextTool::TextTool(CanvasSPtr canvas)
	: Tool(canvas, "Text") {

	canvas->addOption("Overwrte", true);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

fixed_array<std::string> TextTool::getUsedOptions() const {
	return fixed_array<std::string>{"Overwrte"};
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void TextTool::finalize() {
	auto canvas = this->canvas.lock();
	if (drawing) {
		drawing = false;
		auto undoAction = std::make_shared<DrawUndoAction>("Text", canvas->getFrameIndex());
		if (Settings::getMaxUndos() > 0)
			undoAction->setOriginalImage(*canvas->getCurrentFrame());

		auto g = canvas->getCurrentFrameGraphics();
		g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
		g->drawImage(Point2I::ZERO, canvas->getCurrentFrame()->size(), *canvas->getDrawingBuffer());

		if (Settings::getMaxUndos() > 0)
			undoAction->setNewImage(*canvas->getCurrentFrame());
		canvas->addUndo(undoAction);
	}
}
void TextTool::cancel() {
	if (drawing) {
		drawing = false;
	}
}
bool TextTool::characterSelected(unsigned char character, bool transparent) {
	auto canvas = this->canvas.lock();
	if (!transparent) {
		if (selectionLength > 0) {
			text.replace(selectionStart, selectionLength, std::string(1, character));
			selectionStart++;
			selectionLength = 0;
			caretIndex = selectionStart;
			draggingIndex = -1;
			caretTimer = 0;
			drawChanges();
		}
		else if ((int)text.length() < canvas->getCurrentFrame()->size().x - startPoint.x) {
			text.insert(caretIndex, std::string(1, character));
			caretIndex++;
			selectionStart = caretIndex;
			selectionLength = 0;
			draggingIndex = -1;
			caretTimer = 0;
			drawChanges();
		}
	}
	return true;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

int TextTool::getIndexFromPosition(int x) const {
	return GMath::clamp(x - startPoint.x, 0, (int)text.length());
}
int TextTool::getPositionFromIndex(int index) const {
	return startPoint.x + index;
}
void TextTool::drawChanges() {
	auto canvas = this->canvas.lock();
	auto g = canvas->getDrawingBufferGraphics();
	g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
	g->drawImage(invalidatedRegion.point, invalidatedRegion, *canvas->getCurrentFrame());
	g->setModAttributes(canvas->getModAttributes());
	g->setModOptions(canvas->getModOptions());

	Rectangle2I rectangle = Rectangle2I(startPoint, Point2I((int)text.length(), 1));
	invalidatedRegion = Rectangle2I(startPoint, Point2I((int)text.length(), 1)).intersect(Rectangle2I(Point2I::ZERO, canvas->getCurrentFrame()->size()));
	g->drawString(startPoint, text, canvas->getDrawingPixel(secondary));
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void TextTool::onMouseButton(MouseButtonEventArgs e) {
	auto canvas = this->canvas.lock();
	if (!drawing) {
		if (e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right)) {
			secondary = e.isButtonPressed(MouseButtons::Right);
			drawing = true;
			startPoint = e.newPosition;
			caretTimer = 0;
			caretIndex = 0;
			selectionStart = 0;
			selectionLength = 0;
			draggingIndex = -1;
			text = "";
			invalidatedRegion = Rectangle2I(Point2I::ZERO, canvas->getCurrentFrame()->size());
			drawChanges();
		}
	}
	else if (draggingIndex != -1) {
		if (e.isButtonReleased(MouseButtons::Left)) {
			draggingIndex = -1;
			caretTimer = 0;
		}
	}
	else {
		auto bounds = Rectangle2I(startPoint, Point2I((int)text.length() + 1, 1));
		if (bounds.containsPoint(e.newPosition)) {
			if (e.isButtonPressed(MouseButtons::Left)) {
				draggingIndex = getIndexFromPosition(e.newPosition.x);
				caretIndex = draggingIndex;
				selectionStart = caretIndex;
				selectionLength = 0;
				caretTimer = 0;
			}
			else if (e.isButtonPressed(MouseButtons::Right)) {
				finalize();
			}
		}
		else if (e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right)) {
			finalize();
		}
	}
}
void TextTool::onMouseMoved(MouseEventArgs e) {
	auto canvas = this->canvas.lock();
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
void TextTool::onKey(KeyboardEventArgs e) {
	auto canvas = this->canvas.lock();

}
void TextTool::onKeyTyped(KeyTypedEventArgs e) {
	auto canvas = this->canvas.lock();
	if (e.inputKey == Keys::Backspace) {
		if (selectionLength > 0) {
			text.erase(selectionStart, selectionLength);
			selectionLength = 0;
			caretIndex = selectionStart;
			draggingIndex = -1;
			caretTimer = 0;
			drawChanges();
		}
		else if (caretIndex > 0) {
			text.erase(caretIndex - 1, 1);
			caretIndex--;
			selectionStart = caretIndex;
			draggingIndex = -1;
			caretTimer = 0;
			drawChanges();
		}
	}
	else if (e.inputKey == Keys::Escape || e.inputKey == Keys::Enter) {
		finalize();
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
	}
	else if (e.inputKey != Keys::Tab && e.inputChar >= 0x20) {
		if (!getAnyFlag(e.modifiers, Modifiers::Alt | Modifiers::Ctrl | Modifiers::Win, Modifiers::None)) {
			if (selectionLength > 0) {
				text.replace(selectionStart, selectionLength, std::string(1, e.inputChar));
				selectionStart++;
				selectionLength = 0;
				caretIndex = selectionStart;
				draggingIndex = -1;
				caretTimer = 0;
				drawChanges();
			}
			else if ((int)text.length() < canvas->getCurrentFrame()->size().x - startPoint.x) {
				text.insert(caretIndex, std::string(1, e.inputChar));
				caretIndex++;
				selectionStart = caretIndex;
				selectionLength = 0;
				draggingIndex = -1;
				caretTimer = 0;
				drawChanges();
			}
		}
	}
}
void TextTool::onTick(TickEventArgs e) {
	caretTimer += e.timeSinceLastTick;
	caretTimer %= CARET_FLASH_TIME * 2;
}
void TextTool::onPaint(PaintEventArgs e) {
	auto canvas = this->canvas.lock();
	auto g = e.graphics;
	Rectangle2I visibleArea = canvas->getLocalVisibleArea();
	visibleArea.size = GMath::min(visibleArea.size, canvas->getCurrentFrame()->size());
	g->drawImage(visibleArea.point, visibleArea, *canvas->getDrawingBuffer());
	unsigned char selectionColor = toColor(BLUE, WHITE);

	// Draw the selection if there is any
	if (selectionLength > 0) {
		g->setColor(Rectangle2I(getPositionFromIndex(selectionStart), startPoint.y, selectionLength, 1), selectionColor);
	}

	// Draw the caret if it's state is 'show'
	if (caretTimer < CARET_FLASH_TIME) {
		g->setSpecial(
			Point2I(GMath::min(canvas->getCurrentFrame()->size().x - 1, getPositionFromIndex(caretIndex)), startPoint.y),
			PixelAttributes::Invert
		);
	}

}

#pragma endregion
//=========== ACTIONS ============
#pragma region Actions

void TextTool::onCopyCommand() {
	if (selectionLength > 0) {
		Clipboard::setText(text.substr(selectionStart, selectionLength));
	}
}
void TextTool::onCutCommand() {
	std::string previousText = text;
	if (selectionLength > 0) {
		Clipboard::setText(text.substr(selectionStart, selectionLength));
		text.erase(selectionStart, selectionLength);
		selectionLength = 0;
		caretIndex = selectionStart;
		draggingIndex = -1;
		caretTimer = 0;
		drawChanges();
	}
}
void TextTool::onDeleteCommand() {
	std::string previousText = text;
	if (selectionLength > 0) {
		text.erase(selectionStart, selectionLength);
		selectionLength = 0;
		caretIndex = selectionStart;
		draggingIndex = -1;
		caretTimer = 0;
		drawChanges();
	}
	else if (caretIndex < (int)text.length()) {
		text.erase(caretIndex, 1);
		draggingIndex = -1;
		caretTimer = 0;
		drawChanges();
	}
}
void TextTool::onPasteCommand() {
	auto canvas = this->canvas.lock();
	if (Clipboard::containsText()) {
		std::string previousText = text;
		std::string clipboard = Clipboard::getText();
		int maxLength = canvas->getCurrentFrame()->size().x - startPoint.x;
		if (selectionLength > 0) {
			if ((int)clipboard.length() + (int)text.length() - selectionLength > maxLength) {
				text.replace(selectionStart, selectionLength, clipboard);
				text = text.substr(0, maxLength);
				selectionStart = std::min(selectionStart + 1 + (int)clipboard.length(), maxLength);
			}
			else {
				text.replace(selectionStart, selectionLength, clipboard);
				selectionStart += (int)clipboard.length();
			}
			selectionLength = 0;
			caretIndex = selectionStart;
			draggingIndex = -1;
			caretTimer = 0;
			drawChanges();
		}
		else if ((int)text.length() < maxLength) {
			if ((int)clipboard.length() + (int)text.length() > maxLength) {
				text.insert(caretIndex, clipboard);
				text = text.substr(0, maxLength);
				caretIndex = std::min(caretIndex + (int)clipboard.length(), maxLength);
			}
			else {
				text.insert(caretIndex, clipboard);
				caretIndex += (int)clipboard.length();
			}
			selectionStart = caretIndex;
			selectionLength = 0;
			draggingIndex = -1;
			caretTimer = 0;
			drawChanges();
		}
	}
}
void TextTool::onSelectAllCommand() {
	selectionStart = 0;
	selectionLength = (int)text.length();
	caretIndex = selectionStart + selectionLength;
}

#pragma endregion
//=================================================================|
