/*=================================================================|
|* File:				Asciifier.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Asciifier.h"
#include "resource.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Graphics.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/IO/ResourceStream.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/IO/System.h>
#include <AsciiArtist/COM/WIC.h>
#include <PowerConsole/Console.h>

using namespace PowerConsole;
using namespace PowerConsole::IO;
using namespace PowerConsole::Helpers;
using namespace AsciiArtist::Editing;
using namespace AsciiArtist::COM;
//=================================================================|
// CLASSES														   |
//=================================================================/

//http://www.csie.ntu.edu.tw/~b92069/HWs/rgb2lab.c
float H(float q) {
	float value;
	if (q > 0.008856f) {
		value = std::pow(q, 0.333333f);
		return value;
	}
	else {
		value = 7.787f * q + 0.137931f;
		return value;
	}
}
ColorF RGB2LAB(ColorI color) {
	float RGB[3];
	float XYZ[3];
	float adapt[3];

	adapt[0] = 0.950467f;
	adapt[1] = 1.000000f;
	adapt[2] = 1.088969f;

	RGB[0] = color.r * 0.003922f;
	RGB[1] = color.g * 0.003922f;
	RGB[2] = color.b * 0.003922f;

	XYZ[0] = 0.412424f * RGB[0] + 0.357579f * RGB[1] + 0.180464f * RGB[2];
	XYZ[1] = 0.212656f * RGB[0] + 0.715158f * RGB[1] + 0.0721856f * RGB[2];
	XYZ[2] = 0.0193324f * RGB[0] + 0.119193f * RGB[1] + 0.950444f * RGB[2];

	return ColorF(
		116 * H(XYZ[1] / adapt[1]) - 16,
		500 * (H(XYZ[0] / adapt[0]) - H(XYZ[1] / adapt[1])),
		200 * (H(XYZ[1] / adapt[1]) - H(XYZ[2] / adapt[2]))
	);
}
float LAB2Value(ColorF color, ColorF color2) {
	return std::pow((color.r - color2.r), 2) + std::pow((color.g - color2.g), 2) + std::pow((color.b - color2.b), 2);
}

#define TOTAL_CHARACTER_VALUE 96
#define cint int

#define xyToIndex(x, y, width) ((x) + ((y) * (width)))

AsciiAnimationSPtr Asciifier::asciifyImage(const std::string& path, ColorMetrics metric, ProgressDialogSPtr progressDialog) {
	#pragma region Init A
	int totalTime = clock();
	progressDialog->setActionMessage("Reading console font...");
	if (progressDialog->update()) {
		return nullptr;
	}
	CoInitialize(nullptr);
	auto bmp = WIC::readBitmapFromResource(PNG_FONT, "PNG");
	#pragma endregion

	cint characterValues[256] = { 0 };
	for (int x = 0; x < bmp->size.x; x++) {
		for (int y = 0; y < bmp->size.y; y++) {
			if (bmp->pixels[xyToIndex(x, y, bmp->size.x)].r == 255) {
				int charIndex = (x / 8) + (y / 12) * 16;
				characterValues[charIndex]++;
			}
		}
	}
	bmp = nullptr;

	#pragma region Init B
	progressDialog->setActionMessage("Reading image...");
	if (progressDialog->update()) {
		return nullptr;
	}

	bmp = WIC::readBitmapFromFile(path);

	Point2I imageSize = (Point2I)GMath::ceil((Point2F)bmp->size / Point2F(8.0f, 12.0f));
	int totalImageSize = imageSize.x * imageSize.y;
	auto image = std::make_shared<AsciiAnimation>(imageSize, ImageFormats::Basic, Pixel());

	image->resize(imageSize);
	auto graphics = image->createGraphics();

	ColorI palette[16] = {
		ColorI(0,   0,   0),
		ColorI(0,   0, 128),
		ColorI(0, 128,   0),
		ColorI(0, 128, 128),
		ColorI(128,   0,   0),
		ColorI(128,   0, 128),
		ColorI(128, 128,   0),
		ColorI(192, 192, 192),
		ColorI(128, 128, 128),
		ColorI(0,   0, 255),
		ColorI(0, 255,   0),
		ColorI(0, 255, 255),
		ColorI(255,   0,   0),
		ColorI(255,   0, 255),
		ColorI(255, 255,   0),
		ColorI(255, 255, 255)
	};
	#pragma endregion

	#pragma region Init C
	progressDialog->setActionMessage("Calculating character LABs...");
	if (progressDialog->update()) {
		return nullptr;
	}
	ColorF characterLABs[USHRT_MAX];

	for (int col = 1; col < 255; col++) {
		unsigned char f = getFColor(col);
		unsigned char b = getBColor(col);
		if (f == b)
			continue;
		ColorI fcolor = palette[f];
		ColorI bcolor = palette[b];
		for (int ch = 1; ch < 255; ch++) {
			if (ch == 8 || ch == 10 || ch == 219 || ch == 222 || ch == 223)
				continue;

			ColorI characterValue = (
				fcolor * characterValues[ch] + 
				bcolor * (TOTAL_CHARACTER_VALUE - characterValues[ch])
			) / TOTAL_CHARACTER_VALUE;
			characterLABs[col + (ch << 8)] = RGB2LAB(characterValue);
		}
	}
	#pragma endregion


	updateAsciifierProgress(-1, imageSize.x, clock() - totalTime, progressDialog); 
	if (progressDialog->update()) {
		return nullptr;
	}
	for (int imgx = 0; imgx < imageSize.x; imgx++) {
		for (int imgy = 0; imgy < imageSize.y; imgy++) {

			#pragma region Read Image Values
			ColorI imageValue;
			cint r = 0, g = 0, b = 0;
			cint totalRead = 0;
			int ix = imgx * 8;
			int iy = imgy * 12;
			for (int px = 0; px < 8; px++) {
				int p_x = ix + px;
				if (p_x >= bmp->size.x)
					continue;
				for (int py = 0; py < 12; py++) {
					int p_y = iy + py;
					if (p_y >= bmp->size.y)
						continue;
					int index = xyToIndex(p_x, p_y, bmp->size.x);
					r += bmp->pixels[index].r;
					g += bmp->pixels[index].g;
					b += bmp->pixels[index].b;
					totalRead++;
				}
			}
			imageValue = ColorI(
				r / totalRead,
				g / totalRead,
				b / totalRead
			);
			ColorF imageLAB = RGB2LAB(imageValue);
			#pragma endregion

			Pixel closestPixel = Pixel();
			float closestScore = std::numeric_limits<float>().max();
			for (int col = 1; col < 255; col++) {
				unsigned char f = getFColor(col);
				unsigned char b = getBColor(col);
				if (f == b)
					continue;
				ColorI fcolor = palette[f];
				ColorI bcolor = palette[b];
				for (int ch = 1; ch < 255; ch++) {
					if (ch == 8 || ch == 10 || ch == 219 || ch == 222 || ch == 223)
						continue;

					#pragma region Calc Font Values
					/*ColorI fvalue = fcolor * characterValues[ch];
					ColorI bvalue = bcolor * (TOTAL_CHARACTER_VALUE - characterValues[ch]);
					ColorI value = (fvalue + bvalue) / TOTAL_CHARACTER_VALUE;
					fvalue /= TOTAL_CHARACTER_VALUE;
					bvalue /= TOTAL_CHARACTER_VALUE;*/
					#pragma endregion

					#pragma region Calc Score
					float score;
					/*if (metric == ColorMetrics::Euclidean) {
						score =
						std::pow(
							float((value.r - imageValue.r) * (value.r - imageValue.r)) +
							float((value.g - imageValue.g) * (value.g - imageValue.g)) +
							float((value.b - imageValue.b) * (value.b - imageValue.b)),
						2) +
						std::pow(
							float((fvalue.r - imageValue.r) * (fvalue.r - imageValue.r)) +
							float((fvalue.g - imageValue.g) * (fvalue.g - imageValue.g)) +
							float((fvalue.b - imageValue.b) * (fvalue.b - imageValue.b)),
						2) +
						std::pow(
							float((bcolor.r - imageValue.r) * (bcolor.r - imageValue.r)) +
							float((bcolor.g - imageValue.g) * (bcolor.g - imageValue.g)) +
							float((bcolor.b - imageValue.b) * (bcolor.b - imageValue.b)),
						2);
					}
					else {*/
						//score = RGB2LAB(value.r, value.g, value.b, imageValue.r, imageValue.g, imageValue.b);
						score = LAB2Value(imageLAB, characterLABs[col + (ch << 8)]);
					//}
						
					#pragma endregion

					if (score < closestScore) {
						closestPixel = Pixel((unsigned char)ch, (unsigned char)col);
						closestScore = score;
					}
				}
			}
			graphics->setPixel(Point2I(imgx, imgy), closestPixel);
		}

		updateAsciifierProgress(imgx, imageSize.x, clock() - totalTime, progressDialog);
		if (progressDialog->update()) {
			return nullptr;
		}
	}

	#pragma region Cleanup
	progressDialog->setPercentage(100);
	progressDialog->setActionMessage("Completed");
	if (progressDialog->update()) {
		return nullptr;
	}
	else {
		System::sleep(200);
		progressDialog->close();

		return image;
	}
	#pragma endregion
};

