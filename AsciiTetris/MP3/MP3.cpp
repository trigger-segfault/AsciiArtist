
#include "stdafx.h"
#include "Mp3.h"
#include <iostream>

namespace sfe {

struct Mp3MemoryData {
	void* data;
	size_t size;
	off_t offset;
};

ssize_t MemoryDataRead(void* rawMp3Data, void* buffer, size_t nbyte) {
	Mp3MemoryData* mp3Data = (Mp3MemoryData*)rawMp3Data;
	if (mp3Data->offset >= (ssize_t)mp3Data->size) {
		memset(buffer, 0, nbyte);
		return (ssize_t)0;
	}
	else if (mp3Data->offset + (ssize_t)nbyte > (ssize_t)mp3Data->size) {
		size_t readSize = mp3Data->size - mp3Data->offset;
		size_t memSetSize = mp3Data->offset + nbyte - (ssize_t)mp3Data->size;
		memcpy_s(buffer, readSize, (unsigned char*)mp3Data->data + mp3Data->offset, readSize);
		memset(buffer, 0, memSetSize);
		mp3Data->offset += readSize;
		return (ssize_t)readSize;
	}
	else {
		memcpy_s(buffer, nbyte, (unsigned char*)mp3Data->data + mp3Data->offset, nbyte);
		mp3Data->offset += nbyte;
		return (ssize_t)nbyte;
	}
}

off_t MemoryDataLSeek(void* rawMp3Data, off_t offset, int whence) {
	Mp3MemoryData* mp3Data = (Mp3MemoryData*)rawMp3Data;
	switch (whence) {
	case SEEK_SET: mp3Data->offset = offset; break;
	case SEEK_CUR: mp3Data->offset += offset; break;
	case SEEK_END: mp3Data->offset = mp3Data->size + offset; break;
	}
	return mp3Data->offset;
}

void MemoryDataCleanup(void* rawMp3Data) {
	delete rawMp3Data;
}

Mp3::Mp3() :
	myHandle(NULL),
	myBufferSize(0),
	myBuffer(NULL) {
	int  err = MPG123_OK;
	if ((err = mpg123_init()) != MPG123_OK) {
		std::cerr << mpg123_plain_strerror(err) << std::endl;
		return;
	}

	myHandle = mpg123_new(NULL, &err);
	if (!myHandle) {
		std::cerr << "Unable to create mpg123 handle: " << mpg123_plain_strerror(err) << std::endl;
		return;
	}

	mpg123_replace_reader_handle(myHandle, &MemoryDataRead, &MemoryDataLSeek, &MemoryDataCleanup);
}

Mp3::~Mp3() {
	stop();

	if (myBuffer) {
		delete[] myBuffer;
		myBuffer = NULL;
	}

	mpg123_close(myHandle);
	mpg123_delete(myHandle);
	mpg123_exit();
}

bool Mp3::openFromFile(const std::string& filename) {
	stop();

	if (myBuffer) {
		delete[] myBuffer;
		myBuffer = NULL;
	}

	if (myHandle)
		mpg123_close(myHandle);

	if (mpg123_open(myHandle, filename.c_str()) != MPG123_OK) {
		std::cerr << mpg123_strerror(myHandle) << std::endl;
		return false;
	}

	long rate = 0;
	int  channels = 0, encoding = 0;
	if (mpg123_getformat(myHandle, &rate, &channels, &encoding) != MPG123_OK) {
		std::cerr << "Failed to get format information for \"" << filename << "\"" << std::endl;
		return false;
	}

	myBufferSize = mpg123_outblock(myHandle);
	myBuffer = new unsigned char[myBufferSize];
	if (!myBuffer) {
		std::cerr << "Failed to reserve memory for decoding one frame for \"" << filename << "\"" << std::endl;
		return false;
	}

	initialize(channels, rate);

	return true;
}
bool Mp3::openFromMemory(void* data, size_t sizeInBytes) {
	stop();

	if (myBuffer) {
		delete[] myBuffer;
		myBuffer = NULL;
	}

	if (myHandle)
		mpg123_close(myHandle);

	Mp3MemoryData* mp3Data = new Mp3MemoryData{ data, sizeInBytes, 0 };
	if (!mp3Data) {
		std::cerr << "Failed to reserve memory for keeping track of memory data" << std::endl;
	}

	if (mpg123_open_handle(myHandle, mp3Data) != MPG123_OK) {
		std::cerr << mpg123_strerror(myHandle) << std::endl;
		delete mp3Data;
		mp3Data = NULL;
		return false;
	}

	long rate = 0;
	int  channels = 0, encoding = 0;
	if (mpg123_getformat(myHandle, &rate, &channels, &encoding) != MPG123_OK) {
		std::cerr << "Failed to get format information for \"Memory Object\"" << std::endl;
		return false;
	}

	myBufferSize = mpg123_outblock(myHandle);
	myBuffer = new unsigned char[myBufferSize];
	if (!myBuffer) {
		std::cerr << "Failed to reserve memory for decoding one frame for \"Memory Object\"" << std::endl;
		return false;
	}

	initialize(channels, rate);

	return true;
}

bool Mp3::onGetData(Chunk& data) {
	sf::Lock lock(myMutex);

	if (myHandle) {
		size_t done;
		mpg123_read(myHandle, myBuffer, myBufferSize, &done);

		data.samples = (short*)myBuffer;
		data.sampleCount = done / sizeof(short);

		return (data.sampleCount > 0);
	}
	else {
		return false;
	}
}

void Mp3::onSeek(sf::Time timeOffset) {
	sf::Lock lock(myMutex);

	if (myHandle)
		mpg123_seek(myHandle, (off_t)timeOffset.asSeconds(), 0);
}

} // namespace sfe