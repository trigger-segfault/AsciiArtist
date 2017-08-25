/*=================================================================|
|* File:				NowPlaying.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef NOW_PLAYING_H
#define NOW_PLAYING_H

#include <PowerConsole/Controls/Control.h>
#include <AsciiVis/Colors.h>

using namespace PowerConsole::Controls;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

class NowPlaying : public Control {

	//========== CONSTANTS ===========
	#pragma region Constants

	static const int MAX_WIDTH = 26;
	static const int SCROLL_DELAY = 4000;
	static const int SCROLL_SPEED = 120;
	static const int SCROLL_SEPARATION = 4;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	std::string title;
	std::string artist;
	std::string album;

	int scrollLength;

	int timerTitle;
	int timerArtist;
	int timerAlbum;

	bool waitingForMetadata;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	NowPlaying();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	void updatePlayIndex();

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(NowPlaying);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
