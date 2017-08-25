/*=================================================================|
|* File:				Canvas.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Canvas.h"
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Controls/Toolbox/MessageBox.h>
#include <AsciiArtist/Settings.h>
#include <AsciiArtist/Editing/Tool.h>
#include <AsciiArtist/Editing/Tools/PenTool.h>
#include <AsciiArtist/Editing/Tools/LineTool.h>
#include <AsciiArtist/Editing/Tools/SquareTool.h>
#include <AsciiArtist/Editing/Tools/CircleTool.h>
#include <AsciiArtist/Editing/Tools/FloodTool.h>
#include <AsciiArtist/Editing/Tools/BorderTool.h>
#include <AsciiArtist/Editing/Tools/TextTool.h>
#include <AsciiArtist/Editing/Tools/SelectTool.h>
#include <AsciiArtist/Editing/Tools/EyedropperTool.h>
#include <AsciiArtist/Editing/UndoAction.h>
#include <AsciiArtist/Editing/UndoActions/FrameSpeedUndoAction.h>
#include <AsciiArtist/Editing/UndoActions/AnimationUndoAction.h>
#include <AsciiArtist/Editing/UndoActions/DrawUndoAction.h>
//#include <AsciiArtist/Editing/UndoActions/FormatUndoAction.h>
#include <AsciiArtist/Editing/UndoActions/FrameUndoAction.h>
#include <AsciiArtist/Editing/UndoActions/SettingsUndoAction.h>
#include <PowerConsole/IO/Clipboard.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/PowerConsoleSettings.h>

#define makeTool(toolType) std::make_shared<toolType>(std::static_pointer_cast<Canvas>(shared_from_this()));

using namespace PowerConsole::IO;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiArtist::Editing::UndoActions;
using namespace AsciiArtist::Editing::Tools;
using namespace AsciiArtist::Toolbox::Controls;
using namespace AsciiArtist;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Canvas::Canvas(AsciiAnimationSPtr loadedImage)
 :	// Image
	image(Settings::createDefaultImage()),
	drawingBuffer(nullptr),
	//image(std::make_shared<AsciiAnimation>(Point2I(64, 32), ImageFormats::Attributes | ImageFormats::Animation, Pixel(' ', 0x00, PixelAttributes::Fill))),
	//drawingBuffer(std::make_shared<AsciiImage>(Point2I(64, 32), ImageFormats::Attributes, Pixel(' ', 0x00, PixelAttributes::None))),
	fileVersion(Settings::getFileVersion()),
	smallFile(Settings::getSmallFile()),
	frameIndex(0),
	previewIndex(-1),
	animating(false),
	cursorVisible(true),

	// Drawing
	primaryPixel(Pixel('A', 0x07)),
	secondaryPixel(Pixel(' ', 0x00)),
	modAttributes(PixelAttributes::All),
	modOptions(PixelOptions::CopySpecial | PixelOptions::Replace),

	// Tools
	currentTool(nullptr),
	toolPoint(-Point2I::ONE),
	toolSize(-Point2I::ONE),

	// Resizing
	resizing(Point2I::ZERO),

	// History
	history(),
	historyIndex(0),

	// Events
	_eventImageChanged(EventDirections::FirstInFirstCalled),
	_eventMessageSent(EventDirections::FirstInFirstCalled),
	_eventToolChanged(EventDirections::FirstInFirstCalled),
	_eventUndoAvailabilityChanged(EventDirections::FirstInFirstCalled),
	_eventPixelChanged(EventDirections::FirstInFirstCalled),
	_eventStatusBarDimensions(EventDirections::FirstInFirstCalled),
	_eventAnimationCanceled(EventDirections::FirstInFirstCalled),

	_eventFrameChanged(EventDirections::FirstInFirstCalled),
	_eventFrameListChanged(EventDirections::FirstInFirstCalled),
	_eventFrameAdded(EventDirections::FirstInFirstCalled), 
	_eventFrameRemoved(EventDirections::FirstInFirstCalled), 
	_eventFrameRenamed(EventDirections::FirstInFirstCalled), 
	_eventFrameMoved(EventDirections::FirstInFirstCalled) {

	drawingBuffer = std::make_shared<AsciiAnimation>(image->size(), ImageFormats::Attributes | ImageFormats::Animation, Pixel(' ', 0x00, PixelAttributes::None));

	eventInitialize().add(bindEvent(Canvas::onInitialize));
	eventPaint().add(bindEvent(Canvas::onPaint));
	eventTick().add(bindEvent(Canvas::onTick));
	eventMouseButton().add(bindEvent(Canvas::onMouseButton));
	eventMouseMovedGlobal().add(bindEvent(Canvas::onMouseMovedGlobal));
	eventKeyGlobal().add(bindEvent(Canvas::onKeyGlobal));
	eventKeyTypedGlobal().add(bindEvent(Canvas::onKeyTypedGlobal));

	setSizeSafe(Point2I(SIZE_AUTO));
}
void Canvas::onInitialize(ControlSPtr sender, EventArgs& e) {
	addOption("Overwrte", false);

	textTool = makeTool(TextTool);
	selectTool = makeTool(SelectTool);
	eyedropperTool = makeTool(EyedropperTool);

	tools[0] = makeTool(PenTool);
	tools[1] = makeTool(LineTool);
	tools[2] = makeTool(SquareTool);
	tools[3] = makeTool(CircleTool);
	tools[4] = makeTool(FloodTool);
	tools[5] = makeTool(BorderTool);
	tools[6] = textTool;
	tools[7] = selectTool;
	tools[8] = eyedropperTool;
	currentTool = tools[0];
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

AsciiImageFrame* Canvas::getDrawingBuffer(int index) {
	return &drawingBuffer->at(index);
}
AsciiAnimationSPtr Canvas::getImage() {
	return image;
}
AsciiImageFrame* Canvas::getCurrentFrame() {
	return &image->at(frameIndex);
}
AsciiImageFrame* Canvas::getPreviewFrame() {
	return &image->at(previewIndex != -1 ? previewIndex : frameIndex);
}
int Canvas::getFrameIndex() const {
	return frameIndex;
}
void Canvas::setFrameIndex(int index) {
	if (index != frameIndex) {
		currentTool->finalize();
		stopAnimation();
		int oldIndex = frameIndex;
		frameIndex = index;
		updateAutoSize();
		_eventFrameChanged(shared_from_this(), SelectionChangedEventArgs(index, oldIndex));
	}
}
int Canvas::getPreviewIndex() const {
	return previewIndex;
}
void Canvas::setPreviewIndex(int index) {
	if (index != previewIndex) {
		int oldIndex = frameIndex;
		previewIndex = index;
		updateAutoSize();
	}
}
unsigned short Canvas::getFileVersion() const {
	return fileVersion;
}
bool Canvas::isSmallFile() const {
	return smallFile;
}
void Canvas::setFileVersion(unsigned short fileVersion, bool smallFile) {
	this->fileVersion = fileVersion;
	this->smallFile = smallFile;
}

Pixel Canvas::getDrawingPixel(bool secondary) const {
	return (secondary ? secondaryPixel : primaryPixel);
}
void Canvas::setDrawingPixel(Pixel pixel, bool secondary) {
	currentTool->finalize();
	(secondary ? secondaryPixel : primaryPixel) = pixel;
	_eventPixelChanged(shared_from_this(), PixelChangedEventArgs(pixel, secondary));
}
PixelAttributes Canvas::getModAttributes() const {
	return modAttributes;
}
/*void Canvas::setModAttributes(PixelAttributes modAttributes) {
	this->modAttributes = modAttributes;
}*/
PixelOptions Canvas::getModOptions() const {
	if (getOption("Overwrte")) {
		return PixelOptions::Replace | PixelOptions::CopySpecial;
	}
	return PixelOptions::None;
	//return modOptions;
}
/*void Canvas::setModOptions(PixelOptions modOptions) {
	this->modOptions = modOptions;
}*/
GraphicsSPtr Canvas::getDrawingBufferGraphics(int index) {
	auto g = drawingBuffer->createGraphics();
	g->setCurrentFrame(index);
	return g;
}
GraphicsSPtr Canvas::getCurrentFrameGraphics() {
	auto g = image->createGraphics();
	g->setCurrentFrame(frameIndex);
	return g;
}
bool Canvas::isCursorVisible() const {
	return cursorVisible;
}
void Canvas::setCursorVisible(bool visible) {
	cursorVisible = visible;
}
bool Canvas::isCharacterEnabled() const {
	return getFlag(modAttributes, PixelAttributes::Char);
}
void Canvas::setCharacterEnabled(bool enabled) {
	currentTool->finalize();
	setFlag(modAttributes, PixelAttributes::Char, enabled);
}
bool Canvas::isForegroundEnabled() const {
	return getFlag(modAttributes, PixelAttributes::ForegroundAll);
}
void Canvas::setForegroundEnabled(bool enabled) {
	currentTool->finalize();
	setFlag(modAttributes, PixelAttributes::ForegroundAll, enabled);
}
bool Canvas::isBackgroundEnabled() const {
	return getFlag(modAttributes, PixelAttributes::BackgroundAll);
}
void Canvas::setBackgroundEnabled(bool enabled) {
	currentTool->finalize();
	setFlag(modAttributes, PixelAttributes::BackgroundAll, enabled);
}

