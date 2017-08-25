/*=================================================================|
|* File:				FileList.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <PowerConsole/Controls/Toolbox/ScrollBars.h>
#include <PowerConsole/IO/DirectoryInfo.h>
#include <PowerConsole/Helpers/fixed_array.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Arguments for drawing file details. */
struct DrawFileDetailArgs {
	/* The id of the detail. */
	int detailId;
	/* The file to get the details from. */
	PcIO::FileInfo* file;
	/* The output string. */
	std::string outString;
	/* The output alignment of the string. */
	PcGeo::HorizontalAlignments alignment;

	/* Constructs the draw file detail arguments. */
	inline DrawFileDetailArgs()
		: detailId(0), file(nullptr), outString(""), alignment(PcGeo::HorizontalAlignments::Left) {}
	/* Constructs the draw file detail arguments. */
	inline DrawFileDetailArgs(int detailId, PcIO::FileInfo* file)
		: detailId(detailId), file(file), outString(""), alignment(PcGeo::HorizontalAlignments::Left) {}
};

/* A detail column used in the file dialog file list. */
struct DetailColumn {
	/* The displayed name of the column. */
	std::string name;
	/* The id of the column. */
	int id;
	/* The current width of the column. */
	int width;
	/* The minimum width of the column. */
	int minWidth;

	/* Constructs a detail column. */
	inline DetailColumn()
		: name(""), id(0), width(0), minWidth(0) {}
	/* Constructs a detail column. */
	inline DetailColumn(const std::string& name, int id, int width, int minWidth)
		: name(name), id(id), width(width), minWidth(minWidth) {}
};

//=================================================================|
} /* End namespace */
namespace PowerConsole::Controls::Toolbox::Internal {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Shows changes to the directory in the file list. */
struct DirectoryChangedEventArgs : public EventArgs {
	/* The path directory that was loaded. */
	std::string directory;
	/* The view position of the file list before loading the new directory. */
	int viewPosition;

	DirectoryChangedEventArgs()
		: directory("") {}
	DirectoryChangedEventArgs(const std::string& directory, int viewPosition = 0)
		: directory(directory), viewPosition(viewPosition) {}
};
/* Shows a selected file. */
struct FileSelectedEventArgs : public EventArgs {
	/* The file that was chosen or selected. */
	PcIO::FileInfo* file;

	inline FileSelectedEventArgs()
		: file(nullptr) {}
	inline FileSelectedEventArgs(PcIO::FileInfo* file)
		: file(file) {}
};

/* The file list used internally in the file dialog class. */
class FileList : public ScrollBars {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	static const int DETAIL_FILE_NAME = -1;
	static const int DETAIL_FILE_SIZE = -2;
	static const int DETAIL_FILE_MODIFIED = -3;
	static const int DETAIL_FILE_CREATED = -4;
private:
	/* The extra space at the bottom of the file list. */
	static const int BOTTOM_SPACING = 1;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	typedef std::function<void(DrawFileDetailArgs&)> DrawFileDetailFunction;
	typedef std::function<void(PcIO::FileInfo& file)> LoadFileDetailsFunction;

	// Items
	/* The loaded directory. */
	PcIO::DirectoryInfo directory;
	/* The currently selected list item. -1 if there is no selection. */
	int selection;
	/* The index of the selection currently being hovered over. */
	int hoverIndex;
	/* The index that has been pressed down by the mouse. */
	int pressedIndex;

	// Settings
	/* The extensions of files to show. Includes the '.' in the extension. */
	std::vector<std::string> filters;
	/* True if readonly files and folders are shown in the browser. */
	bool allowReadOnly;
	/* True if system files and folders are shown in the browser. */
	bool allowSystem;

	// Columns
	/* The categories for details. The string is the detail name, the first int is the column width, and the second int is the id. */
	std::vector<DetailColumn> columns;
	/* The id of the column being dragged. -1 if nothing is being dragged. */
	int columnDraggingIndex;
	/* The id of the column separator being hovered over. */
	int columnHoverIndex;

