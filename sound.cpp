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
	soLoud = SoLoud::Soloud();
	soLoud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, SoLoud::Soloud::SDL2);
}

SoundManager::~SoundManager()
{
	soLoud.deinit(); //crashes on exit, even if removed | WORKS AS VAR INSTEAD OF POINTER
}

void SoundManager::update()
{
	//update 3d sound eventually
}

void SoundManager::playSound()
{
	std::shared_ptr<SoLoud::Speech> speech = std::make_shared<SoLoud::Speech>();
	speech->setText("testing testing");
	soLoud.play(*speech);

	soundList.push_back(speech); //should find a way to clean up
}
