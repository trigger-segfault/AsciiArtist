/*=================================================================|
|* File:				FrameCanvas.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#pragma once
#ifndef FRAME_CANVAS_H
#define FRAME_CANVAS_H

#include "Stdafx.h"
#include "AsciiImage.h"
#include "Frame.h"
#include "ComponentButton.h"
#include "ComponentList.h"
#include "ComponentCharacterGrid.h"
#include "ComponentColorPalette.h"
#include "ComponentCharacterDisplay.h"
#include "ComponentScrollBar.h"
#include "ComponentCanvas.h"
#include "ComponentStatusBar.h"
#include "ComponentFrameList.h"
#include "ComponentMessageBox.h"
#include "ComponentResizeDialog.h"
#include "ComponentTranslateDialog.h"
#include "ComponentAnimationSpeedDialog.h"
#include "ComponentImageSettingsDialog.h"
#include "ComponentImportFileDialog.h"
#include "AsciiArtistManager.h"

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
class Component;

//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* The list of different canvas tools available. */
enum class CanvasTools : short {
	Pen,
	Line,
	Square,
	Flood,
	Replace,
	Text,
	Select
};

#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The main base class that runs the current screen in the console. */
class FrameCanvas : public Frame {

	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	//--------------------------------
	#pragma region Components

	ComponentButton* _newFileButton;
	ComponentButton* _openFileButton;
	ComponentButton* _saveFileButton;
	ComponentButton* _saveAsFileButton;
	ComponentButton* _copyButton;
	ComponentButton* _cutButton;
	ComponentButton* _deleteButton;
	ComponentButton* _pasteButton;
	ComponentButton* _selectAllButton;
	ComponentButton* _undoButton;
	ComponentButton* _redoButton;
	ComponentButton* _resizeButton;
	ComponentButton* _translateButton;
	ComponentButton* _saveBitmapButton;
	ComponentButton* _importFileButton;
	
	ComponentButton* _helpButton;
	ComponentButton* _settingsButton;

	ComponentButton* _newFrameButton;
	ComponentButton* _duplicateFrameButton;
	ComponentButton* _deleteFrameButton;
	ComponentButton* _moveFrameDownButton;
	ComponentButton* _moveFrameUpButton;
	ComponentButton* _animationSpeedButton;
	ComponentButton* _animateButton;

	ComponentButton* _charFilterButton;
	ComponentButton* _fColorFilterButton;
	ComponentButton* _bColorFilterButton;
	
	ComponentButton* _cursorButton;
	ComponentButton* _outlineButton;
	
	ComponentList* _toolList;

	ComponentColorPalette* _colorPalette;
	ComponentCharacterGrid* _characterGrid;
	ComponentCharacterDisplay* _characterDisplay;
	
	ComponentScrollBar* _frameListScrollBar;
	ComponentFrameList* _frameList;
	
	ComponentStatusBar* _statusBar;
	
	ComponentScrollBar* _canvasScrollBar;
	ComponentCanvas* _canvas;

	#pragma endregion
	//--------------------------------
	#pragma region Ascii Images

	// Images
	AsciiAnimation* _newFileImage;
	AsciiAnimation* _openFileImage;
	AsciiAnimation* _saveFileImage;
	AsciiAnimation* _saveAsFileImage;
	AsciiAnimation* _copyImage;
	AsciiAnimation* _cutImage;
	AsciiAnimation* _deleteImage;
	AsciiAnimation* _pasteImage;
	AsciiAnimation* _selectAllImage;
	AsciiAnimation* _undoImage;
	AsciiAnimation* _undoDisabledImage;
	AsciiAnimation* _redoImage;
	AsciiAnimation* _redoDisabledImage;
	AsciiAnimation* _resizeImage;
	AsciiAnimation* _translateImage;
	AsciiAnimation* _saveBitmapImage;
	AsciiAnimation* _importFileImage;

	AsciiAnimation* _helpImage;
	AsciiAnimation* _newFrameImage;
	AsciiAnimation* _duplicateFrameImage;
	AsciiAnimation* _deleteFrameImage;
	AsciiAnimation* _moveFrameDownImage;
	AsciiAnimation* _moveFrameUpImage;
	AsciiAnimation* _animationSpeedImage;

	AsciiImage* _colorPaletteImage;
	AsciiImage* _characterGridImage;
	AsciiImage* _uiMainImage;
	AsciiImage* _uiStatusBarImage;
	AsciiAnimation* _uiFramesTopImage;
	AsciiAnimation* _uiFramesBottomImage;

	AsciiImage* _resizeDialogImage;
	AsciiImage* _translateDialogImage;
	AsciiImage* _animationSpeedDialogImage;
	AsciiImage* _imageSettingsImage;
	AsciiImage* _importFileDialogImage;
	AsciiImage* _messageBoxImage;
	
	#pragma endregion
	//--------------------------------
	#pragma region Members
	
	/* The ascii artist manager. */
	AsciiArtistManager* _aa;

	/* True if the window is being resized. */
	bool _resizing;
	/* True if the window was resized. */
	bool _resized;
	/* True if the mouse is hovering over the resize corner. */
	bool _resizeHover;

	Coord _consoleSize;

	Coord _importSize;
	unsigned int _importStartPosition;
	unsigned int _importCharSpacing;
	unsigned int _importColorSpacing;
	bool _importChar;
	bool _importColor;
	/* The currently sellected tool. */
	//CanvasTools _currentTool;
	
	/* The primary drawing pixel. */
	//Pixel _primaryPixel;
	/* The secondary drawing pixel. */
	//Pixel _secondaryPixel;
	
	/* The link to the current ascii image. */
	//AsciiAnimation* _image;
	//int _currentFrame;
	
	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default frame. */
	FrameCanvas(AsciiArtistManager* aa);
	/* Cleans up the frame. */
	~FrameCanvas();
	/* Initializes the frame. */
	virtual void Initialize(ConsoleApp* app);
	/* Uninitailizes the frame. */
	virtual void Uninitialize();
	/* Called when the frame becomes active. */
	virtual void OnEnter();
	/* Called when the frame becomes inactive. */
	virtual void OnLeave();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Containment

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the frame. */
	virtual void Update();

	void UpdateDimensions();
	void UpdatePalette();
	void UpdateSaving();
	void UpdateTools();
	void UpdateSpecial();
	void UpdateFrames();
	void UpdateResizing();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Called every step to draw the frame to the window. */
	virtual void Draw(AsciiImageBase& g);
	
	#pragma endregion
	//========== COMPONENTS ==========
	#pragma region Components

	void AddFrame();
	void DuplicateFrame();
	void DeleteFrame();
	void MoveFrameDown();
	void MoveFrameUp();
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace PowerConsole */
#endif /* FRAME_CANVAS_H */
//=================================================================|