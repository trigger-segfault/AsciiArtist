/*=================================================================|
|* File:				CircleTool.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "CircleTool.h"
#include <AsciiArtist/Editing/UndoActions/DrawUndoAction.h>
#include <PowerConsole/Geometry/GMath.h>
#include <AsciiArtist/Settings.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Controls/Window.h>

using namespace AsciiArtist;
using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::Tools;
using namespace AsciiArtist::Editing::UndoActions;
using namespace PowerConsole::Input;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

CircleTool::CircleTool(CanvasSPtr canvas)
	: Tool(canvas, "Circle"),
	secondary(false),
	startPoint(Point2I::ZERO),
	shift(false) {

	canvas->addOption("Overwrte", false);
	canvas->addOption("Outline", true);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

fixed_array<std::string> CircleTool::getUsedOptions() const {
	return fixed_array<std::string>{"Overwrte", "Outline"};
}
Point2I CircleTool::getSelectionSize() const {
	auto canvas = this->canvas.lock();
	return Rectangle2I(startPoint, canvas->getMousePosition() - startPoint).pixelBounds().size;
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void CircleTool::finalize() {
	auto canvas = this->canvas.lock();
	if (drawing) {
		drawing = false;
		auto undoAction = std::make_shared<DrawUndoAction>("Pen", canvas->getFrameIndex());
		if (Settings::getMaxUndos() > 0)
			undoAction->setOriginalImage(*canvas->getCurrentFrame());

		auto g = canvas->getCurrentFrameGraphics();
		g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
		g->drawImage(Point2I::ZERO, *canvas->getDrawingBuffer());

		if (Settings::getMaxUndos() > 0)
			undoAction->setNewImage(*canvas->getCurrentFrame());
		canvas->addUndo(undoAction);
	}
}
void CircleTool::cancel() {
	if (drawing) {
		drawing = false;
	}
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void CircleTool::drawChanges() {
	auto canvas = this->canvas.lock();
	auto g = canvas->getDrawingBufferGraphics();
	g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
	g->drawImage(invalidatedRegion.point, invalidatedRegion, *canvas->getCurrentFrame());
	g->setModAttributes(canvas->getModAttributes());
	g->setModOptions(canvas->getModOptions());

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
	invalidatedRegion = rectangle.intersection(Rectangle2I(Point2I::ZERO, canvas->getCurrentFrame()->size()));
	g->drawEllipse(rectangle.point, rectangle.size, canvas->getDrawingPixel(secondary), !canvas->getOption("Outline"));
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void CircleTool::onMouseButton(MouseButtonEventArgs e) {
	auto canvas = this->canvas.lock();
	shift = getFlag(e.modifiers, Modifiers::Shift);
	if (e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right)) {
		if (!drawing) {
			drawing = true;
			secondary = e.buttonChanged == MouseButtons::Right;
			startPoint = e.newPosition;

			// Draw the first pixel
			auto g = canvas->getDrawingBufferGraphics();
			g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
			g->drawImage(Point2I::ZERO, *canvas->getCurrentFrame());
			g->setModAttributes(canvas->getModAttributes());
			g->setModOptions(canvas->getModOptions());
			g->setPixel(e.newPosition, canvas->getDrawingPixel(secondary));
			invalidatedRegion = Rectangle2I(e.newPosition, Point2I::ONE);
		}
		else if (secondary != (e.buttonChanged == MouseButtons::Right)) {
			// Holding down both left and right cancels the action
			cancel();
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left) || e.isButtonReleased(MouseButtons::Right)) {
		if (drawing && secondary == (e.buttonChanged == MouseButtons::Right)) {
			finalize();
		}
	}
}
void CircleTool::onMouseMoved(MouseEventArgs e) {
	drawChanges();
}
void CircleTool::onKey(KeyboardEventArgs e) {
	shift = getFlag(e.modifiers, Modifiers::Shift);
	drawChanges();
}
void CircleTool::onPaint(PaintEventArgs e) {
	auto canvas = this->canvas.lock();
	auto g = e.graphics;
	Rectangle2I visibleArea = canvas->getLocalVisibleArea();
	visibleArea.size = GMath::min(visibleArea.size, canvas->getCurrentFrame()->size());
	g->drawImage(visibleArea.point, visibleArea, *canvas->getDrawingBuffer());
}

#pragma endregion
//=================================================================|
