/*=================================================================|
|* File:				EventArgs.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef EVENT_ARGS_H
#define EVENT_ARGS_H

#include <string>
#include <PowerConsole/Geometry/Rectangle.h>
#include <PowerConsole/Input/Hotkey.h>
#include <PowerConsole/Input/MouseButtons.h>
#include <PowerConsole/Input/InputHandler.h>
#include <PowerConsole/Drawing/Graphics.h>
#include <PowerConsole/Controls/Events/Event.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region EventArgs

/* The default event args to pass to events. */
struct EventArgs {
	/* True if the event in line will not be called. */
	bool handled;
	EventArgs();
};

#pragma endregion
//--------------------------------
#pragma region SizeChangedEventArgs

/* Shows changes in layout of a control. */
struct LayoutChangedEventArgs : public EventArgs {
	/* True if the actual size of the control has changed. */
	bool sizeChanged;
	/* True if the margin of the control has changed. */
	bool marginChanged;
	/* True if the alignment of the control has changed. */
	bool alignmentChanged;
	/* True if the local position of the control has changed. */
	bool positionChanged;
	/* True if the visible area of the control has changed. */
	bool visibleAreaChanged;

	inline LayoutChangedEventArgs()
		: EventArgs(), sizeChanged(false), marginChanged(false),
		alignmentChanged(false), positionChanged(false), visibleAreaChanged(false) {}
	inline LayoutChangedEventArgs(bool sizeChanged, bool marginChanged, bool alignmentChanged, bool positionChanged, bool visibleAreaChanged)
		: EventArgs(), sizeChanged(sizeChanged), marginChanged(marginChanged),
		alignmentChanged(alignmentChanged), positionChanged(positionChanged),
		visibleAreaChanged(visibleAreaChanged) {}
};
/* Shows changes in size of a control. */
struct SizeChangedEventArgs : public EventArgs {
	/* The new size of the control. */
	PcGeo::Point2I newSize;
	/* The previous size of the control. */
	PcGeo::Point2I previousSize;

	inline SizeChangedEventArgs()
		: EventArgs(), newSize(), previousSize() {}
	inline SizeChangedEventArgs(PcGeo::Point2I newSize, PcGeo::Point2I previousSize)
		: EventArgs(), newSize(newSize), previousSize(previousSize) {}
};

#pragma endregion
//--------------------------------
#pragma region MouseEventArgs

/* Shows mouse input. */
struct MouseEventArgs : public EventArgs {
	/* The current position of the mouse in the control. */
	PcGeo::Point2I newPosition;
	/* The previous position of the mouse in the control. */
	PcGeo::Point2I previousPosition;

	inline MouseEventArgs()
		: EventArgs(), newPosition(PcGeo::Point2I::ZERO), previousPosition(PcGeo::Point2I::ZERO) {}
	inline MouseEventArgs(PcGeo::Point2I newPosition, PcGeo::Point2I previousPosition)
		: EventArgs(), newPosition(newPosition), previousPosition(previousPosition) {}
};
/* Shows mouse button input. */
struct MouseButtonEventArgs : public MouseEventArgs {
	/* The mouse button that received input. */
	PcInput::MouseButtons buttonChanged;
	/* The state of the mouse button. */
	PcInput::PressedStates buttonState;
	/* The key modifiers that are currently down. */
	PcInput::Modifiers modifiers;

	inline MouseButtonEventArgs()
		: MouseEventArgs(), buttonChanged(PcInput::MouseButtons::None), buttonState(PcInput::PressedStates::Released), modifiers(PcInput::Modifiers::None) {}
	inline MouseButtonEventArgs(PcInput::MouseButtons buttonChanged, PcInput::PressedStates buttonState, PcGeo::Point2I newPosition, PcGeo::Point2I previousPosition, PcInput::Modifiers modifiers)
		: MouseEventArgs(newPosition, previousPosition), buttonChanged(buttonChanged), buttonState(buttonState), modifiers(modifiers) {}

