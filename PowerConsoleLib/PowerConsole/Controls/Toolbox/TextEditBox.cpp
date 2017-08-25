/*=================================================================|
|* File:				TextEditBox.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "TextEditBox.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

#undef max
#undef min

using namespace PowerConsole::Helpers;
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

TextEditBox::TextEditBox(const std::string& title, DialogBoxButtons buttons, int width)
 :	DialogBox(title, buttons, DialogBoxIcons::None),

	// Controls
	textBox(nullptr) {

	eventInitialize().add(bindEvent(TextEditBox::onInitialize));
	eventPaint().add(bindEvent(TextEditBox::onPaint));

	setSizeSafe(Point2I(std::max(width, getMinSize().x), getMinSize().y));
}
void TextEditBox::onInitialize(ControlSPtr sender, EventArgs& e) {
	textBox = makeControl(TextBox);
	textBox->setSize(Point2I(SIZE_AUTO));
	textBox->setHorizontalAlignment(HorizontalAlignments::Stretch);
	textBox->setMargin(Thickness(3, 5, 3, 0));
	textBox->setSelectAllOnFocus(true);
	children().add(textBox);
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

DialogBoxResults TextEditBox::show(WindowSPtr owner, std::string& textResult, int width) {
	return show(owner, textResult, "", DialogBoxButtons::OK, width);
}
DialogBoxResults TextEditBox::show(WindowSPtr owner, std::string& textResult, const std::string& title, int width) {
	return show(owner, textResult, title, DialogBoxButtons::OK, width);
}
DialogBoxResults TextEditBox::show(WindowSPtr owner, std::string& textResult, const std::string& title, DialogBoxButtons buttons, int width) {
	TextEditBoxSPtr messageBox = makePrivateDialog(TextEditBox, title, buttons, width);
	messageBox->textBox->setText(textResult);
	messageBox->textBox->focus();
	owner->showDialog(messageBox);
	textResult = messageBox->textBox->getText();
	return messageBox->getDialogBoxResult();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void TextEditBox::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw the text box border
	g->drawBorder(Rectangle2I(textBox->getLocalPosition() - 1, textBox->getActualSize() + 2), Pixel(' ', frameColor), false);
}

#pragma endregion
//=================================================================|
