/*=================================================================|
|* File:				TextSlider.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TEXT_SLIDER_H
#define TEXT_SLIDER_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>

using namespace PowerConsole::Controls;

namespace AsciiTetris::UI {
//=================================================================|
// CLASSES														   |
//=================================================================/

class TextSlider : public Control {

	//========== CONSTANTS ===========
	#pragma region Constants

	/* The animation length for the zoom in and out of the slide. */
	static const int ZOOM_ANIM_LENGTH = 200 / (1000 / 60);
	/* The animation length for sliding across the screen. */
	static const int SLIDE_ANIM_LENGTH = 600 / (1000 / 60);
	/* The extra animation delay between each row of text. */
	static const int EXTRA_ANIM_DELAY = 100 / (1000 / 60);

	/* The distance passed during the zoom in one direction. */
	static const float ZOOM_DISTANCE;
	/* The distance passed during the slide. */
	static const float SLIDE_DISTANCE;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	/* The text used for the animation. */
	std::string text;
	/* The timer for the text. */
	int timer;

	// Events
	/* Called when the animation finshes. */
	Event<EventArgs> _eventAnimationFinished;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	TextSlider();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the text used for the animation. */
	std::string getText() const;
	/* Returns true if the text slider is animating. */
	bool isAnimating() const;

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Starts the animation with the specified text. */
	void start(const std::string& text);
	/* Restarts the animation with the current text. */
	void restart();

	#pragma endregion
	//============ EVENTS ============
	#pragma region Events
public:
	/* Called when the animation finshes. */
	Event<EventArgs>& eventAnimationFinished();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(TextSlider);


//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
