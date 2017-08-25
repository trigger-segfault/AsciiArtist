/*=================================================================|
|* File:				FileList.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FileList.h"
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/Drawing/Borders.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::IO;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace PowerConsole::Controls::Toolbox::Internal;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

FileList::FileList()
	:	// Items
	directory(),
	selection(-1),
	hoverIndex(-1),
	pressedIndex(-1),

	// Settings
	filters(),
	allowReadOnly(true),
	allowSystem(false),

	// Columns
	columns(),
	columnDraggingIndex(-1),
	columnHoverIndex(-1),

	// Functions
	functionLoadFileDetails(),
	functionDrawFileDetail(),

	// Events
	_eventSelectionChanged(EventDirections::FirstInFirstCalled),
	_eventDirectoryChanged(EventDirections::FirstInFirstCalled),
	_eventFileDoubleClicked(EventDirections::FirstInFirstCalled),
	_eventFileSelected(EventDirections::FirstInFirstCalled) {

	eventInitialize().add(bindEvent(FileList::onInitialize));
	eventLayoutChanged().add(bindEvent(FileList::onLayoutChanged));
	eventPaint().add(bindEvent(FileList::onPaint));
	eventMouseMovedGlobal().add(bindEvent(FileList::onMouseMovedGlobal));
	eventMouseWheelGlobal().add(bindEvent(FileList::onMouseWheelGlobal));
	eventMouseButton().add(bindEvent(FileList::onMouseButton));
	eventDoubleClick().add(bindEvent(FileList::onDoubleClick));

	setPadding(Thickness(1, 3, 1, 1));
	setIncrement(Point2I(3));

	columns.push_back(DetailColumn("Name", DETAIL_FILE_NAME, 49, 20));
	columns.push_back(DetailColumn("Modified", DETAIL_FILE_MODIFIED, 16, 6));
	columns.push_back(DetailColumn("Size", DETAIL_FILE_SIZE, 10, 9));
}
void FileList::onInitialize(ControlSPtr sender, EventArgs& e) {
	setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Selection
FileInfo* FileList::getSelectedItem() {
	if (selection == -1)
		return nullptr;
	return &directory.fileList[selection];
}
int FileList::getSelectedIndex() const {
	return selection;
}
void FileList::setSelectedIndex(int index) {
	if (index < -1 && index >= (int)directory.fileList.size())
		throw std::out_of_range("Invalid index in 'FileList::setSelectedIndex(...)'");

	if (index != selection) {
		int oldSelection = selection;
		selection = index;
		_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(selection, oldSelection));
		if (selection != -1 && directory.fileList[selection].fileType == FileTypes::File) {
			_eventFileSelected(shared_from_this(), FileSelectedEventArgs(&directory.fileList[selection]));
		}
	}
}

// Navigation
std::string FileList::getCurrentDirectory() const {
	return directory.directoryFile.path;
}
bool FileList::navigateUp() {
	if (directory.fileList[0].parentDirectory) {
		loadDirectory(directory.fileList[0].path);
		return true;
	}
	return false;
}

// Settings
bool FileList::getAllowReadOnlyFiles() const {
	return allowReadOnly;
}
void FileList::setAllowReadOnlyFiles(bool allow) {
	this->allowReadOnly = allow;
}
bool FileList::getAllowSystemFiles() const {
	return allowSystem;
}
void FileList::setAllowSystemFiles(bool allow) {
	this->allowSystem = allow;
}
void FileList::setAllowedFiles(bool allowReadonly, bool allowSystem) {
	this->allowReadOnly = allowReadOnly;
	this->allowSystem = allowSystem;
}

// File Details
void FileList::addColumn(const DetailColumn& column) {
	columns.push_back(column);
}
void FileList::insertColumn(int index, const DetailColumn& column) {
	if (index > 0) {
		columns.insert(columns.begin() + index, column);
	}
}
void FileList::setColumnWidth(int id, int width) {
	auto it = find_if(columns.begin(), columns.end(), [id](DetailColumn& column) {
		return column.id == id;
	});
	if (it != columns.end()) {
		it->width = width;
	}
}
DetailColumn FileList::getColumn(int id) const {
	auto it = find_if(columns.begin(), columns.end(), [id](const DetailColumn& column) {
		return column.id == id;
	});
	if (it != columns.end()) {
		*it;
	}
	return DetailColumn();
}
void FileList::removeColumn(int id) {
	auto it = find_if(columns.begin() + 1, columns.end(), [id](DetailColumn& column) {
		return column.id == id;
	});
	if (it != columns.end()) {
		columns.erase(it);
	}
}
void FileList::clearColumns() {
	columns.erase(columns.begin() + 1, columns.end());
}

// Functions
void FileList::setLoadFileDetailsFunction(LoadFileDetailsFunction func) {
	functionLoadFileDetails = func;
}
void FileList::setDrawFileDetailFunction(DrawFileDetailFunction func) {
	functionDrawFileDetail = func;
}

#pragma endregion
//=========== FILTERS ============
#pragma region Filters

bool FileList::hasFilters() const {
	return !filters.empty();
}
void FileList::addFilter(const std::string& extension) {
	filters.push_back(extension);
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void FileList::loadDirectory(const FileInfo& file) {
	loadDirectory(file.path);
}
void FileList::loadDirectory(const std::string& path) {
	std::string previousDirectory = directory.directoryFile.path;
	selection = -1;
	columnDraggingIndex = -1;
	columnHoverIndex = -1;
	hoverIndex = -1;
	//directory = DirectoryInfo();
	invalidateMousePosition();
	if (!directory.load(path)) {
		if (directory.directoryFile.fileType == FileTypes::Computer) {
			// That's it. Give up. There's nothing more we can do.
			// Throw error event
		}
		FileInfo parent = FileInfo();
		if (!previousDirectory.empty() && (parent.load(previousDirectory) || parent.load(Path::COMPUTER_DIRECTORY))) {
			parent.name = "Back";
			parent.parentDirectory = true;
			directory.fileList.clear();
			directory.fileList.push_back(parent);
		}
		else {
			loadDirectory(FileInfo(Path::COMPUTER_DIRECTORY));
		}
	}
	else {
		if (!allowReadOnly)
			directory.removeFileAttributes(FileAttributes::ReadOnly);
		if (!allowSystem)
			directory.removeFileAttributes(FileAttributes::System);
		if (!filters.empty())
			directory.removeFileExtensions(filters, true);

		for (FileInfo& file : directory.fileList) {
			if (file.fileType == FileTypes::File) {
				if ((bool)functionLoadFileDetails) {
					functionLoadFileDetails(file);
				}
			}
		}
	}
	updateScrollBars();
}
void FileList::refresh() {
	loadDirectory(directory.directoryFile.path);
}

#pragma endregion
//========== SCROLLING ===========
#pragma region Scrolling

void FileList::scrollIntoView(int index) {
	int y = index - getViewPosition().y;
	int offset = GMath::min(getViewportSize().y / 2, 2);
	if (y - offset < 0) {
		setViewPosition(Point2I(0, GMath::max(y - offset, 0)), true);
	}
	else if (y + offset >= getViewportSize().y) {
		setViewPosition(Point2I(0, y + offset - getViewportSize().y + 1), true);
	}
}
void FileList::scrollSelectionIntoView() {
	scrollIntoView(GMath::max(0, selection));
}
int FileList::getItemIndexFromMousePosition(Point2I position) const {
	Point2I paddedPosition = position - getPadding().topLeft();
	if (getLocalVisibleArea().containsPoint(position) && paddedPosition >= Point2I::ZERO && paddedPosition < getViewportSize()) {
		int index = paddedPosition.y + getViewPosition().y;
		return (index < (int)directory.fileList.size() ? index : -1);
	}
	return -1;
}
void FileList::updateScrollBars() {
	resizeScrollBars(Point2I(getActualSize().x - getPadding().horizontal(), (int)directory.fileList.size() + BOTTOM_SPACING));
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<SelectionChangedEventArgs>& FileList::eventSelectionChanged() {
	return _eventSelectionChanged;
}
Event<DirectoryChangedEventArgs>& FileList::eventDirectoryChanged() {
	return _eventDirectoryChanged;
}
Event<FileSelectedEventArgs>& FileList::eventFileDoubleClicked() {
	return _eventFileDoubleClicked;
}
Event<FileSelectedEventArgs>& FileList::eventFileSelected() {
	return _eventFileSelected;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void FileList::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	g->setExtraTranslation(getPadding().topLeft() - Point2I(0, 2));
	g->setConnectGlobalBorders(true);

	unsigned char color;
	const unsigned char borderColor = toColor(ConsoleColors::BLACK, ConsoleColors::DARK_GRAY);
	const unsigned char borderHoverColor = toColor(ConsoleColors::BLACK, ConsoleColors::GRAY);

	const unsigned char computerColor = toColor(ConsoleColors::BLACK, ConsoleColors::DARK_CYAN);
	const unsigned char parentColor = toColor(ConsoleColors::BLACK, ConsoleColors::DARK_GREEN);
	const unsigned char folderColor = toColor(ConsoleColors::BLACK, ConsoleColors::DARK_YELLOW);
	const unsigned char driveColor = toColor(ConsoleColors::BLACK, ConsoleColors::DARK_MAGENTA);
	const unsigned char shortcutColor = toColor(ConsoleColors::BLACK, ConsoleColors::CYAN);
	const unsigned char itemColor = toColor(ConsoleColors::BLACK, ConsoleColors::GRAY);
	const unsigned char itemHoverColor = toColor(ConsoleColors::BLUE, ConsoleColors::WHITE);
	const unsigned char itemPressColor = toColor(ConsoleColors::BLUE, ConsoleColors::DARK_GRAY);
	const unsigned char errorColor = toColor(ConsoleColors::BLACK, ConsoleColors::RED);
	const unsigned char errorHoverColor = toColor(ConsoleColors::DARK_RED, ConsoleColors::WHITE);
	const unsigned char errorPressColor = toColor(ConsoleColors::DARK_RED, ConsoleColors::DARK_GRAY);

	int offset;
	#pragma region Draw Columns
	// Draw the file horizontal border separating the column headers from the file list
	g->drawBorder(Rectangle2I(-1, 1, getActualSize().x - getPadding().horizontal() + 2, 1), Pixel(' ', borderColor), false);
	// Draw the column Headers
	offset = 2;
	for (int i = 0; i < (int)columns.size(); i++) {
		DetailColumn detail = columns[i];
		g->drawString(Point2I(offset, 0), detail.name, Pixel(' ', itemColor));
		offset += detail.width;

		color = borderColor;
		if (i == columnHoverIndex || i == columnDraggingIndex)
			color = borderHoverColor;
		if (i != (int)columns.size() - 1)
			g->setPixel(Point2I(offset - 2, 0), Pixel(Borders::LINE_THIN_VERTICAL, color));
		g->setPixel(Rectangle2I(offset - 1, 0, getActualSize().x - getPadding().horizontal() - offset + 1, 1), Pixel(' ', itemColor));
	}
	#pragma endregion

	// Start the region for the file list
	g->startRegion(getPadding().topLeft(), (getLocalVisibleArea() - getPadding().topLeft()).intersect(getViewportSize()));

	// Draw the files visible in the list
	for (int i = getViewPosition().y; i < getViewPosition().y + getViewportSize().y && i < (int)directory.fileList.size(); i++) {
		FileInfo& file = directory.fileList[i];
		int y = i - getViewPosition().y;
		offset = 2;

		#pragma region Draw Directory Arrows
		if (file.parentDirectory) {
			if (file.fileType == FileTypes::Computer)
				g->setPixel(Point2I(offset, y), Pixel(Characters::BIG_ARROW_LEFT, computerColor));
			else
				g->setPixel(Point2I(offset, y), Pixel(Characters::BIG_ARROW_LEFT, parentColor));
		}
		else if (file.fileType == FileTypes::Directory) {
			g->setPixel(Point2I(offset, y), Pixel(Characters::BIG_ARROW_RIGHT, folderColor));
		}
		else if (file.fileType == FileTypes::Drive) {
			g->setPixel(Point2I(offset, y), Pixel(Characters::BIG_ARROW_RIGHT, driveColor));
		}
		else if (file.fileType == FileTypes::Shortcut) {
			if (file.shortcutType == FileTypes::Directory || file.shortcutType == FileTypes::Drive)
				g->setPixel(Point2I(offset, y), Pixel('>', shortcutColor));
			else
				g->setPixel(Point2I(offset, y), Pixel(Characters::SMALL_ARROW_RIGHT, shortcutColor));
		}
		#pragma endregion

		#pragma region Draw Filename
		std::string fileName;
		if (file.fileType == FileTypes::Shortcut) {
			fileName = Path::removeExtension(file.name);
		}
		else {
			fileName = file.name;
		}
		int maxSize;
		std::string drawString;
		offset = 2;
		maxSize = (file.fileType == FileTypes::Drive && !file.parentDirectory ? 20 : columns[0].width - 4);
		if (maxSize > 0) {
			if ((int)fileName.length() > maxSize)
				g->drawString(Point2I(offset + 2, y), fileName.substr(0, maxSize - 1) + "..", Pixel(' ', itemColor));
			else
				g->drawString(Point2I(offset + 2, y), fileName, Pixel(' ', itemColor));
		}
		offset += columns[0].width;
		#pragma endregion

		if (file.fileType == FileTypes::File) {
			for (int i = 1; i < (int)columns.size(); i++) {
				DetailColumn& detail = columns[i];
				switch (detail.id) {
					#pragma region Draw File Detils
				case DETAIL_FILE_SIZE:
					g->drawString(Point2I(offset, y), Path::getFileSizeString(file.fileSize), Pixel(' ', itemColor), HorizontalAlignments::Right, detail.width - 2);
					break;
				case DETAIL_FILE_MODIFIED:
					drawString = Path::getFileTimeString(file.lastModified);
					if (maxSize > 0) {
						if ((int)drawString.length() > detail.width - 2)
							g->drawString(Point2I(offset, y), drawString.substr(0, detail.width - 3) + "..", Pixel(' ', itemColor));
						else
							g->drawString(Point2I(offset, y), drawString, Pixel(' ', itemColor));
					}
					break;
				case DETAIL_FILE_CREATED:
					drawString = Path::getFileTimeString(file.creationDate);
					if (maxSize > 0) {
						if ((int)drawString.length() > detail.width - 2)
							g->drawString(Point2I(offset, y), drawString.substr(0, detail.width - 3) + "..", Pixel(' ', itemColor));
						else
							g->drawString(Point2I(offset, y), drawString, Pixel(' ', itemColor));
					}
					break;
					#pragma endregion

					#pragma region Draw Custom Details
				default:
					if ((bool)functionDrawFileDetail && !file.isInvalid()) {
						DrawFileDetailArgs args = DrawFileDetailArgs(i, &file);
						functionDrawFileDetail(args);
						if (detail.width - 2 > 0) {
							if ((int)args.outString.length() > detail.width - 2)
								g->drawString(Point2I(offset, y), args.outString.substr(0, detail.width - 3) + "..", Pixel(' ', itemColor));
							else
								g->drawString(Point2I(offset, y), args.outString, Pixel(' ', itemColor));
						}
					}
					#pragma endregion
				}
				offset += detail.width;
			}
		}
		else if (file.fileType == FileTypes::Drive && !file.parentDirectory) {
			// Draw drive capacity
			#pragma region Draw drive capacity
			if (file.fileSize != 0UL) {
				const unsigned char driveBarColor = toColor(ConsoleColors::BLACK, ConsoleColors::DARK_CYAN);
				const unsigned char driveBackColor = toColor(ConsoleColors::BLACK, ConsoleColors::DARK_CYAN);
				int availableSpace = getActualSize().x - getPadding().horizontal() - 4 - 24;
				int x = 26;
				const int barLength = 14;
				int usageAmount = (int)((double)file.usedSize / (double)file.fileSize * barLength);
				g->setPixel(Rectangle2I(x, y, usageAmount, 1), Pixel(Characters::BLOCK_FULL, driveBarColor));
				g->setPixel(Rectangle2I(x + usageAmount, y, barLength - usageAmount, 1), Pixel(Characters::GRADIENT_MEDIUM, driveBackColor));
				x += barLength + 2;
				std::string usageStr = Path::getFileSizeString(file.usedSize) + " / " + Path::getFileSizeString(file.fileSize);
				g->drawString(Point2I(x, y), usageStr, Pixel(' ', itemColor));
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region Set the row color
		if (!file.isInvalid()) {
			if (i == pressedIndex)
				color = itemPressColor;
			else if (i == hoverIndex || i == selection)
				color = itemHoverColor;
			else
				color = itemColor;
		}
		else {
			if (i == pressedIndex)
				color = errorPressColor;
			else if (i == hoverIndex || i == selection)
				color = errorHoverColor;
			else
				color = errorColor;
		}
		if (color != itemColor)
			g->setColor(Rectangle2I(0, y, getViewportSize().x, 1), color);
		#pragma endregion
	}

	// End the region for the file list
	g->endRegion();
	g->resetOptions();
}
void FileList::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		int offset = 0;
		// Column compression logic
		for (int j = 0; j < (int)columns.size(); j++) {
			offset += columns[j].width;
		}
		const int totalMaxSize = getActualSize().x - getPadding().horizontal() - 3;
		if (offset <= totalMaxSize) {
			offset -= columns.back().width;
			columns.back().width = totalMaxSize - offset;
		}
		else {
			int backOffset = 0;
			for (int j = (int)columns.size() - 1; j >= 0 && (offset + backOffset > totalMaxSize); j--) {
				offset -= columns[j].width;
				columns[j].width = GMath::max(columns[j].minWidth, totalMaxSize - offset - backOffset);
				backOffset += columns[j].width;
			}
		}
	}
}
void FileList::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	static int hoverCount = 0;
	hoverIndex = getItemIndexFromMousePosition(e.newPosition);

	if (hoverIndex == -1) {
		hoverCount++;
		//OutputDebugStringA((to_string(hoverCount) + "\n").c_str());
	}
	// Get the column hover ID
	columnHoverIndex = -1;
	int offset = 0;
	if (columnDraggingIndex == -1) {
		if (e.newPosition.y == getPadding().top - 2) {
			for (int i = 0; i < (int)columns.size(); i++) {
				offset += columns[i].width;
				if (e.newPosition.x - getPadding().left == offset) {
					columnHoverIndex = i;
					break;
				}
			}
		}
	}
	else {
		for (int i = 0; i < (int)columns.size(); i++) {
			if (i == DETAIL_FILE_SIZE)
				continue;
			if (i == columnDraggingIndex) {
				columns[i].width = GMath::max(columns[i].minWidth, e.newPosition.x - getPadding().left - offset);

				// Column compression logic
				for (int j = i; j < (int)columns.size(); j++) {
					offset += columns[j].width;
				}
				const int totalMaxSize = getActualSize().x - getPadding().horizontal() - 3;
				if (offset <= totalMaxSize) {
					offset -= columns.back().width;
					columns.back().width = totalMaxSize - offset;
				}
				else {
					int backOffset = 0;
					for (int j = (int)columns.size() - 1; j >= 0 && (offset + backOffset > totalMaxSize); j--) {
						offset -= columns[j].width;
						columns[j].width = GMath::max(columns[j].minWidth, totalMaxSize - offset - backOffset);
						backOffset += columns[j].width;
					}
				}
				break;
			}
			offset += columns[i].width;
		}
	}
}
void FileList::onMouseWheelGlobal(ControlSPtr sender, MouseWheelEventArgs& e) {
	onMouseMovedGlobal(sender, MouseEventArgs(e.newPosition, e.previousPosition));
}
void FileList::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left)) {
		if (hoverIndex != -1) {
			pressedIndex = hoverIndex;
			int oldSelection = selection;
			if (pressedIndex != selection) {
				selection = hoverIndex;
				_eventSelectionChanged(shared_from_this(), SelectionChangedEventArgs(selection, oldSelection));
				if (selection != -1 && directory.fileList[selection].fileType == FileTypes::File) {
					_eventFileSelected(shared_from_this(), FileSelectedEventArgs(&directory.fileList[selection]));
				}
			}
		}
		else if (columnHoverIndex != -1) {
			columnDraggingIndex = columnHoverIndex;
		}
	}
	else if (e.isButtonReleased(MouseButtons::Left)) {
		pressedIndex = -1;
		if (columnDraggingIndex != -1) {
			columnDraggingIndex = -1;
			onMouseMovedGlobal(sender, MouseEventArgs(e.newPosition, e.previousPosition));
		}
	}
}
void FileList::onDoubleClick(ControlSPtr sender, MouseEventArgs& e) {
	if (hoverIndex != -1) {
		int viewPosition = getViewPosition().y;
		FileInfo file = directory.fileList[hoverIndex];
		if (file.fileType == FileTypes::Directory || file.fileType == FileTypes::Drive ||
			file.fileType == FileTypes::Computer) {
			loadDirectory(file);
			_eventDirectoryChanged(shared_from_this(), DirectoryChangedEventArgs(file.path, viewPosition));
		}
		else if (file.fileType == FileTypes::Shortcut) {
			if (file.shortcutType == FileTypes::Directory || file.shortcutType == FileTypes::Drive ||
				file.shortcutType == FileTypes::Computer) {
				loadDirectory(file.shortcutPath);
				_eventDirectoryChanged(shared_from_this(), DirectoryChangedEventArgs(file.shortcutPath, viewPosition));
			}
			else {
				loadDirectory(file.path);
				_eventDirectoryChanged(shared_from_this(), DirectoryChangedEventArgs(file.path, viewPosition));
			}
		}
		else if (file.fileType == FileTypes::File || file.fileType == FileTypes::Shortcut) {
			_eventFileDoubleClicked(shared_from_this(), FileSelectedEventArgs(&file));
		}
	}
}

#pragma endregion
//=================================================================|
