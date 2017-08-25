/*=================================================================|
|* File:				PresetFileLocations.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef PRESET_FILE_LOCATIONS_H
#define PRESET_FILE_LOCATIONS_H

#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Controls/Toolbox/Internal/FileList.h>

namespace PowerConsole::Controls::Toolbox::Internal {
//=================================================================|
// CLASSES														   |
//=================================================================/

class PresetFileLocations : public Control {

	//=========== MEMBERS ============
	#pragma region Members

	// Files
	/* The name of the section header. */
	std::string sectionName;
	/* The directories to list. */
	std::vector<PcIO::FileInfo> directories;
	/* The index being hovered over. */
	int hoverIndex;
	/* The index being pressed. */
	int pressedIndex;

	// Events
	/* Called when a new directory has been loaded. */
	Event<DirectoryChangedEventArgs> _eventDirectoryChanged;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	PresetFileLocations();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Settings
	/* Gets the name of the section header. */
	std::string getSectionName() const;
	/* Sets the name of the section header. */
	void setSectionName(const std::string& sectionName);
	/* Adds a directory to the list. */
	void addDirectory(const std::string& directory);
	/* Adds a directory to the list. */
	void addDirectory(const PcIO::FileInfo& directory);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const final;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when a new directory has been loaded. */
	Event<DirectoryChangedEventArgs>& eventDirectoryChanged();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMouseMoved(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(PresetFileLocations);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
