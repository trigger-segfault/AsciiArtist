/*=================================================================|
|* File:				ComponentImageSettingsDialog.h			  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/20/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_IMAGE_SETTINGS_DIALOG_H
#define COMPONENT_IMAGE_SETTINGS_DIALOG_H

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
class ComponentImageSettingsDialog : public ComponentWindow {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The window background. */
	AsciiImage* _backgroundImage;

	/* The attributes format toggle button. */
	ComponentButton* _attributesButton;
	/* The animation format toggle button. */
	ComponentButton* _animationButton;
	/* The multisize format toggle button. */
	ComponentButton* _multisizeButton;
	/* The layered format toggle button. */
	ComponentButton* _layeredButton;
	
	/* The v2 file version toggle button. */
	ComponentButton* _version2Button;
	/* The v3 file version toggle button. */
	ComponentButton* _version3Button;
	/* The v3 small file version toggle button. */
	ComponentButton* _version3SmallButton;

	/* The animation speed text box. */
	ComponentTextBox* _animSpeedTextBox;

	/* The save button. */
	ComponentButton* _saveButton;
	/* The cancel button. */
	ComponentButton* _cancelButton;
	
	/* The image format. */
	unsigned short _format;
	/* The image file version. */
	int _version;
	/* The background pixel. */
	Pixel _background;
	/* The animation speed. */
	unsigned int _animSpeed;
	/* The primary pixel. */
	Pixel _primaryPixel;
	/* The secondary pixel. */
	Pixel _secondaryPixel;
	
	/* True if the settings were saved. */
	bool _saved;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component window. */
	ComponentImageSettingsDialog();
	/* Cleans up the component window. */
	~ComponentImageSettingsDialog();
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
	/* Gets the new background pixel. */
	Pixel GetBackground() const;
	/* Sets the current background pixel. */
	void SetBackground(Pixel background);
	/* Sets the current primary pixel. */
	void SetPrimaryPixel(Pixel pixel);
	/* Sets the current secondary pixel. */
	void SetSecondaryPixel(Pixel pixel);
	/* Gets the new format. */
	unsigned short GetFormat() const;
	/* Sets the current format. */
	void SetFormat(unsigned short format);
	/* Gets the new file version. */
	int GetVersion() const;
	/* Sets the current file version. */
	void SetVersion(int version);

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the save button was pressed. */
	bool IsSaved() const;
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
#endif /* COMPONENT_IMAGE_SETTINGS_DIALOG_H */
//=================================================================|
