/*=================================================================|
|* File:				AIList.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resources.h"
#include "AIList.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/IO/Resources.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace AsciiTetris::Resources;
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region AISettings
//========= CONSTRUCTORS =========
#pragma region Constructors

AISettings::AISettings()
 :	// Name
	name(""),
	description(""),
	sfxEnabled(true),

	// Positive Weights
	edgeWeight(0),
	lineClearWeight(0),


	// Negative Weights
	heightWeight(0),
	coveredWeight(0),
	alreadyCoveredWeight(0),

	// Speeds,
	thinkSpeed(1000 / (1000 / 60)),
	actionSpeed(500 / (1000 / 60)) {}

#pragma endregion
//========== OPERATORS ===========
#pragma region Operators

bool AISettings::operator==(const AISettings& ai) const {
	return
		// Weights
		edgeWeight == ai.edgeWeight &&
		lineClearWeight == ai.lineClearWeight &&
		heightWeight == ai.heightWeight &&
		coveredWeight == ai.coveredWeight &&
		alreadyCoveredWeight == ai.alreadyCoveredWeight &&

		// Speeds
		thinkSpeed == ai.thinkSpeed &&
		actionSpeed == ai.actionSpeed;

}
bool AISettings::operator!=(const AISettings& ai) const {
	return !(*this == ai);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma region AIList
//=========== MEMBERS ============
#pragma region Tetrominos

std::vector<AISettings> AIList::ais;
IniDocumentSPtr AIList::document;

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

/*AIList::AIList() {
	loadAIs();
}*/

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

const std::vector<AISettings>& AIList::getAIs() {
	return ais;
}
AISettings AIList::getAI(int index) {
	return ais[index];
}
int AIList::getAICount() {
	return (int)ais.size();
}
int AIList::indexOfAI(AISettings ai) {
	auto it = std::find(ais.begin(), ais.end(), ai);
	if (it != ais.end())
		return std::distance(ais.begin(), it);
	return -1;
}

#pragma endregion
//=========== CREATION ===========
#pragma region Creation

void AIList::addAI(IniSectionSPtr section) {
	auto ai = AISettings();

	ai.name = section->getString("Name", "NO AI NAME");
	if (ai.name.length() > 15)
		ai.name = ai.name.substr(0, 15);
	ai.description = section->getString("Description", "");
	ai.sfxEnabled = section->getBool("SFXEnabled", "true", true);

	ai.thinkSpeed = section->getInt("ThinkSpeed", RADIX_DECIMAL, 1000);
	ai.actionSpeed = section->getInt("ActionSpeed", RADIX_DECIMAL, 50);
	ai.thinkSpeed /= (1000 / 60);
	ai.actionSpeed /= (1000 / 60);

	ai.edgeWeight = section->getInt("EdgeWeight", RADIX_DECIMAL, 0);
	ai.lineClearWeight = section->getInt("LineClearWeight", RADIX_DECIMAL, 0);
	ai.heightWeight = section->getInt("HeightWeight", RADIX_DECIMAL, 0);
	ai.coveredWeight = section->getInt("CoveredWeight", RADIX_DECIMAL, 0);
	ai.alreadyCoveredWeight = section->getInt("AlreadyCoveredWeight", RADIX_DECIMAL, 0);

	ais.push_back(ai);
}
void AIList::loadAIs() {
	ais.clear();

	document = makeIniDocument();
	document->setAllowDuplicates(true);
	document->setKeepComments(true);
	document->setEscapeEnabled(false);

	std::string path = Path::combine(Path::getExecutableDirectory(), "AIList.ini");
	if (!Path::fileExists(path) || !document->load(path)) {
		document->loadFromText(PowerConsole::IO::Resources::loadText(INI_AI_LIST, "INI_FILE"));
		document->save(path);
	}

	for (IniSectionSPtr section : document->getSections()) {
		if (section->getKey() == "AI") {
			addAI(section);
		}
	}
	if (ais.empty()) {
		document->loadFromText(PowerConsole::IO::Resources::loadText(INI_AI_LIST, "INI_FILE"));
		for (IniSectionSPtr section : document->getSections()) {
			if (section->getKey() == "AI") {
				addAI(section);
			}
		}
	}
}
void AIList::saveAIs() {
	std::string path = Path::combine(Path::getExecutableDirectory(), "AIList.ini");
	document->save(path);
}

#pragma endregion
//================================
#pragma endregion
//=================================================================|
