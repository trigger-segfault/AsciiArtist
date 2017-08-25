/*=================================================================|
|* File:				ImageSettingsDialog.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ImageSettingsDialog.h"
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Helpers/Flags.h>
#include <AsciiArtist/Editing/PixelRepresentation.h>
#include <AsciiArtist/Toolbox/Dialogs/PixelPickerDialog.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace AsciiArtist::Toolbox::Dialogs;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

ImageSettingsDialog::ImageSettingsDialog(ImageSettings settingsResult)
	: DialogBox("Image Settings", DialogBoxButtons::OKCancel, DialogBoxIcons::None),

	// Setup
	settingsResult(settingsResult) {

	eventInitialize().add(bindEvent(ImageSettingsDialog::onInitialize));

	setSizeSafe(Point2I(42, 22));
}
void ImageSettingsDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	#pragma region Group Boxes

	groupBoxFormat = makeControl(GroupBox);
	groupBoxFormat->setHeader("Format");
	groupBoxFormat->setSize(Point2I(16, 8));
	groupBoxFormat->setMargin(Thickness(2, 3, 0, 0));
	children().add(groupBoxFormat);

	groupBoxVersion = makeControl(GroupBox);
	groupBoxVersion->setHeader("File Version");
	groupBoxVersion->setSize(Point2I(21, 8));
	groupBoxVersion->setMargin(Thickness(19, 3, 0, 0));
	children().add(groupBoxVersion);

	groupBoxBackground = makeControl(GroupBox);
	groupBoxBackground->setHeader("Background");
	groupBoxBackground->setSize(Point2I(38, 7));
	groupBoxBackground->setMargin(Thickness(2, 11, 0, 0));
	groupBoxBackground->eventPaint().add(bindEvent(ImageSettingsDialog::onBackgroundGroupBoxPaint));
	children().add(groupBoxBackground);

	#pragma endregion

	#pragma region Format Toggle Buttons

	toggleButtonAttributes = makeControl(ToggleButton);
	toggleButtonAttributes->setText("Attributes");
	toggleButtonAttributes->setChecked(getFlag(settingsResult.format, ImageFormats::Attributes));
	toggleButtonAttributes->setMargin(Thickness(2, 1, 0, 0));
	toggleButtonAttributes->eventClick().add(bindEvent(ImageSettingsDialog::onToggleButtonClick));
	groupBoxFormat->children().add(toggleButtonAttributes);

	toggleButtonAnimation = makeControl(ToggleButton);
	toggleButtonAnimation->setText("Animation");
	toggleButtonAnimation->setChecked(getFlag(settingsResult.format, ImageFormats::Animation));
	toggleButtonAnimation->setMargin(Thickness(2, 2, 0, 0));
	toggleButtonAnimation->eventClick().add(bindEvent(ImageSettingsDialog::onToggleButtonClick));
	groupBoxFormat->children().add(toggleButtonAnimation);

	toggleButtonMultisize = makeControl(ToggleButton);
	toggleButtonMultisize->setText("Multisize");
	toggleButtonMultisize->setChecked(getFlag(settingsResult.format, ImageFormats::Multisize));
	toggleButtonMultisize->setMargin(Thickness(2, 3, 0, 0));
	toggleButtonMultisize->setEnabled(!settingsResult.smallFile);
	toggleButtonMultisize->eventClick().add(bindEvent(ImageSettingsDialog::onToggleButtonClick));
	groupBoxFormat->children().add(toggleButtonMultisize);

	#pragma endregion

	#pragma region File Version Toggle Buttons

	/*toggleButtonVersion2 = makeControl(ToggleButton);
	toggleButtonVersion2->setText("Version 2");
	toggleButtonVersion2->setChecked(settingsResult.fileVersion == 2 && !settingsResult.smallFile);
	toggleButtonVersion2->setMargin(Thickness(2, 1, 0, 0));
	toggleButtonVersion2->eventClick().add(bindEvent(ImageSettingsDialog::onToggleButtonClick));
	groupBoxVersion->children().add(toggleButtonVersion2);

	toggleButtonVersion3 = makeControl(ToggleButton);
	toggleButtonVersion3->setText("Version 3");
	toggleButtonVersion3->setChecked(settingsResult.fileVersion == 3 && !settingsResult.smallFile);
	toggleButtonVersion3->setMargin(Thickness(2, 2, 0, 0));
	toggleButtonVersion3->eventClick().add(bindEvent(ImageSettingsDialog::onToggleButtonClick));
	groupBoxVersion->children().add(toggleButtonVersion3);

	toggleButtonSmallVersion1 = makeControl(ToggleButton);
	toggleButtonSmallVersion1->setText("Small Version 1");
	toggleButtonSmallVersion1->setChecked(settingsResult.fileVersion == 1 && settingsResult.smallFile);
	toggleButtonSmallVersion1->setMargin(Thickness(2, 3, 0, 0));
	toggleButtonSmallVersion1->eventClick().add(bindEvent(ImageSettingsDialog::onToggleButtonClick));
	groupBoxVersion->children().add(toggleButtonSmallVersion1);*/

	listBoxVersion = makeControl(ListBox);
	listBoxVersion->setSize(Point2I(17, 4));
	listBoxVersion->setMargin(Thickness(1, 1, 0, 0));
	listBoxVersion->setNoSelectionAllowed(false);
	listBoxVersion->setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	listBoxVersion->setVerticalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	groupBoxVersion->children().add(listBoxVersion);

	listBoxVersion->items().add("Version 2");
	listBoxVersion->items().add("Version 3");
	listBoxVersion->items().add("Version 4");
	listBoxVersion->items().add("Small Version 1");
	switch (settingsResult.fileVersion) {
	case 2: listBoxVersion->setSelectedIndex(0); break;
	case 3: listBoxVersion->setSelectedIndex(1); break;
	case 4: listBoxVersion->setSelectedIndex(2); break;
	case 1: listBoxVersion->setSelectedIndex(3); break;
	}
	listBoxVersion->eventSelectionChanged().add(bindEvent(ImageSettingsDialog::onVersionChanged));

	#pragma endregion

	#pragma region Background

	buttonBackground = makeControl(Button);
	buttonBackground->setText("Change");
	buttonBackground->setSize(Point2I(8, 1));
	buttonBackground->setMargin(Thickness(3, 2, 0, 0));
	buttonBackground->eventClick().add(bindEvent(ImageSettingsDialog::onChangeBackgroundClick));
	groupBoxBackground->children().add(buttonBackground);

	labelCurrent = makeControl(Label);
	labelCurrent->setText("Current:");
	labelCurrent->setMargin(Thickness(15, 2, 0, 0));
	groupBoxBackground->children().add(labelCurrent);

	#pragma endregion
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

