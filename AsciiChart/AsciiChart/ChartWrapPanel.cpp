/*=================================================================|
|* File:				ChartWrapPanel.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "ChartWrapPanel.h"
#include <PowerConsole/Geometry/GMath.h>

using namespace AsciiChart;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::Controls;
//using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

ChartWrapPanel::ChartWrapPanel()
 :	orientation(Orientations::Vertical),
	spacing(Point2I::ZERO),
	itemSize(Point2I::ONE),
	individualSizes(),
	uniformSize(true),
	itemCount(0),
	// Events
	_eventPaintItem(EventDirections::FirstInFirstCalled) {


	eventInitialize().add(bindEvent(ChartWrapPanel::onInitialize));
	//eventLayoutChanged().add(bindEvent(ChartWrapPanel::onLayoutChanged));
}
void ChartWrapPanel::onInitialize(ControlSPtr sender, EventArgs& e) {
	setSize(Point2I(SIZE_AUTO));
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region General

Orientations ChartWrapPanel::getOrientation() const {
	return orientation;
}
void ChartWrapPanel::setOrientation(Orientations orientation) {
	this->orientation = orientation;
}
Point2I ChartWrapPanel::getSpacing() const {
	return spacing;
}
void ChartWrapPanel::setSpacing(Point2I spacing) {
	if (!(spacing >= Point2I::ZERO))
		throw std::exception("Spacing size is less than 0x0");
	this->spacing = spacing;
}

Point2I ChartWrapPanel::getUniformItemSize() const {
	if (!uniformSize)
		throw std::exception("Item sizes are not uniform");
	return itemSize;
}
const PcHelp::fixed_array<PcGeo::Point2I>& ChartWrapPanel::getIndividualItems() const {
	return this->individualSizes;
}
void ChartWrapPanel::setIndividualItems(const PcHelp::fixed_array<PcGeo::Point2I>& individualSizes) {
	bool uniformSize = true;
	if (!individualSizes.empty()) {
		Point2I maxSize = individualSizes[0];
		// include first element in loop to do valid size check
		for (int i = 0; i < (int)individualSizes.size(); i++) {
			Point2I size = individualSizes[i];
			if (!(size >= Point2I::ONE))
				throw std::exception("An individual item size is less than 1x1");

			if (size != maxSize) {
				uniformSize = false;
				maxSize = GMath::max(size, maxSize);
			}
		}
		this->itemSize = maxSize; // used to store maximum dimensions
	}
	else {
		this->itemSize = Point2I::ONE; // default when no items
	}
	this->itemCount = (int)individualSizes.size();
	this->individualSizes = individualSizes;
	this->uniformSize = uniformSize;
}

bool ChartWrapPanel::isUniformSize() const {
	return uniformSize;
}

void ChartWrapPanel::setUniformItems(Point2I itemSize, int itemCount) {
	if (itemCount < 0)
		throw std::exception("Item count is less than zero");
	if (itemCount > 0) {
		if (!(itemSize >= Point2I::ONE))
			throw std::exception("Item size is less than 1x1");
		this->itemSize = itemSize;
		this->individualSizes = fixed_array<Point2I>(itemCount);
		for (int i = 0; i < itemCount; i++) {
			this->individualSizes[i] = itemSize;
		}
	}
	else {
		this->itemSize = Point2I::ONE; // default when no items
	}
	this->itemCount = itemCount;
	this->uniformSize = true;
}
int ChartWrapPanel::getItemCount() const {
	return itemCount;
}
bool ChartWrapPanel::isEmpty() const {
	return itemCount == 0;
}

#pragma endregion
//--------------------------------
#pragma region Container Layout

Point2I ChartWrapPanel::getGridDimensions() const {
	if (itemCount == 0)
		return Point2I::ZERO;
	if (itemSize == Point2I::ZERO && spacing == Point2I::ZERO)
		return Point2I::ZERO;

	Point2I availableSize = getActualSize();
	return calculateGridDimensions(availableSize);
}
Point2I ChartWrapPanel::calculateGridDimensions(Point2I& availableSize) const {
	if (itemCount == 0)
		return Point2I::ZERO;
	if (itemSize == Point2I::ZERO && spacing == Point2I::ZERO)
		return Point2I::ZERO;

	Point2I sizeTaken;
	if (orientation == Orientations::Horizontal) {
		int rows = (availableSize.y + spacing.y) / (itemSize.y + spacing.y);
		if (rows <= 0)
			rows = 1; // avoid divide-by-zero (force minimum space for one item)

		sizeTaken.y = rows * (itemSize.y + spacing.y) - spacing.y;
		if (!uniformSize && rows > 0 && rows < itemCount) {
			// check if one more row can be squeazed in
			// only calculate extra space from trimming final row
			// (this way spacing between each row is uniform)
			int trimmedEdge = 0;
			for (int j = rows; j < itemCount; j += rows + 1) {
				trimmedEdge = GMath::max(trimmedEdge, individualSizes[j].y);
			}

			if (rows * (itemSize.y + spacing.y) + trimmedEdge <= availableSize.y) {
				rows++; // we have room!
				sizeTaken.y += spacing.y + trimmedEdge;
			}
			else {
				// go one step further, we need to shrink to the minimum size with
				// the current number of rows, otherwise there'll be spots where
				// the centered control shifts to the left when you're increasing available area.
				trimmedEdge = 0;
				for (int j = rows - 1; j < itemCount; j += rows) {
					trimmedEdge = GMath::max(trimmedEdge, individualSizes[j].y);
				}
				if ((rows - 1) * (itemSize.y + spacing.y) + trimmedEdge <= availableSize.y) {
					// we have room!
					sizeTaken.y = (rows - 1) * (itemSize.y + spacing.y) + trimmedEdge;
				}
			}
		}
		int cols = (itemCount + rows - 1) / rows;
		sizeTaken.x = cols * (itemSize.x + spacing.x) - spacing.x;
		availableSize = sizeTaken;
		return Point2I(cols, rows);
	}
	else {
		int cols = (availableSize.x + spacing.x) / (itemSize.x + spacing.x);
		if (cols <= 0)
			cols = 1; // avoid divide-by-zero (force minimum space for one item)

		sizeTaken.x = cols * (itemSize.x + spacing.x) - spacing.x;
		if (!uniformSize && cols > 0 && cols < itemCount) {
			// check if one more column can be squeazed in
			// only calculate extra space from trimming final column
			// (this way spacing between each column is uniform)
			int trimmedEdge = 0;
			for (int j = cols; j < itemCount; j += cols + 1) {
				trimmedEdge = GMath::max(trimmedEdge, individualSizes[j].x);
			}

			if (cols * (itemSize.x + spacing.x) + trimmedEdge <= availableSize.x) {
				cols++; // we have room!
				sizeTaken.x += spacing.x + trimmedEdge;
			}
			else {
				// go one step further, we need to shrink to the minimum size with
				// the current number of columns, otherwise there'll be spots where
				// the centered control shifts to the left when you're increasing available area.
				trimmedEdge = 0;
				for (int j = cols-1; j < itemCount; j += cols) {
					trimmedEdge = GMath::max(trimmedEdge, individualSizes[j].x);
				}
				if ((cols - 1) * (itemSize.x + spacing.x) + trimmedEdge <= availableSize.x) {
					// we have room!
					sizeTaken.x = (cols - 1) * (itemSize.x + spacing.x) + trimmedEdge;
				}
			}
		}
		int rows = (itemCount + cols - 1) / cols;
		sizeTaken.y = rows * (itemSize.y + spacing.y) - spacing.y;
		availableSize = sizeTaken;
		return Point2I(cols, rows);
	}
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

Point2I ChartWrapPanel::autoSizeBehavior() const {
	//int size = 0;
	if (itemCount == 0)
		return Point2I::ZERO;

	Point2I availableSize = getContainerSize() - getMargin().horizontalVertical();// -1;
	if (getSize().x != SIZE_AUTO)
		availableSize.x = getSize().x;
	if (getSize().y != SIZE_AUTO)
		availableSize.y = getSize().y;

	Point2I dimensions = calculateGridDimensions(availableSize);
	dimensions = GMath::max(Point2I::ZERO, dimensions * (itemSize + spacing) - spacing);
	return availableSize + Point2I::ONE;
}
void ChartWrapPanel::repaint(GraphicsSPtr g) {
	g->startRegion(getLocalPosition(), getLocalVisibleArea());
	eventPaint()(shared_from_this(), PaintEventArgs(g));
	g->resetOptions();

	Rectangle2I visibleArea = getLocalVisibleArea();
	Point2I dimensions = getGridDimensions();
	if (orientation == Orientations::Horizontal) // swap and always treat width as primary size
		dimensions = Point2I(dimensions.y, dimensions.x);

	for (int i = 0; i < itemCount; i++) {
		Point2I location = Point2I(i % dimensions.x, i / dimensions.x);
		if (orientation == Orientations::Horizontal) // swap back to normal axes
			location = Point2I(location.y, location.x);
		Rectangle2I bounds = Rectangle2I(location * (itemSize + spacing), itemSize);
		Rectangle2I visible = bounds.intersection(visibleArea);
		if (visible.size != Point2I::ZERO) {
			g->setExtraTranslation(bounds.point);
			//g->startRegion(bounds.point, visible);
			eventPaintItem()(shared_from_this(), PaintItemEventArgs(g, i));
			g->setExtraTranslation(Point2I(0, 0));
			//g->resetOptions();
			//g->endRegion();
			//g->resetOptions();
		}
	}

	g->endRegion();
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

// Update
Event<PaintItemEventArgs>& ChartWrapPanel::eventPaintItem() {
	return _eventPaintItem;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

//void ChartWrapPanel::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {}

#pragma endregion
//=================================================================|
