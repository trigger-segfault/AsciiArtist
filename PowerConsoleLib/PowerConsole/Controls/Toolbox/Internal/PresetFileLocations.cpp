/*=================================================================|
|* File:				PresetFileLocations.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PresetFileLocations.h"
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::IO;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox::Internal;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

PresetFileLocations::PresetFileLocations()
	:	// Locations
	sectionName("Section"),
	directories(),
	hoverIndex(-1),
	pressedIndex(-1),

	// Events
	_eventDirectoryChanged(EventDirections::FirstInFirstCalled) {
	setSizeSafe(Point2I(25, SIZE_AUTO));

	eventPaint().add(bindEvent(PresetFileLocations::onPaint));
	eventMouseMoved().add(bindEvent(PresetFileLocations::onMouseMoved));
	eventMouseButton().add(bindEvent(PresetFileLocations::onMouseButton));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Settings
std::string PresetFileLocations::getSectionName() const {
	return sectionName;
}
void PresetFileLocations::setSectionName(const std::string& sectionName) {
	this->sectionName = sectionName;
}
void PresetFileLocations::addDirectory(const std::string& directory) {
	directories.push_back(FileInfo(directory));
	updateAutoSize();
}
void PresetFileLocations::addDirectory(const FileInfo& directory) {
	directories.push_back(directory);
	updateAutoSize();
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I PresetFileLocations::autoSizeBehavior() const {
	return (int)directories.size() + 2;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<DirectoryChangedEventArgs>& PresetFileLocations::eventDirectoryChanged() {
	return _eventDirectoryChanged;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void PresetFileLocations::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char arrowColor = toColor(BLACK, DARK_CYAN);
	const unsigned char headerColor = toColor(BLACK, WHITE);
	const unsigned char itemColor = toColor(BLACK, GRAY);
	const unsigned char itemHoverColor = toColor(BLUE, WHITE);
	const unsigned char itemPressColor = toColor(BLUE, DARK_GRAY);

	g->drawString(Point2I(1, 0), sectionName, Pixel(' ', headerColor));

	for (int i = 0; i < (int)directories.size(); i++) {
		g->setPixel(Point2I(1, i + 1), Pixel(Characters::BIG_ARROW_RIGHT, arrowColor));
		g->drawString(Point2I(3, i + 1), directories[i].name, Pixel(' ', itemColor));
	}

	if (pressedIndex != -1)
		g->setColor(Rectangle2I(0, pressedIndex, getActualSize().x, 1), itemPressColor);
	if (hoverIndex != -1 && hoverIndex != pressedIndex)
		g->setColor(Rectangle2I(0, hoverIndex, getActualSize().x, 1), itemHoverColor);
}
void PresetFileLocations::onMouseMoved(ControlSPtr sender, MouseEventArgs& e) {
	hoverIndex = -1;
	if (e.newPosition.y > 0 && e.newPosition.y <= (int)directories.size() && isMouseInside()) {
		hoverIndex = e.newPosition.y;
	}
}
void PresetFileLocations::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (hoverIndex != -1) {
			pressedIndex = hoverIndex;
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left) && pressedIndex != -1) {
		FileInfo& file = directories[pressedIndex - 1];
		if (pressedIndex == hoverIndex) {
			_eventDirectoryChanged(shared_from_this(), DirectoryChangedEventArgs(file.fileType == FileTypes::Shortcut ? file.shortcutPath : file.path));
		}
		pressedIndex = -1;
	}
}

#pragma endregion
//=================================================================|
