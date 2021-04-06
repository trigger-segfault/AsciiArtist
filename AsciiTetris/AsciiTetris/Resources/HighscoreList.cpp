/*=================================================================|
|* File:				HighscoreList.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "HighscoreList.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/IO/Resources.h>
#include <AsciiTetris/Helpers/Helpers.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Helpers/Parsing.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace AsciiTetris::Helpers;
using namespace AsciiTetris::Resources;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region GameMode
//========= CONSTRUCTORS =========
#pragma region Constructors

Highscore::Highscore()
 :	// Name
	name(""),
	aiMode(false),

	score(0UL),
	infScore(false),
	lines(0),
	level(1),
	startLevel(1),
	placed(0),
	playTime(0),
	date("") {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

bool Highscore::operator==(const Highscore& highscore) const {
	return
		((infScore && highscore.infScore) || (score == highscore.score)) &&
		lines == highscore.lines &&
		playTime == highscore.playTime;
}
bool Highscore::operator!=(const Highscore& highscore) const {
	return !(*this == highscore);
}
bool Highscore::operator>(const Highscore& highscore) const {
	if ((infScore && highscore.infScore) || (score == highscore.score)) {
		if (lines == highscore.lines) {
			return playTime < highscore.playTime;
		}
		else {
			return lines < highscore.lines;
		}
	}
	else {
		return score > highscore.score;
	}
}
bool Highscore::operator<(const Highscore& highscore) const {
	if ((infScore && highscore.infScore) || (score == highscore.score)) {
		if (lines == highscore.lines) {
			return playTime > highscore.playTime;
		}
		else {
			return lines > highscore.lines;
		}
	}
	else {
		return score < highscore.score;
	}
}
bool Highscore::operator>=(const Highscore& highscore) const {
	if ((infScore && highscore.infScore) || (score == highscore.score)) {
		if (lines == highscore.lines) {
			return playTime <= highscore.playTime;
		}
		else {
			return lines < highscore.lines;
		}
	}
	else {
		return score > highscore.score;
	}
}
bool Highscore::operator<=(const Highscore& highscore) const {
	if ((infScore && highscore.infScore) || (score == highscore.score)) {
		if (lines == highscore.lines) {
			return playTime >= highscore.playTime;
		}
		else {
			return lines > highscore.lines;
		}
	}
	else {
		return score < highscore.score;
	}
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma region HighscoreList
//=========== MEMBERS ============
#pragma region Tetrominos

std::unordered_map<GameMode, std::vector<Highscore>> HighscoreList::highscores;
IniDocumentSPtr HighscoreList::document;

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

/*HighscoreList::HighscoreList() {

}*/

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

const std::vector<Highscore>& HighscoreList::getHighscores(GameMode gameMode) {
	return highscores[gameMode];
}
Highscore HighscoreList::getHighscore(GameMode gameMode, int index) {
	return highscores[gameMode][index];
}
int HighscoreList::getHighscoreCount(GameMode gameMode) {
	return (int)highscores[gameMode].size();
}
void HighscoreList::updateHighscoreName(GameMode gameMode, int index, const std::string& name) {
	highscores[gameMode][index].name = name;
}

#pragma endregion
//=========== CREATION ===========
#pragma region Creation

