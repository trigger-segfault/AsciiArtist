/*=================================================================|
|* File:				YesNoToggleButton.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "MyYesNoToggleButton.h"

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

MyYesNoToggleButton::MyYesNoToggleButton() 
 :	_eventChecked(EventDirections::FirstInFirstCalled) {

	eventInitialize().add(bindEvent(MyYesNoToggleButton::onInitialize));
	setSizeSafe(Point2I(SIZE_AUTO));
}
void MyYesNoToggleButton::onInitialize(ControlSPtr sender, EventArgs& e) {
	toggleButtonYes = makeControl(MyToggleButton);
	toggleButtonYes->setText("Yes");
	toggleButtonYes->setChecked(false);
	toggleButtonYes->eventClick().add(bindEvent(MyYesNoToggleButton::onToggleButtonClick));
	children().add(toggleButtonYes);

	toggleButtonNo = makeControl(MyToggleButton);
	toggleButtonNo->setText("No");
	toggleButtonNo->setChecked(true);
	toggleButtonNo->setMargin(Thickness(4, 0, 0, 0));
	toggleButtonNo->eventClick().add(bindEvent(MyYesNoToggleButton::onToggleButtonClick));
	children().add(toggleButtonNo);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool MyYesNoToggleButton::isChecked() const {
	return toggleButtonYes->isChecked();
}
void MyYesNoToggleButton::setChecked(bool checked) {
	toggleButtonYes->setChecked(checked);
	toggleButtonNo->setChecked(!checked);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I MyYesNoToggleButton::autoSizeBehavior() const {
	return Point2I(6, 1);
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<EventArgs>& MyYesNoToggleButton::eventChecked() {
	return _eventChecked;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void MyYesNoToggleButton::onToggleButtonClick(ControlSPtr sender, MouseEventArgs& e) {
	if (sender == toggleButtonYes) {
		toggleButtonNo->setChecked(!toggleButtonYes->isChecked());
		_eventChecked(shared_from_this());
	}
	else {
		toggleButtonYes->setChecked(!toggleButtonNo->isChecked());
		_eventChecked(shared_from_this());
	}
}

#pragma endregion
//=================================================================|
