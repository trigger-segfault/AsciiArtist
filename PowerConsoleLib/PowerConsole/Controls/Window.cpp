/*=================================================================|
|* File:				WindowNew.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Window.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/HiddenWinAPI.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Drawing/Characters.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/IO/System.h>
#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Controls/Dialog.h>
#include <PowerConsole/PowerConsoleSettings.h>

using namespace PowerConsole;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::Input;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

Window::Window()
	:	// Main Loop
	running(false),
	frameStart(0),
	frameDuration(0),
	fps(0.0f),

	// Drawing
	graphicsImage(std::make_shared<AsciiImage>(Point2I::ONE, ImageFormats::Basic, Pixel(' ', 0x00))),
	forceRedrawBuffer(true),

	owner(nullptr),
	currentFocus(nullptr),
	dialog(nullptr),

	// Resize Handle
	resizing(false),
	resizeHandleEnabled(false),
	resizeHandlePixel(),
	maxSize(MINMAX_NONE),
	minSize(MINMAX_NONE),

	consoleSettings(std::make_shared<ConsoleSettings>()),
	consoleWrapper(std::make_shared<WindowConsoleWrapper>(consoleSettings)),
	tabStopCollection(std::make_shared<TabStopCollection>()),
	keyboardInput(std::make_shared<Keyboard>()),
	mouseInput(std::make_shared<Mouse>()),

	// Events
	_eventLoad(EventDirections::FirstInFirstCalled),
	_eventUnload(EventDirections::FirstInFirstCalled),
	_eventFocusChanged(EventDirections::FirstInFirstCalled),
	_eventWindowMoved(EventDirections::FirstInFirstCalled),
	_eventWindowActivated(EventDirections::FirstInFirstCalled),
	_eventWindowDeactivated(EventDirections::FirstInFirstCalled),
	_eventPaintDebug(EventDirections::FirstInFirstCalled),
	_eventGlobalTick(EventDirections::FirstInFirstCalled) {

	setSizeSafe(Point2I(SIZE_AUTO));
	setHorizontalAlignmentSafe(HorizontalAlignments::Stretch);
	setVerticalAlignmentSafe(VerticalAlignments::Stretch);
	eventInitialize().add(bindEvent(Window::onInitialize));
	eventLayoutChanged().add(bindEvent(Window::onLayoutChanged));
	eventMouseButtonGlobal().add(bindEvent(Window::onMouseButtonGlobal));
	eventMouseMovedGlobal().add(bindEvent(Window::onMouseMovedGlobal));
	eventWindowActivated().add(bindEvent(Window::onWindowActivated));
}
Window::~Window() {

}
void Window::load() {
	consoleSettings->start();
	running = true;

	redraw();
	_eventLoad(shared_from_this());
}
void Window::unload() {
	_eventUnload(shared_from_this());
	consoleSettings = nullptr;
}
void Window::onInitialize(ControlSPtr sender, EventArgs& e) {
	consoleSettings->setOwner(std::dynamic_pointer_cast<Window>(shared_from_this()));
	tabStopCollection->setOwner(std::dynamic_pointer_cast<Window>(shared_from_this()));
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void Window::mainLoop() {
	bool firstLoop = true;

	frameStart = clock();
	while (running) {
		// Update the consoleSettings and apply changes not made by the PowerConsole
		consoleSettings->checkForChanges();

		// Sleep until the desired frame rate is met.
		// Note: Desired frame rate by default is zero.
		int newFrameDuration = clock() - frameStart;
		int desiredFrameDuration = PowerConsoleSettings::getDesiredFrameRate();
		if (newFrameDuration < desiredFrameDuration)
			System::sleep((unsigned int)(desiredFrameDuration - newFrameDuration));

		// Update the frame time
		frameDuration = clock() - frameStart;
		frameStart = clock();
		fps = 1000.0f / (float)frameDuration;

		// Handle Input
		auto inRec = consoleSettings->nextInputRecord();
		keyboardInput->update(
			inRec,
			consoleSettings->isActive()
		);
		mouseInput->update(
			inRec,
			consoleSettings->isActive(),
			consoleSettings->getClientPosition(),
			consoleSettings->getBufferSize(),
			consoleSettings->getBufferPosition(),
			consoleSettings->getFontSize()
		);
		if (keyboardInput->isHotkeyPressed(PowerConsoleSettings::getDebugHotkey())) {
			PowerConsoleSettings::toggleDebugging();
		}
		if (keyboardInput->isHotkeyPressed(PowerConsoleSettings::getRecordHotkey())) {
			PowerConsoleSettings::toggleRecording();
		}

		// Handle Tab Stops
		if (keyboardInput->isKeyPressed(Keys::Tab)) {
			nextTabStop();
		}

		// Handle Events
		handleEvents();

		// Handle Drawing
		redraw();

		if (keyboardInput->isHotkeyPressed(PowerConsoleSettings::getPrintScreenHotkey())) {
			graphicsImage->saveClipboard();
		}

		firstLoop = false;
	}
}
void Window::handleEvents() {
	// Store all the controls to send events to.
	// We do this now so that nothing goes wrong if a control is removed or added during the event calling.
	std::deque<ControlSPtr> queuedControls;
	forEach([&queuedControls](ControlSPtr control) {
		if (control->isEnabled() && control->isActuallyVisible())
			queuedControls.push_back(control);
	});

	// Mouse Events:
	Point2I newPosition;
	for (ControlSPtr control : queuedControls) {
		newPosition = mouseInput->getPosition() - control->getGlobalPosition();
		if (control->getMousePositionLast() != newPosition) {
			control->eventMouseMovedGlobal()(control, MouseEventArgs(newPosition, control->getMousePosition()));
		}
		else {
			// Update changes to the last mouse position either way.
			control->updateMousePositionLast();
		}
	}
	for (int i = 1; i < Mouse::NUM_BUTTONS; i++) {
		if (mouseInput->hasButtonStateChanged(i)) {
			for (ControlSPtr control : queuedControls) {
				control->eventMouseButtonGlobal()(control, MouseButtonEventArgs(
					(MouseButtons)i, (PressedStates)mouseInput->getButtonState(i),
					control->getMousePosition(), control->getMousePositionLast(),
					keyboardInput->getModifiers()
				));
			}
		}
	}
	if (mouseInput->isButtonDoubleClicked(MouseButtons::Left)) {
		for (ControlSPtr control : queuedControls) {
			if (control->isMouseInside()) {
				control->eventDoubleClick()(control, MouseEventArgs(
					control->getMousePosition(), control->getMousePositionLast()
				));
			}
		}
	}
	for (int i = 1; i < Mouse::NUM_WHEELS; i++) {
		if (mouseInput->isWheelScrolled(i)) {
			for (ControlSPtr control : queuedControls) {
				control->eventMouseWheelGlobal()(control, MouseWheelEventArgs(
					(MouseWheels)i, mouseInput->getWheelDelta(i),
					control->getMousePosition(), control->getMousePositionLast(),
					keyboardInput->getModifiers()
				));
			}
		}
	}

	// Keyboard Events:
	for (int i = 1; i < Keyboard::NUM_KEYS; i++) {
		if (keyboardInput->hasKeyStateChanged(i)) {
			for (ControlSPtr control : queuedControls) {
				control->eventKeyGlobal()(control, KeyboardEventArgs(
					(Keys)i, (PressedStates)keyboardInput->getKeyState(i),
					keyboardInput->getModifiers()
				));
			}
		}
	}
	if (keyboardInput->isCharTyped() || keyboardInput->isAnyKeyTyped()) {
		for (ControlSPtr control : queuedControls) {
			control->eventKeyTypedGlobal()(control, KeyTypedEventArgs(
				keyboardInput->getCharTyped(), keyboardInput->getKeyTyped(),
				keyboardInput->getModifiers()
			));
		}
	}
	
	// Command Events:
	for (ControlSPtr control : queuedControls) {
		for (Command& command : control->commands()) {
			if (command.hasHotkey() && keyboardInput->isHotkeyPressed(command.getHotkey()) &&
				(control->hasFocus() || !command.isFocusRequired())) {
				command(control);
			}
		}
	}

	// Update Events:
	for (ControlSPtr control : queuedControls) {
		control->eventTick()(control, TickEventArgs(frameDuration));
	}

	eventGlobalTick()(shared_from_this(), TickEventArgs(frameDuration));
}

bool Window::showDialog(DialogSPtr dialog) {
	int dialogStart = clock();
	this->dialog = dialog;
	dialog->setOwner(std::dynamic_pointer_cast<Window>(shared_from_this()));
	dialog->load();
	dialog->mainLoop();

	dialog->unload();

	// Restore consoleSettings settings from this window
	consoleSettings->applyChanges();
	consoleSettings->clearInputRecords();
	forceRedrawBuffer = true;

	keyboardInput->disable(true);
	mouseInput->disable(true);

	frameStart += clock() - dialogStart;
	this->dialog = nullptr;
	return dialog->getDialogResult();
}

void Window::repaint(GraphicsSPtr g) {
	g->startRegion(getLocalPosition(), getLocalVisibleArea());
	g->clear();
	eventPaint()(shared_from_this(), PaintEventArgs(g));
	g->resetOptions();

	for (ControlSPtr control : children()) {
		if (control->isVisible())
			control->repaint(g);
	}

	if (resizeHandleEnabled) {
		Point2I corner = consoleSettings->getBufferSize() - 1;
		if (resizing)
			g->setPixel(corner, Pixel(resizeHandlePixel.character, resizeHandlePixel.resizeColor));
		else if (mouseInput->getPosition() == corner)
			g->setPixel(corner, Pixel(resizeHandlePixel.character, resizeHandlePixel.hoverColor));
		else
			g->setPixel(corner, Pixel(resizeHandlePixel.character, resizeHandlePixel.normalColor));
	}
	if (PowerConsoleSettings::isDebugging()) {
		eventPaintDebug()(shared_from_this(), PaintEventArgs(g));
	}
	g->resetOptions();
	g->endRegion();

	if (PowerConsoleSettings::isDebugging() && PowerConsoleSettings::getDrawDebugFrameRate()) {
		g->drawString(Point2I::ZERO, std::to_string(frameDuration), Pixel(' ', PowerConsoleSettings::getDebugFrameRateColor()));
	}
}
void Window::redraw() {
	// Handle Drawing
	GraphicsSPtr g = graphicsImage->createGraphics();
	g->clear();
	repaint(g);

	PowerConsoleSettings::updateRecording(*graphicsImage);

	if (PowerConsoleSettings::isRecording()) {
		int time = PowerConsoleSettings::getRecordingTime();
		int sec = (time / 1000) % 60;
		int min = (time / 1000) / 60;
		std::string recStr =
			std::to_string(min) + ":" +
			(sec < 10 ? "0" : "") +
			std::to_string(sec);
		recStr = std::string(1, (char)7) + " REC [" + recStr + "]";
		g->drawString(Point2I(getContainerSize().x - (int)recStr.length(), 0), recStr, Pixel(' ', toColor(BLACK, RED)));
	}

	// Write the changes to the buffer
	consoleSettings->writeBuffer(*graphicsImage, forceRedrawBuffer);
	forceRedrawBuffer = false;
}

void Window::close() {
	running = false;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<EventArgs>& Window::eventLoad() {
	return _eventLoad;
}
Event<EventArgs>& Window::eventUnload() {
	return _eventUnload;
}
Event<FocusChangedEventArgs>& Window::eventFocusChanged() {
	return _eventFocusChanged;
}
Event<WindowMovedEventArgs>& Window::eventWindowMoved() {
	return _eventWindowMoved;
}
Event<EventArgs>& Window::eventWindowActivated() {
	return _eventWindowActivated;
}
Event<EventArgs>& Window::eventWindowDeactivated() {
	return _eventWindowDeactivated;
}
Event<PaintEventArgs>& Window::eventPaintDebug() {
	return _eventPaintDebug;
}
Event<TickEventArgs>& Window::eventGlobalTick() {
	return _eventGlobalTick;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Window::onMouseButtonGlobal(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (resizeHandleEnabled) {
		if (e.buttonChanged == MouseButtons::Left) {
			if (e.buttonState == PressedStates::Pressed && e.newPosition == getActualSize() - 1)
				resizing = true;
			else if (e.buttonState == PressedStates::Released)
				resizing = false;
		}
	}
}
void Window::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	if (resizing) {
		Point2I newSize = GMath::clamp(e.newPosition + 1, getActualMinSize(), getActualMaxSize());
		if (newSize != getActualSize()) {
			setSize(newSize);
		}
	}
}
void Window::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged && getContainerSize() != graphicsImage->size()) {
		graphicsImage->resize(getContainerSize());
	}
}
void Window::onChildRemoved(ControlSPtr sender, ChildModifiedEventArgs& e) {
	if (tabStopCollection->contains(e.control)) {
		tabStopCollection->remove(e.control);
	}
}
void Window::onWindowActivated(ControlSPtr sender, EventArgs& e) {
	keyboardInput->disable(true);
	mouseInput->disable(true);
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Control Tree

void Window::setParent(ControlSPtr parent) {
	throw std::exception("Cannot set the parent of a window");
}
bool Window::hasOwner() const {
	return owner != nullptr;
}
WindowSPtr Window::getOwner() {
	return owner;
}
void Window::setOwner(WindowSPtr owner) {
	this->owner = owner;
}
WindowSPtr Window::getWindow() {
	return std::dynamic_pointer_cast<Window>(shared_from_this());
}
bool Window::isShowingDialog() const {
	return dialog != nullptr;
}
DialogSPtr Window::getDialog() const {
	return dialog;
}
WindowSPtr Window::getCurrentWindow() {
	if (dialog != nullptr)
		return dialog->getCurrentWindow();
	else
		return std::static_pointer_cast<Window>(shared_from_this());
}

#pragma endregion
//--------------------------------
#pragma region Contained Classes

WindowConsoleWrapper& Window::console() {
	return *consoleWrapper;
}
const WindowConsoleWrapper& Window::console() const {
	return *consoleWrapper;
}

TabStopCollection& Window::tabStops() {
	return *tabStopCollection;
}
const TabStopCollection& Window::tabStops() const {
	return *tabStopCollection;
}

const Keyboard& Window::keyboard() {
	return *keyboardInput;
}
const Mouse& Window::mouse() {
	return *mouseInput;
}

const Keyboard& Window::globalKeyboard() {
	return getCurrentWindow()->keyboard();
}
const Mouse& Window::globalMouse() {
	return getCurrentWindow()->mouse();
}

#pragma endregion
//--------------------------------
#pragma region Main Loop

bool Window::isRunning() const {
	return running;
}
int Window::getFrameDuration() const {
	return frameDuration;
}
float Window::getFPS() const {
	return fps;
}
void Window::forceRedraw() {
	forceRedrawBuffer = true;
}

#pragma endregion
//--------------------------------
#pragma region Input

ControlSPtr Window::getFocus() {
	return currentFocus;
}
void Window::setFocus(ControlSPtr control) {
	// Don't change focus if the new focus is the same
	//ControlSPtr focus = (this->focus.expired() ? nullptr : this->focus.lock());
	if (currentFocus != control) {
		if (currentFocus == nullptr || (currentFocus->isEnabled() && currentFocus->isVisible())) {
			ControlSPtr lostFocusControl = currentFocus;
			currentFocus = control;

			if (lostFocusControl != nullptr) {
				lostFocusControl->eventFocusLost()(lostFocusControl);
			}

			// Don't do anything if there were changes in focus caused by the last event.
			if (currentFocus == nullptr && defaultFocus != nullptr && defaultFocus->isEnabled() && defaultFocus->isVisible()) {
				currentFocus = defaultFocus;
				_eventFocusChanged(shared_from_this(), FocusChangedEventArgs(defaultFocus, lostFocusControl));
				defaultFocus->eventFocusGained()(defaultFocus);
			}
			else if (currentFocus == control && control != nullptr) {
				_eventFocusChanged(shared_from_this(), FocusChangedEventArgs(control, lostFocusControl));
				control->eventFocusGained()(control);
			}
		}
	}
}
ControlSPtr Window::getFocusDefault() const {
	return defaultFocus;
}
void Window::setFocusDefault(ControlSPtr control) {
	defaultFocus = control;
	if (currentFocus == nullptr && control != nullptr) {
		currentFocus = control;
		control->eventFocusGained()(control);
	}
}
void Window::clearFocus() {
	setFocus(nullptr);
}
void Window::nextTabStop() {
	ControlSPtr next = tabStopCollection->getNextTabStop();
	if (next != nullptr) {
		setFocus(next);
	}
}

#pragma endregion
//--------------------------------
#pragma region Rendering

Pixel Window::getBackground() const {
	return this->graphicsImage->getBackground();
}
void Window::setBackground(Pixel background) {
	this->graphicsImage->setBackground(background);
}

#pragma endregion
//--------------------------------
#pragma region Local Layout

Point2I Window::getSize() const {
	return consoleSettings->getBufferSize();
}
void Window::setSize(Point2I size) {
	size = GMath::clamp(size, getActualMinSize(), getActualMaxSize());
	consoleSettings->setConsoleScreen(size, Point2I::ZERO, size);
}
Point2I Window::getMaxSize() {
	return maxSize;
}
void Window::setMaxSize(Point2I maxSize) {
	this->maxSize = maxSize;
}
Point2I Window::getMinSize() {
	return minSize;
}
void Window::setMinSize(Point2I minSize) {
	this->minSize = minSize;
}
Point2I Window::getActualMaxSize() {
	Point2I actualMaxSize = GMath::min(consoleSettings->getConsoleMaxSize(), maxSize);
	if (maxSize.x == MINMAX_NONE)
		actualMaxSize.x = consoleSettings->getConsoleMaxSize().x;
	if (maxSize.y == MINMAX_NONE)
		actualMaxSize.y = consoleSettings->getConsoleMaxSize().y;
	return actualMaxSize;
}
Point2I Window::getActualMinSize() {
	return GMath::max(consoleSettings->getConsoleMinSize(), minSize);
}

void Window::setMargin(Thickness margin) {
	throw std::exception("'Window::setMargin(...)' should not be called");
}
void Window::setHorizontalAlignment(HorizontalAlignments horizontalAlignment) {
	throw std::exception("'Window::setHorizontalAlignment(...)' should not be called");
}
void Window::setVerticalAlignment(VerticalAlignments verticalAlignment) {
	throw std::exception("'Window::setVerticalAlignment(...)' should not be called");
}

#pragma endregion
//--------------------------------
#pragma region Resize Handle

bool Window::isResizeHandleEnabled() {
	return resizeHandleEnabled;
}
void Window::setResizeHandleEnabled(bool resizeHandleEnabled) {
	this->resizeHandleEnabled = resizeHandleEnabled;
}
ResizeHandlePixel Window::getResizeHandlePixel() const {
	return resizeHandlePixel;
}
void Window::setResizeHandlePixel(ResizeHandlePixel pixel) {
	this->resizeHandlePixel = pixel;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=================================================================|
