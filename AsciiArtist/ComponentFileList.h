/*=================================================================|
|* File:				ComponentFileList.h						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/17/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_FILE_LIST_H
#define COMPONENT_FILE_LIST_H

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
class ComponentFileList : public Component {
	
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
	/* The list of ascii images in the directory. */
	AsciiImageHeader* _imageList;
	/* The length of the image list. */
	int _imageListLength;
	
	/* The scrollbar for the file list. */
	ComponentScrollBar* _scrollBar;
	
	/* The index of the selected list item. */
	int _selection;
	/* The current item being hovered over. */
	int _hoverIndex;
	/* True if the selection was changed. */
	bool _selectionChanged;
	/* True if the selection was clicked. */
	bool _selectionClicked;
	/* True if the selection was double clicked. */
	bool _selectionDoubleClicked;
	
	/* The file dialog image mode for which files to look for. */
	int _imageMode;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentFileList();
	/* Cleans up the component. */
	~ComponentFileList();
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
	
	/* Gets the current selection. */
	int GetSelection() const;
	/* Sets the scrollbar for the file list. */
	void SetScrollBar(ComponentScrollBar* scrollBar);
	/* Sets the list of files in the directory. */
	void SetFileList(File* fileList, int length);
	/* Sets the list of ascii images in the directory. */
	void SetImageList(AsciiImageHeader* imageList, int length);
	/* Gets the file dialog image mode. */
	int GetImageMode() const;
	/* Sets the file dialog image mode. */
	void SetImageMode(int mode);

	#pragma endregion
	//--------------------------------
	#pragma region Events

	/* Returns true if the selection was changed. */
	bool IsSelectionChanged() const;
	/* Returns true if the selection was clicked. */
	bool IsSelectionClicked() const;
	/* Returns true if the selection was double clicked. */
	bool IsSelectionDoubleClicked() const;

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
#endif /* COMPONENT_FILE_LIST_H */
//=================================================================|
