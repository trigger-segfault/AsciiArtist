/*=================================================================|
|* File:				UndoAction.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/16/2014								  *|
|*																  *|
|* Description: A structure for storing undo actions.			  *|
|=================================================================*/

#pragma once
#ifndef UNDO_ACTION_H
#define UNDO_ACTION_H

#include "stdafx.h"
#include "AsciiImage.h"
#include "Component.h"
#include "ComponentCanvas.h"

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

/* The list of different actions available. */
enum class Actions : short {
	None,
	Draw,
	Resize,
	ResizeAll,
	Translate,
	TranslateAll,
	FormatChange,
	AnimationSpeedChange,
	AddFrame,
	DuplicateFrame,
	DeleteFrame,
	MoveFrameDown,
	MoveFrameUp
};

#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* A structure for storing undo actions. */
class UndoAction {

	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The name of the action. */
	string _actionName;
	/* The type of action. */
	Actions _action;
	/* The index of modified frame. */
	int _modifiedFrame;
	/* The before ascii image. */
	AsciiAnimation* _beforeImage;
	/* The result ascii image. */
	AsciiAnimation* _resultImage;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default undo action. */
	UndoAction();
	/* Constructs the undo action. */
	UndoAction(const string& actionName, Actions action, int modifiedFrame);
	/* Constructs the undo action. */
	UndoAction(const string& actionName, Actions action, int modifiedFrame, AsciiAnimation* before, AsciiAnimation* result);
	/* Cleans up the undo action. */
	~UndoAction();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Information
	
	/* Gets the name of the undo action. */
	string GetActionName() const;
	/* Sets the name of the undo action. */
	void SetActionName(const string& actionName);

	/* Gets the type of the undo action. */
	Actions GetAction() const;
	/* Sets the type of the undo action. */
	void SetAction(Actions action);

	/* Gets the modified frame. */
	int GetModifiedFrame() const;
	/* Sets the modified frame. */
	void SetModifiedFrame(int modifiedFrame);

	/* Gets the before ascii image. */
	AsciiAnimation* GetBeforeImage() const;
	/* Sets the before ascii image. */
	void SetBeforeImage(AsciiAnimation* beforeImage);

	/* Gets the result ascii image. */
	AsciiAnimation* GetResultImage() const;
	/* Sets the result ascii image. */
	void SetResultImage(AsciiAnimation* resultImage);

	#pragma endregion
	//--------------------------------
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace AsciiArtistApp */
} /* namespace PowerConsole */
#endif /* UNDO_ACTION_H */
//=================================================================|