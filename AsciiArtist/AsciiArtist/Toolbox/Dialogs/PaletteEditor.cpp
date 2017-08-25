/*=================================================================|
|* File:				PaletteEditor.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PaletteEditor.h"
#include <PowerConsole/Helpers/ToString.h>
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <AsciiArtist/Toolbox/Dialogs/AsciiImageFileDialog.h>

using namespace PowerConsole;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::IO;
using namespace AsciiArtist::Toolbox::Dialogs;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

PaletteEditor::PaletteEditor(ConsolePalette paletteResult)
 :	DialogBox("Palette Editor", DialogBoxButtons::OKCancel, DialogBoxIcons::None),
 
	paletteResult(paletteResult),
	selection(0) {

	eventInitialize().add(bindEvent(PaletteEditor::onInitialize));
	eventPaint().add(bindEvent(PaletteEditor::onPaint));

	setSizeSafe(Point2I(54, 25));

	palettePostions[WHITE] = Point2I(0, 0);
	palettePostions[GRAY] = Point2I(1, 0);
	palettePostions[RED] = Point2I(2, 0);
	palettePostions[YELLOW] = Point2I(3, 0);
	palettePostions[GREEN] = Point2I(4, 0);
	palettePostions[CYAN] = Point2I(5, 0);
	palettePostions[BLUE] = Point2I(6, 0);
	palettePostions[MAGENTA] = Point2I(7, 0);

	palettePostions[BLACK] = Point2I(0, 1);
	palettePostions[DARK_GRAY] = Point2I(1, 1);
	palettePostions[DARK_RED] = Point2I(2, 1);
	palettePostions[DARK_YELLOW] = Point2I(3, 1);
	palettePostions[DARK_GREEN] = Point2I(4, 1);
	palettePostions[DARK_CYAN] = Point2I(5, 1);
	palettePostions[DARK_BLUE] = Point2I(6, 1);
	palettePostions[DARK_MAGENTA] = Point2I(7, 1);
}
void PaletteEditor::onInitialize(ControlSPtr sender, EventArgs& e) {
	#pragma region Group Boxes

	groupBoxColor = makeControl(GroupBox);
	groupBoxColor->setHeader("Color");
	groupBoxColor->setSize(Point2I(29, 6));
	groupBoxColor->setMargin(Thickness(2, 3, 0, 0));
	groupBoxColor->eventPaint().add(bindEvent(PaletteEditor::onColorGroupBoxPaint));
	children().add(groupBoxColor);

	groupBoxPalette = makeControl(GroupBox);
	groupBoxPalette->setHeader("Palette");
	groupBoxPalette->setSize(Point2I(29, 9));
	groupBoxPalette->setMargin(Thickness(2, 9, 0, 0));
	groupBoxPalette->eventPaint().add(bindEvent(PaletteEditor::onPaletteGroupBoxPaint));
	groupBoxPalette->eventClick().add(bindEvent(PaletteEditor::onPaletteClicked));
	children().add(groupBoxPalette);

	groupBoxInfo = makeControl(GroupBox);
	groupBoxInfo->setHeader("Info");
	groupBoxInfo->setSize(Point2I(20, 15));
	groupBoxInfo->setMargin(Thickness(32, 3, 0, 0));
	groupBoxInfo->eventPaint().add(bindEvent(PaletteEditor::onInfoGroupBoxPaint));
	children().add(groupBoxInfo);

	#pragma endregion

	#pragma region Buttons

	buttonImport = makeControl(Button);
	buttonImport->setText("Import");
	buttonImport->setSize(Point2I(8, 1));
	buttonImport->setMargin(Thickness(3, 19, 0, 0));
	buttonImport->eventClick().add(bindEvent(PaletteEditor::onImportClicked));
	children().add(buttonImport);

	buttonExport = makeControl(Button);
	buttonExport->setText("Export");
	buttonExport->setSize(Point2I(8, 1));
	buttonExport->setMargin(Thickness(13, 19, 0, 0));
	buttonExport->eventClick().add(bindEvent(PaletteEditor::onExportClicked));
	children().add(buttonExport);

	buttonDefaults = makeControl(Button);
	buttonDefaults->setText("Defaults");
	buttonDefaults->setSize(Point2I(10, 1));
	buttonDefaults->setMargin(Thickness(23, 19, 0, 0));
	buttonDefaults->eventClick().add(bindEvent(PaletteEditor::onDefaultsClicked));
	children().add(buttonDefaults);

	#pragma endregion

	#pragma region Color

	labelRed = makeControl(Label);
	labelRed->setText("Red");
	labelRed->setMargin(Thickness(3, 0, 0, 0));
	groupBoxColor->children().add(labelRed);

	labelGrn = makeControl(Label);
	labelGrn->setText("Grn");
	labelGrn->setMargin(Thickness(10, 0, 0, 0));
	groupBoxColor->children().add(labelGrn);

	labelBlue = makeControl(Label);
	labelBlue->setText("Blue");
	labelBlue->setMargin(Thickness(17, 0, 0, 0));
	groupBoxColor->children().add(labelBlue);

	textBoxR = makeControl(IntTextBox);
	textBoxR->setValue(paletteResult[selection].r);
	textBoxR->setMinimum(0);
	textBoxR->setMaximum(255);
	textBoxR->setIncrement(2);
	textBoxR->setMaxLength(3);
	textBoxR->setSize(Point2I(4, 1));
	textBoxR->setMargin(Thickness(5, 2, 0, 0));
	textBoxR->setSelectAllOnFocus(true);
	textBoxR->eventValueChanged().add(bindEvent(PaletteEditor::onValueChanged));
	groupBoxColor->children().add(textBoxR);
	tabStops().add(textBoxR);

	textBoxG = makeControl(IntTextBox);
	textBoxG->setValue(paletteResult[selection].g);
	textBoxG->setMinimum(0);
	textBoxG->setMaximum(255);
	textBoxG->setIncrement(2);
	textBoxG->setMaxLength(3);
	textBoxG->setSize(Point2I(4, 1));
	textBoxG->setMargin(Thickness(12, 2, 0, 0));
	textBoxG->setSelectAllOnFocus(true);
	textBoxG->eventValueChanged().add(bindEvent(PaletteEditor::onValueChanged));
	groupBoxColor->children().add(textBoxG);
	tabStops().add(textBoxG);

	textBoxB = makeControl(IntTextBox);
	textBoxB->setValue(paletteResult[selection].b);
	textBoxB->setMinimum(0);
	textBoxB->setMaximum(255);
	textBoxB->setIncrement(2);
	textBoxB->setMaxLength(3);
	textBoxB->setSize(Point2I(4, 1));
	textBoxB->setMargin(Thickness(19, 2, 0, 0));
	textBoxB->setSelectAllOnFocus(true);
	textBoxB->eventValueChanged().add(bindEvent(PaletteEditor::onValueChanged));
	groupBoxColor->children().add(textBoxB);
	tabStops().add(textBoxB);

	#pragma endregion

	#pragma region Info

	labelColorName = makeControl(Label);
	labelColorName->setText("Color Name:");
	labelColorName->setMargin(Thickness(1, 1, 0, 0));
	groupBoxInfo->children().add(labelColorName);

	labelColorNameValue = makeControl(Label);
	labelColorNameValue->setText("Black");
	labelColorNameValue->setMargin(Thickness(3, 2, 0, 0));
	groupBoxInfo->children().add(labelColorNameValue);

	labelColorCode = makeControl(Label);
	labelColorCode->setText("Color Code:");
	labelColorCode->setMargin(Thickness(1, 4, 0, 0));
	groupBoxInfo->children().add(labelColorCode);

	labelColorCodeValue = makeControl(Label);
	labelColorCodeValue->setText("0x0");
	labelColorCodeValue->setMargin(Thickness(3, 5, 0, 0));
	groupBoxInfo->children().add(labelColorCodeValue);

	labelInvertColor = makeControl(Label);
	labelInvertColor->setText("Invert Color:");
	labelInvertColor->setMargin(Thickness(1, 8, 0, 0));
	groupBoxInfo->children().add(labelInvertColor);

	labelLightDarkColor = makeControl(Label);
	labelLightDarkColor->setText("Light Color:");
	labelLightDarkColor->setMargin(Thickness(1, 11, 0, 0));
	groupBoxInfo->children().add(labelLightDarkColor);

	#pragma endregion

	console().setPalette(paletteResult);
	textBoxR->focus();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool PaletteEditor::show(WindowSPtr owner, ConsolePalette& paletteResult) {
	auto dialogBox = makePrivateDialog(PaletteEditor, paletteResult);
	owner->showDialog(dialogBox);
	if (dialogBox->getDialogBoxResult() == DialogBoxResults::OK) {
		paletteResult = dialogBox->paletteResult;
		return true;
	}
	return false;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

bool PaletteEditor::buttonBehavior(DialogBoxResults buttonPressed) {
	if (buttonPressed == DialogBoxResults::OK) {

	}
	return true;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void PaletteEditor::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Button Borders
	g->drawBorder(Rectangle2I(buttonImport->getLocalPosition() - 1, buttonImport->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(buttonExport->getLocalPosition() - 1, buttonExport->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(buttonDefaults->getLocalPosition() - 1, buttonDefaults->getActualSize() + 2), Pixel(' ', frameColor), false);
}
void PaletteEditor::onColorGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Text Box Borders
	g->drawBorder(Rectangle2I(textBoxR->getLocalPosition() - 1, textBoxR->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxG->getLocalPosition() - 1, textBoxG->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxB->getLocalPosition() - 1, textBoxB->getActualSize() + 2), Pixel(' ', frameColor), false);
}
void PaletteEditor::onPaletteGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Palette Borders
	g->drawBorder(Rectangle2I(2, 1, 25, 7), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(2, 4, 25, 1), Pixel(' ', frameColor), false);
	for (int i = 1; i < 8; i++) {
		g->drawBorder(Rectangle2I(2 + i * 3, 1, 1, 7), Pixel(' ', frameColor), false);
	}

	// Draw the actual palette
	for (unsigned char i = 0; i < ConsolePalette::NUM_COLORS; i++) {
		Point2I point = palettePostions[i] * 3 + Point2I(3, 2);
		Rectangle2I colorBounds = Rectangle2I(point, 2, 2);
		if (colorBounds.containsPoint(groupBoxPalette->getMousePosition())) {
			unsigned char color = toColor(i, BLACK);
			if (palettePostions[i].y == 1)
				color = toColor(i, WHITE);
			g->setPixel(point, Pixel('/', color));
			g->setPixel(point + 1, Pixel('/', color));
			g->setPixel(point + Point2I(1, 0), Pixel('\\', color));
			g->setPixel(point + Point2I(0, 1), Pixel('\\', color));
		}
		else {
			g->drawRectangle(colorBounds, Pixel(Characters::BLOCK_FULL, i), true);
		}
		if (i == selection)
			g->drawBorder(Rectangle2I(point - 1, 4, 4), Pixel(' ', frameColor), true);
	}
}
void PaletteEditor::onInfoGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Invert/Light/Dark Borders
	g->drawBorder(Rectangle2I(15, 8, 3, 3), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(15, 11, 3, 3), Pixel(' ', frameColor), false);

	// Invert/Light/Dark Colors
	g->setPixel(Point2I(16, 9), Pixel(Characters::BLOCK_FULL, invertColor(selection)));
	g->setPixel(Point2I(16, 12), Pixel(Characters::BLOCK_FULL, (selection < 0x8 ? lightenColor(selection) : darkenColor(selection))));
}
void PaletteEditor::onValueChanged(ControlSPtr sender, IntChangedEventArgs& e) {
	paletteResult[selection].r = textBoxR->getValue();
	paletteResult[selection].g = textBoxG->getValue();
	paletteResult[selection].b = textBoxB->getValue();

	console().setPaletteColor(selection, paletteResult[selection]);

	textBoxR->setValue(paletteResult[selection].r);
	textBoxG->setValue(paletteResult[selection].g);
	textBoxB->setValue(paletteResult[selection].b);
}
void PaletteEditor::onImportClicked(ControlSPtr sender, MouseEventArgs& e) {
	auto fileDialog = makeDialog(OpenCustomFileDialog);
	fileDialog->setDefaultExt(".conpal");
	fileDialog->addFilter(".conpal");
	fileDialog->setFileName("*.conpal");
	if (showDialog(fileDialog)) {
		std::string filePath = fileDialog->getFileName();
		if (paletteResult.loadFile(filePath)) {

			console().setPalette(paletteResult);
			textBoxR->setValue(paletteResult[selection].r);
			textBoxG->setValue(paletteResult[selection].g);
			textBoxB->setValue(paletteResult[selection].b);
			textBoxR->focus();
		}
		else {
			MessageBox::show(getWindow(),
				"Error importing palette! The palette may have been invalid or this program may not have access to open files in this directory.",
				"Import Palette",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
}
void PaletteEditor::onExportClicked(ControlSPtr sender, MouseEventArgs& e) {
	auto fileDialog = makeDialog(SaveCustomFileDialog);
	fileDialog->setDefaultExt(".conpal");
	fileDialog->addFilter(".conpal");
	fileDialog->setFileName("*.conpal");
	if (showDialog(fileDialog)) {
		std::string filePath = fileDialog->getFileName();
		if (!paletteResult.saveFile(filePath)) {
			MessageBox::show(getWindow(),
				"Error exporting palette! The palette may have been invalid or this program may not have access to modify files in this directory.",
				"Export Palette",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
}
void PaletteEditor::onDefaultsClicked(ControlSPtr sender, MouseEventArgs& e) {
	paletteResult = ConsolePalette::DEFAULT;
	console().setPalette(paletteResult);

	textBoxR->setValue(paletteResult[selection].r);
	textBoxG->setValue(paletteResult[selection].g);
	textBoxB->setValue(paletteResult[selection].b);
}
void PaletteEditor::onPaletteClicked(ControlSPtr sender, MouseEventArgs& e) {
	for (unsigned char i = 0; i < ConsolePalette::NUM_COLORS; i++) {
		Rectangle2I colorBounds = Rectangle2I(palettePostions[i] * 3 + Point2I(3, 2), 2, 2);
		if (colorBounds.containsPoint(e.newPosition)) {
			selection = i;

			textBoxR->setValue(paletteResult[selection].r);
			textBoxG->setValue(paletteResult[selection].g);
			textBoxB->setValue(paletteResult[selection].b);

			if (i < 0x8) {
				labelLightDarkColor->setText("Light Color:");
			}
			else {
				labelLightDarkColor->setText("Dark Color:");
			}

			std::string colorNames[] = {
				"Black", "Dark Blue", "Dark Green", "Dark Cyan", "Dark Red", "Dark Magenta", "Dark Yellow", "Gray",
				"Dark Gray", "Blue", "Green", "Cyan", "Red", "Magenta", "Yellow", "White"
			};

			labelColorNameValue->setText(colorNames[i]);
			labelColorCodeValue->setText("0x" + ToString::intTypeToString(i, RADIX_HEX));
			textBoxR->focus();
		}
	}
}

#pragma endregion
//=================================================================|
