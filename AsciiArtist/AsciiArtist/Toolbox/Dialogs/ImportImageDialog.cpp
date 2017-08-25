/*=================================================================|
|* File:				ImportImageDialog.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ImportImageDialog.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Controls/Toolbox/MessageBox.h>

using namespace PowerConsole;
using namespace AsciiArtist::Toolbox::Dialogs;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const std::string ImportImageDialog::ASCIIFY_NOTICE = "Asciification may take some time to complete.";

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

ImportImageDialog::ImportImageDialog(ImportImageSettings importResult)
	: DialogBox("Asciify Image", DialogBoxButtons::OKCancel, DialogBoxIcons::None),

	// Setup
	importResult(importResult) {

	eventInitialize().add(bindEvent(ImportImageDialog::onInitialize));

	wordWrap = WordWrapper::wrap(ASCIIFY_NOTICE, 1, 21, 21);

	setSizeSafe(Point2I(48, 22));
}
void ImportImageDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	#pragma region Group Boxes

	groupBoxColorMetrics = makeControl(GroupBox);
	groupBoxColorMetrics->setHeader("Color Metrics");
	groupBoxColorMetrics->setSize(Point2I(18, 7));
	groupBoxColorMetrics->setMargin(Thickness(2, 3, 0, 0));
	children().add(groupBoxColorMetrics);

	groupBoxPrecision = makeControl(GroupBox);
	groupBoxPrecision->setHeader("Precision");
	groupBoxPrecision->setSize(Point2I(18, 8));
	groupBoxPrecision->setMargin(Thickness(2, 10, 0, 0));
	groupBoxPrecision->eventPaint().add(bindEvent(ImportImageDialog::onPrecisionGroupBoxPaint));
	children().add(groupBoxPrecision);

	groupBoxImage = makeControl(GroupBox);
	groupBoxImage->setHeader("Image");
	groupBoxImage->setSize(Point2I(26, 8));
	groupBoxImage->setMargin(Thickness(20, 3, 0, 0));
	groupBoxImage->eventPaint().add(bindEvent(ImportImageDialog::onImageGroupBoxPaint));
	children().add(groupBoxImage);

	groupBoxNotice = makeControl(GroupBox);
	groupBoxNotice->setHeader("Notice");
	groupBoxNotice->setSize(Point2I(26, 7));
	groupBoxNotice->setMargin(Thickness(20, 11, 0, 0));
	groupBoxNotice->eventPaint().add(bindEvent(ImportImageDialog::onNoticeGroupBoxPaint));
	children().add(groupBoxNotice);

	#pragma endregion

	#pragma region Color Metrics

	listBoxMetric = makeControl(ListBox);
	listBoxMetric->setSize(Point2I(14, 13));
	listBoxMetric->setMargin(Thickness(1, 1, 0, 0));
	listBoxMetric->setNoSelectionAllowed(false);
	listBoxMetric->setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxMetric->setVerticalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxMetric->items().add("Euclidean");
	listBoxMetric->items().add("LAB Delta E");
	listBoxMetric->setSelectedIndex((int)importResult.metric);
	groupBoxColorMetrics->children().add(listBoxMetric);

	#pragma endregion

	#pragma region Precision

	labelFontSize = makeControl(Label);
	labelFontSize->setText("Font: " + std::to_string(importResult.fontSize.x) + "x" + std::to_string(importResult.fontSize.y));
	labelFontSize->setMargin(Thickness(1, 1, 0, 0));
	groupBoxPrecision->children().add(labelFontSize);

	labelX = makeControl(Label);
	labelX->setText("X");
	labelX->setMargin(Thickness(8, 3, 0, 0));
	groupBoxPrecision->children().add(labelX);

	textBoxPrecisionX = makeControl(IntTextBox);
	textBoxPrecisionX->setValue(importResult.split.x);
	textBoxPrecisionX->setSelectAllOnFocus(true);
	textBoxPrecisionX->setMaxLength(2);
	textBoxPrecisionX->setMinimum(1);
	textBoxPrecisionX->setSize(Point2I(4, 1));
	textBoxPrecisionX->setMargin(Thickness(3, 3, 0, 0));
	groupBoxPrecision->children().add(textBoxPrecisionX);
	tabStops().add(textBoxPrecisionX);

	textBoxPrecisionY = makeControl(IntTextBox);
	textBoxPrecisionY->setValue(importResult.split.y);
	textBoxPrecisionY->setSelectAllOnFocus(true);
	textBoxPrecisionY->setMaxLength(2);
	textBoxPrecisionY->setMinimum(1);
	textBoxPrecisionY->setSize(Point2I(4, 1));
	textBoxPrecisionY->setMargin(Thickness(10, 3, 0, 0));
	groupBoxPrecision->children().add(textBoxPrecisionY);
	tabStops().add(textBoxPrecisionY);

	#pragma endregion

	#pragma region Image

	labelScale = makeControl(Label);
	labelScale->setText("Scale:");
	labelScale->setMargin(Thickness(3, 1, 0, 0));
	groupBoxImage->children().add(labelScale);

	labelPercent = makeControl(Label);
	labelPercent->setText("%");
	labelPercent->setMargin(Thickness(18, 1, 0, 0));
	groupBoxImage->children().add(labelPercent);

	Point2I finalSize = (Point2I)GMath::ceil(((Point2F)importResult.imageSize * importResult.scale) / (Point2F)importResult.fontSize);
	labelImageSize = makeControl(Label);
	labelImageSize->setText("Image Size: " + std::to_string(finalSize.x) + "x" + std::to_string(finalSize.y));
	labelImageSize->setMargin(Thickness(2, 3, 0, 0));
	groupBoxImage->children().add(labelImageSize);

	int minImageDimension = std::min(importResult.imageSize.x, importResult.imageSize.y);
	int minScale = (int)std::ceil(1.0f / (float)minImageDimension);
	textBoxScale = makeControl(IntTextBox);
	textBoxScale->setValue((int)(importResult.scale * 100));
	textBoxScale->setErrorValue(100);
	textBoxScale->setMinimum(minScale);
	textBoxScale->setMaximum(99999);
	textBoxScale->setIncrement(5);
	textBoxScale->setSelectAllOnFocus(true);
	textBoxScale->setMaxLength(5);
	textBoxScale->setSize(Point2I(6, 1));
	textBoxScale->setMargin(Thickness(11, 1, 0, 0));
	textBoxScale->eventValueChanged().add(bindEvent(ImportImageDialog::onScaleValueChanged));
	groupBoxImage->children().add(textBoxScale);
	tabStops().add(textBoxScale);

	#pragma endregion
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool ImportImageDialog::show(WindowSPtr owner, ImportImageSettings& importResult) {
	auto dialogBox = makePrivateDialog(ImportImageDialog, importResult);
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

bool ImportImageDialog::buttonBehavior(DialogBoxResults buttonPressed) {
	if (buttonPressed == DialogBoxResults::OK) {
		importResult.metric = (ColorMetrics)listBoxMetric->getSelectedIndex();
		importResult.split.x = textBoxPrecisionX->getValue();
		importResult.split.y = textBoxPrecisionY->getValue();
		importResult.scale = (float)textBoxScale->getValue() / 100.0f;

		Point2I finalSize = (Point2I)GMath::ceil(((Point2F)importResult.imageSize * importResult.scale) / (Point2F)importResult.fontSize);
		if (!(finalSize <= AsciiImageInfo::MAX_SIZE)) {
			MessageBox::show(getWindow(),
				"Image size cannot be greater than " + std::to_string(AsciiImageInfo::MAX_SIZE.x) + "x" + std::to_string(AsciiImageInfo::MAX_SIZE.y) + ". " +
				"Change the scale to fit the maximum size.",
				"Invalid Size",
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

void ImportImageDialog::onPrecisionGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	g->drawBorder(Rectangle2I(textBoxPrecisionX->getLocalPosition() - 1, textBoxPrecisionX->getActualSize() + 2), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(textBoxPrecisionY->getLocalPosition() - 1, textBoxPrecisionY->getActualSize() + 2), Pixel(' ', frameColor), false);
}
void ImportImageDialog::onImageGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);
	const unsigned char errorColor = toColor(BLACK, RED);

	g->drawBorder(Rectangle2I(textBoxScale->getLocalPosition() - 1, textBoxScale->getActualSize() + 2), Pixel(' ', frameColor), false);

	Point2I finalSize = (Point2I)((Point2F)importResult.imageSize * importResult.scale) / importResult.fontSize;
	if (!(finalSize <= AsciiImageInfo::MAX_SIZE)) {
		g->drawString(
			Point2I(5, 5),
			"Max Size: " + std::to_string(AsciiImageInfo::MAX_SIZE.x) + "x" + std::to_string(AsciiImageInfo::MAX_SIZE.y),
			Pixel(' ', errorColor)
		);
	}
}
void ImportImageDialog::onNoticeGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char textColor = toColor(BLACK, GRAY);

	for (int i = 0; i < wordWrap.lines; i++) {
		g->drawString(Point2I(3, 2 + i), ASCIIFY_NOTICE.substr(wordWrap.lineStarts[i], wordWrap.lineLengths[i]), Pixel(' ', textColor));
	}
}
void ImportImageDialog::onScaleValueChanged(ControlSPtr sender, IntChangedEventArgs& e) {
	importResult.scale = (float)textBoxScale->getValue() / 100.0f;
	Point2I finalSize = (Point2I)GMath::ceil(((Point2F)importResult.imageSize * importResult.scale) / (Point2F)importResult.fontSize);
	labelImageSize->setText("Image Size: " + std::to_string(finalSize.x) + "x" + std::to_string(finalSize.y));
}

#pragma endregion
//=================================================================|
