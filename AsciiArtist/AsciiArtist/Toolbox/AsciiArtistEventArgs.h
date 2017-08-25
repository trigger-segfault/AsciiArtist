/*=================================================================|
|* File:				AAEventArgs.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef AA_EVENT_ARGS_H
#define AA_EVENT_ARGS_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Events/EventArgs.h>
#include <PowerConsole/Helpers/fixed_array.h>

using namespace PowerConsole::Helpers;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Geometry;

namespace AsciiArtist::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

struct MessageEventArgs : public EventArgs {
	/* The message to show. */
	std::string message;
	/* The color of the message. */
	unsigned char color;

	inline MessageEventArgs()
		: message(""), color(0x07) {}
	inline MessageEventArgs(const std::string& message, unsigned char color = 0x07)
		: message(message), color(color) {}
};

struct StatusBarDimensionsEventArgs : public EventArgs {
	/* The new point to show in the status bar. */
	Point2I point;
	/* The new size to show in the status bar. */
	Point2I size;

	inline StatusBarDimensionsEventArgs()
		: point(-Point2I::ONE), size(-Point2I::ONE) {}
	inline StatusBarDimensionsEventArgs(Point2I point, Point2I size)
		: point(point), size(size) {}
};

struct FrameInfo {
	/* The name of the frame. */
	std::string name;
	/* The speed of the frame. */
	unsigned int frameSpeed;

	inline FrameInfo()
		: name(""), frameSpeed(0U) {}
	inline FrameInfo(const std::string& name, unsigned int frameSpeed)
		: name(name), frameSpeed(frameSpeed) {}
};

struct FrameModifiedEventArgs : public EventArgs {
	/* The new information of the modified frame. */
	FrameInfo frame;
	/* The index of the modified frame. */
	int index;

	inline FrameModifiedEventArgs()
		: index(0), frame() {}
	inline FrameModifiedEventArgs(FrameInfo frame, int index)
		: index(index), frame(frame) {}
};
struct FrameMovedEventArgs : public EventArgs {
	/* The index of the moved frame. */
	int index;
	/* The new index of the moved frame. */
	int newIndex;

	inline FrameMovedEventArgs()
		: index(0), newIndex(0) {}
	inline FrameMovedEventArgs(int index, int newIndex)
		: index(index), newIndex(newIndex) {}
};

struct FrameListChangedEventArgs : public EventArgs {
	/* The new list of frame infos. */
	fixed_array<FrameInfo> names;

	FrameListChangedEventArgs()
		: names() {}
	FrameListChangedEventArgs(const fixed_array<FrameInfo>& names)
		: names(names) {}
	FrameListChangedEventArgs(fixed_array<FrameInfo>&& names)
		: names(std::move(names)) {}
};

struct UndoAvailabilityEventArgs : public EventArgs {
	/* True if undos are available. */
	bool undoAvailable;
	/* True if redos are available. */
	bool redoAvailable;

	inline UndoAvailabilityEventArgs()
		: undoAvailable(false), redoAvailable(false) {}
	inline UndoAvailabilityEventArgs(bool undoAvailable, bool redoAvailable)
		: undoAvailable(undoAvailable), redoAvailable(redoAvailable) {}
};

struct PixelChangedEventArgs : public EventArgs {
	/* The new pixel. */
	Pixel pixel;
	/* True if this is the secondary pixel. */
	bool secondary;

	inline PixelChangedEventArgs()
		: pixel(), secondary(false) {}

	inline PixelChangedEventArgs(Pixel pixel, bool secondary)
		: pixel(pixel), secondary(secondary) {}
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
