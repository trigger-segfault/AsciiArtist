/*=================================================================|
|* File:				AsciiChartWindow.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "AsciiChartWindow.h"
#include <PowerConsole/PowerConsoleSettings.h>
//#include <PowerConsole/Controls/Toolbox/ScrollViewer.h>
//#include <PowerConsole/Controls/Toolbox/StackPanel.h>
//#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Geometry/GMath.h>
//#include <PowerConsole/Helpers/Flags.h>
#include <PowerConsole/Helpers/ToString.h>
#include <PowerConsole/Console.h>

//using namespace std;
using namespace AsciiChart;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
//using namespace PowerConsole::IO;
using namespace PowerConsole::Input;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const Point2I AsciiChartWindow::DEFAULT_WINDOW_SIZE = Point2I(74, 68);

/* Names printed for each color. */
static const std::array<std::string, 16> COLOR_NAMES = {
	"Black", "DarkBlue", "DarkGreen", "DarkCyan", "DarkRed", "DarkMagenta", "DarkYellow", "Gray",
	"DarkGray", "Blue", "Green", "Cyan", "Red", "Magenta", "Yellow", "White"
};
/* List of radixes to toggle through. */
static const std::array<unsigned short, 2> RADIXES = { (unsigned short)16, (unsigned short)10 };

/* The duration that a message lasts for. */
static const int MESSAGE_DURATION = 2000;
static const int MESSAGE_DARKEN_1 = 400;
static const int MESSAGE_DARKEN_2 = 200;

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

AsciiChartWindow::AsciiChartWindow(int argc, char** argv)
 :	darkMode(false), 
	charRadix(RADIXES[0]),
	colorRadix(RADIXES[0]),
	textColor(Pixel()),
	messageColor(Pixel()),
	messageColorDarken1(Pixel()),
	messageColorDarken2(Pixel()),
	resizeColor(ResizeHandlePixel(0xB2,0,0,0)), // heavy shaded square
	luminanceThreshold(55),
	message(""),
	messageTimer(0) {

	updateTheme();

	//srand((unsigned int)time(nullptr));

	eventInitialize().add(bindEvent(AsciiChartWindow::onInitialize));
	eventLoad().add(bindEvent(AsciiChartWindow::onLoad));
	eventTick().add(bindEvent(AsciiChartWindow::onTick));
	//eventPaint().add(bindEvent(AsciiChartWindow::onPaint));

	commandLine = fixed_array<std::string>((size_t)argc);
	for (int i = 0; i < argc; i++)
		commandLine[i] = argv[i];

	PowerConsoleSettings::setDrawEntireBuffer(false);
}

