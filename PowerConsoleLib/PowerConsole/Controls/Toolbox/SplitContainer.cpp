/*=================================================================|
|* File:				SplitContainer.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "SplitContainer.h"
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Geometry/GMath.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

SplitContainer::SplitContainer() 
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

	eventPaint().add(bindEvent(SplitContainer::onPaint));
	eventLayoutChanged().add(bindEvent(SplitContainer::onLayoutChanged));
	eventMouseButton().add(bindEvent(SplitContainer::onMouseButton));
	eventMouseMovedGlobal().add(bindEvent(SplitContainer::onMouseMovedGlobal));
	_eventLeftContentAdded.add(bindEvent(SplitContainer::onContentAdded));
	_eventRightContentAdded.add(bindEvent(SplitContainer::onContentAdded));
}
SplitContainer::~SplitContainer() {
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

bool SplitContainer::containsName(const std::string& name) const {
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

int SplitContainer::getLeftMinWidth() const {
	return leftMinWidth;
}
int SplitContainer::getRightMinWidth() const {
	return rightMinWidth;
}
void SplitContainer::setLeftMinWidth(int leftMinWidth) {
	this->leftMinWidth = std::max(0, leftMinWidth);
	updateSeparatorPosition();
}
void SplitContainer::setRightMinWidth(int rightMinWidth) {
	this->rightMinWidth = std::max(0, rightMinWidth);
	updateSeparatorPosition();
}

int SplitContainer::getLeftMaxWidth() const {
	return leftMaxWidth;
}
int SplitContainer::getRightMaxWidth() const {
	return rightMaxWidth;
}
void SplitContainer::setLeftMaxWidth(int leftMaxWidth) {
	this->leftMaxWidth = std::max(0, leftMaxWidth);
	updateSeparatorPosition();
}
void SplitContainer::setRightMaxWidth(int rightMaxWidth) {
	this->rightMaxWidth = std::max(0, rightMaxWidth);
	updateSeparatorPosition();
}

bool SplitContainer::isRightFixed() const {
	return rightFixed;
}
void SplitContainer::setRightFixed(bool rightFixed) {
	if (rightFixed != this->rightFixed) {
		separator = getActualSize().x - 1 - separator;
		this->rightFixed = rightFixed;
		updateSeparatorPosition();
	}
}

int SplitContainer::getLeftWidth() const {
	if (rightFixed)
		return getActualSize().x - 1 - separator;
	else
		return separator;
}
int SplitContainer::getRightWidth() const {
	if (rightFixed)
		return separator;
	else
		return getActualSize().x - 1 - separator;
}
void SplitContainer::setLeftWidth(int leftWidth) {
	if (rightFixed)
		separator = getActualSize().x - 1 - leftWidth;
	else
		separator = leftWidth;
	updateSeparatorPosition();
}
void SplitContainer::setRightWidth(int rightWidth) {
	if (rightFixed)
		separator = rightWidth;
	else
		separator = getActualSize().x - 1 - rightWidth;
	updateSeparatorPosition();
}

#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void SplitContainer::updateContentLayout() {
	auto leftRect = Rectangle2I(0, 0, getSeparatorPosition() + 1, getActualSize().y);
	auto rightRect = Rectangle2I(getSeparatorPosition(), 0, getActualSize() - Point2I(getSeparatorPosition(), 0));
	if (leftContent != nullptr)
		leftContent->setContainerLayout(leftRect, leftRect.intersection(getLocalVisibleArea()));
	if (rightContent != nullptr)
		rightContent->setContainerLayout(rightRect, rightRect.intersection(getLocalVisibleArea()));
}

void SplitContainer::forEach(std::function<void(ControlSPtr control)> func) {
	func(shared_from_this());
	if (leftContent != nullptr) {
		leftContent->forEach(func);
	}
	if (rightContent != nullptr) {
		rightContent->forEach(func);
	}
}
void SplitContainer::repaint(GraphicsSPtr g) {
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

	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);
	const unsigned char hoverColor = toColor(BLACK, GRAY);

	unsigned char color = (hovering || dragging ? hoverColor : frameColor);

	g->setColor(Rectangle2I(getSeparatorPosition(), 0, 1, getActualSize().y), color);

	g->endRegion();
}

void SplitContainer::updateSeparatorPosition() {
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
int SplitContainer::getSeparatorPosition() const {
	if (rightFixed)
		return getActualSize().x - 1 - separator;
	else
		return separator;
}

#pragma endregion
//=========== ELEMENTS ===========
#pragma region Elements

ControlSPtr SplitContainer::getLeftContent() {
	return leftContent;
}
ControlSPtr SplitContainer::getRightContent() {
	return rightContent;
}
void SplitContainer::setLeftContent(ControlSPtr control) {
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
void SplitContainer::setRightContent(ControlSPtr control) {
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
void SplitContainer::removeLeftContent() {
	if (leftContent == nullptr)
		throw std::exception("No content to remove in 'SplitContainer::removeLeftContent()'");

	ControlSPtr removedContent = leftContent;
	leftContent->setParent(nullptr);
	leftContent = nullptr;

	_eventLeftContentRemoved(shared_from_this(), ContentModifiedEventArgs(removedContent));
}
void SplitContainer::removeRightContent() {
	if (rightContent == nullptr)
		throw std::exception("No content to remove in 'SplitContainer::removeRightContent()'");

	ControlSPtr removedContent = rightContent;
	rightContent->setParent(nullptr);
	rightContent = nullptr;

	_eventRightContentRemoved(shared_from_this(), ContentModifiedEventArgs(removedContent));
}

bool SplitContainer::hasLeftContent() const {
	return leftContent != nullptr;
}
bool SplitContainer::hasRightContent() const {
	return rightContent != nullptr;
}

#pragma endregion
//============ EVENTS ============
#pragma region Events

Event<ContentModifiedEventArgs>& SplitContainer::eventLeftContentAdded() {
	return _eventLeftContentAdded;
}
Event<ContentModifiedEventArgs>& SplitContainer::eventRightContentAdded() {
	return _eventRightContentAdded;
}
Event<ContentModifiedEventArgs>& SplitContainer::eventLeftContentRemoved() {
	return _eventLeftContentRemoved;
}
Event<ContentModifiedEventArgs>& SplitContainer::eventRightContentRemoved() {
	return _eventRightContentRemoved;
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void SplitContainer::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char frameColor = toColor(BLACK, DARK_GRAY);

	g->setConnectGlobalBorders(true);
	g->drawBorder(Rectangle2I(getSeparatorPosition(), 0, 1, getActualSize().y), Pixel(' ', frameColor), true);
	g->resetOptions();
}
void SplitContainer::onLayoutChanged(ControlSPtr sender, LayoutChangedEventArgs& e) {
	if (e.sizeChanged) {
		updateSeparatorPosition();
	}
	else if (e.visibleAreaChanged){
		updateContentLayout();
	}
}
void SplitContainer::onContentAdded(ControlSPtr sender, ContentModifiedEventArgs& e) {
	updateContentLayout();
}
void SplitContainer::onMouseButton(ControlSPtr sender, MouseButtonEventArgs& e) {
	if (hovering && e.isButtonPressed(MouseButtons::Left)) {
		dragging = true;
	}
	else if (e.isButtonReleased(MouseButtons::Left)) {
		dragging = false;
	}
}
void SplitContainer::onMouseMovedGlobal(ControlSPtr sender, MouseEventArgs& e) {
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
