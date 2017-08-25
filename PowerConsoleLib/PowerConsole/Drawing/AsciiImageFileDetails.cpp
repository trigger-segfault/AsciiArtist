/*=================================================================|
|* File:				AsciiImageFileDetails.cpp				  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AsciiImageFileDetails.h"
#include <PowerConsole/IO/FileStream.h>

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

AsciiImageFileDetails::AsciiImageFileDetails()
 :	size(Point2I::ZERO),
	frameCount(0) {}

#pragma endregion
//=========== LOADING ============
#pragma region Loading

bool AsciiImageFileDetails::load(const std::string& path) {
	AsciiImageHeader header;
	FileInputStream in = FileInputStream();
	in.open(path);
	if (in.isOpen()) {
		if (AsciiImageInfo::loadHeader(&in, header, false)) {
			size = header.size;
			frameCount = header.frameCount;
			return true;
		}
	}
	return false;
}

#pragma endregion
//=================================================================|
