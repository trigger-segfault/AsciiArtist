/*=================================================================|
|* File:				AsciiImageFileDialog.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCII_IMAGE_FILE_DIALOG_H
#define ASCII_IMAGE_FILE_DIALOG_H

#include <PowerConsole/Controls/Toolbox/FileDialog.h>

using namespace PowerConsole::IO;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiArtist::Toolbox::Dialogs {
//=================================================================|
// CLASSES														   |
//=================================================================/

class CustomFileDialog : public FileDialog {
public:
	/* Constructs the dialog. */
	CustomFileDialog(FileDialogModes mode);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
};

class OpenCustomFileDialog : public CustomFileDialog {
public:
	/* Constructs the dialog. */
	inline OpenCustomFileDialog()
		: CustomFileDialog(FileDialogModes::Open) {}
};
class SaveCustomFileDialog : public CustomFileDialog {
public:
	/* Constructs the dialog. */
	inline SaveCustomFileDialog()
		: CustomFileDialog(FileDialogModes::Save) {}
};

class ImageFileDialog : public CustomFileDialog {
	void loadFileDetails(FileInfo& file);
	void drawFileDetail(DrawFileDetailArgs& args);

public:
	/* Constructs the dialog. */
	ImageFileDialog(FileDialogModes mode);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
};

class OpenImageFileDialog : public ImageFileDialog {
public:
	/* Constructs the dialog. */
	inline OpenImageFileDialog()
		: ImageFileDialog(FileDialogModes::Open) {}
};
class SaveImageFileDialog : public ImageFileDialog {
public:
	/* Constructs the dialog. */
	inline SaveImageFileDialog()
		: ImageFileDialog(FileDialogModes::Save) {}
};

/* The file dialog for opening and saving ascii images. */
class AsciiImageFileDialog : public CustomFileDialog {
	static const int DETAIL_DIMENSIONS = 1;
	static const int DETAIL_FRAMES = 1;

	void loadFileDetails(FileInfo& file);
	void drawFileDetail(DrawFileDetailArgs& args);

public:
	/* Constructs the dialog. */
	AsciiImageFileDialog(FileDialogModes mode);
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
};

/* The file dialog for opening ascii images. */
class OpenAsciiImageFileDialog : public AsciiImageFileDialog {
public:
	inline OpenAsciiImageFileDialog()
		: AsciiImageFileDialog(FileDialogModes::Open) {}
};

/* The file dialog for saving ascii images. */
class SaveAsciiImageFileDialog : public AsciiImageFileDialog {
public:
	inline SaveAsciiImageFileDialog()
		: AsciiImageFileDialog(FileDialogModes::Save) {}
};

DECLARE_SMART_PTRS(AsciiImageFileDialog);
DECLARE_SMART_PTRS(OpenAsciiImageFileDialog);
DECLARE_SMART_PTRS(SaveAsciiImageFileDialog);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
