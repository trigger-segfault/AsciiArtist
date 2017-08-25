/*=================================================================|
|* File:				Image.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Controls/Control.h>

namespace PowerConsole::Controls::Toolbox {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Image : public Control {
	//=========== MEMBERS ============
	#pragma region Members

	/* The image to display. */
	PcDraw::AsciiImage image;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	Image();

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	/* Gets the image to display. */
	const PcDraw::AsciiImageFrame& getImage() const;
	/* Sets the image to display. */
	void setImage(const PcDraw::AsciiImageFrame& image);
	/* Sets the image to display. */
	void setImage(PcDraw::AsciiImageFrame&& image);

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

DECLARE_SMART_PTRS(Image);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
