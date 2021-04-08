/*=================================================================|
|* File:				AsciiChartWindow.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_CHART_WINDOW_H
#define ASCII_CHART_WINDOW_H

#include <AsciiChart/ChartWrapPanel.h>
#include <PowerConsole/Helpers/fixed_array.h>
#include <PowerConsole/Controls/MainWindow.h>
#include <PowerConsole/Controls/Toolbox/Panel.h>
//#include <PowerConsole/Controls/Toolbox/ScrollViewer.h>
#include <PowerConsole/Controls/Toolbox/StackPanel.h>

//using namespace PowerConsole;
//using namespace PowerConsole::Geometry;
//using namespace PowerConsole::Helpers;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiChart {
//=================================================================|
// CLASSES														   |
//=================================================================/

class AsciiChartWindow : public MainWindow {

	//========== CONSTANTS ===========
	#pragma region Constants


	static const PcGeo::Point2I DEFAULT_WINDOW_SIZE;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
	//--------------------------------
	#pragma region Controls

	// Controls
	PanelSPtr panelBottom;
	ChartWrapPanelSPtr charChart;
	ChartWrapPanelSPtr colorChart;
	PanelSPtr panelMessage;
	PanelSPtr panelLogo;
	PanelSPtr panelCharacters;
	PanelSPtr panelColors;
	StackPanelSPtr stackPanel;
	
	#pragma endregion
	//--------------------------------
	#pragma region Layout

	#pragma endregion
	//--------------------------------
	#pragma region Other

	/* The command line arguments of the program. */
	PcHelp::fixed_array<std::string> commandLine;

	/* Using dark mode theme */
	bool darkMode;
	/* Radix for drawing character codes. */
	unsigned short charRadix;
	/* Radix for drawing color codes. */
	unsigned short colorRadix;
	/* Theme: Text color. */
	PcDraw::Pixel textColor;
	/* The color of the message. */
	PcDraw::Pixel messageColor;
	/* Theme: Text color of lv1 darkened message. */
	PcDraw::Pixel messageColorDarken1;
	/* Theme: Text color of lv2 darkened message. */
	PcDraw::Pixel messageColorDarken2;
	/* Theme: Color for resize handle. */
	PcCtrls::ResizeHandlePixel resizeColor;
	/* Threshold percentage for choosing white (lower) or black (greater) foreground. */
	int luminanceThreshold;

	// Message
	/* The current message to display. */
	std::string message;
	/* The timer for removing the message. */
	int messageTimer;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default console window. */
	AsciiChartWindow(int argc, char** argv);
private:
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Command Line
public:
	/* Gets the command line arguments. */
	PcHelp::fixed_array<std::string> getCommandLineArguments() const;
	/* Gets the command line argument at the specified index. */
	std::string getCommandLineArgument(int index) const;
	/* Gets the bumber of command line arguments. */
	int getNumCommandLineArguments() const;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers

	/* Updates the window title. */
	void updateTitle();
	/* Updates theme colors after dark mode setting is changed. */
	void updateTheme();
	/* Choose a foreground color to match the background color. */
	PcDraw::ConsoleColors chooseForeground(PcDraw::ConsoleColors bgColor);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Sends a new message to display, using the default text color. */
	void sendMessage(const std::string& message);
	/* Sends a new message to display. */
	void sendMessage(const std::string& message, unsigned char messageColor);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onMessagePaint(ControlSPtr sender, PaintEventArgs& e);
	void onLogoPaint(ControlSPtr sender, PaintEventArgs& e);
	void onCharactersPaint(ControlSPtr sender, PaintEventArgs& e);
	void onColorsPaint(ControlSPtr sender, PaintEventArgs& e);
	void onCharactersPaintItem(ControlSPtr sender, PaintItemEventArgs& e);
	void onColorsPaintItem(ControlSPtr sender, PaintItemEventArgs& e);
	void onBottomDarkenPaint(ControlSPtr sender, PaintEventArgs& e);
	void onTick(ControlSPtr sender, TickEventArgs& e);
	//void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
	//======= COMMAND HANDLERS =======
	#pragma region Command Handlers

	void onToggleDarkModeCommand(ControlSPtr sender, EventArgs& e);
	void onToggleCharRadixCommand(ControlSPtr sender, EventArgs& e);
	void onToggleColorRadixCommand(ControlSPtr sender, EventArgs& e);
	void onLowerLuminanceCommand(ControlSPtr sender, EventArgs& e);
	void onRaiseLuminanceCommand(ControlSPtr sender, EventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* AsciiChart */
#endif /* ASCII_CHART_WINDOW_H */
//=================================================================|
