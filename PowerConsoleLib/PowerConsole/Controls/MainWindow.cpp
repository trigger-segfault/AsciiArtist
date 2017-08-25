/*=================================================================|
|* File:				MainWindow.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "MainWindow.h"

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

MainWindow::MainWindow() {
	eventInitialize().add(bindEvent(MainWindow::onInitialize));

}
MainWindow::~MainWindow() {

}
void MainWindow::onInitialize(ControlSPtr sender, EventArgs& e) {
	console().loadDefaults(true);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void MainWindow::run(MainWindowSPtr app) {
	app->load();

	app->mainLoop();

	app->unload();
}

void MainWindow::repaint(GraphicsSPtr g) {
	Window::repaint(g);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Control Tree

void MainWindow::setOwner(WindowSPtr owner) {
	throw std::exception("Cannot set owner for MainWindow");
}

#pragma endregion
//--------------------------------
#pragma region Local Layout

Point2I MainWindow::getSize() const {
	return Window::getSize();
}
void MainWindow::setSize(Point2I size) {
	Window::setSize(size);
}

#pragma endregion
//--------------------------------
#pragma endregion
//=================================================================|
