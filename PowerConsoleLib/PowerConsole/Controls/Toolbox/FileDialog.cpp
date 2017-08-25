/*=================================================================|
|* File:				Blank.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FileDialog.h"
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Controls/Toolbox/TextEditBox.h>
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <PowerConsole/Helpers/StringHelpers.h>

#undef MessageBox

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace PowerConsole::Controls::Toolbox::Internal;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

FileDialog::FileDialog(FileDialogModes mode)
 :	// Setup
	Dialog(DialogModes::EntireWindow),

	// History
	historyIndex(0),

	mode(mode),
	fileName(""),
	initialDirectory(Path::getCurrentDirectory()),
	defaultExt(""),
	filters(),
	allowReadOnly(true),
	allowSystem(false) {

	eventInitialize().add(bindEvent(FileDialog::onInitialize));
	eventLoad().add(bindEvent(FileDialog::onLoad));
	eventPaint().add(bindEvent(FileDialog::onPaint));
	eventKeyGlobal().add(bindEvent(FileDialog::onKeyGlobal));
}
void FileDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	
	#pragma region Window Settings

	setResizeHandleEnabled(true);
	console().setPalette(ConsolePalette::DEFAULT);
	console().setFont(ConsoleFont::DEFAULT);

	#pragma endregion

	#pragma region Dialog Buttons

	buttonOpenSave = makeControl(Button);
	buttonOpenSave->setSize(Point2I(8, 1));
	buttonOpenSave->setText(mode == FileDialogModes::Open ? "Open" : "Save");
	buttonOpenSave->setHorizontalAlignment(HorizontalAlignments::Right);
	buttonOpenSave->setVerticalAlignment(VerticalAlignments::Bottom);
	buttonOpenSave->setMargin(Thickness(0, 0, 10, 1));
	buttonOpenSave->eventClick().add(bindEvent(FileDialog::onOpenSaveClicked));
	children().add(buttonOpenSave);

	buttonCancel = makeControl(Button);
	buttonCancel->setSize(Point2I(8, 1));
	buttonCancel->setText("Cancel");
	buttonCancel->setHorizontalAlignment(HorizontalAlignments::Right);
	buttonCancel->setVerticalAlignment(VerticalAlignments::Bottom);
	buttonCancel->setMargin(Thickness(0, 0, 1, 1));
	buttonCancel->eventClick().add(bindEvent(FileDialog::onCancelClicked));
	children().add(buttonCancel);

	#pragma endregion

	#pragma region Navigation Buttons

	buttonRefresh = makeControl(ImageButton);
	buttonRefresh->setSize(Point2I(3, 1));
	buttonRefresh->setMargin(Thickness(0, 1, 1, 0));
	buttonRefresh->setHorizontalAlignment(HorizontalAlignments::Right);
	buttonRefresh->eventClick().add(bindEvent(FileDialog::onRefreshClicked));
	children().add(buttonRefresh);

	buttonForward = makeControl(ImageButton);
	buttonForward->setSize(Point2I(3, 1));
	buttonForward->setMargin(Thickness(0, 1, 5, 0));
	buttonForward->setHorizontalAlignment(HorizontalAlignments::Right);
	buttonForward->setEnabled(false);
	buttonForward->eventClick().add(bindEvent(FileDialog::onForwardClicked));
	children().add(buttonForward);

	buttonBack = makeControl(ImageButton);
	buttonBack->setSize(Point2I(3, 1));
	buttonBack->setMargin(Thickness(0, 1, 9, 0));
	buttonBack->setHorizontalAlignment(HorizontalAlignments::Right);
	buttonBack->setEnabled(false);
	buttonBack->eventClick().add(bindEvent(FileDialog::onBackClicked));
	children().add(buttonBack);

	makeButtonGraphics();

	#pragma endregion

	#pragma region Labels

	labelPath = makeControl(Label);
	labelPath->setSize(Point2I(SIZE_AUTO, 1));
	labelPath->setHorizontalAlignment(HorizontalAlignments::Stretch);
	labelPath->setMargin(Thickness(13, 1, 14, 0));
	children().add(labelPath);

	labelFilePath = makeControl(Label);
	labelFilePath->setText("File Path:");
	labelFilePath->setMargin(Thickness(2, 1, 0, 0));
	children().add(labelFilePath);

	labelFileName = makeControl(Label);
	labelFileName->setText("File Name:");
	labelFileName->setVerticalAlignment(VerticalAlignments::Bottom);
	labelFileName->setMargin(Thickness(2, 0, 0, 1));
	children().add(labelFileName);

	labelCommonLocations = makeControl(Label);
	labelCommonLocations->setText("Common Locations");
	labelCommonLocations->setMargin(Thickness(2, 3, 0, 0));
	children().add(labelCommonLocations);

	#pragma endregion

	#pragma region Text Box & File List

	textBoxName = makeControl(TextBox);
	textBoxName->setText(fileName);
	textBoxName->setSize(Point2I(SIZE_AUTO, 1));
	textBoxName->setHorizontalAlignment(HorizontalAlignments::Stretch);
	textBoxName->setVerticalAlignment(VerticalAlignments::Bottom);
	textBoxName->setMargin(Thickness(13, 0, 20, 1));
	textBoxName->setSelectAllOnFocus(true);
	children().add(textBoxName);

	fileList = makeControl(FileList);
	fileList->setSize(Point2I(SIZE_AUTO));
	fileList->setHorizontalAlignment(HorizontalAlignments::Stretch);
	fileList->setVerticalAlignment(VerticalAlignments::Stretch);
	fileList->setMargin(Thickness(26, 2, 0, 2));
	fileList->setAllowedFiles(allowReadOnly, allowSystem);
	fileList->eventDirectoryChanged().add(bindEvent(FileDialog::onFileListDirectoryChanged));
	fileList->eventFileSelected().add(bindEvent(FileDialog::onFileSelected));
	fileList->eventFileDoubleClicked().add(bindEvent(FileDialog::onFileChosen));
	fileList->addColumn(DetailColumn("Name", DETAIL_FILE_NAME, 47, 20));
	fileList->addColumn(DetailColumn("Modified", DETAIL_FILE_MODIFIED, 18, 6));
	fileList->addColumn(DetailColumn("Size", DETAIL_FILE_SIZE, 10, 9));
	children().add(fileList);
	#pragma endregion

	#pragma region Common Locations

	scrollViewerLocations = makeControl(ScrollViewer);
	scrollViewerLocations->setSize(Point2I(27, SIZE_AUTO));
	scrollViewerLocations->setVerticalAlignment(VerticalAlignments::Stretch);
	scrollViewerLocations->setMargin(Thickness(0, 4, 0, 2));
	scrollViewerLocations->setPadding(Thickness(1));
	scrollViewerLocations->setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	children().add(scrollViewerLocations);

	stackPanelLocations = makeControl(StackPanel);
	stackPanelLocations->setSize(Point2I(25, SIZE_AUTO));
	stackPanelLocations->setOrientation(Orientations::Vertical);
	stackPanelLocations->setVerticalAlignment(VerticalAlignments::Top);
	scrollViewerLocations->setContent(stackPanelLocations);

	locationsFavorites = makeControl(FileLocations);
	FileInfo favorites = FileInfo(Path::getSpecialFolderPath(FOLDERID_Links));
	favorites.name = "Favorites";
	locationsFavorites->setDirectory(favorites);
	locationsFavorites->eventDirectoryChanged().add(bindEvent(FileDialog::onLocationDirectoryChanged));
	stackPanelLocations->children().add(locationsFavorites);
	
	locationsLibraries = makeControl(PresetFileLocations);
	locationsLibraries->setSectionName("Libraries");
	/*locationsLibraries->addDirectory(FileInfo(Path::getSpecialFolderPath(FOLDERID_Documents)));
	locationsLibraries->addDirectory(FileInfo(Path::getSpecialFolderPath(FOLDERID_Music)));
	locationsLibraries->addDirectory(FileInfo(Path::getSpecialFolderPath(FOLDERID_Pictures)));
	locationsLibraries->addDirectory(FileInfo(Path::getSpecialFolderPath(FOLDERID_Videos)));*/
	locationsLibraries->eventDirectoryChanged().add(bindEvent(FileDialog::onLocationDirectoryChanged));
	stackPanelLocations->children().add(locationsLibraries);

	locationsComputer = makeControl(FileLocations);
	locationsComputer->setDirectory(Path::COMPUTER_DIRECTORY);
	locationsComputer->eventDirectoryChanged().add(bindEvent(FileDialog::onLocationDirectoryChanged));
	stackPanelLocations->children().add(locationsComputer);

	#pragma endregion

	#pragma region Commands

	// Commands
	setFocusDefault(shared_from_this());
	commands().add(Command("Backspace", Hotkey(Keys::Backspace), true,
		"[]", [this](ControlSPtr sender, EventArgs& e) {
		onBackClicked(shared_from_this(), MouseEventArgs());
	}));
	commands().add(Command("Back", Hotkey(Keys::Left, Modifiers::Alt), true,
		"[]", [this](ControlSPtr sender, EventArgs& e) {
		onBackClicked(shared_from_this(), MouseEventArgs());
	}));
	commands().add(Command("Forward", Hotkey(Keys::Right, Modifiers::Alt), true,
		"[]", [this](ControlSPtr sender, EventArgs& e) {
		onForwardClicked(shared_from_this(), MouseEventArgs());
	}));
	commands().add(Command("Up", Hotkey(Keys::Up, Modifiers::Alt), true,
		"[]", [this](ControlSPtr sender, EventArgs& e) {
		if (fileList->navigateUp()) {
			if (historyIndex != (int)history.size() - 1)
				history.erase(history.begin() + historyIndex + 1, history.end());
			history[historyIndex].viewPosition = fileList->getViewPosition().y;
			history.push_back(FileDialogHistory(fileList->getCurrentDirectory()));
			historyIndex++;
			if ((int)history.size() > MAX_HISTORY) {
				history.erase(history.begin(), history.begin() + ((int)history.size() - MAX_HISTORY));
				historyIndex = 29;
			}
			buttonBack->setEnabled(true);
			buttonForward->setEnabled(false);
			updateFilePath(fileList->getCurrentDirectory());
		}
	}));
	commands().add(Command("Delete File", Hotkey(Keys::Delete), true,
		"[]", [this](ControlSPtr sender, EventArgs& e) {
		if (fileList->getCurrentDirectory() == Path::COMPUTER_DIRECTORY) {
			MessageBox::show(getWindow(), "Cannot delete file inside of the computer directory.", "Delete File", DialogBoxIcons::Warning, SystemSounds::Warning);
		}
		else if (!Path::directoryExists(fileList->getCurrentDirectory())) {
			MessageBox::show(getWindow(), "Cannot delete file inside of a shortcut.", "Delete File", DialogBoxIcons::Warning, SystemSounds::Warning);
		}
		else {
			auto result = MessageBox::show(getWindow(), "Would you like to delete the selected file?", "Delete File", DialogBoxButtons::YesNo);
			if (result == DialogBoxResults::Yes) {
				std::string filePath = fileList->getSelectedItem()->path;
				if (Path::deleteFile(filePath, true)) {
					fileList->deleteFile();
				}
				else {
					MessageBox::show(getWindow(),
						"File deletion failed! This program may not have access to modify files in this directory.",
						"Delete File",
						DialogBoxIcons::Error,
						SystemSounds::Error
					);
				}
			}
		}
	}));
	commands().add(Command("Rename", Hotkey(Keys::F2), true,
		"[]", [this](ControlSPtr sender, EventArgs& e) {
		if (fileList->getCurrentDirectory() == Path::COMPUTER_DIRECTORY) {
			MessageBox::show(getWindow(), "Cannot rename file inside of the computer directory.", "Rename File", DialogBoxIcons::Warning, SystemSounds::Warning);
		}
		else if (!Path::directoryExists(fileList->getCurrentDirectory())) {
			MessageBox::show(getWindow(), "Cannot rename file inside of a shortcut.", "Rename File", DialogBoxIcons::Warning, SystemSounds::Warning);
		}
		else if (fileList->getSelectedIndex() > 0) {
			std::string extension = Path::getExtension(fileList->getSelectedItem()->name);
			std::string newName = Path::removeExtension(fileList->getSelectedItem()->name);
			auto result = TextEditBox::show(getWindow(), newName, "Rename File", DialogBoxButtons::OKCancel);
			if (!Path::hasExtension(newName))
				newName += extension;
			if (result == DialogBoxResults::OK) {
				std::string newPath = Path::combine(Path::getDirectory(fileList->getSelectedItem()->path), newName);
				if (!Path::fileExists(newPath)) {
					if (Path::renameFile(fileList->getSelectedItem()->path, newName)) {
						fileList->renameFile(newName);
					}
					else {
						MessageBox::show(getWindow(),
							"File rename failed! This program may not have access to modify files in this directory.",
							"Rename File",
							DialogBoxIcons::Error,
							SystemSounds::Error
						);
					}
				}
				else {
					MessageBox::show(getWindow(),
						"A file with that name already exists.",
						"Rename File",
						DialogBoxIcons::Warning,
						SystemSounds::Warning
					);
				}
			}
		}
	}));
	commands().add(Command("New Folder", Hotkey(Keys::N, Modifiers::Ctrl | Modifiers::Shift), true,
		"[]", [this](ControlSPtr sender, EventArgs& e) {
		if (fileList->getCurrentDirectory() == Path::COMPUTER_DIRECTORY) {
			MessageBox::show(getWindow(), "Cannot create folder inside of the computer directory.", "New Folder", DialogBoxIcons::Warning, SystemSounds::Warning);
		}
		else if (!Path::directoryExists(fileList->getCurrentDirectory())) {
			MessageBox::show(getWindow(), "Cannot create folder inside of a shortcut.", "New Folder", DialogBoxIcons::Warning, SystemSounds::Warning);
		}
		else {
			std::string folderName = "New Folder";
			auto result = TextEditBox::show(getWindow(), folderName, "New Folder Name", DialogBoxButtons::OKCancel);
			if (result == DialogBoxResults::OK) {
				std::string folderPath = Path::combine(fileList->getResolvedCurrentDirectory(), folderName);
				if (!Path::fileExists(folderPath)) {
					if (Path::createDirectory(folderPath)) {
						fileList->addFile(folderPath);
					}
					else {
						MessageBox::show(getWindow(),
							"Folder creation failed! This program may not have access to modify files in this directory.",
							"New Folder",
							DialogBoxIcons::Error,
							SystemSounds::Error
						);
					}
				}
				else {
					MessageBox::show(getWindow(),
						"A file with that name already exists.",
						"New Folder",
						DialogBoxIcons::Warning,
						SystemSounds::Warning
					);
				}
			}
		}
	}));

	#pragma endregion

	#pragma region Tab Stops

	tabStops().add(shared_from_this());
	tabStops().add(textBoxName);

	#pragma endregion
	setSize(Point2I(104, 42));
	setMinSize(Point2I(90, 30));
}
void FileDialog::onLoad(ControlSPtr sender, EventArgs& e) {
	// Custom Places
	if (!customPlaces.empty()) {
		locationsPlaces = makeControl(PresetFileLocations);
		locationsPlaces->setSectionName("Custom Places");
		for (const FileInfo& place : customPlaces) {
			locationsPlaces->addDirectory(place);
		}
		locationsPlaces->eventDirectoryChanged().add(bindEvent(FileDialog::onLocationDirectoryChanged));
		stackPanelLocations->children().insert(0, locationsPlaces);
	}

	// Libraries
	addLibrary(Path::getSpecialFolderPath(FOLDERID_Documents));
	addLibrary(Path::getSpecialFolderPath(FOLDERID_Music));
	addLibrary(Path::getSpecialFolderPath(FOLDERID_Pictures));
	addLibrary(Path::getSpecialFolderPath(FOLDERID_Videos));
	for (const FileInfo& library : libraries) {
		locationsLibraries->addDirectory(library);
	}
	fileList->loadDirectory(FileInfo(initialDirectory));
	history.push_back(FileDialogHistory(initialDirectory));
	updateFilePath(initialDirectory);

	textBoxName->focus();
}
void FileDialog::makeButtonGraphics() {
	GraphicsSPtr g;
	AsciiAnimation ascRefresh = AsciiAnimation(Point2I(3, 1), ImageFormats::Basic, 4);
	AsciiAnimation ascForward = AsciiAnimation(Point2I(3, 1), ImageFormats::Basic, 4);
	AsciiAnimation ascBack = AsciiAnimation(Point2I(3, 1), ImageFormats::Basic, 4);
	ascRefresh.setName(0, "Normal"); ascRefresh.setName(1, "Hover"); ascRefresh.setName(2, "Pressed"); ascRefresh.setName(3, "Disabled");
	ascForward.setName(0, "Normal"); ascForward.setName(1, "Hover"); ascForward.setName(2, "Pressed"); ascForward.setName(3, "Disabled");
	ascBack.setName(0, "Normal"); ascBack.setName(1, "Hover"); ascBack.setName(2, "Pressed"); ascBack.setName(3, "Disabled");

	std::string strRefresh = { (char)Characters::SMALL_ARROW_UP, (char)Characters::BLOCK_FULL, (char)Characters::SMALL_ARROW_DOWN };
	std::string strForward = { (char)Borders::CORNER_THIN_BOTTOM_THIN_RIGHT, (char)Borders::LINE_THIN_HORIZONTAL, (char)Characters::BIG_ARROW_RIGHT };
	std::string strBack = { (char)Characters::BIG_ARROW_LEFT, (char)Borders::LINE_THIN_HORIZONTAL, (char)Borders::CORNER_THIN_BOTTOM_THIN_LEFT };

	g = ascRefresh.createGraphics(); // Normal
	//g->drawString(Point2I::ZERO, strRefresh, Pixel(' ', toColor(BLACK, CYAN)));
	//g->setColor(Point2I(1, 0), toColor(BLACK, GRAY));
	g->drawString(Point2I::ZERO, strRefresh, Pixel(' ', toColor(BLACK, DARK_CYAN)));
	g->setColor(Point2I(1, 0), toColor(BLACK, GRAY));
	g->setCurrentFrame(1); // Hover
	g->drawString(Point2I::ZERO, strRefresh, Pixel(' ', toColor(BLACK, CYAN)));
	g->setColor(Point2I(1, 0), toColor(BLACK, WHITE));
	//g->drawString(Point2I::ZERO, strRefresh, Pixel(' ', toColor(BLACK, CYAN)));
	//g->setColor(Point2I(1, 0), toColor(BLACK, WHITE));
	g->setCurrentFrame(2); // Pressed
	g->drawString(Point2I::ZERO, strRefresh, Pixel(' ', toColor(BLACK, DARK_CYAN)));
	g->setColor(Point2I(1, 0), toColor(BLACK, DARK_GRAY));
	g->setCurrentFrame(3); // Disabled
	g->drawString(Point2I::ZERO, strRefresh, Pixel(' ', toColor(BLACK, DARK_GRAY)));
	buttonRefresh->setImage(std::move(ascRefresh));

	g = ascForward.createGraphics(); // Normal
	g->drawString(Point2I::ZERO, strForward, Pixel(' ', toColor(BLACK, DARK_GREEN)));
	g->setCurrentFrame(1); // Hover
	g->drawString(Point2I::ZERO, strForward, Pixel(' ', toColor(BLACK, GREEN)));
	g->setCurrentFrame(2); // Pressed
	g->drawString(Point2I::ZERO, strForward, Pixel(' ', toColor(BLACK, DARK_GREEN)));
	g->setCurrentFrame(3); // Disabled
	g->drawString(Point2I::ZERO, strForward, Pixel(' ', toColor(BLACK, DARK_GRAY)));
	buttonForward->setImage(std::move(ascForward));

	g = ascBack.createGraphics(); // Normal
	g->drawString(Point2I::ZERO, strBack, Pixel(' ', toColor(BLACK, DARK_GREEN)));
	g->setCurrentFrame(1); // Hover
	g->drawString(Point2I::ZERO, strBack, Pixel(' ', toColor(BLACK, GREEN)));
	g->setCurrentFrame(2); // Pressed
	g->drawString(Point2I::ZERO, strBack, Pixel(' ', toColor(BLACK, DARK_GREEN)));
	g->setCurrentFrame(3); // Disabled
	g->drawString(Point2I::ZERO, strBack, Pixel(' ', toColor(BLACK, DARK_GRAY)));
	buttonBack->setImage(std::move(ascBack));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Settings
FileDialogModes FileDialog::getMode() const {
	return mode;
}
void FileDialog::setMode(FileDialogModes mode) {
	this->mode = mode;
	this->buttonOpenSave->setText(mode == FileDialogModes::Open ? "Open" : "Save");
}
std::string FileDialog::getFileName() const {
	return fileName;
}
void FileDialog::setFileName(const std::string& fileName) {
	this->fileName = fileName;
	textBoxName->setText(Path::getFileName(fileName));
}
std::string FileDialog::getDefaultExt() const {
	return defaultExt;
}
void FileDialog::setDefaultExt(const std::string& defaultExt) {
	this->defaultExt = defaultExt;
}
bool FileDialog::getAllowReadOnlyFiles() const {
	return allowReadOnly;
}
void FileDialog::setAllowReadOnlyFiles(bool allow) {
	this->allowReadOnly = allow;
	if (fileList != nullptr)
		fileList->setAllowReadOnlyFiles(allow);
}
bool FileDialog::getAllowSystemFiles() const {
	return allowSystem;
}
void FileDialog::setAllowSystemFiles(bool allow) {
	this->allowSystem = allow;
	if (fileList != nullptr)
		fileList->setAllowSystemFiles(allow);
}
void FileDialog::setAllowedFiles(bool allowReadOnly, bool allowSystem) {
	this->allowReadOnly = allowReadOnly;
	this->allowSystem = allowSystem;
	if (fileList != nullptr)
		fileList->setAllowedFiles(allowReadOnly, allowSystem);
}
std::string FileDialog::getInitialDirectory() const {
	return initialDirectory;
}
void FileDialog::setInitialDirectory(const std::string& initialDirectory) {
	this->initialDirectory = Path::getFullPath(initialDirectory);
}

// Custom Places
bool FileDialog::hasCustomPlaces() const {
	return !customPlaces.empty();
}
void FileDialog::addCustomPlace(const std::string& path, const std::string& name) {
	FileInfo place = FileInfo(path);
	if (!name.empty())
		place.name = name;
	customPlaces.push_back(place);

	std::sort(customPlaces.begin(), customPlaces.end(), [](const FileInfo& placeA, const FileInfo& placeB)->bool {
		return _stricmp(placeA.name.c_str(), placeB.name.c_str()) < 0;
	});
}

// Libraries
void FileDialog::addLibrary(const std::string& path, const std::string& name) {
	FileInfo place = FileInfo(path);
	if (!name.empty())
		place.name = name;
	libraries.push_back(place);

	std::sort(libraries.begin(), libraries.end(), [](const FileInfo& libA, const FileInfo& libB)->bool {
		return _stricmp(libA.name.c_str(), libB.name.c_str()) < 0;
	});
}

// Filters
bool FileDialog::hasFilters() const {
	return fileList->hasFilters();
}
void FileDialog::addFilter(const std::string& extension) {
	fileList->addFilter(extension);
}

// File Details
void FileDialog::addColumn(const DetailColumn& column) {
	fileList->addColumn(column);
}
void FileDialog::insertColumn(int index, const DetailColumn& column) {
	fileList->insertColumn(index, column);
}
void FileDialog::setColumnWidth(int id, int width) {
	fileList->setColumnWidth(id, width);
}
DetailColumn FileDialog::getColumn(int id) const {
	return fileList->getColumn(id);
}
void FileDialog::removeColumn(int id) {
	fileList->removeColumn(id);
}
void FileDialog::clearColumns() {
	fileList->clearColumns();
}

// Functions
void FileDialog::setLoadFileDetailsFunction(LoadFileDetailsFunction func) {
	fileList->setLoadFileDetailsFunction(func);
}
void FileDialog::setDrawFileDetailFunction(DrawFileDetailFunction func) {
	fileList->setDrawFileDetailFunction(func);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void FileDialog::updateFilePath(const std::string& path) {
	labelPath->setText(path == Path::COMPUTER_DIRECTORY ? "Computer" : path);
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void FileDialog::finishSavingOpening() {
	if (fileList->getResolvedCurrentDirectory() == Path::COMPUTER_DIRECTORY) {
		MessageBox::show(getWindow(),
			(std::string)"Cannot " + (mode == FileDialogModes::Open ? "open" : "save") + " files in the computer directory",
			"Invalid Directory",
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
		return;
	}
	//else if (Path::getE
	fileName = Path::combine(fileList->getResolvedCurrentDirectory(), textBoxName->getText());
	if (!Path::hasExtension(fileName) && !defaultExt.empty())
		fileName = Path::renameExtension(fileName, defaultExt);
	if (!Path::isValidFileName(textBoxName->getText())) {
		MessageBox::show(getWindow(),
			"The given file name has one or more of following invalid characters:\n< > : \" / \\ | ? *",
			"Invalid File Name",
			DialogBoxIcons::Warning,
			SystemSounds::Warning,
			MessageBox::DEFAULT_MAX_WIDTH,
			MessageBox::DEFAULT_MIN_WIDTH,
			3
		);
	}
	else if (fileName == "." || fileName == ".." || fileName.empty()) {
		MessageBox::show(getWindow(),
			"\"" + fileName + "\" is an invalid file name.",
			"Invalid File Name",
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
	}
	else {
		
		if (mode == FileDialogModes::Save) {
			auto result = DialogBoxResults::Yes;
			if (Path::fileExists(fileName)) {
				result = MessageBox::show(getWindow(),
					"The selected file already exists. Would you like to overwrite it?",
					"File Already Exists",
					DialogBoxButtons::YesNo,
					DialogBoxIcons::Warning,
					SystemSounds::Warning
				);
			}
			if (result == DialogBoxResults::Yes) {
				Path::setCurrentDirectory(fileList->getResolvedCurrentDirectory());
				setDialogResult(true);
				close();
			}
		}
		else if (!Path::fileExists(fileName)) {
			MessageBox::show(getWindow(),
				"No file with the given name exists in this directory.",
				"No File Exists",
				DialogBoxIcons::Warning,
				SystemSounds::Warning
			);
		}
		else {
			Path::setCurrentDirectory(fileList->getResolvedCurrentDirectory());
			setDialogResult(true);
			close();
		}
	}
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void FileDialog::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	// Outside border
	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', 0x08), true);

	// Common Locations right border
	g->drawBorder(Rectangle2I(26, 2, 1, getActualSize().y - 4), Pixel(' ', 0x08), true);

	// Common Locations/File List label bottom border
	g->drawBorder(Rectangle2I(0, 4, 27, 1), Pixel(' ', 0x08), false);

	// File Path bottom border
	g->drawBorder(Rectangle2I(0, 2, getActualSize().x, 1), Pixel(' ', 0x08), true);

	// File Name top border
	g->drawBorder(Rectangle2I(0, getActualSize().y - 3, getActualSize().x, 1), Pixel(' ', 0x08), true);

	// Draw Save/Open and Cancel buttons border
	g->drawBorder(Rectangle2I(getActualSize() - Point2I(19, 3), Point2I(10, 3)), Pixel(' ', 0x08), true);

	// Draw the border for the back/forward button
	g->drawBorder(Rectangle2I(getActualSize().x - 5, 0, 1, 3), Pixel(' ', 0x08), true);
	g->drawBorder(Rectangle2I(getActualSize().x - 9, 0, 1, 3), Pixel(' ', 0x08), true);
	g->drawBorder(Rectangle2I(getActualSize().x - 13, 0, 1, 3), Pixel(' ', 0x08), true);
}
void FileDialog::onOpenSaveClicked(ControlSPtr sender, MouseEventArgs& e) {
	finishSavingOpening();
}
void FileDialog::onCancelClicked(ControlSPtr sender, MouseEventArgs& e) {
	setDialogResult(false);
	close();
}
void FileDialog::onRefreshClicked(ControlSPtr sender, MouseEventArgs& e) {
	fileList->refresh();
	locationsFavorites->refresh();
	locationsComputer->refresh();
}
void FileDialog::onForwardClicked(ControlSPtr sender, MouseEventArgs& e) {
	if (historyIndex < (int)history.size() - 1) {
		historyIndex++;
		fileList->loadDirectory(FileInfo(history[historyIndex].path));
		fileList->setViewPosition(Point2I(0, history[historyIndex].viewPosition));
		updateFilePath(history[historyIndex].path);
		buttonBack->setEnabled(true);
		buttonForward->setEnabled(historyIndex < (int)history.size() - 1);
	}
}
void FileDialog::onBackClicked(ControlSPtr sender, MouseEventArgs& e) {
	if (historyIndex > 0) {
		historyIndex--;
		fileList->loadDirectory(FileInfo(history[historyIndex].path));
		fileList->setViewPosition(Point2I(0, history[historyIndex].viewPosition));
		updateFilePath(history[historyIndex].path);
		buttonBack->setEnabled(historyIndex > 0);
		buttonForward->setEnabled(true);
	}
}
void FileDialog::onFileChosen(ControlSPtr sender, FileSelectedEventArgs& e) {
	textBoxName->setText(e.file->name);
	finishSavingOpening();
}
void FileDialog::onFileSelected(ControlSPtr sender, PcIntr::FileSelectedEventArgs& e) {
	textBoxName->setText(e.file->name);
}
void FileDialog::onFileListDirectoryChanged(ControlSPtr sender, DirectoryChangedEventArgs& e) {
	if (historyIndex != (int)history.size() - 1)
		history.erase(history.begin() + historyIndex + 1, history.end());
	history[historyIndex].viewPosition = e.viewPosition;
	history.push_back(e.directory);
	historyIndex++;
	if ((int)history.size() > MAX_HISTORY) {
		history.erase(history.begin(), history.begin() + ((int)history.size() - MAX_HISTORY));
		historyIndex = 29;
	}
	buttonBack->setEnabled(true);
	buttonForward->setEnabled(false);
	updateFilePath(e.directory);
}
void FileDialog::onLocationDirectoryChanged(ControlSPtr sender, DirectoryChangedEventArgs& e) {
	if (e.directory != fileList->getCurrentDirectory()) {
		if (historyIndex != (int)history.size() - 1)
			history.erase(history.begin() + historyIndex + 1, history.end());
		history[historyIndex].viewPosition = e.viewPosition;
		history.push_back(e.directory);
		historyIndex++;
		if ((int)history.size() > MAX_HISTORY) {
			history.erase(history.begin(), history.begin() + ((int)history.size() - MAX_HISTORY));
			historyIndex = 29;
		}
		buttonBack->setEnabled(true);
		buttonForward->setEnabled(false);

		fileList->loadDirectory(FileInfo(e.directory));
		updateFilePath(e.directory);
	}
}
void FileDialog::onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e) {
	if (e.isKeyPressed(Keys::Enter) && textBoxName->hasFocus()) {
		finishSavingOpening();
	}
	else if (e.isKeyPressed(Keys::Escape)) {
		setDialogResult(false);
		close();
	}
}

#pragma endregion
//=================================================================|
