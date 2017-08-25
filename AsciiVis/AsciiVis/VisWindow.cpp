/*=================================================================|
|* File:				TetrisWindow.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resource.h"
#include "VisWindow.h"
#include <PowerConsole/PowerConsoleSettings.h>
#include <PowerConsole/IO/FileStream.h>
#include <PowerConsole/IO/DirectoryInfo.h>
#include <PowerConsole/IO/Path.h>
#include <PowerConsole/Console.h>
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <PowerConsole/Drawing/ConsoleColors.h>
#include <valarray>
#include <fstream>
#include <string.h>
//#include <MP3/SoundFileReaderMp3.h>

using namespace PowerConsole;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Input;
using namespace PowerConsole::IO;
using namespace PowerConsole::Helpers;
using namespace AsciiVis;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

VisWindow::VisWindow() {
	srand((unsigned int)time(nullptr));

	eventInitialize().add(bindEvent(VisWindow::onInitialize));
	eventLoad().add(bindEvent(VisWindow::onLoad));
	eventTick().add(bindEvent(VisWindow::onTick));
	eventGlobalTick().add(bindEvent(VisWindow::onGlobalTick));
	eventPaint().add(bindEvent(VisWindow::onPaint));

	//PowerConsoleSettings::setDesiredFrameRate(1000 / 60);
	//PowerConsoleSettings::setDebugFrameRateColor(toColor(BLACK, DARK_GRAY));
	PowerConsoleSettings::setDrawEntireBuffer(true);
	//sf::SoundFileFactory::registerReader<sfe::SoundFileReaderMp3>();
}

void replaceString(std::wstring& text, const std::wstring& search, const std::wstring& replace) {
	if (!search.empty()) {
		size_t pos = 0;
		while ((pos = text.find(search, pos)) != std::wstring::npos) {
			text.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}
}
void splitString(const std::wstring& text, std::vector<std::wstring>& output, const std::wstring& delimiter) {
	output.clear();

	std::wstring::size_type lastPos = 0;
	std::wstring::size_type pos = text.find(delimiter, 0);

	while (std::wstring::npos != pos) {
		output.push_back(text.substr(lastPos, pos - lastPos));
		lastPos = pos + delimiter.length();
		pos = text.find_first_of(delimiter, lastPos);
	}
	output.push_back(text.substr(lastPos, text.length() - lastPos));
}
int searchString(const std::wstring& text, const std::wstring& search) {
	return (int)text.find(search);
}
bool comparei(std::string stringA, std::string stringB) {
	std::transform(stringA.begin(), stringA.end(), stringA.begin(), toupper);
	std::transform(stringB.begin(), stringB.end(), stringB.begin(), toupper);

	return (stringA < stringB);
}

void VisWindow::onInitialize(ControlSPtr sender, EventArgs& e) {
	ResizeHandlePixel resizePixel;
	resizePixel.hoverColor = toColor(BLACK, CYAN);
	resizePixel.resizeColor = toColor(BLACK, CYAN);
	resizePixel.normalColor = toColor(BLACK, DARK_BLUE);
	setResizeHandlePixel(resizePixel);
	setResizeHandleEnabled(true);
	console().centerWindow();
	console().setTitle("AsciiVis ))");
	console().setRasterFontSize(RasterFontSizes::R8x8);
	setSize(Point2I(80, 40));
	ConsolePalette palette;
	if (palette.loadResource(CONPAL_PALETTE)) {
		console().setPalette(palette);
	}

	spectrumMode = true;
	expMode = true;
	peakMode = false;
	browseIndex = 0;
	browseTimer = -1;

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

	playIndex = 0;
	/*DirectoryInfo dir;
	if (dir.load(R"(C:\Users\Onii-chan\Music\AsciiVis)")) {
		dir.removeFileExtensions(".ogg", true);
		dir.removeParentDirectory();
		dir.removeFileAttributes(FileAttributes::Directory);

		while (!dir.fileList.empty()) {
			int index = 0;// rand() % (int)dir.fileList.size();
			replaceString(dir.fileList[index].path, "\\", "/");
			playlist.push_back(dir.fileList[index].path);
			dir.fileList.erase(dir.fileList.begin() + index);
		}
		//playIndex = 0;
		playIndex = rand() % (int)playlist.size();
		nextSong(false);
	}*/
	std::wifstream in;
	in.open(R"(C:\Users\Onii-chan\Music\New All.m3u)");
	if (in.is_open()) {
		std::wstring text = L"";
		while (in) {
			std::wstring line;
			std::getline(in, line);
			text += line + L"\n";
		}
		std::vector<std::wstring> lines;
		//replaceString(text, L"\r", L"");
		splitString(text, lines, L"\n");
		std::wstring currentTitle = L"";
		in.close();

		for (std::wstring& line : lines) {
			if (searchString(line, L"#EXTM3U") == 0) {
				continue;
			}
			else if (searchString(line, L"#EXTINF:") == 0) {
				int index = 7;
				for (; index < (int)line.length(); index++) {
					if (line[index] == ',') {
						index++;
						break;
					}
				}
				currentTitle = line.substr(index);
			}
			else {
				wchar_t wShortPath[MAX_PATH] = { 0 };
				if (GetShortPathNameW(line.c_str(), wShortPath, MAX_PATH)) {
					playlist.push_back(wstr_to_str(wShortPath));
					titles.push_back(wstr_to_str(currentTitle));
				}
			}
		}


	}

	for (int i = 1; i < (int)playlist.size(); i++) {
		std::string title = titles[i];
		std::string path = playlist[i];
		titles.erase(titles.begin() + i);
		playlist.erase(playlist.begin() + i);
		int index = i;
		while (index > 0 && comparei(title, titles[index - 1]))
			index--;

		titles.insert(titles.begin() + index, title);
		playlist.insert(playlist.begin() + index, path);
	}
	playIndex = rand() % (int)playlist.size();
	nextSong(false);

	//FileInputStream in;
	/*if (in.open(R"(C:\Users\Onii-chan\Music\New All.m3u)")) {

		std::string text = in.readString();
		std::vector<std::string> lines;
		std::vector<std::string> tempPlaylist;
		replaceString(text, "\r", "");
		splitString(text, lines, "\n");
		std::string currentTitle = "";

		for (std::string& line : lines) {
			if (searchString(line, "#EXTM3U") == 0) {
				continue;
			}
			else if (searchString(line, "#EXTINF:") == 0) {
				int index = 7;
				for (; index < (int)line.length(); index++) {
					if (line[index] == ',') {
						index++;
						break;
					}
				}
				currentTitle = line.substr(index);
			}
			else {//if (!compareString(Path::getExtension(line), ".m4a", false)) {
				tempPlaylist.push_back(line);
			}
		}
		for (std::string& file : tempPlaylist) {
			Path::copyFile(file, Path::combine(R"(C:\Users\Onii-chan\Music\AsciiVis\)", Path::getFileName(file)), false);
		}

		while (!tempPlaylist.empty()) {
			int index = rand() % (int)tempPlaylist.size();
			playlist.push_back(tempPlaylist[index]);
			tempPlaylist.erase(tempPlaylist.begin() + index);
		}
		
		in.close();
		nextSong();
	}*/
}
void VisWindow::onLoad(ControlSPtr sender, EventArgs& e) {
	
}

