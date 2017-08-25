/*=================================================================|
|* File:				PixelPicker.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PixelPickerDialog.h"
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace AsciiArtist::Toolbox;
using namespace AsciiArtist::Toolbox::Dialogs;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

PixelPickerDialog::PixelPickerDialog(Pixel pixelResult, bool allowAttributes)
	: DialogBox("Character/Color Picker", DialogBoxButtons::OKCancel, DialogBoxIcons::None),

	// Setup
	pixelResult(pixelResult),
	allowAttributes(allowAttributes) {

	eventInitialize().add(bindEvent(PixelPickerDialog::onInitialize));

	setSizeSafe(Point2I(38 - (!allowAttributes ? 2 : 0), 25));
}
void PixelPickerDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	groupBoxCharacter = makeControl(GroupBox);
	groupBoxCharacter->setHeader("Character");
	groupBoxCharacter->setSize(Point2I(22, 18));
	groupBoxCharacter->setMargin(Thickness(2, 3, 0, 0));
	children().add(groupBoxCharacter);

	groupBoxColor = makeControl(GroupBox);
	groupBoxColor->setHeader("Color");
	groupBoxColor->setSize(Point2I(12 - (!allowAttributes ? 2 : 0), 4));
	groupBoxColor->setMargin(Thickness(24, 3, 0, 0));
	children().add(groupBoxColor);

	groupBoxDisplay = makeControl(GroupBox);
	groupBoxDisplay->setHeader("Display");
	groupBoxDisplay->setSize(Point2I(12 - (!allowAttributes ? 2 : 0), 5));
	groupBoxDisplay->setMargin(Thickness(24, 8, 0, 0));
	groupBoxDisplay->eventPaint().add(bindEvent(PixelPickerDialog::onDisplayGroupBoxPaint));
	children().add(groupBoxDisplay);

	characterGrid = makeControl(CharacterGrid);
	characterGrid->setSingular(true);
	characterGrid->setAllowAttributes(allowAttributes);
	characterGrid->setDrawingPixel(pixelResult);
	characterGrid->eventPixelChanged().add(bindEvent(PixelPickerDialog::onPixelChanged));
	groupBoxCharacter->children().add(characterGrid);

	colorPalette = makeControl(ColorPalette);
	colorPalette->setSingular(true);
	colorPalette->setAllowAttributes(allowAttributes);
	colorPalette->setDrawingPixel(pixelResult);
	colorPalette->eventPixelChanged().add(bindEvent(PixelPickerDialog::onPixelChanged));
	groupBoxColor->children().add(colorPalette);

	pixelDisplay = makeControl(PixelDisplay);
	pixelDisplay->setMargin(Thickness(!allowAttributes ? 1 : 2, 1, 0, 0));
	pixelDisplay->setSingular(true);
	pixelDisplay->setLeftToRight(false);
	pixelDisplay->setDrawingPixel(pixelResult);
	pixelDisplay->eventPixelChanged().add(bindEvent(PixelPickerDialog::onPixelChanged));
	groupBoxDisplay->children().add(pixelDisplay);
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool PixelPickerDialog::show(WindowSPtr owner, Pixel& pixelResult, bool allowAttributes) {
	auto dialogBox = makePrivateDialog(PixelPickerDialog, pixelResult, allowAttributes);
	owner->showDialog(dialogBox);
	if (dialogBox->getDialogBoxResult() == DialogBoxResults::OK) {
		pixelResult = dialogBox->pixelResult;
		return true;
	}
	return false;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

bool PixelPickerDialog::buttonBehavior(DialogBoxResults buttonPressed) {
	if (buttonPressed == DialogBoxResults::OK) {
		
	}

	return true;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void PixelPickerDialog::onDisplayGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw the pixel display borders
	g->drawBorder(Rectangle2I(pixelDisplay->getLocalPosition() - 1, pixelDisplay->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(pixelDisplay->getLocalPosition() + Point2I(1, -1), 1, 3), Pixel(' ', frameColor), false);
}
void PixelPickerDialog::onPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e) {
	if (sender != characterGrid)
		characterGrid->setDrawingPixel(e.pixel);
	if (sender != colorPalette)
		colorPalette->setDrawingPixel(e.pixel);
	if (sender != pixelDisplay)
		pixelDisplay->setDrawingPixel(e.pixel);
	pixelResult = e.pixel;
}

#pragma endregion
//=================================================================|
