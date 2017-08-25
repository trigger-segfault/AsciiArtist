/*=================================================================|
|* File:				FileLocations.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FileLocations.h"
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox::Internal;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

FileLocations::FileLocations() 
 :	// Locations
	directory(),
	hoverIndex(-1),
	pressedIndex(-1),
	
	// Events
	_eventDirectoryChanged(EventDirections::FirstInFirstCalled) {
	setSizeSafe(Point2I(25, SIZE_AUTO));

	eventPaint().add(bindEvent(FileLocations::onPaint));
	eventMouseMoved().add(bindEvent(FileLocations::onMouseMoved));
	eventMouseButton().add(bindEvent(FileLocations::onMouseButton));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Settings
const FileInfo& FileLocations::getDirectory() const {
	return directory.directoryFile;
}
void FileLocations::setDirectory(const std::string& directory) {
	setDirectory(FileInfo(directory));
}
void FileLocations::setDirectory(const FileInfo& directory) {
	this->directory.directoryFile = directory;
	refresh();
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I FileLocations::autoSizeBehavior() const {
	return (int)directory.fileList.size() + 2;
}

void FileLocations::refresh() {
	std::string nameBackup = directory.directoryFile.name;
	directory.refresh();
	directory.directoryFile.name = nameBackup;
	directory.removeParentDirectory();
	directory.removeFileTypes(FileTypes::File);
	updateAutoSize();
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<DirectoryChangedEventArgs>& FileLocations::eventDirectoryChanged() {
	return _eventDirectoryChanged;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void FileLocations::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char arrowColor = toColor(BLACK, DARK_CYAN);
	const unsigned char headerColor = toColor(BLACK, WHITE);
	const unsigned char itemColor = toColor(BLACK, GRAY);
	const unsigned char itemHoverColor = toColor(BLUE, WHITE);
	const unsigned char itemPressColor = toColor(BLUE, DARK_GRAY);

	g->drawString(Point2I(1, 0), directory.directoryFile.name, Pixel(' ', headerColor));

	for (int i = 0; i < (int)directory.fileList.size(); i++) {
		g->setPixel(Point2I(1, i + 1), Pixel(Characters::BIG_ARROW_RIGHT, arrowColor));
		g->drawString(Point2I(3, i + 1), directory.fileList[i].name, Pixel(' ', itemColor));
	}

	if (pressedIndex != -1)
		g->setColor(Rectangle2I(0, pressedIndex, getActualSize().x, 1), itemPressColor);
	if (hoverIndex != -1 && hoverIndex != pressedIndex)
		g->setColor(Rectangle2I(0, hoverIndex, getActualSize().x, 1), itemHoverColor);
}
void FileLocations::onMouseMoved(ControlSPtr sender, MouseEventArgs& e) {
	hoverIndex = -1;
	if (e.newPosition.y <= (int)directory.fileList.size() && isMouseInside()) {
		hoverIndex = e.newPosition.y;
	}
}
void FileLocations::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (hoverIndex != -1) {
			pressedIndex = hoverIndex;
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left) && pressedIndex != -1) {
		FileInfo& file = (pressedIndex == 0 ? directory.directoryFile : directory.fileList[pressedIndex - 1]);
		if (pressedIndex == hoverIndex) {
			_eventDirectoryChanged(shared_from_this(), DirectoryChangedEventArgs(file.fileType == FileTypes::Shortcut ? file.shortcutPath : file.path));
		}
		pressedIndex = -1;
	}
}

#pragma endregion
//=================================================================|
