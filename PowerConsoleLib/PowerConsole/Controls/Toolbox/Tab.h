/*=================================================================|
|* File:				Tab.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TAB_H
#define TAB_H

#include <PowerConsole/Controls/ContainerControl.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Tab : public ContainerControl {

	//=========== MEMBERS ============
	#pragma region Members

	/* The header text of the tab button. */
	std::string header;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tab. */
	Tab();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the header text of the tab button. */
	std::string getHeader() const;
	/* Sets the header text of the tab button. */
	void setHeader(const std::string& text);

	#pragma endregion
};

DECLARE_SMART_PTRS(Tab);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
