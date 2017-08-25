/*=================================================================|
|* File:				FileLocations.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FILE_LOCATIONS_H
#define FILE_LOCATIONS_H

#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Controls/Toolbox/Internal/FileList.h>

namespace PowerConsole::Controls::Toolbox::Internal {
//=================================================================|
// CLASSES														   |
//=================================================================/

class FileLocations : public Control {
	//=========== MEMBERS ============
	#pragma region Members

	// Files
	/* The directory to show locations from. */
	PcIO::DirectoryInfo directory;
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
	FileLocations();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Settings
	/* Gets the directory to show locations from. */
	const PcIO::FileInfo& getDirectory() const;
	/* Sets the directory to show locations from. */
	void setDirectory(const std::string& directory);
	/* Sets the directory to show locations from. */
	void setDirectory(const PcIO::FileInfo& directory);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const final;

	/* Refreshes the directory and its contents. */
	void refresh();

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

DECLARE_SMART_PTRS(FileLocations);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
