/*=================================================================|
|* File:				FloodTool.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FloodTool.h"
#include <AsciiArtist/Editing/UndoActions/DrawUndoAction.h>
#include <PowerConsole/Geometry/GMath.h>
#include <AsciiArtist/Settings.h>

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

FloodTool::FloodTool(CanvasSPtr canvas)
	: Tool(canvas, "Flood") {

	canvas->addOption("Global", false);
	canvas->addOption("Specific", true);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

fixed_array<std::string> FloodTool::getUsedOptions() const {
	return fixed_array<std::string>{"Global", "Specific"};
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void FloodTool::finalize() {
	// Finalize is handled in the mouse button event
}
void FloodTool::cancel() {
	// Nothing to cancel
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void FloodTool::onMouseButton(MouseButtonEventArgs e) {
	auto canvas = this->canvas.lock();
	if (e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right)) {
		bool secondary = e.isButtonPressed(MouseButtons::Right);

		// Finalize will be handled here
		auto undoAction = std::make_shared<DrawUndoAction>("Flood fill", canvas->getFrameIndex());
		if (Settings::getMaxUndos() > 0)
			undoAction->setOriginalImage(*canvas->getCurrentFrame());

		auto g = canvas->getCurrentFrameGraphics();
		if (canvas->getOption("Global"))
			g->drawReplace(e.newPosition, canvas->getDrawingPixel(secondary), canvas->getOption("Specific"));
		else
			g->drawFloodFill(e.newPosition, canvas->getDrawingPixel(secondary), canvas->getOption("Specific"));

		if (Settings::getMaxUndos() > 0)
			undoAction->setNewImage(*canvas->getCurrentFrame());
		canvas->addUndo(undoAction);
	}
}

#pragma endregion
//=================================================================|
