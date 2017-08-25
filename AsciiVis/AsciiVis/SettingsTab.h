/*=================================================================|
|* File:				SettingsTab.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SETTINGS_TAB_H
#define SETTINGS_TAB_H

#include <PowerConsole/Controls/Toolbox/Panel.h>
#include <AsciiVis/Visualization.h>
#include <AsciiVis/MyLabel.h>
#include <AsciiVis/MyYesNoToggleButton.h>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

class SettingsTab : public Panel {

	//=========== MEMBERS ============
	#pragma region Members

	VisualizationSPtr vis;

	// Controls
	MyLabelSPtr labelCompact;
	MyYesNoToggleButtonSPtr toggleCompact;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	SettingsTab(VisualizationSPtr vis);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onCompactChecked(ControlSPtr sender, EventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(SettingsTab);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
