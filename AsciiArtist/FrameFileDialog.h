/*=================================================================|
|* File:				FrameFileDialog.h						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/18/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#pragma once
#ifndef FRAME_FILE_DIALOG_H
#define FRAME_FILE_DIALOG_H

#include "Stdafx.h"
#include "AsciiImage.h"
#include "Frame.h"
#include "ComponentButton.h"
#include "ComponentList.h"
#include "ComponentScrollBar.h"
#include "ComponentCanvas.h"
#include "ComponentTextBox.h"
#include "ComponentFileList.h"
#include "ComponentFileShortcutList.h"
#include "ComponentMessageBox.h"
#include "AsciiArtistManager.h"

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

//=================================================================|
// PREDEFINITIONS												   |
//=================================================================/

class ConsoleApp;
class Component;

//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations


#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The main base class that runs the current screen in the console. */
class FrameFileDialog : public Frame {

	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	//--------------------------------
	#pragma region Components

	ComponentButton* _saveButton;
	ComponentButton* _cancelButton;

	ComponentTextBox* _fileNameTextBox;

	ComponentScrollBar* _fileListScrollBar;
	ComponentFileList* _fileList;

	ComponentFileShortcutList* _commonLocationsList;
	ComponentFileShortcutList* _computerLocationsList;
	ComponentFileShortcutList* _recentFilesList;

	#pragma endregion
	//--------------------------------
	#pragma region Ascii Images

	// Images
	AsciiImage* _uiFileDialogImage;
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

	/* The current browser directory. */
	Directory _directory;

	/* The list of ascii images in the directory. */
	vector<AsciiImageHeader> _imageList;
	
	/* The list of common locations. */
	vector<File> _commonList;
	/* The list of computer locations. */
	vector<File> _computerList;
	/* The list of recent files. */
	vector<File> _recentList;

	/* The return file from the file dialog. */
	File _returnFile;
	/* True if a file is being saved and not opened. */
	bool _saving;
	/* True if the operation was canceled. */
	bool _canceled;
	/* True if the cursor is hovering over the file path. */
	bool _pathHover;
	
	/* The file dialog image mode for which files to look for. */
	int _imageMode;
	
	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default frame. */
	FrameFileDialog(AsciiArtistManager* aa);
	/* Cleans up the frame. */
	~FrameFileDialog();
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

	/* Gets the return file from the operation. */
	File GetReturnFile() const;
	/* Returns true if the dialog is saving a file. */
	bool IsSaving() const;
	/* Sets if the dialog is saving a file. */
	void SetSaving(bool saving);
	/* Sets the current file name. */
	void SetFileName(const string& fileName);
	/* Gets the file dialog image mode. */
	int GetImageMode() const;
	/* Sets the file dialog image mode. */
	void SetImageMode(int mode);

	#pragma endregion
	//--------------------------------
	#pragma region Events

	/* Returns true if the operation was canceled. */
	bool IsCanceled() const;
	/* Returns true if the operation was successful. */
	bool IsSuccessful() const;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the frame. */
	virtual void Update();

	void UpdateDimensions();
	void UpdateResizing();
	void LoadDirectory(File file);
	void Close();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Called every step to draw the frame to the window. */
	virtual void Draw(AsciiImageBase& g);
	
	#pragma endregion
	//========== COMPONENTS ==========
	#pragma region Components
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace PowerConsole */
#endif /* FRAME_FILE_DIALOG_H */
//=================================================================|