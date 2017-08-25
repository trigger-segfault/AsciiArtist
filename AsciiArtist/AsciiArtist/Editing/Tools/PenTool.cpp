/*=================================================================|
|* File:				PenTool.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PenTool.h"
#include <AsciiArtist/Editing/UndoActions/DrawUndoAction.h>
#include <AsciiArtist/Settings.h>
#include <PowerConsole/Geometry/GMath.h>

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

PenTool::PenTool(CanvasSPtr canvas)
 :	Tool(canvas, "Pen"),
	secondary(false) {

	canvas->addOption("Overwrte", true);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

fixed_array<std::string> PenTool::getUsedOptions() const {
	return fixed_array<std::string>{"Overwrte"};
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void PenTool::finalize() {
	auto canvas = this->canvas.lock();
	if (drawing) {
		drawing = false;
		auto undoAction = std::make_shared<DrawUndoAction>("Pen", canvas->getFrameIndex());
		if (Settings::getMaxUndos() > 0)
			undoAction->setOriginalImage(*canvas->getCurrentFrame());

		auto g = canvas->getCurrentFrameGraphics();
		if (canvas->getOption("Overwrte"))
			g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
		g->drawImage(Point2I::ZERO, *canvas->getDrawingBuffer());

		if (Settings::getMaxUndos() > 0)
			undoAction->setNewImage(*canvas->getCurrentFrame());
		canvas->addUndo(undoAction);
	}
}
void PenTool::cancel() {
	if (drawing) {
		drawing = false;
	}
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void PenTool::onMouseButton(MouseButtonEventArgs e) {
	auto canvas = this->canvas.lock();
	if (e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right)) {
		if (!drawing) {
			drawing = true;
			secondary = e.buttonChanged == MouseButtons::Right;

			// Draw the first pixel
			auto g = canvas->getDrawingBufferGraphics();
			g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
			if (canvas->getOption("Overwrte")) {
				g->drawImage(Point2I::ZERO, *canvas->getCurrentFrame());
				g->setModOptions(canvas->getModOptions());
			}
			else {
				g->fill(Pixel(' ', 0x00, PixelAttributes::None));
			}
			g->setModAttributes(canvas->getModAttributes());
			g->setPixel(e.newPosition, canvas->getDrawingPixel(secondary));
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
void PenTool::onMouseMoved(MouseEventArgs e) {
	auto canvas = this->canvas.lock();
	auto g = canvas->getDrawingBufferGraphics();
	g->setModAttributes(canvas->getModAttributes());
	if (!canvas->getOption("Overwrte"))
		g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);
	else
		g->setModOptions(canvas->getModOptions());
	g->drawLine(e.previousPosition, e.newPosition - e.previousPosition, canvas->getDrawingPixel(secondary));
}
void PenTool::onPaint(PaintEventArgs e) {
	auto canvas = this->canvas.lock();
	auto g = e.graphics;
	Rectangle2I visibleArea = canvas->getLocalVisibleArea();
	visibleArea.size = GMath::min(visibleArea.size, canvas->getCurrentFrame()->size());
	if (!canvas->getOption("Overwrte"))
		g->drawImage(visibleArea.point, visibleArea, *canvas->getCurrentFrame());
	g->drawImage(visibleArea.point, visibleArea, *canvas->getDrawingBuffer());
}

#pragma endregion
//=================================================================|
