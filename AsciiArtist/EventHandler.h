//================================================================
// EVENT HANDLER HEADER:
//================================================================
#pragma once
#ifndef EVENTS_H
#define EVENTS_H

#include "stdafx.h"
#include "Console.h"

using namespace std;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Windows;
//================================================================
// NAMESPACE:
//================================================================
/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of event handling classes. */
namespace Listeners {
//================================================================
// CLASSES:
//================================================================
/* A static class for gathering input data to use in events. */
class EventHandler {

	//=========== MEMBERS ============
	
private:
	/* The structure to store the data about the event handler class . */
	struct _EventHandlerMembers {

		// Event Data
		/* The structure for the input record event. */
		INPUT_RECORD InRec;

		// Update Rate
		/* The point in time at which the frame started. */
		int FrameStart;
		/* The duration of the last frame. */
		int FrameDuration;
		/* The frame rate of the program. */
		double FPS;
	};
	/* The collection of data in the event handler class. */
	static _EventHandlerMembers _Data;
	
	//========= CONSTRUCTORS =========

public:
	/* Initializes and sets up the event handler. */
	static void Initialize();
	/* Uninitializes and cleans up the event handler. */
	static void Uninitialize();
	
	//=========== UPDATING ===========

	/* Updates and records console input. */
	static void RecordInput(int waitDuration = 0);

	//========= INFORMATION ==========

	// Event Data
	/* Gets the input record structure of the event handler. */
	static INPUT_RECORD GetInputRecord();

	// Update Rate
	/* Gets the frame rate of the program. */
	static double getFPS();
	/* Gets the duration of the last frame. */
	static int getFrameDuration();
};
//================================================================
// END EVENT HANDLER HEADER:
//================================================================
}}
#endif