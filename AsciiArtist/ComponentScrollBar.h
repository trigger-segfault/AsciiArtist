/*=================================================================|
|* File:				ComponentScrollBar.h					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/12/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_SCROLL_BAR_H
#define COMPONENT_SCROLL_BAR_H

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
// ENUMERATIONS													   |
//=================================================================/
#pragma region Enumerations


#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* A class to manage the state of a control. */
class ComponentScrollBar : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Dimensions
	/* The size of the scrollbar area. */
	Coord _scrollSize;
	/* The size of the scrollbars. */
	Coord _barSize;
	/* The size of the list viewport. */
	Coord _listViewSize;
	/* The size of the list. */
	Coord _listSize;
	/* The scroll to list size ratio. */
	Point2D _scrollRatio;
	
	// Dragging
	/* The view position of the list. */
	Coord _listPosition;
	/* The scroll position of the list. */
	Coord _scrollPosition;
	/* The grip position of the scrollbar. */
	int _gripPosition;
	/* The dragging state of the scrollbars. 0 = None, 1 = Y, 2 = X. */
	int _dragging;
	/* The hover visual of the scrollbars. 0 = None, 1 = Up, 2 = VBar, 3 = Down, 4 = Left, 5 = HBar, 6 = Right. */
	int _hovering;
	
	// State
	/* True if the vertical bar is active. */
	bool _verticalActive;
	/* True if the horizontal bar is active. */
	bool _horizontalActive;
	/* True if the vertical bar is enabled. */
	bool _verticalEnabled;
	/* True if the horizontal bar is enabled. */
	bool _horizontalEnabled;
	/* True if the mouse wheel can scroll the list. */
	bool _mouseWheelEnabled;
	/* True if the list position was changed. */
	bool _listPositionChanged;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentScrollBar();
	/* Cleans up the component. */
	~ComponentScrollBar();
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
	#pragma region Dragging
	
	/* Gets the position of the list view. */
	Coord GetListPosition() const;
	/* Gets the size of the list. */
	Coord GetListSize() const;
	/* Sets the position of the list view. */
	void SetListPosition(Coord position);
	/* Sets the position of the scrollbar. */
	void SetScrollPosition(Coord position);
	/* Gets the maximum list view position. */
	Coord GetMaxListPosition() const;
	
	#pragma endregion
	//--------------------------------
	#pragma region State
	
	/* Returns true if the vertical scrollbar is active. */
	bool IsVerticalBarActive() const;
	/* Returns true if the horizontal scrollbar is active. */
	bool IsHorizontalBarActive() const;

	/* Returns true if the vertical scrollbar is enabled. */
	bool IsVerticalBarEnabled() const;
	/* Sets if the vertical scrollbar should be enabled. */
	void SetVerticalBarEnabled(bool enabled);
	/* Returns true if the horizontal scrollbar is enabled. */
	bool IsHorizontalBarEnabled() const;
	/* Sets if the horizontal scrollbar should be enabled. */
	void SetHorizontalBarEnabled(bool enabled);

	/* Returns true if the mouse wheel can scroll the list. */
	bool IsMouseWheelEnabled() const;
	/* Sets if the mouse wheel can scroll the list. */
	void SetMouseWheelEnabled(bool enabled);
	
	#pragma endregion
	//--------------------------------
	#pragma region Dimensions

	/* Resizes the scrollbars. */
	void Resize(Coord listViewSize, Coord listSize, bool keepListPosition = false);

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the list view position was changed. */
	bool IsListPositionChanged() const;

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
#endif /* COMPONENT_SCROLL_BAR_H */
//=================================================================|
