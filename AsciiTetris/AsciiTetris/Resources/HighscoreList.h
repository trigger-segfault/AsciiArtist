/*=================================================================|
|* File:				HighscoreList.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef HIGHSCORE_LIST_H
#define HIGHSCORE_LIST_H

#include <PowerConsole/Geometry/Point.h>
#include <AsciiTetris/Resources/GameModeList.h>
#include <AsciiTetris/Helpers/Helpers.h>
#include <PowerConsole/IO/IniDocument.h>
#include <memory>
#include <vector>
#include <unordered_map>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
using namespace AsciiTetris::Resources;
using namespace AsciiTetris::Helpers;

//=================================================================|
// PREDEFINITIONS												   |
//=================================================================/
namespace std {
template <> class hash<GameMode> {
public:
	size_t operator()(const GameMode& gameMode) const {
		// ... your hash function for Key object ...
		size_t res = 0;
		hash_combine(res, gameMode.wellSize.x);
		hash_combine(res, gameMode.wellSize.y);
		hash_combine(res, gameMode.levelProgression);
		hash_combine(res, gameMode.usePentominos);
		return res;
	}
};
}

namespace AsciiTetris::Resources {
//=================================================================|
// CLASSES														   |
//=================================================================/

struct Highscore {

	//=========== MEMBERS ============
	#pragma region Tetrominos

	/* The name of the highscore holder. The maximum length of this name is 15 characters. */
	std::string name;
	/* True if an ai was running the game. */
	bool aiMode;

	/* The score for the game. */
	unsigned long long score;
	/* True if the score is infinite. */
	bool infScore;
	/* The number of lines cleared in the game. */
	int lines;
	/* The level of the game. */
	int level;
	/* The starting level of the game. */
	int startLevel;
	/* The number of tetrominos and pentominos placed. */
	int placed;
	/* The playtime for the game. 1 unit is equal to 1/60th of a second. */
	int playTime;
	/* The date and time of the score. */
	std::string date;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the highscore. */
	Highscore();

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Returns true if the highscore is equal to the specified highscore. */
	bool operator==(const Highscore& highscore) const;
	/* Returns true if the highscore is not equal to the specified highscore. */
	bool operator!=(const Highscore& highscore) const;
	/* Returns true if the highscore is greater than the specified highscore. */
	bool operator>(const Highscore& highscore) const;
	/* Returns true if the highscore is less than the specified highscore. */
	bool operator<(const Highscore& highscore) const;
	/* Returns true if the highscore is greater than or equal to the specified highscore. */
	bool operator>=(const Highscore& highscore) const;
	/* Returns true if the highscore is less than or equal to the specified highscore. */
	bool operator<=(const Highscore& highscore) const;

	#pragma endregion
};

DECLARE_SMART_PTRS(Highscore);

/* The resource list for tetrominos. */
class HighscoreList {

	//=========== MEMBERS ============
	#pragma region Tetrominos

	/* The list of highscores. */
	static std::unordered_map<GameMode, std::vector<Highscore>> highscores;
	/* The loaded highscore document. */
	static IniDocumentSPtr document;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Initializes the resources database. */
	//HighscoreList();

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the full list of highscores. */
	static const std::vector<Highscore>& getHighscores(GameMode gameMode);
	/* Gets the highscore at the specified index in the list. */
	static Highscore getHighscore(GameMode gameMode, int index);
	/* Gets the number of highscores in the list. */
	static int getHighscoreCount(GameMode gameMode);
	/* Updates the name of the highscore at the specified index. */
	static void updateHighscoreName(GameMode gameMode, int index, const std::string& name);

	#pragma endregion
	//=========== CREATION ===========
	#pragma region Creation
private:
	/* Reads a highscore section and adds it to the list. */
	static void addHighscore(GameMode gameMode, IniSectionSPtr section);
public:
	/* Adds a highscore to the list. */
	static int addHighscore(GameMode gameMode, Highscore highscore);
	/* Loads the list of highscores. */
	static void loadHighscores();
	/* Saves the list of highscores. */
	static void saveHighscores();

	#pragma endregion
};

DECLARE_SMART_PTRS(HighscoreList);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
