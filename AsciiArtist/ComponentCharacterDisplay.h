/*=================================================================|
|* File:				ComponentCharacterDisplay.h				  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/12/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_CHAR_DISPLAY_H
#define COMPONENT_CHAR_DISPLAY_H

#include "Stdafx.h"
#include "Geometry.h"
#include "AsciiImage.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "Component.h"

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
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* A class to manage the state of a control. */
class ComponentCharacterDisplay : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:
	

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Visual
	/* The primary display pixel. */
	Pixel _primaryPixel;
	/* The secondary display pixel. */
	Pixel _secondaryPixel;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentCharacterDisplay();
	/* Cleans up the component. */
	~ComponentCharacterDisplay();
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
	
	/* Sets the primary display pixel. */
	void SetPrimaryPixel(Pixel pixel);
	/* Sets the secondary display pixel. */
	void SetSecondayPixel(Pixel pixel);

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
#endif /* COMPONENT_CHAR_DISPLAY_H */
//=================================================================|
