/*=================================================================|
|* File:				AsciiArtistManager.h					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/14/2013								  *|
|*																  *|
|* Description: Decleration of the Keyboard listener class.		  *|
|=================================================================*/

#pragma once
#ifndef ASCII_ARTIST_MANAGER_H
#define ASCII_ARTIST_MANAGER_H

#include "stdafx.h"
#include "AsciiImage.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "FileManager.h"
#include "Tool.h"
#include "ToolPen.h"
#include "ToolLine.h"
#include "ToolSquare.h"
#include "ToolCircle.h"
#include "ToolFlood.h"
#include "ToolReplace.h"
#include "ToolText.h"
#include "ToolSelect.h"
#include "UndoAction.h"
#include "ComponentCanvas.h"
#include "ComponentStatusBar.h"

using namespace std;
using namespace PowerConsole::Components;
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

class AsciiArtist;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The main class that runs Ascii Artist. */
class AsciiArtistManager {

	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	//--------------------------------
	#pragma region Containment

	/* The current image loaded into the program. */
	AsciiArtist* _aaApp;
	/* A link to the drawing canvas. */
	ComponentCanvas* _canvas;
	/* A link to the status bar. */
	ComponentStatusBar* _statusBar;
	
	#pragma endregion
	//--------------------------------
	#pragma region Image
	
	/* The file for the current image. */
	File _imageFile;
	/* True if the image file is untitled. */
	bool _untitled;
	/* The version to save the image as. */
	int _version;
	/* The current image loaded into the program. */
	AsciiAnimation* _image;
	/* The buffer used for tool drawing. */
	AsciiAnimation* _drawingBuffer;
	/* The current frame being drawn to. */
	int _currentFrame;
	/* True if the image has been modified since last saving. */
	bool _saved;
	/* True if the image is animating. */
	bool _animating;
	/* The timer used for animting the image. */
	unsigned int _animationTimer;
	/* True if there was an error while loading the program and the selected image. */
	bool _loadError;
	
	#pragma endregion
	//--------------------------------
	#pragma region Drawing

	/* The primary drawing pixel. */
	Pixel _primaryPixel;
	/* The secondary drawing pixel. */
	Pixel _secondaryPixel;
	
	#pragma endregion
	//--------------------------------
	#pragma region Undos

	/* True if undos are enabled. */
	bool _undoEnabled;
	/* The maximum number of undos allowed. */
	int _maxUndos;
	/* The index of the current undo. */
	int _currentUndo;
	/* The list of undo actions. */
	vector<UndoAction*> _undoList;
	
	#pragma endregion
	//--------------------------------
	#pragma region Tools
	
	/* The current tool. */
	Tool* _currentTool;
	/* The pen tool. */
	ToolPen* _toolPen;
	/* The line tool. */
	ToolLine* _toolLine;
	/* The square tool. */
	ToolSquare* _toolSquare;
	/* The circle tool. */
	ToolCircle* _toolCircle;
	/* The flood fill tool. */
	ToolFlood* _toolFlood;
	/* The replace tool. */
	ToolReplace* _toolReplace;
	/* The text fill tool. */
	ToolText* _toolText;
	/* The select tool. */
	ToolSelect* _toolSelect;

	#pragma endregion
	//--------------------------------
	#pragma region Settings

	// Settings
	/* True if only the shape outline is drawn. */
	bool _outline;
	/* True if pixel replacement is specific. */
	bool _specific;
	/* True if the cursor highlights on the canvas. */
	bool _showCursor;
	/* True if the character should be drawn. */
	bool _charFilter;
	/* True if the foreground color should be drawn. */
	bool _fColorFilter;
	/* True if the background color should be drawn. */
	bool _bColorFilter;
	/* True if the debug information should be drawn. */
	bool _debugMode;
	
	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the ascii artist manager. */
	AsciiArtistManager();
	/* Cleans up the ascii artist manager. */
	~AsciiArtistManager();
	/* Initializes the ascii artist manager. */
	void Initialize(AsciiArtist* aaApp);
	/* Uninitializes the ascii artist manager. */
	void Uninitialize();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Containment
	
