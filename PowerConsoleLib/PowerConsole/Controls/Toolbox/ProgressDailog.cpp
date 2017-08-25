/*=================================================================|
|* File:				ProgressDailog.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "ProgressDailog.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/IO/System.h>
#include <PowerConsole/Input/Keys.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

ProgressDialog::ProgressDialog(bool showActionMessage, bool allowCancel, int width, CallFunction callFunction)
 :	Dialog(DialogModes::InnerWindow),
	
	percentage(0),
	showActionMessage(showActionMessage),
	allowCancel(allowCancel),
	callFunction(callFunction) {

	eventInitialize().add(bindEvent(ProgressDialog::onInitialize));
	eventLoad().add(bindEvent(ProgressDialog::onLoad));
	eventPaint().add(bindEvent(ProgressDialog::onPaint));

	setSizeSafe(Point2I(width, 9 + (showActionMessage ? 1 : 0) + (allowCancel ? 2 : 0)));
}
void ProgressDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	labelTitle = makeControl(Label);
	labelTitle->setText("Progress Window");
	labelTitle->setMargin(Thickness(2, 1, 0, 0));
	children().add(labelTitle);

	labelPercentage = makeControl(Label);
	labelPercentage->setText("0%");
	labelPercentage->setTextAlignment(HorizontalAlignments::Right);
	labelPercentage->setHorizontalAlignment(HorizontalAlignments::Right);
	labelPercentage->setMargin(Thickness(0, 5, 2, 0));
	children().add(labelPercentage);

	int offset = 8;
	if (showActionMessage) {
		labelActionMessage = makeControl(Label);
		labelActionMessage->setText("Performing action...");
		labelActionMessage->setMargin(Thickness(3, 7, 2, 0));
		children().add(labelActionMessage);
		offset += 1;
	}

	if (allowCancel) {
		labelPressEscape = makeControl(Label);
		labelPressEscape->setText("Press <escape> to cancel.");
		labelPressEscape->setMargin(Thickness(3, offset, 2, 0));
		children().add(labelPressEscape);
	}
}
void ProgressDialog::onLoad(ControlSPtr sender, EventArgs& e) {
	redraw();
	if (callFunction != nullptr)
		callFunction(std::static_pointer_cast<ProgressDialog>(shared_from_this()));
	close();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

void ProgressDialog::show(WindowSPtr owner, const std::string& title, bool showActionMessage, bool allowCancel, CallFunction callFunction) {
	show(owner, title, showActionMessage, allowCancel, DEFAULT_WIDTH, callFunction);
}
void ProgressDialog::show(WindowSPtr owner, const std::string& title, bool showActionMessage, bool allowCancel, int width, CallFunction callFunction) {
	auto dialog = makePrivateDialog(ProgressDialog, showActionMessage, allowCancel, width, callFunction);
	dialog->setTitle(title);
	owner->showDialog(dialog);
}

#pragma endregion
//========== PROPERTIES ==========

std::string ProgressDialog::getTitle() const {
	return labelTitle->getText();
}
void ProgressDialog::setTitle(const std::string& title) {
	labelTitle->setText(title);
}
int ProgressDialog::getPercentage() const {
	return percentage;
}
void ProgressDialog::setPercentage(int percentage) {
	this->percentage = GMath::clamp(percentage, 0, 100);
	labelPercentage->setText(std::to_string(percentage) + "%");
}
std::string ProgressDialog::getActionMessage() const {
	return labelActionMessage->getText();
}
void ProgressDialog::setActionMessage(const std::string& message) {
	labelActionMessage->setText(message);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void ProgressDialog::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Window border
	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', frameColor), true);
	// Title bar bottom border
	g->drawBorder(Rectangle2I(0, 2, getActualSize().x, 1), Pixel(' ', frameColor), true);
	// Draw progress border
	g->drawBorder(Rectangle2I(2, 4, getActualSize().x - 9, 3), Pixel(' ', frameColor), false);

	// Draw progress bar
	int barWidth = getActualSize().x - 11;
	int progressWidth = barWidth * percentage / 100;
	//int progressWidth = (int)(((float)barWidth + 0.5f) * percentage / 100.0f);
	int emptyWidth = barWidth - progressWidth;
	g->setPixel(Rectangle2I(3, 5, progressWidth, 1), Pixel(Characters::GRADIENT_MEDIUM, toColor(DARK_GREEN, GREEN)));
	g->setPixel(Rectangle2I(3 + progressWidth, 5, emptyWidth, 1), Pixel(Characters::GRADIENT_MEDIUM, toColor(BLACK, DARK_GREEN)));
}
bool ProgressDialog::update() {
	bool canceled = false;

	// Check for escape press
	if (allowCancel) {
		HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD inRec = INPUT_RECORD();

		// Read all of the events that have occured
		DWORD numRead = 0;
		GetNumberOfConsoleInputEvents(hIn, &numRead);
		for (int i = 0; i < (int)numRead; i++) {
			ReadConsoleInputA(hIn, &inRec, 1, &numRead);
			if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown == TRUE &&
				inRec.Event.KeyEvent.wVirtualKeyCode == (WORD)Keys::Escape) {
				canceled = true;
				break;
			}
		}
	}
	if (canceled && showActionMessage) {
		labelActionMessage->setText("Canceled");
	}

	redraw();
	
	if (canceled) {
		System::sleep(500);
		close();
	}
	return canceled;
}

#pragma endregion
//=================================================================|