#undef TOTAL_CHARACTER_VALUE

#define init2DArray(name, type, width, height) \
	name = new type*[width]; \
	for (int _i = 0; _i < width; _i++) {\
		name[_i] = new type[height]; \
		for (int _j = 0; _j < height; _j++) \
			name[_i][_j] = type(); \
	}
#define init1DArray(name, type, size) \
	name = new type[size]; \
	for (int _i = 0; _i < size; _i++) \
		name[_i] = type();

#define cleanupVar(name) if (name != nullptr) {delete name; name = nullptr;}
#define cleanup1DArray(name) if (name != nullptr) {delete[] name; name = nullptr;}
#define cleanup2DArray(name, width) \
	if (name != nullptr) { \
		for (int _i = 0; _i < width; _i++) \
			delete[] name[_i]; \
		delete[] name; name = nullptr; \
	}
#define cleanup() \
	cleanup2DArray(characterCounts, NUM_CHARACTERS); \
	cleanup2DArray(characterCounts, NUM_CHARACTERS); \
	cleanup2DArray(characterValues, NUM_CHARACTERS * NUM_COLORS); \
	cleanup2DArray(characterFValues, NUM_CHARACTERS * NUM_COLORS); \
	cleanup2DArray(characterBValues, NUM_CHARACTERS * NUM_COLORS); \
	cleanup2DArray(characterLABs, NUM_CHARACTERS * NUM_COLORS); \
	cleanup1DArray(imageValues); \
	cleanup1DArray(imageLABs);

