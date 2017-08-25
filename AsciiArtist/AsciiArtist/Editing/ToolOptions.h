/*=================================================================|
|* File:				ToolOptions.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef TOOL_OPTIONS_H
#define TOOL_OPTIONS_H

#include <PowerConsole/Declarations.h>

namespace PowerConsole::Controls {
//=================================================================|
// CLASSES														   |
//=================================================================/

/* Options for all tools, accessible from the canvas. */
struct ToolOptions {
	/* True if shapes only draw outlines. */
	bool outline;
	/* True if characters need to have the exact code in order to match one another in replace and flood fill. */
	bool specific;
	/* True if borders are drawn thick. */
	bool thick;




};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
