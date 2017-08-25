/*=================================================================|
|* File:				Canvas.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CANVAS_H
#define CANVAS_H

#include <memory>
#include <vector>
#include <PowerConsole/Declarations.h>
#include <AsciiArtist/Toolbox/AsciiArtistEventArgs.h>
#include <AsciiArtist/Editing/AsciiArtistEnums.h>
#include <AsciiArtist/Editing/ImageSettings.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Geometry/Rectangle.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace AsciiArtist::Editing;

DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(AsciiArtist::Editing, Tool);
DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(AsciiArtist::Editing, UndoAction);
DECLARE_NAMESPACE_AND_CLASS(AsciiArtist::Editing::UndoActions, FrameUndoAction);
DECLARE_NAMESPACE_AND_CLASS(AsciiArtist::Editing::UndoActions, FrameSpeedUndoAction);
DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(AsciiArtist::Editing::Tools, TextTool);
DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(AsciiArtist::Editing::Tools, SelectTool);
DECLARE_NAMESPACE_CLASS_AND_SMART_PTRS(AsciiArtist::Editing::Tools, EyedropperTool);

using namespace AsciiArtist::Editing::Tools;
using namespace AsciiArtist::Editing::UndoActions;

namespace AsciiArtist::Toolbox::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum MessageColors : unsigned char {
	MESSAGE_COLOR_NORMAL = toColor(BLACK, GRAY),
	MESSAGE_COLOR_WARNING = toColor(BLACK, YELLOW),
	MESSAGE_COLOR_ERROR = toColor(BLACK, RED)
};


class Canvas : public Control {
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The maximum amount of undos to be stored. */
	static const int MAX_UNDO_HISTORY = 50;

	static const int NUM_TOOLS = 9;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	// Image
	/* The buffer for overlaying the drawing actions onto the image. */
	AsciiAnimationSPtr drawingBuffer;
	/* The currently loaded image to draw to. */
	AsciiAnimationSPtr image;
	/* The file version of the ascii image. */
	unsigned short fileVersion;
	/* True if the ascii image is a small file. */
	bool smallFile;
	/* The index of the frame currently being drawn to. */
	int frameIndex;
	/* The index of the frame currently being previewed. */
	int previewIndex;
	/* True if the image is currently animating. */
	bool animating;
	/* The timer for the animation. */
	int animationTimer;
	/* True if the cursor is visible. */
	bool cursorVisible;

	// Drawing
	/* The primary drawing pixel. */
	Pixel primaryPixel;
	/* The secondary drawing pixel. */
	Pixel secondaryPixel;
	/* The drawing mod attributes. */
	PixelAttributes modAttributes;
	/* The drawing mod options. */
	PixelOptions modOptions;
	/* The boolean options for tools. */
	std::map<std::string, bool> options;

	// Tools
	/* The current tool being drawn with. */
	ToolSPtr currentTool;
	/* The list of tools to choose from. */
	ToolSPtr tools[NUM_TOOLS];
	/* Quick access for the text tool. */
	TextToolSPtr textTool;
	/* Quick access for the selection tool. */
	SelectToolSPtr selectTool;
	/* Quick access for the eyedropper tool. */
	EyedropperToolSPtr eyedropperTool;
	/* The current status bar point. */
	Point2I toolPoint;
	/* The current status bar size. */
	Point2I toolSize;

	// Resizing
	/* Non-zero if resizing. (1,0) = Resizing width, (0,1) = Resizing height, (1,1) = Resizing both. */
	Point2I resizing;
	/* The temporary image size while resizing. */
	Point2I tempSize;

	// History
	/* The history of undo actions. */
	std::vector<UndoActionSPtr> history;
	/* The current index in the history. */
	int historyIndex;

	// Events
	/* Called when changes have been made to the image. */
	Event<EventArgs> _eventImageChanged;
	/* Called when a message is ready to be sent to the status bar. */
	Event<MessageEventArgs> _eventMessageSent;
	/* Called when the current tool has been changed. */
	Event<SelectionChangedEventArgs> _eventToolChanged;
	/* Called when undo availability has changed. */
	Event<UndoAvailabilityEventArgs> _eventUndoAvailabilityChanged;
	/* Called when the color has been changed with the color palette. */
	Event<PixelChangedEventArgs> _eventPixelChanged;
	/* Called when the status bar tool dimensions have changed. */
	Event<StatusBarDimensionsEventArgs> _eventStatusBarDimensions;
	/* Called when the canvas has stopped the animation. */
	Event<EventArgs> _eventAnimationCanceled;

	/* Called when a current frame index has changed. */
	Event<SelectionChangedEventArgs> _eventFrameChanged;
	/* Called when all of the frames in the list have been changed. */
	Event<FrameListChangedEventArgs> _eventFrameListChanged;
	/* Called when a frame has been added to the image. */
	Event<FrameModifiedEventArgs> _eventFrameAdded;
	/* Called when a frame has been removed from the image. */
	Event<FrameModifiedEventArgs> _eventFrameRemoved;
	/* Called when a frame has been renamed. */
	Event<FrameModifiedEventArgs> _eventFrameRenamed;
	/* Called when a frame has been moved. */
	Event<FrameMovedEventArgs> _eventFrameMoved;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the canvas editor. */
	Canvas(AsciiAnimationSPtr loadedImage = nullptr);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the drawing buffer. */
	AsciiImageFrame* getDrawingBuffer(int index = 0);
	/* Gets the currently loaded image to draw to. */
	AsciiAnimationSPtr getImage();
	/* Gets the frame currently being drawn to. */
	AsciiImageFrame* getCurrentFrame();
	/* Gets the frame currently being displayed. */
	AsciiImageFrame* getPreviewFrame();
	/* Gets the current frame index. */
	int getFrameIndex() const;
	/* Sets the current frame index. */
	void setFrameIndex(int index);
	/* Gets the current preview frame index. */
	int getPreviewIndex() const;
	/* Sets the current preview frame index. */
	void setPreviewIndex(int index);
	/* Gets the file version of the ascii image. */
	unsigned short getFileVersion() const;
	/* Gets if the ascii image is a small file. */
	bool isSmallFile() const;
	/* Sets the file version and small file setting of the ascii image. */
	void setFileVersion(unsigned short fileVersion, bool smallFile);

	/* Gets the specified drawing pixel. */
	Pixel getDrawingPixel(bool secondary) const;
	/* Sets the specified drawing pixel. */
	void setDrawingPixel(Pixel pixel, bool secondary);
	/* Gets the drawing mod attributes. */
	PixelAttributes getModAttributes() const;
	/* Sets the drawing mod attributes. */
	//void setModAttributes(PixelAttributes modAttributes);
	/* Gets the drawing mod options. */
	PixelOptions getModOptions() const;
	/* Sets the drawing mod options. */
	//void setModOptions(PixelOptions modOptions);
	/* Gets the graphics for the drawing buffer. */
	GraphicsSPtr getDrawingBufferGraphics(int index = 0);
	/* Gets the graphics for the current frame. */
	GraphicsSPtr getCurrentFrameGraphics();
	/* Gets if the cursor is visible. */
	bool isCursorVisible() const;
	/* Sets if the cursor is visible. */
	void setCursorVisible(bool visible);
	/* Gets if the character is enabled. */
	bool isCharacterEnabled() const;
	/* Sets if the character is enabled. */
	void setCharacterEnabled(bool enabled);
	/* Gets if the foreground is enabled. */
	bool isForegroundEnabled() const;
	/* Sets if the foreground is enabled. */
	void setForegroundEnabled(bool enabled);
	/* Gets if the foreground is enabled. */
	bool isBackgroundEnabled() const;
	/* Sets if the foreground is enabled. */
	void setBackgroundEnabled(bool enabled);

	/* Gets the temporary resizing image size. */
	Point2I getResizingSize() const;

	/* Gets the  current tool being drawn with. */
	ToolSPtr getCurrentTool();
	/* Gets the current tool index. */
	int getToolIndex() const;
	/* Sets the current tool index. */
	void setToolIndex(int tool);
	/* Sets the current tool by name. */
	void setToolByName(const std::string& name);
