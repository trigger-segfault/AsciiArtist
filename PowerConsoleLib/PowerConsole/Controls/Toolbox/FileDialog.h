/*=================================================================|
|* File:				FileDialog.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <PowerConsole/Controls/Dialog.h>
#include <PowerConsole/IO/DirectoryInfo.h>
#include <PowerConsole/Helpers/fixed_array.h>
#include <PowerConsole/Controls/Toolbox/Internal/FileList.h>
#include <PowerConsole/Controls/Toolbox/Internal/FileLocations.h>
#include <PowerConsole/Controls/Toolbox/Internal/PresetFileLocations.h>
#include <PowerConsole/Controls/Toolbox/Button.h>
#include <PowerConsole/Controls/Toolbox/ImageButton.h>
#include <PowerConsole/Controls/Toolbox/Label.h>
#include <PowerConsole/Controls/Toolbox/Panel.h>
#include <PowerConsole/Controls/Toolbox/ScrollViewer.h>
#include <PowerConsole/Controls/Toolbox/StackPanel.h>
#include <PowerConsole/Controls/Toolbox/TextBox.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* The types of modes a file dialog can be in. */
enum class FileDialogModes {
	/* The file dialog will try to open a file. */
	Open,
	/* The file dialog will try to save a file. */
	Save
};


/* The file dialog class for opening and saving files in a browser. */
class FileDialog : public Dialog {

	//========== CONSTANTS ===========
	#pragma region Constants
public:
	/* The maximum number of pages to remember in history. */
	static const int MAX_HISTORY = 30;

	static const int DETAIL_FILE_NAME = -1;
	static const int DETAIL_FILE_SIZE = -2;
	static const int DETAIL_FILE_MODIFIED = -3;
	static const int DETAIL_FILE_CREATED = -4;

	#pragma endregion
	//=========== STRUCTS ============
	#pragma region Structs
private:
	struct FileDialogHistory {
		/* The path of the directory. */
		std::string path;
		/* The view position in the file list. This makes browsing a little more convenient. */
		int viewPosition;

		/* Constructs a file dialog history item. */
		/* Don't set the view position now. It's set when changing to the newest directory. */
		inline FileDialogHistory(const std::string& path)
			: path(path), viewPosition(0) {}
	};

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

public:
	typedef std::function<void(DrawFileDetailArgs&)> DrawFileDetailFunction;
	typedef std::function<void(PcIO::FileInfo& file)> LoadFileDetailsFunction;

private:
	// Controls
	ButtonSPtr buttonOpenSave;
	ButtonSPtr buttonCancel;
	ImageButtonSPtr buttonRefresh;
	ImageButtonSPtr buttonBack;
	ImageButtonSPtr buttonForward;
	LabelSPtr labelPath;
	LabelSPtr labelFilePath;
	LabelSPtr labelFileName;
	LabelSPtr labelCommonLocations;
	TextBoxSPtr textBoxName;
	PcIntr::FileListSPtr fileList;
	ScrollViewerSPtr scrollViewerLocations;
	StackPanelSPtr stackPanelLocations;
	PcIntr::PresetFileLocationsSPtr locationsPlaces;
	PcIntr::FileLocationsSPtr locationsFavorites;
	PcIntr::PresetFileLocationsSPtr locationsLibraries;
	PcIntr::FileLocationsSPtr locationsComputer;

	// Common Locations
	/* The list of custom places to show. */
	std::vector<PcIO::FileInfo> customPlaces;
	/* The list of libraries to show. */
	std::vector<PcIO::FileInfo> libraries;

	// History
	/* The list of navigated paths in the history. */
	std::vector<FileDialogHistory> history;
	/* The travel index in the history. */
	int historyIndex;

