/*=================================================================|
|* File:				Component.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/5/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "Component.h"
#include "ConsoleApp.h"
#include "Frame.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region Component
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Component::Component() {
	this->_app				= nullptr;
	this->_frame			= nullptr;
	this->_id				= "";

	this->_position			= Coord::Zero;
	this->_size				= Coord::Zero;
	//this->_anchor			= Anchor::TopLeft;
	//this->_anchorComponent	= nullptr;

	this->_enabled			= true;
	this->_visible			= true;
}
PowerConsole::Component::~Component() {
	this->_app				= nullptr;
	this->_frame			= nullptr;

	//this->_anchorComponent	= nullptr;
}
void PowerConsole::Component::Initialize(ConsoleApp* app, Frame* frame) {
	this->_app				= app;
	this->_frame			= frame;
}
void PowerConsole::Component::Uninitialize() {
	this->_app				= nullptr;
	this->_frame			= nullptr;
}
void PowerConsole::Component::OnEnter() {

}
void PowerConsole::Component::OnLeave() {

}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

bool PowerConsole::Component::IsInitialized() const {
	return (_app != nullptr);
}
string PowerConsole::Component::GetID() const {
	return _id;
}
void PowerConsole::Component::SetID(const string& id) {
	this->_id = id;
}
	
#pragma endregion
//--------------------------------
#pragma region Position

Coord PowerConsole::Component::GetPosition() const {
	return _position;
}
void PowerConsole::Component::SetPosition(Coord position) {
	this->_position = position;
}
Coord PowerConsole::Component::GetSize() const {
	return _size;
}
void PowerConsole::Component::SetSize(Coord size) {
	this->_size = size;
}
SmallRect PowerConsole::Component::GetBounds() const {
	return SmallRect(_position, _size);
}
/*Anchor PowerConsole::Component::GetAnchor() const {
	return _anchor;
}
void PowerConsole::Component::SetAnchor(Anchor anchor) {
	this->_anchor = anchor;
}
Component* PowerConsole::Component::GetAnchorComponent() const {
	return _anchorComponent
}
void PowerConsole::Component::SetAnchorComponent(Component* anchorComponent) {
	this->_anchorComponent = anchorComponent;
}
Coord PowerConsole::Component::GetAnchorPosition() {
	Coord position		= _position;
	
	Coord anchorPos		= Coord(0, 0);
	Coord anchorSize	= _app->GetConsoleSize();

	if (_anchorComponent != nullptr) {
		anchorPos	= _anchorComponent->GetPosition();
		anchorSize	= _anchorComponent->GetSize();
	}

	if (_anchor == Anchor::BottomLeft || _anchor == Anchor::Left || _anchor == Anchor::TopLeft)
		position

	if (_anchorComponent != nullptr) {
		position += _anchorComponent->GetAnchorPosition();
	}
}*/

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Component::Update() {

}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Component::Draw(AsciiImageBase& g) {

}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|