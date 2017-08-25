/*=================================================================|
|* File:				HighscoreMenu.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resources.h"
#include "HighscoreMenu.h"
#include <PowerConsole/PowerConsoleSettings.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/Flags.h>
#include <AsciiTetris/Helpers/Helpers.h>
#include <AsciiTetris/Resources/SoundList.h>

using namespace PowerConsole;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace AsciiTetris;
using namespace AsciiTetris::Helpers;
using namespace AsciiTetris::Menus;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

HighscoreMenu::HighscoreMenu(GameMode gameMode, Highscore* highscoreToAdd)
	: Dialog(DialogModes::EntireWindow),

	gameMode(gameMode),
	addingHighscore(highscoreToAdd != nullptr),
	selection(0),

	// Visibility
	players(true),
	ais(true),
	
	// Scrolling
	scrollSize(0),
	thumbSize(0),
	viewportSize(0),
	objectSize(0),
	scrollRatio(0.0f),
	viewPosition(0),
	scrollPosition(0),
	scrollbarActive(false),
	
	// Textbox
	caretIndex(0),
	caretTimer(0) {

	eventInitialize().add(bindEvent(HighscoreMenu::onInitialize));
	eventLoad().add(bindEvent(HighscoreMenu::onLoad));
	eventTick().add(bindEvent(HighscoreMenu::onTick));
	eventPaint().add(bindEvent(HighscoreMenu::onPaint));
	eventKeyGlobal().add(bindEvent(HighscoreMenu::onKey));
	eventKeyTypedGlobal().add(bindEvent(HighscoreMenu::onKeyTyped));

	background = std::make_shared<AsciiImage>();
	background->loadResource(ASC_HIGHSCORE_MENU);

	auto& list = HighscoreList::getHighscores(gameMode);

	if (addingHighscore) {
		selection = HighscoreList::addHighscore(gameMode, *highscoreToAdd);

		// No need to enter a highscore name.
		if (highscoreToAdd->aiMode) {
			addingHighscore = false;
			HighscoreList::saveHighscores();
		}
	}

	resizeScrollBars((int)list.size() * 2 + 1);
	scrollToSelection();
}

void HighscoreMenu::onInitialize(ControlSPtr sender, EventArgs& e) {
	setSize(background->size());
}
void HighscoreMenu::onLoad(ControlSPtr sender, EventArgs& e) {
	console().centerWindow();
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

void HighscoreMenu::show(WindowSPtr owner, GameMode gameMode) {
	auto menu = makePrivateDialog(HighscoreMenu, gameMode, nullptr);
	owner->showDialog(menu);
}
void HighscoreMenu::show(WindowSPtr owner, GameMode gameMode, Highscore highscoreToAdd) {
	auto menu = makePrivateDialog(HighscoreMenu, gameMode, &highscoreToAdd);
	owner->showDialog(menu);
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void HighscoreMenu::setViewPosition(int position, bool relative) {
	viewPosition = GMath::clamp(position + (relative ? viewPosition : 0), 0, objectSize - viewportSize);
	if (viewPosition == objectSize - viewportSize)
		scrollPosition = (scrollSize - thumbSize) * 2;
	else
		scrollPosition = (int)(viewPosition * scrollRatio * 2.0f);
}
void HighscoreMenu::setScrollPosition(int position, bool relative) {
	scrollPosition = GMath::clamp(position, 0, (scrollSize - thumbSize) * 2);
	if (scrollPosition == 0)
		viewPosition = 0;
	else if (scrollPosition / 2 == scrollSize - thumbSize)
		viewPosition = objectSize - viewportSize;
	else
		viewPosition = (int)(scrollPosition / scrollRatio / 2);
}
void HighscoreMenu::resizeScrollBars(int newObjectSize) {
	viewportSize = LIST_HEIGHT;
	scrollSize = viewportSize;
	objectSize = newObjectSize;

	scrollbarActive = (viewportSize < objectSize);
	viewPosition = 0;
	scrollPosition = 0;

	if (scrollbarActive) {
		thumbSize = (int)((float)scrollSize * (float)viewportSize / (float)objectSize);
		thumbSize = GMath::max(thumbSize, 1);

		// Set the scrollbar to child size ratio.
		scrollRatio = (float)(scrollSize - thumbSize) / (float)(objectSize - viewportSize);
	}
	else {
		thumbSize = scrollSize;
	}
}
void HighscoreMenu::scrollToSelection() {
	int y = selection * 2 - viewPosition;
	int offset = 2;
	if (y - offset < 0) {
		setViewPosition(GMath::min(y - offset, 0), true);
	}
	else if (y + offset + 3 >= viewportSize) {
		setViewPosition(y + offset - viewportSize + 3, true);
	}
}
void HighscoreMenu::finishTyping() {
	if (addingHighscore) {
		addingHighscore = false;
		HighscoreList::saveHighscores();
	}
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void HighscoreMenu::onTick(ControlSPtr sender, TickEventArgs& e) {
	// Update the caret flashing if adding a highscore name
	if (addingHighscore) {
		caretTimer += e.timeSinceLastTick;
		caretTimer %= CARET_FLASH_TIME * 2;
	}
}
void HighscoreMenu::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char textColor = toColor(BLACK, DARK_GRAY);
	const unsigned char highlightColor = toColor(BLACK, CYAN);

	// Draw the background
	g->drawImage(Point2I::ZERO, *background);

	// Draw the game mode
	g->drawString(Point2I(9, 43), gameMode.name, Pixel(' ', textColor));

	// Draw the show/hide players/ais
	g->drawString(Point2I(28, 43), std::string(players ? "Hide" : "Show") + " Players: P", Pixel(' ', textColor));
	g->drawString(Point2I(49, 43), std::string(ais ? "Hide" : "Show") + " AIs: A", Pixel(' ', textColor));

	g->startRegion(Point2I(1, 7), Rectangle2I(67, 35));

	auto& list = HighscoreList::getHighscores(gameMode);

	// Draw highscores
	int offset = 0;
	for (int i = viewPosition / 2; i < viewPosition / 2 + viewportSize / 2 && i + offset < (int)list.size(); i++) {
		unsigned char color = (i == selection ? highlightColor : textColor);

		Highscore h = list[i + offset];
		while ((!h.aiMode && !players) || (h.aiMode && !ais)) {
			offset++;
			if (i + offset >= (int)list.size())
				break;
			h = list[i + offset];
		}
		if (i + offset >= (int)list.size())
			break;
		int y = 1 + i * 2 - viewPosition;

		// AI/Player symbol
		g->setPixel(Point2I(1, y), Pixel(h.aiMode ? 234 : 2, color));

		// Highscore name
		g->drawString(Point2I(3, y), h.name, Pixel(' ', color));

		// Score
		g->drawString(Point2I(21, y), (h.infScore ? "INFINITY" : largeIntToString(h.score, 11)), Pixel(' ', color), HorizontalAlignments::Right, 11);

		// Lines
		g->drawString(Point2I(35, y), largeIntToString(h.lines, 7), Pixel(' ', color), HorizontalAlignments::Right, 7);

		// Time
		g->drawString(Point2I(45, y), playTimeToString(h.playTime), Pixel(' ', color), HorizontalAlignments::Right, 8);

		// Date
		g->drawString(Point2I(56, y), h.date, Pixel(' ', color), HorizontalAlignments::Right, 8);

		if (i == selection) {
			if (addingHighscore && caretTimer < CARET_FLASH_TIME) {
				g->setColor(Point2I(3 + caretIndex, y), toColor(WHITE, BLACK));
			}
			g->setPixel(Point2I(0, y), Pixel(254, toColor(DARK_CYAN, CYAN)));
			g->setPixel(Point2I(64, y), Pixel(254, toColor(DARK_CYAN, CYAN)));
		}
	}

	// Draw the scroll bar thumb
	const int scrollX = 66;
	g->setPixel(Point2I(scrollX, scrollPosition / 2),
		Pixel(scrollPosition % 2 == 0 ? Characters::BLOCK_FULL : Characters::BLOCK_BOTTOM_HALF, toColor(BLACK, BLUE))
	);
	if (thumbSize > 2) {
		g->setPixel(Rectangle2I(scrollX, scrollPosition / 2 + 1, 1, thumbSize - 2 + (scrollPosition % 2)), Pixel(Characters::BLOCK_FULL, toColor(BLACK, BLUE)));
	}
	if (thumbSize > 1) {
		g->setPixel(Point2I(scrollX, scrollPosition / 2 + thumbSize - 1 + (scrollPosition % 2)),
			Pixel(scrollPosition % 2 == 0 ? Characters::BLOCK_FULL : Characters::BLOCK_TOP_HALF, toColor(BLACK, BLUE))
		);
	}

	g->endRegion();
}
void HighscoreMenu::onKey(ControlSPtr sender, KeyboardEventArgs& e) {
	if (e.isKeyPressed(Keys::Escape)) {
		if (addingHighscore && HighscoreList::getHighscore(gameMode, selection).name.empty()) {
			SoundList::playSound("Warning");
		}
		else {
			SoundList::playSound("MenuSelect");
			finishTyping();
			close();
		}
	}
	if (!addingHighscore) {
		if (e.isKeyPressed(Keys::Backspace) || e.isKeyPressed(Keys::Enter) || e.isKeyPressed(Keys::Space)) {
			SoundList::playSound("MenuSelect");
			close();
		}
		if (e.isKeyPressed(Keys::Down) && objectSize / 2 > 1) {
			SoundList::playSound("MenuMove");
			if (selection + 1 == objectSize / 2)
				selection = GMath::wrap(selection + 1, objectSize / 2);
			else
				selection = GMath::min(objectSize / 2 - 1, selection + (getFlag(e.modifiers, Modifiers::Shift) ? SHIFT_SPEED : 1));
			scrollToSelection();
		}
		else if (e.isKeyPressed(Keys::Up) && objectSize / 2 > 1) {
			SoundList::playSound("MenuMove");
			if (selection == 0)
				selection = GMath::wrap(selection - 1, objectSize / 2);
			else
				selection = GMath::max(0, selection - (getFlag(e.modifiers, Modifiers::Shift) ? SHIFT_SPEED : 1));
			scrollToSelection();
		}
		bool visibilityChanged = false;
		if (e.isKeyPressed(Keys::P)) {
			SoundList::playSound("MenuSelect");
			players = !players;
			visibilityChanged = true;
		}
		else if (e.isKeyPressed(Keys::A)) {
			SoundList::playSound("MenuSelect");
			ais = !ais;
			visibilityChanged = true;
		}
		if (visibilityChanged) {
			int count = 0;
			if (players || ais) {
				auto& list = HighscoreList::getHighscores(gameMode);
				for (int i = 0; i < (int)list.size(); i++) {
					if ((!list[i].aiMode && players) || (list[i].aiMode && ais))
						count++;
				}
			}
			resizeScrollBars(1 + count * 2);
			selection = 0;
		}
	}
	if (addingHighscore && e.isKeyPressed(Keys::Enter)) {
		std::string text = HighscoreList::getHighscore(gameMode, selection).name;
		if (text.empty()) {
			SoundList::playSound("Warning");
		}
		else {
			SoundList::playSound("MenuSelect");
			finishTyping();
		}
	}
}
void HighscoreMenu::onKeyTyped(ControlSPtr sender, KeyTypedEventArgs& e) {
	if (!addingHighscore) {
		if (e.inputKey == Keys::Down && !keyboard().isKeyPressed(Keys::Down) && selection < objectSize / 2 - 1) {
			SoundList::playSound("MenuMove");
			selection = GMath::min(objectSize / 2 - 1, selection + (getFlag(e.modifiers, Modifiers::Shift) ? SHIFT_SPEED : 1));
			//selection++;
			scrollToSelection();
		}
		else if (e.inputKey == Keys::Up && !keyboard().isKeyPressed(Keys::Up) && selection > 0) {
			SoundList::playSound("MenuMove");
			selection = GMath::max(0, selection - (getFlag(e.modifiers, Modifiers::Shift) ? SHIFT_SPEED : 1));
			//selection--;
			scrollToSelection();
		}
	}
	else {
		std::string text = HighscoreList::getHighscore(gameMode, selection).name;
		if (e.inputKey == Keys::Backspace) {
			if (caretIndex > 0) {
				SoundList::playSound("MenuMove");
				text.erase(caretIndex - 1, 1);
				caretIndex--;
				caretTimer = 0;
			}
		}
		else if (e.inputKey == Keys::Left) {
			if (caretIndex > 0) {
				SoundList::playSound("MenuMove");
				caretIndex--;
				caretTimer = 0;
			}
		}
		else if (e.inputKey == Keys::Right) {
			if (caretIndex < (int)text.length()) {
				SoundList::playSound("MenuMove");
				caretIndex++;
				caretTimer = 0;
			}
		}
		else if (e.inputKey != Keys::Tab && e.inputChar >= 0x20 && (int)text.length() < MAX_NAME_LENGTH) {
			if (!getAnyFlag(e.modifiers, Modifiers::Alt | Modifiers::Ctrl | Modifiers::Win, Modifiers::None)) {
				SoundList::playSound("MenuMove");
				text.insert(caretIndex, std::string(1, e.inputChar));
				caretIndex++;
				caretTimer = 0;
			}
		}
		HighscoreList::updateHighscoreName(gameMode, selection, text);
	}
}

#pragma endregion
//=================================================================|
