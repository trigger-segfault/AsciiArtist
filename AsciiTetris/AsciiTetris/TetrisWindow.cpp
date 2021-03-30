/*=================================================================|
|* File:				TetrisWindow.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resources.h"
#include "TetrisWindow.h"
#include <PowerConsole/PowerConsoleSettings.h>
#include <AsciiTetris/Resources/TetrominoList.h>
#include <AsciiTetris/Resources/SoundList.h>
#include <AsciiTetris/Menus/HighscoreMenu.h>
#include <AsciiTetris/Menus/ControlsMenu.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole;
using namespace AsciiTetris;
using namespace AsciiTetris::Menus;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

TetrisWindow::TetrisWindow() {
	srand((unsigned int)time(nullptr));
	TetrominoList::createTetrominos();
	TetrominoList::createPentominos();
	TetrominoList::loadCustom();
	GameModeList::loadGameModes();
	HighscoreList::loadHighscores();
	AIList::loadAIs();
	SoundList::loadAudio();

	gameMode = GameModeList::getGameMode(0);
	gameModeIndex = 0;
	ai = AIList::getAI(0);
	aiIndex = -1;
	blockSize = Point2I(2, 2);  // At the moment this is hardcoded.

	eventInitialize().add(bindEvent(TetrisWindow::onInitialize));
	eventLoad().add(bindEvent(TetrisWindow::onLoad));
	eventTick().add(bindEvent(TetrisWindow::onTick));
	eventGlobalTick().add(bindEvent(TetrisWindow::onGlobalTick));
	eventPaint().add(bindEvent(TetrisWindow::onPaint));

	PowerConsoleSettings::setDesiredFrameRate(FRAME_RATE);
	PowerConsoleSettings::setDebugFrameRateColor(toColor(BLACK, DARK_GRAY));

	background = std::make_shared<AsciiImage>();
	background->loadResource(ASC_START_MENU);
}

void TetrisWindow::onInitialize(ControlSPtr sender, EventArgs& e) {
	console().setRasterFontSize(RasterFontSizes::R8x8);
	console().setPaletteColor(BLUE, ColorB(44, 44, 255));
	console().setPaletteColor(DARK_BLUE, ColorB(0, 0, 160));
	console().setPaletteColor(WHITE, ColorB(255, 128, 0));
	console().setPaletteColor(GRAY, ColorB(128, 64, 0));
	console().setPaletteColor(DARK_GRAY, ColorB(255, 255, 255));
	setSize(background->size());

	selectionList = makeControl(SelectionList);
	selectionList->setMargin(Thickness(0, 26, 0, 0));
	selectionList->addItem("Start Game", GREEN);
	selectionList->addItem("AI: Player", MAGENTA);
	selectionList->addItem("Mode: " + gameMode.name, CYAN);
	selectionList->addItem("Highscores", RED);
	selectionList->addItem("Controls", BLUE);
	selectionList->addItem("Reload", WHITE);
	selectionList->addItem("Exit Game", YELLOW);
	selectionList->eventItemSelected().add(bindEvent(TetrisWindow::onItemSelected));
	children().add(selectionList);
}
void TetrisWindow::onLoad(ControlSPtr sender, EventArgs& e) {
	console().centerWindow();
	//SoundList::playMusic();
	/*HRSRC hRsrc = FindResourceA(NULL, ("#" + std::to_string(MP3_MUSIC)).c_str(), "MP3");
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	LPVOID data = LockResource(hGlobal);
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	mp3.openFromMemory(data, dwSize);
	mp3.openFromFile(R"(C:\Users\Onii-chan\Desktop\Digital Love - Sabrepulse .mp3)");
	mp3.setVolume(45.0f);
	mp3.play();
	mp3.setLoop(true);
	mp3.setPitch(2.0f);*/
	SoundList::playMusic();
}

#pragma endregion
//=========== RUNNING ============
#pragma region Running

void TetrisWindow::startGame() {
	GameInstanceResults result;
	do {
		// Clear the place counts
		for (auto tetromino : TetrominoList::getTetrominos()) {
			tetromino->clearPlaceCount();
		}
		for (auto pentomino : TetrominoList::getPentominos()) {
			pentomino->clearPlaceCount();
		}
		for (auto custom : TetrominoList::getCustoms()) {
			custom->clearPlaceCount();
		}
		Highscore highscore;
		if (aiIndex == -1)
			result = GameInstance::runPlayerGame(getWindow(), highscore, gameMode, blockSize);
		else
			result = GameInstance::runAIGame(getWindow(), highscore, gameMode, ai, blockSize);

		if (result == GameInstanceResults::GameOver) {
			HighscoreMenu::show(getWindow(), gameMode, highscore);
			console().centerWindow();
		}

	} while (result == GameInstanceResults::Restart);
	console().centerWindow();
}
void TetrisWindow::reload() {
	TetrominoList::loadCustom();
	GameModeList::loadGameModes();
	HighscoreList::loadHighscores();
	AIList::loadAIs();

	gameMode = GameModeList::getGameMode(0);
	gameModeIndex = 0;
	ai = AIList::getAI(0);
	aiIndex = -1;

	selectionList->renameItem(1, "AI: Player");
	selectionList->renameItem(2, "Mode: " + gameMode.name);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void TetrisWindow::onTick(ControlSPtr sender, TickEventArgs& e) {
	
}
void TetrisWindow::onGlobalTick(ControlSPtr sender, TickEventArgs& e) {
	// Update music/sound manager here
	if (globalKeyboard().isHotkeyPressed(Hotkey(Keys::M, Modifiers::Alt))) {
		SoundList::toggleMusicMute();
	}
	if (globalKeyboard().isHotkeyPressed(Hotkey(Keys::S, Modifiers::Alt))) {
		SoundList::toggleSoundMute();
	}
}
void TetrisWindow::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	// Draw the background
	g->drawImage(Point2I::ZERO, *background);
}
void TetrisWindow::onItemSelected(ControlSPtr sender, EventArgs& e) {
	switch (selectionList->getSelectedIndex()) {
	case 0: // Start Game
		startGame();
		break;
	case 1: // AI
		aiIndex++;
		if (aiIndex >= AIList::getAICount())
			aiIndex = -1;
		else
			ai = AIList::getAI(aiIndex);
		selectionList->renameItem(1, "AI: " + (aiIndex == -1 ? "Player" : ai.name));
		break;
	case 2: // Game Mode
		gameModeIndex++;
		if (gameModeIndex >= GameModeList::getGameModeCount())
			gameModeIndex = 0;
		gameMode = GameModeList::getGameMode(gameModeIndex);
		selectionList->renameItem(2, "Mode: " + gameMode.name);
		break;
	case 3: // Highscores
		HighscoreMenu::show(getWindow(), gameMode);
		console().centerWindow();
		break;
	case 4: // Controls
		ControlsMenu::show(getWindow());
		break;
	case 5: // Reload
		reload();
		break;
	case 6: // Exit
		close();
		break;
	}
}


#pragma endregion
//=================================================================|
