/*=================================================================|
|* File:				SoundList.h								  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef SOUND_LIST_H
#define SOUND_LIST_H

#include <PowerConsole/Declarations.h>
#include <SFML/Audio.hpp>
#include <map>

namespace AsciiTetris::Resources {
//=================================================================|
// CLASSES														   |
//=================================================================/

struct SoundPair {

	//=========== MEMBERS ============
	#pragma region Tetrominos

	/* The sound buffer. */
	sf::SoundBuffer buffer;
	/* The sound instance. */
	sf::Sound sound;

	#pragma endregion
};

DECLARE_SMART_PTRS(SoundPair);

typedef std::shared_ptr<sf::Music> MusicSPtr;

class SoundList {
	//=========== MEMBERS ============
	#pragma region Tetrominos

	/* The map of sounds. */
	static std::map<std::string, SoundPairSPtr> sounds;
	/* The loaded music. */
	static MusicSPtr music;

	/* True if the sounds are muted. */
	static bool soundMuted;
	/* True if the music is muted. */
	static bool musicMuted;

	#pragma endregion
	//=========== PLAYBACK ===========
	#pragma region Playing
public:
	/* Plays the specified sound. */
	static void playSound(const std::string& name);
	/* Plays the music. */
	static void playMusic();
	/* Toggles the sound mutedness. */
	static void toggleSoundMute();
	/* Toggles the music mutedness. */
	static void toggleMusicMute();

	#pragma endregion
	//=========== CREATION ===========
	#pragma region Creation
private:
	/* Adds a sound to the list. */
	static void addSound(const std::string& name, unsigned int id);
	/* Adds the music. */
	static void addMusic(unsigned int id);
public:
	/* Loads the list of sounds and music. */
	static void loadAudio();

	#pragma endregion

};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
