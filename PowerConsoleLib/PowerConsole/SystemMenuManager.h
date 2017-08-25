/*=================================================================|
|* File:				SystemMenuManager.h						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SYSTEM_MENU_MANAGER_H
#define SYSTEM_MENU_MANAGER_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Keeps track of, and makes changes to the system menu. */
class SystemMenuManager {

	//=========== MEMBERS ============
	#pragma region Members

	/* True if the menu manager has been initialized. */
	static bool initialized;

	/* The close menu item to store. */
	static MENUITEMINFOA closeMenuItem;
	/* The close menu item position. */
	static unsigned int closeMenuItemPosition;
	/* True if the close menu item is enabnled. */
	static bool closeMenuItemEnabled;

	// 0 = Edit, 1 = Defaults, 2 = Properties
	/* The console menu items to store. */
	static MENUITEMINFOA consoleMenuItems[3];
	/* The console menu item positions. */
	static unsigned int consoleMenuItemPositions[3];
	/* True if the console menu items are enabnled. */
	static bool consoleMenuItemsEnabled;

	#pragma endregion
	//========== MANAGEMENT ==========
	#pragma region Management

	/* Initializes the system menu items. */
	static void initialize();
	/* Applies changes to the menu item settings. */
	static bool SystemMenuManager::applyChanges(bool closeEnabled, bool consoleEnabled);
public:
	/* Gets if the close menu item is enabnled. */
	static bool isCloseMenuItemEnabled();
	/* Sets if the close menu item is enabnled. */
	static bool setCloseMenuItemEnabled(bool enabled);

	/* Gets if the console menu items are enabnled. */
	static bool isConsoleMenuItemsEnabled();
	/* Sets if the console menu items are enabnled. */
	static bool setConsoleMenuItemEnabled(bool enabled);

	/* Sets if the close menu item and console menu items are enabnled. */
	static bool setMenuItemsEnabled(bool closeEnabled, bool consoleEnabled);

	#pragma endregion
};

//=================================================================|
} /* Trigger::PowerConsole */
#endif /* SYSTEM_MENU_MANAGER_H */
//=================================================================|
