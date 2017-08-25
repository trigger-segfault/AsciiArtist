/*=================================================================|
|* File:				SystemMenuManager.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "SystemMenuManager.h"

using namespace PowerConsole;
//=================================================================|
// CLASSES														   |
//=================================================================/
//=========== MEMBERS ============
#pragma region Members

bool SystemMenuManager::initialized = false;

MENUITEMINFOA SystemMenuManager::closeMenuItem;
unsigned int SystemMenuManager::closeMenuItemPosition;
bool SystemMenuManager::closeMenuItemEnabled = true;

MENUITEMINFOA SystemMenuManager::consoleMenuItems[3];
unsigned int SystemMenuManager::consoleMenuItemPositions[3];
bool SystemMenuManager::consoleMenuItemsEnabled = true;

#pragma endregion
//========== MANAGEMENT ==========
#pragma region Management

void SystemMenuManager::initialize() {
	
	HMENU hMenu = GetSystemMenu(GetConsoleWindow(), false);
	if (hMenu != nullptr) {
		int menuItemCount = GetMenuItemCount(hMenu);
		for (int i = menuItemCount - 1; i >= 0; i--) {
			MENUITEMINFOA item = { sizeof(MENUITEMINFOA) };

			item.fMask = MIIM_STRING;
			if (GetMenuItemInfoA(hMenu, i, true, &item) == 0) {
				break;
			}
			item.cch++;
			item.dwTypeData = new char[item.cch];
			item.fMask = MIIM_ID | MIIM_FTYPE | MIIM_DATA | MIIM_STATE | MIIM_CHECKMARKS | MIIM_SUBMENU | MIIM_STRING | MIIM_BITMAP;
			if (GetMenuItemInfoA(hMenu, i, true, &item) == 0) {
				break;
			}

			std::string name = item.dwTypeData;
			//delete[] item.dwTypeData;
			if (name == "&Close") {
				closeMenuItem = item;
				closeMenuItemPosition = (unsigned int)i;
			}
			else if (name == "&Edit") {
				consoleMenuItems[0] = item;
				consoleMenuItemPositions[0] = (unsigned int)i;
			}
			else if (name == "&Defaults") {
				consoleMenuItems[1] = item;
				consoleMenuItemPositions[1] = (unsigned int)i;
			}
			else if (name == "&Properties") {
				consoleMenuItems[2] = item;
				consoleMenuItemPositions[2] = (unsigned int)i;
			}
		}
	}
	initialized = true;
}
bool SystemMenuManager::applyChanges(bool closeEnabled, bool consoleEnabled) {
	bool error = false, closeError = false, consoleError = false;

	HMENU hMenu = GetSystemMenu(GetConsoleWindow(), false);
	if (hMenu != nullptr) {
		// Close menu item
		if (!closeEnabled && closeMenuItemEnabled && !error) {
			if (RemoveMenu(hMenu, closeMenuItemPosition, MF_BYPOSITION) == 0) {
				error = true;
				closeError = true;
			}
		}
		else if (closeEnabled && !closeMenuItemEnabled && !error) {
			if (InsertMenuItemA(hMenu, closeMenuItemPosition, true, &closeMenuItem) == 0) {
				error = true;
				closeError = true;
			}
		}

		// Console menu items
		if (!consoleEnabled && consoleMenuItemsEnabled && !error) {
			for (int i = 2; i >= 0; i--) {
				if (RemoveMenu(hMenu, consoleMenuItemPositions[i] + (closeEnabled ? 0 : -1), MF_BYPOSITION) == 0) {
					error = true;
					consoleError = true;
					break;
				}
			}
		}
		else if (consoleEnabled && !consoleMenuItemsEnabled && !error) {
			for (int i = 0; i < 3; i++) {
				if (InsertMenuItemA(hMenu, consoleMenuItemPositions[i] + (closeEnabled ? 0 : -1), true, &consoleMenuItems[i]) == 0) {
					error = true;
					consoleError = true;
					break;
				}
			}
		}

		DrawMenuBar(GetConsoleWindow());
	}
	else {
		error = true;
	}

	if (!closeError)
		closeMenuItemEnabled = closeEnabled;
	if (!consoleError)
		consoleMenuItemsEnabled = consoleEnabled;

	return !error;
}

bool SystemMenuManager::isCloseMenuItemEnabled() {
	return closeMenuItemEnabled;
}
bool SystemMenuManager::setCloseMenuItemEnabled(bool enabled) {
	if (!initialized)
		initialize();
	return applyChanges(enabled, consoleMenuItemsEnabled);
}

bool SystemMenuManager::isConsoleMenuItemsEnabled() {
	return consoleMenuItemsEnabled;
}
bool SystemMenuManager::setConsoleMenuItemEnabled(bool enabled) {
	if (!initialized)
		initialize();
	return applyChanges(closeMenuItemEnabled, enabled);
}

bool SystemMenuManager::setMenuItemsEnabled(bool closeEnabled, bool consoleEnabled) {
	if (!initialized)
		initialize();
	return applyChanges(closeEnabled, consoleEnabled);
}

#pragma endregion
//=================================================================|