Point2I Canvas::getResizingSize() const {
	return (resizing != Point2I::ZERO ? tempSize : image->at(frameIndex).size());
}

ToolSPtr Canvas::getCurrentTool() {
	return currentTool;
}
int Canvas::getToolIndex() const {
	for (int i = 0; i < NUM_TOOLS; i++) {
		if (tools[i] == currentTool) {
			return i;
		}
	}
	return -1;
}
void Canvas::setToolIndex(int tool) {
	int oldIndex = getToolIndex();
	if (tool != oldIndex) {
		eyedropperTool->setPreviousToolIndex(oldIndex);
		currentTool->finalize();
		currentTool = tools[tool];
		_eventToolChanged(shared_from_this(), SelectionChangedEventArgs(tool, oldIndex));
	}
}
void Canvas::setToolByName(const std::string& name) {
	if (currentTool->getToolName() != name) {
		int oldIndex = getToolIndex();
		int newIndex = -1;
		for (int i = 0; i < NUM_TOOLS; i++) {
			if (tools[i]->getToolName() == name) {
				newIndex = i;
				break;
			}
		}
		if (newIndex == -1)
			throw std::invalid_argument("No tool with that name exists");
		eyedropperTool->setPreviousToolIndex(oldIndex);
		currentTool->finalize();
		currentTool = tools[newIndex];
		_eventToolChanged(shared_from_this(), SelectionChangedEventArgs(newIndex, oldIndex));
	}
}
void Canvas::setCurrentTool(ToolSPtr tool) {
	if (tool != currentTool) {
		int oldIndex = getToolIndex();
		eyedropperTool->setPreviousToolIndex(oldIndex);
		currentTool->finalize();
		currentTool = tool;
		_eventToolChanged(shared_from_this(), SelectionChangedEventArgs(getToolIndex(), oldIndex));
	}
}
bool Canvas::isAnimating() const {
	return animating;
}

