/*=================================================================|
|* File:				Main.cpp								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include <iomanip>
#include <iostream>
#include <PowerConsole/Console.h>
#include <PowerConsole/ConsoleState.h>
#include <PowerConsole/IO/Clipboard.h>
#include <PowerConsole/IO/System.h>
#include <AsciiChart/AsciiChartWindow.h>

using namespace PowerConsole;
using namespace PowerConsole::IO;
using namespace AsciiChart;

//========== LIBRARIES ===========
#pragma comment(lib,"shlwapi.lib") // Used for path helper functions
#pragma comment(lib,"winmm.lib") // Used for PlaySound
#pragma comment(lib,"version.lib") // Used to get exe and dll file versions
#pragma comment(lib,"windowscodecs.lib") // Used for opening and modifying images


//=================================================================|
// MAIN															   |
//=================================================================/
#pragma region Main

/* The entry point of the program. */
int main(int argc, char** argv) {
	ConsoleState state;
	state.storeCurrentState(true);

	//std::cout << std::setfill('0') << std::setw(5) << 25;
    //std::cout << "Hello World!\n";

	Console::setTitle("Ascii Chart");
	//Console::setIconFromResourceByIndex(0);
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

	runWindow(AsciiChartWindow, argc, argv);

	state.revertToStoredState(true);
	return 0;
}

#pragma endregion
//=================================================================|