	/* Returns true if the specified mouse button was pressed. */
	inline bool isButtonPressed(PcInput::MouseButtons buttonCode) const {
		return buttonChanged == buttonCode && buttonState == PcInput::PressedStates::Pressed;
	}
	/* Returns true if the specified mouse button was released. */
	inline bool isButtonReleased(PcInput::MouseButtons buttonCode) const {
		return buttonChanged == buttonCode && buttonState == PcInput::PressedStates::Released;
	}
};

/* Shows mouse wheel input. */
struct MouseWheelEventArgs : public MouseEventArgs {
	/* The mouse wheel that received input. */
	PcInput::MouseWheels wheelChanged;
	/* The delta change of the mouse wheel. Positive values equates to the top-left direction. */
	int delta;
	/* The key modifiers that are currently down. */
	PcInput::Modifiers modifiers;

	inline MouseWheelEventArgs()
		: MouseEventArgs(), wheelChanged(PcInput::MouseWheels::None), delta(0), modifiers(PcInput::Modifiers::None) {}
	inline MouseWheelEventArgs(PcInput::MouseWheels wheelChanged, int delta, PcGeo::Point2I newPosition, PcGeo::Point2I previousPosition, PcInput::Modifiers modifiers)
		: MouseEventArgs(newPosition, previousPosition), wheelChanged(wheelChanged), delta(delta), modifiers(modifiers) {}

	/* Returns true if the mouse wheel was scrolled up. */
	inline bool isScrolledUp() const {
		return wheelChanged == PcInput::MouseWheels::Vertical && delta > 0;
	}
	/* Returns true if the mouse wheel was scrolled down. */
	inline bool isScrolledDown() const {
		return wheelChanged == PcInput::MouseWheels::Vertical && delta < 0;
	}
	/* Returns true if the mouse wheel was scrolled left. */
	inline bool isScrolledLeft() const {
		return wheelChanged == PcInput::MouseWheels::Horizontal && delta > 0;
	}
	/* Returns true if the mouse wheel was scrolled right. */
	inline bool isScrolledRight() const {
		return wheelChanged == PcInput::MouseWheels::Horizontal && delta < 0;
	}
};

#pragma endregion
//--------------------------------
#pragma region KeyboardEventArgs

/* Shows keyboard input. */
struct KeyboardEventArgs : public EventArgs {
	/* The key that received input. */
	PcInput::Keys keyChanged;
	/* The state of the key. */
	PcInput::PressedStates keyState;
	/* The key modifiers that are currently down. */
	PcInput::Modifiers modifiers;

	inline KeyboardEventArgs()
		: EventArgs(), keyChanged(PcInput::Keys::None), keyState(PcInput::PressedStates::Released), modifiers(PcInput::Modifiers::None) {}
	inline KeyboardEventArgs(PcInput::Keys keyChanged, PcInput::PressedStates keyState, PcInput::Modifiers modifiers)
		: EventArgs(), keyChanged(keyChanged), keyState(keyState), modifiers(modifiers) {}

	/* Returns true if the specified key was pressed. */
	inline bool isKeyPressed(PcInput::Keys keyCode) const {
		return keyChanged == keyCode && keyState == PcInput::PressedStates::Pressed;
	}
	/* Returns true if the specified key was released. */
	inline bool isKeyReleased(PcInput::Keys keyCode) const {
		return keyChanged == keyCode && keyState == PcInput::PressedStates::Released;
	}
	/* Returns true if the specified hotkey was pressed. */
	inline bool isHotkeyPressed(PcInput::Hotkey hotkey) const {
		return keyChanged == hotkey.key && keyState == PcInput::PressedStates::Pressed && modifiers == hotkey.modifiers;
	}
};
/* Shows keys and characters typed. */
struct KeyTypedEventArgs : public EventArgs {
	/* The character that was typed. */
	char inputChar;
	/* The character that was typed. */
	PcInput::Keys inputKey;
	/* The key modifiers that are currently down. */
	PcInput::Modifiers modifiers;