#pragma endregion
//=========== OPTIONS ============
#pragma region Options

bool Canvas::hasOption(const std::string& name) const {
	return options.find(name) != options.end();
}
bool Canvas::getOption(const std::string& name) const {
	return options.at(name);
}
void Canvas::setOption(const std::string& name, bool enabled) {
	currentTool->finalize();
	options.at(name) = enabled;
}
void Canvas::addOption(const std::string& name, bool enabled) {
	auto it = options.find(name);
	if (it == options.end())
		options.insert(std::pair<std::string, bool>(name, enabled));
}

#pragma endregion
//=========== EDITING ============
#pragma region Editing

// Image
void Canvas::loadImage(AsciiAnimationSPtr image, unsigned short fileVersion, bool smallFile) {
	currentTool->cancel();
	stopAnimation();
	frameIndex = 0;
	this->image = image;
	if (fileVersion == 0) {
		this->fileVersion = Settings::getFileVersion();
		this->smallFile = Settings::getSmallFile();
	}
	drawingBuffer->resize(image->size());
	updateAutoSize();

	// Clear undo history
	history.clear();
	historyIndex = 0;
	fixed_array<FrameInfo> frames = fixed_array<FrameInfo>(image->frameCount());
	for (int i = 0; i < (int)image->frameCount(); i++) {
		frames[i] = FrameInfo(image->getName(i), (unsigned int)image->getFrameSpeed(i));
	}
	_eventFrameListChanged(shared_from_this(), FrameListChangedEventArgs(std::move(frames)));
	_eventUndoAvailabilityChanged(shared_from_this(), UndoAvailabilityEventArgs(false, false));
}
void Canvas::resize(Point2I size, bool resizeAll) {
	currentTool->finalize();
	stopAnimation();
	if (resizeAll) {
		if (size != image->size() || getFlag(image->getFormat(), ImageFormats::Multisize)) {
			auto undoAction = std::make_shared<AnimationUndoAction>("Resize image");
			undoAction->setOriginalImage(*image);
			image->resize(size, -1);
			drawingBuffer->resize(image->size());
			updateAutoSize();
			undoAction->setNewImage(*image);
			addUndo(undoAction);
			sendMessage("Image resized");
			drawingBuffer->resize(image->size());
		}
	}
	else if (size != image->at(frameIndex).size()) {
		auto undoAction = std::make_shared<DrawUndoAction>("Resize frame", frameIndex);
		undoAction->setOriginalImage(image->at(frameIndex));
		image->resize(size, frameIndex);
		drawingBuffer->resize(image->size());
		updateAutoSize();
		undoAction->setNewImage(image->at(frameIndex));
		addUndo(undoAction);
		sendMessage("Frame resized");
		drawingBuffer->resize(image->size());
	}
}
void Canvas::translate(Point2I distance, bool translateAll) {
	currentTool->finalize();
	stopAnimation();
	if (distance != Point2I::ZERO) {
		if (translateAll) {
			auto undoAction = std::make_shared<AnimationUndoAction>("Translate image");
			undoAction->setOriginalImage(*image);
			image->translate(distance, -1);
			undoAction->setNewImage(*image);
			addUndo(undoAction);
			sendMessage("Image translated");
		}
		else {
			auto undoAction = std::make_shared<DrawUndoAction>("Translate frame", frameIndex);
			undoAction->setOriginalImage(image->at(frameIndex));
			image->translate(distance, frameIndex);
			undoAction->setNewImage(image->at(frameIndex));
			addUndo(undoAction);
			sendMessage("Frame translated");
		}
	}
}
void Canvas::setFrameSpeed(unsigned int frameSpeed, bool setAll) {
	currentTool->finalize();
	stopAnimation();
	if (setAll) {
		auto undoAction = std::make_shared<FrameSpeedUndoAction>(frameSpeed);
		fixed_array<unsigned int> originalFrameSpeeds = fixed_array<unsigned int>(image->frameCount());
		for (int i = 0; i < (int)image->frameCount(); i++) {
			originalFrameSpeeds[i] = image->getFrameSpeed(i);
			image->setFrameSpeed(frameSpeed, i);
			_eventFrameRenamed(shared_from_this(), FrameModifiedEventArgs(
				FrameInfo(
					image->at(i).getName(),
					image->at(i).getFrameSpeed()),
				i
			));
		}
		undoAction->setOriginalFrameSpeeds(std::move(originalFrameSpeeds));
		addUndo(undoAction);
		sendMessage("Frame speeds changed");
	}
	else {
		auto undoAction = std::make_shared<FrameSpeedUndoAction>(frameIndex, image->getFrameSpeed(frameIndex), frameSpeed);
		image->setFrameSpeed(frameSpeed, frameIndex);
		_eventFrameRenamed(shared_from_this(), FrameModifiedEventArgs(
			FrameInfo(
				image->at(frameIndex).getName(),
				image->at(frameIndex).getFrameSpeed()),
			frameIndex
		));
		addUndo(undoAction);
		sendMessage("Frame speed changed");
	}
}
void Canvas::setImageSettings(ImageSettings settings) {
	currentTool->finalize();
	stopAnimation();
	ImageSettings originalSettings = ImageSettings(image->getFormat(), image->getBackground(), fileVersion, smallFile);
	auto undoAction = std::make_shared<SettingsUndoAction>(originalSettings, settings);

	// Only set the original image if it's needed. We'll do a basic guess to see if it will be modified
	if ((getFlag(originalSettings.format, ImageFormats::Animation) && !getFlag(settings.format, ImageFormats::Animation) && image->frameCount() > 1) ||
		(getFlag(originalSettings.format, ImageFormats::Multisize) && !getFlag(settings.format, ImageFormats::Multisize) && image->frameCount() > 1) ||
		(getFlag(originalSettings.format, ImageFormats::Attributes) && !getFlag(settings.format, ImageFormats::Attributes))) {
		undoAction->setOriginalImage(*image);
	}
	image->setFormat(settings.format);
	image->setBackground(settings.background);
	fileVersion = settings.fileVersion;
	smallFile = settings.smallFile;
	fixed_array<FrameInfo> frames = fixed_array<FrameInfo>(image->frameCount());
	for (int i = 0; i < (int)image->frameCount(); i++) {
		frames[i] = FrameInfo(image->getName(i), (unsigned int)image->getFrameSpeed(i));
	}
	_eventFrameListChanged(shared_from_this(), FrameListChangedEventArgs(std::move(frames)));
	if (!getFlag(settings.format, ImageFormats::Animation))
		setFrameIndex(0);
	updateAutoSize();



	addUndo(undoAction);
	sendMessage("Settings changed");
}

