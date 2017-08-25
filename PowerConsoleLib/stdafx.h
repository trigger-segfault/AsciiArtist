/*=================================================================|
|* File:				stdafx.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#include "targetver.h"

//========= DEFINITIONS ==========
// Shows exceptions when an stl array goes out of bounds
#ifdef _DEBUG
//#define _SECURE_SCL 0
//#define _HAS_ITERATOR_DEBUGGING 0
#endif

// MS Standard Library Math Definitions
#define _USE_MATH_DEFINES

// min and max macros are not defined in 'windows.h'
#define NOMINMAX

//========== LIBRARIES ===========
#pragma comment(lib,"shlwapi.lib") // Used for path helper functions
#pragma comment(lib,"winmm.lib") // Used for PlaySound
#pragma comment(lib,"version.lib") // Used to get exe and dll file versions

//=========== HEADERS ============
// Windows
#include <windows.h>
//#include <objidl.h> // Currently unused
#include <shlobj.h> // Used to get special folder paths and resolve shortcut paths
#include <shlwapi.h> // Used for path helper functions
#include <mmsystem.h> // Used for PlaySound

// Standard Library
#include <cmath>
#include <string>
#include <locale>
#include <codecvt>
#include <memory>
#include <array>
#include <vector>
#include <cstdio>
#include <functional>
#include <stack>
#include <deque>
#include <queue>
#include <ctime>
#include <memory>
#include <iostream>
#include <utility>
#include <initializer_list>
#include <conio.h>

// Other
//...

//=================================================================|
