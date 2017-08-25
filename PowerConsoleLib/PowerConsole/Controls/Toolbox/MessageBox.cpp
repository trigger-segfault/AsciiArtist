/*=================================================================|
|* File:				MessageBox.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "MessageBox.h"
#include <PowerConsole/Drawing/ConsoleColors.h>

#undef max
#undef min

using namespace PowerConsole;
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

MessageBox::MessageBox(const std::string& message, const std::string& title, DialogBoxButtons buttons, DialogBoxIcons icon,
	int maxWidth, int minWidth, int minLines)
 :	DialogBox(title, buttons, icon),
	
	// Setup
	message(message),
	wordWrap(),
	
	// Controls
	panelMessage(nullptr) {

	eventInitialize().add(bindEvent(MessageBox::onInitialize));

	int offset = (hasIcon() ? 9 : 4);
	minWidth = std::max(minWidth, getMinSize().x);
	wordWrap = WordWrapper::wrap(message, minLines, minWidth - offset, maxWidth - offset);

	setSizeSafe(Point2I(std::max(minWidth, wordWrap.width + offset), std::max(getMinSize().y, wordWrap.lines + 9)));
}
void MessageBox::onInitialize(ControlSPtr sender, EventArgs& e) {
	panelMessage = makeControl(Panel);
	panelMessage->setMargin(Thickness(hasIcon() ? 7 : 2, 4, 2, 5));
	panelMessage->setHorizontalAlignment(HorizontalAlignments::Stretch);
	panelMessage->setVerticalAlignment(VerticalAlignments::Center);
	panelMessage->setSize(Point2I(wordWrap.width, wordWrap.lines));
	panelMessage->eventPaint().add(bindEvent(MessageBox::onMessagePaint));
	children().add(panelMessage);
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

DialogBoxResults MessageBox::show(WindowSPtr owner, const std::string& message,
	SystemSounds sound, int maxWidth, int minWidth, int minLines) {
	return show(owner, message, "", DialogBoxButtons::OK, DialogBoxIcons::None, sound, maxWidth, minWidth, minLines);
}
DialogBoxResults MessageBox::show(WindowSPtr owner, const std::string& message, const std::string& title,
	SystemSounds sound, int maxWidth, int minWidth, int minLines) {
	return show(owner, message, title, DialogBoxButtons::OK, DialogBoxIcons::None, sound, maxWidth, minWidth, minLines);
}
DialogBoxResults MessageBox::show(WindowSPtr owner, const std::string& message, const std::string& title, DialogBoxButtons buttons,
	SystemSounds sound, int maxWidth, int minWidth, int minLines) {
	return show(owner, message, title, buttons, DialogBoxIcons::None, sound, maxWidth, minWidth, minLines);
}
DialogBoxResults MessageBox::show(WindowSPtr owner, const std::string& message, const std::string& title, DialogBoxIcons icon,
	SystemSounds sound, int maxWidth, int minWidth, int minLines) {
	return show(owner, message, title, DialogBoxButtons::OK, icon, sound, maxWidth, minWidth, minLines);
}
DialogBoxResults MessageBox::show(WindowSPtr owner, const std::string& message, const std::string& title, DialogBoxButtons buttons, DialogBoxIcons icon,
	SystemSounds sound, int maxWidth, int minWidth, int minLines) {
	if (sound != SystemSounds::None)
		owner->console().playSystemSound(sound);
	MessageBoxSPtr messageBox = makePrivateDialog(MessageBox, message, title, buttons, icon, maxWidth, minWidth, minLines);
	owner->showDialog(messageBox);
	return messageBox->getDialogBoxResult();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void MessageBox::onMessagePaint(ControlSPtr sender, PaintEventArgs& e) {
	const unsigned char color = toColor(BLACK, GRAY);
	for (int i = 0; i < wordWrap.lines; i++) {
		e.graphics->drawString(Point2I(0, i), message.substr(wordWrap.lineStarts[i], wordWrap.lineLengths[i]), Pixel(' ', color));
	}
}

#pragma endregion
//=================================================================|
