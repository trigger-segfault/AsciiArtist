/*=================================================================|
|* File:				AsciiArtistManager.cpp					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/14/2014								  *|
|*																  *|
|* Description: The container for all components.				  *|
|=================================================================*/

#include "Stdafx.h"
#include "AsciiArtistManager.h"
#include "AsciiArtist.h"
#include "Frame.h"
#include "FrameCanvas.h"
#include "ConsoleApp.h"
#include "Component.h"
#include "HiddenWinAPI.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::AsciiArtistApp;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Windows;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region AsciiArtistManager
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::AsciiArtistApp::AsciiArtistManager::AsciiArtistManager() {
	// Containment
	this->_aaApp			= nullptr;
	this->_canvas			= nullptr;
	this->_statusBar		= nullptr;

	// Image
	this->_imageFile		= File();
	this->_untitled			= true;
	this->_version			= 3;
	this->_image			= nullptr;
	this->_drawingBuffer	= nullptr;
	this->_currentFrame		= 0;
	this->_saved			= true;
	this->_animating		= false;
	this->_loadError		= false;

	// Drawing
	this->_primaryPixel		= Pixel('A', 0x07);
	this->_secondaryPixel	= Pixel(' ', 0x00);

	// Undos
	this->_undoEnabled		= true;
	this->_maxUndos			= 40;
	this->_currentUndo		= 0;
	
	// Tools
	this->_toolPen			= new ToolPen();
	this->_toolLine			= new ToolLine();
	this->_toolSquare		= new ToolSquare();
	this->_toolCircle		= new ToolCircle();
	this->_toolFlood		= new ToolFlood();
	this->_toolReplace		= new ToolReplace();
	this->_toolText			= new ToolText();
	this->_toolSelect		= new ToolSelect();
	this->_currentTool		= this->_toolPen;

	// Settings
	this->_outline			= true;
	this->_specific			= true;
	this->_showCursor		= true;
	this->_charFilter		= true;
	this->_fColorFilter		= true;
	this->_bColorFilter		= true;

	this->_debugMode		= false;
}
PowerConsole::AsciiArtistApp::AsciiArtistManager::~AsciiArtistManager() {
	
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Initialize(AsciiArtist* aaApp) {
	this->_aaApp			= aaApp;

	bool failedLoad = true;
	if (aaApp->GetNumArguments() > 1) {
		this->_image			= new AsciiAnimation();
		if (this->_image->LoadFile(aaApp->GetArgument(1))) {
			this->_imageFile.Load(aaApp->GetArgument(1));
			AsciiImageHeader header = AsciiImageHeader();
			FileInputStream in = FileInputStream();
			in.Open(aaApp->GetArgument(1));
			if (in.IsOpen()) {
				if (AsciiImageInfo::LoadHeader(&in, header, false)) {
					this->_version = (int)header.Version;
				}
				in.Close();
			}
			this->_untitled = false;
			failedLoad = false;
		}
		else {
			delete this->_image;
			this->_image = nullptr;

			this->_loadError = true;
		}

		if (FileManager::GetExecutableDirectory() != FileManager::GetWorkingDirectory()) {
			HICON icon = ExtractIconA(GetModuleHandleA(nullptr), FileManager::GetExecutablePath().c_str(), 0);

			SetConsoleIcon(icon);
		}
	}
	if (failedLoad) {
		this->_image = new AsciiAnimation(Coord(64, 32), ImageFormats::FormatAnimation | ImageFormats::FormatAttributes, Pixel(' ', 0x00, PixelAttributes::AttributeFill));
		this->_imageFile.Load(FileManager::GetWorkingDirectory() + "\\" + "Untitled.asc");
	}
	this->_drawingBuffer	= new AsciiAnimation(this->_image->Size(), ImageFormats::FormatAnimation | ImageFormats::FormatAttributes, this->_image->GetBackground(), 5);
	
	_aaApp->SetTitle(_imageFile.Name + " - Ascii Artist");


	this->_toolPen->Initialize(this, _aaApp, _drawingBuffer);
	this->_toolLine->Initialize(this, _aaApp, _drawingBuffer);
	this->_toolSquare->Initialize(this, _aaApp, _drawingBuffer);
	this->_toolCircle->Initialize(this, _aaApp, _drawingBuffer);
	this->_toolFlood->Initialize(this, _aaApp, _drawingBuffer);
	this->_toolReplace->Initialize(this, _aaApp, _drawingBuffer);
	this->_toolText->Initialize(this, _aaApp, _drawingBuffer);
	this->_toolSelect->Initialize(this, _aaApp, _drawingBuffer);

	this->_undoList.push_back(new UndoAction("N/A", Actions::None, -1));
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Uninitialize() {
	delete this->_drawingBuffer;
	this->_drawingBuffer	= nullptr;
	delete this->_image;
	this->_image			= nullptr;

	this->_aaApp			= nullptr;
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Containment
	
ComponentCanvas* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetCanvas() const {
	return _canvas;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetCanvas(ComponentCanvas* canvas) {
	this->_canvas = canvas;
}
ComponentStatusBar* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetStatusBar() const {
	return _statusBar;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetStatusBar(ComponentStatusBar* statusBar) {
	this->_statusBar = statusBar;

	if (_loadError) {
		statusBar->SetUrgentMessage("Failed to Open Image");
	}
}

#pragma endregion
//--------------------------------
#pragma region Image

File PowerConsole::AsciiArtistApp::AsciiArtistManager::GetImageFile() const {
	return _imageFile;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsUntitled() const {
	return _untitled;
}
int PowerConsole::AsciiArtistApp::AsciiArtistManager::GetVersion() const {
	return _version;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetVersion(int version) {
	this->_version = max(1, min(3, version));
}
AsciiAnimation* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetImage() const {
	return _image;
}
AsciiAnimation* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetDrawingBuffer() const {
	return _drawingBuffer;
}
int PowerConsole::AsciiArtistApp::AsciiArtistManager::GetCurrentFrame() const {
	return _currentFrame;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetCurrentFrame(int index) {
	index = max(0, min(_image->NumFrames() - 1, index));
	if (index != this->_currentFrame) {
		_currentTool->Finalize();
	}
	if (_animating) {
		_animating = false;
	}
	this->_currentFrame = index;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsSaved() const {
	return _saved;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetImageModified() {
	this->_saved = false;
	_aaApp->SetTitle(_imageFile.Name + "* - Ascii Artist");
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsAnimating() const {
	return _animating;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetAnimating(bool animating) {
	_currentTool->Finalize();
	this->_animating = animating;
	if (animating) {
		_animationTimer = clock();
	}
}
unsigned int PowerConsole::AsciiArtistApp::AsciiArtistManager::GetAnimationTimer() const {
	return _animationTimer;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetAnimationTimer(unsigned int animTimer) {
	this->_animationTimer = animTimer;
}

#pragma endregion
//--------------------------------
#pragma region Drawing
	
Pixel PowerConsole::AsciiArtistApp::AsciiArtistManager::GetPrimaryPixel() const {
	return _primaryPixel;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetPrimaryPixel(Pixel pixel) {
	this->_primaryPixel = pixel;
}
Pixel PowerConsole::AsciiArtistApp::AsciiArtistManager::GetSecondaryPixel() const {
	return _secondaryPixel;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetSecondaryPixel(Pixel pixel) {
	this->_secondaryPixel = pixel;
}

#pragma endregion
//--------------------------------
#pragma region Undos
	
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsUndoEnabled() const {
	return _undoEnabled;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetUndoEnabled(bool enabled) {
	this->_undoEnabled = enabled;
}
int PowerConsole::AsciiArtistApp::AsciiArtistManager::GetMaxUndos() const {
	return _maxUndos;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetMaxUndos(int maxUndos) {
	this->_maxUndos = max(1, maxUndos);
}
int PowerConsole::AsciiArtistApp::AsciiArtistManager::GetCurrentUndo() const {
	return _currentUndo;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsUndoAvailable() const {
	return (_currentUndo < (int)_undoList.size() - 1);
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsRedoAvailable() const {
	return (_currentUndo > 0);
}

#pragma endregion
//--------------------------------
#pragma region Tools
	
Tool* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetCurrentTool() const {
	return _currentTool;
}
ToolTypes PowerConsole::AsciiArtistApp::AsciiArtistManager::GetCurrentToolType() const {
	return _currentTool->GetToolType();
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetCurrentTool(ToolTypes tool) {
	if (_currentTool->GetToolType() != tool) {
		_currentTool->Finalize();
		if (_animating) {
			_animating = false;
		}

		switch (tool) {
		case ToolTypes::Pen:		_currentTool = _toolPen;		break;
		case ToolTypes::Line:		_currentTool = _toolLine;		break;
		case ToolTypes::Square:		_currentTool = _toolSquare;		break;
		case ToolTypes::Circle:		_currentTool = _toolCircle;		break;
		case ToolTypes::Flood:		_currentTool = _toolFlood;		break;
		case ToolTypes::Replace:	_currentTool = _toolReplace;	break;
		case ToolTypes::Text:		_currentTool = _toolText;		break;
		case ToolTypes::Select:		_currentTool = _toolSelect;		break;
		}

		_currentTool->Activate();
	}
}
ToolPen* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetPenTool() const {
	return _toolPen;
}
ToolLine* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetLineTool() const {
	return _toolLine;
}
ToolSquare* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetSquareTool() const {
	return _toolSquare;
}
ToolCircle* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetCircleTool() const {
	return _toolCircle;
}
ToolFlood* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetFloodTool() const {
	return _toolFlood;
}
ToolReplace* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetReplaceTool() const {
	return _toolReplace;
}
ToolText* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetTextTool() const {
	return _toolText;
}
ToolSelect* PowerConsole::AsciiArtistApp::AsciiArtistManager::GetSelectTool() const {
	return _toolSelect;
}

#pragma endregion
//--------------------------------
#pragma region Settings
	
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsShapeOutline() const {
	return _outline;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetShapeOutline(bool outline) {
	this->_outline = outline;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsPixelSpecific() const {
	return _specific;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetPixelSpecific(bool specific) {
	this->_specific = specific;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsCursorVisible() const {
	return _showCursor;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetCursorVisible(bool visible) {
	this->_showCursor = visible;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsCharacterDrawn() const {
	return _charFilter;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetCharacterDrawn(bool characterFilter) {
	this->_charFilter = characterFilter;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsFColorDrawn() const {
	return _fColorFilter;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetFColorDrawn(bool fColorFilter) {
	this->_fColorFilter = fColorFilter;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsBColorDrawn() const {
	return _bColorFilter;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetBColorDrawn(bool bColorFilter) {
	this->_bColorFilter = bColorFilter;
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::IsDebugMode() const {
	return _debugMode;
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetDebugMode(bool enabled) {
	this->_debugMode = enabled;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::AsciiArtistApp::AsciiArtistManager::UpdateAnimation() {
	if (_animating) {
		if (_currentTool->IsDrawing()) {
			_animating = false;
		}
		else {
			unsigned int time = clock();
			while (time - _animationTimer >= _image->GetAnimationSpeed()) {
				_animationTimer += _image->GetAnimationSpeed();
				_currentFrame++;
				if (_currentFrame >= (int)_image->NumFrames()) {
					_currentFrame = 0;
				}
			}
		}
	}
}
	
#pragma endregion
//============ SAVING ============
#pragma region Saving

void PowerConsole::AsciiArtistApp::AsciiArtistManager::New() {
	*_image = AsciiAnimation(Coord(64, 32), ImageFormats::FormatAttributes | ImageFormats::FormatAnimation, Pixel(' ', 0x00));
	_version = 3;
	_saved = true;
	_untitled = true;
	_imageFile.Load(FileManager::GetWorkingDirectory() + "\\" + "Untitled.asc");
	_aaApp->SetTitle(_imageFile.Name + " - Ascii Artist");
	SetMessage("New Image Created");
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::Save() {
	if (_image->SaveFile(_imageFile.Path, _version, _version == 1)) {
		_saved = true;
		_untitled = false;
		_aaApp->SetTitle(_imageFile.Name + " - Ascii Artist");
		SetMessage("Image Saved");
		return true;
	}
	else {
		SetUrgentMessage("Failed to Save File");
		return false;
	}
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::Save(File imageFile) {
	if (imageFile.Extension.empty()) {
		string path = imageFile.Path;
		if (imageFile.Name[imageFile.Name.length() - 1] == '.') {
			imageFile = File();
			imageFile.Load(path + "asc");
		}
		else {
			imageFile = File();
			imageFile.Load(path + ".asc");
		}
	}

	if (_image->SaveFile(imageFile.Path, _version, _version == 1)) {
		_saved = true;
		_untitled = false;
		_imageFile = imageFile;
		_imageFile.Load(_imageFile.Path);
		_aaApp->SetTitle(_imageFile.Name + " - Ascii Artist");
		SetMessage("Image Saved");
		FileManager::SetWorkingDirectory(_imageFile.Directory);
		
		return true;
	}
	else {
		SetUrgentMessage("Failed to Save Image");
		return false;
	}
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::Open(File imageFile) {
	if (imageFile.Extension.empty()) {
		string path = imageFile.Path;
		if (imageFile.Name[imageFile.Name.length() - 1] == '.') {
			imageFile = File();
			imageFile.Load(path + "asc");
		}
		else {
			imageFile = File();
			imageFile.Load(path + ".asc");
		}
	}

	if (_image->LoadFile(imageFile.Path)) {
		AsciiImageHeader header = AsciiImageHeader();
		FileInputStream in = FileInputStream();
		in.Open(imageFile.Path);
		if (in.IsOpen()) {
			if (AsciiImageInfo::LoadHeader(&in, header, false)) {
				_version = (int)header.Version;
			}
			in.Close();
		}
		
		FileManager::SetWorkingDirectory(imageFile.Directory);
		_saved = true;
		_untitled = false;
		_imageFile = imageFile;
		_aaApp->SetTitle(_imageFile.Name + " - Ascii Artist");
		SetMessage("Image Opened");
		
		while ((int)_undoList.size() > 0) {
			delete _undoList[0];
			_undoList.erase(_undoList.begin());
		}
		_undoList.push_back(new UndoAction("N/A", Actions::None, -1));
		_currentUndo = 0;

		_currentFrame = 0;
		_drawingBuffer->Resize(_image->Size());
		_drawingBuffer->SetBackground(_image->GetBackground());

		_canvas->SetHoverFrame(-1);
		_canvas->SetFrame(_currentFrame);

		return true;
	}
	else {
		SetUrgentMessage("Failed to Open Image");
		return false;
	}
}
bool PowerConsole::AsciiArtistApp::AsciiArtistManager::ImportFile(File imageFile, Coord size, unsigned int startPosition,
																  unsigned int charSpacing, unsigned int colorSpacing,
																  bool importChar, bool importColor) {
	FileInputStream in = FileInputStream();
	in.Open(imageFile.Path);
	if (in.IsOpen()) {
		AsciiAnimation* image = new AsciiAnimation(size, ImageFormats::FormatAttributes | ImageFormats::FormatAnimation);
		in.SetPointer(startPosition);

		for (int y = 0; y < size.Y; y++) {
			for (int x = 0; x < size.X; x++) {
				if (importChar) {
					image->GetFrame(0).SetChar(Coord(x, y), in.ReadUChar());
					in.SetPointer(charSpacing, true);
				}
				else {
					image->GetFrame(0).SetChar(Coord(x, y), _primaryPixel.Char);
				}
				if (importColor) {
					image->GetFrame(0).SetColor(Coord(x, y), in.ReadUChar());
					in.SetPointer(colorSpacing, true);
				}
				else {
					image->GetFrame(0).SetColor(Coord(x, y), _primaryPixel.Color);
				}
			}
		}

		*_image = *image;
		delete image;
		image = nullptr;

		in.Close();
		
		_saved = false;
		_untitled = true;

		FileManager::SetWorkingDirectory(imageFile.Directory);
		_imageFile = File();
		_imageFile.Load(FileManager::GetWorkingDirectory() + "\\" + "Untitled.asc");
		_aaApp->SetTitle(_imageFile.Name + "* - Ascii Artist");
		SetMessage("File Imported");
		
		while ((int)_undoList.size() > 0) {
			delete _undoList[0];
			_undoList.erase(_undoList.begin());
		}
		_undoList.push_back(new UndoAction("N/A", Actions::None, -1));
		_currentUndo = 0;

		_currentFrame = 0;
		_drawingBuffer->Resize(_image->Size());
		_drawingBuffer->SetBackground(_image->GetBackground());

		_canvas->SetHoverFrame(-1);
		_canvas->SetFrame(_currentFrame);
		return true;
	}
	else {
		SetUrgentMessage("Failed to Import File");
		return false;
	}
}
	
#pragma endregion
//=========== MESSAGES ===========
#pragma region Messages

void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetMessage(const string& message) {
	_statusBar->SetMessage(message);
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetUrgentMessage(const string& message) {
	_statusBar->SetUrgentMessage(message);
}
	
#pragma endregion
//============ UNDOS =============
#pragma region Undos
	
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Undo() {
	if (_undoEnabled) {
		if (_currentUndo < (int)_undoList.size() - 1) {
			SetImageModified();
			_currentTool->Cancel();
			if (_animating) {
				_animating = false;
			}

			UndoAction* undo = _undoList[_currentUndo];

			switch (undo->GetAction()) {
			case Actions::Draw:
			case Actions::Translate:
				_image->GetFrame(undo->GetModifiedFrame()).DrawImage(Coord::Zero, undo->GetBeforeImage()->GetFrame(0), PixelAttributes::AttributeFill, PixelOptions::OptionReplace);
				break;

			case Actions::Resize:
				_image->SetFrame(undo->GetModifiedFrame(), undo->GetBeforeImage()->GetFrame(0));
				break;

			case Actions::ResizeAll:
			case Actions::TranslateAll:
				*_image = *(undo->GetBeforeImage());
				break;
				
			case Actions::FormatChange:
				if (undo->GetModifiedFrame() == -1)
					*_image = *(undo->GetBeforeImage());
				else
					_image->SetFormat(undo->GetBeforeImage()->GetFormat());
				_image->SetAnimationSpeed(undo->GetBeforeImage()->GetAnimationSpeed());
				_image->SetBackground(undo->GetBeforeImage()->GetBackground());
				break;

			case Actions::AnimationSpeedChange:
				_image->SetAnimationSpeed(undo->GetBeforeImage()->GetAnimationSpeed());
				break;

			case Actions::AddFrame:
			case Actions::DuplicateFrame:
				_image->RemoveFrame(undo->GetModifiedFrame() + 1);
				break;
				
			case Actions::DeleteFrame:
				_image->AddFrame(undo->GetModifiedFrame(), undo->GetBeforeImage()->GetFrame(0));
				break;
				
			case Actions::MoveFrameDown:
				_image->MoveFrame(undo->GetModifiedFrame() + 1, undo->GetModifiedFrame());
				break;
				
			case Actions::MoveFrameUp:
				_image->MoveFrame(undo->GetModifiedFrame() - 1, undo->GetModifiedFrame());
				break;

			case Actions::None:
				break;
			}

			if (undo->GetModifiedFrame() != -1) {
				_currentFrame = undo->GetModifiedFrame();
			}
			_currentUndo++;

			SetMessage(undo->GetActionName() + " Action Undone");
		}
		else {
			SetMessage("No More Actions to Undo");
		}
	}
	else {
		SetMessage("Undos Disabled");
	}
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Redo() {
	if (_undoEnabled) {
		if (_currentUndo > 0) {
			SetImageModified();
			_currentTool->Cancel();
			if (_animating) {
				_animating = false;
			}

			_currentUndo--;
			UndoAction* undo = _undoList[_currentUndo];
			if (undo->GetModifiedFrame() != -1) {
				_currentFrame = undo->GetModifiedFrame();
			}

			switch (undo->GetAction()) {
			case Actions::Draw:
			case Actions::Translate:
				_image->GetFrame(undo->GetModifiedFrame()).DrawImage(Coord::Zero, undo->GetResultImage()->GetFrame(0), PixelAttributes::AttributeFill, PixelOptions::OptionReplace);
				break;

			case Actions::Resize:
				_image->SetFrame(undo->GetModifiedFrame(), undo->GetResultImage()->GetFrame(0));
				break;

			case Actions::ResizeAll:
			case Actions::TranslateAll:
				*_image = *(undo->GetResultImage());
				break;
				
			case Actions::FormatChange:
				if (undo->GetModifiedFrame() == -1) {
					*_image = *(undo->GetResultImage());
					if ((undo->GetResultImage()->GetFormat() & ImageFormats::FormatAnimation) == 0x0) {
						_currentFrame = 0;
					}
				}
				else {
					_image->SetFormat(undo->GetResultImage()->GetFormat());
				}
				_image->SetAnimationSpeed(undo->GetResultImage()->GetAnimationSpeed());
				_image->SetBackground(undo->GetResultImage()->GetBackground());
				break;

			case Actions::AnimationSpeedChange:
				_image->SetAnimationSpeed(undo->GetResultImage()->GetAnimationSpeed());
				break;

			case Actions::AddFrame:
				_currentFrame += 1;
				_image->AddFrame(_currentFrame);
				break;
			case Actions::DuplicateFrame:
				_currentFrame += 1;
				_image->AddFrame(_currentFrame, _image->GetFrame(_currentFrame - 1));
				break;
				
			case Actions::DeleteFrame:
				_image->RemoveFrame(_currentFrame);
				if (_currentFrame > 0)
					_currentFrame -= 1;
				break;
				
			case Actions::MoveFrameDown:
				_currentFrame += 1;
				_image->MoveFrame(_currentFrame - 1, _currentFrame);
				break;
				
			case Actions::MoveFrameUp:
				_currentFrame -= 1;
				_image->MoveFrame(_currentFrame + 1, _currentFrame);
				break;

			case Actions::None:
				break;
			}

			SetMessage(undo->GetActionName() + " Action Redone");
		}
		else {
			SetMessage("No More Actions to Redo");
		}
	}
	else {
		SetMessage("Undos Disabled");
	}
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::AddUndo(UndoAction* undoAction)  {
	SetImageModified();
	if (_animating) {
		_animating = false;
	}
	if (_undoEnabled) {
		if (_currentUndo != 0) {
			for (int i = 0; i < _currentUndo; i++) {
				delete _undoList[0];
				_undoList.erase(_undoList.begin());
			}
		}
		_currentUndo = 0;
		_undoList.insert(_undoList.begin(), undoAction);
		if ((int)_undoList.size() > _maxUndos + 1) {
			delete _undoList[_undoList.size() - 1];
			_undoList.pop_back();
		}
	}
}
	
#pragma endregion
//============ FORMAT ============
#pragma region Format
	
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetImageFormat(unsigned short format, unsigned int speed, Pixel background) {
	bool attributesBefore	= (_image->GetFormat() & ImageFormats::FormatAttributes) != 0x0;
	bool animationBefore	= (_image->GetFormat() & ImageFormats::FormatAnimation) != 0x0;
	bool multiSizeBefore	= (_image->GetFormat() & ImageFormats::FormatMultiSize) != 0x0;

	bool attributesResult	= (format & ImageFormats::FormatAttributes) != 0x0;
	bool animationResult	= (format & ImageFormats::FormatAnimation) != 0x0;
	bool multiSizeResult	= (format & ImageFormats::FormatMultiSize) != 0x0;

	bool allFrames = false;

	if ((attributesBefore && !attributesResult) ||
		(animationBefore && !animationResult) ||
		(multiSizeBefore && !multiSizeResult)) {
		allFrames = true;
	}

	UndoAction* undo = new UndoAction("Format Change", Actions::FormatChange, allFrames ? -1 : _currentFrame);
	if (_undoEnabled) {
		AsciiAnimation* before = nullptr;
		if (allFrames)
			before = new AsciiAnimation(*_image);
		else
			before = new AsciiAnimation(_image->GetFrame(_currentFrame));
		before->SetAnimationSpeed(_image->GetAnimationSpeed());
		undo->SetBeforeImage(before);
	}

	_image->SetFormat(format);
	_image->SetAnimationSpeed(speed);
	_image->SetBackground(background);
	if (!animationResult) {
		_currentFrame = 0;
	}
	
	if (_undoEnabled) {
		AsciiAnimation* result = nullptr;
		if (allFrames)
			result = new AsciiAnimation(*_image);
		else
			result = new AsciiAnimation(_image->GetFrame(_currentFrame));
		result->SetAnimationSpeed(speed);
		undo->SetResultImage(result);
	}
	AddUndo(undo);
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SetAnimationSpeed(unsigned int speed) {
	UndoAction* undo = new UndoAction("Speed Change", Actions::AnimationSpeedChange, _currentFrame);
	if (_undoEnabled) {
		AsciiAnimation* before = new AsciiAnimation(_image->GetFrame(_currentFrame));
		before->SetAnimationSpeed(_image->GetAnimationSpeed());
		undo->SetBeforeImage(before);
	}
	
	_image->SetAnimationSpeed(speed);

	if (_undoEnabled) {
		AsciiAnimation* result = new AsciiAnimation(_image->GetFrame(_currentFrame));
		result->SetAnimationSpeed(speed);
		undo->SetResultImage(result);
	}
	AddUndo(undo);
}
	
#pragma endregion
//============ TOOLS =============
#pragma region Tools

void PowerConsole::AsciiArtistApp::AsciiArtistManager::Resize(Coord newSize, bool allFrames) {
	if (allFrames) {
		UndoAction* undo = new UndoAction("Resize", Actions::ResizeAll, -1);
		if (_undoEnabled)
			undo->SetBeforeImage(new AsciiAnimation(*_image));

		_image->Resize(newSize);
		
		if (_undoEnabled)
			undo->SetResultImage(new AsciiAnimation(*_image));
		AddUndo(undo);

		_drawingBuffer->Resize(_image->Size());
	}
	else {
		UndoAction* undo = new UndoAction("Resize", Actions::Resize, _currentFrame);
		if (_undoEnabled)
			undo->SetBeforeImage(new AsciiAnimation(_image->GetFrame(_currentFrame)));

		_image->Resize(newSize, _currentFrame);

		if (_undoEnabled)
			undo->SetResultImage(new AsciiAnimation(_image->GetFrame(_currentFrame)));
		AddUndo(undo);
		
		_drawingBuffer->Resize(_image->Size());
	}
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Translate(Coord distance, bool allFrames) {
	if (allFrames) {
		UndoAction* undo = new UndoAction("Translate", Actions::TranslateAll, -1);
		if (_undoEnabled)
			undo->SetBeforeImage(new AsciiAnimation(*_image));
		
		_image->Translate(distance, -1);
		
		if (_undoEnabled)
			undo->SetResultImage(new AsciiAnimation(*_image));
		AddUndo(undo);
	}
	else {
		UndoAction* undo = new UndoAction("Translate", Actions::Translate, _currentFrame);
		if (_undoEnabled)
			undo->SetBeforeImage(new AsciiAnimation(_image->GetFrame(_currentFrame)));
		
		_image->Translate(distance, _currentFrame);
		
		if (_undoEnabled)
			undo->SetResultImage(new AsciiAnimation(_image->GetFrame(_currentFrame)));
		AddUndo(undo);
	}
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Copy() {
	if (_currentTool->GetToolType() == ToolTypes::Select) {
		_toolSelect->Copy();
	}
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Cut() {
	if (_currentTool->GetToolType() == ToolTypes::Select) {
		_toolSelect->Cut();
	}
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Delete() {
	if (_currentTool->GetToolType() == ToolTypes::Select) {
		_toolSelect->Delete();
	}
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::Paste() {
	SetCurrentTool(ToolTypes::Select);
	_toolSelect->Paste();
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::SelectAll() {
	SetCurrentTool(ToolTypes::Select);
	_toolSelect->SelectAll();
}
	
#pragma endregion
//============ FRAMES ============
#pragma region Frames
	
void PowerConsole::AsciiArtistApp::AsciiArtistManager::AddFrame() {
	_currentTool->Finalize();

	UndoAction* undo = new UndoAction("Add Frame", Actions::AddFrame, _currentFrame);
	AddUndo(undo);

	_currentFrame += 1;
	if (_currentFrame == _image->NumFrames())
		_image->AddFrame();
	else
		_image->AddFrame(_currentFrame);
	
	SetMessage("Frame Added");
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::DuplicateFrame() {
	_currentTool->Finalize();

	UndoAction* undo = new UndoAction("Duplicate Frame", Actions::DuplicateFrame, _currentFrame);
	AddUndo(undo);

	_currentFrame += 1;
	_image->AddFrame(_currentFrame, _image->GetFrame(_currentFrame - 1));
	
	SetMessage("Frame Duplicated");
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::DeleteFrame() {
	_currentTool->Finalize();

	UndoAction* undo = new UndoAction("Remove Frame", Actions::DeleteFrame, _currentFrame);
	if (_undoEnabled)
		undo->SetBeforeImage(new AsciiAnimation(_image->GetFrame(_currentFrame)));
	AddUndo(undo);

	_image->RemoveFrame(_currentFrame);
	if (_currentFrame > 0)
		_currentFrame -= 1;
	
	SetMessage("Frame Removed");
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::MoveFrameDown() {
	_currentTool->Finalize();

	UndoAction* undo = new UndoAction("Move Frame Down", Actions::MoveFrameDown, _currentFrame);
	AddUndo(undo);

	_currentFrame += 1;
	_image->MoveFrame(_currentFrame - 1, _currentFrame);
	
	SetMessage("Frame Moved Down");
}
void PowerConsole::AsciiArtistApp::AsciiArtistManager::MoveFrameUp() {
	_currentTool->Finalize();

	UndoAction* undo = new UndoAction("Move Frame Up", Actions::MoveFrameUp, _currentFrame);
	AddUndo(undo);

	_currentFrame -= 1;
	_image->MoveFrame(_currentFrame + 1, _currentFrame);
	
	SetMessage("Frame Moved Up");
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|