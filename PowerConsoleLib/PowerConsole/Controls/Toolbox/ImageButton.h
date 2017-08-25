/*=================================================================|
|* File:				ImageButton.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef IMAGE_BUTTON_H
#define IMAGE_BUTTON_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class ImageButton : public Control {

	//=========== MEMBERS ============
	#pragma region Members

	/* The images for the button */
	PcDraw::AsciiAnimation image;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the iamge button. */
	ImageButton();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the images for the button. */
	const PcDraw::AsciiAnimation& getImage() const;
	/* Sets the text of the button. */
	void setImage(const PcDraw::AsciiAnimation& image);
	/* Sets the text of the button. */
	void setImage(PcDraw::AsciiAnimation&& image);

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

DECLARE_SMART_PTRS(ImageButton);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
