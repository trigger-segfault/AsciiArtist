/*=================================================================|
|* File:				GroupBox.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "GroupBox.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

GroupBox::GroupBox() 
 :	// Settings
	header("GroupBox") {

	eventPaint().add(bindEvent(GroupBox::onPaint));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string GroupBox::getHeader() const {
	return header;
}
void GroupBox::setHeader(const std::string& header) {
	this->header = header;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void GroupBox::updateChildLayout(ControlSPtr control) {
	control->setContainerLayout(Rectangle2I(1, 1, getActualSize() - 2), getLocalVisibleArea().intersect(Rectangle2I(1, 1, getActualSize() - 2)));
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void GroupBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);
	const unsigned char headerColor = toColor(BLACK, GRAY);

	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', frameColor), false);
	if ((int)header.length() > getActualSize().x - 3)
		g->drawString(Point2I(2, 0), header.substr(0, getActualSize().x - 3), Pixel(' ', headerColor));
	else
		g->drawString(Point2I(2, 0), header, Pixel(' ', headerColor));
}

#pragma endregion
//=================================================================|
