/*=================================================================|
|* File:				FrameSpeedDialogBox.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FrameSpeedDialogBox.h"
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace AsciiArtist::Toolbox::Dialogs;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

FrameSpeedDialogBox::FrameSpeedDialogBox(unsigned int speedResult, bool setAllResult)
	: DialogBox("Frame Speed", DialogBoxButtons::OKCancel, DialogBoxIcons::None),

	// Setup
	speedResult(speedResult),
	setAllResult(setAllResult) {

	eventInitialize().add(bindEvent(FrameSpeedDialogBox::onInitialize));
	eventPaint().add(bindEvent(FrameSpeedDialogBox::onPaint));

	setSizeSafe(Point2I(28, 12));
}
void FrameSpeedDialogBox::onInitialize(ControlSPtr sender, EventArgs& e) {
	labelSpeed = makeControl(Label);
	labelSpeed->setText("Speed:");
	labelSpeed->setMargin(Thickness(3, 4, 0, 0));
	children().add(labelSpeed);

	labelMS = makeControl(Label);
	labelMS->setText("ms");
	labelMS->setMargin(Thickness(22, 4, 0, 0));
	children().add(labelMS);

	labelSetAll = makeControl(Label);
	labelSetAll->setText("Set All:");
	labelSetAll->setMargin(Thickness(3, 6, 0, 0));
	children().add(labelSetAll);

	textBoxSpeed = makeControl(IntTextBox);
	textBoxSpeed->setValue(speedResult);
	textBoxSpeed->setErrorValue(speedResult);
	textBoxSpeed->setMinimum(1);
	textBoxSpeed->setMaximum(10 * 60 * 100); // Max frame length for gif
	textBoxSpeed->setSize(Point2I(10, 1));
	textBoxSpeed->setMargin(Thickness(11, 4, 0, 0));
	textBoxSpeed->setMaxLength(9);
	textBoxSpeed->setSelectAllOnFocus(true);
	children().add(textBoxSpeed);
	tabStops().add(textBoxSpeed);

	toggleButtonAll = makeControl(YesNoToggleButton);
	toggleButtonAll->setMargin(Thickness(12, 6, 0, 0));
	toggleButtonAll->setChecked(setAllResult);
	children().add(toggleButtonAll);

	textBoxSpeed->focus();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool FrameSpeedDialogBox::show(WindowSPtr owner, unsigned int& speedResult, bool& setAllResult) {
	auto dialogBox = makePrivateDialog(FrameSpeedDialogBox, speedResult, setAllResult);
	owner->showDialog(dialogBox);
	if (dialogBox->getDialogBoxResult() == DialogBoxResults::OK) {
		speedResult = dialogBox->speedResult;
		setAllResult = dialogBox->setAllResult;
		return true;
	}
	return false;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

bool FrameSpeedDialogBox::buttonBehavior(DialogBoxResults buttonPressed) {
	if (buttonPressed == DialogBoxResults::OK) {
		speedResult = (unsigned int)textBoxSpeed->getValue();
		setAllResult = toggleButtonAll->isChecked();
	}

	return true;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void FrameSpeedDialogBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw the text box borders
	g->drawBorder(Rectangle2I(textBoxSpeed->getLocalPosition() - 1, textBoxSpeed->getActualSize() + 2), Pixel(' ', frameColor), false);
}

#pragma endregion
//=================================================================|
