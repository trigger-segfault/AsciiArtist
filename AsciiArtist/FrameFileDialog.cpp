/*=================================================================|
|* File:				FrameFileDialog.cpp						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/17/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "FrameFileDialog.h"
#include "ConsoleApp.h"
#include "Component.h"
#include "AsciiArtist.h"
#include "AsciiArtistManager.h"
#include "ComponentWindow.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Components;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;
using namespace PowerConsole::AsciiArtistApp;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region Frame
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::FrameFileDialog::FrameFileDialog(AsciiArtistManager* aa) : Frame() {
	this->_id			= "FileDialog";

	this->_aa			= aa;

	this->_resizing		= false;
	this->_resized		= false;
	this->_resizeHover	= false;

	this->_directory	= Directory();

	this->_returnFile	= File();
	this->_saving		= false;
	this->_canceled		= true;
	this->_pathHover	= false;

	this->_imageMode	= 0;
}
PowerConsole::FrameFileDialog::~FrameFileDialog() {
	
}
void PowerConsole::FrameFileDialog::Initialize(ConsoleApp* app) {
	Frame::Initialize(app);

	
	this->_directory.Load(File(FileManager::ComputerDirectory));
	this->_directory.RemoveFileExtensions(".asc", true);
	this->_directory.RemoveFileAttributes(FileAttributes::FileAttributeSystem);
	
	for (int i = 0; i < this->_directory.NumFiles(); i++) {
		if (this->_directory[i].Type == FileTypes::File) {
			AsciiImageHeader header = AsciiImageHeader();
			FileInputStream in = FileInputStream();
			in.Open(this->_directory[i].Path);
			if (in.IsOpen()) {
				AsciiImageInfo::LoadHeader(&in, header, false);
				in.Close();
			}
			this->_imageList.push_back(header);
		}
	}

	this->_commonList.push_back(File(FileManager::GetExecutableDirectory()));
	this->_commonList.push_back(File(FileManager::GetUserDirectory()));
	this->_commonList.push_back(File(FileManager::GetUserDirectory() + "\\" + "Desktop"));
	this->_commonList.push_back(File(FileManager::GetUserDirectory() + "\\" + "Documents"));
	this->_commonList.push_back(File(FileManager::GetUserDirectory() + "\\" + "Pictures"));
	this->_commonList.push_back(File(FileManager::GetUserDirectory() + "\\" + "Downloads"));
	this->_commonList[0].Name = "Program Directory";

	Directory computerDir = Directory();
	computerDir.Load(File(FileManager::ComputerDirectory));

	this->_computerList.push_back(computerDir.DirectoryFile);
	for (int i = 0; i < computerDir.NumFiles(); i++) {
		this->_computerList.push_back(computerDir[i]);
	}

	// Images
	this->_uiFileDialogImage = new AsciiImage();
	//this->_uiFileDialogImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\UIFileDialog.asc");
	this->_uiFileDialogImage->LoadResource(ASC_UI_FILE_DIALOG);
	this->_messageBoxImage = new AsciiImage();
	//this->_messageBoxImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\MessageBox.asc");
	this->_messageBoxImage->LoadResource(ASC_MESSAGE_BOX);

	// Save/Open
	this->_saveButton = new ComponentButton();
	this->_saveButton->SetSize(Coord(8, 1));
	this->_saveButton->SetText("Save");
	this->_saveButton->SetSpacing(Coord(2, 0));
	AddComponent(this->_saveButton);

	// Cancel
	this->_cancelButton = new ComponentButton();
	this->_cancelButton->SetSize(Coord(8, 1));
	this->_cancelButton->SetText("Cancel");
	this->_cancelButton->SetSpacing(Coord(1, 0));
	AddComponent(this->_cancelButton);

	// File Name
	this->_fileNameTextBox = new ComponentTextBox();
	this->_fileNameTextBox->SetSize(Coord(0, 1));
	this->_fileNameTextBox->SetText("Untitled.asc");
	AddComponent(this->_fileNameTextBox);

	// File List Scroll Bar
	this->_fileListScrollBar = new ComponentScrollBar();
	this->_fileListScrollBar->SetPosition(Coord(25, 5));
	this->_fileListScrollBar->SetSize(Coord(78, 34));
	this->_fileListScrollBar->SetVerticalBarEnabled(true);
	this->_fileListScrollBar->SetMouseWheelEnabled(true);
	AddComponent(this->_fileListScrollBar);
	
	// File List 
	this->_fileList = new ComponentFileList();
	this->_fileList->SetPosition(Coord(25, 5));
	this->_fileList->SetSize(Coord(78, 34));
	this->_fileList->SetScrollBar(this->_fileListScrollBar);
	this->_fileList->SetFileList(this->_directory.FileList.data(), this->_directory.NumFiles());
	this->_fileList->SetImageList(this->_imageList.data(), this->_imageList.size());
	AddComponent(this->_fileList);

	// Common Locations
	this->_commonLocationsList = new ComponentFileShortcutList();
	this->_commonLocationsList->SetPosition(Coord(1, 6));
	this->_commonLocationsList->SetSize(Coord(23, 6));
	this->_commonLocationsList->SetFileList(_commonList.data(), _commonList.size());
	AddComponent(this->_commonLocationsList);

	// Computer Locations
	this->_computerLocationsList = new ComponentFileShortcutList();
	this->_computerLocationsList->SetPosition(Coord(1, 15));
	this->_computerLocationsList->SetSize(Coord(23, 7));
	this->_computerLocationsList->SetFileList(_computerList.data(), _computerList.size());
	AddComponent(this->_computerLocationsList);
	
	// Recent Files
	this->_recentFilesList = new ComponentFileShortcutList();
	this->_recentFilesList->SetPosition(Coord(1, 27));
	this->_recentFilesList->SetSize(Coord(23, 11));
	this->_recentFilesList->SetFileList(_recentList.data(), _recentList.size());
	AddComponent(this->_recentFilesList);
}
void PowerConsole::FrameFileDialog::Uninitialize() {
	Frame::Uninitialize();
}
void PowerConsole::FrameFileDialog::OnEnter() {
	Frame::OnEnter();

	_app->SetConsoleSize(Coord(104, 42));
	//_fileListScrollBar->Resize(Coord(104, 42) - Coord(26, 8), Coord(0, _directory.NumFiles()), true);
	//_fileList->SetSize(Coord(Coord(104, 42) - Coord(26 + (_fileListScrollBar->IsVerticalBarActive() ? 2 : 0), 8)));
	_resized = true;
	_fileNameTextBox->SetTyping(true);
}
void PowerConsole::FrameFileDialog::OnLeave() {
	Frame::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment

File PowerConsole::FrameFileDialog::GetReturnFile() const {
	return _returnFile;
}
bool PowerConsole::FrameFileDialog::IsSaving() const {
	return _saving;
}
void PowerConsole::FrameFileDialog::SetSaving(bool saving) {
	this->_saving = saving;
}
void PowerConsole::FrameFileDialog::SetFileName(const string& fileName) {
	this->_fileNameTextBox->SetText(fileName);
}
int PowerConsole::FrameFileDialog::GetImageMode() const {
	return _imageMode;
}
void PowerConsole::FrameFileDialog::SetImageMode(int mode) {
	this->_imageMode = mode;
}

#pragma endregion
//--------------------------------
#pragma region Events

bool PowerConsole::FrameFileDialog::IsCanceled() const {
	return _canceled;
}
bool PowerConsole::FrameFileDialog::IsSuccessful() const {
	return !_canceled;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::FrameFileDialog::Update() {
	_pathHover = false;

	UpdateDimensions();

	Frame::Update();

	if (!IsWindowOpen()) {
		
		if (SmallRect(Coord(13, 1), Coord(_app->GetConsoleSize().X - 14, 1)).ContainsPoint(_app->GetMouse()->GetPosition())) {
			_pathHover = true;
			if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left)) {
				LoadDirectory(_directory.DirectoryFile);
			}
		}

		if (_fileList->IsSelectionDoubleClicked()) {
			if (_directory[_fileList->GetSelection()].Type == FileTypes::Directory || _directory[_fileList->GetSelection()].Type == FileTypes::Drive) {
				LoadDirectory(_directory[_fileList->GetSelection()]);
			}
			else {
				_fileNameTextBox->SetText(_directory[_fileList->GetSelection()].Name);
				File path = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
				if (path.Extension.empty()) {
					string path2 = path.Path;
					if (path.Name[path.Name.length() - 1] == '.') {
						path = File();
						path.Load(path2 + "asc");
					}
					else {
						path = File();
						path.Load(path2 + ".asc");
					}
				}

				if (_saving && FileManager::FileExists(path.Path)) {
					ComponentMessageBox* window = new ComponentMessageBox();
					window->SetBackgroundImage(_messageBoxImage);
					window->SetLine1("Image already exists");
					window->SetLine2("Overwrite image?");
					window->SetID("OverwriteMessage");
					OpenWindow(window);
				}
				else {
					_returnFile = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
					_app->ChangeFrame("Canvas");
					_canceled = false;
					Close();
				}
			}
		}
		else if (_commonLocationsList->IsSelectionClicked()) {
			LoadDirectory(_commonLocationsList->GetFileSelection());
		}
		else if (_computerLocationsList->IsSelectionClicked()) {
			LoadDirectory(_computerLocationsList->GetFileSelection());
		}
		else if (_recentFilesList->IsSelectionClicked()) {
			if (!_saving) {
				_returnFile = _recentFilesList->GetFileSelection();
				_app->ChangeFrame("Canvas");
				_canceled = false;
				Close();
			}
		}
		else if (_fileList->IsSelectionClicked()) {
			if (_directory[_fileList->GetSelection()].Type == FileTypes::File) {
				_fileNameTextBox->SetText(_directory[_fileList->GetSelection()].Name);
			}
		}

		int selection = _fileList->GetSelection();
		bool typing = _fileNameTextBox->IsTyping();
		if (_cancelButton->IsButtonPressed()) {
			_app->ChangeFrame("Canvas");
			_canceled = true;
			Close();
		}
		else if (_fileNameTextBox->IsFinishedTyping()) {
			_saveButton->SetText(_saving ? "Save" : "Open");
			if (_app->GetKeyboard()->IsKeyPressed(Keys::Enter) || _saveButton->IsButtonPressed()) {
				File path = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
				if (path.Extension.empty()) {
					string path2 = path.Path;
					if (path.Name[path.Name.length() - 1] == '.') {
						path = File();
						path.Load(path2 + "asc");
					}
					else {
						path = File();
						path.Load(path2 + ".asc");
					}
				}

				if (_saving && FileManager::FileExists(path.Path)) {
					ComponentMessageBox* window = new ComponentMessageBox();
					window->SetBackgroundImage(_messageBoxImage);
					window->SetLine1("Image already exists");
					window->SetLine2("Overwrite image?");
					window->SetID("OverwriteMessage");
					OpenWindow(window);
				}
				else {
					_returnFile = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
					_app->ChangeFrame("Canvas");
					_canceled = false;
					Close();
				}
			}
		}
		else if (selection != -1 && !typing) {
			if ((_directory[_fileList->GetSelection()].Type == FileTypes::Directory || _directory[_fileList->GetSelection()].Type == FileTypes::Drive)) {
				_saveButton->SetText("Open");
				if (_app->GetKeyboard()->IsKeyPressed(Keys::Enter) || _saveButton->IsButtonPressed()) {
					LoadDirectory(_directory[selection]);
				}
			}
			else {
				_saveButton->SetText(_saving ? "Save" : "Open");
				if (_app->GetKeyboard()->IsKeyPressed(Keys::Enter) || _saveButton->IsButtonPressed()) {
					File path = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
						if (path.Extension.empty()) {
							string path2 = path.Path;
							if (path.Name[path.Name.length() - 1] == '.') {
								path = File();
								path.Load(path2 + "asc");
							}
							else {
								path = File();
								path.Load(path2 + ".asc");
							}
						}

					if (_saving && FileManager::FileExists(path.Path)) {
						ComponentMessageBox* window = new ComponentMessageBox();
						window->SetBackgroundImage(_messageBoxImage);
						window->SetLine1("Image already exists");
						window->SetLine2("Overwrite image?");
						window->SetID("OverwriteMessage");
						OpenWindow(window);
					}
					else {
						_returnFile = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
						_app->ChangeFrame("Canvas");
						_canceled = false;
						Close();
					}
				}
			}
		}
		else {
			_saveButton->SetText(_saving ? "Save" : "Open");
			if (_app->GetKeyboard()->IsKeyPressed(Keys::Enter) || _saveButton->IsButtonPressed()) {
				File path = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
				if (path.Extension.empty()) {
					string path2 = path.Path;
					if (path.Name[path.Name.length() - 1] == '.') {
						path = File();
						path.Load(path2 + "asc");
					}
					else {
						path = File();
						path.Load(path2 + ".asc");
					}
				}

				if (_saving && FileManager::FileExists(path.Path)) {
					ComponentMessageBox* window = new ComponentMessageBox();
					window->SetBackgroundImage(_messageBoxImage);
					window->SetLine1("Image already exists");
					window->SetLine2("Overwrite image?");
					window->SetID("OverwriteMessage");
					OpenWindow(window);
				}
				else {
					_returnFile = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
					_app->ChangeFrame("Canvas");
					_canceled = false;
					Close();
				}
			}
		}
	}
	else {
		if (GetWindow()->GetID() == "OverwriteMessage") {
			if (((ComponentMessageBox*)GetWindow())->IsAccepted()) {
				_returnFile = File(_directory.DirectoryFile.Path + "\\" + _fileNameTextBox->GetText());
				_app->ChangeFrame("Canvas");
				_canceled = false;
				Close();
			}
		}
	}
	
	//UpdateResizing();
}
void PowerConsole::FrameFileDialog::UpdateDimensions() {

	if (_resized) {
		_fileListScrollBar->Resize(_app->GetConsoleSize() - Coord(26, 8), Coord(0, _directory.NumFiles()), true);
		_fileList->SetSize(Coord(_app->GetConsoleSize() - Coord(26 + (_fileListScrollBar->IsVerticalBarActive() ? 2 : 0), 8)));
		_resized = false;
	}

	_saveButton->SetPosition(_app->GetConsoleSize() - Coord(18, 2));
	_cancelButton->SetPosition(_app->GetConsoleSize() - Coord(9, 2));
	
	_fileNameTextBox->SetPosition(Coord(13, _app->GetConsoleSize().Y - 2));
	_fileNameTextBox->SetSize(Coord(_app->GetConsoleSize().X - 32, 1));
}
void PowerConsole::FrameFileDialog::UpdateResizing() {
	
	if (!_resizing) {
		_resizeHover = (_app->GetMouse()->GetPosition() == (_app->GetConsoleSize() - Coord::One));
		if (_app->GetMouse()->IsButtonPressed(MouseButtons::Left) && _resizeHover) {
			_resizing = true;
		}
	}
	else {
		Coord pos = _app->GetMouse()->GetPosition();
		pos = Coord(max(0, pos.X), max(0, pos.Y));
		if (_app->GetMouse()->IsButtonReleased(MouseButtons::Left)) {
			_resizing = false;
		}
		else if (pos != _app->GetWindowSize() - 1) {
			pos += 1;
			if (pos.X < 72)
				pos.X = 72;
			if (pos.Y < 18)
				pos.Y = 18;
			_app->SetConsoleSize(pos);
			_resized = true;
		}
	}
}
void PowerConsole::FrameFileDialog::LoadDirectory(File file) {
	_directory = Directory();
	if (!_directory.Load(file)) {
		File computerParent = File();
		computerParent.Load(FileManager::ComputerDirectory);
		computerParent.IsParentDirectory = true;
		_directory.FileList.push_back(computerParent);
	}
	if (_imageMode == 0)
		_directory.RemoveFileExtensions("asc", true);
	else if (_imageMode == 1)
		_directory.RemoveFileExtensions("bmp", true);
	_directory.RemoveFileAttributes(FileAttributes::FileAttributeSystem);
	
	_imageList.clear();
	int numImages = 0;

	for (int i = 0; i < _directory.NumFiles(); i++) {
		if (_directory[i].Type == FileTypes::File) {
			if (_imageMode == 0) {
				AsciiImageHeader header = AsciiImageHeader();
				FileInputStream in = FileInputStream();
				in.Open(_directory[i].Path);
				if (in.IsOpen()) {
					AsciiImageInfo::LoadHeader(&in, header, false);
					in.Close();
				}
				_imageList.push_back(header);
			}
			numImages++;
		}
	}

	_fileList->SetImageMode(_imageMode);
	_fileList->SetFileList(_directory.FileList.data(), _directory.NumFiles());
	_fileList->SetImageList(_imageList.data(), numImages);
	_fileListScrollBar->Resize(_app->GetConsoleSize() - Coord(26, 8), Coord(0, _directory.NumFiles()), true);
	_fileList->SetSize(Coord(_app->GetConsoleSize() - Coord(26 + (_fileListScrollBar->IsVerticalBarActive() ? 2 : 0), 8)));

	Directory computerDir = Directory();
	computerDir.Load(File(FileManager::ComputerDirectory));
	_computerList.clear();

	_computerList.push_back(computerDir.DirectoryFile);
	for (int i = 0; i < computerDir.NumFiles(); i++) {
		_computerList.push_back(computerDir[i]);
	}

	_computerLocationsList->SetFileList(_computerList.data(), _computerList.size());
}
void PowerConsole::FrameFileDialog::Close() {
	_directory = Directory();
	_imageList.clear();
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::FrameFileDialog::Draw(AsciiImageBase& g) {

	g.DrawImage(Coord::Zero, *_uiFileDialogImage);

	Frame::Draw(g);

	if (_directory.DirectoryFile.Type == FileTypes::Computer) {
		g.DrawString(Coord(13, 1), "Computer", Pixel(' ', _pathHover ? 0x0F : 0x07));
	}
	else {
		string path = _directory.DirectoryFile.Path;
		g.DrawString(Coord(13, 1), ((path.length() > 90) ? path.substr(0, 90) : path), Pixel(' ', _pathHover ? 0x0F : 0x07));
	}
}

#pragma endregion
//========== COMPONENTS ==========
#pragma region Components

#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|