#pragma endregion
//=========== RUNNING ============
#pragma region Running

void VisWindow::nextSong(bool increment) {
	if (increment) {
		playIndex = GMath::wrap(playIndex + 1, (int)playlist.size());
	}
	//while (!buffer.openFromFile(R"(C:\Users\Onii-chan\Music\iTunes\iTunes Media\Music\Ylvis\The Fox (What Does The Fox Say_) - Singl\02 The Fox (Instrumental).m4a)")) {
	while (!buffer.openFromFile(playlist[playIndex])) {
		playIndex = GMath::wrap(playIndex + 1, (int)playlist.size());
	}
	buffer.play();
	//sound.setBuffer(buffer);
	//sound.play();
	sampleRate = (int)buffer.getSampleRate();
	sampleCount = (int)buffer.getSampleCount();
	channelCount = (int)buffer.getChannelCount();
}

#pragma endregion
//======== EVENT HANDLERS ========
#pragma region Event Handlers

void VisWindow::onTick(ControlSPtr sender, TickEventArgs& e) {
	
}
void VisWindow::onGlobalTick(ControlSPtr sender, TickEventArgs& e) {
	if (buffer.getStatus() == sf::SoundSource::Status::Stopped) {
		nextSong();
	}
	if (browseTimer >= 0) {
		browseTimer -= e.timeSinceLastTick;
	}
	if (globalKeyboard().isKeyPressed(Keys::Shift)) {
		int newIndex = playIndex;
		do {
			newIndex = rand() % (int)playlist.size();
		} while (newIndex == playIndex);
		playIndex = newIndex;
		nextSong(false);
	}
	if (globalKeyboard().isKeyPressed(Keys::Escape)) {
		browseTimer = -1;
	}
	if (globalKeyboard().isKeyPressed(Keys::Space)) {
		if (browseTimer >= 0)
			playIndex = GMath::wrap(browseIndex - 1, (int)playlist.size());
		browseTimer = -1;
		nextSong();
	}
	const int browseDuration = 6000;// / (1000 / 60);
	if (globalKeyboard().isKeyTyped(Keys::Up)) {
		if (browseTimer <= -1)
			browseIndex = GMath::wrap(playIndex - 1, (int)playlist.size());
		else
			browseIndex = GMath::wrap(browseIndex - 1, (int)playlist.size());
		browseTimer = browseDuration;
	}
	else if (globalKeyboard().isKeyTyped(Keys::Down)) {
		if (browseTimer <= -1)
			browseIndex = GMath::wrap(playIndex + 1, (int)playlist.size());
		else
			browseIndex = GMath::wrap(browseIndex + 1, (int)playlist.size());
		browseTimer = browseDuration;
	}
	if (globalKeyboard().isKeyPressed(Keys::Enter))
		spectrumMode = !spectrumMode;
	if (globalKeyboard().isKeyPressed(Keys::Backspace))
		expMode = !expMode;
	if (globalKeyboard().isKeyPressed(Keys::OemPipe))
		peakMode = !peakMode;
	if (globalKeyboard().isKeyPressed(Keys::Right))
		buffer.setPitch(5.0f);
	else if (globalKeyboard().isKeyReleased(Keys::Right))
		buffer.setPitch(1.0f);


	int mark = (int)(buffer.getPlayingOffset().asSeconds() * sampleRate);
	for (int i = 0; i < (spectrumMode ? SPECTRUM_BUFFER_SIZE : WAVEFORM_BUFFER_SIZE); i++) {
		samples[i] = 0.0f;

		if (i < (spectrumMode ? SPECTRUM_BUFFER_SIZE / SPECTRUM_BUFFER_SHRINK : WAVEFORM_BUFFER_SIZE) && i + mark >= 0 && (i + mark) * channelCount < sampleCount) {
			samples[i] = (float)buffer.getSamples()[(i + mark) * channelCount];
			if (channelCount >= 2) {
				samples[i] += (float)buffer.getSamples()[(i + mark) * channelCount + 1];
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
}
void VisWindow::onPaint(ControlSPtr sender, PaintEventArgs& e) {
	auto g = e.graphics;

	g->drawBorder(Rectangle2I(getActualSize()), Pixel(' ', toColor(BLACK, DARK_BLUE)), true);
	g->drawBorder(Rectangle2I(0, 2, getActualSize().x, 1), Pixel(' ', toColor(BLACK, DARK_BLUE)), true);

	g->drawString(Point2I(2, 1), (std::string)"Mode: " + (spectrumMode ? "spectrum" : "waveform"), Pixel(' ', toColor(BLACK, WHITE)));
	unsigned char color = (spectrumMode ? toColor(BLACK, WHITE) : toColor(BLACK, DARK_GRAY));
	g->drawString(Point2I(18, 1), (std::string)"| Exponential: " + (expMode ? "on" : "off"), Pixel(' ', color));
	g->drawString(Point2I(38, 1), (std::string)"| Peak: " + (peakMode ? "on" : "off"), Pixel(' ', color));

	Point2I size = getActualSize() - Point2I(2, 4);
	int y = (spectrumMode ? size.y - 1 : size.y / 2);
	int max = (spectrumMode ? size.y : size.y / 2);
	float inc = (float)(spectrumMode ? BUCKETS : WAVEFORM_BUFFER_SIZE) / (float)size.x;

	g->startRegion(Point2I(1, 3), Rectangle2I(size));

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

	g->endRegion();

	if (browseTimer >= 0) {
		for (int i = 0; i < 9; i++) {
			unsigned char color;
			switch (i) {
			case 0: case 1: case 7: case 8:
				color = toColor(BLACK, DARK_GRAY); break;
			case 2: case 3: case 5: case 6:
				color = toColor(BLACK, GRAY); break;
			case 4:
				color = toColor(BLACK, WHITE); break;
			}
			int offset = (i > 4 ? 2 : (i == 4 ? 1 : 0));
			int index = GMath::wrap(browseIndex - 4 + i, (int)playlist.size());
			std::string title = Path::getFileNameWithoutExtension(playlist[index]);
			if (!titles[index].empty())
				title = titles[index];
			g->drawString(Point2I(2, 3 + i + offset), title, Pixel(' ', color, PixelAttributes::Char | PixelAttributes::FColor));
		}
	}
	else {
		std::string name = buffer.getTitle();
		if (name.empty())
			name = titles[playIndex];
		if (name.empty())
			name = Path::getFileNameWithoutExtension(playlist[playIndex]);
		g->drawString(Point2I(2, 4), name, Pixel(' ', toColor(BLACK, WHITE), PixelAttributes::Char | PixelAttributes::FColor));
		g->drawString(Point2I(3, 5), buffer.getArtist(), Pixel(' ', toColor(BLACK, WHITE), PixelAttributes::Char | PixelAttributes::FColor));
		g->drawString(Point2I(3, 6), buffer.getAlbum(), Pixel(' ', toColor(BLACK, WHITE), PixelAttributes::Char | PixelAttributes::FColor));
	}
}

#pragma endregion
//=================================================================|
