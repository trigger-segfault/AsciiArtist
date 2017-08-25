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
#include <AsciiArtist/COM/WIC.h>

using namespace AsciiArtist::COM;
using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Controls;
using namespace PowerConsole::Controls::Toolbox;

namespace AsciiArtist::Editing {
//=================================================================|
// CLASSES														   |
//=================================================================/

enum class ColorMetrics {
	Euclidean = 0,
	LABDeltaE = 1
};

class Asciifier {
public:
	static AsciiAnimationSPtr asciifyImage(const std::string& path, ColorMetrics metric, ProgressDialogSPtr progressDialog);
	static AsciiAnimationSPtr asciifyImagePrecision(BitmapFrameSPtr bitmap, Point2I split, ColorMetrics metric, bool& canceled, ProgressDialogSPtr progressDialog);
	static bool writeBitmap(const std::string& path, const AsciiImageFrame& frame, Point2I finalSize = Point2I::ZERO);
	static bool writeGif(const std::string& path, const AsciiAnimation& animation);
	static bool writeVideo(const std::string& path, const AsciiAnimation& animation, bool& canceled, ProgressDialogSPtr progressDialog);
private:
	static void writeFrameToBitmap(BitmapFrameSPtr bmp, BitmapFrameSPtr fontBmp, Point2I fontSize, const ConsolePalette& palette, const AsciiImageFrame& frame);
	static void updateAsciifierProgress(int x, int width, int time, ProgressDialogSPtr progressDialog);
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