#define updateProgress() \
	if (progressDialog->update()) { \
		canceled = true; \
		cleanup(); \
		return nullptr; \
	}

#define SPLIT_X split.x
#define SPLIT_Y split.y
#define SPLIT_MAG (SPLIT_X * SPLIT_Y)
#define TOTAL_CHARACTER_VALUE (fontMag / SPLIT_MAG)
#define NUM_CHARACTERS 256
#define NUM_COLORS 256

AsciiAnimationSPtr Asciifier::asciifyImagePrecision(BitmapFrameSPtr bmp, Point2I split, ColorMetrics metric, bool& canceled, ProgressDialogSPtr progressDialog) {
	#pragma region Declarations
	int totalTime = clock();
	BitmapFrameSPtr fontBmp = nullptr;

	int** characterCounts = nullptr;
	ColorI** characterValues = nullptr;
	ColorI** characterFValues = nullptr;
	ColorI** characterBValues = nullptr;
	ColorF** characterLABs = nullptr;
	ColorI* imageValues = nullptr;
	ColorF* imageLABs = nullptr;

	init2DArray(characterCounts, int, NUM_CHARACTERS, SPLIT_MAG);
	if (metric == ColorMetrics::Euclidean) {
		init2DArray( characterValues, ColorI, NUM_CHARACTERS * NUM_COLORS, SPLIT_MAG);
		init2DArray(characterFValues, ColorI, NUM_CHARACTERS * NUM_COLORS, SPLIT_MAG);
		init2DArray(characterBValues, ColorI, NUM_CHARACTERS * NUM_COLORS, SPLIT_MAG);
		init1DArray(imageValues, ColorI, SPLIT_MAG);
	}
	else {
		init2DArray(characterLABs, ColorF, NUM_CHARACTERS * NUM_COLORS, SPLIT_MAG);
		init1DArray(imageLABs, ColorF, SPLIT_MAG);
	}
	#pragma endregion

	#pragma region Init A
	progressDialog->setActionMessage("Reading console font image...");
	updateProgress();
	unsigned int pngId = PNG_FONT_8X12;
	Point2I fontSize = Point2I(8, 12);// Console::getFontSize();
	if (Console::getFontType() == ConsoleFonts::Terminal) {
		fontSize = Console::getFontSize();
		switch (Console::getRasterFontSize()) {
		case RasterFontSizes::R4x6:   pngId = PNG_FONT_4X6; break;
		case RasterFontSizes::R6x8:   pngId = PNG_FONT_6X8; break;
		case RasterFontSizes::R8x8:   pngId = PNG_FONT_8X8; break;
		case RasterFontSizes::R16x8:  pngId = PNG_FONT_16X8; break;
		case RasterFontSizes::R5x12:  pngId = PNG_FONT_5X12; break;
		case RasterFontSizes::R7x12:  pngId = PNG_FONT_7X12; break;
		case RasterFontSizes::R8x12:  pngId = PNG_FONT_8X12; break;
		case RasterFontSizes::R16x12: pngId = PNG_FONT_16X12; break;
		case RasterFontSizes::R12x16: pngId = PNG_FONT_12X16; break;
		case RasterFontSizes::R10x18: pngId = PNG_FONT_10X18; break;
		default:
			fontSize = Point2I(8, 12);
		}
	}
	int fontMag = fontSize.x * fontSize.y;

	fontBmp = WIC::readBitmapFromResource(pngId, "PNG");
	if (fontBmp == nullptr) {
		return nullptr;
	}
	#pragma endregion

	#pragma region Read Character Values
	progressDialog->setActionMessage("Reading console font values...");
	updateProgress();
	for (int x = 0; x < fontBmp->size.x; x++) {
		for (int y = 0; y < fontBmp->size.y; y++) {
			if (fontBmp->pixels[xyToIndex(x, y, fontBmp->size.x)].r == 255) {
				int charIndex = (x / fontSize.x) + (y / fontSize.y) * 16;
				int subIndex = (x % fontSize.x * SPLIT_X / fontSize.x) + (y % fontSize.y * SPLIT_Y / fontSize.y) * SPLIT_X;
				characterCounts[charIndex][subIndex]++;
			}
		}
	}
	fontBmp = nullptr;
	#pragma endregion

	#pragma region Init B
	Point2I imageSize = (Point2I)GMath::ceil((Point2F)bmp->size / (Point2F)fontSize);
	int totalImageSize = imageSize.x * imageSize.y;
	auto image = std::make_shared<AsciiAnimation>(imageSize, ImageFormats::Basic, Pixel());

	image->resize(imageSize);
	auto graphics = image->createGraphics();

	ConsolePalette palette = Console::getPalette();
	#pragma endregion

	#pragma region Init C
	progressDialog->setActionMessage((std::string)"Calculating character " + (metric == ColorMetrics::LABDeltaE ? "LABs" : "values") + "...");
	updateProgress();

	for (int col = 1; col < 255; col++) {
		unsigned char f = getFColor(col);
		unsigned char b = getBColor(col);
		if (f == b)
			continue;
		ColorI fcolor = palette[f];
		ColorI bcolor = palette[b];
		for (int ch = 1; ch < 255; ch++) {
			if (ch == 8 || ch == 10 || ch == 219 || ch == 222 || ch == 223)
				continue;
			for (int s = 0; s < SPLIT_MAG; s++) {
				if (metric == ColorMetrics::Euclidean) {
					ColorI fvalue = fcolor * characterCounts[ch][s];
					ColorI bvalue = bcolor * (TOTAL_CHARACTER_VALUE - characterCounts[ch][s]);
					ColorI value = (fvalue + bvalue) / TOTAL_CHARACTER_VALUE;
					fvalue /= TOTAL_CHARACTER_VALUE;
					bvalue /= TOTAL_CHARACTER_VALUE;
					int charIndex = col + (ch << 8);
					characterValues [charIndex][s] =  value;
					characterFValues[charIndex][s] = fvalue;
					characterBValues[charIndex][s] = bvalue;
				}
				else {
					ColorI characterValue = (
						fcolor * characterCounts[ch][s] +
						bcolor * (TOTAL_CHARACTER_VALUE - characterCounts[ch][s])
					) / TOTAL_CHARACTER_VALUE;
					characterLABs[col + (ch << 8)][s] = RGB2LAB(characterValue);
				}
			}
		}
	}
	#pragma endregion

	updateAsciifierProgress(-1, imageSize.x, clock() - totalTime, progressDialog);
	updateProgress();
	for (int imgx = 0; imgx < imageSize.x; imgx++) {
		for (int imgy = 0; imgy < imageSize.y; imgy++) {

			#pragma region Read Image Values
			int ix = imgx * fontSize.x;
			int iy = imgy * fontSize.y;
			for (int s = 0; s < SPLIT_MAG; s++) {
				ColorI imageValue;
				cint totalRead = 0;
				int sx = ix + (s % SPLIT_X * fontSize.x / SPLIT_X);
				int sy = iy + (s / SPLIT_X * fontSize.y / SPLIT_Y);
				for (int px = 0; px < fontSize.x / SPLIT_X; px++) {
					int p_x = sx + px;
					if (p_x >= bmp->size.x)
						continue;
					for (int py = 0; py < fontSize.y / SPLIT_Y; py++) {
						int p_y = sy + py;
						if (p_y >= bmp->size.y)
							continue;
						imageValue += bmp->pixels[xyToIndex(p_x, p_y, bmp->size.x)];
						totalRead++;
					}
				}
				if (totalRead > 0) {
					if (metric == ColorMetrics::Euclidean)
						imageValues[s] = imageValue / totalRead;
					else
						imageLABs[s] = RGB2LAB(imageValue / totalRead);
				}
				else {
					if (metric == ColorMetrics::Euclidean)
						imageValues[s] = ColorI();
					else
						imageLABs[s] = ColorF();
				}
			}
			#pragma endregion

			Pixel closestPixel = Pixel();
			float closestScore = std::numeric_limits<float>().max();
			for (int col = 1; col < 255; col++) {
				unsigned char f = getFColor(col);
				unsigned char b = getBColor(col);
				if (f == b)
					continue;
				ColorI fcolor = palette[f];
				ColorI bcolor = palette[b];
				for (int ch = 1; ch < 255; ch++) {
					if (ch == 8 || ch == 10 || ch == 219 || ch == 222 || ch == 223)
						continue;
					float score = 0.0f;

					for (int s = 0; s < SPLIT_MAG; s++) {
						#pragma region Euclidean
						if (metric == ColorMetrics::Euclidean) {
							int charIndex = col + (ch << 8);
							ColorI imageValue = imageValues[s];
							ColorI  value =  characterValues[charIndex][s] - imageValue;
							ColorI fvalue = characterFValues[charIndex][s] - imageValue;
							ColorI bvalue = characterBValues[charIndex][s] - imageValue;

							#pragma region Calc Score
							score +=
								std::pow(
									std::pow(
										float(value.r * value.r) +
										float(value.g * value.g) +
										float(value.b * value.b),
									2) +
									std::pow(
										float(fvalue.r * fvalue.r) +
										float(fvalue.g * fvalue.g) +
										float(fvalue.b * fvalue.b),
									2) +
									std::pow(
										float(bvalue.r * bvalue.r) +
										float(bvalue.g * bvalue.g) +
										float(bvalue.b * bvalue.b),
									2),
								2);
							#pragma endregion
						}
						#pragma endregion
						#pragma region LAB Delta E
						else {
							score += std::pow(LAB2Value(imageLABs[s], characterLABs[col + (ch << 8)][s]), 2);
						}
						#pragma endregion
					}

					if (score < closestScore) {
						closestPixel = Pixel((unsigned char)ch, (unsigned char)col);
						closestScore = score;
					}
				}
			}
			graphics->setPixel(Point2I(imgx, imgy), closestPixel);
		}
		updateAsciifierProgress(imgx, imageSize.x, clock() - totalTime, progressDialog);
		updateProgress();
	}

	#pragma region Cleanup
	progressDialog->setPercentage(100);
	progressDialog->setActionMessage("Completed");
	updateProgress();

	System::sleep(200);
	progressDialog->close();
	cleanup();
	return image;
	#pragma endregion
};