	// Settings
	/* The mode of the file dialog. Either opening or saving. */
	FileDialogModes mode;
	/* The initial file name and returned file. */
	std::string fileName;
	/* The initial directory to start in. */
	std::string initialDirectory;
	/* The default extension to save files with. Includes the '.' of the extension. */
	std::string defaultExt;
	/* The extensions of files to show. Includes the '.' in the extension. */
	std::vector<std::string> filters;
	/* True if readonly files and folders are shown in the browser. */
	bool allowReadOnly;
	/* True if system files and folders are shown in the browser. */
	bool allowSystem;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the file dialog. */
	FileDialog(FileDialogModes mode);
	/* Initializes the dialog. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the dialog. */
	void onLoad(ControlSPtr sender, EventArgs& e);
	/* Makes the button graphics. */
	void makeButtonGraphics();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	// Settings
	/* Gets the mode of the file dialog. Either opening or saving. */
	FileDialogModes getMode() const;
	/* Sets the mode of the file dialog. Either opening or saving. */
	void setMode(FileDialogModes mode);
	/* Gets the initial file name and returned file. */
	std::string getFileName() const;
	/* Sets the initial file name and returned file. */
	void setFileName(const std::string& fileName);
	/* Gets the default extension to save files with. Extensions must include the '.' when needed. */
	std::string getDefaultExt() const;
	/* Sets the default extension to save files with. Includes the '.' in the extension. */
	void setDefaultExt(const std::string& defaultExt);
	/* Gets if readonly files and folders are shown in the browser. */
	bool getAllowReadOnlyFiles() const;
	/* Sets if readonly files and folders are shown in the browser. */
	void setAllowReadOnlyFiles(bool allow);
	/* Gets if system files and folders are shown in the browser. */
	bool getAllowSystemFiles() const;
	/* Sets if system files and folders are shown in the browser. */
	void setAllowSystemFiles(bool allow);
	/* Sets if system or readonly files and folders are shown in the browser. */
	void setAllowedFiles(bool allowReadOnly, bool allowSystem);
	/* Gets the initial directory of the file dialog. */
	std::string getInitialDirectory() const;
	/* Sets the initial directory of the file dialog. */
	void setInitialDirectory(const std::string& initialDirectory);

	// Custom Places
	/* Returns true if the file dialog has any custom places. */
	bool hasCustomPlaces() const;
	/* Adds a custom place to the common locations bar on the left side of the file dialog. */
	void addCustomPlace(const std::string& path, const std::string& name = "");

	// Libraries
	/* Adds a library to the common locations bar on the left side of the file dialog. */
	void addLibrary(const std::string& path, const std::string& name = "");

	// Filters
	/* Returns true if the file dialog has any filters. */
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
private:
	/* Updates the file path label. */
	void updateFilePath(const std::string& path);

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers

	/* Gets the file name from the textbox and adds the extension if necessary. */
	/* Then shows the message box confirming to overwrite the file if needed. */
	void finishSavingOpening();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onOpenSaveClicked(ControlSPtr sender, MouseEventArgs& e);
	void onCancelClicked(ControlSPtr sender, MouseEventArgs& e);
	void onRefreshClicked(ControlSPtr sender, MouseEventArgs& e);
	void onForwardClicked(ControlSPtr sender, MouseEventArgs& e);
	void onBackClicked(ControlSPtr sender, MouseEventArgs& e);
	void onFileChosen(ControlSPtr sender, PcIntr::FileSelectedEventArgs& e);
	void onFileSelected(ControlSPtr sender, PcIntr::FileSelectedEventArgs& e);
	void onFileListDirectoryChanged(ControlSPtr sender, PcIntr::DirectoryChangedEventArgs& e);
	void onLocationDirectoryChanged(ControlSPtr sender, PcIntr::DirectoryChangedEventArgs& e);
	void onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e);

	#pragma endregion
};

/* The file dialog class for opening files in a browser. */
class OpenFileDialog : public FileDialog {
public:
	inline OpenFileDialog()
		: FileDialog(FileDialogModes::Open) {}
};
/* The file dialog class for saving files in a browser. */
class SaveFileDialog : public FileDialog {
public:
	inline SaveFileDialog()
		: FileDialog(FileDialogModes::Save) {}
};

DECLARE_SMART_PTRS(FileDialog);
DECLARE_SMART_PTRS(OpenFileDialog);
DECLARE_SMART_PTRS(SaveFileDialog);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