	inline KeyTypedEventArgs()
		: EventArgs(), inputChar('\0'), inputKey(PcInput::Keys::None), modifiers(PcInput::Modifiers::None) {}
	inline KeyTypedEventArgs(char inputChar, PcInput::Keys inputKey, PcInput::Modifiers modifiers)
		: EventArgs(), inputChar(inputChar), inputKey(inputKey), modifiers(modifiers) {}

	/* Returns true if the specified hotkey was typed. */
	inline bool isHotkeyTyped(PcInput::Hotkey hotkey) const {
		return inputKey == hotkey.key && modifiers == hotkey.modifiers;
	}
};

#pragma endregion
//--------------------------------
#pragma region UpdateEventArgs

/* Shows changes in time since the last tick. */
struct TickEventArgs : public EventArgs {
	/* The change in time since the last tick. */
	int timeSinceLastTick;

	inline TickEventArgs()
		: EventArgs(), timeSinceLastTick(0) {}
	inline TickEventArgs(int timeSinceLastTick)
		: EventArgs(), timeSinceLastTick(timeSinceLastTick) {}
};

/* Allows access to the graphics for drawing. */
struct PaintEventArgs : public EventArgs {
	/* The graphics object to paint to. */
	PcDraw::GraphicsSPtr graphics;

	inline PaintEventArgs()
		: EventArgs(), graphics(nullptr) {}
	inline PaintEventArgs(PcDraw::GraphicsSPtr graphics)
		: EventArgs(), graphics(graphics) {}
};

#pragma endregion
//--------------------------------
#pragma region ChildModifiedEventArgs

/* Shows addition or removal of a child control in a collection. */
struct ChildModifiedEventArgs : public EventArgs {
	/* The child control that was added or removed. */
	ControlSPtr control;
	/* The index of the child control. */
	int index;

	inline ChildModifiedEventArgs()
		: EventArgs(), control(nullptr), index(-1) {}
	inline ChildModifiedEventArgs(ControlSPtr control, int index)
		: EventArgs(), control(control), index(index) {}
};
/* Shows addition or removal of an item in a collection. */
struct ItemModifiedEventArgs : public EventArgs {

	/* The item that was added or removed. */
	std::string item;
	/* The index of the item. */
	int index;

	inline ItemModifiedEventArgs()
		: EventArgs(), item(""), index(-1) {}
	inline ItemModifiedEventArgs(const std::string& item, int index)
		: EventArgs(), item(item), index(index) {}
};
/* Shows addition or removal of content from a control. */
struct ContentModifiedEventArgs : public EventArgs {
	/* The content control that was added or removed. */
	ControlSPtr control;

	inline ContentModifiedEventArgs()
		: EventArgs(), control(nullptr) {}
	inline ContentModifiedEventArgs(ControlSPtr control)
		: EventArgs(), control(control) {}
};
/* Shows change in focus. */
struct FocusChangedEventArgs : public EventArgs {
	/* The currently focused control. */
	ControlSPtr newFocus;
	/* The previously focused control. */
	ControlSPtr previouseFocus;

	inline FocusChangedEventArgs()
		: EventArgs(), newFocus(nullptr), previouseFocus(nullptr) {}
	inline FocusChangedEventArgs(ControlSPtr newFocus, ControlSPtr previouseFocus)
		: EventArgs(), newFocus(newFocus), previouseFocus(previouseFocus) {}
};
/* Shows changes to a list selection. */
struct SelectionChangedEventArgs : public EventArgs {

	/* The new selected index. */
	int newIndex;
	/* The previous selected index. */
	int previousIndex;

	inline SelectionChangedEventArgs()
		: EventArgs(), newIndex(-1), previousIndex(-1) {}
	inline SelectionChangedEventArgs(int newIndex, int previousIndex)
		: EventArgs(), newIndex(newIndex), previousIndex(previousIndex) {}
};

#pragma endregion
//--------------------------------
#pragma region WindowMovedEventArgs