// History
void Canvas::addUndo(UndoActionSPtr action) {
	if (Settings::getMaxUndos() > 0) {
		if (historyIndex < (int)history.size()) {
			// Remove up future redos
			history.erase(history.begin() + historyIndex, history.end());
		}
		else if ((int)history.size() >= Settings::getMaxUndos()) {
			// Limit the amount of memory used to store undo history
			history.erase(history.begin(), history.begin() + (int)history.size() - Settings::getMaxUndos() + 1);
			historyIndex = Settings::getMaxUndos() - 1;
		}
		history.push_back(action);
		historyIndex++;
		signalImageChanged();
		_eventUndoAvailabilityChanged(shared_from_this(), UndoAvailabilityEventArgs(true, false));
	}
}
void Canvas::undo() {
	if (currentTool->isDrawing()) {
		// Undo should cancel what we're currently doing instead.
		// It's supposed to be a helpful way to cancel the current action.
		currentTool->cancel();
	}
	else if (Settings::getMaxUndos() > 0) {
		stopAnimation();
		if (historyIndex > 0) {
			historyIndex--;

			history[historyIndex]->undo(std::static_pointer_cast<Canvas>(shared_from_this()));
			drawingBuffer->resize(image->size());
			updateAutoSize();

			sendMessage(history[historyIndex]->getActionName() + " action undone");
			signalImageChanged();
		}
		else {
			sendMessage("No more actions to undo", MESSAGE_COLOR_WARNING);
		}
		_eventUndoAvailabilityChanged(shared_from_this(), UndoAvailabilityEventArgs(
			(historyIndex > 0),
			(historyIndex < (int)history.size())
		));
	}
	else {
		sendMessage("Undo history is disabled", MESSAGE_COLOR_WARNING);
	}
}
void Canvas::redo() {
	currentTool->cancel();
	stopAnimation();

	if (Settings::getMaxUndos() > 0) {
		if (historyIndex < (int)history.size()) {
			history[historyIndex]->redo(std::static_pointer_cast<Canvas>(shared_from_this()));
			drawingBuffer->resize(image->size());
			updateAutoSize();

			sendMessage(history[historyIndex]->getActionName() + " action redone");
			historyIndex++;
			signalImageChanged();
		}
		else {
			sendMessage("No more actions to redo", MESSAGE_COLOR_WARNING);
		}
		_eventUndoAvailabilityChanged(shared_from_this(), UndoAvailabilityEventArgs(
			(historyIndex > 0),
			(historyIndex < (int)history.size())
		));
	}
	else {
		sendMessage("Undo history is disabled", MESSAGE_COLOR_WARNING);
	}
}

