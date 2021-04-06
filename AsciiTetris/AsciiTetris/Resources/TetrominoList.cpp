/*=================================================================|
|* File:				TetrominoList.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "TetrominoList.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/IO/Resources.h>
#include <PowerConsole/Helpers/StringHelpers.h>

using namespace AsciiTetris::Resources;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Helpers;
//=================================================================|
// CLASSES														   |
//=================================================================/
//=========== MEMBERS ============
#pragma region Tetrominos

std::vector<TetrominoShapeSPtr> TetrominoList::tetrominos;
std::vector<TetrominoShapeSPtr> TetrominoList::pentominos;
std::vector<TetrominoShapeSPtr> TetrominoList::custom;

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

/*TetrominoList::TetrominoList() {
	createTetrominos();
	createPentominos();
	loadCustom();
}*/

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

const std::vector<TetrominoShapeSPtr>& TetrominoList::getTetrominos() {
	return tetrominos;
}
const std::vector<TetrominoShapeSPtr>& TetrominoList::getPentominos() {
	return pentominos;
}
const std::vector<TetrominoShapeSPtr>& TetrominoList::getCustoms() {
	return custom;
}
TetrominoShapeSPtr TetrominoList::getTetromino(int index) {
	return tetrominos[index];
}
TetrominoShapeSPtr TetrominoList::getPentomino(int index) {
	return pentominos[index];
}
TetrominoShapeSPtr TetrominoList::getCustom(int index) {
	return custom[index];
}
int TetrominoList::getTetrominoCount() {
	return (int)tetrominos.size();
}
int TetrominoList::getPentominoCount() {
	return (int)pentominos.size();
}
int TetrominoList::getCustomCount() {
	return (int)custom.size();
}
int TetrominoList::indexOfTetromino(TetrominoShapeSPtr tetromino) {
	auto it = std::find(tetrominos.begin(), tetrominos.end(), tetromino);
	if (it != tetrominos.end())
		return std::distance(tetrominos.begin(), it);
	return -1;
}
int TetrominoList::indexOfPentomino(TetrominoShapeSPtr pentomino) {
	auto it = std::find(pentominos.begin(), pentominos.end(), pentomino);
	if (it != pentominos.end())
		return std::distance(pentominos.begin(), it);
	return -1;
}
int TetrominoList::indexOfCustom(TetrominoShapeSPtr custom) {
	auto it = std::find(TetrominoList::custom.begin(), TetrominoList::custom.end(), custom);
	if (it != TetrominoList::custom.end())
		return std::distance(TetrominoList::custom.begin(), it);
	return -1;
}

#pragma endregion
//=========== CREATION ===========
#pragma region Creation

void TetrominoList::addTetromino(const std::string& name, unsigned char color, Point2F origin, const std::string& data) {
	tetrominos.push_back(std::make_shared<TetrominoShape>(name, color, false, origin, data));
}
void TetrominoList::addPentomino(const std::string& name, unsigned char color, Point2F origin, const std::string& data) {
	pentominos.push_back(std::make_shared<TetrominoShape>(name, color, true, origin, data));
}
void TetrominoList::addCustom(IniSectionSPtr section) {
	// Name
	std::string name = section->getString("Name", "?");
	if (name.length() > 12)
		name = name.substr(0, 12);

	// Color
	std::string colorStr = section->getString("Color", "Red");
	const std::string COLOR_NAMES[7] = {
		"Red", "Orange", "Yellow", "Green", "Cyan", "Blue", "Magenta"
	};
	const unsigned char COLORS[7] = {
		RED, WHITE, YELLOW, GREEN, CYAN, BLUE, MAGENTA
	};
	unsigned char color = RED;
	for (int i = 0; i < 7; i++) {
		if (compareString(colorStr, COLOR_NAMES[i], false)) {
			color = COLORS[i];
			break;
		}
	}

	// Warning
	bool warning = section->getBool("Warning", "true");

	// Origin
	Point2F origin = section->getPoint2F("Origin", Point2F::ONE);
	Point2F fmod = Point2F(GMath::fmod(origin.x, 1.0f), GMath::fmod(origin.y, 1.0f));
	// Check for an invalid origin
	if (fmod != Point2F::ZERO && fmod != Point2F(0.5f))
		return;

	// Data
	std::string data = "";
	bool hasABlock = false;
	for (int i = 0; i < 5; i++) {
		std::string row = section->getString("Row" + std::to_string(i + 1), "");
		// Check for invalid length
		if (row.length() > 5)
			return;
		for (int j = 0; j < (int)row.length(); j++) {
			// Check for invalid characters
			if (row[j] == '#')
				hasABlock = true;
			else if (row[j] != '`')
				return;
		}
		if (row.empty())
			row = "`";

		data += "|" + row + "|";
	}
	if (!hasABlock)
		return;

	custom.push_back(std::make_shared<TetrominoShape>(name, color, warning, origin, data));
}