	/* Gets the drawing canvas. */
	ComponentCanvas* GetCanvas() const;
	/* Sets the drawing canvas. */
	void SetCanvas(ComponentCanvas* canvas);
	/* Gets the status bar. */
	ComponentStatusBar* GetStatusBar() const;
	/* Sets the status bar. */
	void SetStatusBar(ComponentStatusBar* statusBar);

	#pragma endregion
	//--------------------------------
	#pragma region Image
	
	/* Gets the current image file. */
	File GetImageFile() const;
	/* Returns true if the image file is untitled. */
	bool IsUntitled() const;
	/* Gets the version to save the image as. */
	int GetVersion() const;
	/* Sets the version to save the image as. */
	void SetVersion(int version);
	/* Gets the current image loaded into the program. */
	AsciiAnimation* GetImage() const;
	/* Gets the buffer used for tool drawing. */
	AsciiAnimation* GetDrawingBuffer() const;
	/* Gets the current frame being drawn to. */
	int GetCurrentFrame() const;
	/* Sets the current frame being drawn to. */
	void SetCurrentFrame(int index);
	/* Returns true if the image has been saved since the last modification. */
	bool IsSaved() const;
	/* Sets that the image has been modified since the last save. */
	void SetImageModified();
	/* Returns true if the image is animating. */
	bool IsAnimating() const;
	/* Sets it the image is animating. */
	void SetAnimating(bool animating);
	/* Gets the timer used for animations. */
	unsigned int GetAnimationTimer() const;
	/* Sets the timer used for animations. */
	void SetAnimationTimer(unsigned int animTimer);

	#pragma endregion
	//--------------------------------
	#pragma region Drawing
	
	/* Gets the primary drawing pixel. */
	Pixel GetPrimaryPixel() const;
	/* Sets the primary drawing pixel. */
	void SetPrimaryPixel(Pixel pixel);
	/* Gets the secondary drawing pixel. */
	Pixel GetSecondaryPixel() const;
	/* Sets the secondary drawing pixel. */
	void SetSecondaryPixel(Pixel pixel);

	#pragma endregion
	//--------------------------------
	#pragma region Undos
	
	/* Returns true if undos are enabled. */
	bool IsUndoEnabled() const;
	/* Sets if undos are enabled. */
	void SetUndoEnabled(bool enabled);
	/* Gets the maximum number of undos. */
	int GetMaxUndos() const;
	/* Sets the maximum number of undos. */
	void SetMaxUndos(int maxUndos);
	/* Gets the index of the current undo. */
	int GetCurrentUndo() const;
	/* Sets the index of the current undo. */
	void SetCurrentUndo(int currentUndo);
	/* Returns true if an undo action is available. */
	bool IsUndoAvailable() const;
	/* Returns true if a redo action is available. */
	bool IsRedoAvailable() const;

	#pragma endregion
	//--------------------------------
	#pragma region Tools
	
	/* Gets the current tool. */
	Tool* GetCurrentTool() const;
	/* Gets the current tool type. */
	ToolTypes GetCurrentToolType() const;
	/* Sets the current tool. */
	void SetCurrentTool(ToolTypes tool);
	
	/* Gets the pen tool. */
	ToolPen* GetPenTool() const;
	/* Gets the line tool. */
	ToolLine* GetLineTool() const;
	/* Gets the square tool. */
	ToolSquare* GetSquareTool() const;
	/* Gets the circle tool. */
	ToolCircle* GetCircleTool() const;
	/* Gets the flood fill tool. */
	ToolFlood* GetFloodTool() const;
	/* Gets the replace tool. */
	ToolReplace* GetReplaceTool() const;
	/* Gets the text tool. */
	ToolText* GetTextTool() const;
	/* Gets the select tool. */
	ToolSelect* GetSelectTool() const;

