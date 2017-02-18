/*=================================================================|
|* File:				ComponentColorPalette.h					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/12/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_COLOR_PALETTE_H
#define COMPONENT_COLOR_PALETTE_H

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
class ComponentColorPalette : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Visual
	/* The color palette image. */
	AsciiImage* _colorPaletteImage;

	/* The color selected from the palette. */
	Pixel _colorSelection;
	/* The index of the color selected from the palette. */
	int _colorIndex;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentColorPalette();
	/* Cleans up the component. */
	~ComponentColorPalette();
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
	
	/* Gets the image used for the color palette. */
	AsciiImage* GetColorPaletteImage() const;
	/* Sets the image used for the color palette. */
	void SetColorPaletteImage(AsciiImage* image);
	
	/* Gets the new color selection. */
	Pixel GetNewColor() const;

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the primary foreground color has been changed. */
	bool IsPrimaryForegroundColorChanged() const;
	/* Returns true if the primary background color has been changed. */
	bool IsPrimaryBackgroundColorChanged() const;
	/* Returns true if the secondary foreground color has been changed. */
	bool IsSecondaryForegroundColorChanged() const;
	/* Returns true if the secondary background color has been changed. */
	bool IsSecondaryBackgroundColorChanged() const;

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
#endif /* COMPONENT_COLOR_PALETTE_H */
//=================================================================|
