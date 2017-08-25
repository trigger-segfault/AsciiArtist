/*=================================================================|
|* File:				SettingsTab.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "SettingsTab.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Controls/Window.h>
#include <AsciiVis/Colors.h>
#include <PowerConsole/Console.h>

using namespace PowerConsole;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

SettingsTab::SettingsTab(VisualizationSPtr vis) {
	eventInitialize().add(bindEvent(SettingsTab::onInitialize));
	eventPaint().add(bindEvent(SettingsTab::onPaint));
}
void SettingsTab::onInitialize(ControlSPtr sender, EventArgs& e) {
	int offset = 1;

	labelCompact = makeControl(MyLabel);
	labelCompact->setMargin(Thickness(1, offset, 0, 0));
	labelCompact->setText("Compact Font:");
	children().add(labelCompact); offset += 1;

	toggleCompact = makeControl(MyYesNoToggleButton);
	toggleCompact->setMargin(Thickness(3, offset, 0, 0));
	toggleCompact->setChecked(Console::getRasterFontSize() == RasterFontSizes::R8x8);
	toggleCompact->eventChecked().add(bindEvent(SettingsTab::onCompactChecked));
	children().add(toggleCompact); offset += 2;

	setSize(Point2I(40, offset));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void SettingsTab::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	
}
void SettingsTab::onCompactChecked(ControlSPtr sender, EventArgs& e) {
	if (toggleCompact->isChecked())
		getWindow()->console().setRasterFontSize(RasterFontSizes::R8x8);
	else
		getWindow()->console().setRasterFontSize(RasterFontSizes::R8x12);
}

#pragma endregion
//=================================================================|
