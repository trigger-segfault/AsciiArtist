/*=================================================================|
|* File:				VisWindow.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#ifndef VIS_WINDOW_H
#define VIS_WINDOW_H

#include <PowerConsole/Controls/MainWindow.h>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <memory>
#include <vector>
#include <complex>
#include <fftw3.h>

using namespace PowerConsole::Controls;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/
/* The base manager for the game. */
class VisWindow : public MainWindow {

	//========== CONSTANTS ===========
	#pragma region Constants

	static const int WAVEFORM_BUFFER_SIZE = 256 * 8;
	static const int SPECTRUM_BUFFER_SIZE = 256 * 128;
	static const int SPECTRUM_BUFFER_SHRINK = 16;

	static const int BUCKETS = 256;

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	//sf::SoundBuffer buffer;
	//sf::Sound sound;
	sf::Music buffer;

	//int bufferSize;
	int sampleRate;
	int sampleCount;
	int channelCount;
	std::vector<float> window;
	std::vector<float> spectrumWindow;
	std::vector<float> samples;
	std::vector<float> magnitudes;
	std::vector<float> spectrum;
	std::vector<float> frequencies;
	bool spectrumMode;
	bool expMode;
	bool peakMode;

	fftwf_complex signal[SPECTRUM_BUFFER_SIZE];
	fftwf_complex results[SPECTRUM_BUFFER_SIZE];
	fftwf_plan plan;

	std::vector<std::string> playlist;
	std::vector<std::string> titles;
	int playIndex;
	int browseIndex;
	int browseTimer;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors
public:
	/* Contructs the tetris manager. */
	VisWindow();
private:
	/* Initializes the window. */
	void onInitialize(ControlSPtr sender, EventArgs& e);
	/* Loads the window. */
	void onLoad(ControlSPtr sender, EventArgs& e);

	#pragma endregion
	//=========== RUNNING ============
	#pragma region Running

	void nextSong(bool increment = true);

	#pragma endregion
	//======== EVENT HANDLERS ========
	#pragma region Event Handlers
private:
	void onTick(ControlSPtr sender, TickEventArgs& e);
	void onGlobalTick(ControlSPtr sender, TickEventArgs& e);
	void onPaint(ControlSPtr sender, PaintEventArgs& e);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