void Asciifier::updateAsciifierProgress(int x, int width, int time, ProgressDialogSPtr progressDialog) {
	progressDialog->setActionMessage(
		(std::string)"Asciifying image... [" +
		std::to_string((time / 1000) / 60) + ":" +
		((time / 1000) % 60 < 10 ? "0" : "") + std::to_string((time / 1000) % 60) +
		"]"
	);
	progressDialog->setPercentage(1 + (x + 1) * 99 / width);
}

bool Asciifier::writeBitmap(const std::string& path, const AsciiImageFrame& frame, Point2I finalSize) {

	unsigned int pngId = PNG_FONT_8X12;
	Point2I fontSize = Point2I(8, 12);
	if (Console::getFontType() == ConsoleFonts::Terminal) {
		fontSize = Console::getFontSize();
		switch (Console::getRasterFontSize()) {
		case RasterFontSizes::R4x6:   pngId = PNG_FONT_4X6; break;
		case RasterFontSizes::R6x8:   pngId = PNG_FONT_6X8; break;
		case RasterFontSizes::R8x8:   pngId = PNG_FONT_8X8; break;
		case RasterFontSizes::R16x8:  pngId = PNG_FONT_16X8; break;
		case RasterFontSizes::R5x12:  pngId = PNG_FONT_5X12; break;
		case RasterFontSizes::R7x12:  pngId = PNG_FONT_7X12; break;
		case RasterFontSizes::R8x12:  pngId = PNG_FONT_8X12; break;
		case RasterFontSizes::R16x12: pngId = PNG_FONT_16X12; break;
		case RasterFontSizes::R12x16: pngId = PNG_FONT_12X16; break;
		case RasterFontSizes::R10x18: pngId = PNG_FONT_10X18; break;
		default:
			fontSize = Point2I(8, 12);
		}
	}
	int fontMag = fontSize.x * fontSize.y;
	BitmapFrameSPtr fontBmp = WIC::readBitmapFromResource(pngId, "PNG");
	ConsolePalette palette = Console::getPalette();

	Point2I size = (finalSize != Point2I::ZERO ? finalSize : frame.size()) * fontSize;
	BitmapFrameSPtr bmp = WIC::createBitmap(size);

	writeFrameToBitmap(bmp, fontBmp, fontSize, palette, frame);

	return WIC::writeBitmapToFile(path, bmp);
}