/* Shows window movement. */
struct WindowMovedEventArgs : public EventArgs {
	/* The current position of the window on the screen. */
	PcGeo::Point2I newWindowPosition;
	/* The previous position of the window on the screen. */
	PcGeo::Point2I previousWindowPosition;
	/* The current client position of the window on the screen. */
	PcGeo::Point2I newClientPosition;
	/* The previous client position of the window on the screen. */
	PcGeo::Point2I previousClientPosition;

	inline WindowMovedEventArgs()
		: EventArgs(), newWindowPosition(PcGeo::Point2I::ZERO), previousWindowPosition(PcGeo::Point2I::ZERO),
		newClientPosition(PcGeo::Point2I::ZERO), previousClientPosition(PcGeo::Point2I::ZERO) {}
	inline WindowMovedEventArgs(PcGeo::Point2I newWindowPosition, PcGeo::Point2I previousWindowPosition, PcGeo::Point2I newClientPosition, PcGeo::Point2I previousClientPosition)
		: EventArgs(), newWindowPosition(newWindowPosition), previousWindowPosition(previousWindowPosition),
		newClientPosition(newClientPosition), previousClientPosition(previousClientPosition) {}
};

#pragma endregion
//--------------------------------
#pragma region ScrollEventArgs

/* Shows 2-dimensional scrollbar movement. */
struct ScrollEventArgs : public EventArgs {
	/* The current offset of the view area. */
	PcGeo::Point2I newViewPosition;
	/* The previous offset of the view area. */
	PcGeo::Point2I previousViewPosition;
	/* The current scroll thumb position. */
	PcGeo::Point2I newScrollPosition;
	/* The previous scroll thumb position. */
	PcGeo::Point2I previousScrollPosition;

	inline ScrollEventArgs()
		: EventArgs(), newViewPosition(PcGeo::Point2I::ZERO), previousViewPosition(PcGeo::Point2I::ZERO),
		newScrollPosition(PcGeo::Point2I::ZERO), previousScrollPosition(PcGeo::Point2I::ZERO) {}
	inline ScrollEventArgs(PcGeo::Point2I newViewPosition, PcGeo::Point2I previousViewPosition, PcGeo::Point2I newScrollPosition, PcGeo::Point2I previousScrollPosition)
		: EventArgs(), newViewPosition(newViewPosition), previousViewPosition(previousViewPosition),
		newScrollPosition(newScrollPosition), previousScrollPosition(previousScrollPosition) {}
};

#pragma endregion
//--------------------------------
#pragma region TextChangedEventArgs

/* Shows changes to an numeric value. */
template<typename T> struct ValueChangedEventArgs : public EventArgs {
	/* The current value. */
	T newValue;
	/* The previous value. */
	T previousValue;

	inline ValueChangedEventArgs()
		: EventArgs(), newValue((T)0), previousValue((T)0) {}
	inline ValueChangedEventArgs(T newValue, T previousValue)
		: EventArgs(), newValue(newValue), previousValue(previousValue) {}
};

typedef ValueChangedEventArgs<int> IntChangedEventArgs;
typedef ValueChangedEventArgs<float> FloatChangedEventArgs;

/* Shows changes to text. */
struct TextChangedEventArgs : public EventArgs {
	/* The current text. */
	std::string newText;
	/* The previous text. */
	std::string previousText;

	inline TextChangedEventArgs()
		: EventArgs(), newText(""), previousText("") {}
	inline TextChangedEventArgs(const std::string& newText, const std::string& previousText)
		: EventArgs(), newText(newText), previousText(previousText) {}
};
/* Shows input to the text before changes are made. This event can be handled. */
struct TextInputEventArgs : public EventArgs {
	/* The current text. */
	std::string text;
	/* The text to insert. */
	std::string input;
	/* The position to insert it at. */
	int index;
	/* The amount of text to erase first. */
	int length;

	inline TextInputEventArgs()
		: EventArgs(), text(""), input(""), index(0), length(0) {}
	inline TextInputEventArgs(const std::string& text, const std::string& input, int index, int length)
		: EventArgs(), text(text), input(input), index(index), length(length) {}
};


#pragma endregion
//--------------------------------
#pragma region CommandEventArgs


#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
