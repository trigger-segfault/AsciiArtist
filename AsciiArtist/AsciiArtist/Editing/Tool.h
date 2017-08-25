/*=================================================================|
|* File:				Tool.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>
#include <PowerConsole/Geometry/Rectangle.h>
#include <AsciiArtist/Toolbox/Controls/Canvas.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace AsciiArtist::Toolbox;
using namespace AsciiArtist::Toolbox::Controls;

namespace AsciiArtist::Editing {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Tool {
	//========== CONSTANTS ===========
	#pragma region Constants


	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members
protected:
	/* The canvas containing this tool. */
	CanvasWPtr canvas;
	/* The name of the tool. */
	std::string toolName;
	/* True if the tool is currently drawing. */
	bool drawing;
	/* The invalidated region of the drawing buffer. */
	Rectangle2I invalidatedRegion;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the tool. */
	Tool(CanvasSPtr canvas, const std::string& name);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the name of the tool. */
	std::string getToolName();
	/* Gets if the tool is currently drawing. */
	bool isDrawing() const;
	/* Gets the invalidated region of the drawing buffer. */
	Rectangle2I getInvalidatedRegion() const;
	/* Gets the options used by the tool. */
	virtual fixed_array<std::string> getUsedOptions() const;
	/* Gets the selection size of teh tool. */
	virtual Point2I getSelectionSize() const;

	#pragma endregion
	//=========== EDITING ============
	#pragma region Editing

	/* Finishes drawing with the tool. */
	virtual void finalize() = 0;
	/* Cancels drawing with the tool. */
	virtual void cancel() = 0;
	/* Handles a selected character. Returns true if handled by the tool. */
	virtual bool characterSelected(unsigned char character, bool transparent);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers

	virtual void onMouseButton(MouseButtonEventArgs e) = 0;
	virtual void onMouseMoved(MouseEventArgs e);
	virtual void onKey(KeyboardEventArgs e);
	virtual void onKeyTyped(KeyTypedEventArgs e);
	virtual void onTick(TickEventArgs e);
	virtual void onPaint(PaintEventArgs e);

	#pragma endregion
};

DECLARE_SMART_PTRS(Tool);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
