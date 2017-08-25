/*=================================================================|
|* File:				Clipboard.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "Clipboard.h"

using namespace PowerConsole::IO;
//=================================================================|
// CLASSES														   |
//=================================================================/
//========== CLIPBOARD ===========
#pragma region Clipboard

ClipboardFormat Clipboard::registerFormat(const std::string& formatName) {
	return RegisterClipboardFormatA(formatName.c_str());
}
bool Clipboard::containsFormat(ClipboardFormat clipboardFormat) {
	return IsClipboardFormatAvailable(clipboardFormat) != 0;
}
std::vector<ClipboardFormat> Clipboard::getAvailableFormats() {
	std::vector<ClipboardFormat> formats;
	ClipboardFormat format = CF_NULL;
	if (OpenClipboard(NULL)) {
		while ((format = EnumClipboardFormats(format)) != CF_NULL) {
			formats.push_back(format);
		}
		CloseClipboard();
	}
	return formats;
}
std::vector<std::string> Clipboard::getAvailableFormatNames() {
	std::vector<std::string> formats;
	ClipboardFormat format = CF_NULL;
	if (OpenClipboard(NULL)) {
		while ((format = EnumClipboardFormats(format)) != CF_NULL) {
			formats.push_back(getClipboardFormatName(format));
		}
		CloseClipboard();
	}
	return formats;
}
bool Clipboard::isClipboardEmpty() {
	bool empty = true;
	if (OpenClipboard(NULL)) {
		empty = (EnumClipboardFormats(CF_NULL) == CF_NULL);
		CloseClipboard();
	}
	return empty;
}
void Clipboard::emptyClipboard() {
	if (OpenClipboard(NULL) ) {
		EmptyClipboard();
		CloseClipboard();
	}
}

bool Clipboard::containsText() {
	return IsClipboardFormatAvailable(CF_TEXT) != 0;
}
std::string Clipboard::getText() {
	std::string text = "";
	if (IsClipboardFormatAvailable(CF_TEXT) != 0) {
		if (OpenClipboard(nullptr)) {
			HANDLE hMem = GetClipboardData(CF_TEXT);
			if (hMem != nullptr) {
				text = (char*)hMem;
			}
			CloseClipboard();
		}
	}
	return text;
}
void Clipboard::setText(const std::string& text) {
	if (OpenClipboard(GetConsoleWindow())) {
		EmptyClipboard();
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
		if (hGlobal != nullptr) {
			LPVOID data = GlobalLock(hGlobal);
			memcpy_s(data, text.size() + 1, text.c_str(), text.size() + 1);
			GlobalUnlock(hGlobal);
			SetClipboardData(CF_TEXT, hGlobal);
			GlobalFree(hGlobal);
		}
		CloseClipboard();
	}
}

bool Clipboard::containsFileDrop() {
	return IsClipboardFormatAvailable(CF_HDROP) != 0;
}
std::vector<std::string> Clipboard::getFileDropNames() {
	std::vector<std::string> names;
	if (OpenClipboard(NULL)) {
		HANDLE hMem = GetClipboardData(CF_HDROP);
		if (hMem != NULL) {
			HDROP hDrop = (HDROP)GlobalLock(hMem);
			if (hDrop != NULL) {
				// Read off each file name
				UINT numFiles = DragQueryFileA(hDrop, -1, NULL, 0);
				for (int i = 0; i < (int)numFiles; i++) {
					UINT length = DragQueryFileA(hDrop, 0, NULL, 0);

					char* cFileName = new char[length + 1];
					if (DragQueryFileA(hDrop, 0, cFileName, length + 1) != 0 && length > 0) {
						names.push_back(cFileName);
					}
					else {
						break;
					}
				}
				DragFinish(hDrop);
			}
		}
		CloseClipboard();
	}
	return names;
}

int Clipboard::countClipboardFormats() {
	return CountClipboardFormats();
}
std::string Clipboard::getClipboardFormatName(ClipboardFormat clipboardFormat) {
	char cFormatName[MAX_PATH] = { 0 };
	if (GetClipboardFormatNameA(clipboardFormat, cFormatName, MAX_PATH)) {
		return cFormatName;
	}
	return "";
}

#pragma endregion
//=================================================================|
