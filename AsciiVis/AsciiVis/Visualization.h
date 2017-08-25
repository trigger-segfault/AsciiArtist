/*=================================================================|
|* File:				Visualization.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <PowerConsole/Controls/Control.h>
#include <fftw3.h>

using namespace PowerConsole::Controls;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

class Visualization : public Control {

	//========== CONSTANTS ===========
	#pragma region Constants

	static const int WAVEFORM_BUFFER_SIZE = 256 * 8;
	static const int SPECTRUM_BUFFER_SIZE = 256 * 128;
	static const int SPECTRUM_BUFFER_SHRINK = 16;

	static const int BUCKETS = 256;
	static const unsigned char HUES[7];

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	// Settings
	bool spectrumMode;
	bool expMode;
	bool peakMode;
	bool colorMode;
	int hue;
	int hueTimer;
	int hueSpeed;

	// Data
	int sampleRate;
	int sampleCount;
	int channelCount;
	std::vector<float> window;
	std::vector<float> spectrumWindow;
	std::vector<float> samples;
	std::vector<float> magnitudes;
	std::vector<float> spectrum;
	std::vector<float> frequencies;

	fftwf_plan plan;
	fftwf_complex signal[SPECTRUM_BUFFER_SIZE];
	fftwf_complex results[SPECTRUM_BUFFER_SIZE];

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Constructs the control. */
	Visualization();
	/* Initializes the control. */
	void onInitialize(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties

	void updatePlayIndex();

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);
	void onKeyGlobal(ControlSPtr sender, KeyboardEventArgs& e);

	#pragma endregion
};

DECLARE_SMART_PTRS(Visualization);

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
