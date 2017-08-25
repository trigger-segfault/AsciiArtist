/*=================================================================|
|* File:				Asciifier.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef ASCIIFIER_H
#define ASCIIFIER_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <PowerConsole/Controls/Toolbox/ProgressDailog.h>
#include <AsciiArtist/Editing/WIC.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiArtist::Editing {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum class ColorMetrics {
	Euclidean,
	LABDeltaE
};

class Asciifier {
public:
	static AsciiAnimationSPtr asciifyImage(const std::string& path, ColorMetrics metric, ProgressDialogSPtr progressDialog);
	static AsciiAnimationSPtr asciifyImagePrecision(const std::string& path, Point2I split, ColorMetrics metric, ProgressDialogSPtr progressDialog);
private:
	static void updateAsciifierProgress(int x, int width, int time, ProgressDialogSPtr progressDialog);
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
