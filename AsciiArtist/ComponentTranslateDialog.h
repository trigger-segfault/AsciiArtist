/*=================================================================|
|* File:				ComponentTranslateDialog.h				  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/16/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_TRANSLATE_DIALOG_H
#define COMPONENT_TRANSLATE_DIALOG_H

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
class ComponentTranslateDialog : public ComponentWindow {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The resize window background. */
	AsciiImage* _backgroundImage;

	/* The image x text box. */
	ComponentTextBox* _xTextBox;
	/* The image y text box. */
	ComponentTextBox* _yTextBox;
	/* The translate button. */
	ComponentButton* _translateButton;
	/* The cancel button. */
	ComponentButton* _cancelButton;
	/* The translate all toggle button. */
	ComponentButton* _translateAllButton;

	/* The translate distance. */
	Coord _distance;

	/* True if all frames should be translated. */
	bool _translateAll;

	/* True if the translate button was pressed and not the cancel button. */
	bool _translated;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component window. */
	ComponentTranslateDialog();
	/* Cleans up the component window. */
	~ComponentTranslateDialog();
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
	/* Gets the new image size. */
	Coord GetDistance() const;

	/* Returns true if all frames are translated. */
	bool IsTranslateAllSelected() const;

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the translate button was pressed. */
	bool IsTranslated() const;
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
#endif /* COMPONENT_TRANSLATE_DIALOG_H */
//=================================================================|
