//================================================================
// GEOMETRY CPP:
//================================================================
#include "stdafx.h"
#include "Geometry.h"

using namespace std;
using namespace PowerConsole::Geometry;
//================================================================
// CLASSES:
//================================================================
// Coord:
//----------------------------------------------------------------
//========== CONSTANTS ===========
const Coord PowerConsole::Geometry::Coord::Zero = Coord(0, 0);
const Coord PowerConsole::Geometry::Coord::One = Coord(1, 1);
//========= CONSTRUCTORS =========
PowerConsole::Geometry::Coord::Coord() {
	this->X		= 0;
	this->Y		= 0;
}
PowerConsole::Geometry::Coord::Coord(int x, int y) {
	this->X		= x;
	this->Y		= y;
}
PowerConsole::Geometry::Coord::Coord(int xy) {
	this->X		= xy;
	this->Y		= xy;
}
PowerConsole::Geometry::Coord::Coord(const Coord& coord) {
	this->X		= coord.X;
	this->Y		= coord.Y;
}
PowerConsole::Geometry::Coord::Coord(const COORD& coord) {
	this->X		= coord.X;
	this->Y		= coord.Y;
}
PowerConsole::Geometry::Coord::Coord(const POINT& point) {
	this->X		= point.x;
	this->Y		= point.y;
}
PowerConsole::Geometry::Coord::Coord(const SMALL_RECT& rect, bool asSize) {
	this->X		= (asSize ? (rect.Right - rect.Left + 1) : rect.Left);
	this->Y		= (asSize ? (rect.Bottom - rect.Top + 1) : rect.Top);
}
PowerConsole::Geometry::Coord::Coord(const RECT& rect, bool asSize) {
	this->X		= (asSize ? (rect.right - rect.left + 1) : rect.left);
	this->Y		= (asSize ? (rect.bottom - rect.top + 1) : rect.top);
}
//========== OPERATORS ===========
Coord& PowerConsole::Geometry::Coord::operator++() {
	X++; Y++;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator--() {
	X--; Y--;
	return *this;
}
//--------------------------------
Coord& PowerConsole::Geometry::Coord::operator+=(int rhs) {
	X += rhs; Y += rhs;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator+=(Coord rhs) {
	X += rhs.X; Y += rhs.Y;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator-=(int rhs) {
	X -= rhs; Y -= rhs;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator-=(Coord rhs) {
	X -= rhs.X; Y -= rhs.Y;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator*=(int rhs) {
	X *= rhs; Y *= rhs;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator*=(Coord rhs) {
	X *= rhs.X; Y *= rhs.Y;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator/=(int rhs) {
	X /= rhs; Y /= rhs;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator/=(Coord rhs) {
	X /= rhs.X; Y /= rhs.Y;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator%=(int rhs) {
	X %= rhs; Y %= rhs;
	return *this;
}
Coord& PowerConsole::Geometry::Coord::operator%=(Coord rhs) {
	X %= rhs.X; Y %= rhs.Y;
	return *this;
}
//--------------------------------
Coord PowerConsole::Geometry::Coord::operator+() const {
	return Coord(+X, +Y);
}
Coord PowerConsole::Geometry::Coord::operator-() const {
	return Coord(-X, -Y);
}
//--------------------------------
Coord PowerConsole::Geometry::Coord::operator+(int rhs) const {
	return Coord(X + rhs, Y + rhs);
}
Coord PowerConsole::Geometry::Coord::operator+(Coord rhs) const {
	return Coord(X + rhs.X, Y + rhs.Y);
}
Coord PowerConsole::Geometry::Coord::operator-(int rhs) const {
	return Coord(X - rhs, Y - rhs);
}
Coord PowerConsole::Geometry::Coord::operator-(Coord rhs) const {
	return Coord(X - rhs.X, Y - rhs.Y);
}
Coord PowerConsole::Geometry::Coord::operator*(int rhs) const {
	return Coord(X * rhs, Y * rhs);
}
Coord PowerConsole::Geometry::Coord::operator*(Coord rhs) const {
	return Coord(X * rhs.X, Y * rhs.Y);
}
Coord PowerConsole::Geometry::Coord::operator/(int rhs) const {
	return Coord(X / rhs, Y / rhs);
}
Coord PowerConsole::Geometry::Coord::operator/(Coord rhs) const {
	return Coord(X / rhs.X, Y / rhs.Y);
}
Coord PowerConsole::Geometry::Coord::operator%(int rhs) const {
	return Coord(X % rhs, Y % rhs);
}
Coord PowerConsole::Geometry::Coord::operator%(Coord rhs) const {
	return Coord(X % rhs.X, Y % rhs.Y);
}
//--------------------------------
bool PowerConsole::Geometry::Coord::operator==(Coord rhs) const {
	return (X == rhs.X && Y == rhs.Y);
}
bool PowerConsole::Geometry::Coord::operator!=(Coord rhs) const {
	return (X != rhs.X || Y != rhs.Y);
}
bool PowerConsole::Geometry::Coord::operator>(Coord rhs) const {
	return (X > rhs.X && Y > rhs.Y);
}
bool PowerConsole::Geometry::Coord::operator<(Coord rhs) const {
	return (X < rhs.X && Y < rhs.Y);
}
bool PowerConsole::Geometry::Coord::operator>=(Coord rhs) const {
	return (X >= rhs.X && Y >= rhs.Y);
}
bool PowerConsole::Geometry::Coord::operator<=(Coord rhs) const {
	return (X <= rhs.X && Y <= rhs.Y);
}
//========== CONVERSION ==========
PowerConsole::Geometry::Coord::operator Point2D() const {
	return Point2D(X, Y);
}
PowerConsole::Geometry::Coord::operator COORD() const {
	COORD c = {(SHORT)X, (SHORT)Y};
	return c;
}
PowerConsole::Geometry::Coord::operator POINT() const {
	POINT p = {(LONG)X, (LONG)Y};
	return p;
}
//========= INFORMATION ==========
double PowerConsole::Geometry::Coord::Length() const {
	return sqrt(X*X + Y*Y);
}
double PowerConsole::Geometry::Coord::Direction() const {
	return (double)atan2(Y, X) * 180.0/M_PI;
}
//----------------------------------------------------------------
// Point2D:
//----------------------------------------------------------------
//========== CONSTANTS ===========
const Point2D PowerConsole::Geometry::Point2D::Zero = Point2D(0.0, 0.0);
const Point2D PowerConsole::Geometry::Point2D::One = Point2D(1.0, 1.0);
//========= CONSTRUCTORS =========
PowerConsole::Geometry::Point2D::Point2D() {
	this->X		= 0.0;
	this->Y		= 0.0;
}
PowerConsole::Geometry::Point2D::Point2D(double x, double y) {
	this->X		= x;
	this->Y		= y;
}
PowerConsole::Geometry::Point2D::Point2D(double xy) {
	this->X		= xy;
	this->Y		= xy;
}
PowerConsole::Geometry::Point2D::Point2D(const Point2D& point) {
	this->X		= point.X;
	this->Y		= point.Y;
}
PowerConsole::Geometry::Point2D::Point2D(const COORD& coord) {
	this->X		= coord.X;
	this->Y		= coord.Y;
}
PowerConsole::Geometry::Point2D::Point2D(const POINT& point) {
	this->X		= point.x;
	this->Y		= point.y;
}
PowerConsole::Geometry::Point2D::Point2D(const SMALL_RECT& rect, bool asSize) {
	this->X		= (asSize ? (rect.Right - rect.Left + 1) : rect.Left);
	this->Y		= (asSize ? (rect.Bottom - rect.Top + 1) : rect.Top);
}
PowerConsole::Geometry::Point2D::Point2D(const RECT& rect, bool asSize) {
	this->X		= (asSize ? (rect.right - rect.left + 1) : rect.left);
	this->Y		= (asSize ? (rect.bottom - rect.top + 1) : rect.top);
}
//========== OPERATORS ===========
Point2D& PowerConsole::Geometry::Point2D::operator++() {
	X++; Y++;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator--() {
	X--; Y--;
	return *this;
}
//--------------------------------
Point2D& PowerConsole::Geometry::Point2D::operator+=(double rhs) {
	X += rhs; Y += rhs;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator+=(Point2D rhs) {
	X += rhs.X; Y += rhs.Y;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator-=(double rhs) {
	X -= rhs; Y -= rhs;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator-=(Point2D rhs) {
	X -= rhs.X; Y -= rhs.Y;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator*=(double rhs) {
	X *= rhs; Y *= rhs;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator*=(Point2D rhs) {
	X *= rhs.X; Y *= rhs.Y;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator/=(double rhs) {
	X /= rhs; Y /= rhs;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator/=(Point2D rhs) {
	X /= rhs.X; Y /= rhs.Y;
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator%=(double rhs) {
	X = fmod(X, rhs); Y = fmod(Y, rhs);
	return *this;
}
Point2D& PowerConsole::Geometry::Point2D::operator%=(Point2D rhs) {
	X = fmod(X, rhs.X); Y = fmod(Y, rhs.Y);
	return *this;
}
//--------------------------------
Point2D PowerConsole::Geometry::Point2D::operator+() const {
	return Point2D(+X, +Y);
}
Point2D PowerConsole::Geometry::Point2D::operator-() const {
	return Point2D(-X, -Y);
}
//--------------------------------
Point2D PowerConsole::Geometry::Point2D::operator+(double rhs) const {
	return Point2D(X + rhs, Y + rhs);
}
Point2D PowerConsole::Geometry::Point2D::operator+(Point2D rhs) const {
	return Point2D(X + rhs.X, Y + rhs.Y);
}
Point2D PowerConsole::Geometry::Point2D::operator-(double rhs) const {
	return Point2D(X - rhs, Y - rhs);
}
Point2D PowerConsole::Geometry::Point2D::operator-(Point2D rhs) const {
	return Point2D(X - rhs.X, Y - rhs.Y);
}
Point2D PowerConsole::Geometry::Point2D::operator*(double rhs) const {
	return Point2D(X * rhs, Y * rhs);
}
Point2D PowerConsole::Geometry::Point2D::operator*(Point2D rhs) const {
	return Point2D(X * rhs.X, Y * rhs.Y);
}
Point2D PowerConsole::Geometry::Point2D::operator/(double rhs) const {
	return Point2D(X / rhs, Y / rhs);
}
Point2D PowerConsole::Geometry::Point2D::operator/(Point2D rhs) const {
	return Point2D(X / rhs.X, Y / rhs.Y);
}
Point2D PowerConsole::Geometry::Point2D::operator%(double rhs) const {
	return Point2D(fmod(X, rhs), fmod(Y, rhs));
}
Point2D PowerConsole::Geometry::Point2D::operator%(Point2D rhs) const {
	return Point2D(fmod(X, rhs.X), fmod(Y, rhs.Y));
}
//--------------------------------
bool PowerConsole::Geometry::Point2D::operator==(Point2D rhs) const {
	return (X == rhs.X && Y == rhs.Y);
}
bool PowerConsole::Geometry::Point2D::operator!=(Point2D rhs) const {
	return (X != rhs.X || Y != rhs.Y);
}
bool PowerConsole::Geometry::Point2D::operator>(Point2D rhs) const {
	return (X > rhs.X && Y > rhs.Y);
}
bool PowerConsole::Geometry::Point2D::operator<(Point2D rhs) const {
	return (X < rhs.X && Y < rhs.Y);
}
bool PowerConsole::Geometry::Point2D::operator>=(Point2D rhs) const {
	return (X >= rhs.X && Y >= rhs.Y);
}
bool PowerConsole::Geometry::Point2D::operator<=(Point2D rhs) const {
	return (X <= rhs.X && Y <= rhs.Y);
}
//========== CONVERSION ==========
PowerConsole::Geometry::Point2D::operator Coord() const {
	return Coord((int)X, (int)Y);
}
PowerConsole::Geometry::Point2D::operator COORD() const {
	COORD c = {(SHORT)X, (SHORT)Y};
	return c;
}
PowerConsole::Geometry::Point2D::operator POINT() const {
	POINT p = {(LONG)X, (LONG)Y};
	return p;
}
//========= INFORMATION ==========
double PowerConsole::Geometry::Point2D::Length() const {
	return sqrt(X*X + Y*Y);
}
double PowerConsole::Geometry::Point2D::Direction() const {
	return (double)atan2(Y, X) * 180.0/M_PI;
}
//----------------------------------------------------------------
// SmallRect:
//----------------------------------------------------------------
//========= CONSTRUCTORS =========
PowerConsole::Geometry::SmallRect::SmallRect() {
	this->Point		= Coord();
	this->Size		= Coord();
}
PowerConsole::Geometry::SmallRect::SmallRect(int x, int y, int width, int height) {
	this->Point		= Coord(x, y);
	this->Size		= Coord(width, height);
}
PowerConsole::Geometry::SmallRect::SmallRect(Coord point, int width, int height) {
	this->Point		= point;
	this->Size		= Coord(width, height);
}
PowerConsole::Geometry::SmallRect::SmallRect(int x, int y, Coord size) {
	this->Point		= Coord(x, y);
	this->Size		= size;
}
PowerConsole::Geometry::SmallRect::SmallRect(Coord point, Coord size) {
	this->Point		= point;
	this->Size		= size;
}
/*SmallRect(const Rectangle2D& rect) {
	this->Point		= rect.Point;
	this->Size		= rect.Size;
}*/
PowerConsole::Geometry::SmallRect::SmallRect(const SMALL_RECT& rect) {
	this->Point		= Coord(rect.Left, rect.Top);
	this->Size		= Coord(rect.Right - rect.Left + 1, rect.Bottom - rect.Top + 1);
}
PowerConsole::Geometry::SmallRect::SmallRect(const RECT& rect) {
	this->Point		= Coord(rect.left, rect.top);
	this->Size		= Coord(rect.right - rect.left + 1, rect.bottom - rect.top + 1);
}
//========== OPERATORS ===========
SmallRect PowerConsole::Geometry::SmallRect::operator+(int rhs) const {
	return SmallRect(Point + rhs, Size);
}
SmallRect PowerConsole::Geometry::SmallRect::operator+(Coord rhs) const {
	return SmallRect(Point + rhs, Size);
}
SmallRect PowerConsole::Geometry::SmallRect::operator-(int rhs) const {
	return SmallRect(Point - rhs, Size);
}
SmallRect PowerConsole::Geometry::SmallRect::operator-(Coord rhs) const {
	return SmallRect(Point - rhs, Size);
}
SmallRect PowerConsole::Geometry::SmallRect::operator*(int rhs) const {
	return SmallRect(Point, Size * rhs);
}
SmallRect PowerConsole::Geometry::SmallRect::operator*(Coord rhs) const {
	return SmallRect(Point, Size * rhs);
}
SmallRect PowerConsole::Geometry::SmallRect::operator/(int rhs) const {
	return SmallRect(Point, Size / rhs);
}
SmallRect PowerConsole::Geometry::SmallRect::operator/(Coord rhs) const {
	return SmallRect(Point, Size / rhs);
}
SmallRect PowerConsole::Geometry::SmallRect::operator%(int rhs) const {
	return SmallRect(Point, Size % rhs);
}
SmallRect PowerConsole::Geometry::SmallRect::operator%(Coord rhs) const {
	return SmallRect(Point, Size % rhs);
}
//--------------------------------
bool PowerConsole::Geometry::SmallRect::operator==(SmallRect rhs) const {
	return (Point == rhs.Point && Size == rhs.Size);
}
bool PowerConsole::Geometry::SmallRect::operator!=(SmallRect rhs) const {
	return (Point != rhs.Point || Size != rhs.Size);
}
//========== CONVERSION ==========
PowerConsole::Geometry::SmallRect::operator SMALL_RECT() const {
	SMALL_RECT r;
	r.Left		= Point.X;
	r.Top		= Point.Y;
	r.Right		= Point.X + Size.X - 1;
	r.Bottom	= Point.Y + Size.Y - 1;
	return r;
}
PowerConsole::Geometry::SmallRect::operator RECT() const {
	RECT r;
	r.left		= Point.X;
	r.top		= Point.Y;
	r.right		= Point.X + Size.X - 1;
	r.bottom	= Point.Y + Size.Y - 1;
	return r;
}
//========= INFORMATION ==========
int PowerConsole::Geometry::SmallRect::Area() const {
	return Size.X * Size.Y;
}
int PowerConsole::Geometry::SmallRect::Perimeter() const {
	return Size.X*2 + Size.Y*2;
}
//========== COLLISION ===========
bool PowerConsole::Geometry::SmallRect::ContainsPoint(Coord coord) const {
	return (coord >= Point && coord < Point + Size);
}
bool PowerConsole::Geometry::SmallRect::ContainsRect(SmallRect rect) const {
	return (rect.Point >= Point && rect.Point + rect.Size <= Point + Size);
}
//================================================================
// END GEOMETRY CPP:
//================================================================