bool ImageSettingsDialog::show(WindowSPtr owner, ImageSettings& settingsResult) {
	auto dialogBox = makePrivateDialog(ImageSettingsDialog, settingsResult);
	owner->showDialog(dialogBox);
	if (dialogBox->getDialogBoxResult() == DialogBoxResults::OK) {
		settingsResult = dialogBox->settingsResult;
		return true;
	}
	return false;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

bool ImageSettingsDialog::buttonBehavior(DialogBoxResults buttonPressed) {
	if (buttonPressed == DialogBoxResults::OK) {
		
	}

	return true;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ImageSettingsDialog::onBackgroundGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw the text box borders
	g->drawBorder(Rectangle2I(buttonBackground->getLocalPosition() - 1, buttonBackground->getActualSize() + 2), Pixel(' ', frameColor), false);

	// Draw the background preview border
	g->drawBorder(Rectangle2I(25, 2, 7, 3), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(27, 2, 1, 3), Pixel(' ', frameColor), false);

	// Draw the background preview
	g->setPixel(Point2I(26, 3), settingsResult.background);
	g->setPixel(Point2I(28, 3), PixelRepresentation::getPixelPart(settingsResult.background, PixelParts::Character));
	g->setPixel(Point2I(29, 3), PixelRepresentation::getPixelPart(settingsResult.background, PixelParts::Foreground));
	g->setPixel(Point2I(30, 3), PixelRepresentation::getPixelPart(settingsResult.background, PixelParts::Background));
}
void ImageSettingsDialog::onToggleButtonClick(ControlSPtr sender, MouseEventArgs& e) {
	bool checked = std::static_pointer_cast<ToggleButton>(sender)->isChecked();

	if (sender == toggleButtonAttributes) {
		setFlag(settingsResult.format, ImageFormats::Attributes, checked);
		if (!checked)
			settingsResult.background.attributes = PixelAttributes::Fill;
	}
	else if (sender == toggleButtonAnimation) {
		setFlag(settingsResult.format, ImageFormats::Animation, checked);
	}
	else if (sender == toggleButtonMultisize) {
		setFlag(settingsResult.format, ImageFormats::Multisize, checked);
	}
	
	/*if (sender == toggleButtonVersion2) {
		toggleButtonVersion2->setChecked(true);
		toggleButtonVersion3->setChecked(false);
		toggleButtonSmallVersion1->setChecked(false);
		toggleButtonMultisize->setEnabled(true);
		settingsResult.fileVersion = 2;
		settingsResult.smallFile = false;
	}
	else if (sender == toggleButtonVersion3) {
		toggleButtonVersion3->setChecked(true);
		toggleButtonVersion2->setChecked(false);
		toggleButtonSmallVersion1->setChecked(false);
		toggleButtonMultisize->setEnabled(true);
		settingsResult.fileVersion = 3;
		settingsResult.smallFile = false;
	}
	else if (sender == toggleButtonSmallVersion1) {
		toggleButtonSmallVersion1->setChecked(true);
		toggleButtonVersion2->setChecked(false);
		toggleButtonVersion3->setChecked(false);
		toggleButtonMultisize->setChecked(false);
		toggleButtonMultisize->setEnabled(false);
		setFlag(settingsResult.format, ImageFormats::Multisize, false);
		settingsResult.fileVersion = 1;
		settingsResult.smallFile = true;
	}*/

}
void ImageSettingsDialog::onVersionChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	switch (e.newIndex) {
	case 0: // Version 2
		toggleButtonMultisize->setChecked(false);
		toggleButtonMultisize->setEnabled(false);
		settingsResult.fileVersion = 2;
		settingsResult.smallFile = false;
		setFlag(settingsResult.format, ImageFormats::Multisize, false);
		break;
	case 1: // Version 3
		toggleButtonMultisize->setEnabled(true);
		settingsResult.fileVersion = 3;
		settingsResult.smallFile = false;
		break;
	case 2: // Version 4
		toggleButtonMultisize->setEnabled(true);
		settingsResult.fileVersion = 4;
		settingsResult.smallFile = false;
		break;
	case 3: // Small Version 1
		toggleButtonMultisize->setChecked(false);
		toggleButtonMultisize->setEnabled(false);
		settingsResult.fileVersion = 1;
		settingsResult.smallFile = true;
		setFlag(settingsResult.format, ImageFormats::Multisize, false);
		break;
	}
}
void ImageSettingsDialog::onChangeBackgroundClick(ControlSPtr sender, MouseEventArgs& e) {
	// Show dialog here
	Pixel background = settingsResult.background;
	if (PixelPickerDialog::show(getWindow(), background, getFlag(settingsResult.format, ImageFormats::Attributes))) {
		settingsResult.background = background;
	}
}

#pragma endregion
//=================================================================|
