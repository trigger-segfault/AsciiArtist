/*=================================================================|
|* File:				ComponentWindow.cpp						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/16/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentWindow.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "AsciiImage.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Components;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ComponentWindow
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentWindow::ComponentWindow() : Component() {
	this->_closed			= false;
}
PowerConsole::Components::ComponentWindow::~ComponentWindow() {
	
}
void PowerConsole::Components::ComponentWindow::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentWindow::Uninitialize() {
	ClearComponents();

	Component::Uninitialize();
}
void PowerConsole::Components::ComponentWindow::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentWindow::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment
	
unsigned int PowerConsole::Components::ComponentWindow::GetNumComponents() {
	return (unsigned int)_components.size();
}
vector<Component*>& PowerConsole::Components::ComponentWindow::GetComponents() {
	return _components;
}
void PowerConsole::Components::ComponentWindow::Close() {
	this->_closed = true;
}

#pragma endregion
//--------------------------------
#pragma region Events
	
bool PowerConsole::Components::ComponentWindow::IsClosed() const {
	return _closed;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentWindow::Update() {

	for (int i = 0; i < (int)_components.size(); i++) {
		_components[i]->Update();
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentWindow::Draw(AsciiImageBase& g) {

	for (int i = 0; i < (int)_components.size(); i++) {
		_components[i]->Draw(g);
	}
}
	
#pragma endregion
//========== COMPONENTS ==========
#pragma region Components

void PowerConsole::Components::ComponentWindow::ClearComponents() {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_app != nullptr) {
			_components[i]->Uninitialize();
		}
		delete _components[i];
	}
	_components.clear();
}

void PowerConsole::Components::ComponentWindow::AddComponent(Component* component) {
	if (!component->IsInitialized()) {
		_components.push_back(component);
		if (_app != nullptr) {
			component->Initialize(_app, _frame);
		}
	}
}
void PowerConsole::Components::ComponentWindow::RemoveComponent(Component* component) {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_components[i] == component) {
			if (_app != nullptr) {
				_components[i]->Uninitialize();
			}
			delete _components[i];
			_components.erase(_components.begin() + i);
			return;
		}
	}
}
void PowerConsole::Components::ComponentWindow::RemoveComponent(const string& id) {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_components[i]->GetID() == id) {
			if (_app != nullptr) {
				_components[i]->Uninitialize();
			}
			delete _components[i];
			_components.erase(_components.begin() + i);
			return;
		}
	}
}
void PowerConsole::Components::ComponentWindow::RemoveComponent(int index) {
	if (index < (int)_components.size()) {
		if (_app != nullptr) {
			_components[index]->Uninitialize();
		}
		delete _components[index];
		_components.erase(_components.begin() + index);
		return;
	}
}

Component* PowerConsole::Components::ComponentWindow::GetComponent(const string& id) {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_components[i]->GetID() == id) {
			return _components[i];
		}
	}
	return nullptr;
}
Component* PowerConsole::Components::ComponentWindow::GetComponent(int index) {
	if (index < (int)_components.size()) {
		return _components[index];
	}
	return nullptr;
}

bool PowerConsole::Components::ComponentWindow::ComponentExists(Component* component) {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_components[i] == component) {
			return true;
		}
	}
	return false;
}
bool PowerConsole::Components::ComponentWindow::ComponentExists(const string& id) {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_components[i]->GetID() == id) {
			return true;
		}
	}
	return false;
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|