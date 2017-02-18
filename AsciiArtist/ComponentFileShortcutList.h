/*=================================================================|
|* File:				ComponentFileShortcutList.h				  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/18/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_FILE_SHORTCUT_LIST_H
#define COMPONENT_FILE_SHORTCUT_LIST_H

#include "Stdafx.h"
#include "Geometry.h"
#include "AsciiImage.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "Component.h"
#include "ComponentScrollBar.h"

using namespace std;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Components;

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
class ComponentFileShortcutList : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Visual
	/* The list of files in the directory. */
	File* _fileList;
	/* The length of the file list. */
	int _fileListLength;
	
	/* The index of the selected list item. */
	int _selection;
	/* The current item being hovered over. */
	int _hoverIndex;
	/* True if the selection was clicked. */
	bool _selectionClicked;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentFileShortcutList();
	/* Cleans up the component. */
	~ComponentFileShortcutList();
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
	
	/* Gets the current file selection. */
	File& GetFileSelection() const;
	/* Sets the list of file shortcuts. */
	void SetFileList(File* fileList, int length);

	#pragma endregion
	//--------------------------------
	#pragma region Events

	/* Returns true if the selection was clicked. */
	bool IsSelectionClicked() const;

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
#endif /* COMPONENT_FILE_SHORTCUT_LIST_H */
//=================================================================|