	#pragma endregion
	//--------------------------------
	#pragma region Settings
	
	/* Returns true if only the shape outline is drawn. */
	bool IsShapeOutline() const;
	/* Sets if only the shape outline is drawn. */
	void SetShapeOutline(bool outline);
	/* Returns true if pixel replacement is specific. */
	bool IsPixelSpecific() const;
	/* Sets if pixel replacement is specific. */
	void SetPixelSpecific(bool specific);
	/* Returns true if the cursor highlights on the canvas. */
	bool IsCursorVisible() const;
	/* Sets if the cursor highlights on the canvas. */
	void SetCursorVisible(bool visible);
	/* Returns true if the character should be drawn. */
	bool IsCharacterDrawn() const;
	/* Sets if the character should be drawn. */
	void SetCharacterDrawn(bool characterFilter);
	/* Returns true if the foreground color should be drawn. */
	bool IsFColorDrawn() const;
	/* Sets if the foreground color should be drawn. */
	void SetFColorDrawn(bool fColorFilter);
	/* Returns true if the background color should be drawn. */
	bool IsBColorDrawn() const;
	/* Sets if the background color should be drawn. */
	void SetBColorDrawn(bool bColorFilter);
	/* Returns true if the program is in debug mode. */
	bool IsDebugMode() const;
	/* Sets if the program is in debug mode. */
	void SetDebugMode(bool enabled);

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating
	
	/* Updates the image animation. */
	void UpdateAnimation();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	
	
	#pragma endregion
	//============ SAVING ============
	#pragma region Saving
	
	/* Creates a new image. */
	void New();
	/* Saves the image as the current file. */
	bool Save();
	/* Saves the image as the specified file. */
	bool Save(File imageFile);
	/* Opens the specified image file. */
	bool Open(File imageFile);
	/* Imports the specified file as an image. */
	bool ImportFile(File imageFile, Coord size, unsigned int startPosition, unsigned int charSpacing, unsigned int colorSpacing, bool importChar, bool importColor);
	
	#pragma endregion
	//=========== MESSAGES ===========
	#pragma region Messages

	/* Sets the status bar message text. */
	void SetMessage(const string& message);
	/* Sets the status bar urgent message text. */
	void SetUrgentMessage(const string& message);
	
	#pragma endregion
	//============ UNDOS =============
	#pragma region Undos
	
	/* Undos the last action. */
	void Undo();
	/* Redos the last action. */
	void Redo();
	/* Adds the undo action to the list of undos. */
	void AddUndo(UndoAction* undoAction);
	
	#pragma endregion
	//============ FORMAT ============
	#pragma region Format
	
	/* Sets the format of the image. */
	void SetImageFormat(unsigned short format, unsigned int speed, Pixel background);
	/* Sets the animation speed of the image. */
	void SetAnimationSpeed(unsigned int speed);
	
	#pragma endregion
	//============ TOOLS =============
	#pragma region Tools
	
	/* Resizes the image. */
	void Resize(Coord newSize, bool allFrames);
	/* Translates the image. */
	void Translate(Coord distance, bool allFrames);
	/* Copies the current selection. */
	void Copy();
	/* Cuts the current selection. */
	void Cut();
	/* Deletes the current selection. */
	void Delete();
	/* Pastes the current ascii image on the clipboard. */
	void Paste();
	/* Selects the entire image. */
	void SelectAll();
	
	#pragma endregion
	//============ FRAMES ============
	#pragma region Frames
	
	/* Adds a new frame. */
	void AddFrame();
	/* Duplicates the current frame. */
	void DuplicateFrame();
	/* Deletes the current frame. */
	void DeleteFrame();
	/* Moves the current frame down in the list. */
	void MoveFrameDown();
	/* Moves the current frame up in the list. */
	void MoveFrameUp();
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace AsciiArtistApp */
} /* namespace PowerConsole */
#endif /* ASCII_ARTIST_MANAGER_H */
//=================================================================|