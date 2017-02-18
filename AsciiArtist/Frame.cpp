/*=================================================================|
|* File:				Frame.cpp								  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "Frame.h"
#include "ConsoleApp.h"
#include "Component.h"
#include "ComponentWindow.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Components;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region Frame
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Frame::Frame() {
	this->_app				= nullptr;
	this->_id				= "";
}
PowerConsole::Frame::~Frame() {
	this->_app				= nullptr;
}
void PowerConsole::Frame::Initialize(ConsoleApp* app) {
	this->_app				= app;
}
void PowerConsole::Frame::Uninitialize() {
	ClearComponents();

	this->_app				= nullptr;
}
void PowerConsole::Frame::OnEnter() {

}
void PowerConsole::Frame::OnLeave() {

}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

bool PowerConsole::Frame::IsInitialized() const {
	return (_app != nullptr);
}
string PowerConsole::Frame::GetID() const {
	return _id;
}
void PowerConsole::Frame::SetID(const string& id) {
	this->_id = id;
}
bool PowerConsole::Frame::IsWindowOpen() const {
	return (_window != nullptr);
}
ComponentWindow* PowerConsole::Frame::GetWindow() const {
	return _window;
}

unsigned int PowerConsole::Frame::GetNumComponents() {
	return (unsigned int)_components.size();
}
vector<Component*>& PowerConsole::Frame::GetComponents() {
	return _components;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Frame::Update() {
	
	if (_window != nullptr) {
		if (_window->IsClosed()) {
			_window->Uninitialize();
			delete _window;
			_window = nullptr;
		}
		else {
			_window->Update();
		}
	}

	if (_window == nullptr) {
		for (int i = 0; i < (int)_components.size(); i++) {
			_components[i]->Update();
		}
	}

}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Frame::Draw(AsciiImageBase& g) {

	for (int i = 0; i < (int)_components.size(); i++) {
		_components[i]->Draw(g);
	}

	if (_window != nullptr) {
		_window->Draw(g);
	}
}

#pragma endregion
//========== COMPONENTS ==========
#pragma region Components

void PowerConsole::Frame::ClearComponents() {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_app != nullptr) {
			_components[i]->Uninitialize();
		}
		delete _components[i];
	}
	_components.clear();
}

void PowerConsole::Frame::AddComponent(Component* component) {
	if (!component->IsInitialized()) {
		_components.push_back(component);
		if (_app != nullptr) {
			component->Initialize(_app, this);
		}
	}
}
void PowerConsole::Frame::RemoveComponent(Component* component) {
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
void PowerConsole::Frame::RemoveComponent(const string& id) {
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
void PowerConsole::Frame::RemoveComponent(int index) {
	if (index < (int)_components.size()) {
		if (_app != nullptr) {
			_components[index]->Uninitialize();
		}
		delete _components[index];
		_components.erase(_components.begin() + index);
		return;
	}
}

Component* PowerConsole::Frame::GetComponent(const string& id) {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_components[i]->GetID() == id) {
			return _components[i];
		}
	}
	return nullptr;
}
Component* PowerConsole::Frame::GetComponent(int index) {
	if (index < (int)_components.size()) {
		return _components[index];
	}
	return nullptr;
}

bool PowerConsole::Frame::ComponentExists(Component* component) {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_components[i] == component) {
			return true;
		}
	}
	return false;
}
bool PowerConsole::Frame::ComponentExists(const string& id) {
	for (int i = 0; i < (int)_components.size(); i++) {
		if (_components[i]->GetID() == id) {
			return true;
		}
	}
	return false;
}

void PowerConsole::Frame::OpenWindow(ComponentWindow* window) {
	if (_window != nullptr) {
		_window->Uninitialize();
		delete _window;
		_window = nullptr;
	}

	if (window != nullptr) {
		_window = window;
		_window->Initialize(_app, this);
	}
}
void PowerConsole::Frame::CloseWindow() {
	if (_window != nullptr) {
		_window->Close();
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|