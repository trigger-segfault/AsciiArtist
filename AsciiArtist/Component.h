/*=================================================================|
|* File:				Component.h								  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "Stdafx.h"
#include "Geometry.h"
#include "AsciiImage.h"

using namespace std;
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
class Frame;

//=================================================================|
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations

/* The different anchor methods for the component. */
enum class Anchor : short {
	TopLeft,
	Top,
	TopRight,
	Right,
	BottomRight,
	Bottom,
	BottomLeft,
	Left
};

#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* A class to manage the state of a control. */
class Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Containment
	/* The console app running the program. */
	ConsoleApp* _app;
	/* The frame containing this component. */
	Frame* _frame;
	/* The string identifier of the component. */
	string _id;

	// Position
	/* The position of the component. */
	Coord _position;
	/* The size of the component. */
	Coord _size;
	/* The anchoring method for the component. */
	//Anchor _anchor;
	/* The component this component is anchored to. */
	//Component* _anchorComponent;

	// State
	/* True if the component is visible. */
	bool _visible;
	/* True if the component is enabled. */
	bool _enabled;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	Component();
	/* Cleans up the component. */
	~Component();
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
	#pragma region Containment
	
	/* Returns true if the component has been initialized. */
	bool IsInitialized() const;
	/* Gets the string identifier of the component. */
	string GetID() const;
	/* Sets the string identifier of the component. */
	void SetID(const string& id);
	
	#pragma endregion
	//--------------------------------
	#pragma region Position

	/* Gets the position of the component. */
	Coord GetPosition() const;
	/* Sets the position of the component. */
	void SetPosition(Coord position);
	/* Gets the size of the component. */
	Coord GetSize() const;
	/* Sets the size of the component. */
	void SetSize(Coord size);
	/* Gets the bounding box of the component. */
	SmallRect GetBounds() const;
	/* Gets the anchor of the component. */
	//Anchor GetAnchor() const;
	/* Sets the anchor of the component. */
	//void SetAnchor(Anchor anchor);
	/* Gets the anchor component of the component. */
	//Component* GetAnchorComponent() const;
	/* Sets the anchor component of the component. */
	//void SetAnchorComponent(Component* anchorComponent);
	/* Gets the position of the component at its anchor. */
	//Coord GetAnchorPosition();

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
	//========== MANAGEMENT ==========
	#pragma region Management
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace PowerConsole */
#endif /* COMPONENT_H */
//=================================================================|
