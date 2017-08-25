/*=================================================================|
|* File:				SelectTool.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "SelectTool.h"
#include <AsciiArtist/Editing/UndoActions/DrawUndoAction.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/IO/Clipboard.h>
#include <PowerConsole/PowerConsoleSettings.h>
#include <PowerConsole/Controls/Window.h>
#include <AsciiArtist/Settings.h>

using namespace AsciiArtist;
using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::Tools;
using namespace AsciiArtist::Editing::UndoActions;
using namespace PowerConsole;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

SelectTool::SelectTool(CanvasSPtr canvas)
 :	Tool(canvas, "Select"),
	selection(nullptr),
	startPoint(Point2I::ZERO),
	selectionPoint(Point2I::ZERO),
	gripPoint(Point2I::ZERO),
	dragging(false),
	copied(false),
	boundsTimer(0) {

	canvas->addOption("Overwrte", true);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

fixed_array<std::string> SelectTool::getUsedOptions() const {
	return fixed_array<std::string>{"Overwrte"};
}
Point2I SelectTool::getSelectionSize() const {
	auto canvas = this->canvas.lock();
	if (hasSelection())
		return selection->size();
	else
		return Rectangle2I(startPoint, canvas->getMousePosition() - startPoint).pixelBounds().size;
}
bool SelectTool::hasSelection() const {
	return selection != nullptr;
}
Rectangle2I SelectTool::getSelectionBounds() const {
	return Rectangle2I(selectionPoint, selection->size());
}

#pragma endregion
//=========== ACTIONS ============
#pragma region Actions

void SelectTool::copy() {
	auto canvas = this->canvas.lock();
	if (hasSelection()) {
		if (selection->saveClipboard()) {
			canvas->sendMessage("Selection copied");
		}
		else {
			canvas->sendMessage("Error saving selection to clipboard", MESSAGE_COLOR_ERROR);
		}
	}
	else {
		canvas->sendMessage("Nothing selected");
	}
}
void SelectTool::cut() {
	auto canvas = this->canvas.lock();
	if (hasSelection()) {
		if (selection->saveClipboard()) {
			finalizeDelete();
			canvas->sendMessage("Selection cut");
		}
		else {
			canvas->sendMessage("Error saving selection to clipboard", MESSAGE_COLOR_ERROR);
		}
	}
	else {
		canvas->sendMessage("Nothing selected");
	}
}
void SelectTool::remove() {
	auto canvas = this->canvas.lock();
	if (hasSelection()) {
		finalizeDelete();
		canvas->sendMessage("Selection deleted");
	}
	else {
		canvas->sendMessage("Nothing selected");
	}
}
void SelectTool::selectAll() {
	auto canvas = this->canvas.lock();
	selection = std::make_shared<AsciiImage>(ImageFormats::Attributes, canvas->getCurrentFrame()->getBackground(), *canvas->getCurrentFrame());
	selectionPoint = Point2I::ZERO;
	drawing = true;
	copied = false;
	dragging = false;
	boundsTimer = 0;
	invalidatedRegion = Rectangle2I(Point2I::ZERO, canvas->getCurrentFrame()->size());

	drawChanges();
}
void SelectTool::paste() {
	auto canvas = this->canvas.lock();
	selection = std::make_shared<AsciiImage>();
	if (selection->loadClipboard()) {
		// Paste the selection in the top right corner of the visible canvas
		selectionPoint = canvas->getLocalVisibleArea().point;
		drawing = true;
		copied = true;
		dragging = false;
		boundsTimer = 0;
		invalidatedRegion = Rectangle2I(Point2I::ZERO, canvas->getCurrentFrame()->size());

		drawChanges();
		canvas->sendMessage("Clipboard pasted");
	}
	else {
		selection = nullptr;
		canvas->sendMessage("Error loading clipboard", MESSAGE_COLOR_ERROR);
	}
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void SelectTool::finalize() {
	auto canvas = this->canvas.lock();
	if (drawing) {
		drawing = false;
		if (hasSelection()) {
			auto undoAction = std::make_shared<DrawUndoAction>("Selection", canvas->getFrameIndex());
			if (Settings::getMaxUndos() > 0)
				undoAction->setOriginalImage(*canvas->getCurrentFrame());

			auto g = canvas->getCurrentFrameGraphics();
			g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
			g->drawImage(Point2I::ZERO, canvas->getCurrentFrame()->size(), *canvas->getDrawingBuffer());

			if (Settings::getMaxUndos() > 0)
				undoAction->setNewImage(*canvas->getCurrentFrame());
			canvas->addUndo(undoAction);

			selection = nullptr;
			copied = false;
			dragging = false;
		}
	}
}
void SelectTool::finalizeCopy() {
	auto canvas = this->canvas.lock();
	if (drawing) {
		if (hasSelection()) {
			auto undoAction = std::make_shared<DrawUndoAction>("Selection", canvas->getFrameIndex());
			if (Settings::getMaxUndos() > 0)
				undoAction->setOriginalImage(*canvas->getCurrentFrame());

			auto g = canvas->getCurrentFrameGraphics();
			g->drawImage(Point2I::ZERO, canvas->getCurrentFrame()->size(), *canvas->getDrawingBuffer());

			if (Settings::getMaxUndos() > 0)
				undoAction->setNewImage(*canvas->getCurrentFrame());
			canvas->addUndo(undoAction);

			copied = true;
		}
	}
}
void SelectTool::finalizeDelete() {
	auto canvas = this->canvas.lock();
	if (drawing) {
		drawing = false;
		if (hasSelection()) {
			if (!copied) {
				auto undoAction = std::make_shared<DrawUndoAction>("Selection", canvas->getFrameIndex());
				if (Settings::getMaxUndos() > 0)
					undoAction->setOriginalImage(*canvas->getCurrentFrame());

				auto g = canvas->getCurrentFrameGraphics();
				g->setModAttributes(canvas->getModAttributes());
				g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
				g->drawRectangle(Rectangle2I(startPoint, selection->size()), canvas->getCurrentFrame()->getBackground(), true);

				if (Settings::getMaxUndos() > 0)
					undoAction->setNewImage(*canvas->getCurrentFrame());
				canvas->addUndo(undoAction);
			}
			selection = nullptr;
		}
	}
}
void SelectTool::cancel() {
	if (drawing) {
		drawing = false;
		if (hasSelection()) {
			selection = nullptr;
			copied = false;
			dragging = false;
		}
	}
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void SelectTool::drawChanges() {
	auto canvas = this->canvas.lock();
	auto g = canvas->getDrawingBufferGraphics();
	g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
	g->drawImage(invalidatedRegion.point, invalidatedRegion, *canvas->getCurrentFrame());

	// Test if it's cheaper to overwrite changes to two regions or one next time.
	invalidatedRegion = Rectangle2I(selectionPoint, selection->size());

	g->setModAttributes(canvas->getModAttributes());
	if (!copied)
		g->drawRectangle(Rectangle2I(startPoint, selection->size()), canvas->getCurrentFrame()->getBackground(), true);
	g->setModOptions(canvas->getModOptions());
	g->drawImage(selectionPoint, *selection);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void SelectTool::onMouseButton(MouseButtonEventArgs e) {
	auto canvas = this->canvas.lock();
	shift = getFlag(e.modifiers, Modifiers::Shift);
	if (!drawing) {
		if (e.isButtonPressed(MouseButtons::Left)) {
			drawing = true;
			startPoint = e.newPosition;
		}
	}
	else if (!hasSelection()) {
		Point2I offset = GMath::sign(e.newPosition - startPoint);
		if (offset.x == 0) offset.x = 1;
		if (offset.y == 0) offset.y = 1;

		Point2I fontSize = canvas->getWindow()->console().getFontSize();
		Point2I distance = (e.newPosition - startPoint);
		if (shift) {
			Point2I abs = GMath::abs(distance * fontSize);
			if (abs.x >= abs.y)
				distance.x = (int)std::round((float)abs.y / (float)fontSize.x) * GMath::sign(distance.x);
			else
				distance.y = (int)std::round((float)abs.x / (float)fontSize.y) * GMath::sign(distance.y);
		}

		//Rectangle2I rectangle = Rectangle2I(startPoint, e.newPosition - startPoint + offset).pixelBounds();
		Rectangle2I rectangle = Rectangle2I(startPoint, distance).pixelBounds();
		rectangle = rectangle.intersection(Rectangle2I(Point2I::ZERO, canvas->getCurrentFrame()->size()));
		if (e.isButtonPressed(MouseButtons::Right)) {
			cancel();
		}
		else if (e.isButtonReleased(MouseButtons::Left)) {
			if (rectangle.size != Point2I::ONE) {
				startPoint = rectangle.point;
				selectionPoint = rectangle.point;
				selection = std::make_shared<AsciiImage>(rectangle, ImageFormats::Attributes, canvas->getCurrentFrame()->getBackground(), *canvas->getCurrentFrame());
				boundsTimer = 0;
				copied = false;
				dragging = false;
				invalidatedRegion = Rectangle2I(Point2I::ZERO, canvas->getCurrentFrame()->size());

				drawChanges();
			}
			else {
				// No selections of size 1x1 or smaller
				cancel();
			}
		}
	}
	else if (!dragging) {
		if (getSelectionBounds().containsPoint(e.newPosition)) {
			if (e.isButtonPressed(MouseButtons::Left)) {
				if (getFlag(e.modifiers, Modifiers::Ctrl)) {
					finalizeCopy();
				}
				gripPoint = e.newPosition - selectionPoint;
				dragging = true;
			}
		}
		else if (e.isButtonPressed(MouseButtons::Left)) {
			finalize();
		}
		if (e.isButtonPressed(MouseButtons::Right)) {
			finalize();
		}
	}
	else {
		if (e.isButtonReleased(MouseButtons::Left)) {
			dragging = false;
		}
	}
}
void SelectTool::onMouseMoved(MouseEventArgs e) {
	auto canvas = this->canvas.lock();
	if (dragging) {
		selectionPoint = e.newPosition - gripPoint;

		drawChanges();
	}
}
void SelectTool::onKey(KeyboardEventArgs e) {
	shift = getFlag(e.modifiers, Modifiers::Shift);
}
void SelectTool::onTick(TickEventArgs e) {
	auto canvas = this->canvas.lock();
	if (hasSelection()) {
		boundsTimer += e.timeSinceLastTick;
		boundsTimer %= BOUNDS_FLASH_TIME * 2;
	}
}
void SelectTool::onPaint(PaintEventArgs e) {
	auto canvas = this->canvas.lock();
	auto g = e.graphics;
	if (drawing) {
		if (!hasSelection()) {
			g->drawImage(canvas->getLocalVisibleArea().point, canvas->getLocalVisibleArea(), *canvas->getCurrentFrame());

			Point2I fontSize = canvas->getWindow()->console().getFontSize();
			Point2I distance = (canvas->getMousePosition() - startPoint);
			if (shift) {
				Point2I abs = GMath::abs(distance * fontSize);
				if (abs.x >= abs.y) 
					distance.x = (int)std::round((float)abs.y / (float)fontSize.x) * GMath::sign(distance.x);
				else
					distance.y = (int)std::round((float)abs.x / (float)fontSize.y) * GMath::sign(distance.y);
			}

			Rectangle2I rectangle = Rectangle2I(startPoint, distance).pixelBounds();
			g->setModAttributes(PixelAttributes::Invert);
			g->drawRectangle(rectangle, Pixel(' ', 0x00, PixelAttributes::Invert));
			g->resetOptions();
		}
		else {
			Rectangle2I visibleArea = canvas->getLocalVisibleArea();
			visibleArea.size = GMath::min(visibleArea.size, canvas->getCurrentFrame()->size());
			g->drawImage(visibleArea.point, visibleArea, *canvas->getDrawingBuffer());
			bool boundsState = (boundsTimer < BOUNDS_FLASH_TIME);
			int xoffset1 = boundsState ? 0 : 1;
			int yoffset1 = (xoffset1 + selection->size().x) % 2 == 0 ? 0 : 1;
			int xoffset2 = (yoffset1 + selection->size().y) % 2 == 0 ? 0 : 1;
			int yoffset2 = (xoffset2 + selection->size().x) % 2 == 0 ? 0 : 1;

			for (int x = xoffset1; x < selection->size().x; x += 2) {
				g->setSpecial(selectionPoint + Point2I(x, 0), PixelAttributes::Invert);
			}
			for (int y = yoffset1 + 1; y + 1 < selection->size().y; y += 2) {
				g->setSpecial(selectionPoint + Point2I(selection->size().x - 1, y), PixelAttributes::Invert);
			}
			for (int x = xoffset2; x < selection->size().x && selection->size().y > 1; x += 2) {
				g->setSpecial(selectionPoint + selection->size() - Point2I(1 + x, 1), PixelAttributes::Invert);
			}
			for (int y = yoffset2 + 1; y + 1 < selection->size().y && selection->size().x > 1; y += 2) {
				g->setSpecial(selectionPoint + Point2I(0, selection->size().y - 1 - y), PixelAttributes::Invert);
			}
			if (PowerConsoleSettings::isDebugging()) {
				g->drawRectangle(Rectangle2I(startPoint, selection->size()), Pixel(' ', toColor(CYAN, CYAN)));
				g->drawRectangle(invalidatedRegion, Pixel(' ', toColor(GREEN, GREEN)));
			}
		}
	}
}

#pragma endregion
//=================================================================|