void AsciiChartWindow::onInitialize(ControlSPtr sender, EventArgs& e) {

	#pragma region Window Setup

	setSize(DEFAULT_WINDOW_SIZE);
	setMinSize(Point2I(64, 26));
	setResizeHandleEnabled(true);
	updateTitle();
	//console().setIconFromResourceByIndex(0);

	#pragma endregion

	#pragma region Controls

	stackPanel = makeControl(StackPanel);
	//stackPanel->setSize(Point2I(SIZE_AUTO, SIZE_AUTO));
	stackPanel->setHorizontalAlignment(HorizontalAlignments::Stretch);
	stackPanel->setMargin(Thickness(0, 0, 0, 0)); // 1, 1, 1, 0));
	stackPanel->setOrientation(Orientations::Vertical);
	stackPanel->setSpacing(1);
	children().add(stackPanel);

	// Cheap trick to draw over other controls
	// shows a darkened border when you're resizing and cutting off information
	panelBottom = makeControl(Panel);
	panelBottom->setSize(Point2I(SIZE_AUTO, 1));
	panelBottom->setHorizontalAlignment(HorizontalAlignments::Stretch);
	panelBottom->setVerticalAlignment(VerticalAlignments::Bottom);
	panelBottom->eventPaint().add(bindEvent(AsciiChartWindow::onBottomDarkenPaint));
	children().add(panelBottom);

	panelLogo = makeControl(Panel);
	panelLogo->setSize(Point2I(64, 6));
	panelLogo->setMargin(Thickness(0, 0, 0, 0));
	panelLogo->setHorizontalAlignment(HorizontalAlignments::Center);
	panelLogo->eventPaint().add(bindEvent(AsciiChartWindow::onLogoPaint));
	stackPanel->children().add(panelLogo);


	charChart = makeControl(ChartWrapPanel);
	//charChart->setSize(Point2I(SIZE_AUTO, SIZE_AUTO));
	charChart->setHorizontalAlignment(HorizontalAlignments::Center);
	charChart->setOrientation(Orientations::Vertical);
	charChart->setSpacing(Point2I(1, 1));
	charChart->setMargin(Thickness(1, 0, 1, 1));
	charChart->setUniformItems(Point2I(6, 1), 256);
	charChart->eventPaintItem().add(bindEvent(AsciiChartWindow::onCharactersPaintItem));
	stackPanel->children().add(charChart);

	colorChart = makeControl(ChartWrapPanel);
	colorChart->setSize(Point2I(SIZE_AUTO, SIZE_AUTO));
	colorChart->setHorizontalAlignment(HorizontalAlignments::Center);
	colorChart->setOrientation(Orientations::Vertical);
	// 0 x spacing because of "DarkMagenta", which is the longest name, but the next number is always 1-width.
	colorChart->setSpacing(Point2I(0, 1));
	colorChart->setMargin(Thickness(1, 0, 1, 0));
	fixed_array<Point2I> colorSizes = fixed_array<Point2I>(COLOR_NAMES.size());
	for (int i = 0; i < (int)COLOR_NAMES.size(); i++) {
		colorSizes[i] = Point2I(COLOR_NAMES[i].length() + 4, 1);
	}
	colorChart->setIndividualItems(colorSizes);
	colorChart->eventPaintItem().add(bindEvent(AsciiChartWindow::onColorsPaintItem));
	stackPanel->children().add(colorChart);

	panelMessage = makeControl(Panel);
	panelMessage->setSize(Point2I(SIZE_AUTO, 1));
	panelMessage->setMargin(Thickness(0, 0, 0, 0));
	panelMessage->setVisible(false);
	panelMessage->eventPaint().add(bindEvent(AsciiChartWindow::onMessagePaint));
	children().add(panelMessage);

	#pragma endregion

	#pragma region Commands

	commands().add(Command("Toggle Dark Mode", Hotkey(Keys::Oemtilde), false,
		bindEvent(AsciiChartWindow::onToggleDarkModeCommand)
	));

	commands().add(Command("Toggle Char Hex", Hotkey(Keys::D1), false,
		bindEvent(AsciiChartWindow::onToggleCharRadixCommand)
	));
	commands().add(Command("Toggle Color Hex", Hotkey(Keys::D2), false,
		bindEvent(AsciiChartWindow::onToggleColorRadixCommand)
	));

	commands().add(Command("Lower Luminance", Hotkey(Keys::D3), false,
		bindEvent(AsciiChartWindow::onLowerLuminanceCommand)
	));
	commands().add(Command("Raise Luminance", Hotkey(Keys::D4), false,
		bindEvent(AsciiChartWindow::onRaiseLuminanceCommand)
	));

	#pragma endregion
}
void AsciiChartWindow::onLoad(ControlSPtr sender, EventArgs& e) {

	setSize(DEFAULT_WINDOW_SIZE); // size won't set the full dimensions in initialization (is this a bug?)
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties
//--------------------------------
#pragma region Command Line

fixed_array<std::string> AsciiChartWindow::getCommandLineArguments() const {
	return commandLine;
}
std::string AsciiChartWindow::getCommandLineArgument(int index) const {
	return commandLine[index];
}
int AsciiChartWindow::getNumCommandLineArguments() const {
	return (int)commandLine.size();
}

#pragma endregion
//--------------------------------
#pragma endregion
//=========== UPDATING ===========
#pragma region Updating

void AsciiChartWindow::sendMessage(const std::string& message) {
	sendMessage(message, textColor.color);
}
void AsciiChartWindow::sendMessage(const std::string& message, unsigned char messageColor) {
	this->panelMessage->setSize(Point2I(message.length() + 4, 3));
	this->panelMessage->setVisible(true);
	this->message = message;
	this->messageTimer = MESSAGE_DURATION;
}

#pragma endregion
//=========== HELPERS ============
#pragma region Helpers

void AsciiChartWindow::updateTitle() {
	console().setTitle("Ascii Chart");
}
void AsciiChartWindow::updateTheme() {
	if (darkMode) {
		textColor.color    = toColor(BLACK, GRAY);
		messageColor.color = toColor(BLACK, WHITE);
		messageColorDarken1.color = toColor(BLACK, GRAY);
		messageColorDarken2.color = toColor(BLACK, DARK_GRAY);
		resizeColor.normalColor = toColor(BLACK, DARK_GRAY);
		resizeColor.hoverColor  = toColor(BLACK, WHITE);
		resizeColor.resizeColor = toColor(BLACK, GRAY);
	}
	else {
		textColor.color    = toColor(WHITE, BLACK);
		messageColor.color = toColor(WHITE, BLACK);
		messageColorDarken1.color = toColor(WHITE, DARK_GRAY);
		messageColorDarken2.color = toColor(WHITE, GRAY);
		resizeColor.normalColor = toColor(WHITE, DARK_GRAY);
		resizeColor.hoverColor  = toColor(WHITE, GRAY);
		resizeColor.resizeColor = toColor(WHITE, BLACK);
	}
	setBackground(textColor);
	setResizeHandlePixel(resizeColor);
}

ConsoleColors AsciiChartWindow::chooseForeground(ConsoleColors bgColor) {
	// normal ugly method:
	//return Pixel(' ', toColor(bgColor, (bgColor < 8 ? WHITE : BLACK)));

	//source: <https://stackoverflow.com/a/1855903/7517185>
	ColorI rgb = console().getPaletteColor((int)bgColor);
	// Counting the perceptive luminance - human eye favors green color...
	int luminance = (int)GMath::round(((0.299f * rgb.r + 0.587f * rgb.g + 0.114f * rgb.b) / 255) * 100);

	if (luminance <= 0)
		return WHITE; // always use white on black
	else if (luminance >= 100)
		return BLACK; // always use black on white
	else
		return (luminance >= luminanceThreshold ? BLACK : WHITE);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void AsciiChartWindow::onMessagePaint(ControlSPtr sender, PaintEventArgs& e) {
	if (messageTimer > 0) {
		auto g = e.graphics;

		g->fill(textColor);
		Pixel color;
		if (messageTimer > MESSAGE_DARKEN_1)
			color = messageColor;
		else if (messageTimer > MESSAGE_DARKEN_2)
			color = messageColorDarken1;
		else
			color = messageColorDarken2;

		g->drawBorder(panelMessage->getActualBounds(), color, true);
		g->drawString(Point2I(2, 1), message, color);
	}
}
void AsciiChartWindow::onTick(ControlSPtr sender, TickEventArgs& e) {
	if (messageTimer > 0) {
		messageTimer -= e.timeSinceLastTick;
		if (messageTimer <= 0) {
			panelMessage->setVisible(false);
		}
	}
}
void AsciiChartWindow::onLogoPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	g->setRegionUnlocked(true);

	int x = 0, y = 0;
	g->drawString(Point2I(x, y++), R"(    ____   _____ _____________    _______  ______    ____  _____)", textColor);
	g->drawString(Point2I(x, y++), R"(   / _  | / ___// ___/  _/  _/   / ___/ /_/ / _  |  / __ \/_  _/)", textColor);
	g->drawString(Point2I(x, y++), R"(  / /_| | \__ \/ /   / / / /    / /  / __  / /_| | / /_/ / / /  )", textColor);
	g->drawString(Point2I(x, y++), R"( / /__| | __/ / /__ / / / /    / /__/ / / / /__| |/ / / / / /   )", textColor);
	g->drawString(Point2I(x, y++), R"(/_/   |_/____/\___/___/___/    \___/_/ /_/_/   |_/_/  |_|/_/    )", textColor);
}
void AsciiChartWindow::onCharactersPaintItem(ControlSPtr sender, PaintItemEventArgs& e) {
	auto g = e.graphics;
	std::string num = ToString::intTypeToString(e.itemIndex, charRadix) + ":";

	g->drawString(Point2I(3, 0), num, textColor, HorizontalAlignments::Right);
	g->setChar(Point2I(5, 0), (unsigned char)e.itemIndex);
}
void AsciiChartWindow::onColorsPaintItem(ControlSPtr sender, PaintItemEventArgs& e) {
	auto g = e.graphics;
	std::string num = ToString::intTypeToString(e.itemIndex, colorRadix) + ":";

	Pixel colorColor = Pixel(' ', toColor(e.itemIndex, chooseForeground((ConsoleColors)e.itemIndex)));
	g->drawString(Point2I(2, 0), num, textColor, HorizontalAlignments::Right);
	g->drawString(Point2I(4, 0), COLOR_NAMES[e.itemIndex], colorColor, HorizontalAlignments::Left);
}
void AsciiChartWindow::onCharactersPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	for (int i = 0; i < 256; i++) {
		std::string num = ToString::intTypeToString(i, charRadix) + ":";
		int col = i % 10;
		int row = i / 10;
		int x = col * 7;
		int y = row * 2;

		g->drawString(Point2I(x + 3, y), num, textColor, HorizontalAlignments::Right);
		g->setChar(Point2I(x + 5, y), (unsigned char)i);
	}
}
void AsciiChartWindow::onColorsPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	for (int i = 0; i < (int)COLOR_NAMES.size(); i++) {
		std::string num = ToString::intTypeToString(i, colorRadix) + ":";
		int col = i % 5;
		int row = i / 5;
		int x = col * 15;
		int y = row * 2;

		Pixel colorColor = Pixel(' ', toColor((ConsoleColors)i, chooseForeground((ConsoleColors)i)));
		g->drawString(Point2I(x + 2, y), num, textColor, HorizontalAlignments::Right);
		g->drawString(Point2I(x + 4, y), COLOR_NAMES[i], colorColor, HorizontalAlignments::Left);
	}
}
void AsciiChartWindow::onBottomDarkenPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	// shows a darkened border when you're resizing and cutting off information
	if (getActualSize().y < stackPanel->getActualSize().y) {
		PixelAttributes attr = PixelAttributes::Darken;
		if (darkMode)
			attr = PixelAttributes::Lighten;
		g->setSpecial(Point2I::ZERO, Point2I(getActualSize().x, 1), attr);
	}
}
//void AsciiChartWindow::onPaint(ControlSPtr sender, PaintEventArgs& e) {}

