/*=================================================================|
|* File:				ComponentScrollBar.cpp					  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		9/12/2014								  *|
|*																  *|
|* Description: The base class for components in the application. *|
|=================================================================*/

#include "Stdafx.h"
#include "ComponentScrollBar.h"
#include "ConsoleApp.h"
#include "Frame.h"
#include "AsciiImage.h"

using namespace std;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::Components;

//=================================================================|
// CLASSES														   |
//=================================================================/
#pragma region Classes
//=================================================================|
#pragma region ComponentScrollBar
//========= CONSTRUCTORS =========
#pragma region Constructors

PowerConsole::Components::ComponentScrollBar::ComponentScrollBar() : Component() {
	// Dimensions
	this->_scrollSize			= Coord::Zero;
	this->_barSize				= Coord::Zero;
	this->_listViewSize			= Coord::Zero;
	this->_listSize				= Coord::Zero;
	this->_scrollRatio			= Point2D::Zero;
	
	// Dragging
	this->_listPosition			= Coord::Zero;
	this->_scrollPosition		= Coord::Zero;
	this->_gripPosition			= 0;
	this->_dragging				= 0;
	this->_hovering				= 0;

	// State
	this->_verticalActive		= false;
	this->_horizontalActive		= false;
	this->_verticalEnabled		= false;
	this->_horizontalEnabled	= false;
	this->_mouseWheelEnabled	= false;
	this->_listPositionChanged	= false;
}
PowerConsole::Components::ComponentScrollBar::~ComponentScrollBar() {
	
}
void PowerConsole::Components::ComponentScrollBar::Initialize(ConsoleApp* app, Frame* frame) {
	Component::Initialize(app, frame);
}
void PowerConsole::Components::ComponentScrollBar::Uninitialize() {
	Component::Uninitialize();
}
void PowerConsole::Components::ComponentScrollBar::OnEnter() {
	Component::OnEnter();
}
void PowerConsole::Components::ComponentScrollBar::OnLeave() {
	Component::OnLeave();
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Dragging

Coord PowerConsole::Components::ComponentScrollBar::GetListPosition() const {
	return _listPosition;
}
Coord PowerConsole::Components::ComponentScrollBar::GetListSize() const {
	return _listSize;
}
void PowerConsole::Components::ComponentScrollBar::SetListPosition(Coord position) {
	if (_verticalActive) {
		_listPosition.Y = max(0, min(position.Y, _listSize.Y - _listViewSize.Y));
		_scrollPosition.Y = int(_listPosition.Y * _scrollRatio.Y);
	}
	if (_horizontalActive) {
		_listPosition.X = max(0, min(position.X, _listSize.X - _listViewSize.X));
		_scrollPosition.X = int(_listPosition.X * _scrollRatio.X);
	}
}
void PowerConsole::Components::ComponentScrollBar::SetScrollPosition(Coord position) {
	if (_verticalActive) {
		_scrollPosition.Y = position.Y;
		if (_scrollPosition.Y < 0) {
			_scrollPosition.Y = 0;
			_listPosition.Y = 0;
		}
		else if (_scrollPosition.Y >= _scrollSize.Y - _barSize.Y) {
			_scrollPosition.Y = _scrollSize.Y - _barSize.Y;
			_listPosition.Y = _listSize.Y - _listViewSize.Y;
		}
		else {
			_listPosition.Y = int(_scrollPosition.Y / _scrollRatio.Y);
		}
	}
	if (_horizontalActive) {
		_scrollPosition.X = position.X;
		if (_scrollPosition.X < 0) {
			_scrollPosition.X = 0;
			_listPosition.X = 0;
		}
		else if (_scrollPosition.X >= _scrollSize.X - _barSize.X) {
			_scrollPosition.X = _scrollSize.X - _barSize.X;
			_listPosition.X = _listSize.X - _listViewSize.X;
		}
		else {
			_listPosition.X = int(_scrollPosition.X / _scrollRatio.X);
		}
	}
}
Coord PowerConsole::Components::ComponentScrollBar::GetMaxListPosition() const {
	return _listSize - _listViewSize - Coord::One;
}

#pragma endregion
//--------------------------------
#pragma region State

bool PowerConsole::Components::ComponentScrollBar::IsVerticalBarActive() const {
	return _verticalActive;
}
bool PowerConsole::Components::ComponentScrollBar::IsHorizontalBarActive() const {
	return _horizontalActive;
}
bool PowerConsole::Components::ComponentScrollBar::IsVerticalBarEnabled() const {
	return _verticalEnabled;
}
void PowerConsole::Components::ComponentScrollBar::SetVerticalBarEnabled(bool enabled) {
	this->_verticalEnabled = enabled;
}
bool PowerConsole::Components::ComponentScrollBar::IsHorizontalBarEnabled() const {
	return _horizontalEnabled;
}
void PowerConsole::Components::ComponentScrollBar::SetHorizontalBarEnabled(bool enabled) {
	this->_horizontalEnabled = enabled;
}
bool PowerConsole::Components::ComponentScrollBar::IsMouseWheelEnabled() const {
	return _mouseWheelEnabled;
}
void PowerConsole::Components::ComponentScrollBar::SetMouseWheelEnabled(bool enabled) {
	this->_mouseWheelEnabled = enabled;
}

#pragma endregion
//--------------------------------
#pragma region Dimensions

void PowerConsole::Components::ComponentScrollBar::Resize(Coord listViewSize, Coord listSize, bool keepListPosition) {
	_scrollSize = listViewSize - Coord(4, 4);
	_listViewSize = listViewSize;
	_listSize = listSize;
	_verticalActive = false;
	_horizontalActive = false;

	_size = listViewSize;

	// Check if the scrollbars are required.
	if (_listViewSize.Y < _listSize.Y && _verticalEnabled) {
		_verticalActive = true;
		_listViewSize.X -= 2;

		// Check if the scrollbar makes the view size small enough to require the other scrollbar.
		if (_listViewSize.X < _listSize.X && _horizontalEnabled) {
			_horizontalActive = true;
			_listViewSize.Y -= 2;
			_scrollSize -= Coord(2, 2);
		}
	}
	else if (_listViewSize.X < _listSize.X && _horizontalEnabled) {
		_horizontalActive = true;
		_listViewSize.Y -= 2;
		
		// Check if the scrollbar makes the view size small enough to require the other scrollbar.
		if (_listViewSize.Y < _listSize.Y && _verticalEnabled) {
			_verticalActive = true;
			_listViewSize.X -= 2;
			_scrollSize -= Coord(2, 2);
		}
	}

	// Setup the vertical scrollbar.
	if (_verticalActive) {
		_barSize.Y = double(_scrollSize.Y) * double(_listViewSize.Y) / double(_listSize.Y);
		if (_barSize.Y == 0)
			_barSize.Y = 1;

		// Set the scrollbar to list size ratio.
		_scrollRatio.Y = double(_scrollSize.Y - _barSize.Y) / double(_listSize.Y - _listViewSize.Y);

		if (keepListPosition) {
			_listPosition.Y = min(_listPosition.Y, _listSize.Y - _listViewSize.Y);
			_scrollPosition.Y = int(_listPosition.Y * _scrollRatio.Y);
		}
		else {
			_listPosition.Y = 0;
			_scrollPosition.Y = 0;
		}
	}
	else {
		_scrollSize.Y = 0;
		_barSize.Y = 0;
		_scrollRatio.Y = 0.0;
		_listPosition.Y = 0;
		_scrollPosition.Y = 0;
	}
	
	// Setup the horizontal scrollbar.
	if (_horizontalActive) {
		_barSize.X = double(_scrollSize.X) * double(_listViewSize.X) / double(_listSize.X);
		if (_barSize.X== 0)
			_barSize.X = 1;

		// Set the scrollbar to list size ratio.
		_scrollRatio.X = double(_scrollSize.X - _barSize.X) / double(_listSize.X - _listViewSize.X);
		
		if (keepListPosition) {
			_listPosition.X = min(_listPosition.X, _listSize.X - _listViewSize.X);
			_scrollPosition.X = int(_listPosition.X * _scrollRatio.X);
		}
		else {
			_listPosition.X = 0;
			_scrollPosition.X = 0;
		}
	}
	else {
		_scrollSize.X = 0;
		_barSize.X = 0;
		_scrollRatio.X = 0.0;
		_listPosition.X = 0;
		_scrollPosition.X = 0;
	}
}

#pragma endregion
//--------------------------------
#pragma region Events

bool PowerConsole::Components::ComponentScrollBar::IsListPositionChanged() const {
	return _listPositionChanged;
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void PowerConsole::Components::ComponentScrollBar::Update() {
	_listPositionChanged = false;
	_hovering = 0;

	Coord mousePos = _app->GetMouse()->GetPosition();
	bool pressed = _app->GetMouse()->IsButtonPressed(MouseButtons::Left);

	if (_app->GetMouse()->IsButtonReleased(MouseButtons::Left))
		_dragging = 0;

	if (_verticalActive) {

		if (_dragging == 0) {
			if (_position + Coord(_listViewSize.X + 1, 0) == mousePos) {
				_hovering = 1;
				if (pressed && _listPosition.Y > 0) {
					SetListPosition(_listPosition - Coord(0, 1));
					_listPositionChanged = true;
				}
			}
			else if (SmallRect(_position + Coord(_listViewSize.X + 1, _scrollPosition.Y + 2), Coord(1, _barSize.Y)).ContainsPoint(mousePos)) {
				_hovering = 2;
				if (pressed) {
					_dragging = 1;
					_gripPosition = mousePos.Y - _position.Y - _scrollPosition.Y - 2;
				}
			}
			else if (_position + Coord(_listViewSize.X + 1, _scrollSize.Y + 3) == mousePos) {
				_hovering = 3;
				if (pressed && _listPosition.Y < _listSize.Y - _listViewSize.Y) {
					SetListPosition(_listPosition + Coord(0, 1));
					_listPositionChanged = true;
				}
			}
			if (SmallRect(_position, _size).ContainsPoint(mousePos) && _mouseWheelEnabled) {
				if (_app->GetMouse()->IsWheelScrolledUp() && (_listPosition.Y > 0)) {
					SetListPosition(_listPosition - Coord(0, 1));
					_listPositionChanged = true;
				}
				else if (_app->GetMouse()->IsWheelScrolledDown() && (_listPosition.Y < _listSize.Y - _listViewSize.Y)) {
					SetListPosition(_listPosition + Coord(0, 1));
					_listPositionChanged = true;
				}

			}
		}
		else if (_dragging == 1) {
			if (mousePos.Y - _position.Y - _gripPosition - 2 != _scrollPosition.Y) {
				int oldScrollPosition = _scrollPosition.Y;
				SetScrollPosition(Coord(_scrollPosition.X, mousePos.Y - _position.Y - _gripPosition - 2));
			}
		}
	}
	
	if (_horizontalActive) {

		if (_dragging == 0) {
			if (_position + Coord(0, _listViewSize.Y + 1) == mousePos) {
				_hovering = 4;
				if (pressed && _listPosition.X > 0) {
					SetListPosition(_listPosition - Coord(1, 0));
					_listPositionChanged = true;
				}
			}
			else if (SmallRect(_position + Coord(_scrollPosition.X + 2, _listViewSize.Y + 1), Coord(_barSize.X, 1)).ContainsPoint(mousePos)) {
				_hovering = 5;
				if (pressed) {
					_dragging = 2;
					_gripPosition = mousePos.X - _position.X - _scrollPosition.X - 2;
				}
			}
			else if (_position + Coord(_scrollSize.X + 3, _listViewSize.Y + 1) == mousePos) {
				_hovering = 6;
				if (pressed && _listPosition.X < _listSize.X - _listViewSize.X) {
					SetListPosition(_listPosition + Coord(1, 0));
					_listPositionChanged = true;
				}
			}
			if (SmallRect(_position, _size).ContainsPoint(mousePos) && _mouseWheelEnabled) {
				if (_app->GetMouse()->IsWheelScrolledRight() && (_listPosition.X > 0)) {
					SetListPosition(_listPosition - Coord(1, 0));
					_listPositionChanged = true;
				}
				else if (_app->GetMouse()->IsWheelScrolledLeft() && (_listPosition.X < _listSize.X - _listViewSize.X)) {
					SetListPosition(_listPosition + Coord(1, 0));
					_listPositionChanged = true;
				}

			}
		}
		else if (_dragging == 2) {
			if (mousePos.X - _position.X - _gripPosition - 2 != _scrollPosition.X) {
				int oldScrollPosition = _scrollPosition.X;
				SetScrollPosition(Coord(mousePos.X - _position.X - _gripPosition - 2, _scrollPosition.Y));
			}
		}
	}
}
	
#pragma endregion
//=========== DRAWING ============
#pragma region Drawing

void PowerConsole::Components::ComponentScrollBar::Draw(AsciiImageBase& g) {

	if (_verticalActive) {
		// Draw frame
		g.SetPixel(_position + Coord(_listViewSize.X, 0), Coord(1, _listViewSize.Y), Pixel(179, 0x08));
		if (g.GetChar(_position + Coord(_listViewSize.X, -1)) == unsigned char(205))
			g.SetChar(_position + Coord(_listViewSize.X, -1), 209);
		else if (g.GetChar(_position + Coord(_listViewSize.X, -1)) == unsigned char(207))
			g.SetChar(_position + Coord(_listViewSize.X, -1), 216);
		else if (g.GetChar(_position + Coord(_listViewSize.X, -1)) == unsigned char(196))
			g.SetChar(_position + Coord(_listViewSize.X, -1), 194);
		else if (g.GetChar(_position + Coord(_listViewSize.X, -1)) == unsigned char(193))
			g.SetChar(_position + Coord(_listViewSize.X, -1), 197);
		
		if (g.GetChar(_position + _listViewSize + Coord(0, (_horizontalActive ? 2 : 0))) == unsigned char(205))
			g.SetChar(_position + _listViewSize + Coord(0, (_horizontalActive ? 2 : 0)), 207);
		else if (g.GetChar(_position + _listViewSize + Coord(0, (_horizontalActive ? 2 : 0))) == unsigned char(209))
			g.SetChar(_position + _listViewSize + Coord(0, (_horizontalActive ? 2 : 0)), 216);

		g.SetPixel(_position + Coord(_listViewSize.X + 1, 1), Pixel(196, 0x08));
		if (g.GetChar(_position + Coord(_listViewSize.X + 2, 1)) == unsigned char(186))
			g.SetChar(_position + Coord(_listViewSize.X + 2, 1), 182);
		else if (g.GetChar(_position + Coord(_listViewSize.X + 2, 1)) == unsigned char(199))
			g.SetChar(_position + Coord(_listViewSize.X + 2, 1), 215);

		g.SetPixel(_position + Coord(_listViewSize.X + 1, _scrollSize.Y + 2), Pixel(196, 0x08));
		if (g.GetChar(_position + Coord(_listViewSize.X + 2, _scrollSize.Y + 2)) == unsigned char(186))
			g.SetChar(_position + Coord(_listViewSize.X + 2, _scrollSize.Y + 2), 182);
		else if (g.GetChar(_position + Coord(_listViewSize.X + 2, _scrollSize.Y + 2)) == unsigned char(199))
			g.SetChar(_position + Coord(_listViewSize.X + 2, _scrollSize.Y + 2), 215);
		
		g.SetPixel(_position + Coord(_listViewSize.X, 1), Pixel(195, 0x08));
		g.SetPixel(_position + Coord(_listViewSize.X, _scrollSize.Y + 2), Pixel(195, 0x08));

		// Draw arrows
		g.SetPixel(_position + Coord(_listViewSize.X + 1, 0), Pixel(30, 0x07));
		if (_listPosition.Y == 0)
			g.SetColor(_position + Coord(_listViewSize.X + 1, 0), 0x08);
		else if (_hovering == 1)
			g.SetColor(_position + Coord(_listViewSize.X + 1, 0), 0x0F);

		g.SetPixel(_position + Coord(_listViewSize.X + 1, _scrollSize.Y + 3), Pixel(31, 0x07));
		if (_listPosition.Y == _listSize.Y - _listViewSize.Y)
			g.SetColor(_position + Coord(_listViewSize.X + 1, _scrollSize.Y + 3), 0x08);
		else if (_hovering == 3)
			g.SetColor(_position + Coord(_listViewSize.X + 1, _scrollSize.Y + 3), 0x0F);

		// Draw bar
		g.SetPixel(_position + Coord(_listViewSize.X + 1, _scrollPosition.Y + 2), Coord(1, _barSize.Y), Pixel(219, 0x08));
		if (_hovering == 2 || _dragging == 1)
			g.SetColor(_position + Coord(_listViewSize.X + 1, _scrollPosition.Y + 2), Coord(1, _barSize.Y), 0x07);
	}
	if (_horizontalActive) {
		// Draw frame
		g.SetPixel(_position + Coord(0, _listViewSize.Y), Coord(_listViewSize.X, 1), Pixel(196, 0x08));
		if (g.GetChar(_position + Coord(-1, _listViewSize.Y)) == unsigned char(186))
			g.SetChar(_position + Coord(-1, _listViewSize.Y), 199);
		else if (g.GetChar(_position + Coord(-1, _listViewSize.Y)) == unsigned char(182))
			g.SetChar(_position + Coord(-1, _listViewSize.Y), 215);

		if (g.GetChar(_position + _listViewSize + Coord((_verticalActive ? 2 : 0), 0)) == unsigned char(186))
			g.SetChar(_position + _listViewSize + Coord((_verticalActive ? 2 : 0), 0), 182);
		else if (g.GetChar(_position + _listViewSize + Coord((_verticalActive ? 2 : 0), 0)) == unsigned char(199))
			g.SetChar(_position + _listViewSize + Coord((_verticalActive ? 2 : 0), 0), 215);

		g.SetPixel(_position + Coord(1, _listViewSize.Y + 1), Pixel(179, 0x08));
		if (g.GetChar(_position + Coord(1, _listViewSize.Y + 2)) == unsigned char(205))
			g.SetChar(_position + Coord(1, _listViewSize.Y + 2), 207);
		else if (g.GetChar(_position + Coord(1, _listViewSize.Y + 2)) == unsigned char(209))
			g.SetChar(_position + Coord(1, _listViewSize.Y + 2), 216);

		g.SetPixel(_position + Coord(_scrollSize.X + 2, _listViewSize.Y + 1), Pixel(179, 0x08));
		if (g.GetChar(_position + Coord(_scrollSize.X + 2, _listViewSize.Y + 2)) == unsigned char(205))
			g.SetChar(_position + Coord(_scrollSize.X + 2, _listViewSize.Y + 2), 207);
		else if (g.GetChar(_position + Coord(_scrollSize.X + 2, _listViewSize.Y + 2)) == unsigned char(209))
			g.SetChar(_position + Coord(_scrollSize.X + 2, _listViewSize.Y + 2), 216);
		
		g.SetPixel(_position + Coord(1, _listViewSize.Y), Pixel(194, 0x08));
		g.SetPixel(_position + Coord(_scrollSize.X + 2, _listViewSize.Y), Pixel(194, 0x08));
		
		// Draw arrows
		g.SetPixel(_position + Coord(0, _listViewSize.Y + 1), Pixel(17, 0x07));
		if (_listPosition.X == 0)
			g.SetColor(_position + Coord(0, _listViewSize.Y + 1), 0x08);
		else if (_hovering == 4)
			g.SetColor(_position + Coord(0, _listViewSize.Y + 1), 0x0F);

		g.SetPixel(_position + Coord(_scrollSize.X + 3, _listViewSize.Y + 1), Pixel(16, 0x07));
		if (_listPosition.X == _listSize.X - _listViewSize.X)
			g.SetColor(_position + Coord(_scrollSize.X + 3, _listViewSize.Y + 1), 0x08);
		else if (_hovering == 6)
			g.SetColor(_position + Coord(_scrollSize.X + 3, _listViewSize.Y + 1), 0x0F);

		// Draw bar
		g.SetPixel(_position + Coord(_scrollPosition.X + 2, _listViewSize.Y + 1), Coord(_barSize.X, 1), Pixel(219, 0x08));
		if (_hovering == 5 || _dragging == 2)
			g.SetColor(_position + Coord(_scrollPosition.X + 2, _listViewSize.Y + 1), Coord(_barSize.X, 1), 0x07);
	}

	if (_verticalActive && _horizontalActive) {
		// Draw frame
		g.SetPixel(_position + _listViewSize, Pixel(197, 0x08));
		g.SetPixel(_position + _listViewSize + Coord(1, 0), Pixel(196, 0x08));
		g.SetPixel(_position + _listViewSize + Coord(0, 1), Pixel(179, 0x08));
	}
}
	
#pragma endregion
//================================
#pragma endregion
//=================================================================|
#pragma endregion
//=================================================================|