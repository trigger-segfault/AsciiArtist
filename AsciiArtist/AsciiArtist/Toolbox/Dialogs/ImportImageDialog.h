/*=================================================================|
|* File:				ImportImageDialog.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef IMPORT_IMAGE_DIALOG_H
#define IMPORT_IMAGE_DIALOG_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Helpers/WordWrapper.h>
#include <PowerConsole/Controls/Toolbox/DialogBox.h>
#include <PowerConsole/Controls/Toolbox/Label.h>
#include <PowerConsole/Controls/Toolbox/ListBox.h>
#include <PowerConsole/Controls/Toolbox/NumericTextBox.h>
#include <PowerConsole/Controls/Toolbox/GroupBox.h>
#include <AsciiArtist/Editing/Asciifier.h>

using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;
using namespace AsciiArtist::Editing;

namespace AsciiArtist::Toolbox::Dialogs {
//=================================================================|
// CLASSES														   |
//=================================================================/

struct ImportImageSettings {
	/* The new scale of the image. */
	float scale;
	/* The split of the characters. */
	Point2I split;
	/* The color metrics to use for measuring color difference. */
	ColorMetrics metric;
	/* The size of the image. */
	Point2I imageSize;
	/* The size of the font. */
	Point2I fontSize;

	inline ImportImageSettings()
		: scale(1.0f), split(Point2I::ONE), metric(ColorMetrics::LABDeltaE),
		imageSize(Point2I::ZERO), fontSize(Point2I::ZERO) {}
};

class ImportImageDialog : public DialogBox {
	//========== CONSTANTS ===========
	#pragma region Constants

	/* The notice to show in the window. */
	static const std::string ASCIIFY_NOTICE;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	// Setup
	/* The asciify settings to return on success. */
	ImportImageSettings importResult;
	/* The data for how to draw the notice with wrapping. */
	WordWrapResult wordWrap;

	// Controls
	GroupBoxSPtr groupBoxColorMetrics;
	GroupBoxSPtr groupBoxPrecision;
	GroupBoxSPtr groupBoxImage;
	GroupBoxSPtr groupBoxNotice;

	LabelSPtr labelFontSize;
	LabelSPtr labelX;
	LabelSPtr labelScale;
	LabelSPtr labelPercent;
	LabelSPtr labelImageSize;
	LabelSPtr labelMaxSize;

	ListBoxSPtr listBoxMetric;

	IntTextBoxSPtr textBoxPrecisionX;
	IntTextBoxSPtr textBoxPrecisionY;
	IntTextBoxSPtr textBoxScale;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	// Yes this is supposed to be private.
	/* Constructs the dialog. */
	ImportImageDialog(ImportImageSettings asciifyResult);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//============ DIALOG ============
	#pragma region Dialog
public:
	/* Shows a resize dialog box with the specified settings. */
	static bool show(WindowSPtr owner, ImportImageSettings& asciifyResult);

	#pragma endregion
	//=========== HELPERS ============
	#pragma region Helpers
protected:
	/* Checks if an action should be performed on a dialog button press. */
	/* Return false if the dialog box should not close. */
	bool buttonBehavior(DialogBoxResults buttonPressed) final;

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onPrecisionGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onImageGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onNoticeGroupBoxPaint(ControlSPtr sender, PaintEventArgs& e);
	void onScaleValueChanged(ControlSPtr sender, IntChangedEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
