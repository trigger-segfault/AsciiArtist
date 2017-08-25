/*=================================================================|
|* File:				DialogNew.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Dialog.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Dialog::Dialog(DialogModes dialogMode) 
 :	dialogMode(dialogMode),
	dialogResult(false) {

	eventInitialize().add(bindEvent(Dialog::onInitialize));
	eventGlobalTick().add(bindEvent(Dialog::onGlobalTick));

	if (dialogMode == DialogModes::InnerWindow) {
		setHorizontalAlignmentSafe(HorizontalAlignments::Center);
		setVerticalAlignmentSafe(VerticalAlignments::Center);
		setSizeSafe(Point2I(40, 10));
	}
	else {
		
	}
}
void Dialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	if (dialogMode == DialogModes::EntireWindow) {
		setSize(Point2I(80, 25));
	}
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void Dialog::repaint(GraphicsSPtr g) {
	if (dialogMode == DialogModes::InnerWindow && hasOwner())
		getOwner()->repaint(g);

	Window::repaint(g);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Control Tree

void Dialog::setOwner(WindowSPtr owner) {
	Window::setOwner(owner);
}

#pragma endregion
//--------------------------------
#pragma region Dialog

DialogModes Dialog::getDialogMode() const {
	return dialogMode;
}
bool Dialog::getDialogResult() const {
	return dialogResult;
}
void Dialog::setDialogResult(bool dialogResult) {
	this->dialogResult = dialogResult;
}

#pragma endregion
//--------------------------------
#pragma region Local Layout

Point2I Dialog::getSize() const {
	if (dialogMode == DialogModes::InnerWindow) {
		return ContainerControl::getSize();
	}
	else {
		return Window::getSize();
	}
}
void Dialog::setSize(Point2I size) {
	size = GMath::clamp(size, getActualMinSize(), getActualMaxSize());
	if (dialogMode == DialogModes::InnerWindow) {
		ContainerControl::setSize(size);
	}
	else {
		Window::setSize(size);
	}
}

#pragma endregion
//--------------------------------
#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Dialog::onGlobalTick(ControlSPtr sender, TickEventArgs& e) {
	getOwner()->eventGlobalTick()(getOwner(), e);
}

#pragma endregion
//=================================================================|
