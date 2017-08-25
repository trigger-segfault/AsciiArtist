/*=================================================================|
|* File:				ResizeDialogBox.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ResizeDialogBox.h"
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

ResizeDialogBox::ResizeDialogBox(Point2I sizeResult, bool resizeAllResult, bool canResizeSingle)
 :	DialogBox("Resize Image", DialogBoxButtons::OKCancel, DialogBoxIcons::None),
	
	// Setup
	sizeResult(sizeResult),
	resizeAllResult(resizeAllResult),
	canResizeSingle(canResizeSingle) {

	eventInitialize().add(bindEvent(ResizeDialogBox::onInitialize));
	eventPaint().add(bindEvent(ResizeDialogBox::onPaint));

	setSizeSafe(Point2I(32, 14));
}
void ResizeDialogBox::onInitialize(ControlSPtr sender, EventArgs& e) {
	labelSize = makeControl(Label);
	labelSize->setText("Size:");
	labelSize->setMargin(Thickness(3, 4, 0, 0));
	children().add(labelSize);

	labelX = makeControl(Label);
	labelX->setText("X");
	labelX->setMargin(Thickness(17, 4, 0, 0));
	children().add(labelX);

	labelResizeAll = makeControl(Label);
	labelResizeAll->setText("Resize All:");
	labelResizeAll->setMargin(Thickness(3, 6, 0, 0));
	children().add(labelResizeAll);

	labelCurrentSize = makeControl(Label);
	labelCurrentSize->setText("Current Size: " + std::to_string(sizeResult.x) + "x" + std::to_string(sizeResult.y));
	labelCurrentSize->setMargin(Thickness(3, 8, 0, 0));
	children().add(labelCurrentSize);

	textBoxWidth = makeControl(IntTextBox);
	textBoxWidth->setValue(sizeResult.x);
	textBoxWidth->setErrorValue(sizeResult.x);
	textBoxWidth->setMinimum(1);
	textBoxWidth->setMaximum(AsciiImageInfo::MAX_SIZE.x);
	textBoxWidth->setSize(Point2I(6, 1));
	textBoxWidth->setMargin(Thickness(10, 4, 0, 0));
	textBoxWidth->setMaxLength(5);
	textBoxWidth->setSelectAllOnFocus(true);
	children().add(textBoxWidth);
	tabStops().add(textBoxWidth);

	textBoxHeight = makeControl(IntTextBox);
	textBoxHeight->setValue(sizeResult.y);
	textBoxHeight->setErrorValue(sizeResult.y);
	textBoxHeight->setMinimum(1);
	textBoxHeight->setMaximum(AsciiImageInfo::MAX_SIZE.y);
	textBoxHeight->setSize(Point2I(6, 1));
	textBoxHeight->setMargin(Thickness(19, 4, 0, 0));
	textBoxHeight->setMaxLength(5);
	textBoxHeight->setSelectAllOnFocus(true);
	children().add(textBoxHeight);
	tabStops().add(textBoxHeight);

	toggleButtonAll = makeControl(YesNoToggleButton);
	toggleButtonAll->setMargin(Thickness(15, 6, 0, 0));
	toggleButtonAll->setChecked(resizeAllResult);
	toggleButtonAll->setEnabled(canResizeSingle);
	children().add(toggleButtonAll);

	textBoxWidth->focus();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool ResizeDialogBox::show(WindowSPtr owner, Point2I& sizeResult, bool& resizeAllResult, bool canResizeSingle) {
	auto dialogBox = makePrivateDialog(ResizeDialogBox, sizeResult, resizeAllResult, canResizeSingle);
	owner->showDialog(dialogBox);
	if (dialogBox->getDialogBoxResult() == DialogBoxResults::OK) {
		sizeResult = dialogBox->sizeResult;
		resizeAllResult = dialogBox->resizeAllResult;
		return true;
	}
	return false;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

bool ResizeDialogBox::buttonBehavior(DialogBoxResults buttonPressed) {
	if (buttonPressed == DialogBoxResults::OK) {
		sizeResult.x = textBoxWidth->getValue();
		sizeResult.y = textBoxHeight->getValue();
		resizeAllResult = toggleButtonAll->isChecked();
	}

	return true;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ResizeDialogBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw the text box borders
	g->drawBorder(Rectangle2I(textBoxWidth->getLocalPosition() - 1, textBoxWidth->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxHeight->getLocalPosition() - 1, textBoxHeight->getActualSize() + 2), Pixel(' ', frameColor), false);
}

#pragma endregion
//=================================================================|
