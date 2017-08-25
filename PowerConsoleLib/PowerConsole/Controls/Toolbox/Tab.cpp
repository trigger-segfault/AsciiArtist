/*=================================================================|
|* File:				Tab.cpp									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Tab.h"

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Tab::Tab()
 :	header("Tab") {}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

std::string Tab::getHeader() const {
	return header;
}
void Tab::setHeader(const std::string& header) {
	this->header = header;
}

#pragma endregion
//=================================================================|
