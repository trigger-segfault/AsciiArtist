/*=================================================================|
|* File:				ComponentTextBox.h						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/16/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_TEXT_BOX_H
#define COMPONENT_TEXT_BOX_H

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
class ComponentTextBox : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	
	/* The current text. */
	string _text;
	/* The text being typed in. */
	string _textTyping;
	/* True if the text box is being typed into. */
	bool _typing;
	/* The position of the cursor. */
	int _cursorPos;
	/* The indent around the text region. */
	Coord _indent;
	/* True if the text box is being hovered over. */
	bool _hovering;
	/* True if the textbox is typing is finished. */
	bool _finishedTyping;
	
	/* The timer used for cursor flashing. */
	unsigned int _cursorTimer;
	/* True if the cursor is flashing. */
	bool _cursorState;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component. */
	ComponentTextBox();
	/* Cleans up the component. */
	~ComponentTextBox();
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
	
	/* Gets the text in the textbox. */
	string GetText() const;
	/* Sets the text of the textbox. */
	void SetText(const string& text);
	
	/* Gets the text indent. */
	Coord GetIndent() const;
	/* Sets the text indent. */
	void SetIndent(Coord indent);

	/* Returns true if the textbox is being typed into. */
	bool IsTyping() const;
	/* Sets if the textbox is being typed into. */
	void SetTyping(bool typing);

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the textbox typing has been finished. */
	bool IsFinishedTyping() const;

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
#endif /* COMPONENT_TEXT_BOX_H */
//=================================================================|
