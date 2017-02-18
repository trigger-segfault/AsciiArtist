/*=================================================================|
|* File:				Tool.h									  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/13/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#pragma once
#ifndef TOOL_H
#define TOOL_H

#include "stdafx.h"
#include "AsciiImage.h"
#include "Component.h"
#include "ComponentCanvas.h"
#include "UndoAction.h"

using namespace std;
using namespace PowerConsole::Components;
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

class ConsoleApp;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* A collection of Windows orientated classes and wrappers. */
namespace AsciiArtistApp {
	
//=================================================================|
// PREDEFINITIONS												   |
//=================================================================/

class AsciiArtistManager;

//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* The list of different canvas tools available. */
enum class ToolTypes : short {
	Pen,
	Line,
	Square,
	Circle,
	Flood,
	Replace,
	Text,
	Select,
	Eyedropper
};

#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The main base class that runs the current screen in the console. */
class Tool {

	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Containment
	/* The ascii artist manager. */
	AsciiArtistManager* _aa;
	/* The ascii artist app. */
	ConsoleApp* _app;

	// Information
	/* The type of the tool. */
	ToolTypes _type;
	/* The name of the tool. */
	string _name;
	
	// Drawing
	/* The buffer used for drawing too. */
	AsciiAnimation* _drawingBuffer;
	/* True if the tool is currently drawing. */
	bool _drawing;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default tool. */
	Tool(ToolTypes type, const string& name);
	/* Cleans up the tool. */
	~Tool();
	/* Initializes the tool. */
	virtual void Initialize(AsciiArtistManager* aa, ConsoleApp* app, AsciiAnimation* drawingBuffer);
	/* Uninitializes the tool. */
	virtual void Uninitialize();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Information
	
	/* Gets the type of the tool. */
	ToolTypes GetToolType() const;
	/* Gets the name of the tool. */
	string GetName() const;
	/* Returns true if the tool is drawing. */
	bool IsDrawing() const;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the tool. */
	virtual void Update() = 0;
	
	/* Activates the tool. */
	virtual void Activate() = 0;
	/* Finalizes the tool action. */
	virtual void Finalize() = 0;
	/* Cancels the tool action. */
	virtual void Cancel() = 0;
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Called every step to draw the tool to the canvas. */
	virtual void Draw(AsciiImageBase& g) = 0;
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace AsciiArtistApp */
} /* namespace PowerConsole */
#endif /* TOOL_H */
//=================================================================|