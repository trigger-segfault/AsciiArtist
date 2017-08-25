/*=================================================================|
|* File:				MusicLibrary.h							  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#pragma once
#ifndef MUSIC_LIBRARY_H
#define MUSIC_LIBRARY_H

#include <PowerConsole/Declarations.h>
#include <PowerConsole/Helpers/StringHelpers.h>
#include <vector>
#include <SFML/Audio/Music.hpp>

using namespace PowerConsole::Helpers;

namespace AsciiVis {
//=================================================================|
// CLASSES														   |
//=================================================================/

//#define STORE_METADATA

struct MusicData;
typedef std::shared_ptr<MusicData> MusicDataSPtr;
typedef std::shared_ptr<const MusicData> MusicDataCSPtr;

struct Playlist;
typedef std::shared_ptr<Playlist> PlaylistSPtr;
typedef std::shared_ptr<const Playlist> PlaylistCSPtr;

struct MusicData {
	std::wstring path;
	std::string title;
	std::string album;
	std::string artist;
	std::string genre;
	#ifdef STORE_METADATA
	std::vector<std::pair<std::string, std::string>> metadata;
	#endif
};

struct Playlist {
	std::string name;

	PlaylistSPtr parent;
	std::vector<PlaylistSPtr> folders;
	std::vector<MusicDataSPtr> songs;

	Playlist();
	Playlist(const std::string& name, PlaylistSPtr parent = nullptr);

	bool hasParent() const;
	int totalCount() const;
	int folderCount() const;
	int songCount() const;
	PlaylistSPtr findFolder(const std::string& name) const;
	bool containsFolder(const std::string& name) const;
	bool containsSong(const std::wstring& path) const;
	void sort();
	bool isEmpty();
	int indexOfSong(MusicDataSPtr song);
};

enum class PlaylistTypes {
	Music,
	Artists,
	Albums,
	Genres,
	Playlists
};

class MusicLibrary {
	//========== CONSTANTS ===========
	#pragma region Constants

	#pragma endregion
	//=========== MEMBERS ============
	#pragma region Members

	sf::Music music;
	std::vector<MusicDataSPtr> songs;
	int playIndex;
	int titleIndex;
	bool forceStopped;

	MusicDataSPtr playingSong;
	PlaylistSPtr playingPlaylist;
	PlaylistSPtr currentPlaylist;

	PlaylistSPtr playlistRoot;
	PlaylistSPtr playlistMusic;
	PlaylistSPtr playlistArtists;
	PlaylistSPtr playlistAlbums;
	PlaylistSPtr playlistGenres;
	PlaylistSPtr playlistPlaylists;

	static std::shared_ptr<MusicLibrary> lib;

	#pragma endregion
	//========= CONSTRUCTORS =========
	#pragma region Constructors

	MusicLibrary();
	void load();
	static bool loadMetadata(MusicDataSPtr song);
public:
	static void initialize();
	static void loadSongs();

	#pragma endregion
	//============ MUSIC =============
	#pragma region Music

	static sf::Music& getMusic();
	static int getPlayIndex();
	static void setPlayIndex(int index);
	static MusicDataCSPtr getMusicData();
	static void playNext(bool increment = true);
	static void playPrevious();
	static bool isForceStopped();
	static void setForceStopped(bool stopped);

	#pragma endregion
	//========== PLAYLISTS ===========
	#pragma region Playlists

	static const std::vector<MusicDataSPtr>& getSongs();
	static int getTotalSongCount();

	static PlaylistCSPtr getCurrentPlaylist();
	static PlaylistCSPtr getPlayingPlaylist();
	static int getTotalCount();
	static int getFolderCount();
	static int getSongCount();
	static bool hasParent();

	static void gotoParentPlaylist();
	static void gotoSubPlaylist(int index);

	#pragma endregion
};

//=================================================================|
} /* End namespace */
#endif /* End include guard */
//=================================================================|
