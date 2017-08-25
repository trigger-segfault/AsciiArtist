/*=================================================================|
|* File:				GameModeList.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef GAME_MODE_LIST_H
#define GAME_MODE_LIST_H

#include <PowerConsole/Geometry/Point.h>
#include <PowerConsole/IO/IniDocument.h>
#include <memory>
#include <vector>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;

namespace AsciiTetris::Resources {
//=================================================================|
// CLASSES														   |
//=================================================================/

struct GameMode {

	//=========== MEMBERS ============
	#pragma region Tetrominos

	/* The name of the game mode. */
	std::string name;
	/* The description of the game mode. */
	std::string description;

	/* The size of the well. */
	Point2I wellSize;
	/* True if levels and level speed are enabled. */
	bool levelProgression;
	/* True if tetrominos are enabled. */
	bool useTetrominos;
	/* True if pentominos are enabled. */
	bool usePentominos;
	/* True if custom tetrominos are enabled. */
	bool useCustom;
	/* The chance of a pentomino appearing. */
	int pentominoChance;
	/* The chance of a custom tetromino appearing. */
	int customChance;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the game mode. */
	GameMode();

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Returns true if the game modes are the same. Does not compare names. */
	bool operator==(const GameMode& gameMode) const;
	/* Returns true if the game modes are not the same. Does not compare names. */
	bool operator!=(const GameMode& gameMode) const;

	#pragma endregion
};

DECLARE_SMART_PTRS(GameMode);

/* The resource list for game modes. */
class GameModeList {

	//=========== MEMBERS ============
	#pragma region Tetrominos

	/* The list of game modes. */
	static std::vector<GameMode> gameModes;
	/* The loaded game mode document. */
	static IniDocumentSPtr document;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Initializes the resources database. */
	//GameModeList();

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the full list of game modes. */
	static const std::vector<GameMode>& getGameModes();
	/* Gets the GameMode mode at the specified index in the list. */
	static GameMode getGameMode(int index);
	/* Gets the number of game modes in the list. */
	static int getGameModeCount();
	/* Gets the index of the game mode in the list. */
	static int indexOfGameMode(GameMode gameMode);

	#pragma endregion
	//=========== CREATION ===========
	#pragma region Creation
private:
	/* Reads a game mode section and adds it to the list. */
	static void addGameMode(IniSectionSPtr section);
public:
	/* Loads the list of game modes. */
	static void loadGameModes();
	/* Saves the list of game modes. */
	static void saveGameModes();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
