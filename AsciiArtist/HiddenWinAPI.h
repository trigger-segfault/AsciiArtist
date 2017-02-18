/*=================================================================|
|* File:				HiddenWinAPI.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Adds hidden WINAPI functions related to the		  *|
|*		console.												  *|
|=================================================================*/

#pragma once
#ifndef HIDDEN_WINAPI_H
#define HIDDEN_WINAPI_H

#include "Stdafx.h"

//=================================================================|
// FUNCTIONS													   |
//=================================================================/
#pragma region Functions

BOOL WINAPI SetConsoleIcon(HICON hIcon);
BOOL WINAPI ShowConsoleCursor(HANDLE hConsole, BOOL bShow);

#pragma endregion
//=================================================================|
#endif /* HIDDEN_WINAPI_H */
//=================================================================|