private:
	/* Sets the  current tool being drawn with. */
	void setCurrentTool(ToolSPtr tool);
public:
	/* Gets if the canvas is animating. */
	bool isAnimating() const;

	#pragma endregion
	//=========== OPTIONS ============
	#pragma region Options

	/* Returns true if the option exists. */
	bool hasOption(const std::string& name) const;
	/* Gets the options with the specified name. */
	bool getOption(const std::string& name) const;
	/* Sets the option with the specified name. */
	void setOption(const std::string& name, bool enabled);
	/* Registers the option with the specified default setting. */
	void addOption(const std::string& name, bool enabled);

	#pragma endregion
	//=========== EDITING ============
	#pragma region Editing
	
	// Tool
	/* Checks if the current tool wants to use the selected character. */
	/* Tool will perform actions with the character in this function. */
	bool checkToolForCharacterUse(Pixel pixel);

	// Image
	/* Loads the new image into the editor. */
	void loadImage(AsciiAnimationSPtr image, unsigned short fileVersion = 0, bool smallFile = false);
	/* Resizes the current frame or every image. */
	void resize(Point2I size, bool resizeAll);
	/* Translates the current frame or every frame. */
	void translate(Point2I distance, bool translateAll);
	/* Sets the animation speed of the frame. */
	void setFrameSpeed(unsigned int frameSpeed, bool setAll);
	/* Sets the settings of the image. */
	void setImageSettings(ImageSettings settings);

	// History
	/* Adds the specified undo action to the history table. */
	void addUndo(UndoActionSPtr action);
	/* Undos the last action. */
	void undo();
	/* Redos the next action. */
	void redo();

	// Selection
	/* Copies the current selection if there is any. */
	void copySelection();
	/* Cuts the current selection if there is any. */
	void cutSelection();
	/* Deletes the current selection if there is any. */
	void deleteSelection();
	/* Pastes the current ascii image in the clipboard. */
	void pasteSelection();
	/* Selects the entire frame. */
	void selectAll();

	// Frames
	/* Adds a new empty frame. */
	void newFrame();
	/* Duplicates the current frame. */
	void duplicateFrame();
	/* Deletes the current frame. */
	void deleteFrame();
	/* Moves the current frame down in the list. */
	void moveFrameDown();
	/* Moves the current frame up in the list. */
	void moveFrameUp();
	/* Renames the current frame. */
	void renameFrame(const std::string& name);

	// Messages
	/* Sends a message to the status bar. */
	void sendMessage(const std::string& message, unsigned char color = MESSAGE_COLOR_NORMAL);

	// Image changes
	/* Lets the canvas know that the image was changed. */
	void signalImageChanged();

	// Animation
	/* Animates the image or stops animating. */
	void animate();
	/* Cancels the animation. */
	void stopAnimation();

	#pragma endregion
	//========= UNDO ACTIONS =========
	#pragma region Undo Actions
