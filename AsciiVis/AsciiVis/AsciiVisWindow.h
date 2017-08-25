/*=================================================================|
|* File:				AsciiVisWindow.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef ASCII_VIS_WINDOW_H
#define ASCII_VIS_WINDOW_H

#include <PowerConsole/Controls/MainWindow.h>
#include <PowerConsole/Controls/Toolbox/ImageButton.h>
#include <PowerConsole/Controls/Toolbox/Panel.h>
#include <PowerConsole/Controls/Toolbox/StackPanel.h>
#include <AsciiVis/MusicLibrary.h>
#include <AsciiVis/MusicList.h>
#include <AsciiVis/ScrollList.h>
#include <AsciiVis/Visualization.h>
#include <AsciiVis/NowPlaying.h>
#include <AsciiVis/Seeker.h>
#include <AsciiVis/SettingsTab.h>
#include <AsciiVis/MySplitContainer.h>
#include <AsciiVis/Slider.h>
#include <vector>

using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* The base manager for the game. */
class AsciiVisWindow : public MainWindow {

	//========== CONSTANTS ===========
	#pragma region Constants

	static const int SIDE_BAR_MIN_WIDTH = 24;
	static const int SIDE_BAR_MAX_WIDTH = 40;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	VisualizationSPtr vis;
	MusicListSPtr musicList;
	ScrollListSPtr visSettingsList;
	ScrollListSPtr settingsList;
	NowPlayingSPtr now;
	SeekerSPtr seek;

	SliderSPtr volume;

	SettingsTabSPtr settings;

	MySplitContainerSPtr splitContainer;
	PanelSPtr leftPanel;
	PanelSPtr rightPanel;

	StackPanelSPtr stackPanelButtons;
	ImageButtonSPtr buttonStop;
	ImageButtonSPtr buttonBackward;
	ImageButtonSPtr buttonPlay;
	ImageButtonSPtr buttonForward;

	ImageButtonSPtr buttonMusic;
	ImageButtonSPtr buttonVisSettings;
	ImageButtonSPtr buttonSettings;
	
	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Contructs the tetris manager. */
	AsciiVisWindow();
private:
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//=========== RUNNING ============
	#pragma region Running

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onGlobalTick(ControlSPtr sender, TickEventArgs& e);
	void onLeftPanelPaint(ControlSPtr sender, PaintEventArgs& e);
	void onRightPanelPaint(ControlSPtr sender, PaintEventArgs& e);
	void onMusicChanged(ControlSPtr sender, EventArgs& e);
	void onStopClicked(ControlSPtr sender, MouseEventArgs& e);
	void onBackwardClicked(ControlSPtr sender, MouseEventArgs& e);
	void onPlayClicked(ControlSPtr sender, MouseEventArgs& e);
	void onForwardClicked(ControlSPtr sender, MouseEventArgs& e);
	void onTabButtonClicked(ControlSPtr sender, MouseEventArgs& e);
	void onVolumeChanged(ControlSPtr sender, FloatChangedEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