void TetrominoList::createTetrominos() {
	tetrominos.clear();

	addTetromino("L", WHITE, Point2F(1.5f),
		"|`#``|"
		"|`#``|"
		"|`##`|"
		"|````|"
	);
	addTetromino("J", BLUE, Point2F(1.5f),
		"|`#``|"
		"|`#``|"
		"|##``|"
		"|````|"
	);
	addTetromino("S", GREEN, Point2F(1.5f),
		"|````|"
		"|`##`|"
		"|##``|"
		"|````|"
	);
	addTetromino("Z", RED, Point2F(1.5f),
		"|````|"
		"|##``|"
		"|`##`|"
		"|````|"
	);
	addTetromino("T", MAGENTA, Point2F(1.5f),
		"|````|"
		"|###`|"
		"|`#``|"
		"|````|"
	);
	addTetromino("O", YELLOW, Point2F(2.0f),
		"|````|"
		"|`##`|"
		"|`##`|"
		"|````|"
	);
	addTetromino("I", CYAN, Point2F(2.0f),
		"|`#``|"
		"|`#``|"
		"|`#``|"
		"|`#``|"
	);
}
void TetrominoList::createPentominos() {
	pentominos.clear();

	addPentomino("I+", RED, Point2F(2.5f),
		"|``#``|"
		"|``#``|"
		"|``#``|"
		"|``#``|"
		"|``#``|"
	);
	addPentomino("F+", WHITE, Point2F(1.5f),
		"|`##`|"
		"|##``|"
		"|`#``|"
		"|````|"
	);
	addPentomino("F-", WHITE, Point2F(1.5f),
		"|##``|"
		"|`##`|"
		"|`#``|"
		"|````|"
	);
	addPentomino("L+", YELLOW, Point2F(2.0f),
		"|`#``|"
		"|`#``|"
		"|`#``|"
		"|`##`|"
	);
	addPentomino("L-", YELLOW, Point2F(2.0f),
		"|``#`|"
		"|``#`|"
		"|``#`|"
		"|`##`|"
	);
	addPentomino("N+", GREEN, Point2F(2.0f),
		"|``#`|"
		"|``#`|"
		"|`##`|"
		"|`#``|"
	);
	addPentomino("N-", GREEN, Point2F(2.0f),
		"|`#``|"
		"|`#``|"
		"|`##`|"
		"|``#`|"
	);
	addPentomino("P+", CYAN, Point2F(2.0f),
		"|````|"
		"|`##`|"
		"|`##`|"
		"|`#``|"
	);
	addPentomino("P-", CYAN, Point2F(2.0f),
		"|````|"
		"|`##`|"
		"|`##`|"
		"|``#`|"
	);
	addPentomino("T+", CYAN, Point2F(1.5f),
		"|###`|"
		"|`#``|"
		"|`#``|"
		"|````|"
	);
	addPentomino("U+", BLUE, Point2F(1.5f),
		"|#`#`|"
		"|###`|"
		"|````|"
		"|````|"
	);
	addPentomino("V+", BLUE, Point2F(1.5f),
		"|``#`|"
		"|``#`|"
		"|###`|"
		"|````|"
	);
	addPentomino("W+", BLUE, Point2F(1.5f),
		"|``#`|"
		"|`##`|"
		"|##``|"
		"|````|"
	);
	addPentomino("X+", MAGENTA, Point2F(1.5f),
		"|`#``|"
		"|###`|"
		"|`#``|"
		"|````|"
	);
	addPentomino("Y+", MAGENTA, Point2F(2.0f),
		"|``#`|"
		"|`##`|"
		"|``#`|"
		"|``#`|"
	);
	addPentomino("Y-", MAGENTA, Point2F(2.0f),
		"|`#``|"
		"|`##`|"
		"|`#``|"
		"|`#``|"
	);
	addPentomino("Z+", RED, Point2F(1.5f),
		"|##``|"
		"|`#``|"
		"|`##`|"
		"|````|"
	);
	addPentomino("Z-", RED, Point2F(1.5f),
		"|`##`|"
		"|`#``|"
		"|##``|"
		"|````|"
	);
}
void TetrominoList::loadCustom() {
	custom.clear();

	auto document = makeIniDocument();
	document->setAllowDuplicates(true);
	document->setKeepComments(true);
	document->setEscapeEnabled(false);

	std::string path = Path::combine(Path::getExecutableDirectory(), "TetrominoList.ini");
	if (Path::fileExists(path) && document->load(path)) {

	}
	else {
		document->loadFromText(PowerConsole::IO::Resources::loadText(INI_TETROMINO_LIST, "INI_FILE"));
		document->save(path);
	}

	for (IniSectionSPtr section : document->getSections()) {
		if (section->getKey() == "Tetromino") {
			addCustom(section);
		}
	}
	if (custom.empty()) {
		document->loadFromText(PowerConsole::IO::Resources::loadText(INI_TETROMINO_LIST, "INI_FILE"));
		for (IniSectionSPtr section : document->getSections()) {
			if (section->getKey() == "Tetromino") {
				addCustom(section);
			}
		}
	}
}

#pragma endregion
//=================================================================|