// Selection
void Canvas::copySelection() {
	if (textTool->isDrawing()) {
		textTool->onCopyCommand();
	}
	else {
		selectTool->copy();
	}
}
void Canvas::cutSelection() {
	if (textTool->isDrawing()) {
		textTool->onCutCommand();
	}
	else {
		selectTool->cut();
	}
}
void Canvas::deleteSelection() {
	if (textTool->isDrawing()) {
		textTool->onDeleteCommand();
	}
	else {
		selectTool->remove();
	}
}
void Canvas::pasteSelection() {
	if (textTool->isDrawing()) {
		textTool->onPasteCommand();
	}
	else {
		currentTool->finalize();
		stopAnimation();
		if (Clipboard::containsFormat(AsciiImageInfo::CLIPBOARD_FORMAT)) {
			auto result = DialogBoxResults::Yes;
			ClipboardInputStream in = ClipboardInputStream();
			in.open();
			if (in.isOpen()) {
				in.getData(AsciiImageInfo::CLIPBOARD_FORMAT);
				AsciiImageHeader header;
				if (AsciiImageInfo::loadHeader(&in, header, false)) {
					if (!(header.size <= image->at(frameIndex).size())) {
						result = MessageBox::show(getWindow(),
							"The image being pasted is larger than the frame size. Do you want to expand the frame size to fit it?",
							"Expand Frame Size",
							DialogBoxButtons::YesNoCancel,
							DialogBoxIcons::Question,
							SystemSounds::Information,
							MessageBox::DEFAULT_MAX_WIDTH,
							MessageBox::DEFAULT_MIN_WIDTH,
							3
						);
						if (result == DialogBoxResults::Yes) {
							resize(GMath::max(image->at(frameIndex).size(), header.size), !getFlag(image->getFormat(), ImageFormats::Multisize));
						}
					}
				}
				in.close();
			}
			if (result != DialogBoxResults::Cancel) {
				setCurrentTool(selectTool);
				// Check if we should ask the user if they want to resize here.
				//auto result = MessageBoxResults::No;
				selectTool->paste();
			}
		}
		else {
			sendMessage("Clipboard is empty", MESSAGE_COLOR_WARNING);
		}
	}
}
void Canvas::selectAll() {
	if (textTool->isDrawing()) {
		textTool->onSelectAllCommand();
	}
	else {
		currentTool->finalize();
		setCurrentTool(selectTool);
		selectTool->selectAll();
	}
}

