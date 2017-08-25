/*=================================================================|
|* File:				TetrisWindow.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "AsciiVisWindow.h"
#include <PowerConsole/PowerConsoleSettings.h>
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/DirectoryInfo.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/Console.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <AsciiVis/LoadingDialog.h>
#include <valarray>
#include <fstream>
#include <string.h>
#include <SFML/Audio/Listener.hpp>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
using namespace PowerConsole::Helpers;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

AsciiVisWindow::AsciiVisWindow() {
	srand((unsigned int)time(nullptr));

	eventInitialize().add(bindEvent(AsciiVisWindow::onInitialize));
	eventLoad().add(bindEvent(AsciiVisWindow::onLoad));
	eventTick().add(bindEvent(AsciiVisWindow::onTick));
	eventGlobalTick().add(bindEvent(AsciiVisWindow::onGlobalTick));
	//eventPaint().add(bindEvent(AsciiVisWindow::onPaint));

	PowerConsoleSettings::setDesiredFrameRate(1000 / 60);
	PowerConsoleSettings::setDrawEntireBuffer(true);
	PowerConsoleSettings::setDebugFrameRateColor(toColor(DARK_NAVY, WHITE));
}


void AsciiVisWindow::onInitialize(ControlSPtr sender, EventArgs& e) {
	ResizeHandlePixel resizePixel;
	resizePixel.hoverColor = toColor(BLACK, LIGHT_NAVY);
	resizePixel.resizeColor = toColor(BLACK, LIGHT_NAVY);
	resizePixel.normalColor = toColor(BLACK, DARK_DARK_NAVY);
	setResizeHandlePixel(resizePixel);
	setResizeHandleEnabled(true);
	console().centerWindow();
	console().setTitle("AsciiVis");
	console().setRasterFontSize(RasterFontSizes::R8x8);
	setSize(Point2I(100, 64));
	ConsolePalette palette;
	if (palette.loadResource(CONPAL_PALETTE)) {
		console().setPalette(palette);
	}

	MusicLibrary::initialize();

	splitContainer = makeControl(MySplitContainer);
	splitContainer->setHorizontalAlignment(HorizontalAlignments::Stretch);
	splitContainer->setVerticalAlignment(VerticalAlignments::Stretch);
	splitContainer->setSize(Point2I(SIZE_AUTO));
	splitContainer->setRightFixed(false);
	splitContainer->setLeftMinWidth(SIDE_BAR_MIN_WIDTH);
	splitContainer->setLeftMaxWidth(SIDE_BAR_MAX_WIDTH);
	children().add(splitContainer);

	leftPanel = makeControl(Panel);
	leftPanel->setHorizontalAlignment(HorizontalAlignments::Stretch);
	leftPanel->setVerticalAlignment(VerticalAlignments::Stretch);
	leftPanel->setSize(Point2I(SIZE_AUTO));
	leftPanel->setMargin(Thickness(0, 0, 1, 0));
	leftPanel->eventPaint().add(bindEvent(AsciiVisWindow::onLeftPanelPaint));
	splitContainer->setLeftContent(leftPanel);

	rightPanel = makeControl(Panel);
	rightPanel->setHorizontalAlignment(HorizontalAlignments::Stretch);
	rightPanel->setVerticalAlignment(VerticalAlignments::Stretch);
	rightPanel->setSize(Point2I(SIZE_AUTO));
	rightPanel->eventPaint().add(bindEvent(AsciiVisWindow::onRightPanelPaint));
	splitContainer->setRightContent(rightPanel);

	vis = makeControl(Visualization);
	//vis->setMargin(Thickness(SIDE_BAR_WIDTH + 1, 1, 1, 3));
	vis->setMargin(Thickness(1, 1, 1, 3));
	vis->setHorizontalAlignment(HorizontalAlignments::Stretch);
	vis->setVerticalAlignment(VerticalAlignments::Stretch);
	vis->setSize(Point2I(SIZE_AUTO));
	rightPanel->children().add(vis);

	musicList = makeControl(MusicList);
	musicList->setMargin(Thickness(1, 3, 1, 6));
	musicList->setHorizontalAlignment(HorizontalAlignments::Stretch);
	musicList->setVerticalAlignment(VerticalAlignments::Stretch);
	//musicList->setSize(Point2I(SIDE_BAR_WIDTH - 2, SIZE_AUTO));
	musicList->setSize(Point2I(SIZE_AUTO));
	musicList->eventMusicChanged().add(bindEvent(AsciiVisWindow::onMusicChanged));
	leftPanel->children().add(musicList);

	visSettingsList = makeControl(ScrollList);
	visSettingsList->setMargin(Thickness(1, 3, 1, 6));
	visSettingsList->setHorizontalAlignment(HorizontalAlignments::Stretch);
	visSettingsList->setVerticalAlignment(VerticalAlignments::Stretch);
	//settingsList->setSize(Point2I(SIDE_BAR_WIDTH - 2, SIZE_AUTO));
	visSettingsList->setSize(Point2I(SIZE_AUTO));
	visSettingsList->setVisible(false);
	leftPanel->children().add(visSettingsList);

	settingsList = makeControl(ScrollList);
	settingsList->setMargin(Thickness(1, 3, 1, 6));
	settingsList->setHorizontalAlignment(HorizontalAlignments::Stretch);
	settingsList->setVerticalAlignment(VerticalAlignments::Stretch);
	//settingsList->setSize(Point2I(SIDE_BAR_WIDTH - 2, SIZE_AUTO));
	settingsList->setSize(Point2I(SIZE_AUTO));
	settingsList->setVisible(false);
	leftPanel->children().add(settingsList);

	settings = makeControl(SettingsTab, vis);
	settingsList->setContent(settings);

	now = makeControl(NowPlaying);
	rightPanel->children().add(now);

	seek = makeControl(Seeker);
	seek->setHorizontalAlignment(HorizontalAlignments::Stretch);
	seek->setVerticalAlignment(VerticalAlignments::Bottom);
	//seek->setMargin(Thickness(SIDE_BAR_WIDTH + 2, 0, 2, 1));
	seek->setMargin(Thickness(2, 0, 2, 1));
	seek->setSize(Point2I(SIZE_AUTO, 1));
	rightPanel->children().add(seek);

	volume = makeControl(Slider);
	volume->setSize(Point2I(16, 1));
	volume->setValue(sf::Listener::getGlobalVolume() / 100.0f);
	volume->setMargin(Thickness(0, 0, 0, 5));
	volume->setHorizontalAlignment(HorizontalAlignments::Center);
	volume->setVerticalAlignment(VerticalAlignments::Bottom);
	volume->eventValueChanged().add(bindEvent(AsciiVisWindow::onVolumeChanged));
	leftPanel->children().add(volume);

	stackPanelButtons = makeControl(StackPanel);
	stackPanelButtons->setSize(Point2I(SIZE_AUTO, 3));
	stackPanelButtons->setOrientation(Orientations::Horizontal);
	stackPanelButtons->setMargin(Thickness(0, 0, 0, 1));
	stackPanelButtons->setHorizontalAlignment(HorizontalAlignments::Center);
	stackPanelButtons->setVerticalAlignment(VerticalAlignments::Bottom);
	leftPanel->children().add(stackPanelButtons);

	AsciiAnimation buttonImage;
	//int buttonX = (SIDE_BAR_WIDTH - 5 * 4) / 2;

	buttonImage.loadResource(ASC_BUTTON_STOP);
	buttonStop = makeControl(ImageButton);
	buttonStop->setVerticalAlignment(VerticalAlignments::Bottom);
	//buttonStop->setMargin(Thickness(buttonX, 0, 0, 1));
	buttonStop->setImage(buttonImage);
	buttonStop->eventClick().add(bindEvent(AsciiVisWindow::onStopClicked));
	stackPanelButtons->children().add(buttonStop);
	//buttonX += 5;

	buttonImage.loadResource(ASC_BUTTON_BACKWARD);
	buttonBackward = makeControl(ImageButton);
	buttonBackward->setVerticalAlignment(VerticalAlignments::Bottom);
	//buttonBackward->setMargin(Thickness(buttonX, 0, 0, 1));
	buttonBackward->setImage(buttonImage);
	buttonBackward->eventClick().add(bindEvent(AsciiVisWindow::onBackwardClicked));
	stackPanelButtons->children().add(buttonBackward);
	//buttonX += 5;

	buttonImage.loadResource(ASC_BUTTON_PLAY);
	buttonPlay = makeControl(ImageButton);
	buttonPlay->setVerticalAlignment(VerticalAlignments::Bottom);
	//buttonPlay->setMargin(Thickness(buttonX, 0, 0, 1));
	buttonPlay->setImage(buttonImage);
	buttonPlay->eventClick().add(bindEvent(AsciiVisWindow::onPlayClicked));
	stackPanelButtons->children().add(buttonPlay);
	//buttonX += 5;

	buttonImage.loadResource(ASC_BUTTON_FORWARD);
	buttonForward = makeControl(ImageButton);
	buttonForward->setVerticalAlignment(VerticalAlignments::Bottom);
	//buttonForward->setMargin(Thickness(buttonX, 0, 0, 1));
	buttonForward->setImage(buttonImage);
	buttonForward->eventClick().add(bindEvent(AsciiVisWindow::onForwardClicked));
	stackPanelButtons->children().add(buttonForward);

	buttonImage = AsciiAnimation(Point2I(1, 2), ImageFormats::Animation, Pixel(' ', toColor(DARK_NAVY, LIGHT_LIGHT_NAVY)), 3);
	buttonImage.setName(0, "Normal");
	buttonImage.setName(1, "Hover");
	buttonImage.setName(2, "Pressed");
	auto g = buttonImage.createGraphics();
	g->setCurrentFrame(1); g->fill(Pixel(' ', toColor(DARK_NAVY, WHITE)));
	g->setCurrentFrame(2); g->fill(Pixel(' ', toColor(DARK_NAVY, NAVY)));

	for (int i = 0; i < 3; i++) {
		g->setCurrentFrame(i);
		g->setChar(Point2I::ZERO, 13);
	}
	buttonMusic = makeControl(ImageButton);
	buttonMusic->setMargin(Thickness(0, 4, 0, 0));
	buttonMusic->setImage(buttonImage);
	buttonMusic->eventClick().add(bindEvent(AsciiVisWindow::onTabButtonClicked));
	leftPanel->children().add(buttonMusic);

	for (int i = 0; i < 3; i++) {
		g->setCurrentFrame(i);
		g->setChar(Point2I::ZERO, 247);
	}
	buttonVisSettings = makeControl(ImageButton);
	buttonVisSettings->setMargin(Thickness(0, 6, 0, 0));
	buttonVisSettings->setImage(buttonImage);
	buttonVisSettings->eventClick().add(bindEvent(AsciiVisWindow::onTabButtonClicked));
	leftPanel->children().add(buttonVisSettings);

	for (int i = 0; i < 3; i++) {
		g->setCurrentFrame(i);
		g->setChar(Point2I::ZERO, 15);
	}
	buttonSettings = makeControl(ImageButton);
	buttonSettings->setMargin(Thickness(0, 8, 0, 0));
	buttonSettings->setImage(buttonImage);
	buttonSettings->eventClick().add(bindEvent(AsciiVisWindow::onTabButtonClicked));
	leftPanel->children().add(buttonSettings);

}
void AsciiVisWindow::onLoad(ControlSPtr sender, EventArgs& e) {
	LoadingDialog::show(getWindow(), "Loading songs...", [](LoadingDialogSPtr sender) {
		MusicLibrary::loadSongs();
	});
}

#pragma endregion
//=========== RUNNING ============
#pragma region Running

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void AsciiVisWindow::onTick(ControlSPtr sender, TickEventArgs& e) {

}
void AsciiVisWindow::onGlobalTick(ControlSPtr sender, TickEventArgs& e) {
	//MusicLibrary::updateTitles();
	if (MusicLibrary::getMusic().getStatus() == sf::SoundSource::Status::Stopped && !MusicLibrary::isForceStopped()) {
		MusicLibrary::playNext();
		onMusicChanged(nullptr, EventArgs());
	}
	if (globalKeyboard().isKeyPressed(Keys::Shift)) {
		int newIndex = MusicLibrary::getPlayIndex();
		do {
			newIndex = rand() % (int)MusicLibrary::getSongs().size();
		} while (newIndex == MusicLibrary::getPlayIndex());
		MusicLibrary::setPlayIndex(newIndex);
		MusicLibrary::playNext(false);
		onMusicChanged(nullptr, EventArgs());
	}
	if (globalKeyboard().isKeyPressed(Keys::Space)) {
		MusicLibrary::playNext();
		onMusicChanged(nullptr, EventArgs());
	}
	/*if (globalKeyboard().isKeyPressed(Keys::Enter))
		spectrumMode = !spectrumMode;
	if (globalKeyboard().isKeyPressed(Keys::Backspace))
		expMode = !expMode;
	if (globalKeyboard().isKeyPressed(Keys::OemPipe))
		peakMode = !peakMode;*/
	if (globalKeyboard().isKeyPressed(Keys::Right))
		MusicLibrary::getMusic().setPitch(5.0f);
	else if (globalKeyboard().isKeyReleased(Keys::Right))
		MusicLibrary::getMusic().setPitch(1.0f);

}
/*void AsciiVisWindow::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	// Draw list background
	g->drawRectangle(Rectangle2I(SIDE_BAR_WIDTH + 1, getActualSize().y), Pixel(' ', toColor(DARK_NAVY, DARK_NAVY)), true);

	// Draw AsciiVis Logo
	g->drawString(Point2I(2, 1), "AsciiVis " + std::string(1, (char)249) + "))", Pixel(' ', toColor(DARK_NAVY, WHITE)));

	g->drawBorder(Rectangle2I(SIDE_BAR_WIDTH, 0, getActualSize() - Point2I(SIDE_BAR_WIDTH, 0)), Pixel(' ', toColor(BLACK, DARK_DARK_NAVY)), true);
	g->drawBorder(Rectangle2I(SIDE_BAR_WIDTH, getActualSize().y - 3, getActualSize().x - SIDE_BAR_WIDTH, 1), Pixel(' ', toColor(BLACK, DARK_DARK_NAVY)), true);
}*/
void AsciiVisWindow::onLeftPanelPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	// Draw list background
	g->drawRectangle(Rectangle2I(leftPanel->getActualSize()), Pixel(' ', toColor(DARK_NAVY, DARK_NAVY)), true);

	// Draw AsciiVis Logo
	g->drawString(Point2I(2, 1), "AsciiVis " + std::string(1, (char)249) + "))", Pixel(' ', toColor(DARK_NAVY, WHITE)));
}
void AsciiVisWindow::onRightPanelPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	g->drawBorder(Rectangle2I(rightPanel->getActualSize()), Pixel(' ', toColor(BLACK, DARK_DARK_NAVY)), true);
	g->drawBorder(Rectangle2I(0, rightPanel->getActualSize().y - 3, rightPanel->getActualSize().x, 1), Pixel(' ', toColor(BLACK, DARK_DARK_NAVY)), true);
}
void AsciiVisWindow::onMusicChanged(ControlSPtr sender, EventArgs& e) {
	musicList->updatePlayIndex();
	vis->updatePlayIndex();
	now->updatePlayIndex();
}
void AsciiVisWindow::onStopClicked(ControlSPtr sender, MouseEventArgs& e) {
	MusicLibrary::getMusic().stop();
	MusicLibrary::setForceStopped(true);
}
void AsciiVisWindow::onPlayClicked(ControlSPtr sender, MouseEventArgs& e) {
	if (MusicLibrary::getMusic().getStatus() != sf::SoundSource::Status::Playing)
		MusicLibrary::getMusic().play();
	else
		MusicLibrary::getMusic().pause();
	MusicLibrary::setForceStopped(false);
}
void AsciiVisWindow::onBackwardClicked(ControlSPtr sender, MouseEventArgs& e) {
	MusicLibrary::playPrevious();
	onMusicChanged(nullptr, EventArgs());
}
void AsciiVisWindow::onForwardClicked(ControlSPtr sender, MouseEventArgs& e) {
	MusicLibrary::playNext();
	onMusicChanged(nullptr, EventArgs());
}
void AsciiVisWindow::onTabButtonClicked(ControlSPtr sender, MouseEventArgs& e) {
	if (sender == buttonMusic) {
		musicList->setVisible(true);
		visSettingsList->setVisible(false);
		settingsList->setVisible(false);
	}
	else if (sender == buttonVisSettings) {
		musicList->setVisible(false);
		visSettingsList->setVisible(true);
		settingsList->setVisible(false);
	}
	else {
		musicList->setVisible(false);
		visSettingsList->setVisible(false);
		settingsList->setVisible(true);
	}
}
void AsciiVisWindow::onVolumeChanged(ControlSPtr sender, FloatChangedEventArgs& e) {
	sf::Listener::setGlobalVolume(volume->getValue() * 100.0f);
}

#pragma endregion
//=================================================================|
