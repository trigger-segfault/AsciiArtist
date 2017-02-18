/*=================================================================|
|* File:				FrameCanvas.cpp							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/10/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "FrameCanvas.h"
#include "ConsoleApp.h"
#include "Component.h"
#include "AsciiArtist.h"
#include "AsciiArtistManager.h"
#include "ComponentWindow.h"
#include "FrameFileDialog.h"

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

PowerConsole::FrameCanvas::FrameCanvas(AsciiArtistManager* aa) : Frame() {
	this->_id			= "Canvas";
	
	this->_aa			= aa;

	this->_resizing		= false;
	this->_resized		= false;
	this->_resizeHover	= false;

	this->_consoleSize	= Coord(102, 38);

	this->_importSize			= Coord(64, 32);
	this->_importStartPosition	= 0;
	this->_importCharSpacing	= 0;
	this->_importColorSpacing	= 0;
	this->_importChar			= true;
	this->_importColor			= true;
}
PowerConsole::FrameCanvas::~FrameCanvas() {
	
}
void PowerConsole::FrameCanvas::Initialize(ConsoleApp* app) {
	Frame::Initialize(app);

	this->_colorPaletteImage = new AsciiImage();
	//this->_colorPaletteImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\ColorPalette.asc");
	this->_colorPaletteImage->LoadResource(ASC_COLOR_PALETTE);
	this->_characterGridImage = new AsciiImage();
	//this->_characterGridImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\CharacterGrid.asc");
	this->_characterGridImage->LoadResource(ASC_CHAR_GRID);
	
	this->_uiMainImage = new AsciiImage();
	//this->_uiMainImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\UIMain.asc");
	this->_uiMainImage->LoadResource(ASC_UI_MAIN_1);
	this->_uiStatusBarImage = new AsciiImage();
	//this->_uiStatusBarImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\UIStatusBar.asc");
	this->_uiStatusBarImage->LoadResource(ASC_UI_STATUS_BAR);
	this->_uiFramesTopImage = new AsciiAnimation();
	//this->_uiFramesTopImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\UIFramesTop.asc");
	this->_uiFramesTopImage->LoadResource(ASC_UI_FRAMES_TOP);
	this->_uiFramesBottomImage = new AsciiAnimation();
	//this->_uiFramesBottomImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\UIFramesBottom.asc");
	this->_uiFramesBottomImage->LoadResource(ASC_UI_FRAMES_BOTTOM);

	this->_resizeDialogImage = new AsciiImage();
	//this->_resizeDialogImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\ResizeDialog.asc");
	this->_resizeDialogImage->LoadResource(ASC_RESIZE_DIALOG);
	this->_translateDialogImage = new AsciiImage();
	//this->_translateDialogImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\TranslateDialog.asc");
	this->_translateDialogImage->LoadResource(ASC_TRANSLATE_DIALOG);
	this->_animationSpeedDialogImage = new AsciiImage();
	//this->_animationSpeedDialogImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\AnimationSpeedDialog.asc");
	this->_animationSpeedDialogImage->LoadResource(ASC_ANIM_SPEED_DIALOG);
	this->_imageSettingsImage = new AsciiImage();
	//this->_imageSettingsImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\ImageSettingsDialog.asc");
	this->_imageSettingsImage->LoadResource(ASC_IMAGE_SETTINGS_DIALOG);
	this->_importFileDialogImage = new AsciiImage();
	//this->_importFileDialogImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\ImportFileDialog.asc");
	this->_importFileDialogImage->LoadResource(ASC_IMPORT_FILE_DIALOG);
	this->_messageBoxImage = new AsciiImage();
	//this->_messageBoxImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\MessageBox.asc");
	this->_messageBoxImage->LoadResource(ASC_MESSAGE_BOX);

	// New File
	this->_newFileImage = new AsciiAnimation();
	//this->_newFileImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\NewFile.asc");
	this->_newFileImage->LoadResource(ASC_BUTTON_NEW_FILE);
	this->_newFileButton = new ComponentButton();
	this->_newFileButton->SetPosition(Coord(1, 1));
	this->_newFileButton->SetSize(Coord(3, 1));
	this->_newFileButton->SetIcons(this->_newFileImage);
	AddComponent(this->_newFileButton);

	// Open File
	this->_openFileImage = new AsciiAnimation();
	//this->_openFileImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\OpenFile.asc");
	this->_openFileImage->LoadResource(ASC_BUTTON_OPEN_FILE);
	this->_openFileButton = new ComponentButton();
	this->_openFileButton->SetPosition(Coord(5, 1));
	this->_openFileButton->SetSize(Coord(3, 1));
	this->_openFileButton->SetIcons(this->_openFileImage);
	AddComponent(this->_openFileButton);
	
	// Save File
	this->_saveFileImage = new AsciiAnimation();
	//this->_saveFileImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\SaveFile.asc");
	this->_saveFileImage->LoadResource(ASC_BUTTON_SAVE_FILE);
	this->_saveFileButton = new ComponentButton();
	this->_saveFileButton->SetPosition(Coord(9, 1));
	this->_saveFileButton->SetSize(Coord(3, 1));
	this->_saveFileButton->SetIcons(this->_saveFileImage);
	AddComponent(this->_saveFileButton);

	// Save As File
	this->_saveAsFileImage = new AsciiAnimation();
	//this->_saveAsFileImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\SaveAsFile.asc");
	this->_saveAsFileImage->LoadResource(ASC_BUTTON_SAVE_AS_FILE);
	this->_saveAsFileButton = new ComponentButton();
	this->_saveAsFileButton->SetPosition(Coord(13, 1));
	this->_saveAsFileButton->SetSize(Coord(3, 1));
	this->_saveAsFileButton->SetIcons(this->_saveAsFileImage);
	AddComponent(this->_saveAsFileButton);
	
	// Copy
	this->_copyImage = new AsciiAnimation();
	//this->_copyImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Copy.asc");
	this->_copyImage->LoadResource(ASC_BUTTON_COPY);
	this->_copyButton = new ComponentButton();
	this->_copyButton->SetPosition(Coord(17, 1));
	this->_copyButton->SetSize(Coord(3, 1));
	this->_copyButton->SetIcons(this->_copyImage);
	AddComponent(this->_copyButton);
	
	// Cut
	this->_cutImage = new AsciiAnimation();
	//this->_cutImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Cut.asc");
	this->_cutImage->LoadResource(ASC_BUTTON_CUT);
	this->_cutButton = new ComponentButton();
	this->_cutButton->SetPosition(Coord(21, 1));
	this->_cutButton->SetSize(Coord(3, 1));
	this->_cutButton->SetIcons(this->_cutImage);
	AddComponent(this->_cutButton);
	
	// Delete
	this->_deleteImage = new AsciiAnimation();
	//this->_deleteImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Delete.asc");
	this->_deleteImage->LoadResource(ASC_BUTTON_DELETE);
	this->_deleteButton = new ComponentButton();
	this->_deleteButton->SetPosition(Coord(25, 1));
	this->_deleteButton->SetSize(Coord(3, 1));
	this->_deleteButton->SetIcons(this->_deleteImage);
	AddComponent(this->_deleteButton);
	
	// Paste
	this->_pasteImage = new AsciiAnimation();
	//this->_pasteImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Paste.asc");
	this->_pasteImage->LoadResource(ASC_BUTTON_PASTE);
	this->_pasteButton = new ComponentButton();
	this->_pasteButton->SetPosition(Coord(29, 1));
	this->_pasteButton->SetSize(Coord(3, 1));
	this->_pasteButton->SetIcons(this->_pasteImage);
	AddComponent(this->_pasteButton);
	
	// Select All
	this->_selectAllImage = new AsciiAnimation();
	//this->_selectAllImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\SelectAll.asc");
	this->_selectAllImage->LoadResource(ASC_BUTTON_SELECT_ALL);
	this->_selectAllButton = new ComponentButton();
	this->_selectAllButton->SetPosition(Coord(33, 1));
	this->_selectAllButton->SetSize(Coord(3, 1));
	this->_selectAllButton->SetIcons(this->_selectAllImage);
	AddComponent(this->_selectAllButton);
	
	// Undo
	this->_undoImage = new AsciiAnimation();
	//this->_undoImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Undo.asc");
	this->_undoImage->LoadResource(ASC_BUTTON_UNDO);
	this->_undoDisabledImage = new AsciiAnimation();
	//this->_undoDisabledImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\UndoDisabled.asc");
	this->_undoDisabledImage->LoadResource(ASC_BUTTON_UNDO_DISABLED);
	this->_undoButton = new ComponentButton();
	this->_undoButton->SetPosition(Coord(37, 1));
	this->_undoButton->SetSize(Coord(3, 1));
	this->_undoButton->SetIcons(this->_undoImage);
	AddComponent(this->_undoButton);
	
	// Redo
	this->_redoImage = new AsciiAnimation();
	//this->_redoImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Redo.asc");
	this->_redoImage->LoadResource(ASC_BUTTON_REDO);
	this->_redoDisabledImage = new AsciiAnimation();
	//this->_redoDisabledImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\RedoDisabled.asc");
	this->_redoDisabledImage->LoadResource(ASC_BUTTON_REDO_DISABLED);
	this->_redoButton = new ComponentButton();
	this->_redoButton->SetPosition(Coord(41, 1));
	this->_redoButton->SetSize(Coord(3, 1));
	this->_redoButton->SetIcons(this->_redoImage);
	AddComponent(this->_redoButton);
	
	// Resize
	this->_resizeImage = new AsciiAnimation();
	//this->_resizeImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Resize.asc");
	this->_resizeImage->LoadResource(ASC_BUTTON_RESIZE);
	this->_resizeButton = new ComponentButton();
	this->_resizeButton->SetPosition(Coord(45, 1));
	this->_resizeButton->SetSize(Coord(3, 1));
	this->_resizeButton->SetIcons(this->_resizeImage);
	AddComponent(this->_resizeButton);
	
	// Translate
	this->_translateImage = new AsciiAnimation();
	//this->_translateImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Translate.asc");
	this->_translateImage->LoadResource(ASC_BUTTON_TRANSLATE);
	this->_translateButton = new ComponentButton();
	this->_translateButton->SetPosition(Coord(49, 1));
	this->_translateButton->SetSize(Coord(3, 1));
	this->_translateButton->SetIcons(this->_translateImage);
	AddComponent(this->_translateButton);

	// Save Bitmap
	this->_saveBitmapImage = new AsciiAnimation();
	//this->_saveBitmapImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\SaveBitmap.asc");
	this->_saveBitmapImage->LoadResource(ASC_BUTTON_SAVE_BITMAP);
	this->_saveBitmapButton = new ComponentButton();
	this->_saveBitmapButton->SetPosition(Coord(53, 1));
	this->_saveBitmapButton->SetSize(Coord(3, 1));
	this->_saveBitmapButton->SetIcons(this->_saveBitmapImage);
	AddComponent(this->_saveBitmapButton);
	
	// Import File
	this->_importFileImage = new AsciiAnimation();
	//this->_importFileImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\ImportFile.asc");
	this->_importFileImage->LoadResource(ASC_BUTTON_IMPORT_FILE);
	this->_importFileButton = new ComponentButton();
	this->_importFileButton->SetPosition(Coord(57, 1));
	this->_importFileButton->SetSize(Coord(3, 1));
	this->_importFileButton->SetIcons(this->_importFileImage);
	AddComponent(this->_importFileButton);
	
	// Help
	this->_helpImage = new AsciiAnimation();
	//this->_helpImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\Help.asc");
	this->_helpImage->LoadResource(ASC_BUTTON_HELP);
	this->_helpButton = new ComponentButton();
	this->_helpButton->SetSize(Coord(3, 1));
	this->_helpButton->SetIcons(this->_helpImage);
	AddComponent(this->_helpButton);

	// Settings
	this->_settingsButton = new ComponentButton();
	this->_settingsButton->SetSize(Coord(8, 1));
	this->_settingsButton->SetText("Settings");
	AddComponent(this->_settingsButton);

	// New Frame
	this->_newFrameImage = new AsciiAnimation();
	//this->_newFrameImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\NewFrame.asc");
	this->_newFrameImage->LoadResource(ASC_BUTTON_NEW_FRAME);
	this->_newFrameButton = new ComponentButton();
	this->_newFrameButton->SetSize(Coord(2, 1));
	this->_newFrameButton->SetIcons(this->_newFrameImage);
	AddComponent(this->_newFrameButton);
	
	// Duplicate Frame
	this->_duplicateFrameImage = new AsciiAnimation();
	//this->_duplicateFrameImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\DuplicateFrame.asc");
	this->_duplicateFrameImage->LoadResource(ASC_BUTTON_DUPLICATE_FRAME);
	this->_duplicateFrameButton = new ComponentButton();
	this->_duplicateFrameButton->SetSize(Coord(2, 1));
	this->_duplicateFrameButton->SetIcons(this->_duplicateFrameImage);
	AddComponent(this->_duplicateFrameButton);
	
	// Delete Frame
	this->_deleteFrameImage = new AsciiAnimation();
	//this->_deleteFrameImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\DeleteFrame.asc");
	this->_deleteFrameImage->LoadResource(ASC_BUTTON_DELETE_FRAME);
	this->_deleteFrameButton = new ComponentButton();
	this->_deleteFrameButton->SetSize(Coord(2, 1));
	this->_deleteFrameButton->SetIcons(this->_deleteFrameImage);
	AddComponent(this->_deleteFrameButton);

	// Animate
	this->_animateButton = new ComponentButton();
	this->_animateButton->SetTogglable(true);
	this->_animateButton->SetSize(Coord(7, 1));
	this->_animateButton->SetText("Animate");
	AddComponent(this->_animateButton);
	
	// Move Frame Down
	this->_moveFrameDownImage = new AsciiAnimation();
	//this->_moveFrameDownImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\MoveFrameDown.asc");
	this->_moveFrameDownImage->LoadResource(ASC_BUTTON_MOVE_FRAME_DOWN);
	this->_moveFrameDownButton = new ComponentButton();
	this->_moveFrameDownButton->SetSize(Coord(2, 1));
	this->_moveFrameDownButton->SetIcons(this->_moveFrameDownImage);
	AddComponent(this->_moveFrameDownButton);
	
	// Move Frame Up
	this->_moveFrameUpImage = new AsciiAnimation();
	//this->_moveFrameUpImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\MoveFrameUp.asc");
	this->_moveFrameUpImage->LoadResource(ASC_BUTTON_MOVE_FRAME_UP);
	this->_moveFrameUpButton = new ComponentButton();
	this->_moveFrameUpButton->SetSize(Coord(2, 1));
	this->_moveFrameUpButton->SetIcons(this->_moveFrameUpImage);
	AddComponent(this->_moveFrameUpButton);
	
	// Animation Speed
	this->_animationSpeedImage = new AsciiAnimation();
	//this->_animationSpeedImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\Buttons\\AnimationSpeed.asc");
	this->_animationSpeedImage->LoadResource(ASC_BUTTON_ANIM_SPEED);
	this->_animationSpeedButton = new ComponentButton();
	this->_animationSpeedButton->SetSize(Coord(2, 1));
	this->_animationSpeedButton->SetIcons(this->_animationSpeedImage);
	AddComponent(this->_animationSpeedButton);

	// Character Filter
	this->_charFilterButton = new ComponentButton();
	this->_charFilterButton->SetTogglable(true);
	this->_charFilterButton->SetToggled(true);
	this->_charFilterButton->SetPosition(Coord(2, 8));
	this->_charFilterButton->SetSize(Coord(2, 1));
	this->_charFilterButton->SetText("Ch");
	AddComponent(this->_charFilterButton);
	
	// Foreground Color Filter
	this->_fColorFilterButton = new ComponentButton();
	this->_fColorFilterButton->SetTogglable(true);
	this->_fColorFilterButton->SetToggled(true);
	this->_fColorFilterButton->SetPosition(Coord(5, 8));
	this->_fColorFilterButton->SetSize(Coord(2, 1));
	this->_fColorFilterButton->SetText("Cf");
	AddComponent(this->_fColorFilterButton);
	
	// Background Color Filter
	this->_bColorFilterButton = new ComponentButton();
	this->_bColorFilterButton->SetTogglable(true);
	this->_bColorFilterButton->SetToggled(true);
	this->_bColorFilterButton->SetPosition(Coord(8, 8));
	this->_bColorFilterButton->SetSize(Coord(2, 1));
	this->_bColorFilterButton->SetText("Cb");
	AddComponent(this->_bColorFilterButton);

	// Cursor
	this->_cursorButton = new ComponentButton();
	this->_cursorButton->SetTogglable(true);
	this->_cursorButton->SetPosition(Coord(13, 12));
	this->_cursorButton->SetSize(Coord(6, 1));
	this->_cursorButton->SetText("Cursor");
	this->_cursorButton->SetToggled(true);
	AddComponent(this->_cursorButton);
	
	// Outline
	this->_outlineButton = new ComponentButton();
	this->_outlineButton->SetTogglable(true);
	this->_outlineButton->SetPosition(Coord(3, 12));
	this->_outlineButton->SetSize(Coord(7, 1));
	this->_outlineButton->SetText("Outline");
	this->_outlineButton->SetToggled(true);
	AddComponent(this->_outlineButton);

	// Tools
	this->_toolList = new ComponentList();
	this->_toolList->SetSelectionRequired(true);
	this->_toolList->SetPosition(Coord(13, 3));
	this->_toolList->SetSize(Coord(7, 8));
	this->_toolList->AddEntry("Pen");
	this->_toolList->AddEntry("Line");
	this->_toolList->AddEntry("Square");
	this->_toolList->AddEntry("Circle");
	this->_toolList->AddEntry("Flood");
	this->_toolList->AddEntry("Replace");
	this->_toolList->AddEntry("Text");
	this->_toolList->AddEntry("Select");
	AddComponent(this->_toolList);

	// Character Grid
	this->_characterGridImage = new AsciiImage();
	//this->_characterGridImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\CharacterGrid.asc");
	this->_characterGridImage->LoadResource(ASC_CHAR_GRID);
	this->_characterGrid = new ComponentCharacterGrid();
	this->_characterGrid->SetPosition(Coord(1, 14));
	this->_characterGrid->SetSize(Coord(20, 0));
	this->_characterGrid->SetCharacterGridImage(this->_characterGridImage);
	this->_characterGrid->SetAsciiArtistManager(this->_aa);
	AddComponent(this->_characterGrid);

	// Color Palette
	this->_colorPaletteImage = new AsciiImage();
	//this->_colorPaletteImage->LoadFile(FileManager::GetExecutableDirectory() + "\\Resources\\ColorPalette.asc");
	this->_colorPaletteImage->LoadResource(ASC_COLOR_PALETTE);
	this->_colorPalette = new ComponentColorPalette();
	this->_colorPalette->SetPosition(Coord(1, 3));
	this->_colorPalette->SetSize(Coord(10, 2));
	this->_colorPalette->SetColorPaletteImage(this->_colorPaletteImage);
	AddComponent(this->_colorPalette);

	// Character Display
	this->_characterDisplay = new ComponentCharacterDisplay();
	this->_characterDisplay->SetPosition(Coord(1, 6));
	this->_characterDisplay->SetSize(Coord(10, 1));
	AddComponent(this->_characterDisplay);

	// Canvas Scroll Bar
	this->_canvasScrollBar = new ComponentScrollBar();
	this->_canvasScrollBar->SetPosition(Coord(22, 3));
	this->_canvasScrollBar->SetVerticalBarEnabled(true);
	this->_canvasScrollBar->SetHorizontalBarEnabled(true);
	this->_canvasScrollBar->Resize(_app->GetConsoleSize() - Coord(38, 6), Coord(58, 34), false);
	AddComponent(this->_canvasScrollBar);

	// Canvas
	this->_canvas = new ComponentCanvas();
	this->_canvas->SetPosition(Coord(22, 3));
	this->_canvas->SetSize(_app->GetConsoleSize() - Coord(38, 6) -
		Coord(this->_canvasScrollBar->IsVerticalBarActive() ? 2 : 0, this->_canvasScrollBar->IsHorizontalBarActive() ? 2 : 0));
	this->_canvas->SetImage(_aa->GetImage());
	this->_canvas->SetScrollBar(this->_canvasScrollBar);
	this->_canvas->SetAsciiArtistManager(this->_aa);
	AddComponent(this->_canvas);

	// Frame List Scroll Bar
	this->_frameListScrollBar = new ComponentScrollBar();
	this->_frameListScrollBar->SetPosition(Coord(_app->GetConsoleSize().X - 15, 5));
	this->_frameListScrollBar->SetVerticalBarEnabled(true);
	this->_frameListScrollBar->SetMouseWheelEnabled(true);
	this->_frameListScrollBar->Resize(Coord(14, _app->GetConsoleSize().Y - 10), Coord(0, _aa->GetImage()->NumFrames() + 2), false);
	AddComponent(this->_frameListScrollBar);
	
	// Frame List
	this->_frameList = new ComponentFrameList();
	this->_frameList->SetPosition(Coord(_app->GetConsoleSize().X - 15, 5));
	this->_frameList->SetSize(Coord(11, _app->GetConsoleSize().Y - 10));
	this->_frameList->SetScrollBar(this->_frameListScrollBar);
	this->_frameList->SetNumFrames(_aa->GetImage()->NumFrames());
	AddComponent(this->_frameList);

	// Status Bar
	this->_statusBar = new ComponentStatusBar();
	this->_statusBar->SetPosition(Coord::Zero);
	this->_statusBar->SetSize(Coord::Zero);
	AddComponent(this->_statusBar);
	
	this->_aa->SetCanvas(this->_canvas);
	this->_aa->SetStatusBar(this->_statusBar);

	
	_app->SetConsoleSize(_consoleSize);
	this->_resized = true;
}
void PowerConsole::FrameCanvas::Uninitialize() {
	Frame::Uninitialize();
}
void PowerConsole::FrameCanvas::OnEnter() {
	Frame::OnEnter();

	_app->SetConsoleSize(_consoleSize);
	_resized = true;

	FrameFileDialog* fileDialog = ((FrameFileDialog*)_app->GetFrame("FileDialog"));
	if (fileDialog->IsSuccessful()) {
		if (fileDialog->GetImageMode() == 0) {
			if (fileDialog->IsSaving()) {
				_aa->Save(fileDialog->GetReturnFile());
			}
			else {
				_aa->Open(fileDialog->GetReturnFile());
			}
		}
		else if (fileDialog->GetImageMode() == 1) {

		}
		else if (fileDialog->GetImageMode() == 2) {
			_aa->ImportFile(fileDialog->GetReturnFile(), _importSize, _importStartPosition, _importCharSpacing, _importColorSpacing, _importChar, _importColor);
		}
	}
}
void PowerConsole::FrameCanvas::OnLeave() {
	Frame::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment


#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::FrameCanvas::Update() {

	UpdateDimensions();

	Frame::Update();

	if (!IsWindowOpen()) {
		UpdatePalette();
		UpdateTools();
		UpdateSpecial();
		UpdateSaving();
		UpdateFrames();
	}
	else {
		if (GetWindow()->GetID() == "ResizeDialog") {
			if (((ComponentResizeDialog*)GetWindow())->IsResized()) {
				_aa->SetAnimating(false);
				_aa->Resize(((ComponentResizeDialog*)GetWindow())->GetNewSize(), ((ComponentResizeDialog*)GetWindow())->IsResizeAllSelected());
			}
		}
		else if (GetWindow()->GetID() == "TranslateDialog") {
			if (((ComponentTranslateDialog*)GetWindow())->IsTranslated()) {
				_aa->SetAnimating(false);
				_aa->Translate(((ComponentTranslateDialog*)GetWindow())->GetDistance(), ((ComponentTranslateDialog*)GetWindow())->IsTranslateAllSelected());
			}
		}
		else if (GetWindow()->GetID() == "NewImageMessage") {
			if (((ComponentMessageBox*)GetWindow())->IsAccepted()) {
				_aa->SetAnimating(false);
				_aa->New();
			}
		}
		else if (GetWindow()->GetID() == "OpenImageMessage") {
			if (((ComponentMessageBox*)GetWindow())->IsAccepted()) {
				_aa->SetAnimating(false);
				((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetImageMode(0);
				((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetSaving(false);
				((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetFileName(_aa->GetImageFile().Name);
				((FrameFileDialog*)_app->GetFrame("FileDialog"))->LoadDirectory(File(_aa->GetImageFile().Directory));

				_app->ChangeFrame("FileDialog");
			}
		}
		else if (GetWindow()->GetID() == "AnimationSpeedDialog") {
			if (((ComponentAnimationSpeedDialog*)GetWindow())->IsSpeedSet()) {
				_aa->SetAnimating(false);
				_aa->SetAnimationSpeed(((ComponentAnimationSpeedDialog*)GetWindow())->GetAnimationSpeed());
				_aa->SetImageModified();
			}
		}
		else if (GetWindow()->GetID() == "ImageSettingsDialog") {
			if (((ComponentImageSettingsDialog*)GetWindow())->IsSaved()) {
				_aa->SetAnimating(false);
				_aa->SetVersion(((ComponentImageSettingsDialog*)GetWindow())->GetVersion());
				_aa->SetImageFormat(((ComponentImageSettingsDialog*)GetWindow())->GetFormat(), ((ComponentImageSettingsDialog*)GetWindow())->GetAnimationSpeed(), ((ComponentImageSettingsDialog*)GetWindow())->GetBackground());
				_aa->SetImageModified();
				_canvas->SetFrame(_aa->GetCurrentFrame());
			}
		}
		else if (GetWindow()->GetID() == "ImportFileDialog") {
			if (((ComponentImportFileDialog*)GetWindow())->IsImported()) {
				_importSize = ((ComponentImportFileDialog*)GetWindow())->GetImageSize();
				_importStartPosition = ((ComponentImportFileDialog*)GetWindow())->GetStartPosition();
				_importCharSpacing = ((ComponentImportFileDialog*)GetWindow())->GetCharSpacing();
				_importColorSpacing = ((ComponentImportFileDialog*)GetWindow())->GetColorSpacing();
				_importChar = ((ComponentImportFileDialog*)GetWindow())->IsCharImported();
				_importColor = ((ComponentImportFileDialog*)GetWindow())->IsColorImported();

				
				((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetImageMode(2);
				((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetSaving(false);
				((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetFileName("");
				((FrameFileDialog*)_app->GetFrame("FileDialog"))->LoadDirectory(File(_aa->GetImageFile().Directory));
				_app->ChangeFrame("FileDialog");
				_canvas->SetImage(_aa->GetImage());
				_canvas->SetFrame(_aa->GetCurrentFrame());
				_frameList->SetCurrentFrame(_aa->GetCurrentFrame());
				_canvasScrollBar->Resize(_app->GetConsoleSize() - Coord(38, 6), _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).Size(), true);
				_canvas->SetSize(_app->GetConsoleSize() - Coord(38, 6) -
					Coord(_canvasScrollBar->IsVerticalBarActive() ? 2 : 0, _canvasScrollBar->IsHorizontalBarActive() ? 2 : 0));
		
				_frameListScrollBar->Resize(Coord(14, _app->GetConsoleSize().Y - 10), Coord(0, _aa->GetImage()->NumFrames() + 2), false);
			}
		}
	}
	
	UpdateResizing();

	if (_aa->IsDebugMode()) {
		ostringstream oss;
		oss << "Update Rate: " << _app->GetUpdateRate();
		_statusBar->SetMessage(oss.str());

	}
}
void PowerConsole::FrameCanvas::UpdateDimensions() {
	if (_resized) {
		_canvasScrollBar->Resize(_app->GetConsoleSize() - Coord(38, 6), _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).Size(), true);
		_canvas->SetSize(_app->GetConsoleSize() - Coord(38, 6) -
			Coord(_canvasScrollBar->IsVerticalBarActive() ? 2 : 0, _canvasScrollBar->IsHorizontalBarActive() ? 2 : 0));
		
		_frameListScrollBar->Resize(Coord(14, _app->GetConsoleSize().Y - 10), Coord(0, _aa->GetImage()->NumFrames() + 2), false);

		_resized = false;
	}
	else if (_canvasScrollBar->GetListSize() != _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).Size()) {
		_canvasScrollBar->Resize(_app->GetConsoleSize() - Coord(38, 6), _aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).Size(), false);
		_canvas->SetSize(_app->GetConsoleSize() - Coord(38, 6) -
			Coord(_canvasScrollBar->IsVerticalBarActive() ? 2 : 0, _canvasScrollBar->IsHorizontalBarActive() ? 2 : 0));
		
		_frameListScrollBar->Resize(Coord(14, _app->GetConsoleSize().Y - 10), Coord(0, _aa->GetImage()->NumFrames() + 2), false);

		_resized = false;
	}

	_frameListScrollBar->SetPosition(Coord(_app->GetConsoleSize().X - 15, 5));
	_frameList->SetPosition(Coord(_app->GetConsoleSize().X - 15, 5));
	_frameList->SetSize(Coord(11, _app->GetConsoleSize().Y - 10));

	_helpButton->SetPosition(Coord(_app->GetConsoleSize().X - 15, 1));
	_settingsButton->SetPosition(Coord(_app->GetConsoleSize().X - 10, 1));
	_animationSpeedButton->SetPosition(_app->GetConsoleSize() - Coord(3, 4));
	_animateButton->SetPosition(_app->GetConsoleSize() - Coord(13, 4));

	_newFrameButton->SetPosition(Coord(_app->GetConsoleSize().X - 15, 3));
	_duplicateFrameButton->SetPosition(Coord(_app->GetConsoleSize().X - 12, 3));
	_deleteFrameButton->SetPosition(Coord(_app->GetConsoleSize().X - 9, 3));
	_moveFrameDownButton->SetPosition(Coord(_app->GetConsoleSize().X - 6, 3));
	_moveFrameUpButton->SetPosition(Coord(_app->GetConsoleSize().X - 3, 3));

	_characterGrid->SetSize(Coord(_characterGrid->GetSize().X, _app->GetConsoleSize().Y - 17));
}
void PowerConsole::FrameCanvas::UpdatePalette() {
	if (_characterGrid->IsPrimaryCharacterChanged()) {
		Pixel primaryPixel = _aa->GetPrimaryPixel();
		primaryPixel.Char = _characterGrid->GetPrimaryCharacter().Char;
		primaryPixel.Attributes &= ~PixelAttributes::AttributeChar;
		primaryPixel.Attributes |= _characterGrid->GetPrimaryCharacter().Attributes;
		_characterDisplay->SetPrimaryPixel(primaryPixel);
		_aa->SetPrimaryPixel(primaryPixel);
	}
	else if (_characterGrid->IsSecondaryCharacterChanged()) {
		Pixel secondaryPixel = _aa->GetSecondaryPixel();
		secondaryPixel.Char = _characterGrid->GetSecondaryCharacter().Char;
		secondaryPixel.Attributes &= ~PixelAttributes::AttributeChar;
		secondaryPixel.Attributes |= _characterGrid->GetSecondaryCharacter().Attributes;
		_characterDisplay->SetSecondayPixel(secondaryPixel);
		_aa->SetSecondaryPixel(secondaryPixel);
	}
	
	if (_colorPalette->IsPrimaryForegroundColorChanged()) {
		Pixel primaryPixel = _aa->GetPrimaryPixel();
		primaryPixel.Color			&= 0xF0;
		primaryPixel.Attributes	&= ~PixelAttributes::AttributeFAll;
		primaryPixel.Color			|= _colorPalette->GetNewColor().Color;
		primaryPixel.Attributes	|= _colorPalette->GetNewColor().Attributes;
		/*if ((primaryPixel.Attributes & PixelAttributes::AttributeColor) != 0x0)
			primaryPixel.Attributes |= PixelAttributes::AttributeChar;
		else
			primaryPixel.Attributes &= ~PixelAttributes::AttributeChar;*/
		_characterDisplay->SetPrimaryPixel(primaryPixel);
		_aa->SetPrimaryPixel(primaryPixel);
	}
	else if (_colorPalette->IsPrimaryBackgroundColorChanged()) {
		Pixel primaryPixel = _aa->GetPrimaryPixel();
		primaryPixel.Color			&= 0x0F;
		primaryPixel.Attributes	&= ~PixelAttributes::AttributeBAll;
		primaryPixel.Color			|= _colorPalette->GetNewColor().Color;
		primaryPixel.Attributes	|= _colorPalette->GetNewColor().Attributes;
		/*if ((primaryPixel.Attributes & PixelAttributes::AttributeColor) != 0x0)
			primaryPixel.Attributes |= PixelAttributes::AttributeChar;
		else
			primaryPixel.Attributes &= ~PixelAttributes::AttributeChar;*/
		_characterDisplay->SetPrimaryPixel(primaryPixel);
		_aa->SetPrimaryPixel(primaryPixel);
	}
	else if (_colorPalette->IsSecondaryForegroundColorChanged()) {
		Pixel secondaryPixel = _aa->GetSecondaryPixel();
		secondaryPixel.Color		&= 0xF0;
		secondaryPixel.Attributes	&= ~PixelAttributes::AttributeFAll;
		secondaryPixel.Color		|= _colorPalette->GetNewColor().Color;
		secondaryPixel.Attributes	|= _colorPalette->GetNewColor().Attributes;
		/*if ((secondaryPixel.Attributes & PixelAttributes::AttributeColor) != 0x0)
			secondaryPixel.Attributes |= PixelAttributes::AttributeChar;
		else
			secondaryPixel.Attributes &= ~PixelAttributes::AttributeChar;*/
		_characterDisplay->SetSecondayPixel(secondaryPixel);
		_aa->SetSecondaryPixel(secondaryPixel);
	}
	else if (_colorPalette->IsSecondaryBackgroundColorChanged()) {
		Pixel secondaryPixel = _aa->GetSecondaryPixel();
		secondaryPixel.Color		&= 0x0F;
		secondaryPixel.Attributes	&= ~PixelAttributes::AttributeBAll;
		secondaryPixel.Color		|= _colorPalette->GetNewColor().Color;
		secondaryPixel.Attributes	|= _colorPalette->GetNewColor().Attributes;
		/*if ((secondaryPixel.Attributes & PixelAttributes::AttributeColor) != 0x0)
			secondaryPixel.Attributes |= PixelAttributes::AttributeChar;
		else
			secondaryPixel.Attributes &= ~PixelAttributes::AttributeChar;*/
		_characterDisplay->SetSecondayPixel(secondaryPixel);
		_aa->SetSecondaryPixel(secondaryPixel);
	}
	else if (_charFilterButton->IsButtonPressed()) {
		_aa->SetCharacterDrawn(_charFilterButton->IsToggled());
	}
	else if (_fColorFilterButton->IsButtonPressed()) {
		_aa->SetFColorDrawn(_fColorFilterButton->IsToggled());
	}
	else if (_bColorFilterButton->IsButtonPressed()) {
		_aa->SetBColorDrawn(_bColorFilterButton->IsToggled());
	}
}
void PowerConsole::FrameCanvas::UpdateTools() {
	
	// Tool Selection
	if (_toolList->IsSelectionChanged()) {
		_aa->SetCurrentTool((ToolTypes)_toolList->GetSelection());
	}
	if (_outlineButton->IsButtonPressed()) {
		ToolTypes tool = _aa->GetCurrentToolType();
		if (tool == ToolTypes::Square || tool == ToolTypes::Circle) {
			_aa->SetShapeOutline(_outlineButton->IsToggled());
		}
		else if (tool == ToolTypes::Flood || tool == ToolTypes::Replace) {
			_aa->SetPixelSpecific(_outlineButton->IsToggled());
		}
	}

	// Tool Settings
	ToolTypes tool = _aa->GetCurrentToolType();
	if (tool == ToolTypes::Square || tool == ToolTypes::Circle) {
		_outlineButton->SetText("Outline");
		_outlineButton->SetSize(Coord(7, 1));
		_outlineButton->SetToggled(_aa->IsShapeOutline());
	}
	else if (tool == ToolTypes::Flood || tool == ToolTypes::Replace) {
		_outlineButton->SetText("Specific");
		_outlineButton->SetSize(Coord(8, 1));
		_outlineButton->SetToggled(_aa->IsPixelSpecific());
	}
	else {
		_outlineButton->SetText("");
		_outlineButton->SetSize(Coord(0, 0));
	}

	// Selection Tools
	if (_copyButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::C))) {
		_aa->Copy();
	}
	else if (_cutButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::X))) {
		_aa->Cut();
	}
	else if (_deleteButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyPressed(Keys::Delete))) {
		_aa->Delete();
	}
	else if (_pasteButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::V))) {
		_aa->Paste();
	}
	else if (_selectAllButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::A))) {
		_aa->SelectAll();
	}

	if ((int)_aa->GetCurrentToolType() != _toolList->GetSelection()) {
		_toolList->SetSelection((int)_aa->GetCurrentToolType());
	}
}
void PowerConsole::FrameCanvas::UpdateSpecial() {
	
	// Show Cursor
	if (_cursorButton->IsButtonPressed()) {
		_aa->SetCursorVisible(_cursorButton->IsToggled());
	}

	// Undo/Redo
	if (_undoButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::Z))) {
		_aa->Undo();
	}
	else if (_redoButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::Y))) {
		_aa->Redo();
	}

	if (_aa->IsUndoAvailable()) {
		_undoButton->SetIcons(_undoImage);
	}
	else {
		_undoButton->SetIcons(_undoDisabledImage);
	}
	if (_aa->IsRedoAvailable()) {
		_redoButton->SetIcons(_redoImage);
	}
	else {
		_redoButton->SetIcons(_redoDisabledImage);
	}
	
	// Resizing
	if (_resizeButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::R))) {
		ComponentResizeDialog* window = new ComponentResizeDialog();
		window->SetOldSize(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).Size());
		window->SetBackgroundImage(_resizeDialogImage);
		window->SetResizeAllEnabled((_aa->GetImage()->GetFormat() & ImageFormats::FormatMultiSize) != 0x0);
		OpenWindow(window);
	}

	// Translating
	if (_translateButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) &&
		_app->GetKeyboard()->IsKeyDown(Keys::LShiftKey) && _app->GetKeyboard()->IsKeyPressed(Keys::R))) {
		ComponentTranslateDialog* window = new ComponentTranslateDialog();
		window->SetBackgroundImage(_translateDialogImage);
		OpenWindow(window);
	}

	// Animation
	if (_animateButton->IsButtonPressed()) {
		_aa->SetAnimating(_animateButton->IsToggled());
	}
	_animateButton->SetToggled(_aa->IsAnimating());
	_aa->UpdateAnimation();
	
	// Save Bitmap
	if (_saveBitmapButton->IsButtonPressed()) {
		_aa->SetMessage("Not Implemented");
	}

	// Import File
	if (_importFileButton->IsButtonPressed()) {
		ComponentImportFileDialog* window = new ComponentImportFileDialog();
		window->SetBackgroundImage(_importFileDialogImage);
		window->SetImageSize(_importSize);
		window->SetStartPosition(_importStartPosition);
		window->SetCharSpacing(_importCharSpacing);
		window->SetColorSpacing(_importColorSpacing);
		window->SetCharImported(_importChar);
		window->SetColorImported(_importColor);
		OpenWindow(window);
	}

	// Help
	if (_helpButton->IsButtonPressed()) {
		ComponentMessageBox* window = new ComponentMessageBox();
		window->SetID("HelpMessage");
		window->SetBackgroundImage(_messageBoxImage);
		window->SetLine1("Read the ReadMe.txt");
		OpenWindow(window);
	}

	
	// Settings
	if (_settingsButton->IsButtonPressed()) {
		ComponentImageSettingsDialog* window = new ComponentImageSettingsDialog();
		
		window->SetBackgroundImage(_imageSettingsImage);
		window->SetFormat(_aa->GetImage()->GetFormat());
		window->SetVersion(_aa->GetVersion());
		window->SetBackground(_aa->GetImage()->GetBackground());
		window->SetAnimationSpeed(_aa->GetImage()->GetAnimationSpeed());
		window->SetPrimaryPixel(_aa->GetPrimaryPixel());
		window->SetSecondaryPixel(_aa->GetSecondaryPixel());
		OpenWindow(window);
	}

	// Debug Mode
	if (_app->GetKeyboard()->IsKeyPressed(Keys::F3)) {
		_aa->SetDebugMode(!_aa->IsDebugMode());
		if (!_aa->IsDebugMode()) {
			_aa->SetMessage("");
		}
	}

	// Eyedropper
	if (_canvas->IsMouseInsideCanvas() && _app->GetMouse()->IsButtonPressed(MouseButtons::Middle)) {
		_aa->SetPrimaryPixel(_aa->GetImage()->GetFrame(_aa->GetCurrentFrame()).GetPixel(_canvas->GetImageMousePosition()));
		_characterDisplay->SetPrimaryPixel(_aa->GetPrimaryPixel());
		Pixel pixel = _aa->GetPrimaryPixel();
		pixel.Color = 0x00;
		pixel.Attributes &= PixelAttributes::AttributeChar;
		if (pixel.Attributes == 0x0)
			pixel.Char = ' ';
		_characterGrid->SetPrimaryCharacter(pixel);
	}
}
void PowerConsole::FrameCanvas::UpdateSaving() {
	
	if (_newFileButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::N))) {
		if (_aa->IsSaved()) {
			_aa->SetAnimating(false);
			_aa->New();
		}
		else {
			ComponentMessageBox* window = new ComponentMessageBox();
			window->SetBackgroundImage(_messageBoxImage);
			window->SetLine1("Image hasn't been saved");
			window->SetLine2("Create new image anyway?");
			window->SetID("NewImageMessage");
			OpenWindow(window);
		}
	}
	else if (_openFileButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::O))) {
		if (_aa->IsSaved()) {
			_aa->SetAnimating(false);
			((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetImageMode(0);
			((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetSaving(false);
			((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetFileName(_aa->GetImageFile().Name);
			((FrameFileDialog*)_app->GetFrame("FileDialog"))->LoadDirectory(File(_aa->GetImageFile().Directory));

			_app->ChangeFrame("FileDialog");
		}
		else {
			ComponentMessageBox* window = new ComponentMessageBox();
			window->SetBackgroundImage(_messageBoxImage);
			window->SetLine1("Image hasn't been saved");
			window->SetLine2("Open an image anyway?");
			window->SetID("OpenImageMessage");
			OpenWindow(window);
		}
	}
	else if (_saveFileButton->IsButtonPressed() || (_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyPressed(Keys::S))) {
		if (_aa->IsUntitled()) {
			_aa->SetAnimating(false);
			((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetImageMode(0);
			((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetSaving(true);
			((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetFileName(_aa->GetImageFile().Name);
			((FrameFileDialog*)_app->GetFrame("FileDialog"))->LoadDirectory(File(_aa->GetImageFile().Directory));

			_app->ChangeFrame("FileDialog");
		}
		else {
			_aa->Save();
		}
	}
	else if (_saveAsFileButton->IsButtonPressed() ||
		(_app->GetKeyboard()->IsKeyDown(Keys::LControlKey) && _app->GetKeyboard()->IsKeyDown(Keys::LShiftKey) && _app->GetKeyboard()->IsKeyPressed(Keys::S))) {
		((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetImageMode(0);
		((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetSaving(true);
		((FrameFileDialog*)_app->GetFrame("FileDialog"))->SetFileName(_aa->GetImageFile().Name);
		((FrameFileDialog*)_app->GetFrame("FileDialog"))->LoadDirectory(File(_aa->GetImageFile().Directory));

		_app->ChangeFrame("FileDialog");
	}
}
void PowerConsole::FrameCanvas::UpdateFrames() {
	if (_frameList->IsFrameChanged()) {
		_aa->SetCurrentFrame(_frameList->GetCurrentFrame());
	}
	else if (_aa->GetCurrentFrame() != _frameList->GetCurrentFrame()) {
		_frameList->SetCurrentFrame(_aa->GetCurrentFrame());
		_canvas->SetFrame(_aa->GetCurrentFrame());
	}
	if (_frameList->GetNumFrames() != _aa->GetImage()->NumFrames()) {
		_frameList->SetNumFrames(_aa->GetImage()->NumFrames());
		
		_frameListScrollBar->Resize(Coord(14, _app->GetConsoleSize().Y - 10), Coord(0, _aa->GetImage()->NumFrames() + 2), true);
	}

	if (_newFrameButton->IsButtonPressed())
		AddFrame();
	if (_duplicateFrameButton->IsButtonPressed())
		DuplicateFrame();
	if (_deleteFrameButton->IsButtonPressed())
		DeleteFrame();
	if (_moveFrameDownButton->IsButtonPressed())
		MoveFrameDown();
	if (_moveFrameUpButton->IsButtonPressed())
		MoveFrameUp();

	if (_animationSpeedButton->IsButtonPressed()) {
		_aa->SetAnimating(false);

		ComponentAnimationSpeedDialog* window = new ComponentAnimationSpeedDialog();
		window->SetBackgroundImage(_animationSpeedDialogImage);
		window->SetAnimationSpeed(_aa->GetImage()->GetAnimationSpeed());
		OpenWindow(window);
	}

	if (_frameList->IsShiftHovered()) {
		_canvas->SetHoverFrame(_frameList->GetHoverFrame());
	}

	_frameList->SetCurrentFrame(_aa->GetCurrentFrame());
	_canvas->SetFrame(_aa->GetCurrentFrame());
}
void PowerConsole::FrameCanvas::UpdateResizing() {
	
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
			if (pos.X < 76)
				pos.X = 76;
			if (pos.Y < 33)
				pos.Y = 33;
			Coord maxSize = _app->GetMaxConsoleSize();
			if (pos.X > maxSize.X)
				pos.X = maxSize.X;
			if (pos.Y > maxSize.Y)
				pos.Y = maxSize.Y;
			_app->SetConsoleSize(pos);
			_resized = true;
			_consoleSize = pos;
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::FrameCanvas::Draw(AsciiImageBase& g) {

	g.DrawImage(Coord::Zero, *_uiMainImage);
	
	for (int i = _uiMainImage->Size().Y; i < _app->GetConsoleSize().Y - _uiStatusBarImage->Size().Y; i++) {
		g.DrawImage(Coord(0, i), Coord(0, 14), Coord(22, 1), *_uiMainImage);
	}

	g.DrawImage(Coord(0, _app->GetConsoleSize().Y - _uiStatusBarImage->Size().Y), *_uiStatusBarImage);

	g.DrawImage(Coord(_app->GetConsoleSize().X - _uiFramesTopImage->Size().X, 0), _uiFramesTopImage->GetFrame(0));
	g.DrawImage(_app->GetConsoleSize() - _uiFramesBottomImage->Size(), _uiFramesBottomImage->GetFrame(0));

	for (int i = _uiMainImage->Size().X; i < _app->GetConsoleSize().X - _uiFramesTopImage->Size().X; i++) {
		g.DrawImage(Coord(i, 0), Coord(61, 0), Coord(1, 3), *_uiMainImage);
	}

	for (int i = _uiStatusBarImage->Size().X; i < _app->GetConsoleSize().X - _uiFramesBottomImage->Size().X; i++) {
		g.DrawImage(Coord(i, _app->GetConsoleSize().Y - 3), Coord(44, 0), Coord(1, 3), *_uiStatusBarImage);
	}
	
	for (int i = _uiFramesTopImage->Size().Y; i < _app->GetConsoleSize().Y - _uiFramesBottomImage->Size().Y; i++) {
		g.DrawImage(Coord(_app->GetConsoleSize().X - _uiFramesTopImage->Size().X, i), Coord(0, 7), Coord(16, 1), _uiFramesTopImage->GetFrame(0));
	}

	if (_resizeHover || _resizing) {
		g.SetColor(_app->GetConsoleSize() - Coord::One, 0x07);
	}

	g.DrawRect(Coord(22, 3), _app->GetConsoleSize() - Coord(17, 4), Pixel(' ', 0x07), true, true);

	Frame::Draw(g);
}

#pragma endregion
//========== COMPONENTS ==========
#pragma region Components

void PowerConsole::FrameCanvas::AddFrame() {
	if ((_aa->GetImage()->GetFormat() & ImageFormats::FormatAnimation) != 0x0) {
		_aa->AddFrame();

		_frameList->SetCurrentFrame(_aa->GetCurrentFrame());
		_frameList->SetNumFrames(_aa->GetImage()->NumFrames());
	
		_frameListScrollBar->Resize(Coord(14, _app->GetConsoleSize().Y - 10), Coord(0, _aa->GetImage()->NumFrames() + 2), true);
		_frameListScrollBar->SetListPosition(_frameListScrollBar->GetListPosition() + 1);
	}
	else {
		_aa->SetMessage("Can't Add Frame");
	}
}
void PowerConsole::FrameCanvas::DuplicateFrame() {
	if ((_aa->GetImage()->GetFormat() & ImageFormats::FormatAnimation) != 0x0) {
		_aa->DuplicateFrame();

		_frameList->SetCurrentFrame(_aa->GetCurrentFrame());
		_frameList->SetNumFrames(_aa->GetImage()->NumFrames());

		_frameListScrollBar->Resize(Coord(14, _app->GetConsoleSize().Y - 10), Coord(0, _aa->GetImage()->NumFrames() + 2), true);
		_frameListScrollBar->SetListPosition(_frameListScrollBar->GetListPosition() + 1);
	}
	else {
		_aa->SetMessage("Can't Duplicate Frame");
	}
}
void PowerConsole::FrameCanvas::DeleteFrame() {
	if (_aa->GetImage()->NumFrames() > 1 && (_aa->GetImage()->GetFormat() & ImageFormats::FormatAnimation) != 0x0) {
		_aa->DeleteFrame();

		_frameList->SetCurrentFrame(_aa->GetCurrentFrame());
		_frameList->SetNumFrames(_aa->GetImage()->NumFrames());

		_frameListScrollBar->Resize(Coord(14, _app->GetConsoleSize().Y - 10), Coord(0, _aa->GetImage()->NumFrames() + 2), true);
		_frameListScrollBar->SetListPosition(_frameListScrollBar->GetListPosition() - 1);
	}
	else {
		_aa->SetMessage("Can't Delete Only Frame");
	}
}
void PowerConsole::FrameCanvas::MoveFrameDown() {
	if (_aa->GetCurrentFrame() < _aa->GetImage()->NumFrames() - 1 && (_aa->GetImage()->GetFormat() & ImageFormats::FormatAnimation) != 0x0) {
		_aa->MoveFrameDown();

		_frameList->SetCurrentFrame(_aa->GetCurrentFrame());
		_frameListScrollBar->SetListPosition(_frameListScrollBar->GetListPosition() + 1);
	}
	else {
		_aa->SetMessage("Can't Move Frame Down");
	}
}
void PowerConsole::FrameCanvas::MoveFrameUp() {
	if (_aa->GetCurrentFrame() > 0 && (_aa->GetImage()->GetFormat() & ImageFormats::FormatAnimation) != 0x0) {
		_aa->MoveFrameUp();

		_frameList->SetCurrentFrame(_aa->GetCurrentFrame());
		_frameListScrollBar->SetListPosition(_frameListScrollBar->GetListPosition() - 1);
	}
	else {
		_aa->SetMessage("Can't Move Frame Up");
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|