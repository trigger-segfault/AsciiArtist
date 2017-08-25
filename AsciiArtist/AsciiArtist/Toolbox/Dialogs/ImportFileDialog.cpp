/*=================================================================|
|* File:				ImportFileDialog.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ImportFileDialog.h"
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

ImportFileDialog::ImportFileDialog(ImportFileSettings importResult)
	: DialogBox("Asciify File", DialogBoxButtons::OKCancel, DialogBoxIcons::None),

	// Setup
	importResult(importResult) {

	eventInitialize().add(bindEvent(ImportFileDialog::onInitialize));
	eventPaint().add(bindEvent(ImportFileDialog::onPaint));

	setSizeSafe(Point2I(43, 20));
}
void ImportFileDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	labelSize = makeControl(Label);
	labelSize->setText("Size:");
	labelSize->setMargin(Thickness(3, 4, 0, 0));
	children().add(labelSize);

	labelX = makeControl(Label);
	labelX->setText("X");
	labelX->setMargin(Thickness(11, 6, 0, 0));
	children().add(labelX);

	labelStartPosition = makeControl(Label);
	labelStartPosition->setText("Start Position:");
	labelStartPosition->setMargin(Thickness(23, 4, 0, 0));
	children().add(labelStartPosition);

	labelBytesAfterChar = makeControl(Label);
	labelBytesAfterChar->setText("Bytes after Char:");
	labelBytesAfterChar->setMargin(Thickness(3, 9, 0, 0));
	children().add(labelBytesAfterChar);

	labelBytesAfterColor = makeControl(Label);
	labelBytesAfterColor->setText("Bytes after Color:");
	labelBytesAfterColor->setMargin(Thickness(23, 9, 0, 0));
	children().add(labelBytesAfterColor);

	labelReadChar = makeControl(Label);
	labelReadChar->setText("Read Char:");
	labelReadChar->setMargin(Thickness(3, 14, 0, 0));
	children().add(labelReadChar);

	labelReadColor = makeControl(Label);
	labelReadColor->setText("Read Color:");
	labelReadColor->setMargin(Thickness(23, 14, 0, 0));
	children().add(labelReadColor);

	#pragma region Text Boxes

	textBoxWidth = makeControl(IntTextBox);
	textBoxWidth->setValue(importResult.size.x);
	textBoxWidth->setSize(Point2I(6, 1));
	textBoxWidth->setMargin(Thickness(4, 6, 0, 0));
	textBoxWidth->setMaxLength(5);
	textBoxWidth->setMinimum(1);
	textBoxWidth->setMaximum(AsciiImageInfo::MAX_SIZE.x);
	textBoxWidth->setErrorValue(1);
	textBoxWidth->setSelectAllOnFocus(true);
	children().add(textBoxWidth);
	tabStops().add(textBoxWidth);

	textBoxHeight = makeControl(IntTextBox);
	textBoxHeight->setValue(importResult.size.y);
	textBoxHeight->setSize(Point2I(6, 1));
	textBoxHeight->setMargin(Thickness(13, 6, 0, 0));
	textBoxHeight->setMaxLength(5);
	textBoxHeight->setMinimum(1);
	textBoxHeight->setMaximum(AsciiImageInfo::MAX_SIZE.y);
	textBoxHeight->setErrorValue(1);
	textBoxHeight->setSelectAllOnFocus(true);
	children().add(textBoxHeight);
	tabStops().add(textBoxHeight);

	textBoxStartPosition = makeControl(IntTextBox);
	textBoxStartPosition->setValue(importResult.startPosition);
	textBoxStartPosition->setSize(Point2I(15, 1));
	textBoxStartPosition->setMargin(Thickness(24, 6, 0, 0));
	textBoxStartPosition->setMaxLength(9);
	textBoxStartPosition->setMinimum(0);
	textBoxStartPosition->setSelectAllOnFocus(true);
	children().add(textBoxStartPosition);
	tabStops().add(textBoxStartPosition);

	textBoxBytesAfterChar = makeControl(IntTextBox);
	textBoxBytesAfterChar->setValue(importResult.bytesAfterCharacter);
	textBoxBytesAfterChar->setSize(Point2I(15, 1));
	textBoxBytesAfterChar->setMargin(Thickness(4, 11, 0, 0));
	textBoxBytesAfterChar->setMaxLength(9);
	textBoxBytesAfterChar->setMinimum(0);
	textBoxBytesAfterChar->setSelectAllOnFocus(true);
	children().add(textBoxBytesAfterChar);
	tabStops().add(textBoxBytesAfterChar);

	textBoxBytesAfterColor = makeControl(IntTextBox);
	textBoxBytesAfterColor->setValue(importResult.bytesAfterColor);
	textBoxBytesAfterColor->setSize(Point2I(15, 1));
	textBoxBytesAfterColor->setMargin(Thickness(24, 11, 0, 0));
	textBoxBytesAfterColor->setMaxLength(9);
	textBoxBytesAfterColor->setMinimum(0);
	textBoxBytesAfterColor->setSelectAllOnFocus(true);
	children().add(textBoxBytesAfterColor);
	tabStops().add(textBoxBytesAfterColor);

	#pragma endregion

	#pragma region Toggle Buttons

	toggleButtonChar = makeControl(YesNoToggleButton);
	toggleButtonChar->setMargin(Thickness(14, 14, 0, 0));
	toggleButtonChar->setChecked(importResult.readCharacters);
	children().add(toggleButtonChar);

	toggleButtonColor = makeControl(YesNoToggleButton);
	toggleButtonColor->setMargin(Thickness(35, 14, 0, 0));
	toggleButtonColor->setChecked(importResult.readCharacters);
	children().add(toggleButtonColor);

	#pragma endregion

	textBoxWidth->focus();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool ImportFileDialog::show(WindowSPtr owner, ImportFileSettings& importResult) {
	auto dialogBox = makePrivateDialog(ImportFileDialog, importResult);
	owner->showDialog(dialogBox);
	if (dialogBox->getDialogBoxResult() == DialogBoxResults::OK) {
		importResult = dialogBox->importResult;
		return true;
	}
	return false;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

bool ImportFileDialog::buttonBehavior(DialogBoxResults buttonPressed) {
	if (buttonPressed == DialogBoxResults::OK) {
		importResult.size.x = textBoxWidth->getValue();
		importResult.size.y = textBoxHeight->getValue();
		importResult.startPosition = textBoxStartPosition->getValue();
		importResult.bytesAfterCharacter = textBoxBytesAfterChar->getValue();
		importResult.bytesAfterColor = textBoxBytesAfterColor->getValue();

		importResult.readCharacters = toggleButtonChar->isChecked();
		importResult.readColors = toggleButtonColor->isChecked();

		if (!importResult.readCharacters && !importResult.readColors) {
			MessageBox::show(getWindow(),
				"Either characters or colors must be read in order to asciify a file.",
				"Input Required",
				DialogBoxIcons::Warning,
				SystemSounds::Warning
			);
			return false;
		}
	}

	return true;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ImportFileDialog::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw the text box borders
	g->drawBorder(Rectangle2I(textBoxWidth->getLocalPosition() - 1, textBoxWidth->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxHeight->getLocalPosition() - 1, textBoxHeight->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxStartPosition->getLocalPosition() - 1, textBoxStartPosition->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxBytesAfterChar->getLocalPosition() - 1, textBoxBytesAfterChar->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxBytesAfterColor->getLocalPosition() - 1, textBoxBytesAfterColor->getActualSize() + 2), Pixel(' ', frameColor), false);
}

#pragma endregion
//=================================================================|