// Frames
void Canvas::newFrame() {
	currentTool->finalize();
	stopAnimation();
	if (!getFlag(image->getFormat(), ImageFormats::Animation)) {
		sendMessage("Image is not an animation", MESSAGE_COLOR_WARNING);
	}
	else if ((int)image->frameCount() < AsciiImageInfo::MAX_FRAMES) {
		auto undoAction = std::make_shared<FrameUndoAction>(FrameUndoAction(frameIndex, FrameActionTypes::New));
		image->insertFrame(frameIndex + 1);
		image->setFrameSpeed((unsigned int)Settings::get()->get("Image")->getInt("FrameSpeed", RADIX_DECIMAL, 800), frameIndex + 1);
		addUndo(undoAction);
		_eventFrameAdded(shared_from_this(), FrameModifiedEventArgs(
			FrameInfo(
				image->at(frameIndex + 1).getName(),
				image->at(frameIndex + 1).getFrameSpeed()),
			frameIndex + 1
		));
		setFrameIndex(frameIndex + 1);
		sendMessage("Frame added");
	}
	else {
		sendMessage("Frame limit has been reached", MESSAGE_COLOR_WARNING);
	}
}
void Canvas::duplicateFrame() {
	currentTool->finalize();
	stopAnimation();
	if (!getFlag(image->getFormat(), ImageFormats::Animation)) {
		sendMessage("Image is not an animation", MESSAGE_COLOR_WARNING);
	}
	else if ((int)image->frameCount() < AsciiImageInfo::MAX_FRAMES) {
		auto undoAction = std::make_shared<FrameUndoAction>(FrameUndoAction(frameIndex, FrameActionTypes::Duplicate));
		image->insertFrame(frameIndex + 1, image->at(frameIndex));
		addUndo(undoAction);
		_eventFrameAdded(shared_from_this(), FrameModifiedEventArgs(
			FrameInfo(
				image->at(frameIndex + 1).getName(),
				image->at(frameIndex + 1).getFrameSpeed()),
			frameIndex + 1
		));
		setFrameIndex(frameIndex + 1);
		sendMessage("Frame duplicated");
	}
	else {
		sendMessage("Frame limit has been reached", MESSAGE_COLOR_WARNING);
	}
}
void Canvas::deleteFrame() {
	currentTool->finalize();
	stopAnimation();
	if (image->frameCount() > 1) {
		auto undoAction = std::make_shared<FrameUndoAction>(FrameUndoAction(frameIndex, FrameActionTypes::Delete));
		undoAction->setDeletedFrame(image->at(frameIndex));
		image->removeFrame(frameIndex);
		addUndo(undoAction);
		_eventFrameRemoved(shared_from_this(), FrameModifiedEventArgs(FrameInfo( "", 0), frameIndex));
		setFrameIndex(std::max(0, frameIndex - 1));
		sendMessage("Frame deleted");
	}
	else {
		sendMessage("Can't delete only frame", MESSAGE_COLOR_WARNING);
	}
}
void Canvas::moveFrameDown() {
	currentTool->finalize();
	stopAnimation();
	if (frameIndex + 1 < (int)image->frameCount()) {
		auto undoAction = std::make_shared<FrameUndoAction>(frameIndex, FrameActionTypes::MoveDown);
		image->moveFrame(frameIndex, 1, true);
		updateAutoSize();
		addUndo(undoAction);
		_eventFrameMoved(shared_from_this(), FrameMovedEventArgs(frameIndex, frameIndex + 1));
		setFrameIndex(frameIndex + 1);
		sendMessage("Frame moved down");
	}
	else {
		sendMessage("Can't move frame down", MESSAGE_COLOR_WARNING);
	}
}
void Canvas::moveFrameUp() {
	currentTool->finalize();
	stopAnimation();
	if (frameIndex > 0) {
		auto undoAction = std::make_shared<FrameUndoAction>(frameIndex, FrameActionTypes::MoveUp);
		image->moveFrame(frameIndex, -1, true);
		updateAutoSize();
		addUndo(undoAction);
		_eventFrameMoved(shared_from_this(), FrameMovedEventArgs(frameIndex, frameIndex - 1));
		setFrameIndex(frameIndex - 1);
		sendMessage("Frame moved up");
	}
	else {
		sendMessage("Can't move frame up", MESSAGE_COLOR_WARNING);
	}
}
void Canvas::renameFrame(const std::string& name) {
	currentTool->finalize();
	stopAnimation();
	if (name != image->getName(frameIndex)) {
		auto undoAction = std::make_shared<FrameUndoAction>(frameIndex, FrameActionTypes::Rename);
		undoAction->setOriginalName(image->getName(frameIndex));
		undoAction->setNewName(name);
		image->setName(frameIndex, name);
		addUndo(undoAction);
		_eventFrameRenamed(shared_from_this(), FrameModifiedEventArgs(
			FrameInfo(
				name,
				image->at(frameIndex).getFrameSpeed()),
			frameIndex
		));
		sendMessage("Frame renamed");
	}
}

// Messages
void Canvas::sendMessage(const std::string& message, unsigned char color) {
	_eventMessageSent(shared_from_this(), MessageEventArgs(message, color));
}

// Image changes
void Canvas::signalImageChanged() {
	_eventImageChanged(shared_from_this());
}

// Tool
bool Canvas::checkToolForCharacterUse(Pixel pixel) {
	if (currentTool->isDrawing()) {
		bool transparent = !getFlag(pixel.attributes, PixelAttributes::Char);
		return currentTool->characterSelected(pixel.character, transparent);
	}
	return false;
}

// Animation
void Canvas::animate() {
	if (!animating) {
		animating = true;
		animationTimer = 0;
	}
	else {
		animating = false;
	}
}
void Canvas::stopAnimation() {
	if (animating) {
		animating = false;
		_eventAnimationCanceled(shared_from_this());
	}
}

#pragma endregion
//========= UNDO ACTIONS =========
#pragma region Undo Actions

