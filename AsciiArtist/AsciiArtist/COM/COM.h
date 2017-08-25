/*=================================================================|
|* File:				COM.h									  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef COM_H
#define COM_H

#include <wincodec.h>
#include <memory>
#include <PowerConsole/Declarations.h>

namespace AsciiArtist::COM {
//=================================================================|
// CLASSES														   |
//=================================================================/
//=================================================================|
#pragma region COMObject

/* A unique_ptr for a COM Object. */
template<typename T> struct COMObject {
	//=========== MEMBERS ============
	#pragma region Members

	/* The COM object to store. */
	T* ipObject;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	/* Constructs the COM Object. */
	COMObject()
		: ipObject(NULL) {
		CoInitialize(NULL);
	}
	/* Constructs the COM Object. */
	COMObject(T* ipObject)
		: ipObject(ipObject) {
		CoInitialize(NULL);
	}
	/* Deconstructs the COM Object by releasing ipObject. */
	~COMObject() {
		if (ipObject != NULL) {
			ipObject->Release();
		}
		CoUninitialize();
	}

	#pragma endregion
};

typedef COMObject<IStream> COMStream;
typedef std::shared_ptr<COMStream> COMStreamSPtr;

#define isCOMObjectNull(comObject) ((comObject) == nullptr) || ((comObject)->ipObject == NULL)
#define fullCOMRelease(comObject) comObject->Release(); comObject = NULL

#pragma endregion
//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
