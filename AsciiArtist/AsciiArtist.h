/*=================================================================|
|* File:				AsciiArtist.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/13/2013								  *|
|*																  *|
|* Description: Decleration of the Keyboard listener class.		  *|
|=================================================================*/

#pragma once
#ifndef ASCII_ARTIST_H
#define ASCII_ARTIST_H

#include "stdafx.h"
#include "AsciiImage.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "FileManager.h"

using namespace std;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Windows;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of Windows orientated classes and wrappers. */
namespace AsciiArtistApp {

//=================================================================|
// PREDEFINITIONS												   |
//=================================================================/

class AsciiArtistManager;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The main class that runs Ascii Artist. */
class AsciiArtist : public ConsoleApp {

	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Containment
	AsciiArtistManager* _aa;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default console app. */
	AsciiArtist(int argc, char** argv);
	/* Cleans up the console app. */
	~AsciiArtist();
	/* Initializes the console app. */
	virtual void Initialize();
	/* Uninitializes the console app. */
	virtual void Uninitialize();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Containment
	
	#pragma endregion
	//--------------------------------
	#pragma region Drawing
	
	#pragma endregion
	//--------------------------------
	#pragma region Settings
	
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the console app. */
	virtual void Update();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Called every step to draw the console app. */
	virtual void Draw();
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace AsciiArtistApp */
} /* namespace PowerConsole */
#endif /* ASCII_ARTIST_H */
//=================================================================|