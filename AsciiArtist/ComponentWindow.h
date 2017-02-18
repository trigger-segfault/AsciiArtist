/*=================================================================|
|* File:				ComponentWindow.h						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/16/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#pragma once
#ifndef COMPONENT_WINDOW_H
#define COMPONENT_WINDOW_H

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
class ComponentWindow : public Component {
	
	//========== CONSTANTS ===========
	#pragma region Constants
public:


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The collection of components in the container. */
	vector<Component*> _components;
	/* True if the window has been closed. */
	bool _closed;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default component window. */
	ComponentWindow();
	/* Cleans up the component window. */
	~ComponentWindow();
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
	
	/* Gets the number of components in the container. */
	unsigned int GetNumComponents();
	/* Gets the list of components in the container. */
	vector<Component*>& GetComponents();
	/* Closes the window. */
	void Close();

	#pragma endregion
	//--------------------------------
	#pragma region Events
	
	/* Returns true if the window has been closed. */
	bool IsClosed() const;

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
	//========== COMPONENTS ==========
	#pragma region Components
	
	/* Removes all the components from the container. */
	void ClearComponents();
	
	/* Adds the specified component to the container. */
	void AddComponent(Component* component);
	/* Removes the specified component from the container. */
	void RemoveComponent(Component* component);
	/* Removes the first component with the specified name from the container. */
	void RemoveComponent(const string& id);
	/* Removes the component at the specified index from the container. */
	void RemoveComponent(int index);
	
	/* Gets the first component with the specified id in the container. */
	Component* GetComponent(const string& id);
	/* Gets the component at the specified index in the container. */
	Component* GetComponent(int index);
	
	/* Returns true if the specified component exists in the container. */
	bool ComponentExists(Component* component);
	/* Returns true if a component with the specified id exists in the container. */
	bool ComponentExists(const string& id);
	
	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace Components */
} /* namespace PowerConsole */
#endif /* COMPONENT_WINDOW_H */
//=================================================================|
