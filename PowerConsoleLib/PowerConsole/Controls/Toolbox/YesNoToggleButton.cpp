/*=================================================================|
|* File:				YesNoToggleButton.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "YesNoToggleButton.h"

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

YesNoToggleButton::YesNoToggleButton() {
	eventInitialize().add(bindEvent(YesNoToggleButton::onInitialize));
	setSizeSafe(Point2I(SIZE_AUTO));
}
void YesNoToggleButton::onInitialize(ControlSPtr sender, EventArgs& e) {
	toggleButtonYes = makeControl(ToggleButton);
	toggleButtonYes->setText("Yes");
	toggleButtonYes->setChecked(false);
	toggleButtonYes->eventClick().add(bindEvent(YesNoToggleButton::onToggleButtonClick));
	children().add(toggleButtonYes);

	toggleButtonNo = makeControl(ToggleButton);
	toggleButtonNo->setText("No");
	toggleButtonNo->setChecked(true);
	toggleButtonNo->setMargin(Thickness(4, 0, 0, 0));
	toggleButtonNo->eventClick().add(bindEvent(YesNoToggleButton::onToggleButtonClick));
	children().add(toggleButtonNo);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool YesNoToggleButton::isChecked() const {
	return toggleButtonYes->isChecked();
}
void YesNoToggleButton::setChecked(bool checked) {
	toggleButtonYes->setChecked(checked);
	toggleButtonNo->setChecked(!checked);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I YesNoToggleButton::autoSizeBehavior() const {
	return Point2I(6, 1);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void YesNoToggleButton::onToggleButtonClick(ControlSPtr sender, MouseEventArgs& e) {
	if (sender == toggleButtonYes) {
		toggleButtonNo->setChecked(!toggleButtonYes->isChecked());
	}
	else {
		toggleButtonYes->setChecked(!toggleButtonNo->isChecked());
	}
}

#pragma endregion
//=================================================================|