bool Asciifier::writeGif(const std::string& path, const AsciiAnimation& animation) {
	unsigned int pngId = PNG_FONT_8X12;
	Point2I fontSize = Point2I(8, 12);
	if (Console::getFontType() == ConsoleFonts::Terminal) {
		fontSize = Console::getFontSize();
		switch (Console::getRasterFontSize()) {
		case RasterFontSizes::R4x6:   pngId = PNG_FONT_4X6; break;
		case RasterFontSizes::R6x8:   pngId = PNG_FONT_6X8; break;
		case RasterFontSizes::R8x8:   pngId = PNG_FONT_8X8; break;
		case RasterFontSizes::R16x8:  pngId = PNG_FONT_16X8; break;
		case RasterFontSizes::R5x12:  pngId = PNG_FONT_5X12; break;
		case RasterFontSizes::R7x12:  pngId = PNG_FONT_7X12; break;
		case RasterFontSizes::R8x12:  pngId = PNG_FONT_8X12; break;
		case RasterFontSizes::R16x12: pngId = PNG_FONT_16X12; break;
		case RasterFontSizes::R12x16: pngId = PNG_FONT_12X16; break;
		case RasterFontSizes::R10x18: pngId = PNG_FONT_10X18; break;
		default:
			fontSize = Point2I(8, 12);
		}
	}
	bool error = false;
	BitmapFrameSPtr fontBmp = WIC::readBitmapFromResource(pngId, "PNG");
	ConsolePalette palette = Console::getPalette();

	Point2I largestSize = animation[0].size();
	for (int i = 1; i < (int)animation.frameCount(); i++) {
		largestSize = GMath::max(largestSize, animation[i].size());
	}
	largestSize *= fontSize;

	//FileOutputStream out = FileOutputStream();
	//std::string writePath = Path::combine(Path::getDirectory(Path::getExecutablePath()), "Frames");
	//Path::createDirectory(writePath);
	//out.open(Path::combine(writePath, "Concat.txt"));
	//if (!out.isOpen())
	//	error = true;

	auto encoder = WIC::createGifEncoderFromStream(WIC::createStreamFromFile(path, false, true, true), largestSize);
	long long totalFrameTime = 0;
	long long offsetFrameTime = 0;
	for (int i = 0; i < (int)animation.frameCount() && !error; i++) {
		Point2I size = animation[i].size() * fontSize;
		BitmapFrameSPtr bmp = WIC::createBitmap(size);
		int speed = (int)animation.getFrameSpeed((i + 1) % (int)animation.frameCount());
		//int speed = (int)animation.getFrameSpeed(i);
		int finalSpeed = speed;
		finalSpeed += (int)(totalFrameTime - offsetFrameTime);
		if (finalSpeed < 0) {
			finalSpeed = 0;
		}
		else {
			int mod = finalSpeed % 10;
			if (mod <= 5)
				finalSpeed -= mod;
			else
				finalSpeed += 10 - mod;
		}
		totalFrameTime += speed;
		offsetFrameTime += finalSpeed;
		bmp->frameSpeed = finalSpeed;
		/*std::string filePath = Path::combine(writePath, std::to_string(i) + ".png");
		error = !Asciifier::writeBitmap(filePath, animation[i]);
		out.writeText("file '" + filePath + "'\r\n");
		float speed2 = (float)speed / 1000.0f;
		out.writeText("duration " + std::to_string(speed2) + "\r\n");*/
		Asciifier::writeFrameToBitmap(bmp, fontBmp, fontSize, palette, animation[i]);
		error = !WIC::writeBitmapToGifEncoder(encoder, bmp);
		//bitmaps->frames.push_back(bmp);
	}
	//std::string filePath = Path::combine(writePath, "0.png");
	//out.writeText("file '" + filePath + "'\r\n");

	/*int initialFrameSpeed = (int)animation.getFrameSpeed(0);
	int mod = initialFrameSpeed % 10;
	if (mod <= 5)
		initialFrameSpeed -= mod;
	else
		initialFrameSpeed += 10 - mod;*/
	encoder = nullptr;
	if (!error) {
		error = !WIC::writeFixesToGifFile(path, (int)animation.getFrameSpeed(0));
	}
	//if (out.isOpen())
	//	out.close();

	return !error;
}

