/*=================================================================|
|* File:				SplitContainer.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "MySplitContainer.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Geometry/GMath.h>
#include <AsciiVis/Colors.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

MySplitContainer::MySplitContainer()
	:	// Resizing
	separator(0),
	hovering(false),
	dragging(false),

	// Settings
	leftMinWidth(MINMAX_NONE),
	rightMinWidth(MINMAX_NONE),
	leftMaxWidth(MINMAX_NONE),
	rightMaxWidth(MINMAX_NONE),
	rightFixed(false),

	// Content
	leftContent(nullptr),
	rightContent(nullptr),

	// Events
	_eventLeftContentAdded(EventDirections::FirstInFirstCalled),
	_eventRightContentAdded(EventDirections::FirstInFirstCalled),
	_eventLeftContentRemoved(EventDirections::FirstInFirstCalled),
	_eventRightContentRemoved(EventDirections::FirstInFirstCalled) {

	eventPaint().add(bindEvent(MySplitContainer::onPaint));
	eventLayoutChanged().add(bindEvent(MySplitContainer::onLayoutChanged));
	eventMouseButton().add(bindEvent(MySplitContainer::onMouseButton));
	eventMouseMovedGlobal().add(bindEvent(MySplitContainer::onMouseMovedGlobal));
	_eventLeftContentAdded.add(bindEvent(MySplitContainer::onContentAdded));
	_eventRightContentAdded.add(bindEvent(MySplitContainer::onContentAdded));
}
MySplitContainer::~MySplitContainer() {
	if (leftContent != nullptr) {
		leftContent->setParent(nullptr);
	}
	if (rightContent != nullptr) {
		rightContent->setParent(nullptr);
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

bool MySplitContainer::containsName(const std::string& name) const {
	if (name.empty())
		return false;
	if (!getName().empty() && getName() == name)
		return true;

	if (leftContent != nullptr && leftContent->containsName(name)) {
		return true;
	}
	if (rightContent != nullptr && rightContent->containsName(name)) {
		return true;
	}
	return false;
}

int MySplitContainer::getLeftMinWidth() const {
	return leftMinWidth;
}
int MySplitContainer::getRightMinWidth() const {
	return rightMinWidth;
}
void MySplitContainer::setLeftMinWidth(int leftMinWidth) {
	this->leftMinWidth = std::max(0, leftMinWidth);
	updateSeparatorPosition();
}
void MySplitContainer::setRightMinWidth(int rightMinWidth) {
	this->rightMinWidth = std::max(0, rightMinWidth);
	updateSeparatorPosition();
}

int MySplitContainer::getLeftMaxWidth() const {
	return leftMaxWidth;
}
int MySplitContainer::getRightMaxWidth() const {
	return rightMaxWidth;
}
void MySplitContainer::setLeftMaxWidth(int leftMaxWidth) {
	this->leftMaxWidth = std::max(0, leftMaxWidth);
	updateSeparatorPosition();
}
void MySplitContainer::setRightMaxWidth(int rightMaxWidth) {
	this->rightMaxWidth = std::max(0, rightMaxWidth);
	updateSeparatorPosition();
}

bool MySplitContainer::isRightFixed() const {
	return rightFixed;
}
void MySplitContainer::setRightFixed(bool rightFixed) {
	if (rightFixed != this->rightFixed) {
		separator = getActualSize().x - 1 - separator;
		this->rightFixed = rightFixed;
		updateSeparatorPosition();
	}
}

int MySplitContainer::getLeftWidth() const {
	if (rightFixed)
		return getActualSize().x - 1 - separator;
	else
		return separator;
}
int MySplitContainer::getRightWidth() const {
	if (rightFixed)
		return separator;
	else
		return getActualSize().x - 1 - separator;
}
void MySplitContainer::setLeftWidth(int leftWidth) {
	if (rightFixed)
		separator = getActualSize().x - 1 - leftWidth;
	else
		separator = leftWidth;
	updateSeparatorPosition();
}
void MySplitContainer::setRightWidth(int rightWidth) {
	if (rightFixed)
		separator = rightWidth;
	else
		separator = getActualSize().x - 1 - rightWidth;
	updateSeparatorPosition();
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void MySplitContainer::updateContentLayout() {
	auto leftRect = Rectangle2I(0, 0, getSeparatorPosition() + 1, getActualSize().y);
	auto rightRect = Rectangle2I(getSeparatorPosition(), 0, getActualSize() - Point2I(getSeparatorPosition(), 0));
	if (leftContent != nullptr)
		leftContent->setContainerLayout(leftRect, leftRect.intersection(getLocalVisibleArea()));
	if (rightContent != nullptr)
		rightContent->setContainerLayout(rightRect, rightRect.intersection(getLocalVisibleArea()));
}

void MySplitContainer::forEach(std::function<void(ControlSPtr control)> func) {
	func(shared_from_this());
	if (leftContent != nullptr) {
		leftContent->forEach(func);
	}
	if (rightContent != nullptr) {
		rightContent->forEach(func);
	}
}
void MySplitContainer::repaint(GraphicsSPtr g) {
	g->startRegion(getLocalPosition(), getLocalVisibleArea());
	eventPaint()(shared_from_this(), PaintEventArgs(g));

	if (leftContent != nullptr) {
		if (leftContent->isVisible())
			leftContent->repaint(g);
	}
	g->resetOptions();
	if (rightContent != nullptr) {
		if (rightContent->isVisible())
			rightContent->repaint(g);
	}
	g->resetOptions();

	const unsigned char frameColor = toColor(BLACK, DARK_DARK_NAVY);
	const unsigned char hoverColor = toColor(BLACK, NAVY);

	unsigned char color = (hovering || dragging ? hoverColor : frameColor);

	g->setColor(Rectangle2I(getSeparatorPosition(), 0, 1, getActualSize().y), color);

	g->endRegion();
}

void MySplitContainer::updateSeparatorPosition() {
	int leftMax = std::min(getActualSize().x - 1, (leftMaxWidth == MINMAX_NONE ? std::numeric_limits<int>().max() : leftMaxWidth));
	int rightMax = std::min(getActualSize().x - 1, (rightMaxWidth == MINMAX_NONE ? std::numeric_limits<int>().max() : rightMaxWidth));
	if (rightFixed) {
		int leftSeparator = getActualSize().x - 1 - separator;
		if (separator > rightMax) {
			separator = rightMax;
		}
		else if (separator < rightMinWidth) {
			separator = rightMinWidth;
		}
		else if (leftSeparator > leftMax) {
			separator = GMath::clamp(getActualSize().x - 1 - leftMax, rightMinWidth, rightMax);
		}
		else if (leftSeparator < leftMinWidth) {
			separator = GMath::clamp(getActualSize().x - 1 - leftMinWidth, rightMinWidth, rightMax);
		}
		leftSeparator = getActualSize().x - 1 - separator;
	}
	else {
		int rightSeparator = getActualSize().x - 1 - separator;
		if (separator > leftMax) {
			separator = leftMax;
		}
		else if (separator < leftMinWidth) {
			separator = leftMinWidth;
		}
		else if (rightSeparator > rightMax) {
			separator = GMath::clamp(getActualSize().x - 1 - rightMax, leftMinWidth, leftMax);
		}
		else if (rightSeparator < rightMinWidth) {
			separator = GMath::clamp(getActualSize().x - 1 - rightMinWidth, leftMinWidth, leftMax);
		}
		rightSeparator = getActualSize().x - 1 - separator;
	}
	updateContentLayout();
}
int MySplitContainer::getSeparatorPosition() const {
	if (rightFixed)
		return getActualSize().x - 1 - separator;
	else
		return separator;
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

ControlSPtr MySplitContainer::getLeftContent() {
	return leftContent;
}
ControlSPtr MySplitContainer::getRightContent() {
	return rightContent;
}
void MySplitContainer::setLeftContent(ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'SplitContainer::setLeftContent(...)'");
	if (control->hasParent())
		throw std::exception("Control already has parent in 'SplitContainer::setLeftContent(...)'");

	if (leftContent != nullptr) {
		removeLeftContent();
	}
	control->setParent(shared_from_this());
	leftContent = control;

	_eventLeftContentAdded(shared_from_this(), ContentModifiedEventArgs(control));
}
void MySplitContainer::setRightContent(ControlSPtr control) {
	if (control == nullptr)
		throw std::invalid_argument("Null pointer exception in 'SplitContainer::setRightContent(...)'");
	if (control->hasParent())
		throw std::exception("Control already has parent in 'SplitContainer::setRightContent(...)'");

	if (rightContent != nullptr) {
		removeRightContent();
	}
	control->setParent(shared_from_this());
	rightContent = control;

	_eventRightContentAdded(shared_from_this(), ContentModifiedEventArgs(control));
}
void MySplitContainer::removeLeftContent() {
	if (leftContent == nullptr)
		throw std::exception("No content to remove in 'SplitContainer::removeLeftContent()'");

	ControlSPtr removedContent = leftContent;
	leftContent->setParent(nullptr);
	leftContent = nullptr;

	_eventLeftContentRemoved(shared_from_this(), ContentModifiedEventArgs(removedContent));
}
void MySplitContainer::removeRightContent() {
	if (rightContent == nullptr)
		throw std::exception("No content to remove in 'SplitContainer::removeRightContent()'");

	ControlSPtr removedContent = rightContent;
	rightContent->setParent(nullptr);
	rightContent = nullptr;

	_eventRightContentRemoved(shared_from_this(), ContentModifiedEventArgs(removedContent));
}

bool MySplitContainer::hasLeftContent() const {
	return leftContent != nullptr;
}
bool MySplitContainer::hasRightContent() const {
	return rightContent != nullptr;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<ContentModifiedEventArgs>& MySplitContainer::eventLeftContentAdded() {
	return _eventLeftContentAdded;
}
Event<ContentModifiedEventArgs>& MySplitContainer::eventRightContentAdded() {
	return _eventRightContentAdded;
}
Event<ContentModifiedEventArgs>& MySplitContainer::eventLeftContentRemoved() {
	return _eventLeftContentRemoved;
}
Event<ContentModifiedEventArgs>& MySplitContainer::eventRightContentRemoved() {
	return _eventRightContentRemoved;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void MySplitContainer::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	g->setConnectGlobalBorders(true);
	g->drawBorder(Rectangle2I(getSeparatorPosition(), 0, 1, getActualSize().y), Pixel(' ', frameColor), true);
	g->resetOptions();
}
void MySplitContainer::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		updateSeparatorPosition();
	}
	else if (e.visibleAreaChanged) {
		updateContentLayout();
	}
}
void MySplitContainer::onContentAdded(ControlSPtr sender, ContentModifiedEventArgs& e) {
	updateContentLayout();
}
void MySplitContainer::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (hovering && e.isButtonPressed(MouseButtons::Left)) {
		dragging = true;
	}
	else if (e.isButtonReleased(MouseButtons::Left)) {
		dragging = false;
	}
}
void MySplitContainer::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
	hovering = (e.newPosition.x == getSeparatorPosition() && isMouseInside());
	if (dragging) {
		if (e.newPosition.x != getSeparatorPosition()) {
			if (rightFixed)
				separator = getActualSize().x - 1 - e.newPosition.x;
			else
				separator = e.newPosition.x;
			updateSeparatorPosition();
		}
	}
}

#pragma endregion
//=================================================================|
