/*=================================================================|
|* File:				TetrisWindow.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "Visualization.h"
#include <AsciiVis/MusicLibrary.h>
#include <PowerConsole/Console.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Controls/Window.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <PowerConsole/Drawing/Characters.h>
#include <AsciiVis/Colors.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
using namespace PowerConsole::Helpers;
using namespace AsciiVis;
//=================================================================|
// FUNCTIONS													   |
//=================================================================/
namespace {

void HSVtoRGB(float& fR, float& fG, float& fB, float fH, float fS, float fV) {
	float fC = fV * fS; // Chroma
	float fHPrime = std::fmod(fH / 60.0f, 6.0f);
	float fX = fC * (1 - std::fabs(std::fmod(fHPrime, 2.0f) - 1));
	float fM = fV - fC;

	if (0 <= fHPrime && fHPrime < 1) {
		fR = fC;
		fG = fX;
		fB = 0;
	}
	else if (1 <= fHPrime && fHPrime < 2) {
		fR = fX;
		fG = fC;
		fB = 0;
	}
	else if (2 <= fHPrime && fHPrime < 3) {
		fR = 0;
		fG = fC;
		fB = fX;
	}
	else if (3 <= fHPrime && fHPrime < 4) {
		fR = 0;
		fG = fX;
		fB = fC;
	}
	else if (4 <= fHPrime && fHPrime < 5) {
		fR = fX;
		fG = 0;
		fB = fC;
	}
	else if (5 <= fHPrime && fHPrime < 6) {
		fR = fC;
		fG = 0;
		fB = fX;
	}
	else {
		fR = 0;
		fG = 0;
		fB = 0;
	}

	fR += fM;
	fG += fM;
	fB += fM;
}

}
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CONSTANTS ===========
#pragma region Constants

const unsigned char Visualization::HUES[7] = {
	RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA
};

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

Visualization::Visualization() {
	srand((unsigned int)time(nullptr));

	eventInitialize().add(bindEvent(Visualization::onInitialize));
	eventTick().add(bindEvent(Visualization::onTick));
	eventPaint().add(bindEvent(Visualization::onPaint));
	eventKeyGlobal().add(bindEvent(Visualization::onKeyGlobal));
}
void Visualization::onInitialize(ControlSPtr sender, EventArgs& e) {
	
	peakMode = false;
	expMode = true;
	spectrumMode = true;
	colorMode = false;
	hue = 120;
	hueTimer = 0;
	hueSpeed = 100;

	samples.resize(std::max(SPECTRUM_BUFFER_SIZE, WAVEFORM_BUFFER_SIZE));
	magnitudes.resize(SPECTRUM_BUFFER_SIZE / 2);
	spectrum.resize(BUCKETS);
	frequencies.resize(BUCKETS + 1);
	for (int i = 0; i < WAVEFORM_BUFFER_SIZE; i++) {
		window.push_back(0.54f - 0.46f * cos(2.0f * GMath::PI * (float)i / (float)WAVEFORM_BUFFER_SIZE));
	}
	for (int i = 0; i < SPECTRUM_BUFFER_SIZE / SPECTRUM_BUFFER_SHRINK; i++) {
		spectrumWindow.push_back(0.54f - 0.46f * cos(2.0f * GMath::PI * (float)i / (float)(SPECTRUM_BUFFER_SIZE / SPECTRUM_BUFFER_SHRINK)));
	}
	for (int i = 0; i <= BUCKETS; i++) {
		frequencies[i] = 32.70f * 1 * (float)std::pow(2, 8.0f * (float)i / (float)BUCKETS);
	}

	plan = fftwf_plan_dft_1d(SPECTRUM_BUFFER_SIZE, signal, results, FFTW_FORWARD, FFTW_ESTIMATE);

	sampleRate = (int)MusicLibrary::getMusic().getSampleRate();
	sampleCount = (int)MusicLibrary::getMusic().getSampleCount();
	channelCount = (int)MusicLibrary::getMusic().getChannelCount();
}

#pragma endregion
//=========== RUNNING ============
#pragma region Running

void Visualization::updatePlayIndex() {
	sampleRate = (int)MusicLibrary::getMusic().getSampleRate();
	sampleCount = (int)MusicLibrary::getMusic().getSampleCount();
	channelCount = (int)MusicLibrary::getMusic().getChannelCount();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void Visualization::onTick(ControlSPtr sender, TickEventArgs& e) {

	if (colorMode) {
		int previousHue = hue;
		hueTimer += e.timeSinceLastTick;
		hue = GMath::wrap(hue + hueTimer / hueSpeed, 360);
		hueTimer %= hueSpeed;
		if (hue != previousHue) {
			float r, g, b;
			HSVtoRGB(r, g, b, (float)hue, 1, 1);
			getWindow()->console().setPaletteColor(GREEN, ColorB((int)(r * 255), (int)(g * 255), (int)(b * 255)));
		}
	}

	int mark = (int)(MusicLibrary::getMusic().getPlayingOffset().asSeconds() * sampleRate);
	for (int i = 0; i < (spectrumMode ? SPECTRUM_BUFFER_SIZE : WAVEFORM_BUFFER_SIZE); i++) {
		samples[i] = 0.0f;

		if (MusicLibrary::getPlayIndex() != -1 && i < (spectrumMode ? SPECTRUM_BUFFER_SIZE / SPECTRUM_BUFFER_SHRINK : WAVEFORM_BUFFER_SIZE) && i + mark >= 0 && (i + mark) * channelCount < sampleCount) {
			samples[i] = (float)MusicLibrary::getMusic().getSamples()[(i + mark) * channelCount];
			if (channelCount >= 2) {
				samples[i] += (float)MusicLibrary::getMusic().getSamples()[(i + mark) * channelCount + 1];
				samples[i] /= 2;
			}
			samples[i] = samples[i] / (float)SHRT_MAX * (spectrumMode ? spectrumWindow[i] : window[i]);
		}

		if (spectrumMode) {
			signal[i][0] = samples[i];
			signal[i][1] = 0.0f;
		}
	}
	if (spectrumMode) {
		fftwf_execute(plan);

		float peak = 0.0f;
		for (int j = 0; j < BUCKETS; j++) {
			//spectrum[j] = 0.0f;
			float timeDif = (float)e.timeSinceLastTick / (float)(1000 / 60);
			float subtraction = spectrum[j] - 0.15f * timeDif;
			float multiple = spectrum[j] * std::pow(0.8f, timeDif);
			spectrum[j] = std::min(subtraction, multiple);
		}
		for (int i = 0; i < SPECTRUM_BUFFER_SIZE / 2; i++) {
			magnitudes[i] = std::sqrt(results[i][0] * results[i][0] + results[i][1] * results[i][1]);
			if (expMode && magnitudes[i] != 0.0f)
				magnitudes[i] = std::log10(magnitudes[i]) * 20.0f / 70.0f;
			else if (!expMode) {
				magnitudes[i] /= 350;
			}
			if (magnitudes[i] > peak)
				peak = magnitudes[i];
		}
		peak = std::min(1.0f, peak);
		for (int i = 0, j = 0; i < SPECTRUM_BUFFER_SIZE / 2; i++) {
			if (peakMode)
				magnitudes[i] /= peak;
			float frequency = (float)i * sampleRate / SPECTRUM_BUFFER_SIZE;
			if (frequency < frequencies[0])
				continue;
			for (; j < BUCKETS; j++) {
				if (frequency >= frequencies[j]) {
					if (magnitudes[i] > spectrum[j])
						spectrum[j] = magnitudes[i];
					if (frequency <= frequencies[j + 1]) {
						if ((float)(i + 1) * sampleRate / SPECTRUM_BUFFER_SIZE >= frequencies[j + 1])
							j++;
						break;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < BUCKETS; i++)
			spectrum[i] = 0.0f;
	}
}
void Visualization::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	Point2I size = getActualSize();
	int y = (spectrumMode ? size.y - 1 : size.y / 2);
	int max = (spectrumMode ? size.y : size.y / 2);
	float inc = (float)(spectrumMode ? BUCKETS : WAVEFORM_BUFFER_SIZE) / (float)size.x;

	for (int i = 0; i < size.x; (spectrumMode ? i += 2 : i++)) {
		float x = 0.0f;
		float z = 0.0f;
		bool negative = false;
		for (float j = i * inc; j < (i + (spectrumMode ? 2 : 1)) * inc; j++) {
			int index = (int)j;// i * inc + j;
			if (index >= (spectrumMode ? BUCKETS : WAVEFORM_BUFFER_SIZE))
				break;
			if (spectrumMode) {
				if (spectrum[index] > x)
					x = spectrum[index];
			}
			else {
				if (samples[index] >= 0.0f) {
					x = std::max(x, samples[index]);
				}
				else {
					z = std::min(z, samples[index]);
				}
			}
		}

		if (spectrumMode) {
			Rectangle2I rect = Rectangle2I(i, y, 1, -(int)(x * max)).absRect();
			g->setPixel(rect, Pixel(Characters::BLOCK_RIGHT_HALF, toColor(BLACK, GREEN)));
			g->setPixel(rect + Point2I(1, 0), Pixel(' ', toColor(GREEN, BLACK)));
		}
		else {
			Rectangle2I rect = Rectangle2I(i, y, 1, -(int)(x * max)).absRect();
			Rectangle2I rect2 = Rectangle2I(i, y, 1, -(int)(z * max)).absRect();
			g->setPixel(rect, Pixel(Characters::BLOCK_RIGHT_HALF, toColor(BLACK, GREEN)));
			g->setPixel(rect2, Pixel(Characters::BLOCK_RIGHT_HALF, toColor(BLACK, GREEN)));
		}
	}
}
void Visualization::onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e) {
	if (e.isKeyPressed(Keys::Insert))
		spectrumMode = !spectrumMode;
	else if (e.isKeyPressed(Keys::Home))
		expMode = !expMode;
	else if (e.isKeyPressed(Keys::PageUp))
		peakMode = !peakMode;
	else if (e.isKeyPressed(Keys::Delete))
		colorMode = !colorMode;
}

#pragma endregion
//=================================================================|
