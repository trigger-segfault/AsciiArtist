/*=================================================================|
|* File:				GameModeList.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resources.h"
#include "GameModeList.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/IO/Resources.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace AsciiTetris::Resources;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region GameMode
//========= CONSTRUCTORS =========
#pragma region Constructors

GameMode::GameMode()
 :	// Name
	name(""),
	description(""),

	wellSize(Point2I(10, 20)),
	levelProgression(true),
	useTetrominos(true),
	usePentominos(false),
	useCustom(false),
	pentominoChance(40),
	customChance(40) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

bool GameMode::operator==(const GameMode& gameMode) const {
	return
		wellSize == gameMode.wellSize &&
		levelProgression == gameMode.levelProgression &&
		useTetrominos == gameMode.useTetrominos &&
		usePentominos == gameMode.usePentominos &&
		useCustom == gameMode.useCustom &&
		(pentominoChance == gameMode.pentominoChance || !usePentominos) &&
		(customChance == gameMode.customChance || !useCustom);
}
bool GameMode::operator!=(const GameMode& gameMode) const {
	return !(*this == gameMode);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma region AIList
//=========== MEMBERS ============
#pragma region Tetrominos

std::vector<GameMode> GameModeList::gameModes;
IniDocumentSPtr GameModeList::document;

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

/*GameModeList::GameModeList() {
	loadGameModes();
}*/

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

const std::vector<GameMode>& GameModeList::getGameModes() {
	return gameModes;
}
GameMode GameModeList::getGameMode(int index) {
	return gameModes[index];
}
int GameModeList::getGameModeCount() {
	return (int)gameModes.size();
}
int GameModeList::indexOfGameMode(GameMode gameMode) {
	auto it = std::find(gameModes.begin(), gameModes.end(), gameMode);
	if (it != gameModes.end())
		return std::distance(gameModes.begin(), it);
	return -1;
}

#pragma endregion
//=========== CREATION ===========
#pragma region Creation

void GameModeList::addGameMode(IniSectionSPtr section) {
	auto gameMode = GameMode();

	gameMode.name = section->getString("Name", "NO MODE NAME");
	if (gameMode.name.length() > 15)
		gameMode.name = gameMode.name.substr(0, 15);
	gameMode.description = section->getString("Description", "");

	gameMode.wellSize = section->getPoint2I("WellSize", Point2I(10, 20));
	gameMode.levelProgression = section->getBool("LevelProgression", "true", true);
	gameMode.useTetrominos = section->getBool("UseTetrominos", "true", true);
	gameMode.usePentominos = section->getBool("UsePentominos", "true", false);
	gameMode.useCustom = section->getBool("UseCustom", "true", false);
	gameMode.pentominoChance = GMath::clamp(section->getInt("PentominoChance", RADIX_DECIMAL, 40), 0, 1000000);
	gameMode.customChance = GMath::clamp(section->getInt("CustomChance", RADIX_DECIMAL, 40), 0, 1000000);

	// We need one or the other enabled
	if (!gameMode.usePentominos && !gameMode.useCustom)
		gameMode.useTetrominos = true;

	gameModes.push_back(gameMode);
}
void GameModeList::loadGameModes() {
	gameModes.clear();

	document = makeIniDocument();
	document->setAllowDuplicates(true);
	document->setKeepComments(true);
	document->setEscapeEnabled(false);

	std::string path = Path::combine(Path::getExecutableDirectory(), "GameModeList.ini");
	if (Path::fileExists(path) && document->load(path)) {

	}
	else {
		document->loadFromText(PowerConsole::IO::Resources::loadText(INI_GAME_MODE_LIST, "INI_FILE"));
		document->save(path);
	}

	for (IniSectionSPtr section : document->getSections()) {
		if (section->getKey() == "GameMode") {
			addGameMode(section);
		}
	}
	if (gameModes.empty()) {
		document->loadFromText(PowerConsole::IO::Resources::loadText(INI_GAME_MODE_LIST, "INI_FILE"));
		for (IniSectionSPtr section : document->getSections()) {
			if (section->getKey() == "GameMode") {
				addGameMode(section);
			}
		}
	}
}
void GameModeList::saveGameModes() {
	std::string path = Path::combine(Path::getExecutableDirectory(), "GameModeList.ini");
	document->save(path);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
