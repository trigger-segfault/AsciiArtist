/*=================================================================|
|* File:				PowerConsoleSettings.cpp				  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "PowerConsoleSettings.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/IO/Path.h>

using namespace PowerConsole;
using namespace PowerConsole::IO;
using namespace PowerConsole::Input;
using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/
//=========== MEMBERS ============
#pragma region Members

// Debug Mode
bool PowerConsoleSettings::debugMode = false;
bool PowerConsoleSettings::drawDebugFrameRate = true;
unsigned char PowerConsoleSettings::debugFrameRateColor = 0x07;
Hotkey PowerConsoleSettings::debugHotkey = Hotkey(Keys::D, Modifiers::Alt);

// Settings
bool PowerConsoleSettings::drawEntireBuffer = false;
int PowerConsoleSettings::desiredFrameRate = 0;

// Screen Capture
Hotkey PowerConsoleSettings::printScreenHotkey = Hotkey(Keys::P, Modifiers::Alt);
Hotkey PowerConsoleSettings::recordHotkey = Hotkey(Keys::R, Modifiers::Alt);
bool PowerConsoleSettings::recording = false;
AsciiAnimationSPtr PowerConsoleSettings::recordingAnimation = nullptr;
int PowerConsoleSettings::recordingFrameRate = 1000 / 30;
int PowerConsoleSettings::recordingLastTime = 0;
int PowerConsoleSettings::recordingStartTime = 0;
std::shared_ptr<FileOutputStream> PowerConsoleSettings::out = nullptr;
std::shared_ptr<AsciiImageHeader> PowerConsoleSettings::header = nullptr;
std::shared_ptr<AsciiImageFrameHeader> PowerConsoleSettings::frameHeader = nullptr;

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

// Debug Mode
bool PowerConsoleSettings::isDebugging() {
	return debugMode;
}
void PowerConsoleSettings::toggleDebugging() {
	debugMode = !debugMode;
}
void PowerConsoleSettings::setDebugging(bool debugMode) {
	PowerConsoleSettings::debugMode = debugMode;
}
bool PowerConsoleSettings::getDrawDebugFrameRate() {
	return drawDebugFrameRate;
}
void PowerConsoleSettings::setDrawDebugFrameRate(bool drawDebugFrameRate) {
	PowerConsoleSettings::drawDebugFrameRate = drawDebugFrameRate;
}
unsigned char PowerConsoleSettings::getDebugFrameRateColor() {
	return debugFrameRateColor;
}
void PowerConsoleSettings::setDebugFrameRateColor(unsigned char color) {
	PowerConsoleSettings::debugFrameRateColor = color;
}
Hotkey PowerConsoleSettings::getDebugHotkey() {
	return debugHotkey;
}
void PowerConsoleSettings::setDebugHotkey(Hotkey debugHotkey) {
	PowerConsoleSettings::debugHotkey = debugHotkey;
}

// Screen Capture
Hotkey PowerConsoleSettings::getPrintScreenHotkey() {
	return printScreenHotkey;
}
void PowerConsoleSettings::setPrintScreenHotkey(Hotkey printScreenHotkey) {
	PowerConsoleSettings::printScreenHotkey = printScreenHotkey;
}
Hotkey PowerConsoleSettings::getRecordHotkey() {
	return recordHotkey;
}
void PowerConsoleSettings::setRecordHotkey(Hotkey recordHotkey) {
	PowerConsoleSettings::recordHotkey = recordHotkey;
}
int PowerConsoleSettings::getRecordingFrameRate() {
	return recordingFrameRate;
}
void PowerConsoleSettings::setRecordingFrameRate(int recordingFrameRate) {
	PowerConsoleSettings::recordingFrameRate = recordingFrameRate;
}

// Misc
bool PowerConsoleSettings::getDrawEntireBuffer() {
	return drawEntireBuffer;
}
void PowerConsoleSettings::setDrawEntireBuffer(bool drawEntireBuffer) {
	PowerConsoleSettings::drawEntireBuffer = drawEntireBuffer;
}
int PowerConsoleSettings::getDesiredFrameRate() {
	return desiredFrameRate;
}
void PowerConsoleSettings::setDesiredFrameRate(int desiredFrameRate) {
	if (desiredFrameRate < 0)
		throw std::exception("Invalid desired frame rate");
	PowerConsoleSettings::desiredFrameRate = desiredFrameRate;
}

#pragma endregion
//========== RECORDING ===========
#pragma region Recording

bool PowerConsoleSettings::isRecording() {
	return recording;
}
void PowerConsoleSettings::startRecording(int desiredFrameRate) {
	if (out != nullptr && out->isOpen()) {
		out->close();
	}
	recordingFrameRate = desiredFrameRate;
	recording = true;
	recordingAnimation = std::make_shared<AsciiAnimation>(Point2I::ONE, ImageFormats::Animation | ImageFormats::Multisize);
	recordingLastTime = 0;
	recordingStartTime = clock();

	header = std::make_shared<AsciiImageHeader>(AsciiImageInfo::createHeader(*recordingAnimation));
	frameHeader = std::make_shared<AsciiImageFrameHeader>();
	out = std::make_shared<FileOutputStream>();

	std::string exeDir = Path::getDirectory(Path::getExecutablePath());
	std::string recDir = Path::combine(exeDir, "Recordings");
	Path::createDirectory(recDir);

	time_t t = time(nullptr);
	tm now;
	localtime_s(&now, &t);
	now.tm_year += 1900;
	//now.tm_year %= 100;
	std::string time =
		std::to_string(now.tm_year) + "-" +
		std::to_string(now.tm_mon + 1) + "-" +
		std::to_string(now.tm_mday) + " " +
		std::to_string(now.tm_hour) + "." +
		(now.tm_min < 10 ? "0" : "") +
		std::to_string(now.tm_min) + "." +
		(now.tm_sec < 10 ? "0" : "") +
		std::to_string(now.tm_sec);

	if (!out->open(Path::combine(recDir, time + ".asc"))) {
		stopRecording();
	}
}
void PowerConsoleSettings::stopRecording() {
	if (recording) {
		// Has the first frame been recorded yet?
		if (recordingLastTime != 0) {
			int newTime = clock();
			int timePassed = newTime - recordingLastTime;

			out->setPointer(frameHeader->frameOffset + header->bytesPerName + 4, false);
			out->writeUInt((unsigned int)timePassed);
		}
		if (out->isOpen())
			out->close();
		out = nullptr;
		header = nullptr;
		frameHeader = nullptr;
		recordingAnimation = nullptr;
		recording = false;
	}
}
void PowerConsoleSettings::toggleRecording() {
	if (recording)
		stopRecording();
	else
		startRecording(1000 / 30);
}
void PowerConsoleSettings::updateRecording(const AsciiImage& frame) {
	if (recording) {
		int newTime = clock();
		int timePassed = newTime - recordingLastTime;
		if (timePassed >= recordingFrameRate || recordingLastTime == 0) {
			AsciiImageFrame& lastFrame = recordingAnimation->at(0);
			// 10 minutes is about the max length of a gif frame (Gif frame is 10 milliseconds per unit)
			bool changed = recordingLastTime == 0 || lastFrame.size() != frame.size() || timePassed >= 60000;
			if (!changed) {
				Point2I p;
				for (p.x = 0; p.x < frame.size().x; p.x++) {
					for (p.y = 0; p.y < frame.size().y; p.y++) {
						if (!frame.getPixel(p).matches(lastFrame.getPixel(p))) {
							changed = true;
							break;
						}
					}
				}
			}
				
			if (changed) {
				if (recordingLastTime != 0) {
					recordingLastTime = newTime;
					header->frameCount++;
					header->size = GMath::max(header->size, frame.size());
				}
				else {
					recordingLastTime = clock();
					header->size = frame.size();
					header->fileSize = header->frameArrayOffset;
				}
				recordingAnimation->setFrame(0, frame);

				// Write the previous frame's duration
				unsigned int offset;
				if (header->frameCount > 1) {
					out->setPointer(frameHeader->frameOffset + header->bytesPerName + 4, false);
					out->writeUInt((unsigned int)timePassed);
					offset = frameHeader->frameOffset + header->frameHeaderSize + (frameHeader->frameSize * frameHeader->layers);
				}
				else {
					offset = header->frameArrayOffset;
				}

				frameHeader->frameOffset = offset;
				frameHeader->name = "";
				frameHeader->frameSpeed = 0;
				frameHeader->size = frame.size();
				frameHeader->layers = 1;
				frameHeader->frameSize = frame.size().x * frame.size().y * (2 + header->bytesPerAttribute);

				// Increment the header size
				header->fileSize += header->frameHeaderSize + (frameHeader->frameSize * frameHeader->layers);

				out->setPointer(frameHeader->frameOffset, false);
				if (!AsciiImageInfo::saveFrameHeader(out.get(), *header, *frameHeader)) {
					stopRecording();
				}

				out->setPointer(frameHeader->frameOffset + header->frameHeaderSize, false);
				if (!frame.AsciiImageFrame::save(out.get(), *header, *frameHeader)) {
					stopRecording();
				}

				out->setPointer(0);
				if (!AsciiImageInfo::saveHeader(out.get(), *header, false)) {
					stopRecording();
				}
			}
		}
	}
}
AsciiAnimationSPtr PowerConsoleSettings::getRecording() {
	return recordingAnimation;
}
int PowerConsoleSettings::getRecordingTime() {
	return clock() - recordingStartTime;
}

#pragma endregion
//=================================================================|
