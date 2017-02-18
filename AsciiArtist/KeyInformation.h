//================================================================
// KEY INFORMATION HEADER:
//================================================================
#pragma once
#ifndef KEYS_INFORMATION_H
#define KEYS_INFORMATION_H

#include "stdafx.h"
#include "Keys.h"

using namespace std;
//================================================================
// NAMESPACE:
//================================================================
/* The library for all Power Console wrappers and APIs. */
namespace PowerConsole {
/* A collection of event handling classes. */
namespace Listeners {
//================================================================
// CLASSES:
//================================================================
/* A static class with a collection of names and information for each key. */
class KeyInformation {

	//=========== MEMBERS ============
private:
	/* The structure for information about a single key. */
	struct KeyInfo {
		/* Constructs the key info with the specified information. */
		KeyInfo(string fullName = "", string shortName = "", char uppercase = '\0', char lowercase = '\0');

		/* The full name of the key. */
		string FullName;
		/* The short name of the key. */
		string ShortName;
		/* The uppercase letter of the key. */
		char Uppercase;
		/* The lowercase letter of the key. */
		char LowerCase;

		/* True if the key is reserved. */
		bool Reserved;
	};
	/* The structure to store the data about the key info class . */
	struct _KeyInformationMembers {

		/* The list of key info structures. */
		KeyInfo* KeyList;
		/* The list of mouse button info structures. */
		KeyInfo* MouseList;

	};
	/* The collection of data in the key info class. */
	static _KeyInformationMembers _Data;
	
	//========= CONSTRUCTORS =========
public:
	/* Initializes and sets up the key information. */
	static void Initialize();
	/* Uninitializes and cleans up the key information. */
	static void Uninitialize();
	
	//========== MANAGEMENT ==========
private:
	/* Adds a key to the list of keys. */
	static void AddKey(Keys keyCode, string fullName, string shortName, char uppercase = '\0', char lowercase = '\0');
	/* Adds a key to the list of keys. */
	static void AddKey(int keyCode, string fullName, string shortName, char uppercase = '\0', char lowercase = '\0');
	/* Adds a button to the list of mouse buttons. */
	static void AddMouse(MouseButtons buttonCode, string fullName, string shortName);
	/* Adds a button to the list of mouse buttons. */
	static void AddMouse(int buttonCode, string fullName, string shortName);

	//========= INFORMATION ==========
public:
	// Names
	/* Gets the name of the specified key. */
	static string GetKeyName(Keys keyCode, bool shortName = false);
	/* Gets the name of the specified key. */
	static string GetKeyName(int keyCode, bool shortName = false);
	/* Gets the name of the specified mouse button. */
	static string GetMouseName(MouseButtons buttonCode, bool shortName = false);
	/* Gets the name of the specified mouse button. */
	static string GetMouseName(int buttonCode, bool shortName = false);

	// Conversion
	/* Gets the character from the specified key. */
	static char GetCharFromKey(Keys keyCode, bool uppercase = false);
	/* Gets the character from the specified key. */
	static char GetCharFromKey(int keyCode, bool uppercase = false);
	/* Gets the key from the specified character. */
	static Keys GetKeyFromChar(char character);
};
//================================================================
// END KEY INFORMATION HEADER:
//================================================================
}}
#endif