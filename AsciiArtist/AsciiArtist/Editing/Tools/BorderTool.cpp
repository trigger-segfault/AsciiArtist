/*=================================================================|
|* File:				BorderTool.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "BorderTool.h"
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

BorderTool::BorderTool(CanvasSPtr canvas)
	: Tool(canvas, "Border"),
	secondary(false),
	startPoint(Point2I::ZERO),
	shift(false) {

	canvas->addOption("Overwrte", true);
	canvas->addOption("Double", true);
	canvas->addOption("Erase", true);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

fixed_array<std::string> BorderTool::getUsedOptions() const {
	return fixed_array<std::string>{"Overwrte", "Double", "Erase"};
}
Point2I BorderTool::getSelectionSize() const {
	auto canvas = this->canvas.lock();
	return Rectangle2I(startPoint, canvas->getMousePosition() - startPoint).pixelBounds().size;
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void BorderTool::finalize() {
	auto canvas = this->canvas.lock();
	if (drawing) {
		drawing = false;
		auto undoAction = std::make_shared<DrawUndoAction>("Border", canvas->getFrameIndex());
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
void BorderTool::cancel() {
	if (drawing) {
		drawing = false;
	}
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void BorderTool::drawChanges() {
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
		if (abs.x >= abs.y) {
			if (abs.x / 2 <= abs.y)
				distance.x = (int)std::round((float)abs.y / (float)fontSize.x) * GMath::sign(distance.x);
			else
				distance.y = 0;
		}
		else {
			if (abs.y / 2 <= abs.x)
				distance.y = (int)std::round((float)abs.x / (float)fontSize.y) * GMath::sign(distance.y);
			else
				distance.x = 0;
		}
	}

	Rectangle2I rectangle = Rectangle2I(startPoint, distance).pixelBounds();
	if (secondary && canvas->getOption("Erase")) {
		g->drawRectangle(rectangle, canvas->getDrawingPixel(secondary), true);
		rectangle.inflate(Thickness(1));
		for (int x = 0; x < rectangle.size.x; x++) {
			Point2I p = rectangle.point + Point2I(x, 0);
			g->forceBorderPixel(p, 0, 0, 0, 0, g->getPixel(p));
			p = rectangle.point + Point2I(x, rectangle.size.y - 1);
			g->forceBorderPixel(p, 0, 0, 0, 0, g->getPixel(p));
		}
		for (int y = 1; y < rectangle.size.y - 1; y++) {
			Point2I p = rectangle.point + Point2I(0, y);
			g->forceBorderPixel(p, 0, 0, 0, 0, g->getPixel(p));
			p = rectangle.point + Point2I(rectangle.size.x - 1, y);
			g->forceBorderPixel(p, 0, 0, 0, 0, g->getPixel(p));
		}
	}
	else {
		g->drawBorder(rectangle, canvas->getDrawingPixel(secondary), canvas->getOption("Double"));
	}
	invalidatedRegion = rectangle.intersection(Rectangle2I(Point2I::ZERO, canvas->getCurrentFrame()->size()));
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void BorderTool::onMouseButton(MouseButtonEventArgs e) {
	auto canvas = this->canvas.lock();
	shift = getFlag(e.modifiers, Modifiers::Shift);
	if (e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right)) {
		if (!drawing) {
			drawing = true;
			secondary = e.buttonChanged == MouseButtons::Right;
			startPoint = e.newPosition;

			Pixel drawingPixel = canvas->getDrawingPixel(secondary);

			// Draw the first pixel
			auto g = canvas->getDrawingBufferGraphics();
			g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
			g->drawImage(Point2I::ZERO, *canvas->getCurrentFrame());
			g->setModAttributes(canvas->getModAttributes());
			g->setModOptions(canvas->getModOptions());

			if (secondary && canvas->getOption("Erase")) {
				invalidatedRegion = Rectangle2I::ZERO;
				drawChanges();
			}
			else {
				invalidatedRegion = Rectangle2I(e.newPosition, Point2I::ONE);
				g->drawBorder(Rectangle2I(e.newPosition, 1), canvas->getDrawingPixel(secondary), canvas->getOption("Double"));
			}
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
void BorderTool::onMouseMoved(MouseEventArgs e) {
	drawChanges();
}
void BorderTool::onKey(KeyboardEventArgs e) {
	shift = getFlag(e.modifiers, Modifiers::Shift);
	drawChanges();
}
void BorderTool::onPaint(PaintEventArgs e) {
	auto canvas = this->canvas.lock();
	auto g = e.graphics;
	Rectangle2I visibleArea = canvas->getLocalVisibleArea();
	visibleArea.size = GMath::min(visibleArea.size, canvas->getCurrentFrame()->size());
	g->drawImage(visibleArea.point, visibleArea, *canvas->getDrawingBuffer());
}

#pragma endregion
//=================================================================|
