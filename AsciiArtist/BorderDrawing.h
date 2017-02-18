
struct BorderConnection {
	BorderConnection() {
		this->Left	= 0;
		this->Right	= 0;
		this->Up	= 0;
		this->Down	= 0;
	}
	BorderConnection(short left, short up, short right, short down) {
		this->Left	= left;
		this->Right	= right;
		this->Up	= up;
		this->Down	= down;

	}

	short Left;
	short Right;
	short Up;
	short Down;

	bool operator==(const BorderConnection& rhs) {
		return (Left == rhs.Left && Right == rhs.Right &&
				Up == rhs.Up && Down == rhs.Down);
	}
};
BorderConnection GetBorderConnection(unsigned char character) {
	switch (character) {
	// Thin H, Thin V
	case 179:	return BorderConnection(0, 1, 0, 1);
	case 196:	return BorderConnection(1, 0, 1, 0);

	case 192:	return BorderConnection(0, 1, 1, 0);
	case 191:	return BorderConnection(1, 0, 0, 1);
	case 218:	return BorderConnection(0, 0, 1, 1);
	case 217:	return BorderConnection(1, 1, 0, 0);

	case 180:	return BorderConnection(1, 1, 0, 1);
	case 195:	return BorderConnection(0, 1, 1, 1);
	case 194:	return BorderConnection(1, 0, 1, 1);
	case 193:	return BorderConnection(1, 1, 1, 0);

	case 197:	return BorderConnection(1, 1, 1, 1);

	// Thick H, Thick V
	case 186:	return BorderConnection(0, 2, 0, 2);
	case 205:	return BorderConnection(2, 0, 2, 0);

	case 200:	return BorderConnection(0, 2, 2, 0);
	case 187:	return BorderConnection(2, 0, 0, 2);
	case 201:	return BorderConnection(0, 0, 2, 2);
	case 188:	return BorderConnection(2, 2, 0, 0);

	case 185:	return BorderConnection(2, 2, 0, 2);
	case 204:	return BorderConnection(0, 2, 2, 2);
	case 203:	return BorderConnection(2, 0, 2, 2);
	case 202:	return BorderConnection(2, 2, 2, 0);

	case 206:	return BorderConnection(2, 2, 2, 2);

	// Thick H, Thin V
	case 212:	return BorderConnection(0, 1, 2, 0);
	case 184:	return BorderConnection(2, 0, 0, 1);
	case 213:	return BorderConnection(0, 0, 2, 1);
	case 190:	return BorderConnection(2, 1, 0, 0);

	case 181:	return BorderConnection(2, 1, 0, 1);
	case 198:	return BorderConnection(0, 1, 2, 1);
	case 209:	return BorderConnection(2, 0, 2, 1);
	case 207:	return BorderConnection(2, 1, 2, 0);

	case 216:	return BorderConnection(2, 1, 2, 1);
		
	// Thin H, Thick V
	case 211:	return BorderConnection(0, 2, 1, 0);
	case 183:	return BorderConnection(1, 0, 0, 2);
	case 214:	return BorderConnection(0, 0, 1, 2);
	case 189:	return BorderConnection(1, 2, 0, 0);

	case 182:	return BorderConnection(1, 2, 0, 2);
	case 199:	return BorderConnection(0, 2, 1, 2);
	case 210:	return BorderConnection(1, 0, 1, 2);
	case 208:	return BorderConnection(1, 2, 1, 0);

	case 215:	return BorderConnection(1, 2, 1, 2);

	// None
	default:	return BorderConnection();
	};
}
unsigned char GetBorder(short left, short up, short right, short down) {
	BorderConnection connection = BorderConnection(left, up, right, down);
	for (int i = 179; i <= 218; i++) {
		if (GetBorderConnection((unsigned char)i) == connection)
			return (unsigned char)i;
	}
	return '\0';
}
unsigned char CreateBorder
short GetConnector(AsciiImageBase& image, Coord point) {


}
void DrawBorder(AsciiImageBase& image, Coord point, Coord size, Pixel pixel, bool thick, bool specific = true, bool highpriority = true) {
	
	BorderConnection* borders;



	delete[] borders;
}
void PowerConsole::Drawing::AsciiImageBase::DrawBorderLine(Coord point, Coord size, Pixel pixel, bool thick, bool priority, bool specific, bool asEnd, unsigned short modAttributes, unsigned short modOptions) {

}
void PowerConsole::Drawing::AsciiImageBase::DrawBorderRect(Coord point, Coord size, Pixel pixel, bool thick, bool priority, bool specific, bool asEnd, unsigned short modAttributes, unsigned short modOptions) {

}