/*=================================================================|
|* File:				AsciiArtistWindow.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_ARTIST_WINDOW_H
#define ASCII_ARTIST_WINDOW_H

#include "stdafx.h"
#include <PowerConsole/Helpers/fixed_array.h>
#include <PowerConsole/Controls/MainWindow.h>
#include <PowerConsole/Controls/Dialog.h>
#include <PowerConsole/Controls/Toolbox/Panel.h>
#include <PowerConsole/Controls/Toolbox/ScrollViewer.h>
#include <PowerConsole/Controls/Toolbox/StackPanel.h>
#include <PowerConsole/Controls/Toolbox/Button.h>
#include <PowerConsole/Controls/Toolbox/ImageButton.h>
#include <PowerConsole/Controls/Toolbox/ListBox.h>
#include <PowerConsole/Controls/Toolbox/ToggleButton.h>
#include <PowerConsole/Controls/Toolbox/SplitContainer.h>
#include <AsciiArtist/Toolbox/AsciiArtistEventArgs.h>
#include <AsciiArtist/Toolbox/Controls/Canvas.h>
#include <AsciiArtist/Toolbox/Controls/FrameListBox.h>
#include <AsciiArtist/Toolbox/Controls/PixelDisplay.h>
#include <AsciiArtist/Toolbox/Controls/ColorPalette.h>
#include <AsciiArtist/Toolbox/Controls/CharacterGrid.h>
#include <AsciiArtist/Toolbox/Controls/PixelBookmarker.h>
#include <AsciiArtist/Toolbox/Controls/StatusBar.h>
#include <AsciiArtist/Toolbox/Dialogs/ImportFileDialog.h>
#include <AsciiArtist/Toolbox/Dialogs/ImportImageDialog.h>

using namespace PowerConsole;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiArtist::Toolbox;
using namespace AsciiArtist::Toolbox::Controls;
using namespace AsciiArtist::Toolbox::Dialogs;

namespace AsciiArtist {
//=================================================================|
// CLASSES														   |
//=================================================================/

class AsciiArtistWindow : public MainWindow {

	//========== CONSTANTS ===========
	#pragma region Constants

	static const Point2I DEFAULT_WINDOW_SIZE;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
	//--------------------------------
	#pragma region Controls

	FrameListBoxSPtr frameList;

	ListBoxSPtr toolList;

	ScrollViewerSPtr scrollViewerCanvas;
	CanvasSPtr canvas;

	ToggleButtonSPtr toggleButtonCh;
	ToggleButtonSPtr toggleButtonFg;
	ToggleButtonSPtr toggleButtonBg;
	ToggleButtonSPtr toggleButtonCursor;
	StackPanelSPtr stackPanelOptions;


	PixelDisplaySPtr pixelDisplay;
	ColorPaletteSPtr colorPalette;
	CharacterGridSPtr characterGrid;
	PixelBookmarkerSPtr pixelBookmarker;

	SplitContainerSPtr splitContainer;
	PanelSPtr panelFrames;

	StackPanelSPtr stackPanelToolbar;
	StackPanelSPtr stackPanelFrameToolbar;
	StackPanelSPtr stackPanelAnimationToolbar;

	ImageButtonSPtr buttonNew;
	ImageButtonSPtr buttonOpen;
	ImageButtonSPtr buttonSave;
	ImageButtonSPtr buttonSaveAs;
	ImageButtonSPtr buttonCopy;
	ImageButtonSPtr buttonCut;
	ImageButtonSPtr buttonDelete;
	ImageButtonSPtr buttonPaste;
	ImageButtonSPtr buttonSelectAll;
	ImageButtonSPtr buttonUndo;
	ImageButtonSPtr buttonRedo;
	ImageButtonSPtr buttonResize;
	ImageButtonSPtr buttonTranslate;
	ImageButtonSPtr buttonImportImage;
	ImageButtonSPtr buttonImportFile;
	ImageButtonSPtr buttonExportImage;
	ImageButtonSPtr buttonExportGif;
	ImageButtonSPtr buttonExportVideo;

	ImageButtonSPtr buttonNewFrame;
	ImageButtonSPtr buttonDuplicateFrame;
	ImageButtonSPtr buttonDeleteFrame;
	ImageButtonSPtr buttonMoveFrameDown;
	ImageButtonSPtr buttonMoveFrameUp;

	ImageButtonSPtr buttonPlay;
	ImageButtonSPtr buttonStop;
	ImageButtonSPtr buttonShowSpeed;
	ImageButtonSPtr buttonFrameSpeed;
	ImageButtonSPtr buttonRenameFrame;

	AsciiAnimationSPtr ascPlayOff;
	AsciiAnimationSPtr ascPlayOn;
	AsciiAnimationSPtr ascStopOff;
	AsciiAnimationSPtr ascStopOn;
	AsciiAnimationSPtr ascShowSpeedOff;
	AsciiAnimationSPtr ascShowSpeedOn;

	StatusBarSPtr statusBar;

	StackPanelSPtr stackPanelSettingsToolbar;

	ImageButtonSPtr buttonHelp;
	ImageButtonSPtr buttonAbout;
	ImageButtonSPtr buttonProgramSettings;
	ImageButtonSPtr buttonImageSettings;

	#pragma endregion
	//--------------------------------
	#pragma region Other

	/* The command line arguments of the program. */
	fixed_array<std::string> commandLine;
	/* True if the image file has been changed since last saving. */
	bool changed;
	/* The path of the current image file. */
	std::string filePath;
	/* True if the image has not been saved once yet. */
	bool untitled;
	/* The name of the file that was imported. Empty if there is none. */
	std::string importFilePath;
	/* The previously used import image settings. */
	ImportImageSettings importImageSettings;
	/* The previously used import file settings. */
	ImportFileSettings importFileSettings;
	/* True if the import file was an image and not a standard file. */
	bool importIsImage;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the default console window. */
	AsciiArtistWindow(int argc, char** argv);
