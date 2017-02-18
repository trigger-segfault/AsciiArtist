/*=================================================================|
|* File:				ToolSelect.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/15/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#pragma once
#ifndef TOOL_SELECT_H
#define TOOL_SELECT_H

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
class ToolSelect : public Tool {

	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Selection
	/* True if something has been selected. */
	bool _hasSelection;
	/* The currently selected image. */
	AsciiImage* _selection;
	
	// Dragging
	/* The starting position of the selection. */
	Coord _startPoint;
	/* The drag position of the selection. */
	Coord _dragPoint;
	/* The grip position while dragging. */
	Coord _gripPoint;
	/* True if the selection is being dragged. */
	bool _dragging;
	/* True if the image was copied and shouldn't draw a shadow. */
	bool _copied;
	
	// Visual
	/* The timer for the selection box. */
	unsigned int _boxTimer;
	/* The state of the selection box. */
	bool _boxState;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default tool. */
	ToolSelect();
	/* Cleans up the tool. */
	~ToolSelect();
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
	/* Finalizes and copies the selection. */
	void FinalizeCopy();
	/* Finalizes and deletes the selection. */
	void FinalizeDelete();
	/* Cancels the tool action. */
	virtual void Cancel();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Called every step to draw the tool to the canvas. */
	virtual void Draw(AsciiImageBase& g);
	
	#pragma endregion
	//=========== ACTIONS ============
	#pragma region Actions
	
	/* Copies the current selection. */
	void Copy();
	/* Cuts the current selection. */
	void Cut();
	/* Delete the current selection. */
	void Delete();
	/* Pastes the current clipboard ascii image. */
	void Paste();
	/* Selects the entire image. */
	void SelectAll();
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace AsciiArtistApp */
} /* namespace PowerConsole */
#endif /* TOOL_SELECT_H */
//=================================================================|