bool Asciifier::writeVideo(const std::string& path, const AsciiAnimation& animation, bool& canceled, ProgressDialogSPtr progressDialog) {
	int totalTime = clock();
	unsigned int pngId = PNG_FONT_8X12;
	Point2I fontSize = Point2I(8, 12);
	if (Console::getFontType() == ConsoleFonts::Terminal) {
		fontSize = Console::getFontSize();
		switch (Console::getRasterFontSize()) {
		case RasterFontSizes::R4x6:   pngId = PNG_FONT_4X6; break;
		case RasterFontSizes::R6x8:   pngId = PNG_FONT_6X8; break;
		case RasterFontSizes::R8x8:   pngId = PNG_FONT_8X8; break;
		case RasterFontSizes::R16x8:  pngId = PNG_FONT_16X8; break;
		case RasterFontSizes::R5x12:  pngId = PNG_FONT_5X12; break;
		case RasterFontSizes::R7x12:  pngId = PNG_FONT_7X12; break;
		case RasterFontSizes::R8x12:  pngId = PNG_FONT_8X12; break;
		case RasterFontSizes::R16x12: pngId = PNG_FONT_16X12; break;
		case RasterFontSizes::R12x16: pngId = PNG_FONT_12X16; break;
		case RasterFontSizes::R10x18: pngId = PNG_FONT_10X18; break;
		default:
			fontSize = Point2I(8, 12);
		}
	}
	bool error = false;
	BitmapFrameSPtr fontBmp = WIC::readBitmapFromResource(pngId, "PNG");
	ConsolePalette palette = Console::getPalette();

	Point2I largestSize = animation[0].size();
	for (int i = 1; i < (int)animation.frameCount(); i++) {
		largestSize = GMath::max(largestSize, animation[i].size());
	}

	GUID guid;
	CoCreateGuid(&guid);
	wchar_t wGuid[MAX_PATH] = { 0 };
	StringFromGUID2(guid, wGuid, MAX_PATH);
	std::string writePath = Path::combine(Path::getTempPath(), "Ascii Artist");
	Path::createDirectory(writePath);
	writePath = Path::combine(writePath, wstr_to_str(wGuid));
	Path::createDirectory(writePath);

	FileOutputStream out = FileOutputStream();
	out.open(Path::combine(writePath, "Input.txt"));
	if (!out.isOpen())
		error = true;
	long long totalFrameTime = 0;
	long long offsetFrameTime = 0;
	int time = clock() - totalTime;
	progressDialog->setActionMessage(
		(std::string)"Saving frames to file... [" +
		std::to_string((time / 1000) / 60) + ":" +
		((time / 1000) % 60 < 10 ? "0" : "") + std::to_string((time / 1000) % 60) +
		"]"
	);
	progressDialog->setPercentage(1);
	progressDialog->update();
	for (int i = 0; i < (int)animation.frameCount() && !error; i++) {
		Point2I size = animation[i].size() * fontSize;
		BitmapFrameSPtr bmp = WIC::createBitmap(size);
		int speed = (int)animation.getFrameSpeed(i);
		int finalSpeed = speed;
		finalSpeed += (int)(totalFrameTime - offsetFrameTime);
		if (finalSpeed < 0) {
			finalSpeed = 0;
		}
		else {
			int mod = finalSpeed % 10;
			if (mod <= 5)
				finalSpeed -= mod;
			else
				finalSpeed += 10 - mod;
		}
		totalFrameTime += speed;
		offsetFrameTime += finalSpeed;
		bmp->frameSpeed = finalSpeed;
		std::string filePath = Path::combine(writePath, std::to_string(i) + ".png");
		error = !Asciifier::writeBitmap(filePath, animation[i], largestSize);
		out.writeText("file '" + std::to_string(i) + ".png'\r\n");
		out.writeText("duration " + std::to_string((float)speed / 1000.0f) + "\r\n");

		time = clock() - totalTime;
		progressDialog->setActionMessage(
			(std::string)"Saving frames to file... [" +
			std::to_string((time / 1000) / 60) + ":" +
			((time / 1000) % 60 < 10 ? "0" : "") + std::to_string((time / 1000) % 60) +
			"]"
		);
		if (i % 6 == 0 || i + 1 == (int)animation.frameCount()) {
			progressDialog->setPercentage(1 + (i * 96) / (int)animation.frameCount());
			if (progressDialog->update()) {
				canceled = true;
				out.close();

				progressDialog->setActionMessage("Cleaning up temporary files...");
				progressDialog->update();
				Path::deleteDirectory(writePath);

				return !error;
			}
		}
	}
	std::string filePath = Path::combine(writePath, "0.png");
	out.writeText("file '0.png'\r\n");

	if (!error)
		error = out.hasError();
	if (out.isOpen())
		out.close();

	progressDialog->setPercentage(98);
	progressDialog->setActionMessage("Running ffmpeg...");
	progressDialog->update();
	System::sleep(400);

	std::string ffmpeg = Path::combine(Path::getExecutableDirectory(), "ffmpeg.exe");
	std::string input = Path::combine(writePath, "Input.txt");
	replaceString(input, "\\", "/");
	//Path::shellExecute("open", ffmpeg, "-f concat -safe 0 -i " + enquote(input) + " -vsync 1 -r 30 -c:v mpeg4 -qscale:v 3 " + enquote(path));
	//Path::system(enquote(ffmpeg) + " -f concat -safe 0 -i " + enquote(input) + " -vsync 1 -r 30 -c:v mpeg4 -qscale:v 3 " + enquote(path));

	PROCESS_INFORMATION processInfo;
	STARTUPINFOA startupInfo = { sizeof(STARTUPINFO) };
	char cCommand[MAX_PATH * 4] = { 0 };
	std::string cmd = (enquote(ffmpeg) + " -y -f concat -safe 0 -i " + enquote(input) + " -vsync 1 -r 30 -c:v mpeg4 -qscale:v 3 " + enquote(path));
	strcpy_s(cCommand, cmd.c_str());
	if (CreateProcessA(NULL, cCommand, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInfo)) {
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
	}
	else {
		error = true;
	}
	
	progressDialog->forceRedraw();
	progressDialog->setPercentage(99);
	progressDialog->setActionMessage("Cleaning up temporary files...");
	progressDialog->update();
	System::sleep(400);

	Path::deleteDirectory(writePath);

	progressDialog->setPercentage(100);
	progressDialog->setActionMessage("Completed");
	progressDialog->update();
	System::sleep(400);
	progressDialog->close();

	return !error;
}

