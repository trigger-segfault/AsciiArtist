#ifndef MP3_H_INCLUDED
#define MP3_H_INCLUDED

#include <SFML/Audio.hpp>
#include <mpg123.h>

namespace sfe {
class Mp3 : public sf::SoundStream {
public:
	Mp3();
	~Mp3();

	bool openFromFile(const std::string& filename);
	bool openFromMemory(void* data, size_t sizeInBytes);

protected:
	bool onGetData(Chunk& data);
	void onSeek(sf::Time timeOffset);

private:
	mpg123_handle*      myHandle;
	size_t              myBufferSize;
	unsigned char*      myBuffer;
	sf::Mutex           myMutex;
};

} // namespace sfe

#endif // MP3_H_INCLUDED