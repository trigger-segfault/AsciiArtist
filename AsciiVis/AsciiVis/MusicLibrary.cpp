/*=================================================================|
|* File:				MusicLibrary.cpp						  *|
|* Author:				Robert Jordan							  *|
|*																  *|
|=================================================================*/

#include "stdafx.h"
#include "MusicLibrary.h"
#include <PowerConsole/Geometry/GMath.h>
#include <PowerConsole/IO/DirectoryInfo.h>
#include <PowerConsole/IO/Path.h>
#include <fstream>
#include <SFML/System/FileInputStream.hpp>
extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswresample/swresample.h>
};

using namespace AsciiVis;
using namespace PowerConsole::Geometry;
using namespace PowerConsole::Helpers;
using namespace PowerConsole::IO;
//=================================================================|
// FUNCTIONS													   |
//=================================================================/
namespace {
void replaceString(std::wstring& text, const std::wstring& search, const std::wstring& replace) {
	if (!search.empty()) {
		size_t pos = 0;
		while ((pos = text.find(search, pos)) != std::wstring::npos) {
			text.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}
}
void splitString(const std::wstring& text, std::vector<std::wstring>& output, const std::wstring& delimiter) {
	output.clear();

	std::wstring::size_type lastPos = 0;
	std::wstring::size_type pos = text.find(delimiter, 0);

	while (std::wstring::npos != pos) {
		output.push_back(text.substr(lastPos, pos - lastPos));
		lastPos = pos + delimiter.length();
		pos = text.find_first_of(delimiter, lastPos);
	}
	output.push_back(text.substr(lastPos, text.length() - lastPos));
}
int searchString(const std::wstring& text, const std::wstring& search) {
	return (int)text.find(search);
}
bool compareLess(std::string stringA, std::string stringB) {
	std::transform(stringA.begin(), stringA.end(), stringA.begin(), toupper);
	std::transform(stringB.begin(), stringB.end(), stringB.begin(), toupper);

	return (stringA < stringB);
}
bool wcompareEqual(std::wstring stringA, std::wstring stringB) {
	std::transform(stringA.begin(), stringA.end(), stringA.begin(), toupper);
	std::transform(stringB.begin(), stringB.end(), stringB.begin(), toupper);

	return (stringA == stringB);
}

void av_no_log(void* avcl, int level, const char* fmt, va_list varg) {
	// Do nothing, screw console output when it messes up my UI
}

int read(void* opaque, uint8_t* buf, int buf_size) {
	sf::InputStream* stream = (sf::InputStream*)opaque;
	return (int)stream->read(buf, buf_size);
}

int64_t seek(void* opaque, int64_t offset, int whence) {
	sf::InputStream* stream = (sf::InputStream*)opaque;
	switch (whence) {
	case SEEK_SET:
		break;
	case SEEK_CUR:
		offset += stream->tell();
		break;
	case SEEK_END:
		offset = stream->getSize() - offset;
		break;
	case AVSEEK_SIZE:
		return (int64_t)stream->getSize();
	}
	return (int64_t)stream->seek(offset);
}
}
//=================================================================|
// CLASSES														   |
//=================================================================/

Playlist::Playlist() 
 :	parent(nullptr),
	name("") {}
Playlist::Playlist(const std::string& name, PlaylistSPtr parent)
 :	parent(parent),
	name(name) {}

bool Playlist::hasParent() const {
	return parent != nullptr;
}
int Playlist::totalCount() const {
	return (int)(folders.size() + songs.size() + (parent != nullptr ? 1 : 0));
}
int Playlist::folderCount() const {
	return (int)folders.size();
}
int Playlist::songCount() const {
	return (int)songs.size();
}
PlaylistSPtr Playlist::findFolder(const std::string& name) const {
	for (PlaylistSPtr playlist : folders) {
		if (compareString(playlist->name, name, false))
			return playlist;
	}
	return nullptr;
}
bool Playlist::containsFolder(const std::string& name) const {
	for (PlaylistSPtr playlist : folders) {
		if (compareString(playlist->name, name, false))
			return true;
	}
	return false;
}
bool Playlist::containsSong(const std::wstring& path) const {
	for (MusicDataSPtr song : songs) {
		if (wcompareEqual(song->path, path))
			return true;
	}
	return false;
}
void Playlist::sort() {
	std::sort(folders.begin(), folders.end(), [](PlaylistSPtr a, PlaylistSPtr b) {
		return compareLess(a->name, b->name);
	});
	std::sort(songs.begin(), songs.end(), [](MusicDataSPtr a, MusicDataSPtr b) {
		return compareLess(a->title, b->title);
	});
	/*for (int i = 1; i < (int)songs.size(); i++) {
		MusicDataSPtr data = songs[i];
		songs.erase(songs.begin() + i);
		int index = i;
		while (index > 0 && compareLess(data->title, songs[index - 1]->title))
			index--;

		songs.insert(songs.begin() + index, data);
	}*/
}
bool Playlist::isEmpty() {
	return folders.empty() && songs.empty();
}
int Playlist::indexOfSong(MusicDataSPtr song) {
	if (song != nullptr) {
		int i = 0;
		for (MusicDataSPtr song2 : songs) {
			if (wcompareEqual(song2->path, song->path))
				return i;
			i++;
		}
	}
	return -1;
}

//=========== MEMBERS ============
#pragma region Members

std::shared_ptr<MusicLibrary> MusicLibrary::lib;

#pragma endregion
//========= CONSTRUCTORS =========
#pragma region Constructors

MusicLibrary::MusicLibrary() :
	playIndex(-1),
	titleIndex(0),
	forceStopped(true) {

	playlistRoot = std::make_shared<Playlist>("Root");
	playlistMusic = std::make_shared<Playlist>("Music", playlistRoot);
	playlistArtists = std::make_shared<Playlist>("Artists", playlistRoot);
	playlistAlbums = std::make_shared<Playlist>("Albums", playlistRoot);
	playlistGenres = std::make_shared<Playlist>("Genres", playlistRoot);
	playlistPlaylists = std::make_shared<Playlist>("Playlists", playlistRoot);
	playlistRoot->folders.push_back(playlistMusic);
	playlistRoot->folders.push_back(playlistArtists);
	playlistRoot->folders.push_back(playlistAlbums);
	playlistRoot->folders.push_back(playlistGenres);
	playlistRoot->folders.push_back(playlistPlaylists);
	currentPlaylist = playlistRoot;
	playingPlaylist = currentPlaylist;
}
void MusicLibrary::load() {
	DirectoryInfo dir;
	std::string path = Path::combine(Path::getSpecialFolderPath(FOLDERID_Music), "AsciiVis");
	if (!Path::directoryExists(path))
		Path::createDirectory(path);
	dir.load(path);
	dir.removeParentDirectory();
	dir.removeFileExtensions(".m3u", true);
	dir.removeFileAttributes(FileAttributes::Directory | FileAttributes::ReadOnly | FileAttributes::System);
	dir.removeFileTypes(FileTypes::Shortcut);

	for (FileInfo& playlistFile : dir.fileList) {

		std::wifstream in;
		in.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		in.open(playlistFile.path);
		if (in.is_open()) {

			PlaylistSPtr playlist = std::make_shared<Playlist>(Path::removeExtension(playlistFile.name), playlistPlaylists);

			std::wstring line = L"";
			std::wstring currentTitle = L"";
			while (in) {
				std::getline(in, line);

				if (searchString(line, L"#EXTM3U") == 0) {
					continue;
				}
				else if (searchString(line, L"#EXTINF:") == 0) {
					int index = 7;
					for (; index < (int)line.length(); index++) {
						if (line[index] == ',') {
							index++;
							break;
						}
					}
					currentTitle = line.substr(index);
				}
				else if (!line.empty()) {
					MusicDataSPtr data = std::make_shared<MusicData>();
					data->path = line;
					data->title = wstr_to_str(currentTitle);
					if (loadMetadata(data)) {
						playlist->songs.push_back(data);
						if (!playlistMusic->containsSong(line)) {
							playlistMusic->songs.push_back(data);
							songs.push_back(data);

							std::string artistStr = (!data->artist.empty() ? data->artist : "Unknown");
							std::string albumStr = (!data->album.empty() ? data->album : "Unknown");
							std::string genreStr = (!data->genre.empty() ? data->genre : "Unknown");

							PlaylistSPtr artist = lib->playlistArtists->findFolder(artistStr);
							if (artist == nullptr) {
								artist = std::make_shared<Playlist>(artistStr, lib->playlistArtists);
								lib->playlistArtists->folders.push_back(artist);
							}
							artist->songs.push_back(data);

							PlaylistSPtr album = lib->playlistAlbums->findFolder(albumStr);
							if (album == nullptr) {
								album = std::make_shared<Playlist>(albumStr, lib->playlistAlbums);
								lib->playlistAlbums->folders.push_back(album);
							}
							album->songs.push_back(data);

							PlaylistSPtr genre = lib->playlistGenres->findFolder(genreStr);
							if (genre == nullptr) {
								genre = std::make_shared<Playlist>(genreStr, lib->playlistGenres);
								lib->playlistGenres->folders.push_back(genre);
							}
							genre->songs.push_back(data);
						}
					}
				}
			}
			in.close();

			playlist->sort();
			playlistPlaylists->folders.push_back(playlist);
		}
	}

	std::sort(lib->songs.begin(), lib->songs.end(), [](MusicDataSPtr a, MusicDataSPtr b) {
		return compareLess(a->title, b->title);
	});
	lib->playlistMusic->sort();
	lib->playlistArtists->sort();
	lib->playlistAlbums->sort();
	lib->playlistGenres->sort();
	playlistPlaylists->sort();
	for (PlaylistSPtr playlist : lib->playlistPlaylists->folders) {
		playlist->sort();
	}
}
bool MusicLibrary::loadMetadata(MusicDataSPtr data) {
	sf::FileInputStream in;
	in.open(data->path);

	AVFormatContext* formatContext = NULL;
	av_register_all();
	avformat_network_init();
	formatContext = avformat_alloc_context();
	uint8_t* ioContextBuffer = (uint8_t*)av_malloc(4096);
	if (!ioContextBuffer) {
		avformat_free_context(formatContext);
		return false;
	}
	AVIOContext* ioContext = avio_alloc_context(
		ioContextBuffer, 4096,
		0, &in, &::read, NULL, &::seek
	);
	if (!ioContext) {
		avformat_free_context(formatContext);
		av_freep(&ioContextBuffer);
		return false;
	}
	formatContext->pb = ioContext;

	if (avformat_open_input(&formatContext, NULL, NULL, NULL) != 0) {
		avformat_free_context(formatContext);
		av_freep(&ioContext->buffer);
		av_freep(&ioContext);
		return false;
	}
	AVDictionaryEntry* tag = NULL;
	while ((tag = av_dict_get(formatContext->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
		#ifdef STORE_METADATA
		data->metadata.push_back(std::pair<std::string, std::string>(tag->key, tag->value));
		#endif
		if (compareString(tag->key, "title", false))
			data->title = tag->value;
		else if (compareString(tag->key, "artist", false))
			data->artist = tag->value;
		else if (compareString(tag->key, "author", false) && data->artist.empty())
			data->artist = tag->value;
		else if (compareString(tag->key, "album_artist", false) && data->artist.empty())
			data->artist = tag->value;
		else if (compareString(tag->key, "album", false))
			data->album = tag->value;
		else if (compareString(tag->key, "genre", false))
			data->genre = tag->value;
	}

	return true;
}

void MusicLibrary::initialize() {
	av_log_set_callback(&::av_no_log);

	lib = std::shared_ptr<MusicLibrary>(new MusicLibrary());
}
void MusicLibrary::loadSongs() {
	lib->load();
}

#pragma endregion
//============ MUSIC =============
#pragma region Music

sf::Music& MusicLibrary::getMusic() {
	return lib->music;
}
int MusicLibrary::getPlayIndex() {
	return lib->playIndex;
}
void MusicLibrary::setPlayIndex(int index) {
	if (lib->currentPlaylist->songCount() == 0) {
		lib->playIndex = -1;
		lib->playingSong = nullptr;
		return;
	}
	lib->playingPlaylist = lib->currentPlaylist;
	if (index == -1)
		return;
	lib->playIndex = index;
	lib->playingSong = lib->playingPlaylist->songs[lib->playIndex];
	playNext(false);
}
MusicDataCSPtr MusicLibrary::getMusicData() {
	return lib->playingSong;
}
void MusicLibrary::playNext(bool increment) {
	if (lib->playIndex == -1)
		return;
	if (increment) {
		lib->playIndex = GMath::wrap(lib->playIndex + 1, (int)lib->playingPlaylist->songCount());
		lib->playingSong = lib->playingPlaylist->songs[lib->playIndex];
	}
	while (!lib->music.openFromFile(lib->playingSong->path)) {
		lib->playIndex = GMath::wrap(lib->playIndex + 1, (int)lib->playingPlaylist->songCount());
		lib->playingSong = lib->playingPlaylist->songs[lib->playIndex];
	}
	if (!lib->forceStopped)
		lib->music.play();
}
void MusicLibrary::playPrevious() {
	if (lib->playIndex == -1)
		return;
	do {
		lib->playIndex = GMath::wrap(lib->playIndex - 1, (int)lib->playingPlaylist->songCount());
		lib->playingSong = lib->playingPlaylist->songs[lib->playIndex];
	} while (!lib->music.openFromFile(lib->playingSong->path));
	if (!lib->forceStopped)
		lib->music.play();
}
bool MusicLibrary::isForceStopped() {
	return lib->forceStopped;
}
void MusicLibrary::setForceStopped(bool stopped) {
	lib->forceStopped = stopped;
}

#pragma endregion
//========== PLAYLISTS ===========
#pragma region Playlists

const std::vector<MusicDataSPtr>& MusicLibrary::getSongs() {
	return lib->songs;
}
int MusicLibrary::getTotalSongCount() {
	return (int)lib->songs.size();
}

PlaylistCSPtr MusicLibrary::getCurrentPlaylist() {
	return lib->currentPlaylist;
}
PlaylistCSPtr MusicLibrary::getPlayingPlaylist() {
	return lib->playingPlaylist;
}
int MusicLibrary::getTotalCount() {
	return lib->currentPlaylist->totalCount();
}
int MusicLibrary::getFolderCount() {
	return lib->currentPlaylist->folderCount();
}
int MusicLibrary::getSongCount() {
	return lib->currentPlaylist->songCount();
}
bool MusicLibrary::hasParent() {
	return lib->currentPlaylist->hasParent();
}

void MusicLibrary::gotoParentPlaylist() {
	lib->currentPlaylist = lib->currentPlaylist->parent;
}
void MusicLibrary::gotoSubPlaylist(int index) {
	lib->currentPlaylist = lib->currentPlaylist->folders[index];
}

#pragma endregion
//=================================================================|
