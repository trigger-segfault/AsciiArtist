/*=================================================================|
|* File:				BlankTool.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "BlankTool.h"
#include <AsciiArtist/Editing/UndoActions/DrawUndoAction.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::Tools;
using namespace AsciiArtist::Editing::UndoActions;
using namespace PowerConsole::Input;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

BlankTool::BlankTool(CanvasSPtr canvas)
 :	Tool(canvas, "Blank") {

	canvas->addOption("Overwrte", true);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

fixed_array<std::string> BlankTool::getUsedOptions() const {
	return fixed_array<std::string>{"Overwrte"};
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void BlankTool::finalize() {
	auto canvas = this->canvas.lock();
	if (drawing) {
		drawing = false;
		auto undoAction = std::make_shared<DrawUndoAction>("Blank", canvas->getFrameIndex());
		undoAction->setOriginalImage(*canvas->getCurrentFrame());

		auto g = canvas->getCurrentFrameGraphics();
		g->drawImage(Point2I::ZERO, canvas->getCurrentFrame()->size(), *canvas->getDrawingBuffer());

		undoAction->setNewImage(*canvas->getCurrentFrame());
		canvas->addUndo(undoAction);
	}
}
void BlankTool::cancel() {
	if (drawing) {
		drawing = false;
	}
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void BlankTool::onMouseButton(MouseButtonEventArgs e) {
	auto canvas = this->canvas.lock();

}
void BlankTool::onMouseMoved(MouseEventArgs e) {
	auto canvas = this->canvas.lock();

}
void BlankTool::onKey(KeyboardEventArgs e) {
	auto canvas = this->canvas.lock();

}
void BlankTool::onKeyTyped(KeyTypedEventArgs e) {
	auto canvas = this->canvas.lock();

}
void BlankTool::onTick(TickEventArgs e) {
	auto canvas = this->canvas.lock();

}
void BlankTool::onPaint(PaintEventArgs e) {
	auto canvas = this->canvas.lock();

}

#pragma endregion
//=================================================================|
