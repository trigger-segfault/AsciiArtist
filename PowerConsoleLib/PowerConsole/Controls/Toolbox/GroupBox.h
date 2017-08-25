/*=================================================================|
|* File:				GroupBox.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef GROUP_BOX_H
#define GROUP_BOX_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/ContainerControl.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class GroupBox : public ContainerControl {

	//=========== MEMBERS ============
	#pragma region Members

	/* The text used by the header. */
	std::string header;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the group box. */
	GroupBox();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the text used by the header. */
	std::string getHeader() const;
	/* Sets the text used by the header. */
	void setHeader(const std::string& header);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Sets the layout of the specified child. */
	void updateChildLayout(ControlSPtr control) override;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(GroupBox);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
