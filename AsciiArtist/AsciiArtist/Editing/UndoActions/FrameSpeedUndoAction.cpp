/*=================================================================|
|* File:				FrameSpeedUndoAction.cpp				  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "FrameSpeedUndoAction.h"

using namespace AsciiArtist::Editing;
using namespace AsciiArtist::Editing::UndoActions;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========= CONSTRUCTORS =========
#pragma region Constructors

FrameSpeedUndoAction::FrameSpeedUndoAction(unsigned int newSpeed)
	: UndoAction("All frame speeds"),
	originalSpeed(0),
	newSpeed(newSpeed),
	originalFrameSpeeds() {}
FrameSpeedUndoAction::FrameSpeedUndoAction(int frameIndex, unsigned int originalSpeed, unsigned int newSpeed)
 :	UndoAction("Frame speed", frameIndex),
	originalSpeed(originalSpeed),
	newSpeed(newSpeed),
	originalFrameSpeeds() {}

#pragma endregion
//============ ACTION ============
#pragma region Action

void FrameSpeedUndoAction::undo(CanvasSPtr canvas) {
	if (frameIndex == -1) {
		if (originalFrameSpeeds.size() == 0) {
			canvas->getImage()->setAnimationSpeed(originalSpeed);
		}
		else {
			for (int i = 0; i < (int)originalFrameSpeeds.size(); i++) {
				canvas->setFrameSpeedUndoAction(i, originalFrameSpeeds[i]);
			}
		}
	}
	else {
		canvas->setFrameSpeedUndoAction(frameIndex, originalSpeed);
	}
}
void FrameSpeedUndoAction::redo(CanvasSPtr canvas) {
	if (frameIndex == -1) {
		if (originalFrameSpeeds.size() == 0) {
			canvas->getImage()->setAnimationSpeed(newSpeed);
		}
		else {
			for (int i = 0; i < (int)originalFrameSpeeds.size(); i++) {
				canvas->setFrameSpeedUndoAction(i, newSpeed);
			}
		}
	}
	else {
		canvas->setFrameSpeedUndoAction(frameIndex, newSpeed);
	}
}

#pragma endregion
//========== PROPERTIES ==========
#pragma region Properties

void FrameSpeedUndoAction::setOriginalFrameSpeeds(fixed_array<unsigned int>&& originalFrameSpeeds) {
	this->originalFrameSpeeds = std::move(originalFrameSpeeds);
}

#pragma endregion
//=================================================================|
