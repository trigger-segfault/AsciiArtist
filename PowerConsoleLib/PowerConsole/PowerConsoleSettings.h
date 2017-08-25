/*=================================================================|
|* File:				PowerConsoleSettings.h					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef POWER_CONSOLE_SETTINGS_H
#define POWER_CONSOLE_SETTINGS_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Input/Hotkey.h>
#include <PowerConsole/Drawing/AsciiAnimation.h>
#include <PowerConsole/IO/FileStream.h>

namespace PowerConsole {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Stores global settings used for Power Console. */
class PowerConsoleSettings {

	//=========== MEMBERS ============
	#pragma region Members

	// Settings
	/* True if the program is in debug mode. */
	static bool debugMode;
	/* True if frame rate is automatically drawn when in debugMode. */
	static bool drawDebugFrameRate;
	/* The color for drawing the debug frame rate. */
	static unsigned char debugFrameRateColor;
	/* The hotkey used for toggling debug mode. */
	static PcInput::Hotkey debugHotkey;
	/* The hotkey used for copying the window as an ascii image to the clipboard. */
	static PcInput::Hotkey printScreenHotkey;
	/* The hotkey used to start and stop recording. */
	static PcInput::Hotkey recordHotkey;
	/* True if the entire buffer is drawn after changes. */
	static bool drawEntireBuffer;
	/* The desired frame rate for console applications. Set to 0 for 'as fast as possible'. */
	static int desiredFrameRate;

	// Recording
	/* True if recording is taking place. */
	static bool recording;
	/* The current recording of the screen. */
	static PcDraw::AsciiAnimationSPtr recordingAnimation;
	/* The frame rate of the recording. */
	static int recordingFrameRate;
	/* The time of the last recording update. */
	static int recordingLastTime;
	/* The start time of the recording. */
	static int recordingStartTime;
	/* The output stream being used to save the recording. */
	static std::shared_ptr<PcIO::FileOutputStream> out;
	/* The header used to save the recording with. */
	static std::shared_ptr<PcDraw::AsciiImageHeader> header;
	/* The frame header of the last frame saved. */
	static std::shared_ptr<PcDraw::AsciiImageFrameHeader> frameHeader;

	#pragma endregion
	//========== PROPERTIES ==========
	#pragma region Properties
public:
	// Debug Mode
	/* Gets if the program is in debug mode. */
	static bool isDebugging();
	/* Toggles if the program is in debug mode. */
	static void toggleDebugging();
	/* Sets if the program is in debug mode. */
	static void setDebugging(bool debugMode);
	/* Gets if frame rate is automatically drawn when in debugMode. */
	static bool getDrawDebugFrameRate();
	/* Sets if frame rate is automatically drawn when in debugMode. */
	static void setDrawDebugFrameRate(bool drawDebugFrameRate);
	/* Gets the color for drawing the debug frame rate. */
	static unsigned char getDebugFrameRateColor();
	/* Sets the color for drawing the debug frame rate. */
	static void setDebugFrameRateColor(unsigned char color);
	/* Gets the hotkey used for toggling debug mode. */
	static PcInput::Hotkey getDebugHotkey();
	/* Sets the hotkey used for toggling debug mode. */
	static void setDebugHotkey(PcInput::Hotkey debugHotkey);

	// Screen Capture
	/* Gets the hotkey used for copying the window as an ascii image to the clipboard. */
	static PcInput::Hotkey getPrintScreenHotkey();
	/* Sets the hotkey used for copying the window as an ascii image to the clipboard. */
	static void setPrintScreenHotkey(PcInput::Hotkey printScreenHotkey);
	/* Gets the hotkey used to start and stop recording. */
	static PcInput::Hotkey getRecordHotkey();
	/* Sets the hotkey used to start and stop recording. */
	static void setRecordHotkey(PcInput::Hotkey recordHotkey);
	/* Gets the desired frame rate for recording. */
	static int getRecordingFrameRate();
	/* Sets the desired frame rate for recording. Set to 0 for 'as fast as possible'. */
	static void setRecordingFrameRate(int recordingFrameRate);

	// Misc
	/* Gets if the entire buffer is drawn after changes. */
	static bool getDrawEntireBuffer();
	/* Sets if the entire buffer is drawn when there is changes. Otherwise only a portion will be drawn. */
	/* Setting this to true fixes minor graphical lag but also increases draw time. */
	static void setDrawEntireBuffer(bool drawEntireBuffer);
	/* Gets the desired frame rate for console applications. */
	static int getDesiredFrameRate();
	/* Sets the desired frame rate for console applications. Set to 0 for 'as fast as possible'. */
	static void setDesiredFrameRate(int desiredFrameRate);

	#pragma endregion
	//========== RECORDING ===========
	#pragma region Recording

	/* Returns true if the power console is recording. */
	static bool isRecording();
	/* Starts a recording. */
	static void startRecording(int desiredFrameRate);
	/* Ends the recording. */
	static void stopRecording();
	/* Toggles the recording state. */
	static void toggleRecording();
	/* Updates the recording. */
	static void updateRecording(const PcDraw::AsciiImage& frame);
	/* Gets the recording of the screen. */
	static PcDraw::AsciiAnimationSPtr getRecording();
	/* Gets the time the recording has been running for. */
	static int getRecordingTime();

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
