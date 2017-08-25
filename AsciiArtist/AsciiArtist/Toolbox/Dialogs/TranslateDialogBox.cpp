/*=================================================================|
|* File:				ResizeDialogBox.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TranslateDialogBox.h"
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

TranslateDialogBox::TranslateDialogBox(Point2I distanceResult, bool translateAllResult)
	: DialogBox("Translate Image", DialogBoxButtons::OKCancel, DialogBoxIcons::None),

	// Setup
	distanceResult(distanceResult),
	translateAllResult(translateAllResult) {

	eventInitialize().add(bindEvent(TranslateDialogBox::onInitialize));
	eventPaint().add(bindEvent(TranslateDialogBox::onPaint));

	setSizeSafe(Point2I(34, 12));
}
void TranslateDialogBox::onInitialize(ControlSPtr sender, EventArgs& e) {
	labelDistance = makeControl(Label);
	labelDistance->setText("Distance:");
	labelDistance->setMargin(Thickness(3, 4, 0, 0));
	children().add(labelDistance);

	labelComma = makeControl(Label);
	labelComma->setText(",");
	labelComma->setMargin(Thickness(21, 4, 0, 0));
	children().add(labelComma);

	labelTranslateAll = makeControl(Label);
	labelTranslateAll->setText("Translate All:");
	labelTranslateAll->setMargin(Thickness(3, 6, 0, 0));
	children().add(labelTranslateAll);

	textBoxX = makeControl(IntTextBox);
	textBoxX->setValue(distanceResult.x);
	textBoxX->setSize(Point2I(6, 1));
	textBoxX->setMargin(Thickness(14, 4, 0, 0));
	textBoxX->setMaxLength(5);
	textBoxX->setSelectAllOnFocus(true);
	children().add(textBoxX);
	tabStops().add(textBoxX);

	textBoxY = makeControl(IntTextBox);
	textBoxY->setValue(distanceResult.y);
	textBoxY->setSize(Point2I(6, 1));
	textBoxY->setMargin(Thickness(23, 4, 0, 0));
	textBoxY->setMaxLength(5);
	textBoxY->setSelectAllOnFocus(true);
	children().add(textBoxY);
	tabStops().add(textBoxY);

	toggleButtonAll = makeControl(YesNoToggleButton);
	toggleButtonAll->setMargin(Thickness(18, 6, 0, 0));
	toggleButtonAll->setChecked(translateAllResult);
	children().add(toggleButtonAll);

	textBoxX->focus();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool TranslateDialogBox::show(WindowSPtr owner, Point2I& distanceResult, bool& translateAllResult) {
	auto dialogBox = makePrivateDialog(TranslateDialogBox, distanceResult, translateAllResult);
	owner->showDialog(dialogBox);
	if (dialogBox->getDialogBoxResult() == DialogBoxResults::OK) {
		distanceResult = dialogBox->distanceResult;
		translateAllResult = dialogBox->translateAllResult;
		return true;
	}
	return false;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

bool TranslateDialogBox::buttonBehavior(DialogBoxResults buttonPressed) {
	if (buttonPressed == DialogBoxResults::OK) {
		distanceResult.x = textBoxX->getValue();
		distanceResult.y = textBoxY->getValue();
		translateAllResult = toggleButtonAll->isChecked();
	}

	return true;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void TranslateDialogBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw the text box borders
	g->drawBorder(Rectangle2I(textBoxX->getLocalPosition() - 1, textBoxX->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxY->getLocalPosition() - 1, textBoxY->getActualSize() + 2), Pixel(' ', frameColor), false);
}

#pragma endregion
//=================================================================|
