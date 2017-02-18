/*=================================================================|
|* File:				Frame.h									  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#pragma once
#ifndef FRAME_H
#define FRAME_H

#include "stdafx.h"
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
class Component;
namespace Components {
	class ComponentWindow;
}
using namespace Components;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes

/* The main base class that runs the current screen in the console. */
class Frame {

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
	/* The collection of components in the frame. */
	vector<Component*> _components;
	/* The string identifier of the frame. */
	string _id;
	/* The currently open component window. */
	ComponentWindow* _window;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default frame. */
	Frame();
	/* Cleans up the frame. */
	~Frame();
	/* Initializes the frame. */
	virtual void Initialize(ConsoleApp* app);
	/* Uninitailizes the frame. */
	virtual void Uninitialize();
	/* Called when the frame becomes active. */
	virtual void OnEnter();
	/* Called when the frame becomes inactive. */
	virtual void OnLeave();
	
	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Containment
	
	/* Returns true if the frame has been initialized. */
	bool IsInitialized() const;
	/* Gets the string identifier of the frame. */
	string GetID() const;
	/* Sets the string identifier of the frame. */
	void SetID(const string& id);
	/* Returns true if a component window is open. */
	bool IsWindowOpen() const;
	/* Gets the currently open component window. */
	ComponentWindow* GetWindow() const;

	/* Gets the number of components in the container. */
	unsigned int GetNumComponents();
	/* Gets the list of components in the container. */
	vector<Component*>& GetComponents();

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Called every step to update the frame. */
	virtual void Update();
	
	#pragma endregion
	//=========== DRAWING ============
	#pragma region Drawing

	/* Called every step to draw the frame to the window. */
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
	
	/* Opens a component window. */
	void OpenWindow(ComponentWindow* window);
	/* Closes the component window. */
	void CloseWindow();

	#pragma endregion
};

#pragma endregion
//=================================================================|
} /* namespace PowerConsole */
#endif /* FRAME_H */
//=================================================================|