/*=================================================================|
|* File:				ChartWrapPanel.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef CHART_WRAP_PANEL_H
#define CHART_WRAP_PANEL_H

#include <PowerConsole/Helpers/fixed_array.h>
//#include <PowerConsole/Controls/ContainerControl.h>
#include <PowerConsole/Controls/Control.h>

//using namespace PowerConsole;
//using namespace PowerConsole::Geometry;
//using namespace PowerConsole::Helpers;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiChart {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Allows access to the graphics for drawing. */
struct PaintItemEventArgs : public PaintEventArgs {
	/* The item number to paint. */
	int itemIndex;

	inline PaintItemEventArgs()
		: PaintEventArgs(nullptr), itemIndex(0) {}
	inline PaintItemEventArgs(PcDraw::GraphicsSPtr graphics, int itemIndex)
		: PaintEventArgs(graphics), itemIndex(itemIndex) {}
};

/* A control for managing left-to-right/top-to-bottom wrapping of spaced items. */
class ChartWrapPanel : public Control {

	//=========== MEMBERS ============
	#pragma region Members
private:
	/* The orientation of the chart wrap panel (vertical = left->right->top->bottom). */
	PcGeo::Orientations orientation;
	/* The extra spacing between items. */
	PcGeo::Point2I spacing;
	/* The size of each item. */
	PcGeo::Point2I itemSize;
	/* The number of items to manage the panel. */
	int itemCount;
	bool uniformSize;

	PcHelp::fixed_array<PcGeo::Point2I> individualSizes;

	// Events
	/* Called to draw each item in the control. */
	Event<PaintItemEventArgs> _eventPaintItem;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the chart wrap panel. */
	ChartWrapPanel();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
	//--------------------------------
	#pragma region General

	/* Gets the orientation of the chart wrap panel (vertical = left->right->top->bottom). */
	PcGeo::Orientations getOrientation() const;
	/* Sets the orientation of the chart wrap panel (vertical = left->right->top->bottom). */
	void setOrientation(PcGeo::Orientations orientation);
	/* Gets the extra spacing between items. */
	PcGeo::Point2I getSpacing() const;
	/* Sets the extra spacing between items. */
	void setSpacing(PcGeo::Point2I spacing);

	/* Gets the item size when all sizes are uniform. (returns 1x1 when there are no items) */
	PcGeo::Point2I getUniformItemSize() const;
	/* Sets the size of each item. */
	//void setItemSize(PcGeo::Point2I itemSize);
	/* Gets the individual sizes of every item. */
	const PcHelp::fixed_array<PcGeo::Point2I>& getIndividualItems() const;
	/* Sets individual sizes for every item. */
	void setIndividualItems(const PcHelp::fixed_array<PcGeo::Point2I>& individualSizes);
	/* Returns true if all items are of the same size. (or if there are no items) */
	bool isUniformSize() const;
	/* Sets a uniform size for the specified number of items. */
	void setUniformItems(PcGeo::Point2I itemSize, int count);
	/* Gets the number of items in the chart to draw. */
	int getItemCount() const;
	/* Sets the number of items in the chart to draw. */
	//void setItemCount(int itemCount);
	/* Gets if the chart has no items in it. */
	bool isEmpty() const;

	#pragma endregion
	//--------------------------------
	#pragma region Container Layout

	/* Calculates the number of columns/rows needed to draw all items. */
	PcGeo::Point2I getGridDimensions() const;

	#pragma endregion
	//--------------------------------
	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const override;
	/* Draws the control to the graphics object. */
	void repaint(PcDraw::GraphicsSPtr g) override;
private:
	/* Calculates the number of columns/rows needed to draw all items for the available size. */
	PcGeo::Point2I calculateGridDimensions(PcGeo::Point2I& availableSize) const;
public:

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events

	// Update
	/* Called to draw a single item in the chart. */
	Event<PaintItemEventArgs>& eventPaintItem();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(ChartWrapPanel);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
