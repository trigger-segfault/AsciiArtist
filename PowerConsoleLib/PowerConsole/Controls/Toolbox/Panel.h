/*=================================================================|
|* File:				Panel.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PANEL_H
#define PANEL_H

#include <PowerConsole/Controls/ContainerControl.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Panel : public ContainerControl {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The background image to draw with the panel. */
	PcDraw::AsciiImageFrameSPtr backgroundImage;
	/* True if the background is stretched to fit. */
	bool backgroundStretched;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	Panel();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the background image of the panel. */
	PcDraw::AsciiImageFrameSPtr getBackgroundImage() const;
	/* Sets the background image of the panel. */
	void setBackgroudImage(PcDraw::AsciiImageFrameSPtr backgroundImage);
	/* Gets if the background is stretched to fit the panel size. */
	bool isBackgroundStretched();
	/* Sets if the background is stretched to fit the panel size. */
	void setBackgroundStretched(bool backgroundStretched);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(Panel);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