void HighscoreList::addHighscore(GameMode gameMode, IniSectionSPtr section) {
	Highscore h = Highscore();

	h.name = section->getString("Name", "NO NAME");
	if (h.name.length() > 15)
		h.name = h.name.substr(0, 15);
	h.aiMode = section->getBool("AI");
	std::string scoreStr = section->getString("Score", "0");
	if (compareString(scoreStr, "INFINITY", false))
		h.infScore = true;
	else {
		try {
			h.score = Parsing::parseIntType<unsigned long long>(scoreStr);
		}
		catch (...) {}
	}

	h.lines = section->getInt("Lines");
	h.level = section->getInt("Level", RADIX_DECIMAL, 1);
	h.startLevel = section->getInt("StartLevel", RADIX_DECIMAL, 1);
	h.placed = section->getInt("Placed");
	h.playTime = parsePlayTime(section->getString("PlayTime"));
	h.date = section->getString("Date", "??/??/??");

	highscores[gameMode].push_back(h);
}
int HighscoreList::addHighscore(GameMode gameMode, Highscore highscore) {
	auto& list = highscores[gameMode];

	for (int i = 0; i < (int)list.size(); i++) {
		if (highscore > list[i]) {
			list.insert(list.begin() + i, highscore);
			return i;
		}
	}
	list.push_back(highscore);
	return (int)list.size() - 1;
}
void HighscoreList::loadHighscores() {
	highscores.clear();

	document = makeIniDocument();
	document->setAllowDuplicates(true);
	document->setKeepComments(true);
	document->setEscapeEnabled(false);

	std::string path = Path::combine(Path::getExecutableDirectory(), "HighscoreList.ini");
	if (Path::fileExists(path) && document->load(path)) {

	}
	else {
		//document->loadFromText(PowerConsole::IO::Resources::loadText(INI_GAME_MODE_LIST, "INI_FILE"));
		//document->save(path);
	}

	GameMode gameMode;
	bool gameModeLoaded = false;

	for (IniSectionSPtr section : document->getSections()) {
		if (section->getKey() == "GameMode") {
			// We're finished loading this game mode. Now we should sort it
			if (gameModeLoaded)
				std::sort(highscores[gameMode].begin(), highscores[gameMode].end(), std::greater<Highscore>());

			gameMode.wellSize = section->getPoint2I("WellSize", Point2I(10, 20));
			gameMode.levelProgression = section->getBool("LevelProgression", "true", true);
			gameMode.useTetrominos = section->getBool("UseTetrominos", "true", true);
			gameMode.usePentominos = section->getBool("UsePentominos", "true", true);
			gameMode.useCustom = section->getBool("UseCustom", "true", false);
			gameMode.pentominoChance = GMath::clamp(section->getInt("PentominoChance", RADIX_DECIMAL, 40), 0, 1000000);
			gameMode.customChance = GMath::clamp(section->getInt("CustomChance", RADIX_DECIMAL, 40), 0, 1000000);
			gameModeLoaded = true;
		}
		else if (section->getKey() == "Highscore" && gameModeLoaded) {
			addHighscore(gameMode, section);
		}
	}
	if (gameModeLoaded)
		std::sort(highscores[gameMode].begin(), highscores[gameMode].end(), std::greater<Highscore>());
}
void HighscoreList::saveHighscores() {
	std::string path = Path::combine(Path::getExecutableDirectory(), "HighscoreList.ini");

	document = makeIniDocument();
	document->setAllowDuplicates(true);
	document->setKeepComments(true);
	document->setEscapeEnabled(false);
	document->setSectionSpacing(1);

	for (auto& scorePair : highscores) {
		GameMode gameMode = scorePair.first;
		auto& scores = scorePair.second;
		if (scores.empty())
			continue;

		auto section = makeIniSection("GameMode");
		section->setPoint2I("WellSize", gameMode.wellSize, "", "x", "");
		section->setBool("LevelProgression", gameMode.levelProgression);
		section->setBool("UseTetrominos", gameMode.useTetrominos);
		section->setBool("UsePentominos", gameMode.usePentominos);
		section->setBool("UseCustom", gameMode.useCustom);
		section->setInt("PentominoChance", gameMode.pentominoChance);
		section->setInt("CustomChance", gameMode.customChance);
		document->add(section);

		for (Highscore& h : scores) {
			section = makeIniSection("Highscore");
			section->setString("Name", h.name);
			section->get("Name")->setUseQuotes(true);
			section->setBool("AI", h.aiMode);
			if (h.infScore)
				section->setString("Score", "INFINITY");
			else
				section->setString("Score", std::to_string(h.score));
			section->setInt("Lines", h.lines);
			section->setInt("Level", h.level);
			section->setInt("StartLevel", h.startLevel);
			section->setInt("Placed", h.placed);
			section->setString("PlayTime", playTimeToString(h.playTime));
			section->setString("Date", h.date);
			document->add(section);
		}
	}

	document->save(path);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
