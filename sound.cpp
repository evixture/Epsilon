#include "main.hpp"

Sound::Sound(Position4 sourcePosition, int worldVol, int playVol)
	: sourcePosition(sourcePosition), worldVolume(worldVol), playbackVolume(playVol), reactable(true), completed(false)
{
	
}

Sound::~Sound()
{
}

void Sound::update()
{
}

SoundManager::SoundManager()
{
	soLoud = std::make_shared<SoLoud::Soloud>();
	soLoud->init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, SoLoud::Soloud::SDL2);
}

SoundManager::~SoundManager()
{
	soLoud->deinit();
}

void SoundManager::update()
{
}

void SoundManager::playSound()
{
	std::shared_ptr<SoLoud::Speech> speech = std::make_shared<SoLoud::Speech>();
	speech->setText("testing testing");
	soLoud->play(*speech);

	soundList.push_back(speech);

	//plays sound
	//adds sound to list?? no need
}
