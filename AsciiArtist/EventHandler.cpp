//================================================================
// EVENT HANDLER CPP:
//================================================================
#include "stdafx.h"
#include "EventHandler.h"

using namespace std;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Windows;
using namespace PowerConsole::Listeners;
//================================================================
// CLASSES:
//================================================================
// EventHandler:
//----------------------------------------------------------------
//=========== MEMBERS ============
PowerConsole::Listeners::EventHandler::_EventHandlerMembers PowerConsole::Listeners::EventHandler::_Data;
//========= CONSTRUCTORS =========
void PowerConsole::Listeners::EventHandler::Initialize() {
	static bool initialized = false;
	if (initialized)
		return;
	initialized = true;

	_Data.FPS			= 30.0;
	_Data.FrameStart	= clock();
	_Data.FrameDuration	= 1;
}
void PowerConsole::Listeners::EventHandler::Uninitialize() {

}
//=========== UPDATING ===========
void PowerConsole::Listeners::EventHandler::RecordInput(int waitDuration) {

	// Update the frame time
	_Data.FrameDuration	= clock() - _Data.FrameStart;
	_Data.FrameStart	= clock();
	_Data.FPS			= 1000.0 / _Data.FrameDuration;

	// Create a new input record
	_Data.InRec = INPUT_RECORD();

	// Check if there are any console events
	DWORD NumRead	= 0;
	bool hasEvent	= false;
	if (waitDuration > 0) {
		hasEvent = (WaitForSingleObject(Console::hIn, waitDuration) == WAIT_OBJECT_0);
	}
	else {
		GetNumberOfConsoleInputEvents(Console::hIn, &NumRead);
		hasEvent = true; //NumRead > 0;
	}

	if (hasEvent) {
		// Record the console event
		ReadConsoleInputA(Console::hIn, &_Data.InRec, 1, &NumRead);
	}
	else {
		// Mark the event type as invalid
		_Data.InRec.EventType = 0;
	}
}
//========= INFORMATION ==========
INPUT_RECORD PowerConsole::Listeners::EventHandler::GetInputRecord() {
	return _Data.InRec;
}
double PowerConsole::Listeners::EventHandler::getFPS() {
	return _Data.FPS;
}
int PowerConsole::Listeners::EventHandler::getFrameDuration() {
	return _Data.FrameDuration;
}
//================================================================
// END EVENT HANDLER CPP:
//================================================================