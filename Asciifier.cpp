/*=================================================================|
|* File:				Asciifier.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Asciifier.h"
#include "resources.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Drawing/AsciiPalette.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Graphics.h>
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/ClipboardStream.h>
#include <PowerConsole/IO/ResourceStream.h>
#include <AsciiArtist/COM/WIC.h>
#include <PowerConsole/Console.h>

using namespace PowerConsole;
using namespace PowerConsole::IO;
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
		progressDialog->console().sleep(200);
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
		cleanup(); \
		return nullptr; \
	}

#define SPLIT_X split.x
#define SPLIT_Y split.y
#define SPLIT_MAG (SPLIT_X * SPLIT_Y)
#define TOTAL_CHARACTER_VALUE (96 / SPLIT_MAG)
#define NUM_CHARACTERS 256
#define NUM_COLORS 256

AsciiAnimationSPtr Asciifier::asciifyImagePrecision(const std::string& path, Point2I split, float scale, ColorMetrics metric, ProgressDialogSPtr progressDialog) {
	#pragma region Declarations
	int totalTime = clock();
	BitmapFrameSPtr bmp = nullptr;

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
	progressDialog->setActionMessage("Reading console font...");
	updateProgress();

	bmp = WIC::readBitmapFromResource(PNG_FONT, "PNG");
	#pragma endregion

	#pragma region Read Character Values
	progressDialog->setActionMessage("Reading image...");
	if (progressDialog->update()) {
		return nullptr;
	}
	for (int x = 0; x < bmp->size.x; x++) {
		for (int y = 0; y < bmp->size.y; y++) {
			if (bmp->pixels[xyToIndex(x, y, bmp->size.x)].r == 255) {
				int charIndex = (x / 8) + (y / 12) * 16;
				int subIndex = (x % 8 * SPLIT_X / 8) + (y % 12 * SPLIT_Y / 12) * SPLIT_X;
				characterCounts[charIndex][subIndex]++;
			}
		}
	}
	bmp = nullptr;
	#pragma endregion

	#pragma region Init B
	if (scale != 1.0f) {
		auto source = WIC::readSourceFromFile(path);
		source = WIC::scaleSource(source, Point2F(scale), BitmapScaleModes::Fant);
		bmp = WIC::readBitmapFromSource(source);
	}
	else {
		bmp = WIC::readBitmapFromFile(path);
	}

	Point2I imageSize = (Point2I)GMath::ceil((Point2F)bmp->size / Point2F(8.0f, 12.0f));
	int totalImageSize = imageSize.x * imageSize.y;
	auto image = std::make_shared<AsciiAnimation>(imageSize, ImageFormats::Basic, Pixel());

	image->resize(imageSize);
	auto graphics = image->createGraphics();

	AsciiPalette palette = progressDialog->console().getPalette();
	/*ColorI palette[16];
	for (int i = 0; i < 16; i++) {
		PowerConsole::Drawing::ColorB color = progressDialog->console().getPaletteColor(i);
		palette[i] = ColorI(color.r, color.g, color.b);
	}*/
	#pragma endregion

	#pragma region Init C
	progressDialog->setActionMessage("Calculating character LABs...");
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
			int ix = imgx * 8;
			int iy = imgy * 12;
			for (int s = 0; s < SPLIT_MAG; s++) {
				ColorI imageValue;
				cint totalRead = 0;
				int sx = ix + (s % SPLIT_X * 8 / SPLIT_X);
				int sy = iy + (s / SPLIT_X * 12 / SPLIT_Y);
				for (int px = 0; px < 8 / SPLIT_X; px++) {
					int p_x = sx + px;
					if (p_x >= bmp->size.x)
						continue;
					for (int py = 0; py < 12 / SPLIT_Y; py++) {
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

	progressDialog->console().sleep(200);
	progressDialog->close();
	cleanup();
	return image;
	#pragma endregion
};

void Asciifier::updateAsciifierProgress(int x, int width, int time, ProgressDialogSPtr progressDialog) {
	progressDialog->setActionMessage(
		(std::string)"Asciifying image... [" +
		(time / 1000 < 10 ? "0" : "") + std::to_string(time / 1000 / 60) + ":" +
		(time / 1000 < 10 ? "0" : "") + std::to_string(time / 1000) +
		"]"
	);
	progressDialog->setPercentage(1 + (x + 1) * 99 / width);
}

void Asciifier::writeBitmap(const std::string& path, const AsciiImageFrame& frame) {
	AsciiPalette palette = Console::getPalette();
	BitmapFrameSPtr fontBmp = WIC::readBitmapFromResource(PNG_FONT, "PNG");
	IWICImagingFactory* ipFactory = NULL;
	UINT stride;
	UINT bufferSize;
	IWICBitmapEncoder* ipEncoder;
	IWICBitmapFrameEncode* ipFrame;
	WICPixelFormatGUID formatGUID = GUID_WICPixelFormat24bppRGB;

	COMStreamSPtr stream = nullptr;

	Point2I fontSize = Point2I(8, 12);
	Point2I imageSize = frame.size();
	Point2I size = frame.size() * fontSize;
	BitmapFrameSPtr bmp = std::make_shared<BitmapFrame>();
	bmp->size = size;
	bmp->pixels = new ColorB[size.x * size.y];

	CoInitialize(nullptr);

	// Create the factory
	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ipFactory))))
		goto Return;

	stream = WIC::createStreamFromFile(path, false, true, true);
	if (isCOMObjectNull(stream))
		goto ReleaseFactory;

	if (FAILED(ipFactory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &ipEncoder)))
		goto ReleaseFactory;

	if (FAILED(ipEncoder->Initialize(stream->ipObject, WICBitmapEncoderNoCache)))
		goto ReleaseEncoder;

	if (FAILED(ipEncoder->CreateNewFrame(&ipFrame, nullptr)))
		goto ReleaseEncoder;

	if (FAILED(ipFrame->Initialize(nullptr)))
		goto ReleaseFrame;

	if (FAILED(ipFrame->SetSize((UINT)size.x, (UINT)size.y)))
		goto ReleaseFrame;

	if (FAILED(ipFrame->SetPixelFormat(&formatGUID)))
		goto ReleaseFrame;

	if (!IsEqualGUID(formatGUID, GUID_WICPixelFormat24bppBGR))
		goto ReleaseFrame;


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

	stride = size.x * 3;
	bufferSize = stride * size.y;
	if (FAILED(ipFrame->WritePixels((UINT)size.y, stride, bufferSize, (BYTE*)bmp->pixels)))
		goto ReleaseFrame;

	if (FAILED(ipFrame->Commit()))
		goto ReleaseFrame;

	if (SUCCEEDED(ipEncoder->Commit()))
		OutputDebugStringA("Success!");

ReleaseFrame:
	ipFrame->Release();
ReleaseEncoder:
	ipEncoder->Release();
ReleaseFactory:
	ipFactory->Release();
Return:
	CoUninitialize();
}

//=================================================================|
