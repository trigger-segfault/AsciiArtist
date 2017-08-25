/*=================================================================|
|* File:				AsciiImageFileDialog.cpp				  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AsciiImageFileDialog.h"
#include <PowerConsole/Drawing/AsciiImageFileDetails.h>
#include <PowerConsole/IO/Path.h>
#include <AsciiArtist/Settings.h>
#include <AsciiArtist/COM/WIC.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
using namespace AsciiArtist;
using namespace AsciiArtist::COM;
using namespace AsciiArtist::Toolbox::Dialogs;
//=================================================================|
// CLASSES														   |
//=================================================================/

CustomFileDialog::CustomFileDialog(FileDialogModes mode) 
 : FileDialog(mode) {
	eventInitialize().add(bindEvent(CustomFileDialog::onInitialize));
}
void CustomFileDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	auto section = Settings::get()->get("CustomPlaces");
	for (int i = 0; i < section->count(); i++) {
		auto property = section->getProperties()[i];
		std::string path = property->getString();
		if (path.empty())
			path = Path::getDirectory(Path::getExecutablePath());
		addCustomPlace(path, property->getKey());
	}
	section = Settings::get()->get("CustomLibraries");
	for (int i = 0; i < section->count(); i++) {
		auto property = section->getProperties()[i];
		std::string path = Path::combine(Path::GetUserDirectory(), property->getString());
		addLibrary(path, property->getKey());
	}
}

ImageFileDialog::ImageFileDialog(FileDialogModes mode)
	: CustomFileDialog(mode) {
	eventInitialize().add(bindEvent(ImageFileDialog::onInitialize));
}

void ImageFileDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	setDefaultExt(".png");
	addFilter(".bmp");
	addFilter(".png");
	addFilter(".gif");
	addFilter(".jpg");
	addFilter(".jpeg");
	addFilter(".tif");
	addFilter(".tiff");
	removeColumn(FileDialog::DETAIL_FILE_MODIFIED);
	//setColumnWidth(FileDialog::DETAIL_FILE_NAME, 42);
	insertColumn(1, DetailColumn("Dimens.", 2, 17, 7));
	//insertColumn(2, DetailColumn("Frames", 3, 12, 8));
	setLoadFileDetailsFunction(std::bind(&ImageFileDialog::loadFileDetails, this, std::placeholders::_1));
	setDrawFileDetailFunction(std::bind(&ImageFileDialog::drawFileDetail, this, std::placeholders::_1));
}

void ImageFileDialog::loadFileDetails(FileInfo& file) {
	auto size = std::make_shared<Point2I>(Point2I::ZERO);
	*size = WIC::readImageSizeFromFile(file.path);
	if (*size != Point2I::ZERO) {
		file.details = size;
	}
	else {
		file.attributes |= FileAttributes::Invalid;
	}
}
void ImageFileDialog::drawFileDetail(DrawFileDetailArgs& args) {
	if (!args.file->isInvalid()) {
		auto size = std::static_pointer_cast<Point2I>(args.file->details);
		if (args.detailId == 2) {
			args.outString = std::to_string(size->x) + " x " + std::to_string(size->y);
		}
		/*else if (args.detailId == 3) {
			args.outString = std::to_string(details->frameCount) + " frame" + (details->frameCount != 1 ? "s" : "");
		}*/
	}
}

AsciiImageFileDialog::AsciiImageFileDialog(FileDialogModes mode)
	: CustomFileDialog(mode) {
	eventInitialize().add(bindEvent(AsciiImageFileDialog::onInitialize));
}

void AsciiImageFileDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	setDefaultExt(".asc");
	addFilter(".asc");
	removeColumn(FileDialog::DETAIL_FILE_MODIFIED);
	setColumnWidth(FileDialog::DETAIL_FILE_NAME, 42);
	insertColumn(1, DetailColumn("Dimens.", 2, 10, 7));
	insertColumn(2, DetailColumn("Frames", 3, 12, 8));
	setLoadFileDetailsFunction(std::bind(&AsciiImageFileDialog::loadFileDetails, this, std::placeholders::_1));
	setDrawFileDetailFunction(std::bind(&AsciiImageFileDialog::drawFileDetail, this, std::placeholders::_1));
}

void AsciiImageFileDialog::loadFileDetails(FileInfo& file) {
	auto details = std::make_shared<AsciiImageFileDetails>();
	if (details->load(file.path)) {
		file.details = details;
	}
	else {
		file.attributes |= FileAttributes::Invalid;
	}
}
void AsciiImageFileDialog::drawFileDetail(DrawFileDetailArgs& args) {
	if (!args.file->isInvalid()) {
		auto details = std::static_pointer_cast<AsciiImageFileDetails>(args.file->details);
		if (args.detailId == 2) {
			args.outString = std::to_string(details->size.x) + "x" + std::to_string(details->size.y);
		}
		else if (args.detailId == 3) {
			args.outString = std::to_string(details->frameCount) + " frame" + (details->frameCount != 1 ? "s" : "");
		}
	}
}

//=================================================================|