#pragma endregion
//======= COMMAND HANDLERS =======
#pragma region Command Handlers

void AsciiChartWindow::onToggleDarkModeCommand(ControlSPtr sender, EventArgs& e) {
	darkMode = !darkMode;
	updateTheme();
	if (darkMode) {
		sendMessage("Theme: dark mode");
	}
	else {
		sendMessage("Theme: light mode");
	}
}

void AsciiChartWindow::onToggleCharRadixCommand(ControlSPtr sender, EventArgs& e) {
	for (int i = 0; i < (int)RADIXES.size(); i++) {
		if (charRadix == RADIXES[i]) {
			charRadix = RADIXES[(i + 1) % (int)RADIXES.size()];
			break;
		}
	}
	sendMessage("Character Radix: " + std::to_string(charRadix));
}
void AsciiChartWindow::onToggleColorRadixCommand(ControlSPtr sender, EventArgs& e) {
	for (int i = 0; i < (int)RADIXES.size(); i++) {
		if (colorRadix == RADIXES[i]) {
			colorRadix = RADIXES[(i + 1) % (int)RADIXES.size()];
			break;
		}
	}
	sendMessage("Color Radix: " + std::to_string(colorRadix));
}

void AsciiChartWindow::onLowerLuminanceCommand(ControlSPtr sender, EventArgs& e) {
	luminanceThreshold = GMath::max(0, luminanceThreshold - 5);
	sendMessage("Color Luminance: " + std::to_string(luminanceThreshold));
}
void AsciiChartWindow::onRaiseLuminanceCommand(ControlSPtr sender, EventArgs& e) {
	luminanceThreshold = GMath::min(100, luminanceThreshold + 5);
	sendMessage("Color Luminance: " + std::to_string(luminanceThreshold));
}

#pragma endregion
//=================================================================|
