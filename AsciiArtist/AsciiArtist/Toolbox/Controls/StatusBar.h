/*=================================================================|
|* File:				StatusBar.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef _H
#define _H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Toolbox/Panel.h>
#include <PowerConsole/Controls/Toolbox/Label.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiArtist::Toolbox::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

class StatusBar : public Panel {
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The duration that a message lasts for. */
	static const int MESSAGE_DURATION = 4000;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
private:
	// Message
	/* The current message to display. */
	std::string message;
	/* The color of the message. */
	unsigned char messageColor;
	/* The timer for removing the message. */
	int messageTimer;

	// Controls
	LabelSPtr labelPoint;
	LabelSPtr labelSize;
	PanelSPtr panelMessage;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	StatusBar();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Sends a new message to the status bar. */
	void sendMessage(const std::string& message, unsigned char messageColor);

	/* Sets the point label. */
	void setLabelPoint(Point2I point);
	/* Sets the size label. */
	void setLabelSize(Point2I size);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onMessagePaint(ControlSPtr sender, PaintEventArgs& e);
	void onTick(ControlSPtr sender, TickEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(StatusBar);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
  //=================================================================|
