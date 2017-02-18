/*=================================================================|
|* File:				ComponentButton.h						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_BUTTON_H
#define COMPONENT_BUTTON_H

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

/* The different states a button can be in. */
enum class ButtonState : short {
	None = -1,
	Hovering = 0,
	Released = 1,
	Down = 2,
	Pressed = 3
};

#pragma endregion
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* A class to manage the state of a control. */
class ComponentButton : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	// Visual
	/* The icons used for the button. */
	AsciiAnimation* _icons;
	/* The text used for the button. */
	string _text;
	/* The spacing used before the icon or text. */
	Coord _spacing;
	/* True if the button can be toggled. */
	bool _togglable;

	// Events
	/* The current state of the button. */
	ButtonState _state;
	/* True if the toggle button is toggled. */
	bool _toggleState;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentButton();
	/* Cleans up the component. */
	~ComponentButton();
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
	
	/* Gets the icons of the button. */
	AsciiAnimation* GetIcons() const;
	/* Sets the icons of the button. */
	void SetIcons(AsciiAnimation* icons);
	/* Gets the text of the button. */
	string GetText() const;
	/* Sets the text of the button. */
	void SetText(const string& text);
	/* Gets the spacing before the icon or text. */
	Coord GetSpacing() const;
	/* Sets the spacing before the icon or text. */
	void SetSpacing(Coord spacing);
	
	/* Returns true if the button is togglable. */
	bool IsTogglable() const;
	/* Sets if the button is togglable. */
	void SetTogglable(bool togglable);
	/* Sets if the button is toggled. */
	void SetToggled(bool toggled);

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Gets the current state of the button. */
	ButtonState GetButtonState() const;
	/* Returns true if the button is being hovered over. */
	bool IsButtonHovering() const;
	/* Returns true if the button was pressed. */
	bool IsButtonPressed() const;
	/* Returns true if the button is down. */
	bool IsButtonDown() const;
	/* Returns true if the button was released. */
	bool IsButtonReleased() const;
	/* Returns true if the button is toggled. */
	bool IsToggled() const;

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
#endif /* COMPONENT_BUTTON_H */
//=================================================================|