private:
	// These are needed to signal changes to the frames without creating undo actions.
	/* Inserts a frame for undo actions. */
	void insertFrameUndoAction(int index);
	/* Inserts a frame for undo actions. */
	void insertFrameUndoAction(int index, const AsciiImageFrame& frame);
	/* Removes a frame for undo actions. */
	void removeFrameUndoAction(int index);
	/* Moves a frame for undo actions. */
	void moveFrameUndoAction(int index, int newIndex);
	/* Renames the frame for undo actions. */
	void renameFrameUndoAction(int index, const std::string& name);
	/* Sets the speed for the frame for undo actions. */
	void setFrameSpeedUndoAction(int index, unsigned int speed);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating
public:
	/* Gets the auto size of the control when not stretching. */
	Point2I autoSizeBehavior() const final;

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	/* Called when changes have been made to the image. */
	Event<EventArgs>& eventImageChanged();
	/* Called when a message is ready to be sent to the status bar. */
	Event<MessageEventArgs>& eventMessageSent();
	/* Called when the current tool has been changed. */
	Event<SelectionChangedEventArgs>& eventToolChanged();
	/* Called when undo availability has changed. */
	Event<UndoAvailabilityEventArgs>& eventUndoAvailabilityChanged();
	/* Called when the color has been changed with the color palette. */
	Event<PixelChangedEventArgs>& eventPixelChanged();
	/* Called when the status bar tool dimensions have changed. */
	Event<StatusBarDimensionsEventArgs>& eventStatusBarDimensions();
	/* Called when the canvas has stopped the animation. */
	Event<EventArgs>& eventAnimationCanceled();

	// Frames
	/* Called when a current frame index has changed. */
	Event<SelectionChangedEventArgs>& eventFrameChanged();
	/* Called when all of the frames in the list have been changed. */
	Event<FrameListChangedEventArgs>& eventFrameListChanged();
	/* Called when a frame has been added to the image. */
	Event<FrameModifiedEventArgs>& eventFrameAdded();
	/* Called when a frame has been removed from the image. */
	Event<FrameModifiedEventArgs>& eventFrameRemoved();
	/* Called when a frame has been renamed. */
	Event<FrameModifiedEventArgs>& eventFrameRenamed();
	/* Called when a frame has been moved. */
	Event<FrameMovedEventArgs>& eventFrameMoved();

	#pragma endregion
	//======= COMMAND HANDLERS =======
	#pragma region Command Handlers


	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e);
	void onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e);
	void onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e);
	void onKeyTypedGlobal(ControlSPtr sender, KeyTypedEventArgs& e);

	#pragma endregion
	//=========== FRIENDS ============
	#pragma region Friends

	friend class FrameUndoAction;
	friend class FrameSpeedUndoAction;

	#pragma endregion
};

DECLARE_SMART_PTRS(Canvas);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
