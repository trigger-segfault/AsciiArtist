/*=================================================================|
|* File:				Tool.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Tool.h"

using namespace AsciiArtist::Editing;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Tool::Tool(CanvasSPtr canvas, const std::string& name)
 :	canvas(canvas),
	toolName(name),
	drawing(false),
	invalidatedRegion(Rectangle2I::ZERO) {}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string Tool::getToolName() {
	return toolName;
}
bool Tool::isDrawing() const {
	return drawing;
}
Rectangle2I Tool::getInvalidatedRegion() const {
	return invalidatedRegion;
}
fixed_array<std::string> Tool::getUsedOptions() const {
	return fixed_array<std::string>(0);
}
Point2I Tool::getSelectionSize() const {
	return -Point2I::ONE;
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

bool Tool::characterSelected(unsigned char character, bool transparent) {
	return false;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Tool::onMouseMoved(MouseEventArgs e) {

}
void Tool::onKey(KeyboardEventArgs e) {

}
void Tool::onKeyTyped(KeyTypedEventArgs e) {

}
void Tool::onTick(TickEventArgs e) {

}
void Tool::onPaint(PaintEventArgs e) {

}

#pragma endregion
//=================================================================|
