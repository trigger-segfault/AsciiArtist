/*=================================================================|
|* File:				EyedropperTool.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "EyedropperTool.h"
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

EyedropperTool::EyedropperTool(CanvasSPtr canvas)
 :	Tool(canvas, "Eyedrpr"),
	previousToolIndex(0) {
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

fixed_array<std::string> EyedropperTool::getUsedOptions() const {
	return fixed_array<std::string>(0);
}
void EyedropperTool::setPreviousToolIndex(int index) {
	this->previousToolIndex = index;
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

void EyedropperTool::finalize() {
	
}
void EyedropperTool::cancel() {
	
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void EyedropperTool::onMouseButton(MouseButtonEventArgs e) {
	auto canvas = this->canvas.lock();
	Pixel pixel = canvas->getCurrentFrame()->getPixel(e.newPosition);
	if (e.isButtonPressed(MouseButtons::Left)) {
		canvas->setDrawingPixel(pixel, false);
		canvas->setToolIndex(previousToolIndex);
	}
	else if (e.isButtonPressed(MouseButtons::Right)) {
		canvas->setDrawingPixel(pixel, true);
		canvas->setToolIndex(previousToolIndex);
	}
}

#pragma endregion
//=================================================================|
