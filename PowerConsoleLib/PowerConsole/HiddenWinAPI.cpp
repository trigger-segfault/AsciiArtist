/*=================================================================|
|* File:				HiddenWinAPI.cpp						  *|
|* Author:				Robert Jordan							  *|
|* Last Updated:		12/20/2013								  *|
|*																  *|
|* Description: Defines hidden WINAPI functions.				  *|
|=================================================================*/

#include "stdafx.h"
#include "HiddenWinAPI.h"

//=================================================================|
// FUNCTIONS													   |
//=================================================================/
#pragma region Functions

BOOL WINAPI SetConsoleIcon(HICON hIcon) {
	typedef BOOL(WINAPI *PSetConsoleIcon)(HICON);
	static PSetConsoleIcon pSetConsoleIcon = NULL;
	if (pSetConsoleIcon == NULL)
		pSetConsoleIcon = (PSetConsoleIcon)GetProcAddress(GetModuleHandleA("kernel32"), "SetConsoleIcon");
	if (pSetConsoleIcon == NULL) return FALSE;
	return pSetConsoleIcon(hIcon);
}

BOOL WINAPI ShowConsoleCursor(HANDLE hConsole, BOOL bShow) {
	typedef BOOL(WINAPI *PShowConsoleCursor)(HANDLE, BOOL);
	static PShowConsoleCursor pShowConsoleCursor = NULL;
	if (pShowConsoleCursor == NULL)
		pShowConsoleCursor = (PShowConsoleCursor)GetProcAddress(GetModuleHandleA("kernel32"), "ShowConsoleCursor");
	if (pShowConsoleCursor == NULL)
		return FALSE;
	return pShowConsoleCursor(hConsole, bShow);
}

NTSTATUS WINAPI RtlGetVersion(LPOSVERSIONINFOEXW lpOSInfo) {
	typedef NTSTATUS(WINAPI *PRtlGetVersion)(LPOSVERSIONINFOEXW);
	static PRtlGetVersion pRtlGetVersion = NULL;
	if (pRtlGetVersion == NULL)
		pRtlGetVersion = (PRtlGetVersion)GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
	if (pRtlGetVersion == NULL)
		return -1;
	return pRtlGetVersion(lpOSInfo);
}

#pragma endregion
//=================================================================|
