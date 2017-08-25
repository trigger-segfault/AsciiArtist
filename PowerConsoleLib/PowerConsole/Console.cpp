/*=================================================================|
|* File:				Console.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Console.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/
//=========== MEMBERS ============
#pragma region Members

ConsoleSettingsUPtr Console::console = std::make_unique<ConsoleSettings>(true);

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Console Cursor

Point2I Console::getCursorPosition() {
	return console->getCursorPosition();
}
void Console::setCursorPosition(Point2I position) {
	console->setCursorPosition(position);
}
unsigned char Console::getCurrentColor() {
	return console->getCurrentColor();
}
void Console::setCurrentColor(unsigned char color) {
	console->setCurrentColor(color);
}
int Console::getCursorFill() {
	return console->getCursorFill();
}
void Console::setCursorFill(int fill) {
	console->setCursorFill(fill);
}
bool Console::isCursorVisible() {
	return console->isCursorVisible();
}
void Console::setCursorVisible(bool visible) {
	console->setCursorVisible(visible);
}

#pragma endregion
//--------------------------------
#pragma region Console IO Mode

ConsoleInputModes Console::getConsoleInputMode() {
	return console->getConsoleInputMode();
}
void Console::setConsoleInputMode(ConsoleInputModes inputMode) {
	console->setConsoleInputMode(inputMode);
}
ConsoleOutputModes Console::getConsoleOutputMode() {
	return console->getConsoleOutputMode();
}
void Console::setConsoleOutputMode(ConsoleOutputModes outputMode) {
	console->setConsoleOutputMode(outputMode);
}

#pragma endregion
//--------------------------------
#pragma region Console Dimensions

Point2I Console::getConsoleSize() {
	return console->getConsoleSize();
}
void Console::setConsoleSize(Point2I size) {
	console->setConsoleSize(size);
}
Point2I Console::getBufferPosition() {
	return console->getBufferPosition();
}
void Console::setBufferPosition(Point2I position) {
	console->setBufferPosition(position);
}
Point2I Console::getBufferSize() {
	return console->getBufferSize();
}
void Console::setBufferSize(Point2I size) {
	console->setBufferSize(size);
}
void Console::setConsoleScreen(Point2I consoleSize, Point2I bufferPosition, Point2I bufferSize) {
	console->setConsoleScreen(consoleSize, bufferPosition, bufferSize);
}
Point2I Console::getConsoleMaxSize() {
	return console->getConsoleMaxSize();
}
Point2I Console::getConsoleMinSize() {
	return console->getConsoleMinSize();
}

#pragma endregion
//--------------------------------
#pragma region Window Dimensions

Point2I Console::getWindowSize() {
	return console->getWindowSize();
}
Point2I Console::getClientSize() {
	return console->getClientSize();
}
Point2I Console::getWindowPosition() {
	return console->getWindowPosition();
}
void Console::setWindowPosition(Point2I position) {
	console->setWindowPosition(position);
}
Point2I Console::getClientPosition() {
	return console->getClientPosition();
}
void Console::setClientPosition(Point2I position) {
	console->setClientPosition(position);
}
Rectangle2I Console::getCurrentMonitor() {
	return console->getCurrentMonitor();
}

#pragma endregion
//--------------------------------
#pragma region Console Visuals

ColorB Console::getPaletteColor(int index) {
	return console->getPaletteColor(index);
}
void Console::setPaletteColor(int index, ColorB color) {
	console->setPaletteColor(index, color);
}
ConsolePalette Console::getPalette() {
	return console->getPalette();
}
void Console::setPalette(const ConsolePalette& palette) {
	console->setPalette(palette);
}
Point2I Console::getFontSize() {
	return console->getFontSize();
}
RasterFontSizes Console::getRasterFontSize() {
	return console->getRasterFontSize();
}
int Console::getTrueTypeFontSize() {
	return console->getTrueTypeFontSize();
}
void Console::setFontSize(Point2I size) {
	console->setFontSize(size);
}
void Console::setRasterFontSize(RasterFontSizes rasterSize) {
	console->setRasterFontSize(rasterSize);
}
void Console::setTrueTypeFontSize(int trueTypeSize) {
	console->setTrueTypeFontSize(trueTypeSize);
}
ConsoleFonts Console::getFontType() {
	return console->getFontType();
}
void Console::setFontType(ConsoleFonts font) {
	console->setFontType(font);
}
ConsoleFontWeights Console::getFontWeight() {
	return console->getFontWeight();
}
void Console::setFontWeight(ConsoleFontWeights weight) {
	console->setFontWeight(weight);
}
ConsoleFont Console::getFont() {
	return console->getFont();
}
void Console::setFont(ConsoleFont font) {
	console->setFont(font);
}

#pragma endregion
//--------------------------------
#pragma region Window Visuals

std::string Console::getTitle() {
	return console->getTitle();
}
void Console::setTitle(const std::string& title) {
	console->setTitle(title);
}
void Console::setIconFromFile(const std::string& path, int index) {
	console->setIconFromFile(path, index);
}
void Console::setIconFromResourceByIndex(int index) {
	console->setIconFromResourceByIndex(index);
}
void Console::setIconFromResourceById(int id) {
	console->setIconFromResourceById(id);
}
bool Console::isMouseVisible() {
	return console->isMouseVisible();
}
void Console::setMouseVisible(bool visible) {
	console->setMouseVisible(visible);
}

#pragma endregion
//--------------------------------
#pragma region Window Settings

bool Console::isResizable() {
	return console->isResizable();
}
void Console::setResizable(bool resizable) {
	console->setResizable(resizable);
}

bool Console::hasToolWindowBorder() {
	return console->hasToolWindowBorder();
}
void Console::setToolWindowBorder(bool enabled) {
	console->setToolWindowBorder(enabled);
}
bool Console::isMenuItemsEnabled() {
	return console->isMenuItemsEnabled();
}
void Console::setMenuItemsEnabled(bool enabled) {
	console->setMenuItemsEnabled(enabled);
}
bool Console::isCloseEnabled() {
	return console->isCloseEnabled();
}
void Console::setCloseEnabled(bool enabled) {
	console->setCloseEnabled(enabled);
}
bool Console::isMaximizeEnabled() {
	return console->isMaximizeEnabled();
}
void Console::setMaximizeEnabled(bool enabled) {
	console->setMaximizeEnabled(enabled);
}
bool Console::isMinimizeEnabled() {
	return console->isMinimizeEnabled();
}
void Console::setMinimizeEnabled(bool enabled) {
	console->setMinimizeEnabled(enabled);
}

#pragma endregion
//--------------------------------
#pragma region Handles

HWND Console::getWindowHandle() {
	return console->getWindowHandle();
}
HANDLE Console::getOuputHandle() {
	return console->getOuputHandle();
}
HANDLE Console::getInputHandle() {
	return console->getInputHandle();
}

#pragma endregion
//--------------------------------
#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

bool Console::isActive() {
	return console->isActive();
}
void Console::activate() {
	console->activate();
}
void Console::maximizeWindow() {
	console->maximizeWindow();
}
void Console::minimizeWindow() {
	console->minimizeWindow();
}
void Console::showWindow() {
	console->showWindow();
}
void Console::hideWindow() {
	console->hideWindow();
}
bool Console::isWindowVisible() {
	return console->isWindowVisible();
}
void Console::centerWindow() {
	console->centerWindow();
}
void Console::pause() {
	console->pause();
}
void Console::pause(const std::string& message) {
	console->pause(message);
}
void Console::clear() {
	console->clear();
}

void Console::flashWindow() {
	console->flashWindow();
}
void Console::playSystemSound(SystemSounds sound, bool async) {
	console->playSystemSound(sound, async);
}
void Console::playSoundFromResourceById(int id, bool async) {
	console->playSoundFromResourceById(id, async);
}
void Console::playSoundFromFile(const std::string& path, bool async) {
	console->playSoundFromFile(path, async);
}

#pragma endregion
//============ OUTPUT ============
#pragma region Output

std::string Console::readLine() {
	return console->readLine();
}

void Console::writeImage(const AsciiImageFrame& image, int indentation) {
	console->writeImage(image, indentation);
}

#pragma endregion
//=================================================================|
