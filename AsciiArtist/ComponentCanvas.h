/*=================================================================|
|* File:				ComponentCanvas.h						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/12/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_CANVAS_H
#define COMPONENT_CANVAS_H

#include "Stdafx.h"
#include "Geometry.h"
#include "AsciiImage.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "Component.h"
#include "ComponentScrollBar.h"

using namespace std;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Components;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {

//=================================================================|
// PREDEFINITIONS												   |
//=================================================================/

namespace AsciiArtistApp {
	class AsciiArtistManager;
}
using namespace AsciiArtistApp;

//=================================================================|
// NAMESPACES													   |
//=================================================================/

/* The list of components in the library. */
namespace Components {
	
//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* The list of different canvas tools available. */
enum class ActionTypes : short {
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

/* A class to manage the state of a control. */
class ComponentCanvas : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Containement
	/* The ascii artist manager. */
	AsciiArtistManager* _aa;

	// Visual
	/* The link to the current canvas image. */
	AsciiAnimation* _image;
	/* The scrollbar for the canvas. */
	ComponentScrollBar* _scrollBar;

	/* The current frame being edited. */
	int _frameIndex;
	/* The frame to display if the user is shift highlighting a frame. */
	int _hoverFrame;
	/* True if the mouse is inside the canvas. */
	bool _insideCanvas;
	/* The mouse position on the image. */
	Coord _mouseImagePos;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentCanvas();
	/* Cleans up the component. */
	~ComponentCanvas();
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
	
	/* Sets the link to the ascii artist manager. */
	void SetAsciiArtistManager(AsciiArtistManager* aa);
	void SetScrollBar(ComponentScrollBar* scrollBar);
	void SetImage(AsciiAnimation* image);
	void SetFrame(int index);
	void SetHoverFrame(int hoverFrame);
	
	/* Gets the image draw size for tools. */
	Coord GetDrawSize() const;
	/* Gets the position of the image view. */
	Coord GetImagePosition() const;
	/* Gets the bounding box of the image view. */
	SmallRect GetImageBounds() const;
	/* Gets the mouse position on the image. */
	Coord GetImageMousePosition() const;

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the mouse is inside the canvas. */
	bool IsMouseInsideCanvas() const;

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
#endif /* COMPONENT_CANVAS_H */
//=================================================================|