private:
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region Command Line
public:
	/* Gets the command line arguments. */
	fixed_array<std::string> getCommandLineArguments() const;
	/* Gets the command line argument at the specified index. */
	std::string getCommandLineArgument(int index) const;
	/* Gets the bumber of command line arguments. */
	int getNumCommandLineArguments() const;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers

	/* Updates the window title. */
	void updateTitle();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onScrollViewerPaint(ControlSPtr sender, PaintEventArgs& e);

	// Options
	void onCursorClicked(ControlSPtr sender, MouseEventArgs& e);
	void onChClicked(ControlSPtr sender, MouseEventArgs& e);
	void onFgClicked(ControlSPtr sender, MouseEventArgs& e);
	void onBgClicked(ControlSPtr sender, MouseEventArgs& e);
	void onOptionClicked(ControlSPtr sender, MouseEventArgs& e);

	// Palette
	void onColorPalettePixelChanged(ControlSPtr sender, PixelChangedEventArgs& e);
	void onPixelDisplayPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e);
	void onCharacterGridPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e);
	void onPixelBookmarkerPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e);

	// Frame List
	void onFrameListSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e);
	void onFrameListPreviewChanged(ControlSPtr sender, SelectionChangedEventArgs& e);
	void onFrameListSelectionDoubleClicked(ControlSPtr sender, EventArgs& e);

	// Tool List
	void onToolListSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e);

	// Canvas
	void onCanvasScrollViewerMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);
	void onCanvasImageChanged(ControlSPtr sender, EventArgs& e);
	void onCanvasMessageSent(ControlSPtr sender, MessageEventArgs& e);
	void onCanvasToolChanged(ControlSPtr sender, SelectionChangedEventArgs& e);
	void onCanvasUndoAvailabilityChanged(ControlSPtr sender, UndoAvailabilityEventArgs& e);
	void onCanvasPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e);
	void onCanvasStatusBarDimensions(ControlSPtr sender, StatusBarDimensionsEventArgs& e);
	void onCanvasAnimationCanceled(ControlSPtr sender, EventArgs& e);

	// Canvas Frame Changes
	void onCanvasFrameChanged(ControlSPtr sender, SelectionChangedEventArgs& e);
	void onCanvasFrameListChanged(ControlSPtr sender, FrameListChangedEventArgs& e);
	void onCanvasFrameAdded(ControlSPtr sender, FrameModifiedEventArgs& e);
	void onCanvasFrameRemoved(ControlSPtr sender, FrameModifiedEventArgs& e);
	void onCanvasFrameRenamed(ControlSPtr sender, FrameModifiedEventArgs& e);
	void onCanvasFrameMoved(ControlSPtr sender, FrameMovedEventArgs& e);

	#pragma endregion
	//======= COMMAND HANDLERS =======
	#pragma region Command Handlers

	void onCloseCommand(ControlSPtr sender, EventArgs& e);

	void onNewCommand(ControlSPtr sender, EventArgs& e);
	void onOpenCommand(ControlSPtr sender, EventArgs& e);
	void onSaveCommand(ControlSPtr sender, EventArgs& e);
	void onSaveAsCommand(ControlSPtr sender, EventArgs& e);

	void onCopyCommand(ControlSPtr sender, EventArgs& e);
	void onCutCommand(ControlSPtr sender, EventArgs& e);
	void onDeleteCommand(ControlSPtr sender, EventArgs& e);
	void onPasteCommand(ControlSPtr sender, EventArgs& e);
	void onSelectAllCommand(ControlSPtr sender, EventArgs& e);

	void onUndoCommand(ControlSPtr sender, EventArgs& e);
	void onRedoCommand(ControlSPtr sender, EventArgs& e);

	void onResizeCommand(ControlSPtr sender, EventArgs& e);
	void onTranslateCommand(ControlSPtr sender, EventArgs& e);
	void onImportImageCommand(ControlSPtr sender, EventArgs& e);
	void onImportFileCommand(ControlSPtr sender, EventArgs& e);
	void onExportImageCommand(ControlSPtr sender, EventArgs& e);
	void onExportGifCommand(ControlSPtr sender, EventArgs& e);
	void onExportVideoCommand(ControlSPtr sender, EventArgs& e);

	void onNewFrameCommand(ControlSPtr sender, EventArgs& e);
	void onDuplicateFrameCommand(ControlSPtr sender, EventArgs& e);
	void onDeleteFrameCommand(ControlSPtr sender, EventArgs& e);
	void onMoveFrameDownCommand(ControlSPtr sender, EventArgs& e);
	void onMoveFrameUpCommand(ControlSPtr sender, EventArgs& e);
	void onRenameFrameCommand(ControlSPtr sender, EventArgs& e);

	void onPlayCommand(ControlSPtr sender, EventArgs& e);
	void onStopCommand(ControlSPtr sender, EventArgs& e);
	void onShowSpeedCommand(ControlSPtr sender, EventArgs& e);
	void onFrameSpeedCommand(ControlSPtr sender, EventArgs& e);

	void onAboutCommand(ControlSPtr sender, EventArgs& e);
	void onHelpCommand(ControlSPtr sender, EventArgs& e);
	void onProgramSettingsCommand(ControlSPtr sender, EventArgs& e);
	void onImageSettingsCommand(ControlSPtr sender, EventArgs& e);
	void onPaletteEditorCommand(ControlSPtr sender, EventArgs& e);
	void onFontChooserCommand(ControlSPtr sender, EventArgs& e);

	void onMaximizeCommand(ControlSPtr sender, EventArgs& e);
	void onRestoreCommand(ControlSPtr sender, EventArgs& e);

	void onToolCommand(ControlSPtr sender, EventArgs& e, const std::string& toolName);

	void onAssignBookmarkerPixelCommand(ControlSPtr sender, EventArgs& e, int index);
	void onSelectBookmarkerPixelCommand(ControlSPtr sender, EventArgs& e, int index);

	#pragma endregion
};

//=================================================================|
} /* AsciiArtist */
#endif /* ASCII_ARTIST_WINDOW_H */
//=================================================================|
