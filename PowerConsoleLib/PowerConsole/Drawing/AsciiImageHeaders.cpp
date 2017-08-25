/*=================================================================|
|* File:				AsciiImageHeaders.cpp					  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "AsciiImageHeaders.h"

using namespace PowerConsole::Drawing;
using namespace PowerConsole::Geometry;
//=================================================================|
// CLASSES														   |
//=================================================================/

//========= CONSTRUCTORS =========
#pragma region Constructors

AsciiImageHeader::AsciiImageHeader() {
	// File Information
	this->signature = "";
	this->version = 3;
	this->fileSize = 0;
	this->fileStart = 0;

	// Header Information
	this->headerSize = 0;
	this->headerOffset = 0;
	this->frameHeaderSize = 0;
	this->frameArrayOffset = 0;
	this->frameSize = 0;

	// Data Sizes
	this->bytesPerAttribute = 2;
	this->bytesPerName = 0;

	// Image Information
	this->format = ImageFormats::Basic;
	this->frameCount = 1;
	this->size = Point2I::ONE;
	this->background = Pixel();
	this->animationSpeed = 800;
}
AsciiImageFrameHeader::AsciiImageFrameHeader() {
	// File Information
	this->frameOffset = 0;
	this->frameSize = 0;

	// Frame Information
	this->name = "";
	this->size = Point2I::ONE;
	this->frameSpeed = 1;
	this->layers = 1;
}

#pragma endregion

//=================================================================|
