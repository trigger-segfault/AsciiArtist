/*=================================================================|
|* File:				SelectionList.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "SelectionList.h"
#include <PowerConsole/Geometry/GMath.h>
#include <AsciiTetris/Resources/SoundList.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;

using namespace AsciiTetris::UI;
using namespace AsciiTetris::Resources;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

SelectionList::SelectionList()
 :	selection(0),
	timer(0),
	
	_eventItemSelected(EventDirections::FirstInFirstCalled) {

	eventInitialize().add(bindEvent(SelectionList::onInitialize));
	eventPaint().add(bindEvent(SelectionList::onPaint));
	eventTick().add(bindEvent(SelectionList::onTick));
	eventKeyGlobal().add(bindEvent(SelectionList::onKey));
	eventKeyTypedGlobal().add(bindEvent(SelectionList::onKeyTyped));
}
void SelectionList::onInitialize(ControlSPtr sender, EventArgs& e) {
	setSize(Point2I(SIZE_AUTO));
	setHorizontalAlignment(HorizontalAlignments::Stretch);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

int SelectionList::getSelectedIndex() const {
	return selection;
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void SelectionList::addItem(const std::string& text, unsigned char color) {
	items.push_back(SelectionItem(text, color));
	updateAutoSize();
}
void SelectionList::renameItem(int index, const std::string& newText) {
	items[index].text = newText;
}

PcGeo::Point2I SelectionList::autoSizeBehavior() const {
	return Point2I(0, std::max((int)items.size() * 3 - 2, 0));
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<EventArgs>& SelectionList::eventItemSelected() {
	return _eventItemSelected;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void SelectionList::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char textColor = toColor(BLACK, DARK_GRAY);

	for (int i = 0; i < (int)items.size(); i++) {
		unsigned char color = textColor;
		if (i == selection) {
			// Change selection color
			color = setBColor(items[i].color, toBColor(BLACK));

			// Draw selection blocks
			if (timer < ANIMATION_LENGTH) {
				int width = (int)items[i].text.length();
				int left = (getActualSize().x - width) / 2 - 2;
				int right = left + (int)items[i].text.length() + 3;

				g->setPixel(Point2I(left, i * 3), Pixel(254, items[i].color));
				g->setPixel(Point2I(right, i * 3), Pixel(254, items[i].color));
			}
		}
		g->drawString(Point2I(0, i * 3), items[i].text, Pixel(' ', color), HorizontalAlignments::Center, getActualSize().x);
	}
}
void SelectionList::onTick(ControlSPtr sender, TickEventArgs& e) {
	timer = GMath::wrap(timer + 1, ANIMATION_LENGTH * 2);
}
void SelectionList::onKey(ControlSPtr sender, KeyboardEventArgs& e) {
	if (e.isKeyPressed(Keys::Enter) || e.isKeyPressed(Keys::Space)) {
		SoundList::playSound("MenuSelect");
		_eventItemSelected(shared_from_this());
		timer = 0;
	}
}
void SelectionList::onKeyTyped(ControlSPtr sender, KeyTypedEventArgs& e) {
	if (e.inputKey == Keys::Down) {
		SoundList::playSound("MenuMove");
		selection = GMath::wrap(selection + 1, (int)items.size());
		timer = 0;
	}
	else if (e.inputKey == Keys::Up) {
		SoundList::playSound("MenuMove");
		selection = GMath::wrap(selection - 1, (int)items.size());
		timer = 0;
	}
}

#pragma endregion
//=================================================================|
