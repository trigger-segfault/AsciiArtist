/*=================================================================|
|* File:				ComponentCharacterDisplay.cpp			  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/12/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentCharacterDisplay.h"
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
#pragma region ComponentCharacterDisplay
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentCharacterDisplay::ComponentCharacterDisplay() : Component() {
	this->_primaryPixel		= Pixel('A', 0x07);
	this->_secondaryPixel	= Pixel(' ', 0x00);
}
PowerConsole::Components::ComponentCharacterDisplay::~ComponentCharacterDisplay() {
	
}
void PowerConsole::Components::ComponentCharacterDisplay::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentCharacterDisplay::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentCharacterDisplay::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentCharacterDisplay::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Visual

void PowerConsole::Components::ComponentCharacterDisplay::SetPrimaryPixel(Pixel pixel) {
	this->_primaryPixel = pixel;
}
void PowerConsole::Components::ComponentCharacterDisplay::SetSecondayPixel(Pixel pixel) {
	this->_secondaryPixel = pixel;
}
	
#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentCharacterDisplay::Update() {
	
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentCharacterDisplay::Draw(AsciiImageBase& g) {
	if ((_primaryPixel.Attributes & PixelAttributes::AttributeFInvert) != 0x00)
		g.SetPixel(_position + Coord(0, 0), Pixel(char(176), 0x08));
	else if ((_primaryPixel.Attributes & PixelAttributes::AttributeFColor) == 0x00)
		g.SetPixel(_position + Coord(0, 0), Pixel(char(178), 0x0F));
	else
		g.SetPixel(_position + Coord(0, 0), Pixel(char(219), _primaryPixel.Color));

	if ((_primaryPixel.Attributes & PixelAttributes::AttributeBInvert) != 0x00)
		g.SetPixel(_position + Coord(1, 0), Pixel(char(176), 0x08));
	else if ((_primaryPixel.Attributes & PixelAttributes::AttributeBColor) == 0x00)
		g.SetPixel(_position + Coord(1, 0), Pixel(char(178), 0x0F));
	else
		g.SetPixel(_position + Coord(1, 0), Pixel(' ', _primaryPixel.Color));
	
	if ((_secondaryPixel.Attributes & PixelAttributes::AttributeFInvert) != 0x00)
		g.SetPixel(_position + Coord(2, 0), Pixel(char(176), 0x08));
	else if ((_secondaryPixel.Attributes & PixelAttributes::AttributeFColor) == 0x00)
		g.SetPixel(_position + Coord(2, 0), Pixel(char(178), 0x0F));
	else
		g.SetPixel(_position + Coord(2, 0), Pixel(char(219), _secondaryPixel.Color));
	
	if ((_secondaryPixel.Attributes & PixelAttributes::AttributeBInvert) != 0x00)
		g.SetPixel(_position + Coord(3, 0), Pixel(char(176), 0x08));
	else if ((_secondaryPixel.Attributes & PixelAttributes::AttributeBColor) == 0x00)
		g.SetPixel(_position + Coord(3, 0), Pixel(char(178), 0x0F));
	else
		g.SetPixel(_position + Coord(3, 0), Pixel(' ', _secondaryPixel.Color));

	g.SetPixel(_position + Coord(6, 0), _primaryPixel);
	g.SetPixel(_position + Coord(8, 0), _secondaryPixel);
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|