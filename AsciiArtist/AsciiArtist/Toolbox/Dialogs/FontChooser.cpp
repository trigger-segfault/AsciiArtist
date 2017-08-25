/*=================================================================|
|* File:				FontChooser.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FontChooser.h"
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

FontChooser::FontChooser(ConsoleFont fontResult)
 :	DialogBox("Font Chooser", DialogBoxButtons::OKCancel, DialogBoxIcons::None),
	
	fontResult(fontResult) {

	eventInitialize().add(bindEvent(FontChooser::onInitialize));
	eventPaint().add(bindEvent(FontChooser::onPaint));

	setSizeSafe(Point2I(36, 24));
}
void FontChooser::onInitialize(ControlSPtr sender, EventArgs& e) {
	groupBoxFont = makeControl(GroupBox);
	groupBoxFont->setHeader("Font");
	groupBoxFont->setSize(Point2I(20, 7));
	groupBoxFont->setMargin(Thickness(2, 3, 0, 0));
	children().add(groupBoxFont);

	groupBoxSize = makeControl(GroupBox);
	groupBoxSize->setHeader("Size");
	groupBoxSize->setSize(Point2I(11, 14));
	groupBoxSize->setMargin(Thickness(23, 3, 0, 0));
	groupBoxSize->eventPaint().add(bindEvent(FontChooser::onSizeGroupBoxPaint));
	children().add(groupBoxSize);

	groupBoxWeight = makeControl(GroupBox);
	groupBoxWeight->setHeader("Weight");
	groupBoxWeight->setSize(Point2I(20, 7));
	groupBoxWeight->setMargin(Thickness(2, 10, 0, 0));
	children().add(groupBoxWeight);

	listBoxFont = makeControl(ListBox);
	listBoxFont->setNoSelectionAllowed(false);
	listBoxFont->setArrowNavigationAllowed(true);
	listBoxFont->setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxFont->setVerticalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxFont->setSize(Point2I(16, 3));
	listBoxFont->setMargin(Thickness(1, 1, 0, 0));
	listBoxFont->eventMouseButton().add(bindEvent(FontChooser::onListBoxMouseButton));
	groupBoxFont->children().add(listBoxFont);
	tabStops().add(listBoxFont);

	listBoxFont->items().add("Terminal");
	listBoxFont->items().add("Consolas");
	listBoxFont->items().add("Lucida Console");
	if (fontResult.getFontType() == ConsoleFonts::None)
		fontResult.setFontType(ConsoleFonts::Terminal);
	listBoxFont->setSelectedIndex((int)fontResult.getFontType() - 1);
	listBoxFont->eventSelectionChanged().add(bindEvent(FontChooser::onFontSelectionChanged));

	listBoxSize = makeControl(ListBox);
	listBoxSize->setNoSelectionAllowed(false);
	listBoxSize->setArrowNavigationAllowed(true);
	listBoxSize->setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxSize->setVerticalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxSize->setSize(Point2I(7, 10));
	listBoxSize->setMargin(Thickness(1, 1, 0, 0));
	listBoxSize->setVisible(fontResult.getFontType() == ConsoleFonts::Terminal);
	listBoxSize->eventMouseButton().add(bindEvent(FontChooser::onListBoxMouseButton));
	groupBoxSize->children().add(listBoxSize);
	tabStops().add(listBoxSize);

	listBoxSize->items().add("4x6");
	listBoxSize->items().add("6x8");
	listBoxSize->items().add("8x8");
	listBoxSize->items().add("16x8");
	listBoxSize->items().add("5x12");
	listBoxSize->items().add("7x12");
	listBoxSize->items().add("8x12");
	listBoxSize->items().add("16x12");
	listBoxSize->items().add("12x16");
	listBoxSize->items().add("10x18");
	listBoxSize->setSelectedIndex(getIndexFromFontSize(fontResult.getFontSize()));
	listBoxSize->eventSelectionChanged().add(bindEvent(FontChooser::onSizeSelectionChanged));

	textBoxSize = makeControl(IntTextBox);
	textBoxSize->setSize(Point2I(5, 1));
	textBoxSize->setMargin(Thickness(2, 2, 0, 0));
	textBoxSize->setSelectAllOnFocus(false);
	textBoxSize->setValue(fontResult.getTrueTypeFontSize());
	textBoxSize->setMinimum(10);
	textBoxSize->setMaximum(24);
	textBoxSize->setErrorValue(12);
	textBoxSize->setVisible(fontResult.getFontType() != ConsoleFonts::Terminal);
	textBoxSize->eventValueChanged().add(bindEvent(FontChooser::onSizeValueChanged));
	groupBoxSize->children().add(textBoxSize);
	tabStops().add(textBoxSize);

	listBoxWeight = makeControl(ListBox);
	listBoxWeight->setNoSelectionAllowed(false);
	listBoxWeight->setArrowNavigationAllowed(true);
	listBoxWeight->setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxWeight->setVerticalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxWeight->setSize(Point2I(10, 2));
	listBoxWeight->setMargin(Thickness(1, 1, 0, 0));
	listBoxWeight->setEnabled(fontResult.getFontType() != ConsoleFonts::Terminal);
	listBoxWeight->eventMouseButton().add(bindEvent(FontChooser::onListBoxMouseButton));
	groupBoxWeight->children().add(listBoxWeight);
	tabStops().add(listBoxWeight);

	listBoxWeight->items().add("Regular");
	listBoxWeight->items().add("Bold");
	listBoxWeight->setSelectedIndex(
		fontResult.getFontWeight() == ConsoleFontWeights::Bold ? 1 : 0
	);
	listBoxWeight->eventSelectionChanged().add(bindEvent(FontChooser::onWeightSelectionChanged));

	buttonImport = makeControl(Button);
	buttonImport->setText("Import");
	buttonImport->setSize(Point2I(8, 1));
	buttonImport->setMargin(Thickness(3, 18, 0, 0));
	buttonImport->eventClick().add(bindEvent(FontChooser::onImportClicked));
	children().add(buttonImport);

	buttonExport = makeControl(Button);
	buttonExport->setText("Export");
	buttonExport->setSize(Point2I(8, 1));
	buttonExport->setMargin(Thickness(13, 18, 0, 0));
	buttonExport->eventClick().add(bindEvent(FontChooser::onExportClicked));
	children().add(buttonExport);

	buttonDefaults = makeControl(Button);
	buttonDefaults->setText("Defaults");
	buttonDefaults->setSize(Point2I(10, 1));
	buttonDefaults->setMargin(Thickness(23, 18, 0, 0));
	buttonDefaults->eventClick().add(bindEvent(FontChooser::onDefaultsClicked));
	children().add(buttonDefaults);

	listBoxFont->focus();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool FontChooser::show(WindowSPtr owner, ConsoleFont& fontResult) {
	auto dialogBox = makePrivateDialog(FontChooser, fontResult);
	owner->showDialog(dialogBox);
	if (dialogBox->getDialogBoxResult() == DialogBoxResults::OK) {
		fontResult = dialogBox->fontResult;
		return true;
	}
	return false;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

bool FontChooser::buttonBehavior(DialogBoxResults buttonPressed) {
	return true;
}
Point2I FontChooser::getFontSizeFromIndex(int index) const {
	switch (index) {
	case 0: return Point2I(4, 6);
	case 1: return Point2I(6, 8);
	case 2: return Point2I(8, 8);
	case 3: return Point2I(16, 8);
	case 4: return Point2I(5, 12);
	case 5: return Point2I(7, 12);
	case 6: return Point2I(8, 12);
	case 7: return Point2I(16, 12);
	case 8: return Point2I(12, 16);
	case 9: return Point2I(10, 18);
	default: return Point2I(8, 12);
	}
}
int FontChooser::getIndexFromFontSize(Point2I fontSize) const {
	auto rasterSize = toRasterSize(fontSize.x, fontSize.y);
	switch (rasterSize) {
	case RasterFontSizes::R4x6: return 0;
	case RasterFontSizes::R6x8: return 1;
	case RasterFontSizes::R8x8: return 2;
	case RasterFontSizes::R16x8: return 3;
	case RasterFontSizes::R5x12: return 4;
	case RasterFontSizes::R7x12: return 5;
	case RasterFontSizes::R8x12: return 6;
	case RasterFontSizes::R16x12: return 7;
	case RasterFontSizes::R12x16: return 8;
	case RasterFontSizes::R10x18: return 9;
	default: return 6;
	}
}
void FontChooser::updateFont() {
	console().setFont(fontResult);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void FontChooser::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	g->drawBorder(Rectangle2I(buttonImport->getLocalPosition() - 1, buttonImport->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(buttonExport->getLocalPosition() - 1, buttonExport->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(buttonDefaults->getLocalPosition() - 1, buttonDefaults->getActualSize() + 2), Pixel(' ', frameColor), false);
}
void FontChooser::onSizeGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	if (textBoxSize->isVisible())
		g->drawBorder(Rectangle2I(textBoxSize->getLocalPosition() - 1, textBoxSize->getActualSize() + 2), Pixel(' ', frameColor), false);
}
void FontChooser::onFontSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	fontResult.setFontType((ConsoleFonts)(e.newIndex + 1));
	listBoxSize->setVisible(e.newIndex == 0);
	textBoxSize->setVisible(e.newIndex != 0);
	listBoxWeight->setEnabled(e.newIndex != 0);
	if (e.newIndex == 0) {
		fontResult.setFontSize(getFontSizeFromIndex(listBoxSize->getSelectedIndex()));
		listBoxWeight->setSelectedIndex(fontResult.getFontWeight() == ConsoleFontWeights::Bold ? 1 : 0);
	}
	else {
		fontResult.setTrueTypeFontSize(textBoxSize->getValue());
	}
	updateFont();
}
void FontChooser::onSizeSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	fontResult.setFontSize(getFontSizeFromIndex(e.newIndex));
	listBoxWeight->setSelectedIndex(fontResult.getFontWeight() == ConsoleFontWeights::Bold ? 1 : 0);
	updateFont();
}
void FontChooser::onWeightSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	fontResult.setFontWeight(e.newIndex == 1 ? ConsoleFontWeights::Bold : ConsoleFontWeights::Regular);
	updateFont();
}
void FontChooser::onSizeValueChanged(ControlSPtr sender, IntChangedEventArgs& e) {
	fontResult.setTrueTypeFontSize(e.newValue);
	updateFont();
}
void FontChooser::onImportClicked(ControlSPtr sender, MouseEventArgs& e) {
	auto fileDialog = makeDialog(OpenCustomFileDialog);
	fileDialog->setDefaultExt(".confnt");
	fileDialog->addFilter(".confnt");
	fileDialog->setFileName("*.confnt");
	if (showDialog(fileDialog)) {
		std::string filePath = fileDialog->getFileName();
		if (fontResult.loadFile(filePath)) {

			console().setFont(fontResult);
			listBoxFont->setSelectedIndex((int)fontResult.getFontType() - 1);
			listBoxWeight->setSelectedIndex(fontResult.getFontWeight() == ConsoleFontWeights::Bold ? 1 : 0);
			listBoxSize->setVisible(fontResult.getFontType() == ConsoleFonts::Terminal);
			textBoxSize->setVisible(fontResult.getFontType() != ConsoleFonts::Terminal);
			listBoxWeight->setEnabled(fontResult.getFontType() != ConsoleFonts::Terminal);
			if (fontResult.getFontType() == ConsoleFonts::Terminal) {
				listBoxSize->setSelectedIndex(getIndexFromFontSize(fontResult.getFontSize()));
			}
			else {
				textBoxSize->setValue(fontResult.getTrueTypeFontSize());
			}
			listBoxFont->focus();
		}
		else {
			MessageBox::show(getWindow(),
				"Error importing font! The font may have been invalid or this program may not have access to open files in this directory.",
				"Import Font",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
}
void FontChooser::onExportClicked(ControlSPtr sender, MouseEventArgs& e) {
	auto fileDialog = makeDialog(SaveCustomFileDialog);
	fileDialog->setDefaultExt(".confnt");
	fileDialog->addFilter(".confnt");
	fileDialog->setFileName("*.confnt");
	if (showDialog(fileDialog)) {
		std::string filePath = fileDialog->getFileName();
		if (!fontResult.saveFile(filePath)) {
			MessageBox::show(getWindow(),
				"Error exporting font! The font may have been invalid or this program may not have access to modify files in this directory.",
				"Export Font",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
}
void FontChooser::onDefaultsClicked(ControlSPtr sender, MouseEventArgs& e) {
	fontResult = ConsoleFont::DEFAULT;
	listBoxSize->setVisible(true);
	textBoxSize->setVisible(false);
	listBoxWeight->setEnabled(false);
	listBoxFont->setSelectedIndex(0);
	listBoxWeight->setSelectedIndex(0);
	listBoxSize->setSelectedIndex(getIndexFromFontSize(Point2I(8, 12)));
}
void FontChooser::onListBoxMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		sender->focus();
	}
}

#pragma endregion
//=================================================================|
