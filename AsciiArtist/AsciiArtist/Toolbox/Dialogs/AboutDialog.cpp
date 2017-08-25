/*=================================================================|
|* File:				AboutDialog.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AboutDialog.h"
#include "resources.h"
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/Drawing/ConsoleColors.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::IO;
using namespace AsciiArtist::Toolbox::Dialogs;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const std::string AboutDialog::ABOUT_DESCRIPTION =
	"Ascii Artist, the console paint program.\n"
	"\n"
	"Create images using combinations of characters and colors to use in your console applications.";

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

AboutDialog::AboutDialog()
 :	DialogBox("About Ascii Artist", DialogBoxButtons::OK, DialogBoxIcons::None) {

	wordWrap = WordWrapper::wrap(ABOUT_DESCRIPTION, 1, 40, 40);

	setSizeSafe(Point2I(48, 26));

	eventInitialize().add(bindEvent(AboutDialog::onInitialize));
}
void AboutDialog::onInitialize(ControlSPtr sender, EventArgs& e) {
	groupBoxLogo = makeControl(GroupBox);
	groupBoxLogo->setHeader("");
	groupBoxLogo->setSize(Point2I(13, 9));
	groupBoxLogo->setMargin(Thickness(2, 3, 0, 0));
	children().add(groupBoxLogo);

	groupBoxDetails = makeControl(GroupBox);
	groupBoxDetails->setHeader("Details");
	groupBoxDetails->setSize(Point2I(30, 9));
	groupBoxDetails->setMargin(Thickness(16, 3, 0, 0));
	children().add(groupBoxDetails);

	groupBoxDescription = makeControl(GroupBox);
	groupBoxDescription->setHeader("Description");
	groupBoxDescription->setSize(Point2I(44, 9));
	groupBoxDescription->setMargin(Thickness(2, 13, 0, 0));
	groupBoxDescription->eventPaint().add(bindEvent(AboutDialog::onDescriptionPaint));
	children().add(groupBoxDescription);

	AsciiImage ascLogo = AsciiImage();
	ascLogo.loadResource(ASC_LOGO_SMALL);
	imageLogo = makeControl(Image);
	imageLogo->setImage(std::move(ascLogo));
	imageLogo->setMargin(Thickness(1, 0, 0, 0));
	groupBoxLogo->children().add(imageLogo);

	labelVersion = makeControl(Label);
	labelVersion->setText("Version: " + Path::getFileVersionString(Path::getExecutablePath()));
	labelVersion->setMargin(Thickness(1, 1, 0, 0));
	groupBoxDetails->children().add(labelVersion);
	std::string path = Path::getExecutablePath();
	labelBuild = makeControl(Label);
	labelBuild->setText("Build: " + Path::getFileTimeString(Path::getFileLastModified(path), true, true, true, true));
	labelBuild->setMargin(Thickness(1, 3, 0, 0));
	groupBoxDetails->children().add(labelBuild);

	labelAuthor = makeControl(Label);
	labelAuthor->setText("Author: Robert Jordan");
	labelAuthor->setMargin(Thickness(1, 5, 0, 0));
	groupBoxDetails->children().add(labelAuthor);
}

#pragma endregion
//============ DIALOG ============
#pragma region Dialog

void AboutDialog::show(WindowSPtr owner) {
	auto dialogBox = makePrivateDialog(AboutDialog);
	owner->showDialog(dialogBox);
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void AboutDialog::onDescriptionPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;
	const unsigned char color = toColor(BLACK, GRAY);

	for (int i = 0; i < (int)wordWrap.lines; i++) {
		g->drawString(Point2I(2, 2 + i), ABOUT_DESCRIPTION.substr(wordWrap.lineStarts[i], wordWrap.lineLengths[i]), Pixel(' ', color));
	}
}

#pragma endregion
//=================================================================|
