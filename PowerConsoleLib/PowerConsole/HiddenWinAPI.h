/*=================================================================|
|* File:				HiddenWinAPI.h							  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Adds hidden WINAPI functions.					  *|
|=================================================================*/

#pragma once
#ifndef HIDDEN_WINAPI_H
#define HIDDEN_WINAPI_H

//=================================================================|
// FUNCTIONS													   |
//=================================================================/
#pragma region Functions

BOOL WINAPI SetConsoleIcon(HICON hIcon);
BOOL WINAPI ShowConsoleCursor(HANDLE hConsole, BOOL bShow);
NTSTATUS WINAPI RtlGetVersion(LPOSVERSIONINFOEXW lpOSInfo);

#define STATUS_SUCCESS 0

#pragma endregion
//=================================================================|
#endif /* End include guard */
//=================================================================|
