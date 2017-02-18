/*=================================================================|
|* File:				ComponentAnimationSpeedDialog.h			  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/19/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_ANIM_SPEED_DIALOG_H
#define COMPONENT_ANIM_SPEED_DIALOG_H

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
class ComponentAnimationSpeedDialog : public ComponentWindow {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The window background. */
	AsciiImage* _backgroundImage;

	/* The animation speed text box. */
	ComponentTextBox* _animSpeedTextBox;
	/* The set button. */
	ComponentButton* _setButton;
	/* The cancel button. */
	ComponentButton* _cancelButton;

	/* The animation speed. */
	unsigned int _animSpeed;

	/* True if the resize button was pressed and not the cancel button. */
	bool _speedSet;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component window. */
	ComponentAnimationSpeedDialog();
	/* Cleans up the component window. */
	~ComponentAnimationSpeedDialog();
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
	/* Gets the new animation speed. */
	unsigned int GetAnimationSpeed() const;
	/* Sets the current animation speed. */
	void SetAnimationSpeed(unsigned int speed);

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the set button was pressed. */
	bool IsSpeedSet() const;
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
#endif /* COMPONENT_ANIM_SPEED_DIALOG_H */
//=================================================================|
