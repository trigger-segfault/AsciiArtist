/*=================================================================|
|* File:				LoadingDialog.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "LoadingDialog.h"
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <AsciiVis/Colors.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

LoadingDialog::LoadingDialog(const std::string& message, CallFunction callFunction)
	: Dialog(DialogModes::InnerWindow),

	// Setup
	message(message),
	callFunction(callFunction) {

	eventInitialize().add(bindEvent(LoadingDialog::onInitialize));
	eventLoad().add(bindEvent(LoadingDialog::onLoad));
	eventPaint().add(bindEvent(LoadingDialog::onPaint));

	setSizeSafe(Point2I((int)message.length() + 6, 5));
}
void LoadingDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	
}
void LoadingDialog::onLoad(ControlSPtr sender, EventArgs& e) {
	redraw();
	if (callFunction != nullptr)
		callFunction(std::static_pointer_cast<LoadingDialog>(shared_from_this()));
	close();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

void LoadingDialog::show(WindowSPtr owner, const std::string& message, CallFunction callFunction) {
	auto dialog = makePrivateDialog(LoadingDialog, message, callFunction);
	owner->showDialog(dialog);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string LoadingDialog::getMessage() const {
	return message;
}
void LoadingDialog::setMessage(const std::string& message) {
	this->message = message;
	setSize(Point2I((int)message.length() + 6, 5));
	redraw();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void LoadingDialog::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	g->drawRectangle(Rectangle2I(getActualSize()), Pixel(' ', toColor(DARK_BLUE, DARK_BLUE)), true);
	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', toColor(DARK_BLUE, LIGHT_NAVY)), false);

	unsigned char color = toColor(DARK_BLUE, LIGHT_LIGHT_NAVY);

	g->drawString(Point2I(3, 2), message, Pixel(' ', color));
}

#pragma endregion
//=================================================================|
