/*=================================================================|
|* File:				ComponentImportFileDialog.h				  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/24/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_IMPORT_FILE_DIALOG_H
#define COMPONENT_IMPORT_FILE_DIALOG_H

#include "Stdafx.h"
#include "Geometry.h"
#include "AsciiImage.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "ComponentWindow.h"
#include "ComponentTextBox.h"
#include "ComponentButton.h"

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
class ComponentImportFileDialog : public ComponentWindow {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The window background. */
	AsciiImage* _backgroundImage;

	/* The image width text box. */
	ComponentTextBox* _widthTextBox;
	/* The image height text box. */
	ComponentTextBox* _heightTextBox;
	/* The start position text box. */
	ComponentTextBox* _startPositionTextBox;
	/* The character spacing text box. */
	ComponentTextBox* _charSpacingTextBox;
	/* The color spacing text box. */
	ComponentTextBox* _colorSpacingTextBox;
	/* The read char toggle button. */
	ComponentButton* _readCharButton;
	/* The read color toggle button. */
	ComponentButton* _readColorButton;

	/* The import button. */
	ComponentButton* _importButton;
	/* The cancel button. */
	ComponentButton* _cancelButton;
	
	/* The size of the image. */
	Coord _imageSize;
	/* The starting position in the file. */
	unsigned int _startPosition;
	/* The number of skipped bytes after each character. */
	unsigned int _charSpacing;
	/* The number of skipped bytes after each color. */
	unsigned int _colorSpacing;
	/* True if characters are imported. */
	bool _charImported;
	/* True if colors are imported. */
	bool _colorImported;

	/* True if the file will be imported. */
	bool _imported;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component window. */
	ComponentImportFileDialog();
	/* Cleans up the component window. */
	~ComponentImportFileDialog();
	/* Initializes the component window. */
	virtual void Initialize(ConsoleApp* app, Frame* frame);
	/* Uninitailizes the component window. */
	virtual void Uninitialize();
	/* Called when the frame containing the component window becomes active. */
	virtual void OnEnter();
	/* Called when the frame containing the component window becomes inactive. */
	virtual void OnLeave();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Containment
	
	/* Sets the background image. */
	void SetBackgroundImage(AsciiImage* backgroundImage);
	
	/* Gets the size of the image. */
	Coord GetImageSize() const;
	/* Sets the size of the image. */
	void SetImageSize(Coord size);
	/* Gets the starting position in the file. */
	unsigned int GetStartPosition() const;
	/* Sets the starting position in the file. */
	void SetStartPosition(unsigned int position);
	/* Gets the number of skipped bytes after each character. */
	unsigned int GetCharSpacing() const;
	/* Sets the number of skipped bytes after each character. */
	void SetCharSpacing(unsigned int spacing);
	/* Gets the number of skipped bytes after each color. */
	unsigned int GetColorSpacing() const;
	/* Sets the number of skipped bytes after each color. */
	void SetColorSpacing(unsigned int spacing);
	/* Returns true if characters are imported. */
	bool IsCharImported() const;
	/* Sets if characters are imported. */
	void SetCharImported(bool imported);
	/* Returns true if colors are imported. */
	bool IsColorImported() const;
	/* Sets if colors are imported. */
	void SetColorImported(bool imported);

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the import button was pressed. */
	bool IsImported() const;
	/* Returns true if the cancel button was pressed. */
	bool IsCanceled() const;

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
#endif /* COMPONENT_IMPORT_FILE_DIALOG_H */
//=================================================================|
