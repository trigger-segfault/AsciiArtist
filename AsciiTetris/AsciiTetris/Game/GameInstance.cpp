/*=================================================================|
|* File:				GameInstance.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "GameInstance.h"
#include <AsciiTetris/Helpers/Helpers.h>
#include <AsciiTetris/Menus/HighscoreMenu.h>
#include <AsciiTetris/Resources/SoundList.h>
#include <AsciiTetris/Game/Controllers/AIController.h>
#include <AsciiTetris/Game/Controllers/PlayerController.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/PowerConsoleSettings.h>
#include <random>
#include <ctime>

using namespace PowerConsole;
using namespace AsciiTetris;
using namespace AsciiTetris::Helpers;
using namespace AsciiTetris::Resources;
using namespace AsciiTetris::Game::Controllers;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const int GameInstance::LINE_SCORE_CHART[] = { 100, 300, 500, 800, 1200, 1800 };

#pragma endregion
//=========== RUNNING ============
#pragma region Running

GameInstanceResults GameInstance::runPlayerGame(WindowSPtr owner, Highscore& highscoreResult, GameMode gameMode, Point2I blockSize) {
	auto game = makePrivateDialog(GameInstance, gameMode, nullptr, blockSize);
	owner->showDialog(game);
	highscoreResult = game->highscore;
	return game->result;
}
GameInstanceResults GameInstance::runAIGame(WindowSPtr owner, Highscore& highscoreResult, GameMode gameMode, AISettings ai, Point2I blockSize) {
	auto game = makePrivateDialog(GameInstance, gameMode, &ai, blockSize);
	owner->showDialog(game);
	highscoreResult = game->highscore;
	return game->result;
}

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

GameInstance::GameInstance(GameMode gameMode, AISettings* ai, Point2I blockSize)
 : Dialog(DialogModes::EntireWindow),

	gameMode(gameMode) {
	result = GameInstanceResults::GameOver;

	running = true;
	highscore.aiMode = ai != nullptr;
	sfxEnabled = (ai != nullptr ? ai->sfxEnabled : true);
	gameOverInput = false;

	linesLeft = (gameMode.levelProgression ? LINES_PER_LEVEL : 0);
	combo = 0;
	linesCleared = 0;

	calculateDropSpeed();

	//well = std::make_shared<TetrisWell>(gameMode.wellSize, Point2I(2, 2), sfxEnabled);
	well = std::make_shared<TetrisWell>(gameMode.wellSize, blockSize, sfxEnabled);
	if (highscore.aiMode) {
		highscore.name = ai->name;
		controller = std::make_shared<AIController>(well, *ai);
	}
	else {
		controller = std::make_shared<PlayerController>(well);
	}

	eventInitialize().add(bindEvent(GameInstance::onInitialize));
	eventLoad().add(bindEvent(GameInstance::onLoad));
	eventTick().add(bindEvent(GameInstance::onTick));
	eventGlobalTick().add(bindEvent(GameInstance::onGlobalTick));
	eventPaint().add(bindEvent(GameInstance::onPaint));
}
void GameInstance::onInitialize(ControlSPtr sender, EventArgs& e) {
	int extraWidth = 26 + std::max(10, well->getBlockSize().x * 5);
	int extraHeight = 36;
	if (gameMode.useTetrominos)
		extraHeight += std::min(5, (gameMode.usePentominos ? 3 : 0) + (gameMode.useCustom ? 3 : 0));
	else if (gameMode.usePentominos)
		extraHeight = 43 + (gameMode.useCustom ? 3 : 0);

	setSize(GMath::max(Point2I(0, extraHeight), well->getDimensions() * well->getBlockSize()) + Point2I(extraWidth, 6));

	generateNextTetromino(false);
	generateNextTetromino(false);

	textSlider = makeControl(TextSlider);
	textSlider->start("READY??");
	textSlider->eventAnimationFinished().add(bindEvent(GameInstance::onTextSliderAnimationFinished));
	children().add(textSlider);
}
void GameInstance::onLoad(ControlSPtr sender, EventArgs& e) {
	console().centerWindow();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

long long GameInstance::getScore() {
	return highscore.score;
}
int GameInstance::getLevel() {
	return highscore.level;
}
float GameInstance::getDropSpeed() {
	return dropSpeed;
}
TetrominoSPtr GameInstance::getLiveTetromino() {
	return liveTetromino;
}
TetrominoSPtr GameInstance::getNextTetromino() {
	return nextTetromino;
}
bool GameInstance::isRunning() {
	return running;
}

#pragma endregion
//=========== CONTROL ============
#pragma region Control

void GameInstance::tryAddScore(int scoreToAdd) {
	if (!highscore.infScore) {
		if (std::numeric_limits<unsigned long long>().max() - scoreToAdd < highscore.score)
			highscore.infScore = true;
		else
			highscore.score += scoreToAdd;
	}
}
void GameInstance::calculateDropSpeed() {
	dropSpeed = ((1.0f / 6.0f) / ((float)MAX_LEVEL + 1.0f - highscore.level));
}
bool GameInstance::generateNextTetromino(bool allowPentominos) {
	liveTetromino = nextTetromino;
	TetrominoShapeSPtr shape;

	if (gameMode.useTetrominos) {
		if (allowPentominos && gameMode.usePentominos && (rand() % gameMode.pentominoChance) == 0)
			shape = TetrominoList::getPentomino(rand() % TetrominoList::getPentominoCount());
		else if (allowPentominos && gameMode.useCustom && (rand() % gameMode.customChance) == 0)
			shape = TetrominoList::getCustom(rand() % TetrominoList::getCustomCount());
		else
			shape = TetrominoList::getTetromino(rand() % TetrominoList::getTetrominoCount());
	}
	else if (gameMode.usePentominos && gameMode.useCustom) {
		int r = rand() % (gameMode.pentominoChance + gameMode.customChance);
		if (r < gameMode.pentominoChance)
			shape = TetrominoList::getPentomino(rand() % TetrominoList::getPentominoCount());
		else
			shape = TetrominoList::getCustom(rand() % TetrominoList::getCustomCount());
	}
	else {
		if (gameMode.usePentominos)
			shape = TetrominoList::getPentomino(rand() % TetrominoList::getPentominoCount());
		else
			shape = TetrominoList::getCustom(rand() % TetrominoList::getCustomCount());
	}

	nextTetromino = std::make_shared<Tetromino>(well, shape);
	controller->setNextTetromino(nextTetromino);
	if (liveTetromino != nullptr) {
		liveTetromino->activate(dropSpeed);
		if (well->isColliding(liveTetromino->getTranslatedShape()))
			return false;
	}
	return true;
}
void GameInstance::gameOver() {
	running = false;

	time_t t = time(nullptr);
	tm now;
	localtime_s(&now, &t);
	now.tm_year += 1900;
	now.tm_year %= 100;
	highscore.date =
		/*(now.tm_mon + 1 < 10 ? "0" : "") +*/ std::to_string(now.tm_mon + 1) + "/" +
		(now.tm_mday < 10 ? "0" : "") + std::to_string(now.tm_mday) + "/" +
		std::to_string(now.tm_year);

	Highscore topScore;
	if (HighscoreList::getHighscoreCount(gameMode) > 0)
		topScore = HighscoreList::getHighscore(gameMode, 0);
	if (highscore > topScore)
		textSlider->start("NEW TOP SCORE!!!");
	else
		textSlider->start("GAME OVER!!!");
	SoundList::playSound("GameOver");
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void GameInstance::onTick(ControlSPtr sender, TickEventArgs& e) {
	if (!running && gameOverInput) {
		if (keyboard().isKeyPressed(Keys::Enter) || keyboard().isKeyPressed(Keys::Space)) {
			close();
		}
		return;
	}
	if (keyboard().isKeyPressed(Keys::Escape) || keyboard().isHotkeyPressed(Keys::P)) {
		SoundList::playSound("MenuSelect");
		auto pauseResult = PauseMenu::show(getWindow(), gameMode);
		switch (pauseResult) {
		case PauseMenuResults::Restart:
			result = GameInstanceResults::Restart;
			close();
			return;
		case PauseMenuResults::Quit:
			result = GameInstanceResults::Quit;
			close();
			return;
		}
	}
	if (textSlider->isAnimating()) {
		return;
	}

	// Increment playtime
	highscore.playTime++;

	if (well->isAnimating()) {
		well->updateAnimation();
		// Nothing else we should update this tick.
		return;
	}

	// Setup the new tetrominos if the last one has landed.
	// (If a line has been cleared, this will only be reached after the animation finishes)
	if (liveTetromino->hasLanded()) {
		if (!generateNextTetromino(true)) {
			gameOver();
			return;
		}
		else if (liveTetromino->getBaseShape()->hasWarning() && gameMode.useTetrominos) {
			if (sfxEnabled)
				SoundList::playSound("Warning");
		}
	}

	controller->update(keyboard());
	liveTetromino->update();

	// Perform tetromino landing checks
	if (liveTetromino->hasLanded()) {
		// Play the landing sound
		if (sfxEnabled)
			SoundList::playSound("Land");

		if (well->placeTetromino(liveTetromino)) {
			// Increment the place count of the current tetromino
			liveTetromino->getBaseShape()->incrementPlaceCount();
			int scoreAddition = 0;
			if (well->isRowCleared()) {
				int lastLinesCleared = linesCleared;
				linesCleared = (int)well->getClearedRows().size();

				// Play the line clear sound
				if (sfxEnabled) {
					if (linesCleared < 4)
						SoundList::playSound("RowComplete");
					else if (linesCleared == 4)
						SoundList::playSound("TetrisComplete");
					else
						SoundList::playSound("QuintrisComplete");
				}

				highscore.lines += linesCleared;
				if (highscore.level < MAX_LEVEL && gameMode.levelProgression) {
					linesLeft -= linesCleared;
					if (linesLeft <= 0) {
						highscore.level++;
						calculateDropSpeed();
						if (highscore.level == MAX_LEVEL)
							linesLeft = 0;
						else
							linesLeft += LINES_PER_LEVEL;
					}
				}

				// Calculate score after level changes
				scoreAddition += LINE_SCORE_CHART[linesCleared - 1] * highscore.level;
				if (lastLinesCleared >= 4 && linesCleared >= 4)
					scoreAddition += LINE_SCORE_CHART[std::min(lastLinesCleared, linesCleared) - 1] * highscore.level;
				if (combo >= 1)
					scoreAddition += COMBO_SCORE * combo * highscore.level;
				combo += linesCleared;
			}
			else {
				combo = 0;
			}
			scoreAddition += liveTetromino->getDropScore() * highscore.level;

			tryAddScore(scoreAddition);
		}
		else {
			gameOver();
		}
	}
}
void GameInstance::onGlobalTick(ControlSPtr sender, TickEventArgs& e) {
	if (PowerConsoleSettings::isDebugging()) {
		int tetrominoIncrement = 0;
		if (globalKeyboard().isKeyTyped(Keys::PageUp)) {
			tetrominoIncrement = -1;
		}
		else if (globalKeyboard().isKeyTyped(Keys::PageDown)) {
			tetrominoIncrement = 1;
		}
		if (tetrominoIncrement != 0) {
			int index = TetrominoList::indexOfTetromino(nextTetromino->getBaseShape());
			if (index == -1)
				index = 0;
			else
				index = GMath::wrap(index + tetrominoIncrement, TetrominoList::getTetrominoCount());
			TetrominoShapeSPtr shape = TetrominoList::getTetromino(index);
			nextTetromino = std::make_shared<Tetromino>(well, shape);
			controller->setNextTetrominoSilent(nextTetromino);
		}
		if (globalKeyboard().isKeyTyped(Keys::Insert)) {
			highscore.level = GMath::min(MAX_LEVEL, highscore.level + 1);
			calculateDropSpeed();
		}
		else if (globalKeyboard().isKeyTyped(Keys::Delete)) {
			highscore.level = GMath::max(1, highscore.level - 1);
			calculateDropSpeed();
		}
		if (globalKeyboard().isKeyTyped(Keys::S)) {
			if (!highscore.infScore) {
				if (std::numeric_limits<unsigned long long>().max() / 10 < highscore.score) {
					highscore.infScore = true;
				}
				else {
					highscore.score *= 10;
				}
			}
		}
	}
}
void GameInstance::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char textColor = toColor(BLACK, DARK_GRAY);
	const unsigned char borderColor = toColor(BLACK, running ? DARK_BLUE : DARK_RED);
	int offset = 0;
	int rightWidth = std::max(10, well->getBlockSize().x * 5) + 4;

	// Draw window border
	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', borderColor), true);

	g->setExtraTranslation(Point2I(2, 2));

	#define drawSeparator() \
	g->setPixel(Rectangle2I(0, offset, 14, 1), Pixel('_', borderColor)); offset += 2
	#define drawSeparator2() \
	g->setPixel(Rectangle2I(0, offset, rightWidth, 1), Pixel('_', borderColor)); offset += 2
	#define drawInfo(name, value) \
	g->drawString(Point2I(1, offset), name, Pixel(' ', textColor)); offset += 2; \
	g->drawString(Point2I(1, offset), value, Pixel(' ', textColor)); offset += 2
	#define drawInfo2(name, value) \
	g->drawString(Point2I(1, offset), name + value, Pixel(' ', textColor)); offset += 2
	#define drawTetrominoInfo(t) \
	tetromino = TetrominoList::getTetromino(t); \
	g->drawString(Point2I(1, offset), tetromino->getName() + ": " + largeIntToString(tetromino->getPlaceCount(), 9), Pixel(' ', textColor)); \
	offset += 2

	drawSeparator();
	Highscore topScore;
	if (HighscoreList::getHighscoreCount(gameMode) > 0)
		topScore = HighscoreList::getHighscore(gameMode, 0);
	drawInfo("Top Score:", (topScore.infScore ? "INFINITY" : largeIntToString(topScore.score, 12)));
	drawInfo("Score:", (highscore.infScore ? "INFINITY" : largeIntToString(highscore.score, 12)));
	drawSeparator();

	drawInfo("Lines:", largeIntToString(highscore.lines, 12));
	drawInfo("Combo:", std::to_string(combo));
	drawSeparator();

	drawInfo("Level:", std::to_string(highscore.level));
	drawInfo("Level up in:", std::to_string(linesLeft));
	drawSeparator();

	int sec = highscore.playTime / 60;
	int min = sec / 60;
	int hour = min / 60;
	min %= 60;
	sec %= 60;
	std::string timeStr = std::to_string(hour) + ":" +
		(min < 10 ? "0" : "") + std::to_string(min) + ":" +
		(sec < 10 ? "0" : "") + std::to_string(sec);

	drawInfo("Time:", playTimeToString(highscore.playTime));
	drawSeparator();

	g->setExtraTranslation(Point2I(getActualSize().x - rightWidth - 2, 2));
	offset = 0;
	TetrominoShapeSPtr tetromino;

	drawSeparator2();

	// Draw next tetromino
	Rectangle2I nextFrame = Rectangle2I(0, offset, well->getBlockSize() * 5 + 2);
	nextFrame.point.x = (rightWidth - nextFrame.size.x) / 2;
	g->drawBorder(nextFrame, Pixel(' ', borderColor), false);
	offset += nextFrame.size.y + 1;
	nextTetromino->drawNext(g, nextFrame.center(), well->getBlockSize());
	if (TetrominoList::indexOfCustom(nextTetromino->getBaseShape()) == -1) {
		drawInfo2("Next: ", nextTetromino->getBaseShape()->getName());
	}
	else {
		drawInfo2(nextTetromino->getBaseShape()->getName(), "");
	}
	drawSeparator2();

	if (gameMode.useTetrominos) {
		for (int i = 0; i < TetrominoList::getTetrominoCount(); i++) {
			drawTetrominoInfo(i);
		}

		if (gameMode.usePentominos) {
			offset++;

			int pentominoCount = 0;
			for (auto pentomino : TetrominoList::getPentominos()) {
				pentominoCount += pentomino->getPlaceCount();
			}
			drawInfo2("P: ", largeIntToString(pentominoCount, 9));
		}
	}
	else if (gameMode.usePentominos) {
		for (int i = 0; i < TetrominoList::getPentominoCount(); i++) {
			int count = TetrominoList::getPentomino(i)->getPlaceCount();
			char name = TetrominoList::getPentomino(i)->getName()[0];
			if (i + 1 < TetrominoList::getPentominoCount()) {
				if (name == TetrominoList::getPentomino(i + 1)->getName()[0]) {
					i++;
					count += TetrominoList::getPentomino(i)->getPlaceCount();
				}
			}
			drawInfo2(std::string(1, name) + ": ", largeIntToString(count, 9));
		}
		if (gameMode.useCustom)
			offset++;
	}
	if (gameMode.useCustom) {
		int customCount = 0;
		for (auto custom : TetrominoList::getCustoms()) {
			customCount += custom->getPlaceCount();
		}
		drawInfo2("C: ", largeIntToString(customCount, 9));
	}
	drawSeparator2();

	g->setExtraTranslation(Point2I::ZERO);

	// Draw the well and everything inside of it.
	g->startRegion(Point2I(18, 3), Rectangle2I(-1, -1, well->getDimensions() * well->getBlockSize() + 2));

	// Draw well walls.
	unsigned char wallColor = toColor(BLACK, DARK_BLUE);
	if (!running)
		wallColor = toColor(BLACK, DARK_RED);
	g->drawBorder(Rectangle2I(-1, -1, well->getDimensions() * well->getBlockSize() + 2), Pixel(' ', wallColor), true);

	well->draw(g, liveTetromino, running, highscore.playTime);
	g->endRegion();
}

void GameInstance::onTextSliderAnimationFinished(ControlSPtr sender, EventArgs& e) {
	if (textSlider->getText() == "READY??") {
		textSlider->start("GO!!!");
	}
	else if (textSlider->getText() == "GAME OVER!!!" || textSlider->getText() == "NEW TOP SCORE!!!") {
		textSlider->restart();
		gameOverInput = true;
	}
}

#pragma endregion
//=================================================================|
