/*=================================================================|
|* File:				AIList.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef AI_LIST_H
#define AI_LIST_H

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
/* The definition of the AI and how it performs and acts. */
struct AISettings {

	//=========== MEMBERS ============
	#pragma region Members

	/* The name of the AI. */
	std::string name;
	/* The description of the AI. */
	std::string description;
	/* True if special effects are shown and played. */
	bool sfxEnabled;

	// Positive Weights
	/* The score modifier for each edge that is touching the placed tetromino. */
	int edgeWeight;
	/* The score modifier for each cleared line. */
	int lineClearWeight;

	// Negative Weights
	/* The score modifier for the height of the placed tetromino in the well. */
	int heightWeight;
	/* The score modifier for when a placed tetromino covers an opening. */
	int coveredWeight;
	/* The score modifier for when a placed tetromino covers an already-covered openeing. */
	int alreadyCoveredWeight;

	// Speeds
	/* The time it takes the AI to pick a location to place the tetromino. */
	/* This calculation will still be done all at once. */
	int thinkSpeed;
	/* The speed at which the AI performs actions. */
	int actionSpeed;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the AI settings. */
	AISettings();

	#pragma endregion
	//========== OPERATORS ===========
	#pragma region Operators

	/* Returns true if the AI settings are the same. Does not compare names. */
	bool operator==(const AISettings& ai) const;
	/* Returns true if the AI settings are not the same. Does not compare names. */
	bool operator!=(const AISettings& ai) const;

	#pragma endregion
};

DECLARE_SMART_PTRS(AISettings);

/* The resource list for AIs. */
class AIList {

	//=========== MEMBERS ============
	#pragma region Tetrominos

	/* The list of AIs. */
	static std::vector<AISettings> ais;
	/* The loaded AI document. */
	static IniDocumentSPtr document;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Initializes the resources database. */
	//AIList();

	#pragma endregion
	//=========== ELEMENTS ===========
	#pragma region Elements

	/* Gets the full list of AIs. */
	static const std::vector<AISettings>& getAIs();
	/* Gets the AI at the specified index in the list. */
	static AISettings getAI(int index);
	/* Gets the number of AIs in the list. */
	static int getAICount();
	/* Gets the index of the AI in the list. */
	static int indexOfAI(AISettings ai);

	#pragma endregion
	//=========== CREATION ===========
	#pragma region Creation
private:
	/* Reads an AI section and adds it to the list. */
	static void addAI(IniSectionSPtr section);
public:
	/* Loads the list of AIs. */
	static void loadAIs();
	/* Saves the list of AIs.*/
	static void saveAIs();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