void Asciifier::writeFrameToBitmap(BitmapFrameSPtr bmp, BitmapFrameSPtr fontBmp, Point2I fontSize, const ConsolePalette& palette, const AsciiImageFrame& frame) {
	Point2I imageSize = frame.size();
	Point2I size = bmp->size;

	for (int imgx = 0; imgx < imageSize.x; imgx++) {
		for (int imgy = 0; imgy < imageSize.y; imgy++) {
			Pixel pixel = frame.getPixel(Point2I(imgx, imgy));
			ColorB fcolor = palette[getFColor(pixel.color)];
			ColorB bcolor = palette[getBColor(pixel.color)];

			int cx = (pixel.character % 16) * fontSize.x;
			int cy = (pixel.character / 16) * fontSize.y;
			int ix = imgx * fontSize.x;
			int iy = imgy * fontSize.y;
			for (int px = 0; px < fontSize.x; px++) {
				int p_x = ix + px;
				int c_x = cx + px;
				for (int py = 0; py < fontSize.y; py++) {
					int p_y = iy + py;
					int c_y = cy + py;
					if (fontBmp->pixels[xyToIndex(c_x, c_y, fontSize.x * 16)].r == 0)
						bmp->pixels[xyToIndex(p_x, p_y, size.x)] = bcolor;
					else
						bmp->pixels[xyToIndex(p_x, p_y, size.x)] = fcolor;
				}
			}
		}
	}
}

//=================================================================|
