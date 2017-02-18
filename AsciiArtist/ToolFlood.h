/*=================================================================|
|* File:				ToolFlood.h								  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/15/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#pragma once
#ifndef TOOL_FLOOD_H
#define TOOL_FLOOD_H

#include "stdafx.h"
#include "AsciiImage.h"
#include "Component.h"
#include "Tool.h"

using namespace std;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::AsciiArtistApp;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of Windows orientated classes and wrappers. */
namespace AsciiArtistApp {

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The main base class that runs the current screen in the console. */
class ToolFlood : public Tool {

	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	
	/* True if the secondary character is being drawn with. */
	bool _secondary;
	/* The starting position of the flood fill. */
	Coord _startPoint;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default tool. */
	ToolFlood();
	/* Cleans up the tool. */
	~ToolFlood();
	/* Initializes the tool. */
	virtual void Initialize(AsciiArtistManager* aa, ConsoleApp* app, AsciiAnimation* drawingBuffer);
	/* Uninitializes the tool. */
	virtual void Uninitialize();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Information
	

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the tool. */
	virtual void Update();
	
	/* Activates the tool. */
	virtual void Activate();
	/* Finalizes the tool action. */
	virtual void Finalize();
	/* Cancels the tool action. */
	virtual void Cancel();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Called every step to draw the tool to the canvas. */
	virtual void Draw(AsciiImageBase& g);
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace AsciiArtistApp */
} /* namespace PowerConsole */
#endif /* TOOL_FLOOD_H */
//=================================================================|