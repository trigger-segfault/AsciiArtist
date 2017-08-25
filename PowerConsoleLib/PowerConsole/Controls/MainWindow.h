/*=================================================================|
|* File:				MainWindow.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Window.h>

/* Creates a window and runs it as a PowerConsole Application. */
#define runWindow(windowType, ...) MainWindow::run(makeWindow(windowType, __VA_ARGS__))

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

class MainWindow : public Window {

	//=========== MEMBERS ============
	#pragma region Members


	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the window. */
	MainWindow();
	/* Deconstructs window. */
	~MainWindow();
private:
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating
public:
	/* Runs the specified powerconsole application. */
	static void run(MainWindowSPtr app);

	/* Draws the control to the graphics object. */
	void repaint(PcDraw::GraphicsSPtr g) final;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Control Tree
public:
	/* Sets the owner of the window. */
	void setOwner(WindowSPtr owner) final;

	#pragma endregion
	//--------------------------------
	#pragma region Local Layout

	/* Gets the size of the control. */
	PcGeo::Point2I getSize() const final;
	/* Sets the size of the control. */
	void setSize(PcGeo::Point2I size) final;

	#pragma endregion
	//--------------------------------
	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
