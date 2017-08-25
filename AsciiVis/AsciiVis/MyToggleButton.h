/*=================================================================|
|* File:				MyToggleButton.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef MY_TOGGLE_BUTTON_H
#define MY_TOGGLE_BUTTON_H

#include <PowerConsole/Controls/Control.h>

using namespace PowerConsole::Controls;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

class MyToggleButton : public Control {

	//=========== MEMBERS ============
	#pragma region Members

	/* The text used by the button. */
	std::string text;
	/* The alignment of the text. */
	PcGeo::HorizontalAlignments textAlignment;
	/* True if the button is checked. */
	bool checked;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the toggle button. */
	MyToggleButton();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the text of the button. */
	std::string getText() const;
	/* Sets the text of the button. */
	void setText(const std::string& text);
	/* Gets the alignment of the text. */
	PcGeo::HorizontalAlignments getTextAlignment() const;
	/* Gets the alignment of the text. */
	void setTextAlignment(PcGeo::HorizontalAlignments textAlignment);
	/* Gets the button is checked. */
	bool isChecked() const;
	/* Sets the button is checked. */
	void setChecked(bool checked);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const override;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onClick(ControlSPtr sender, MouseEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(MyToggleButton);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
