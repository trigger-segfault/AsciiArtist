/*=================================================================|
|* File:				LoadingDialog.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "LoadingDialog.h"
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace AsciiArtist::Toolbox::Dialogs;
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
	labelMessage = makeControl(Label);
	labelMessage->setText(message);
	labelMessage->setMargin(Thickness(3, 2, 0, 0));
	children().add(labelMessage);
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
	return labelMessage->getText();
}
void LoadingDialog::setMessage(const std::string& message) {
	labelMessage->setText(message);
	setSize(Point2I((int)message.length() + 6, 5));
	redraw();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void LoadingDialog::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw the text box borders
	g->drawBorder(Rectangle2I(Point2I::ZERO, getActualSize()), Pixel(' ', frameColor), true);
}

#pragma endregion
//=================================================================|