void Canvas::insertFrameUndoAction(int index) {
	image->insertFrame(index);
	image->setFrameSpeed((unsigned int)Settings::get()->get("Image")->getInt("FrameSpeed", RADIX_DECIMAL, 800), frameIndex + 1);
	_eventFrameAdded(shared_from_this(), FrameModifiedEventArgs(
		FrameInfo(
			image->at(index).getName(),
			image->at(index).getFrameSpeed()),
		index
	));
	setFrameIndex(index);
}
void Canvas::insertFrameUndoAction(int index, const AsciiImageFrame& frame) {
	image->insertFrame(index, frame);
	_eventFrameAdded(shared_from_this(), FrameModifiedEventArgs(
		FrameInfo(
			frame.getName(),
			frame.getFrameSpeed()),
		index
	));
	setFrameIndex(index);
}
void Canvas::removeFrameUndoAction(int index) {
	image->removeFrame(index);
	_eventFrameRemoved(shared_from_this(), FrameModifiedEventArgs(FrameInfo("", 0), index));
	setFrameIndex(std::max(0, index - 1));
}
void Canvas::moveFrameUndoAction(int index, int newIndex) {
	image->moveFrame(index, newIndex);
	_eventFrameMoved(shared_from_this(), FrameMovedEventArgs(index, newIndex));
	setFrameIndex(newIndex);
}
void Canvas::renameFrameUndoAction(int index, const std::string& name) {
	image->setName(index, name);
	_eventFrameRenamed(shared_from_this(), FrameModifiedEventArgs(
		FrameInfo(
			name,
			image->at(index).getFrameSpeed()),
		index
	));
	setFrameIndex(index);
}
void Canvas::setFrameSpeedUndoAction(int index, unsigned int speed) {
	image->setFrameSpeed(speed, index);
	_eventFrameRenamed(shared_from_this(), FrameModifiedEventArgs(
		FrameInfo(
			image->at(index).getName(),
			image->at(index).getFrameSpeed()),
		index
	));
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I Canvas::autoSizeBehavior() const {
	if (resizing != Point2I::ZERO)
		return tempSize + 1;
	else
		return image->at(previewIndex != -1 ? previewIndex : frameIndex).size() + 1;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<EventArgs>& Canvas::eventImageChanged() {
	return _eventImageChanged;
}
Event<MessageEventArgs>& Canvas::eventMessageSent() {
	return _eventMessageSent;
}
Event<SelectionChangedEventArgs>& Canvas::eventToolChanged() {
	return _eventToolChanged;
}
Event<UndoAvailabilityEventArgs>& Canvas::eventUndoAvailabilityChanged() {
	return _eventUndoAvailabilityChanged;
}
Event<PixelChangedEventArgs>& Canvas::eventPixelChanged() {
	return _eventPixelChanged;
}
Event<StatusBarDimensionsEventArgs>& Canvas::eventStatusBarDimensions() {
	return _eventStatusBarDimensions;
}
Event<EventArgs>& Canvas::eventAnimationCanceled() {
	return _eventAnimationCanceled;
}

// Frames
Event<SelectionChangedEventArgs>& Canvas::eventFrameChanged() {
	return _eventFrameChanged;
}
Event<FrameListChangedEventArgs>& Canvas::eventFrameListChanged() {
	return _eventFrameListChanged;
}
Event<FrameModifiedEventArgs>& Canvas::eventFrameAdded() {
	return _eventFrameAdded;
}
Event<FrameModifiedEventArgs>& Canvas::eventFrameRemoved() {
	return _eventFrameRemoved;
}
Event<FrameModifiedEventArgs>& Canvas::eventFrameRenamed() {
	return _eventFrameRenamed;
}
Event<FrameMovedEventArgs>& Canvas::eventFrameMoved() {
	return _eventFrameMoved;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Canvas::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	g->fill(image->getBackground());

	if (previewIndex != -1 && previewIndex != frameIndex && resizing == Point2I::ZERO) {
		g->drawImage(getLocalVisibleArea().point, getLocalVisibleArea(), image->at(previewIndex));
		//g->drawImage(Point2I::ZERO, getLocalVisibleArea(), image->at(previewIndex));
	}
	else {

		if (currentTool->isDrawing()) {
			// Draw the tool if currently drawing
			currentTool->onPaint(e);
		}
		else {
			// Draw the frame
			g->drawImage(getLocalVisibleArea().point, getLocalVisibleArea(), image->at(frameIndex));
		}

		// Draw the cursor invert (Don't draw while a dialog is showing, it looks ugly)
		if (cursorVisible && getMousePosition() < image->size() && !getWindow()->isShowingDialog()) {
			g->setSpecial(getMousePosition(), PixelAttributes::Invert);
		}
	}

	const unsigned char borderColor = toColor(BLACK, DARK_GRAY);
	const unsigned char hoverColor = toColor(BLACK, GRAY);
	const unsigned char resizeColor = toColor(BLACK, WHITE);
	g->setConnectGlobalBorders(true);

	Point2I imageSize = getResizingSize();

	bool onEdge = !(getMousePosition() < image->size()) && isMouseInside();
	bool hoverX = resizing.x != 0 || (onEdge && getMousePosition().x == image->at(frameIndex).size().x);
	bool hoverY = resizing.y != 0 || (onEdge && getMousePosition().y == image->at(frameIndex).size().y);

	// Draw the bounds of the image
	g->drawBorder(Rectangle2I(-1, imageSize.y, imageSize.x + 2, 1), Pixel(' ', borderColor), false, DisabledBorders(false, true, false, true));
	g->drawBorder(Rectangle2I(imageSize.x, -1, 1, imageSize.y + 2), Pixel(' ', borderColor), false, DisabledBorders(true, false, true, false));
	g->drawBorder(Rectangle2I(imageSize, 1, 1), Pixel(' ', borderColor), false);
	if (hoverX)
		g->setColor(Rectangle2I(imageSize.x, 0, 1, imageSize.y), hoverColor);
	if (hoverY)
		g->setColor(Rectangle2I(0, imageSize.y, imageSize.x, 1), hoverColor);
	if (hoverX && hoverY)
		g->setColor(imageSize, hoverColor);

	g->setConnectGlobalBorders(false);
}
void Canvas::onTick(ControlSPtr sender, TickEventArgs& e) {
	Point2I oldPoint = toolPoint;
	Point2I oldSize = toolSize;
	toolPoint = (isMouseInside() ? getMousePosition() : -Point2I::ONE);
	if (currentTool->isDrawing()) {
		stopAnimation();
		currentTool->onTick(e);
		toolSize = currentTool->getSelectionSize();
	}
	else {
		toolSize = -Point2I::ONE;
		if (resizing != Point2I::ZERO) {
			toolSize = tempSize;
		}
		if (animating) {
			animationTimer += e.timeSinceLastTick;
			while (animationTimer >= (int)image->at(frameIndex).getFrameSpeed()) {
				animationTimer -= std::max(1, (int)image->at(frameIndex).getFrameSpeed());
				if (image->frameCount() > 1) {
					int oldIndex = frameIndex;
					currentTool->finalize();
					frameIndex = GMath::wrap(frameIndex + 1, (int)image->frameCount());
					updateAutoSize();
					_eventFrameChanged(shared_from_this(), SelectionChangedEventArgs(frameIndex, oldIndex));
				}
			}
		}
	}
	if (toolPoint != oldPoint || toolSize != oldSize) {
		_eventStatusBarDimensions(shared_from_this(), StatusBarDimensionsEventArgs(toolPoint, toolSize));
	}
}
void Canvas::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	if (resizing == Point2I::ZERO) {
		if (currentTool->isDrawing())
			currentTool->onMouseMoved(e);
	}
	else {
		// Update resizing
		if (resizing.x != 0)
			tempSize.x = e.newPosition.x;
		if (resizing.y != 0)
			tempSize.y = e.newPosition.y;
		tempSize = GMath::clamp(tempSize, Point2I::ONE, AsciiImageInfo::MAX_SIZE);
		updateAutoSize();
	}
}
void Canvas::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if ((e.newPosition < image->size() || e.buttonState == PressedStates::Released) && resizing == Point2I::ZERO) {
		// Special eyedropper shortcut
		if (e.isButtonPressed(MouseButtons::Middle)) {
			currentTool->finalize();
			primaryPixel = image->at(frameIndex).getPixel(e.newPosition);
			_eventPixelChanged(shared_from_this(), PixelChangedEventArgs(primaryPixel, false));
		}
		else {
			currentTool->onMouseButton(e);
		}
	}
	else {
		if (e.isButtonPressed(MouseButtons::Left)) {
			currentTool->finalize();
			tempSize = image->size();
			if (e.newPosition.y < image->size().y) {
				resizing = Point2I(1, 0);
			}
			else if (e.newPosition.x < image->size().x) {
				resizing = Point2I(0, 1);
			}
			else {
				resizing = Point2I::ONE;
			}
		}
		else if (resizing != Point2I::ZERO && e.isButtonReleased(MouseButtons::Left)) {
			resizing = Point2I::ZERO;
			// Finalize resize
			resize(tempSize, !getFlag(image->getFormat(), ImageFormats::Multisize));
		}
	}
}
void Canvas::onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e) {
	if (currentTool->isDrawing())
		currentTool->onKey(e);
}
void Canvas::onKeyTypedGlobal(ControlSPtr sender, KeyTypedEventArgs& e) {
	if (currentTool->isDrawing())
		currentTool->onKeyTyped(e);
}

#pragma endregion
//=================================================================|
