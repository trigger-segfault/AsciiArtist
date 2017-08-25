/*=================================================================|
|* File:				AsciiArtistWindow.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resources.h"
#include "AsciiArtistWindow.h"
#include <PowerConsole/PowerConsoleSettings.h>
#include <PowerConsole/Controls/Toolbox/ToggleButton.h>
#include <PowerConsole/Controls/Toolbox/SplitContainer.h>
#include <PowerConsole/Controls/Toolbox/ScrollViewer.h>
#include <PowerConsole/Controls/Toolbox/FileDialog.h>
#include <PowerConsole/Controls/Toolbox/StackPanel.h>
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <PowerConsole/Controls/Toolbox/TextEditBox.h>
#include <PowerConsole/Drawing/AsciiImageFileDetails.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/IO/ResourceStream.h>
#include <PowerConsole/Console.h>
#include <AsciiArtist/Editing/Tool.h>
#include <AsciiArtist/Toolbox/Dialogs/ResizeDialogBox.h>
#include <AsciiArtist/Toolbox/Dialogs/TranslateDialogBox.h>
#include <AsciiArtist/Toolbox/Dialogs/FrameSpeedDialogBox.h>
#include <AsciiArtist/Toolbox/Dialogs/AsciiImageFileDialog.h>
#include <AsciiArtist/Toolbox/Dialogs/ImageSettingsDialog.h>
#include <AsciiArtist/Toolbox/Dialogs/AboutDialog.h>
#include <AsciiArtist/Toolbox/Dialogs/PaletteEditor.h>
#include <AsciiArtist/Toolbox/Dialogs/FontChooser.h>
#include <AsciiArtist/Toolbox/Dialogs/LoadingDialog.h>
#include <AsciiArtist/Settings.h>
#include <AsciiArtist/Editing/Asciifier.h>
#include <AsciiArtist/COM/WIC.h>

using namespace std;
using namespace AsciiArtist;
using namespace AsciiArtist::COM;
using namespace AsciiArtist::Editing;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::IO;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const Point2I AsciiArtistWindow::DEFAULT_WINDOW_SIZE = Point2I(102, 38);

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

AsciiArtistWindow::AsciiArtistWindow(int argc, char** argv) 
 :	filePath(""),
	untitled(true),
	changed(false),
	importFilePath(""),
	importIsImage(false) {

	srand((unsigned int)time(nullptr));

	eventInitialize().add(bindEvent(AsciiArtistWindow::onInitialize));
	eventLoad().add(bindEvent(AsciiArtistWindow::onLoad));
	eventPaint().add(bindEvent(AsciiArtistWindow::onPaint));

	commandLine = fixed_array<std::string>((size_t)argc);
	for (int i = 0; i < argc; i++)
		commandLine[i] = argv[i];

	PowerConsoleSettings::setDrawEntireBuffer(false);
}

void AsciiArtistWindow::onInitialize(ControlSPtr sender, EventArgs& e) {
	Settings::load();

	AsciiAnimation buttonImage;

	#pragma region Window Setup

	setSize(DEFAULT_WINDOW_SIZE);
	setMinSize(Point2I(89, 36));
	setResizeHandleEnabled(true);
	updateTitle();
	console().setIconFromResourceByIndex(0);

	#pragma endregion

	#pragma region Toolbar

	stackPanelToolbar = makeControl(StackPanel);
	stackPanelToolbar->setOrientation(Orientations::Horizontal);
	stackPanelToolbar->setSize(Point2I(SIZE_AUTO, 1));
	stackPanelToolbar->setMargin(Thickness(1, 1, 0, 0));
	stackPanelToolbar->setSpacing(1);
	children().add(stackPanelToolbar);

	buttonImage.loadResource(ASC_BUTTON_NEW_FILE);
	buttonNew = makeControl(ImageButton);
	buttonNew->setSize(Point2I(3, 1));
	buttonNew->setImage(std::move(buttonImage));
	buttonNew->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onNewCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonNew);

	buttonImage.loadResource(ASC_BUTTON_OPEN_FILE);
	buttonOpen = makeControl(ImageButton);
	buttonOpen->setSize(Point2I(3, 1));
	buttonOpen->setImage(std::move(buttonImage));
	buttonOpen->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onOpenCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonOpen);

	buttonImage.loadResource(ASC_BUTTON_SAVE_FILE);
	buttonSave = makeControl(ImageButton);
	buttonSave->setSize(Point2I(3, 1));
	buttonSave->setImage(std::move(buttonImage));
	buttonSave->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onSaveCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonSave);

	buttonImage.loadResource(ASC_BUTTON_SAVE_AS_FILE);
	buttonSaveAs = makeControl(ImageButton);
	buttonSaveAs->setSize(Point2I(3, 1));
	buttonSaveAs->setImage(std::move(buttonImage));
	buttonSaveAs->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onSaveAsCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonSaveAs);

	buttonImage.loadResource(ASC_BUTTON_COPY);
	buttonCopy = makeControl(ImageButton);
	buttonCopy->setSize(Point2I(3, 1));
	buttonCopy->setImage(std::move(buttonImage));
	buttonCopy->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onCopyCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonCopy);

	buttonImage.loadResource(ASC_BUTTON_CUT);
	buttonCut = makeControl(ImageButton);
	buttonCut->setSize(Point2I(3, 1));
	buttonCut->setImage(std::move(buttonImage));
	buttonCut->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onCutCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonCut);

	buttonImage.loadResource(ASC_BUTTON_DELETE);
	buttonDelete = makeControl(ImageButton);
	buttonDelete->setSize(Point2I(3, 1));
	buttonDelete->setImage(std::move(buttonImage));
	buttonDelete->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onDeleteCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonDelete);

	buttonImage.loadResource(ASC_BUTTON_PASTE);
	buttonPaste = makeControl(ImageButton);
	buttonPaste->setSize(Point2I(3, 1));
	buttonPaste->setImage(std::move(buttonImage));
	buttonPaste->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onPasteCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonPaste);

	buttonImage.loadResource(ASC_BUTTON_SELECT_ALL);
	buttonSelectAll = makeControl(ImageButton);
	buttonSelectAll->setSize(Point2I(3, 1));
	buttonSelectAll->setImage(std::move(buttonImage));
	buttonSelectAll->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onSelectAllCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonSelectAll);

	buttonImage.loadResource(ASC_BUTTON_UNDO);
	buttonUndo = makeControl(ImageButton);
	buttonUndo->setSize(Point2I(3, 1));
	buttonUndo->setEnabled(false);
	buttonUndo->setImage(std::move(buttonImage));
	buttonUndo->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onUndoCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonUndo);

	buttonImage.loadResource(ASC_BUTTON_REDO);
	buttonRedo = makeControl(ImageButton);
	buttonRedo->setSize(Point2I(3, 1));
	buttonRedo->setEnabled(false);
	buttonRedo->setImage(std::move(buttonImage));
	buttonRedo->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onRedoCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonRedo);

	buttonImage.loadResource(ASC_BUTTON_RESIZE);
	buttonResize = makeControl(ImageButton);
	buttonResize->setSize(Point2I(3, 1));
	buttonResize->setImage(std::move(buttonImage));
	buttonResize->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onResizeCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonResize);

	buttonImage.loadResource(ASC_BUTTON_TRANSLATE);
	buttonTranslate = makeControl(ImageButton);
	buttonTranslate->setSize(Point2I(3, 1));
	buttonTranslate->setImage(std::move(buttonImage));
	buttonTranslate->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onTranslateCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonTranslate);

	buttonImage.loadResource(ASC_BUTTON_IMPORT_IMAGE);
	buttonImportImage = makeControl(ImageButton);
	buttonImportImage->setSize(Point2I(3, 1));
	buttonImportImage->setImage(std::move(buttonImage));
	buttonImportImage->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onImportImageCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonImportImage);

	buttonImage.loadResource(ASC_BUTTON_IMPORT_FILE);
	buttonImportFile = makeControl(ImageButton);
	buttonImportFile->setSize(Point2I(3, 1));
	buttonImportFile->setImage(std::move(buttonImage));
	buttonImportFile->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onImportFileCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonImportFile);

	buttonImage.loadResource(ASC_BUTTON_EXPORT_IMAGE);
	buttonExportImage = makeControl(ImageButton);
	buttonExportImage->setSize(Point2I(3, 1));
	buttonExportImage->setImage(std::move(buttonImage));
	buttonExportImage->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onExportImageCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonExportImage);

	buttonImage.loadResource(ASC_BUTTON_EXPORT_GIF);
	buttonExportGif = makeControl(ImageButton);
	buttonExportGif->setSize(Point2I(3, 1));
	buttonExportGif->setImage(std::move(buttonImage));
	buttonExportGif->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onExportGifCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonExportGif);

	buttonImage.loadResource(ASC_BUTTON_EXPORT_VIDEO);
	buttonExportVideo = makeControl(ImageButton);
	buttonExportVideo->setSize(Point2I(3, 1));
	buttonExportVideo->setImage(std::move(buttonImage));
	buttonExportVideo->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onExportVideoCommand(sender, e);
	});
	stackPanelToolbar->children().add(buttonExportVideo);

	#pragma endregion

	splitContainer = makeControl(SplitContainer);
	splitContainer->setSize(Point2I(SIZE_AUTO));
	splitContainer->setMargin(Thickness(21, 2, 0, 2));
	splitContainer->setRightFixed(true);
	splitContainer->setRightMinWidth(15);
	splitContainer->setRightMaxWidth(24);
	splitContainer->setHorizontalAlignment(HorizontalAlignments::Stretch);
	splitContainer->setVerticalAlignment(VerticalAlignments::Stretch);
	children().add(splitContainer);

	panelFrames = makeControl(Panel);
	panelFrames->setSize(Point2I(SIZE_AUTO));
	//panelFrames->setSize(Point2I(19, SIZE_AUTO));
	//panelFrames->setMargin(Thickness(0, 2, 0, 2));
	//panelFrames->setHorizontalAlignment(HorizontalAlignments::Right);
	panelFrames->setHorizontalAlignment(HorizontalAlignments::Stretch);
	panelFrames->setVerticalAlignment(VerticalAlignments::Stretch);
	splitContainer->setRightContent(panelFrames);
	//children().add(panelFrames);

	#pragma region Frame Toolbar

	stackPanelFrameToolbar = makeControl(StackPanel);
	stackPanelFrameToolbar->setOrientation(Orientations::Horizontal);
	stackPanelFrameToolbar->setSize(Point2I(SIZE_AUTO, 1));
	stackPanelFrameToolbar->setMargin(Thickness(1, 1, 1, 0));
	stackPanelFrameToolbar->setSpacing(1);
	panelFrames->children().add(stackPanelFrameToolbar);

	buttonImage.loadResource(ASC_BUTTON_NEW_FRAME);
	buttonNewFrame = makeControl(ImageButton);
	buttonNewFrame->setSize(Point2I(2, 1));
	buttonNewFrame->setImage(std::move(buttonImage));
	buttonNewFrame->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onNewFrameCommand(sender, e);
	});
	stackPanelFrameToolbar->children().add(buttonNewFrame);

	buttonImage.loadResource(ASC_BUTTON_DUPLICATE_FRAME);
	buttonDuplicateFrame = makeControl(ImageButton);
	buttonDuplicateFrame->setSize(Point2I(2, 1));
	buttonDuplicateFrame->setImage(std::move(buttonImage));
	buttonDuplicateFrame->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onDuplicateFrameCommand(sender, e);
	});
	stackPanelFrameToolbar->children().add(buttonDuplicateFrame);

	buttonImage.loadResource(ASC_BUTTON_DELETE_FRAME);
	buttonDeleteFrame = makeControl(ImageButton);
	buttonDeleteFrame->setSize(Point2I(2, 1));
	buttonDeleteFrame->setImage(std::move(buttonImage));
	buttonDeleteFrame->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onDeleteFrameCommand(sender, e);
	});
	stackPanelFrameToolbar->children().add(buttonDeleteFrame);

	buttonImage.loadResource(ASC_BUTTON_MOVE_FRAME_DOWN);
	buttonMoveFrameDown = makeControl(ImageButton);
	buttonMoveFrameDown->setSize(Point2I(2, 1));
	buttonMoveFrameDown->setImage(std::move(buttonImage));
	buttonMoveFrameDown->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onMoveFrameDownCommand(sender, e);
	});
	stackPanelFrameToolbar->children().add(buttonMoveFrameDown);

	buttonImage.loadResource(ASC_BUTTON_MOVE_FRAME_UP);
	buttonMoveFrameUp = makeControl(ImageButton);
	buttonMoveFrameUp->setSize(Point2I(2, 1));
	buttonMoveFrameUp->setImage(std::move(buttonImage));
	buttonMoveFrameUp->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onMoveFrameUpCommand(sender, e);
	});
	stackPanelFrameToolbar->children().add(buttonMoveFrameUp);

	#pragma endregion

	#pragma region Animation Toolbar

	stackPanelAnimationToolbar = makeControl(StackPanel);
	stackPanelAnimationToolbar->setOrientation(Orientations::Horizontal);
	stackPanelAnimationToolbar->setSize(Point2I(SIZE_AUTO, 1));
	stackPanelAnimationToolbar->setMargin(Thickness(1, 0, 1, 1));
	stackPanelAnimationToolbar->setVerticalAlignment(VerticalAlignments::Bottom);
	stackPanelAnimationToolbar->setSpacing(1);
	panelFrames->children().add(stackPanelAnimationToolbar);

	/*toggleButtonAnimate = makeControl(ToggleButton);
	toggleButtonAnimate->setText("Animate");
	toggleButtonAnimate->setSize(Point2I(11, 1));
	toggleButtonAnimate->setMargin(Thickness(0, 0, 4, 3));
	toggleButtonAnimate->setHorizontalAlignment(HorizontalAlignments::Right);
	toggleButtonAnimate->setVerticalAlignment(VerticalAlignments::Bottom);
	toggleButtonAnimate->setTextAlignment(HorizontalAlignments::Center);
	toggleButtonAnimate->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onPlayCommand(sender, e);
	});
	children().add(toggleButtonAnimate);*/

	ascPlayOff = std::make_shared<AsciiAnimation>();
	ascPlayOff->loadResource(ASC_BUTTON_PLAY_OFF);
	ascPlayOn = std::make_shared<AsciiAnimation>();
	ascPlayOn->loadResource(ASC_BUTTON_PLAY_ON);
	buttonPlay = makeControl(ImageButton);
	buttonPlay->setSize(Point2I(2, 1));
	buttonPlay->setImage(*ascPlayOff);
	buttonPlay->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onPlayCommand(sender, e);
	});
	stackPanelAnimationToolbar->children().add(buttonPlay);

	ascStopOff = std::make_shared<AsciiAnimation>();
	ascStopOff->loadResource(ASC_BUTTON_STOP_OFF);
	ascStopOn = std::make_shared<AsciiAnimation>();
	ascStopOn->loadResource(ASC_BUTTON_STOP_ON);
	buttonStop = makeControl(ImageButton);
	buttonStop->setSize(Point2I(2, 1));
	buttonStop->setImage(*ascStopOn);
	buttonStop->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onStopCommand(sender, e);
	});
	stackPanelAnimationToolbar->children().add(buttonStop);

	ascShowSpeedOff = std::make_shared<AsciiAnimation>();
	ascShowSpeedOff->loadResource(ASC_BUTTON_SHOW_SPEED_OFF);
	ascShowSpeedOn = std::make_shared<AsciiAnimation>();
	ascShowSpeedOn->loadResource(ASC_BUTTON_SHOW_SPEED_ON);
	buttonShowSpeed = makeControl(ImageButton);
	buttonShowSpeed->setSize(Point2I(2, 1));
	buttonShowSpeed->setImage(*ascShowSpeedOff);
	buttonShowSpeed->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onShowSpeedCommand(sender, e);
	});
	stackPanelAnimationToolbar->children().add(buttonShowSpeed);

	buttonImage.loadResource(ASC_BUTTON_FRAME_SPEED);
	buttonFrameSpeed = makeControl(ImageButton);
	buttonFrameSpeed->setSize(Point2I(2, 1));
	buttonFrameSpeed->setImage(std::move(buttonImage));
	buttonFrameSpeed->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onFrameSpeedCommand(sender, e);
	});
	stackPanelAnimationToolbar->children().add(buttonFrameSpeed);

	buttonImage.loadResource(ASC_BUTTON_RENAME_FRAME);
	buttonRenameFrame = makeControl(ImageButton);
	buttonRenameFrame->setSize(Point2I(2, 1));
	buttonRenameFrame->setImage(std::move(buttonImage));
	buttonRenameFrame->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onRenameFrameCommand(sender, e);
	});
	stackPanelAnimationToolbar->children().add(buttonRenameFrame);

	#pragma endregion

	#pragma region Settings Toolbar

	stackPanelSettingsToolbar = makeControl(StackPanel);
	stackPanelSettingsToolbar->setOrientation(Orientations::Horizontal);
	stackPanelSettingsToolbar->setSize(Point2I(SIZE_AUTO, 1));
	stackPanelSettingsToolbar->setMargin(Thickness(0, 1, 1, 0));
	stackPanelSettingsToolbar->setHorizontalAlignment(HorizontalAlignments::Right);
	stackPanelSettingsToolbar->setSpacing(1);
	children().add(stackPanelSettingsToolbar);

	buttonImage.loadResource(ASC_BUTTON_IMAGE_SETTINGS);
	buttonImageSettings = makeControl(ImageButton);
	buttonImageSettings->setSize(Point2I(3, 1));
	buttonImageSettings->setImage(std::move(buttonImage));
	buttonImageSettings->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onImageSettingsCommand(sender, e);
	});
	stackPanelSettingsToolbar->children().add(buttonImageSettings);

	buttonImage.loadResource(ASC_BUTTON_PROGRAM_SETTINGS);
	buttonProgramSettings = makeControl(ImageButton);
	buttonProgramSettings->setSize(Point2I(3, 1));
	buttonProgramSettings->setImage(std::move(buttonImage));
	buttonProgramSettings->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onProgramSettingsCommand(sender, e);
	});
	stackPanelSettingsToolbar->children().add(buttonProgramSettings);

	buttonImage.loadResource(ASC_BUTTON_HELP);
	buttonHelp = makeControl(ImageButton);
	buttonHelp->setSize(Point2I(3, 1));
	buttonHelp->setImage(std::move(buttonImage));
	buttonHelp->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onHelpCommand(sender, e);
	});
	stackPanelSettingsToolbar->children().add(buttonHelp);

	buttonImage.loadResource(ASC_BUTTON_ABOUT);
	buttonAbout = makeControl(ImageButton);
	buttonAbout->setSize(Point2I(3, 1));
	buttonAbout->setImage(std::move(buttonImage));
	buttonAbout->eventClick().add("[]", [this](ControlSPtr sender, MouseEventArgs& e) {
		onAboutCommand(sender, e);
	});
	stackPanelSettingsToolbar->children().add(buttonAbout);

	#pragma endregion

	#pragma region Options

	toggleButtonCh = makeControl(ToggleButton);
	toggleButtonCh->setText("Ch");
	toggleButtonCh->setChecked(true);
	toggleButtonCh->setMargin(Thickness(2, 8, 0, 0));
	toggleButtonCh->eventClick().add(bindEvent(AsciiArtistWindow::onChClicked));
	children().add(toggleButtonCh);

	toggleButtonFg = makeControl(ToggleButton);
	toggleButtonFg->setText("Fg");
	toggleButtonFg->setChecked(true);
	toggleButtonFg->setMargin(Thickness(5, 8, 0, 0));
	toggleButtonFg->eventClick().add(bindEvent(AsciiArtistWindow::onFgClicked));
	children().add(toggleButtonFg);

	toggleButtonBg = makeControl(ToggleButton);
	toggleButtonBg->setText("Bg");
	toggleButtonBg->setChecked(true);
	toggleButtonBg->setMargin(Thickness(8, 8, 0, 0));
	toggleButtonBg->eventClick().add(bindEvent(AsciiArtistWindow::onBgClicked));
	children().add(toggleButtonBg);

	toggleButtonCursor = makeControl(ToggleButton);
	toggleButtonCursor->setText("Cursor");
	toggleButtonCursor->setChecked(true);
	toggleButtonCursor->setMargin(Thickness(2, 9, 0, 0));
	toggleButtonCursor->eventClick().add(bindEvent(AsciiArtistWindow::onCursorClicked));
	children().add(toggleButtonCursor);

	stackPanelOptions = makeControl(StackPanel);
	stackPanelOptions->setSize(Point2I(10, 3));
	stackPanelOptions->setMargin(Thickness(1, 10, 0, 0));
	children().add(stackPanelOptions);

	#pragma endregion

	#pragma region Palettes

	colorPalette = makeControl(ColorPalette);
	colorPalette->setMargin(Thickness(1, 3, 0, 0));
	colorPalette->eventPixelChanged().add(bindEvent(AsciiArtistWindow::onColorPalettePixelChanged));
	children().add(colorPalette);

	pixelDisplay = makeControl(PixelDisplay);
	pixelDisplay->setMargin(Thickness(1, 6, 0, 0));
	pixelDisplay->setShowCharacter(false);
	pixelDisplay->eventPixelChanged().add(bindEvent(AsciiArtistWindow::onPixelDisplayPixelChanged));
	children().add(pixelDisplay);

	characterGrid = makeControl(CharacterGrid);
	characterGrid->setMargin(Thickness(1, 14, 0, 6));
	characterGrid->eventPixelChanged().add(bindEvent(AsciiArtistWindow::onCharacterGridPixelChanged));
	children().add(characterGrid);

	pixelBookmarker = makeControl(PixelBookmarker);
	pixelBookmarker->setVerticalAlignment(VerticalAlignments::Bottom);
	pixelBookmarker->setMargin(Thickness(1, 0, 0, 3));
	pixelBookmarker->eventPixelChanged().add(bindEvent(AsciiArtistWindow::onPixelBookmarkerPixelChanged));
	children().add(pixelBookmarker);

	#pragma endregion

	scrollViewerCanvas = makeControl(ScrollViewer);
	scrollViewerCanvas->setSize(Point2I(SIZE_AUTO));
	//scrollViewerCanvas->setMargin(Thickness(21, 2, 18, 2));
	scrollViewerCanvas->setHorizontalAlignment(HorizontalAlignments::Stretch);
	scrollViewerCanvas->setVerticalAlignment(VerticalAlignments::Stretch);
	scrollViewerCanvas->setPadding(Thickness(1));
	scrollViewerCanvas->setIncrement(Point2I(2));
	scrollViewerCanvas->setObjectOffset(-Point2I::ONE);
	scrollViewerCanvas->eventPaint().add(bindEvent(AsciiArtistWindow::onScrollViewerPaint));
	scrollViewerCanvas->eventMouseButton().add(bindEvent(AsciiArtistWindow::onCanvasScrollViewerMouseButton));
	splitContainer->setLeftContent(scrollViewerCanvas);
	//children().add(scrollViewerCanvas);

	canvas = makeControl(Canvas);
	canvas->eventImageChanged().add(bindEvent(AsciiArtistWindow::onCanvasImageChanged));
	canvas->eventMessageSent().add(bindEvent(AsciiArtistWindow::onCanvasMessageSent));
	canvas->eventToolChanged().add(bindEvent(AsciiArtistWindow::onCanvasToolChanged));
	canvas->eventUndoAvailabilityChanged().add(bindEvent(AsciiArtistWindow::onCanvasUndoAvailabilityChanged));
	canvas->eventPixelChanged().add(bindEvent(AsciiArtistWindow::onCanvasPixelChanged));
	canvas->eventStatusBarDimensions().add(bindEvent(AsciiArtistWindow::onCanvasStatusBarDimensions));
	canvas->eventAnimationCanceled().add(bindEvent(AsciiArtistWindow::onCanvasAnimationCanceled));
	canvas->eventFrameChanged().add(bindEvent(AsciiArtistWindow::onCanvasFrameChanged));
	canvas->eventFrameListChanged().add(bindEvent(AsciiArtistWindow::onCanvasFrameListChanged));
	canvas->eventFrameAdded().add(bindEvent(AsciiArtistWindow::onCanvasFrameAdded));
	canvas->eventFrameRemoved().add(bindEvent(AsciiArtistWindow::onCanvasFrameRemoved));
	canvas->eventFrameRenamed().add(bindEvent(AsciiArtistWindow::onCanvasFrameRenamed));
	canvas->eventFrameMoved().add(bindEvent(AsciiArtistWindow::onCanvasFrameMoved));
	scrollViewerCanvas->setContent(canvas);

	frameList = makeControl(FrameListBox);
	frameList->setSize(Point2I(SIZE_AUTO));
	frameList->setMargin(Thickness(0, 2, 0, 2));
	frameList->setHorizontalAlignment(HorizontalAlignments::Stretch);
	frameList->setVerticalAlignment(VerticalAlignments::Stretch);
	frameList->setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	frameList->eventSelectionChanged().add(bindEvent(AsciiArtistWindow::onFrameListSelectionChanged));
	frameList->eventPreviewChanged().add(bindEvent(AsciiArtistWindow::onFrameListPreviewChanged));
	frameList->eventSelectionDoubleClicked().add(bindEvent(AsciiArtistWindow::onFrameListSelectionDoubleClicked));
	frameList->items().add(FrameInfo("frame 1", 800));
	frameList->setSelectedIndex(0);
	panelFrames->children().add(frameList);

	toolList = makeControl(ListBox);
	toolList->setSize(Point2I(9, 10));
	toolList->setMargin(Thickness(12, 3, 0, 0));
	toolList->setHorizontalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	toolList->setVerticalScrollBarVisibility(ScrollBarVisibilities::Hidden);
	toolList->eventSelectionChanged().add(bindEvent(AsciiArtistWindow::onToolListSelectionChanged));
	children().add(toolList);

	statusBar = makeControl(StatusBar);
	statusBar->setSize(Point2I(SIZE_AUTO, 1));
	statusBar->setMargin(Thickness(1, 0, 1, 1));
	statusBar->setHorizontalAlignment(HorizontalAlignments::Stretch);
	statusBar->setVerticalAlignment(VerticalAlignments::Bottom);
	children().add(statusBar);

	toolList->items().add("Pen");
	toolList->items().add("Line");
	toolList->items().add("Square");
	toolList->items().add("Circle");
	toolList->items().add("Flood");
	toolList->items().add("Border");
	toolList->items().add("Text");
	toolList->items().add("Select");
	toolList->items().add("Eyedrpr");
	toolList->setNoSelectionAllowed(false);

	#pragma region Commands

	commands().add(Command("Close", Hotkey(Keys::W, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onCloseCommand)
	));

	commands().add(Command("New", Hotkey(Keys::N, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onNewCommand)
	));
	commands().add(Command("Open", Hotkey(Keys::O, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onOpenCommand)
	));
	commands().add(Command("Save", Hotkey(Keys::S, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onSaveCommand)
	));
	commands().add(Command("Save As", Hotkey(Keys::S, Modifiers::Ctrl | Modifiers::Shift), false,
		bindEvent(AsciiArtistWindow::onSaveAsCommand)
	));

	commands().add(Command("Copy", Hotkey(Keys::C, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onCopyCommand)
	));
	commands().add(Command("Cut", Hotkey(Keys::X, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onCutCommand)
	));
	commands().add(Command("Delete", Hotkey(Keys::Delete), false,
		bindEvent(AsciiArtistWindow::onDeleteCommand)
	));
	commands().add(Command("Paste", Hotkey(Keys::V, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onPasteCommand)
	));
	commands().add(Command("Select All", Hotkey(Keys::A, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onSelectAllCommand)
	));

	commands().add(Command("Undo", Hotkey(Keys::Z, Modifiers::Ctrl, true), false,
		bindEvent(AsciiArtistWindow::onUndoCommand)
	));
	commands().add(Command("Redo", Hotkey(Keys::Y, Modifiers::Ctrl, true), false,
		bindEvent(AsciiArtistWindow::onRedoCommand)
	));

	commands().add(Command("Resize", Hotkey(Keys::R, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onResizeCommand)
	));
	commands().add(Command("Palette Editor", Hotkey(Keys::P, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onPaletteEditorCommand)
	));
	commands().add(Command("Font Chooser", Hotkey(Keys::F, Modifiers::Ctrl), false,
		bindEvent(AsciiArtistWindow::onFontChooserCommand)
	));

	commands().add(Command("Maximize", Hotkey(Keys::X, Modifiers::Alt), false,
		bindEvent(AsciiArtistWindow::onMaximizeCommand)
	));
	commands().add(Command("Restore", Hotkey(Keys::C, Modifiers::Alt), false,
		bindEvent(AsciiArtistWindow::onRestoreCommand)
	));

	commands().add(Command("Pen", Hotkey(Settings::getHotkey("Pen")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Pen")
	));
	commands().add(Command("Line", Hotkey(Settings::getHotkey("Line")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Line")
	));
	commands().add(Command("Square", Hotkey(Settings::getHotkey("Square")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Square")
	));
	commands().add(Command("Circle", Hotkey(Settings::getHotkey("Circle")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Circle")
	));
	commands().add(Command("Flood", Hotkey(Settings::getHotkey("Flood")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Flood")
	));
	commands().add(Command("Border", Hotkey(Settings::getHotkey("Border")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Border")
	));
	commands().add(Command("Text", Hotkey(Settings::getHotkey("Text")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Text")
	));
	commands().add(Command("Select", Hotkey(Settings::getHotkey("Select")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Select")
	));
	commands().add(Command("Eyedrpr", Hotkey(Settings::getHotkey("Eyedrpr")), false,
		bindEvent(AsciiArtistWindow::onToolCommand, "Eyedrpr")
	));

	for (int i = 0; i < 10; i++) {
		int realNumber = (i + 1) % 10;
		std::string realNumberStr = std::to_string(realNumber);
		commands().add(Command(realNumberStr + " Assign Bookmark", Hotkey((Keys)((int)Keys::D0 + realNumber), Modifiers::Shift), false,
			bindEvent(AsciiArtistWindow::onAssignBookmarkerPixelCommand, i)
		));
		commands().add(Command(realNumberStr + " Select Bookmark", Hotkey((Keys)((int)Keys::D0 + realNumber)), false,
			bindEvent(AsciiArtistWindow::onSelectBookmarkerPixelCommand, i)
		));
	}

	#pragma endregion

	// Setup Settings

	// Default Tool:
	int toolIndex = Settings::getDefaultToolIndex();
	toolList->setSelectedIndex(toolIndex);
	canvas->setToolIndex(toolIndex);
	auto section = Settings::get()->get("ToolOptions");
	for (int i = 0; i < section->count(); i++) {
		auto property = section->getProperties()[i];
		if (canvas->hasOption(property->getKey())) {
			canvas->setOption(property->getKey(), property->getBool());
		}
	}
	onCanvasToolChanged(nullptr, SelectionChangedEventArgs(toolIndex, -1));

	// Default Pixels:
	Pixel primary = Settings::getDrawingPixel(false);
	Pixel secondary = Settings::getDrawingPixel(true);
	canvas->setDrawingPixel(primary, false);
	canvas->setDrawingPixel(secondary, true);
	colorPalette->setDrawingPixel(primary, false);
	colorPalette->setDrawingPixel(secondary, true);
	pixelDisplay->setDrawingPixel(primary, false);
	pixelDisplay->setDrawingPixel(secondary, true);
	characterGrid->setDrawingPixel(primary, false);
	characterGrid->setDrawingPixel(secondary, true);
	pixelBookmarker->setDrawingPixel(primary, false);
	pixelBookmarker->setDrawingPixel(secondary, true);

	std::string defaultDirectory = Settings::get()->get("FileDialog")->getString("DefaultDirectory");
	Path::setCurrentDirectory(Path::expandEnvironmentPath(defaultDirectory));

	bool showCursor = Settings::get()->get("Editor")->getBool("ShowCursor", "true", true);
	toggleButtonCursor->setChecked(showCursor);
	canvas->setCursorVisible(showCursor);
}
void AsciiArtistWindow::onLoad(ControlSPtr sender, EventArgs& e) {
	splitContainer->setRightWidth(15);

	bool loaded = false;
	if ((int)commandLine.size() > 1) {
		AsciiAnimationSPtr image = std::make_shared<AsciiAnimation>(Point2I(1, 1), ImageFormats::Animation | ImageFormats::Attributes, Pixel(), 1);
		if (image->loadFile(commandLine[1])) {
			filePath = commandLine[1];
			untitled = false;
			changed = false;
			updateTitle();
			AsciiImageHeader header = AsciiImageHeader();
			FileInputStream in = FileInputStream();
			unsigned short fileVersion = AsciiImageInfo::FILE_VERSION;
			bool smallFile = false;
			in.open(filePath);
			if (in.isOpen()) {
				if (AsciiImageInfo::loadHeader(&in, header, false)) {
					fileVersion = header.version;
					smallFile = (header.signature == AsciiImageInfo::SMALL_FILE_SIGNATURE);
				}
				in.close();
			}
			loaded = true;
			canvas->loadImage(image, fileVersion, smallFile);
		}
		else {
			//console().playSystemSound(SystemSounds::Error);
			statusBar->sendMessage("Error opening image", MESSAGE_COLOR_ERROR);
			MessageBox::show(getWindow(),
				"Error opening image! This program may not have access to open files in this directory.",
				"Open Image",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
	if (!loaded) {
		auto image = Settings::createDefaultImage();
		canvas->loadImage(image);
	}
	
	/*auto g = canvas->getImage()->createGraphics();
	for (int i = 0; i < 256; i++) {
		g->setPixel(Point2I(1 + i % 16, 1 + i / 16), Pixel((unsigned char)i, 0x0F));
	}
	g->drawRectangle(Rectangle2I(18, 18), Pixel(' ', toColor(DARK_GRAY, BLACK)));*/
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Command Line

fixed_array<std::string> AsciiArtistWindow::getCommandLineArguments() const {
	return commandLine;
}
std::string AsciiArtistWindow::getCommandLineArgument(int index) const {
	return commandLine[index];
}
int AsciiArtistWindow::getNumCommandLineArguments() const {
	return (int)commandLine.size();
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void AsciiArtistWindow::updateTitle() {
	console().setTitle(
		(untitled ? "Untitled" : Path::getFileName(filePath)) +
		(changed ? "*" : "") + " - Ascii Artist" +
		(!importFilePath.empty() ? " (Asciified: " + Path::getFileName(importFilePath) + ")" : "")
	);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void AsciiArtistWindow::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	// Draw borders
	// Window border
	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', frameColor), true);

	// Toolbar bottom border
	g->drawBorder(Rectangle2I(0, 2, getActualSize().x, 1), Pixel(' ', frameColor), true);

	// Status bar top border
	g->drawBorder(Rectangle2I(0, getActualSize().y - 3, getActualSize().x, 1), Pixel(' ', frameColor), true);

	// Left panel borders
	// Far right border
	g->drawBorder(Rectangle2I(21, 2, 1, getActualSize().y - 4), Pixel(' ', frameColor), true);
	// Border to split colors from tool list
	g->drawBorder(Rectangle2I(11, 2, 1, 12), Pixel(' ', frameColor), true);
	g->drawBorder(Rectangle2I(0, 5, 12, 1), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(5, 5, 1, 3), Pixel(' ', frameColor), false);

	g->drawBorder(Rectangle2I(0, 7, 12, 1), Pixel(' ', frameColor), true);
	g->drawBorder(Rectangle2I(0, 13, 22, 1), Pixel(' ', frameColor), true);
	g->drawBorder(Rectangle2I(0, getActualSize().y - 6, 22, 1), Pixel(' ', frameColor), true);

	// Right panel borders
	g->drawBorder(Rectangle2I(panelFrames->getGlobalPosition().x, 2, 1, panelFrames->getActualSize().y), Pixel(' ', frameColor), true);
	g->drawBorder(Rectangle2I(panelFrames->getGlobalPosition().x, 4, panelFrames->getActualSize().x, 1), Pixel(' ', frameColor), true);
	g->drawBorder(Rectangle2I(panelFrames->getGlobalPosition().x, getActualSize().y - 5, panelFrames->getActualSize().x, 1), Pixel(' ', frameColor), true);
	//g->drawBorder(Rectangle2I(getActualSize().x - 12, 0, 1, 3), Pixel(' ', frameColor), false);

	int count;

	// Frame button spliters
	count = stackPanelFrameToolbar->children().count();
	for (int i = 0; i < count; i++) {
		if (panelFrames->getGlobalPosition().x + 3 + 3 * i < getActualSize().x - 1)
			g->drawBorder(Rectangle2I(panelFrames->getGlobalPosition().x + 3 + 3 * i, 2, 1, 3), Pixel(' ', frameColor), (i + 1 == count));
	}

	// Animation button splitters
	count = stackPanelAnimationToolbar->children().count();
	for (int i = 0; i < count; i++) {
		if (panelFrames->getGlobalPosition().x + 3 + 3 * i < getActualSize().x - 1)
			g->drawBorder(Rectangle2I(panelFrames->getGlobalPosition().x + 3 + 3 * i, getActualSize().y - 5, 1, 3), Pixel(' ', frameColor), (i + 1 == count));
	}

	// Settings button splitters
	count = stackPanelSettingsToolbar->children().count();
	for (int i = 0; i < count; i++) {
		g->drawBorder(Rectangle2I(getActualSize().x - 5 - i * count, 0, 1, 3), Pixel(' ', frameColor), (i + 1 == count));
	}

	// Toolbar button spliters
	count = stackPanelToolbar->children().count();
	for (int i = 0; i < count; i++) {
		g->drawBorder(Rectangle2I((i + 1) * 4, 0, 1, 3), Pixel(' ', frameColor), (i + 1 == count || i == 12));
	}

	// Status Bar borders
	g->drawBorder(Rectangle2I(22, getActualSize().y - 3, 1, 3), Pixel(' ', frameColor), false);
	g->drawBorder(Rectangle2I(43, getActualSize().y - 3, 1, 3), Pixel(' ', frameColor), false);
}
void AsciiArtistWindow::onScrollViewerPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	g->resetOptions();
	g->setConnectGlobalBorders(true);
	g->setExtraTranslation(scrollViewerCanvas->getPadding().topLeft());
	Point2I frameSize = canvas->getResizingSize();
	Point2I viewportSize = scrollViewerCanvas->getViewportSize();
	if (frameSize.x < scrollViewerCanvas->getViewportSize().x) {
		g->drawBorder(Rectangle2I(frameSize.x, -1, 1, GMath::min(frameSize.y, viewportSize.y) + 2), Pixel(' ', 0x08), false);
	}
	if (frameSize.y < scrollViewerCanvas->getViewportSize().y) {
		g->drawBorder(Rectangle2I(-1, frameSize.y, GMath::min(frameSize.x, viewportSize.x) + 2, 1), Pixel(' ', 0x08), false);
	}
	g->resetOptions();
}

// Options
void AsciiArtistWindow::onCursorClicked(ControlSPtr sender, MouseEventArgs& e) {
	canvas->setCursorVisible(toggleButtonCursor->isChecked());
}
void AsciiArtistWindow::onChClicked(ControlSPtr sender, MouseEventArgs& e) {
	canvas->setCharacterEnabled(toggleButtonCh->isChecked());
}
void AsciiArtistWindow::onFgClicked(ControlSPtr sender, MouseEventArgs& e) {
	canvas->setForegroundEnabled(toggleButtonFg->isChecked());
}
void AsciiArtistWindow::onBgClicked(ControlSPtr sender, MouseEventArgs& e) {
	canvas->setBackgroundEnabled(toggleButtonBg->isChecked());
}
void AsciiArtistWindow::onOptionClicked(ControlSPtr sender, MouseEventArgs& e) {
	auto toggleButton = std::static_pointer_cast<ToggleButton>(sender);
	std::string option = toggleButton->getText();
	canvas->setOption(option, toggleButton->isChecked());
}

// Palette
void AsciiArtistWindow::onColorPalettePixelChanged(ControlSPtr sender, PixelChangedEventArgs& e) {
	canvas->setDrawingPixel(e.pixel, e.secondary);
	pixelDisplay->setDrawingPixel(e.pixel, e.secondary);
	characterGrid->setDrawingPixel(e.pixel, e.secondary);
	pixelBookmarker->setDrawingPixel(e.pixel, e.secondary);
}
void AsciiArtistWindow::onPixelDisplayPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e) {
	canvas->setDrawingPixel(e.pixel, e.secondary);
	colorPalette->setDrawingPixel(e.pixel, e.secondary);
	characterGrid->setDrawingPixel(e.pixel, e.secondary);
	pixelBookmarker->setDrawingPixel(e.pixel, e.secondary);
}
void AsciiArtistWindow::onCharacterGridPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e) {
	if (!canvas->checkToolForCharacterUse(e.pixel)) {
		canvas->setDrawingPixel(e.pixel, e.secondary);
		colorPalette->setDrawingPixel(e.pixel, e.secondary);
		pixelDisplay->setDrawingPixel(e.pixel, e.secondary);
		pixelBookmarker->setDrawingPixel(e.pixel, e.secondary);
	}
	else {
		characterGrid->setDrawingPixel(canvas->getDrawingPixel(e.secondary), e.secondary);
	}
}
void AsciiArtistWindow::onPixelBookmarkerPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e) {
	canvas->setDrawingPixel(e.pixel, e.secondary);
	colorPalette->setDrawingPixel(e.pixel, e.secondary);
	pixelDisplay->setDrawingPixel(e.pixel, e.secondary);
	characterGrid->setDrawingPixel(e.pixel, e.secondary);
}

// Frame List
void AsciiArtistWindow::onFrameListSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	canvas->setFrameIndex(e.newIndex);
}
void AsciiArtistWindow::onFrameListPreviewChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	canvas->setPreviewIndex(e.newIndex);
}
void AsciiArtistWindow::onFrameListSelectionDoubleClicked(ControlSPtr sender, EventArgs& e) {
	onRenameFrameCommand(sender, e);
}

// Tool List
void AsciiArtistWindow::onToolListSelectionChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	canvas->setToolIndex(e.newIndex);
}

// Canvas
void AsciiArtistWindow::onCanvasScrollViewerMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (e.isButtonPressed(MouseButtons::Left) || e.isButtonPressed(MouseButtons::Right)) {
		if (e.newPosition - scrollViewerCanvas->getPadding().topLeft() < scrollViewerCanvas->getViewportSize()) {
			if (!canvas->isMouseInside()) {
				canvas->getCurrentTool()->finalize();
			}
		}
	}
}
void AsciiArtistWindow::onCanvasImageChanged(ControlSPtr sender, EventArgs& e) {
	changed = true;
	updateTitle();
}
void AsciiArtistWindow::onCanvasMessageSent(ControlSPtr sender, MessageEventArgs& e) {
	// Start a message in the status bar
	statusBar->sendMessage(e.message, e.color);
}
void AsciiArtistWindow::onCanvasToolChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	toolList->setSelectedIndex(e.newIndex);
	stackPanelOptions->children().clear();
	auto options = canvas->getCurrentTool()->getUsedOptions();
	for (int i = 0; i < (int)options.size(); i++) {
		auto option = makeControl(ToggleButton);
		option->setText(options[i]);
		option->setChecked(canvas->getOption(options[i]));
		option->setMargin(Thickness(1, 0, 0, 0));
		option->eventClick().add(bindEvent(AsciiArtistWindow::onOptionClicked));
		stackPanelOptions->children().add(option);
	}
}
void AsciiArtistWindow::onCanvasUndoAvailabilityChanged(ControlSPtr sender, UndoAvailabilityEventArgs& e) {
	// Changed undo buttons enabledness
	buttonUndo->setEnabled(e.undoAvailable);
	buttonRedo->setEnabled(e.redoAvailable);
}
void AsciiArtistWindow::onCanvasPixelChanged(ControlSPtr sender, PixelChangedEventArgs& e) {
	colorPalette->setDrawingPixel(e.pixel, e.secondary);
	pixelDisplay->setDrawingPixel(e.pixel, e.secondary);
	characterGrid->setDrawingPixel(e.pixel, e.secondary);
}
void AsciiArtistWindow::onCanvasStatusBarDimensions(ControlSPtr sender, StatusBarDimensionsEventArgs& e) {
	statusBar->setLabelPoint(e.point);
	statusBar->setLabelSize(e.size);
}
void AsciiArtistWindow::onCanvasAnimationCanceled(ControlSPtr sender, EventArgs& e) {
	buttonPlay->setImage(*ascPlayOff);
	buttonStop->setImage(*ascStopOn);
}

// Canvas Frame Changes
void AsciiArtistWindow::onCanvasFrameChanged(ControlSPtr sender, SelectionChangedEventArgs& e) {
	frameList->setSelectedIndex(e.newIndex);
}
void AsciiArtistWindow::onCanvasFrameListChanged(ControlSPtr sender, FrameListChangedEventArgs& e) {
	frameList->items().clear();
	for (int i = 0; i < (int)e.names.size(); i++) {
		frameList->items().add(e.names[i]);
	}
}
void AsciiArtistWindow::onCanvasFrameAdded(ControlSPtr sender, FrameModifiedEventArgs& e) {
	frameList->items().insert(e.index, e.frame);
}
void AsciiArtistWindow::onCanvasFrameRemoved(ControlSPtr sender, FrameModifiedEventArgs& e) {
	frameList->items().removeAt(e.index);
}
void AsciiArtistWindow::onCanvasFrameRenamed(ControlSPtr sender, FrameModifiedEventArgs& e) {
	frameList->items()[e.index] = e.frame;
}
void AsciiArtistWindow::onCanvasFrameMoved(ControlSPtr sender, FrameMovedEventArgs& e) {
	frameList->items().move(e.index, e.newIndex);
}

#pragma endregion
//======= COMMAND HANDLERS =======
#pragma region Command Handlers

void AsciiArtistWindow::onCloseCommand(ControlSPtr sender, EventArgs& e) {
	DialogBoxResults result = DialogBoxResults::No;
	if (changed) {
		result = MessageBox::show(getWindow(),
			"Unsaved changes have been made to this image. Do you want to save before closing?",
			"Closing",
			DialogBoxButtons::YesNoCancel,
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
		if (result == DialogBoxResults::Yes) {
			std::string newFilePath = filePath;
			if (untitled) {
				auto fileDialog= makeDialog(SaveAsciiImageFileDialog);
				fileDialog->setFileName("Untitled.asc");
				if (showDialog(fileDialog))
					newFilePath = fileDialog->getFileName();
				else
					result = DialogBoxResults::Cancel;
			}
			if (result != DialogBoxResults::Cancel) {
				bool result2;
				LoadingDialog::show(getWindow(), "Saving Image...", [this, &newFilePath, &result2](LoadingDialogSPtr sender) {
					result2 = canvas->getImage()->saveFile(newFilePath, canvas->getFileVersion(), canvas->isSmallFile());
				});
				if (!result2) {
					statusBar->sendMessage("Error saving image", MESSAGE_COLOR_ERROR);
					MessageBox::show(getWindow(),
						"Error saving image! This program may not have access to modify files in this directory.",
						"Closing",
						DialogBoxIcons::Error,
						SystemSounds::Error
					);
					result = DialogBoxResults::Cancel;
				}
			}
		}
	}
	if (result != DialogBoxResults::Cancel) {
		close();
	}
}

void AsciiArtistWindow::onNewCommand(ControlSPtr sender, EventArgs& e) {
	DialogBoxResults result = DialogBoxResults::Yes;
	if (changed) {
		result = MessageBox::show(getWindow(),
			"Unsaved changes have been made to this image. Do you still want to create a new image?",
			"New Image",
			DialogBoxButtons::YesNo,
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
	}
	if (result == DialogBoxResults::Yes) {
		auto image = Settings::createDefaultImage();
		result = DialogBoxResults::No;

		if (Clipboard::containsFormat(AsciiImageInfo::CLIPBOARD_FORMAT)) {
			ClipboardInputStream in = ClipboardInputStream();
			in.open();
			if (in.isOpen()) {
				in.getData(AsciiImageInfo::CLIPBOARD_FORMAT);
				AsciiImageHeader header;
				if (AsciiImageInfo::loadHeader(&in, header, false)) {
					if (header.size != image->size()) {
						result = MessageBox::show(getWindow(),
							"The image in the clipboard does not match the frame size. Do you want to resize the frame to fit it?",
							"Match Frame Size",
							DialogBoxButtons::YesNo,
							DialogBoxIcons::Question,
							SystemSounds::Information,
							MessageBox::DEFAULT_MAX_WIDTH,
							MessageBox::DEFAULT_MIN_WIDTH,
							3
						);
						if (result == DialogBoxResults::Yes) {
							image->resize(header.size);
						}
					}
				}
				in.close();
			}
		}

		canvas->loadImage(image);
		importFilePath = "";
		untitled = true;
		changed = false;
		updateTitle();
		statusBar->sendMessage("New image created", MESSAGE_COLOR_NORMAL);
	}
}
void AsciiArtistWindow::onOpenCommand(ControlSPtr sender, EventArgs& e) {
	DialogBoxResults result = DialogBoxResults::Yes;
	if (changed) {
		result = MessageBox::show(getWindow(),
			"Unsaved changes have been made to this image. Do you still want to open?",
			"Open Image",
			DialogBoxButtons::YesNo,
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
	}
	if (result == DialogBoxResults::Yes) {
		auto fileDialog = makeDialog(OpenAsciiImageFileDialog);
		fileDialog->setFileName("");
		if (showDialog(fileDialog)) {
			AsciiAnimationSPtr image = std::make_shared<AsciiAnimation>(Point2I(1, 1), ImageFormats::Animation | ImageFormats::Attributes, Pixel(), 1);
			bool result;
			LoadingDialog::show(getWindow(), "Loading Image...", [this, &result, fileDialog, image](LoadingDialogSPtr sender) {
				result = image->loadFile(fileDialog->getFileName());
			});
			if (result) {
				filePath = fileDialog->getFileName();
				importFilePath = "";
				untitled = false;
				changed = false;
				updateTitle();
				AsciiImageHeader header = AsciiImageHeader();
				FileInputStream in = FileInputStream();
				unsigned short fileVersion = AsciiImageInfo::FILE_VERSION;
				bool smallFile = false;
				in.open(filePath);
				if (in.isOpen()) {
					if (AsciiImageInfo::loadHeader(&in, header, false)) {
						fileVersion = header.version;
						smallFile = (header.signature == AsciiImageInfo::SMALL_FILE_SIGNATURE);
					}
					in.close();
				}
				canvas->loadImage(image, fileVersion, smallFile);
			}
			else {
				statusBar->sendMessage("Error opening image", MESSAGE_COLOR_ERROR);
				MessageBox::show(getWindow(),
					"Error opening image! The image may have been invalid or this program may not have access to open files in this directory.",
					"Open Image",
					DialogBoxIcons::Error,
					SystemSounds::Error
				);
			}
		}
	}
}
void AsciiArtistWindow::onSaveCommand(ControlSPtr sender, EventArgs& e) {
	if (untitled) {
		onSaveAsCommand(sender, e);
	}
	else {

		bool result;
		LoadingDialog::show(getWindow(), "Saving Image...", [this, &result](LoadingDialogSPtr sender) {
			result = canvas->getImage()->saveFile(filePath, canvas->getFileVersion(), canvas->isSmallFile());
		});
		if (result) {
			changed = false;
			updateTitle();
		}
		else {
			statusBar->sendMessage("Error saving image", MESSAGE_COLOR_ERROR);
			MessageBox::show(getWindow(),
				"Error saving image! This program may not have access to modify files in this directory.",
				"Save Image",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
}
void AsciiArtistWindow::onSaveAsCommand(ControlSPtr sender, EventArgs& e) {
	auto fileDialog = makeDialog(SaveAsciiImageFileDialog);
	if (untitled)
		fileDialog->setFileName("Untitled.asc");
	else
		fileDialog->setFileName(Path::getFileName(filePath));
	if (showDialog(fileDialog)) {

		bool result;
		LoadingDialog::show(getWindow(), "Saving Image...", [this, &result, fileDialog](LoadingDialogSPtr sender) {
			result = canvas->getImage()->saveFile(fileDialog->getFileName(), canvas->getFileVersion(), canvas->isSmallFile());
		});
		if (result) {
			filePath = fileDialog->getFileName();
			untitled = false;
			changed = false;
			updateTitle();
		}
		else {
			// Error Message
			statusBar->sendMessage("Error saving image", MESSAGE_COLOR_ERROR);
			MessageBox::show(getWindow(),
				"Error saving image! This program may not have access to modify files in this directory.",
				"Save Image",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
}

void AsciiArtistWindow::onCopyCommand(ControlSPtr sender, EventArgs& e) {
	canvas->copySelection();
}
void AsciiArtistWindow::onCutCommand(ControlSPtr sender, EventArgs& e) {
	canvas->cutSelection();
}
void AsciiArtistWindow::onDeleteCommand(ControlSPtr sender, EventArgs& e) {
	canvas->deleteSelection();
}
void AsciiArtistWindow::onPasteCommand(ControlSPtr sender, EventArgs& e) {
	canvas->pasteSelection();
}
void AsciiArtistWindow::onSelectAllCommand(ControlSPtr sender, EventArgs& e) {
	canvas->selectAll();
}

void AsciiArtistWindow::onUndoCommand(ControlSPtr sender, EventArgs& e) {
	canvas->undo();
}
void AsciiArtistWindow::onRedoCommand(ControlSPtr sender, EventArgs& e) {
	canvas->redo();
}

void AsciiArtistWindow::onResizeCommand(ControlSPtr sender, EventArgs& e) {
	Point2I newSize = canvas->getCurrentFrame()->size();
	bool canResizeSingle = getFlag(canvas->getImage()->getFormat(), ImageFormats::Multisize);
	bool resizeAll = !canResizeSingle;
	if (ResizeDialogBox::show(getWindow(), newSize, resizeAll, canResizeSingle)) {
		canvas->resize(newSize, resizeAll);
	}
}
void AsciiArtistWindow::onTranslateCommand(ControlSPtr sender, EventArgs& e) {
	Point2I distance = Point2I::ZERO;
	bool translateAll = false;
	if (TranslateDialogBox::show(getWindow(), distance, translateAll)) {
		canvas->translate(distance, translateAll);
	}
}
void AsciiArtistWindow::onImportImageCommand(ControlSPtr sender, EventArgs& e) {
	if (changed) {
		DialogBoxResults result = MessageBox::show(getWindow(),
			"Unsaved changes have been made to this image. Do you still want to asciify an image?",
			"Asciify Image",
			DialogBoxButtons::YesNo,
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
		if (result == DialogBoxResults::No)
			return;
	}
	auto fileDialog = makeDialog(OpenImageFileDialog);
	bool useLastFile = keyboard().isModifiersDown(Modifiers::Shift) && !importFilePath.empty() && importIsImage;
	bool useClipboard = keyboard().isModifiersDown(Modifiers::Ctrl);
	WICBitmapSourceSPtr source = nullptr;
	if (useClipboard) {
		source = WIC::readSourceFromClipboard();
		if (source == nullptr)
			useClipboard = false;
	}
	if (useLastFile || useClipboard || showDialog(fileDialog)) {
		if (useClipboard)
			importImageSettings.imageSize = WIC::readImageSizeFromSource(source);
		else
			importImageSettings.imageSize = WIC::readImageSizeFromFile(fileDialog->getFileName());
		importImageSettings.fontSize = console().getFontSize();

		if (ImportImageDialog::show(getWindow(), importImageSettings)) {
			std::string newImportFilePath = (useLastFile ? importFilePath : fileDialog->getFileName());

			if (source == nullptr)
				source = WIC::readSourceFromFile(newImportFilePath);
			if (importImageSettings.scale != 1.0f)
				source = WIC::scaleSource(source, Point2F(importImageSettings.scale), BitmapScaleModes::Fant);
			auto bitmap = WIC::readBitmapFromSource(source);

			AsciiAnimationSPtr image = nullptr;
			bool canceled = false;
			ProgressDialog::show(getWindow(), "Asciify Image", true, true, [this, &image, &newImportFilePath, &bitmap, &canceled](ProgressDialogSPtr sender) {
				image = Asciifier::asciifyImagePrecision(bitmap, importImageSettings.split, importImageSettings.metric, canceled, sender);
			});
			if (image != nullptr) {
				filePath = "";
				changed = false;
				untitled = true;
				importFilePath = newImportFilePath;
				importIsImage = true;
				updateTitle();
				canvas->loadImage(image);
				statusBar->sendMessage("Image asciified", MESSAGE_COLOR_NORMAL);
			}
			else if (!canceled) {
				statusBar->sendMessage("Error reading image", MESSAGE_COLOR_ERROR);
				MessageBox::show(getWindow(),
					"Error reading image! This program may not have access to open files in this directory.",
					"Asciify Image",
					DialogBoxIcons::Error,
					SystemSounds::Error
				);
			}
		}
	}
}
void AsciiArtistWindow::onImportFileCommand(ControlSPtr sender, EventArgs& e) {
	if (changed) {
		DialogBoxResults result = MessageBox::show(getWindow(),
			"Unsaved changes have been made to this image. Do you still want to asciify a file?",
			"Asciify File",
			DialogBoxButtons::YesNo,
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
		if (result == DialogBoxResults::No)
			return;
	}
	auto fileDialog = makeDialog(OpenCustomFileDialog);
	bool useLastFile = keyboard().isModifiersDown(Modifiers::Shift) && !importFilePath.empty() && !importIsImage;
	if (useLastFile || showDialog(fileDialog)) {
		if (ImportFileDialog::show(getWindow(), importFileSettings)) {
			std::string newImportFilePath = (useLastFile ? importFilePath : fileDialog->getFileName());

			FileInputStream in = FileInputStream();
			in.open(fileDialog->getFileName());
			if (in.isOpen()) {
				AsciiAnimationSPtr image = Settings::createDefaultImage();
				image->resize(importFileSettings.size);
				auto g = image->createGraphics();
				g->setModOptions(PixelOptions::Replace | PixelOptions::CopySpecial);

				Point2I p;
				for (p.y = 0; p.y < importFileSettings.size.y && !in.hasError(); p.y++) {
					for (p.x = 0; p.x < importFileSettings.size.x && !in.hasError(); p.x++) {
						if (importFileSettings.readCharacters) {
							g->setChar(p, in.readUChar());
							in.setPointer(importFileSettings.bytesAfterCharacter, true);
						}
						else {
							g->setChar(p, canvas->getDrawingPixel(false).character);
						}
						if (importFileSettings.readColors) {
							g->setColor(p, in.readUChar());
							in.setPointer(importFileSettings.bytesAfterColor, true);
						}
						else {
							g->setColor(p, canvas->getDrawingPixel(false).color);
						}
					}
				}
				in.close();

				filePath = "";
				changed = false;
				untitled = true;
				importFilePath = newImportFilePath;
				importIsImage = false;
				updateTitle();
				canvas->loadImage(image);
				statusBar->sendMessage("File asciified", MESSAGE_COLOR_NORMAL);
			}
			else {
				statusBar->sendMessage("Error reading file", MESSAGE_COLOR_ERROR);
				MessageBox::show(getWindow(),
					"Error reading file! This program may not have access to open files in this directory.",
					"Asciify File",
					DialogBoxIcons::Error,
					SystemSounds::Error
				);
			}
		}
	}
}
void AsciiArtistWindow::onExportImageCommand(ControlSPtr sender, EventArgs& e) {
	auto fileDialog = makeDialog(SaveCustomFileDialog);
	fileDialog->setDefaultExt(".png");
	fileDialog->addFilter(".png");
	fileDialog->setFileName("*.png");
	if (showDialog(fileDialog)) {
		redraw();
		bool result;
		LoadingDialog::show(getWindow(), "Exporting Image...", [this, fileDialog, &result](LoadingDialogSPtr sender) {
			result = Asciifier::writeBitmap(fileDialog->getFileName(), *canvas->getCurrentFrame());
		});
		if (!result) {
			statusBar->sendMessage("Error saving image", MESSAGE_COLOR_ERROR);
			MessageBox::show(getWindow(),
				"Error saving image! This program may not have access to modify files in this directory.",
				"Export Image",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
}
void AsciiArtistWindow::onExportGifCommand(ControlSPtr sender, EventArgs& e) {
	auto fileDialog = makeDialog(SaveCustomFileDialog);
	fileDialog->setDefaultExt(".gif");
	fileDialog->addFilter(".gif");
	fileDialog->setFileName("*.gif");
	if (showDialog(fileDialog)) {
		redraw();
		bool result;
		LoadingDialog::show(getWindow(), "Exporting Gif...", [this, fileDialog, &result](LoadingDialogSPtr sender) {
			result = Asciifier::writeGif(fileDialog->getFileName(), *canvas->getImage());
		});
		if (!result) {
			statusBar->sendMessage("Error saving gif", MESSAGE_COLOR_ERROR);
			MessageBox::show(getWindow(),
				"Error saving gif! This program may not have access to modify files in this directory.",
				"Export Gif",
				DialogBoxIcons::Error,
				SystemSounds::Error
			);
		}
	}
}
void AsciiArtistWindow::onExportVideoCommand(ControlSPtr sender, EventArgs& e) {
	if (!Path::fileExists(Path::combine(Path::getExecutableDirectory(), "ffmpeg.exe"))) {
		statusBar->sendMessage("Missing ffmpeg.exe", MESSAGE_COLOR_ERROR);
		MessageBox::show(getWindow(),
			"ffmpeg.exe is not present in the Ascii Artist program directory. Please download it in order to export videos.",
			"Missing File",
			DialogBoxIcons::Error,
			SystemSounds::Error
		);
	}
	else {
		auto fileDialog = makeDialog(SaveCustomFileDialog);
		fileDialog->setDefaultExt(".mp4");
		fileDialog->addFilter(".mp4");
		fileDialog->setFileName("*.mp4");
		if (showDialog(fileDialog)) {
			bool canceled = false;
			bool error = false;
			ProgressDialog::show(getWindow(), "Export Video", true, true, [this, fileDialog, &error, &canceled](ProgressDialogSPtr sender) {
				error = !Asciifier::writeVideo(fileDialog->getFileName(), *canvas->getImage(), canceled, sender);
			});
			if (error && !canceled) {
				statusBar->sendMessage("Error saving video", MESSAGE_COLOR_ERROR);
				MessageBox::show(getWindow(),
					"Error saving video! This program may not have access to modify files in this directory.",
					"Export Video",
					DialogBoxIcons::Error,
					SystemSounds::Error
				);
			}
		}
	}
}

// Frame Buttons
void AsciiArtistWindow::onNewFrameCommand(ControlSPtr sender, EventArgs& e) {
	canvas->newFrame();
}
void AsciiArtistWindow::onDuplicateFrameCommand(ControlSPtr sender, EventArgs& e) {
	canvas->duplicateFrame();
}
void AsciiArtistWindow::onDeleteFrameCommand(ControlSPtr sender, EventArgs& e) {
	canvas->deleteFrame();
}
void AsciiArtistWindow::onMoveFrameDownCommand(ControlSPtr sender, EventArgs& e) {
	canvas->moveFrameDown();
}
void AsciiArtistWindow::onMoveFrameUpCommand(ControlSPtr sender, EventArgs& e) {
	canvas->moveFrameUp();
}
void AsciiArtistWindow::onRenameFrameCommand(ControlSPtr sender, EventArgs& e) {
	if (canvas->getFileVersion() >= 3 && !canvas->isSmallFile()) {
		std::string newName = canvas->getCurrentFrame()->getName();
		auto result = TextEditBox::show(getWindow(), newName, "Rename Frame", DialogBoxButtons::OKCancel, 30);
		if (result == DialogBoxResults::OK) {
			canvas->renameFrame(newName);
		}
	}
	else {
		MessageBox::show(getWindow(),
			"Frame naming is not supported with this file version.",
			"Not Supported",
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
	}
}
void AsciiArtistWindow::onFrameSpeedCommand(ControlSPtr sender, EventArgs& e) {
	if (canvas->getFileVersion() >= 3 && !canvas->isSmallFile()) {
		unsigned int speed = canvas->getCurrentFrame()->getFrameSpeed();
		bool setAll = false;
		if (FrameSpeedDialogBox::show(getWindow(), speed, setAll)) {
			canvas->setFrameSpeed(speed, setAll);
		}
	}
	else {
		MessageBox::show(getWindow(),
			"Frame speed is not supported with this file version.",
			"Not Supported",
			DialogBoxIcons::Warning,
			SystemSounds::Warning
		);
	}
}

void AsciiArtistWindow::onPlayCommand(ControlSPtr sender, EventArgs& e) {
	if (canvas->isAnimating()) {
		canvas->stopAnimation();
		buttonPlay->setImage(*ascPlayOff);
		buttonStop->setImage(*ascStopOn);
	}
	else {
		canvas->animate();
		buttonPlay->setImage(*ascPlayOn);
		buttonStop->setImage(*ascStopOff);
	}
}

void AsciiArtistWindow::onStopCommand(ControlSPtr sender, EventArgs& e) {
	if (canvas->isAnimating()) {
		canvas->stopAnimation();
		buttonPlay->setImage(*ascPlayOff);
		buttonStop->setImage(*ascStopOn);
	}
}
void AsciiArtistWindow::onShowSpeedCommand(ControlSPtr sender, EventArgs& e) {
	if (frameList->getFrameSpeedMode()) {
		frameList->setFrameSpeedMode(false);
		buttonShowSpeed->setImage(*ascShowSpeedOff);
	}
	else {
		frameList->setFrameSpeedMode(true);
		buttonShowSpeed->setImage(*ascShowSpeedOn);
	}
}


void AsciiArtistWindow::onHelpCommand(ControlSPtr sender, EventArgs& e) {
	statusBar->sendMessage("Not implemented yet", MESSAGE_COLOR_WARNING);
}
void AsciiArtistWindow::onAboutCommand(ControlSPtr sender, EventArgs& e) {
	AboutDialog::show(getWindow());
}
void AsciiArtistWindow::onImageSettingsCommand(ControlSPtr sender, EventArgs& e) {
	ImageSettings settings = ImageSettings(
		canvas->getImage()->getFormat(),
		canvas->getImage()->getBackground(),
		canvas->getFileVersion(),
		canvas->isSmallFile()
	);
	if (ImageSettingsDialog::show(getWindow(), settings)) {
		canvas->setImageSettings(settings);
	}
}
void AsciiArtistWindow::onProgramSettingsCommand(ControlSPtr sender, EventArgs& e) {
	statusBar->sendMessage("Not implemented yet", MESSAGE_COLOR_WARNING);
}
void AsciiArtistWindow::onPaletteEditorCommand(ControlSPtr sender, EventArgs& e) {
	ConsolePalette palette = console().getPalette();
	if (PaletteEditor::show(getWindow(), palette)) {
		console().setPalette(palette);
	}
}
void AsciiArtistWindow::onFontChooserCommand(ControlSPtr sender, EventArgs& e) {
	ConsoleFont fontResult = console().getFont();
	if (FontChooser::show(getWindow(), fontResult)) {
		console().setFont(fontResult);
	}
}

void AsciiArtistWindow::onMaximizeCommand(ControlSPtr sender, EventArgs& e) {
	console().setWindowPosition(console().getCurrentMonitor().point);
	setSize(console().getConsoleMaxSize() - Point2I(2, 1));
}
void AsciiArtistWindow::onRestoreCommand(ControlSPtr sender, EventArgs& e) {
	setSize(DEFAULT_WINDOW_SIZE);
	console().centerWindow();
}

void AsciiArtistWindow::onToolCommand(ControlSPtr sender, EventArgs& e, const std::string& toolName) {
	if (canvas->getCurrentTool()->getToolName() != "Text" || !canvas->getCurrentTool()->isDrawing()) {
		canvas->setToolByName(toolName);
	}
}

void AsciiArtistWindow::onAssignBookmarkerPixelCommand(ControlSPtr sender, EventArgs& e, int index) {
	if (canvas->getCurrentTool()->getToolName() != "Text" || !canvas->getCurrentTool()->isDrawing()) {
		pixelBookmarker->assignBookmarkedPixel(index);
	}
}
void AsciiArtistWindow::onSelectBookmarkerPixelCommand(ControlSPtr sender, EventArgs& e, int index) {
	if (canvas->getCurrentTool()->getToolName() != "Text" || !canvas->getCurrentTool()->isDrawing()) {
		pixelBookmarker->selectBookmarkedPixel(index);
	}
}

#pragma endregion
//=================================================================|
