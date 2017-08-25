// SFMLCustom.cpp : Defines the entry point for the console application.
//

#include <SFML/Audio/Music.hpp>
#include <iostream>

int main()
{


	sf::Music music;
	//music.openFromFile(R"(C:\Users\Onii-chan\Music\AsciiVis\Highscore.ogg)");
	music.openFromFile(R"(C:\Users\Onii-chan\Music\iTunes\iTunes Media\Music\Ylvis\The Fox (What Does The Fox Say_) - Singl\02 The Fox (Instrumental).m4a)");
	//music.openFromFile(R"(C:\Users\Onii-chan\Music\iTunes\iTunes Media\Music\LukHash\Falling Apart\08 Broken Star.m4a)");
	music.play();

	while (music.getStatus() == sf::SoundSource::Status::Playing) {
		//std::cout << music.getSampleStart() << std::endl;
		std::cout << music.getAvailableSamples() << std::endl;
	}

	system("pause");
    return 0;
}

