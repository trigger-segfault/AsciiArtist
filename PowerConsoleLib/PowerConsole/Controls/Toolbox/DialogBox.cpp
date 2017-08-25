/*=================================================================|
|* File:				DialogBox.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "DialogBox.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

#undef max
#undef min

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

DialogBox::DialogBox(const std::string& title, DialogBoxButtons buttons, DialogBoxIcons icon)
	: Dialog(DialogModes::InnerWindow),

	// Setup
	title(title),
	buttonSetup(buttons),
	iconType(icon),

	// Controls
	labelTitle(nullptr),
	buttons{ nullptr },
	imageIcon(nullptr) {

	eventInitialize().add(bindEvent(DialogBox::onInitialize));
	eventPaint().add(bindEvent(DialogBox::onPaint));
	eventKeyGlobal().add(bindEvent(DialogBox::onKeyGlobal));
}
void DialogBox::onInitialize(ControlSPtr sender, EventArgs& e) {
	labelTitle = makeControl(Label);
	labelTitle->setText(title);
	labelTitle->setMargin(Thickness(2, 1, 0, 0));
	children().add(labelTitle);

	for (int i = 0; i < numButtons(); i++) {
		buttons[i] = makeControl(Button);
		buttons[i]->setSize(Point2I(8, 1));
		buttons[i]->setHorizontalAlignment(HorizontalAlignments::Right);
		buttons[i]->setVerticalAlignment(VerticalAlignments::Bottom);
		buttons[i]->setMargin(Thickness(0, 0, 2 + i * 10, 1));
		if ((i == 0 && buttonSetup == DialogBoxButtons::OK) ||
			(i == 1 && buttonSetup == DialogBoxButtons::OKCancel)) {
			buttons[i]->setText("OK");
		}
		else if (i == 0 && (buttonSetup == DialogBoxButtons::OKCancel ||
			buttonSetup == DialogBoxButtons::YesNoCancel)) {
			buttons[i]->setText("Cancel");
		}
		else if ((i == 0 && buttonSetup == DialogBoxButtons::YesNo) ||
			(i == 1 && buttonSetup == DialogBoxButtons::YesNoCancel)) {
			buttons[i]->setText("No");
		}
		else {
			buttons[i]->setText("Yes");
		}
		buttons[i]->eventClick().add(bindEvent(DialogBox::onButtonClick));
		children().add(buttons[i]);
	}

	if (iconType != DialogBoxIcons::None) {
		imageIcon = makeControl(Image);
		imageIcon->setMargin(Thickness(2, 5, 0, 0));
		AsciiImage ascIcon = AsciiImage(Point2I(3, 1), ImageFormats::Basic);
		auto g = ascIcon.createGraphics();
		g->setChar(Point2I(0, 0), Characters::BLOCK_RIGHT_HALF);
		g->setChar(Point2I(2, 0), Characters::BLOCK_LEFT_HALF);
		switch (iconType) {
		case DialogBoxIcons::Information:
			g->setColor(Rectangle2I(0, 0, 3, 1), toColor(BLACK, DARK_BLUE));
			g->setPixel(Point2I(1, 0), Pixel('i', toColor(BLUE, WHITE)));
			break;
		case DialogBoxIcons::Question:
			g->setColor(Rectangle2I(0, 0, 3, 1), toColor(BLACK, DARK_BLUE));
			g->setPixel(Point2I(1, 0), Pixel('?', toColor(BLUE, WHITE)));
			break;
		case DialogBoxIcons::Warning:
			g->setColor(Rectangle2I(0, 0, 3, 1), toColor(BLACK, DARK_YELLOW));
			g->setPixel(Point2I(1, 0), Pixel('!', toColor(YELLOW, BLACK)));
			break;
		case DialogBoxIcons::Error:
			g->setColor(Rectangle2I(0, 0, 3, 1), toColor(BLACK, DARK_RED));
			g->setPixel(Point2I(1, 0), Pixel('X', toColor(RED, WHITE)));
			break;
		}
		imageIcon->setImage(std::move(ascIcon));
		children().add(imageIcon);
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

DialogBoxButtons DialogBox::getButtonSetup() const {
	return buttonSetup;
}
DialogBoxIcons DialogBox::getIconType() const {
	return iconType;
}
bool DialogBox::hasIcon() const {
	return iconType != DialogBoxIcons::None;
}
std::string DialogBox::getTitle() const {
	return title;
}
void DialogBox::setTitle(const std::string& title) {
	this->title = title;
	if (labelTitle != nullptr)
		labelTitle->setText(title);
}
DialogBoxResults DialogBox::getDialogBoxResult() const {
	return dialogBoxResult;
}
void DialogBox::setDialogBoxResult(DialogBoxResults result) {
	dialogBoxResult = result;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

int DialogBox::numButtons() const {
	switch (buttonSetup) {
	case DialogBoxButtons::OK: return 1;
	case DialogBoxButtons::OKCancel:
	case DialogBoxButtons::YesNo: return 2;
	case DialogBoxButtons::YesNoCancel: return 3;
	default: return 0;
	}
}
DialogBoxResults DialogBox::getDialogBoxResultFromButton(ControlSPtr button) const {
	for (int i = 0; i < 3; i++) {
		if (buttons[i] == button) {
			if ((i == 0 && buttonSetup == DialogBoxButtons::OK) ||
				(i == 1 && buttonSetup == DialogBoxButtons::OKCancel)) {
				return DialogBoxResults::OK;
			}
			else if (i == 0 && (buttonSetup == DialogBoxButtons::OKCancel ||
				buttonSetup == DialogBoxButtons::YesNoCancel)) {
				return DialogBoxResults::Cancel;
			}
			else if ((i == 0 && buttonSetup == DialogBoxButtons::YesNo) ||
				(i == 1 && buttonSetup == DialogBoxButtons::YesNoCancel)) {
				return DialogBoxResults::No;
			}
			else {
				return DialogBoxResults::Yes;
			}
		}
	}
	return DialogBoxResults::Cancel;
}
Point2I DialogBox::getMinSize() const {
	return Point2I(numButtons() * 10 + 4, 12);
}
bool DialogBox::buttonBehavior(DialogBoxResults buttonPressed) {
	return true;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void DialogBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);
	// Button borders
	Point2I buttonSize = Point2I(10, 3);
	for (int i = 0; i < numButtons(); i++) {
		g->drawBorder(Rectangle2I(getActualSize() - Point2I(1 + buttonSize.x * (i + 1), buttonSize.y), buttonSize), Pixel(' ', frameColor), false, BORDER_THICK);
	}

	// Window border
	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', frameColor), true);
	// Title bar bottom border
	g->drawBorder(Rectangle2I(0, 2, getActualSize().x, 1), Pixel(' ', frameColor), true);
	// Button/Message separator border
	g->drawBorder(Rectangle2I(0, getActualSize().y - 4, getActualSize().x, 1), Pixel(' ', frameColor), false);

	// Icon border
	if (iconType != DialogBoxIcons::None) {
		g->drawBorder(Rectangle2I(imageIcon->getLocalPosition() - 1, Point2I(5, 3)), Pixel(' ', frameColor), false, BORDER_NONE);
	}
}
void DialogBox::onButtonClick(ControlSPtr sender, MouseEventArgs& e) {
	auto result = getDialogBoxResultFromButton(sender);
	if (buttonBehavior(result)) {
		dialogBoxResult = result;
		close();
	}
}
void DialogBox::onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e) {
	if (e.isKeyPressed(Keys::Enter)) {
		auto result = getDialogBoxResultFromButton(buttons[numButtons() - 1]);
		if (buttonBehavior(result)) {
			dialogBoxResult = result;
			close();
		}
	}
	else if (e.isKeyPressed(Keys::Escape)) {
		auto result = getDialogBoxResultFromButton(buttons[0]);
		if (buttonBehavior(result)) {
			dialogBoxResult = result;
			close();
		}
	}
}

#pragma endregion
//=================================================================|
