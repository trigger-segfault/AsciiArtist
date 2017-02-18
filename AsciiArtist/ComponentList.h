/*=================================================================|
|* File:				ComponentList.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/10/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

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
/* The list of components in the library. */
namespace Components {
	
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations


#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* A class to manage the state of a control. */
class ComponentList : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Visual
	
	/* The list entries in the component. */
	vector<string> _listEntries;

	/* The index of the selected list item. */
	int _selection;
	/* The current item being hovered over. */
	int _hoverIndex;
	/* True if a selection is required. */
	bool _requiresSelection;
	/* True if the selection was changed. */
	bool _selectionChanged;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentList();
	/* Cleans up the component. */
	~ComponentList();
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
	#pragma region Visual
	
	/* Returns true if an item is required to be selected. */
	bool IsSelectionRequired() const;
	/* Sets if an item is required to be selected. */
	void SetSelectionRequired(bool requiresSelection);
	/* Gets the currently selected item. */
	int GetSelection() const;
	/* Sets the currently selected item. */
	void SetSelection(int selection);
	
	/* Adds the specified entry to the list. */
	void AddEntry(const string& entry);

	#pragma endregion
	//--------------------------------
	#pragma region Events

	/* Returns true if the selection was changed. */
	bool IsSelectionChanged() const;

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
#endif /* COMPONENT_LIST_H */
//=================================================================|
