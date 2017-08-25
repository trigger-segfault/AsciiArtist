/*=================================================================|
|* File:				SoundList.cpp							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "resources.h"
#include "SoundList.h"

using namespace AsciiTetris::Resources;
//=================================================================|
// CLASSES														   |
//=================================================================/
//=========== MEMBERS ============
#pragma region Tetrominos

std::map<std::string, SoundPairSPtr> SoundList::sounds;
MusicSPtr SoundList::music;

bool SoundList::soundMuted = false;
bool SoundList::musicMuted = false;

#pragma endregion
//=========== PLAYBACK ===========
#pragma region Playing

void SoundList::playSound(const std::string& name) {
	auto it = sounds.find(name);
	if (it == sounds.end())
		throw std::exception("Invalid sound name");
	if (!soundMuted)
		sounds[name]->sound.play();
}
void SoundList::playMusic() {
	if (!musicMuted)
		music->play();
}
void SoundList::toggleSoundMute() {
	soundMuted = !soundMuted;
}
void SoundList::toggleMusicMute() {
	musicMuted = !musicMuted;
	if (musicMuted)
		music->pause();
	else
		music->play();
}

#pragma endregion
//=========== CREATION ===========
#pragma region Creation

void SoundList::addSound(const std::string& name, unsigned int id) {
	HRSRC hRsrc = FindResourceA(NULL, ("#" + std::to_string(id)).c_str(), "WAVE");
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	LPVOID data = LockResource(hGlobal);
	DWORD dwSize = SizeofResource(NULL, hRsrc);

	auto snd = std::make_shared<SoundPair>();
	snd->buffer.loadFromMemory(data, dwSize);
	snd->sound.setBuffer(snd->buffer);

	sounds[name] = snd;
}
void SoundList::addMusic(unsigned int id) {
	HRSRC hRsrc = FindResourceA(NULL, ("#" + std::to_string(id)).c_str(), "FLAC");
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	LPVOID data = LockResource(hGlobal);
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	
	music = std::make_shared<sf::Music>();

	music->openFromMemory(data, dwSize);
	music->setVolume(30.0f);
	music->setLoop(true);
}

void SoundList::loadAudio() {
	addMusic(FLAC_MUSIC);

	addSound("MenuMove", WAV_MENU_MOVE);
	addSound("MenuSelect", WAV_MENU_SELECT);
	addSound("Land", WAV_LAND);
	addSound("RowComplete", WAV_ROW_COMPLETE);
	addSound("TetrisComplete", WAV_TETRIS_COMPLETE);
	addSound("QuintrisComplete", WAV_QUINTRIS_COMPLETE);
	addSound("Warning", WAV_WARNING);
	addSound("GameOver", WAV_GAME_OVER);
}

#pragma endregion
//=================================================================|
