/*=================================================================|
|* File:				Main.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include <PowerConsole/Console.h>
#include <PowerConsole/ConsoleState.h>
#include <PowerConsole/IO/Clipboard.h>
#include <PowerConsole/IO/System.h>
#include <AsciiTetris/TetrisWindow.h>
#include <Windows.h>

using namespace PowerConsole;
using namespace AsciiTetris;
using namespace PowerConsole::IO;
using namespace std;

//=================================================================|
// MAIN															   |
//=================================================================/
#pragma region Main

/* The entry point of the program. */
int main(int argc, char** argv) {
	ConsoleState state;
	state.storeCurrentState(true);

	Console::setTitle("Ascii Tetris");
	Console::setIconFromResourceByIndex(0);
	if (System::getOSVersion() >= OSVersion::WIN_10) {

		Console::writeLine("    ____  ");
		Console::writeLine("   /_/_///");
		Console::writeLine("  /_/_/// ");
		Console::writeLine("");
		Console::writeLine("Windows 10 Notice:");
		Console::writeLine("  You must enable `legacy console' in order to run this application.");
		Console::writeLine("");
		Console::writeLine("To Enable:");
		Console::writeLine("  Right click on the title bar and select Properties.");
		Console::writeLine("  Check `Use legacy console' at the bottom of the Options tab.");
		Console::writeLine("  Then restart the application.");

		// If the console is not in legacy mode then the application will fail to
		// change the screen past this point. Thus the message will be preserved.
	}

	runWindow(TetrisWindow);

	state.revertToStoredState(true);
	return 0;
}

#pragma endregion
//=================================================================|
