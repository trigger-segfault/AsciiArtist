/*=================================================================|
|* File:				ComponentCharacterGrid.h				  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/10/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_CHAR_GRID_H
#define COMPONENT_CHAR_GRID_H

#include "Stdafx.h"
#include "Geometry.h"
#include "AsciiImage.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "Component.h"

using namespace std;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {

//=================================================================|
// PREDEFINITIONS												   |
//=================================================================/

namespace AsciiArtistApp {
	class AsciiArtistManager;
}
using namespace AsciiArtistApp;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The list of components in the library. */
namespace Components {

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* A class to manage the state of a control. */
class ComponentCharacterGrid : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Visual
	/* The ascii artist manager. */
	AsciiArtistManager* _aa;
	/* The character grid image. */
	AsciiImage* _charGridImage;
	
	/* The locations of each character in the grid. */
	Coord* _charList;
	
	/* The primary selected character. */
	Pixel  _primaryCharacter;
	/* The secondary selected character. */
	Pixel _secondaryCharacter;
	/* The primary selected character position. */
	Coord _primarySelection;
	/* The secondary selected character position. */
	Coord _secondarySelection;
	/* True if the primary character was changed. */
	bool _primaryChanged;
	/* True if the secondary character was changed. */
	bool _secondaryChanged;
	
	/* The position of the character being hovered over. */
	Coord _hoverPosition;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentCharacterGrid();
	/* Cleans up the component. */
	~ComponentCharacterGrid();
	/* Initializes the component. */
	virtual void Initialize(ConsoleApp* app, Frame* frame);
	/* Uninitailizes the component. */
	virtual void Uninitialize();
	/* Called when the frame containing the component becomes active. */
	virtual void OnEnter();
	/* Called when the frame containing the component becomes inactive. */
	virtual void OnLeave();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Containment
	
	/* Sets the link to the ascii artist manager. */
	void SetAsciiArtistManager(AsciiArtistManager* aa);

	#pragma endregion
	//--------------------------------
	#pragma region Visual
	
	/* Gets the image used for the character grid. */
	AsciiImage* GetCharacterGridImage() const;
	/* Sets the image used for the character grid. */
	void SetCharacterGridImage(AsciiImage* image);
	
	/* Gets the primary selected character. */
	Pixel GetPrimaryCharacter() const;
	/* Sets the primary selected character. */
	void SetPrimaryCharacter(Pixel character);
	/* Gets the secondary selected character. */
	Pixel GetSecondaryCharacter() const;
	/* Sets the secondary selected character. */
	void SetSecondaryCharacter(Pixel character);

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the primary character has been changed. */
	bool IsPrimaryCharacterChanged() const;
	/* Returns true if the secondary character has been changed. */
	bool IsSecondaryCharacterChanged() const;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the component. */
	virtual void Update();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Called every step to draw the component to the screen. */
	virtual void Draw(AsciiImageBase& g);
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace Components */
} /* namespace PowerConsole */
#endif /* COMPONENT_CHAR_GRID_H */
//=================================================================|
