/*=================================================================|
|* File:				Label.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef MY_LABEL_H
#define MY_LABEL_H

#include <PowerConsole/Controls/Control.h>

using namespace PowerConsole::Controls;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

class MyLabel : public Control {

	//=========== MEMBERS ============
	#pragma region Members

	/* The text used by the label. */
	std::string text;
	/* The alignment of the text. */
	PcGeo::HorizontalAlignments textAlignment;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the label. */
	MyLabel();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the text of the label. */
	std::string getText() const;
	/* Sets the text of the label. */
	void setText(const std::string& text);
	/* Gets the alignment of the text. */
	PcGeo::HorizontalAlignments getTextAlignment() const;
	/* Gets the alignment of the text. */
	void setTextAlignment(PcGeo::HorizontalAlignments textAlignment);

	#pragma endregion
	//=========== UPDATING ===========
	#pragma region Updating

	/* Gets the auto size of the control when not stretching. */
	PcGeo::Point2I autoSizeBehavior() const override;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(MyLabel);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
