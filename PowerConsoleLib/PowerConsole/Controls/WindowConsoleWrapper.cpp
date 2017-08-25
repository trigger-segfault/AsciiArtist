/*=================================================================|
|* File:				WindowConsoleWrapper.cpp				  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "WindowConsoleWrapper.h"

using namespace PowerConsole;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

WindowConsoleWrapper::WindowConsoleWrapper()
	: console(nullptr) {}
WindowConsoleWrapper::WindowConsoleWrapper(ConsoleSettingsSPtr console) 
	: console(console) {}
void WindowConsoleWrapper::loadDefaults(bool resetDimensions) {
	console->loadDefaults(resetDimensions);
}

//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Console Dimensions

Point2I WindowConsoleWrapper::getConsoleMaxSize() {
	return console->getConsoleMaxSize();
}
Point2I WindowConsoleWrapper::getConsoleMinSize() {
	return console->getConsoleMinSize();
}

#pragma endregion
//--------------------------------
#pragma region Window Dimensions

Point2I WindowConsoleWrapper::getWindowSize() {
	return console->getWindowSize();
}
Point2I WindowConsoleWrapper::getClientSize() {
	return console->getClientSize();
}
Point2I WindowConsoleWrapper::getWindowPosition() {
	return console->getWindowPosition();
}
void WindowConsoleWrapper::setWindowPosition(Point2I position) {
	console->setWindowPosition(position);
}
Point2I WindowConsoleWrapper::getClientPosition() {
	return console->getClientPosition();
}
void WindowConsoleWrapper::setClientPosition(Point2I position) {
	console->setClientPosition(position);
}
Rectangle2I WindowConsoleWrapper::getCurrentMonitor() {
	return console->getCurrentMonitor();
}

#pragma endregion
//--------------------------------
#pragma region Console Visuals

ColorB WindowConsoleWrapper::getPaletteColor(int index) {
	return console->getPaletteColor(index);
}
void WindowConsoleWrapper::setPaletteColor(int index, ColorB color) {
	console->setPaletteColor(index, color);
}
ConsolePalette WindowConsoleWrapper::getPalette() {
	return console->getPalette();
}
void WindowConsoleWrapper::setPalette(const ConsolePalette& palette) {
	console->setPalette(palette);
}
Point2I WindowConsoleWrapper::getFontSize() {
	return console->getFontSize();
}
RasterFontSizes WindowConsoleWrapper::getRasterFontSize() {
	return console->getRasterFontSize();
}
int WindowConsoleWrapper::getTrueTypeFontSize() {
	return console->getTrueTypeFontSize();
}
void WindowConsoleWrapper::setFontSize(Point2I size) {
	console->setFontSize(size);
}
void WindowConsoleWrapper::setRasterFontSize(RasterFontSizes rasterSize) {
	console->setRasterFontSize(rasterSize);
}
void WindowConsoleWrapper::setTrueTypeFontSize(int trueTypeSize) {
	console->setTrueTypeFontSize(trueTypeSize);
}
ConsoleFonts WindowConsoleWrapper::getFontType() {
	return console->getFontType();
}
void WindowConsoleWrapper::setFontType(ConsoleFonts font) {
	console->setFontType(font);
}
ConsoleFontWeights WindowConsoleWrapper::getFontWeight() {
	return console->getFontWeight();
}
void WindowConsoleWrapper::setFontWeight(ConsoleFontWeights weight) {
	console->setFontWeight(weight);
}
ConsoleFont WindowConsoleWrapper::getFont() {
	return console->getFont();
}
void WindowConsoleWrapper::setFont(ConsoleFont font) {
	console->setFont(font);
}

#pragma endregion
//--------------------------------
#pragma region Window Visuals

std::string WindowConsoleWrapper::getTitle() {
	return console->getTitle();
}
void WindowConsoleWrapper::setTitle(const std::string& title) {
	console->setTitle(title);
}
void WindowConsoleWrapper::setIconFromFile(const std::string& path, int index) {
	console->setIconFromFile(path, index);
}
void WindowConsoleWrapper::setIconFromResourceByIndex(int index) {
	console->setIconFromResourceByIndex(index);
}
void WindowConsoleWrapper::setIconFromResourceById(int id) {
	console->setIconFromResourceById(id);
}
bool WindowConsoleWrapper::isMouseVisible() {
	return console->isMouseVisible();
}
void WindowConsoleWrapper::setMouseVisible(bool visible) {
	console->setMouseVisible(visible);
}

#pragma endregion
//--------------------------------
#pragma region Window Settings

/*bool WindowConsoleWrapper::isResizable() {
	return consoleSettings->isResizable();
}
void WindowConsoleWrapper::setResizable(bool resizable) {
	consoleSettings->setResizable(resizable);
}*/
bool WindowConsoleWrapper::hasToolWindowBorder() {
	return console->hasToolWindowBorder();
}
void WindowConsoleWrapper::setToolWindowBorder(bool enabled) {
	console->setToolWindowBorder(enabled);
}
bool WindowConsoleWrapper::isMenuItemsEnabled() {
	return console->isMenuItemsEnabled();
}
void WindowConsoleWrapper::setMenuItemsEnabled(bool enabled) {
	console->setMenuItemsEnabled(enabled);
}
bool WindowConsoleWrapper::isCloseEnabled() {
	return console->isCloseEnabled();
}
void WindowConsoleWrapper::setCloseEnabled(bool enabled) {
	console->setCloseEnabled(enabled);
}
bool WindowConsoleWrapper::isMaximizeEnabled() {
	return console->isMaximizeEnabled();
}
void WindowConsoleWrapper::setMaximizeEnabled(bool enabled) {
	console->setMaximizeEnabled(enabled);
}
bool WindowConsoleWrapper::isMinimizeEnabled() {
	return console->isMinimizeEnabled();
}
void WindowConsoleWrapper::setMinimizeEnabled(bool enabled) {
	console->setMinimizeEnabled(enabled);
}

#pragma endregion
//--------------------------------
#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

bool WindowConsoleWrapper::isActive() {
	return console->isActive();
}
void WindowConsoleWrapper::activate() {
	console->activate();
}
void WindowConsoleWrapper::maximizeWindow() {
	console->maximizeWindow();
}
void WindowConsoleWrapper::minimizeWindow() {
	console->minimizeWindow();
}
void WindowConsoleWrapper::showWindow() {
	console->showWindow();
}
void WindowConsoleWrapper::hideWindow() {
	console->hideWindow();
}
bool WindowConsoleWrapper::isWindowVisible() {
	return console->isWindowVisible();
}
void WindowConsoleWrapper::centerWindow() {
	console->centerWindow();
}
void WindowConsoleWrapper::pause() {
	console->pause();
}

void WindowConsoleWrapper::flashWindow() {
	console->flashWindow();
}
void WindowConsoleWrapper::playSystemSound(SystemSounds sound, bool async) {
	console->playSystemSound(sound, async);
}
void WindowConsoleWrapper::playSoundFromResourceById(int id, bool async) {
	console->playSoundFromResourceById(id, async);
}
void WindowConsoleWrapper::playSoundFromFile(const std::string& path, bool async) {
	console->playSoundFromFile(path, async);
}

#pragma endregion
//=================================================================|