	// Functions
	/* Used to draw a type of file detail as a string. */
	DrawFileDetailFunction functionDrawFileDetail;
	/* Loads the details into the extra data shared pointer in the file info class. */
	LoadFileDetailsFunction functionLoadFileDetails;

	// Events
	/* Called when the selection of the list has changed. */
	Event<SelectionChangedEventArgs> _eventSelectionChanged;
	/* Called when a new directory has been loaded. */
	Event<DirectoryChangedEventArgs> _eventDirectoryChanged;
	/* Called when a file has been double clicked. */
	Event<FileSelectedEventArgs> _eventFileDoubleClicked;
	/* Called when a file has been selected. */
	Event<FileSelectedEventArgs> _eventFileSelected;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	FileList();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Selection
	/* Gets the currently selected list item. Returns "" if there is no selection. */
	PcIO::FileInfo* getSelectedItem();
	/* Gets the index currently selected list item. Returns -1 if there is no selection. */
	int getSelectedIndex() const;
	/* Sets the index currently selected list item. Use -1 to unset any selection. */
	void setSelectedIndex(int index);

	// Navigation
	/* Gets the current directory. */
	std::string getCurrentDirectory() const;
	/* Navigates the file list up one directory. */
	bool navigateUp();

	// Settings
	/* Gets if readonly files and folders are shown in the browser. */
	bool getAllowReadOnlyFiles() const;
	/* Sets if readonly files and folders are shown in the browser. */
	void setAllowReadOnlyFiles(bool allow);
	/* Gets if system files and folders are shown in the browser. */
	bool getAllowSystemFiles() const;
	/* Sets if system files and folders are shown in the browser. */
	void setAllowSystemFiles(bool allow);
	/* Sets if system or readonly files and folders are shown in the browser. */
	void setAllowedFiles(bool allowReadonly, bool allowSystem);

	// Filters
	/* Returns true if the file list has any filters. */
	bool hasFilters() const;
	/* Adds a filter extension to the list. Extensions should include the '.' if needed. */
	void addFilter(const std::string& extension);

	// File Details
	/* Adds a column into the detail columns. */
	void addColumn(const DetailColumn& column);
	/* Inserts a column into the detail columns. */
	void insertColumn(int index, const DetailColumn& column);
	/* Sets the width of the specified detail column. */
	void setColumnWidth(int id, int width);
	/* Gets the column with the specified id. */
	DetailColumn getColumn(int id) const;
	/* Removes the column with the specified id. */
	void removeColumn(int id);
	/* Clears all of the columns except the name column. */
	void clearColumns();

	// Functions
	/* Sets the function to load file details and store them in the shared pointer in the file info structure. */
	void setLoadFileDetailsFunction(LoadFileDetailsFunction func);
	/* Sets the draw file detail function used to draw a single type of custom file detail. */
	void setDrawFileDetailFunction(DrawFileDetailFunction func);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Loads the specified directory. */
	void loadDirectory(const PcIO::FileInfo& file);
	/* Loads the specified directory. */
	void loadDirectory(const std::string& path);
	/* Refreshes the current directory. */
	void refresh();

	#pragma endregion
	//========== SCROLLING ===========
	#pragma region Scrolling

	/* Scrolls the specified index into view. */
	void scrollIntoView(int index);
	/* Scrolls the selection into view. */
	void scrollSelectionIntoView();
	/* Gets the item being hovered over by the mouse position. */
	int getItemIndexFromMousePosition(PcGeo::Point2I position) const;
private:
	/* Updates the dimensions of the scrollbar after a property has been changed. */
	void updateScrollBars();

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
public:
	/* Called when the selection of the list has changed. */
	Event<SelectionChangedEventArgs>& eventSelectionChanged();
	/* Called when a new directory has been loaded. */
	Event<DirectoryChangedEventArgs>& eventDirectoryChanged();
	/* Called when a file has been double clicked. */
	Event<FileSelectedEventArgs>& eventFileDoubleClicked();
	/* Called when a file has been selected. */
	Event<FileSelectedEventArgs>& eventFileSelected();
	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);
	void onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);
	void onDoubleClick(ControlSPtr sender, MouseEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(FileList);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
