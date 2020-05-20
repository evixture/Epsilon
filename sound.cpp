#include "main.hpp"

Sound::Sound(std::string speechText, Position4 sourcePosition, int worldVol, int playVol)
	: sourcePosition(sourcePosition), worldVolume(worldVol), playbackVolume(playVol), reactable(true), completed(false)
{
	speech = std::make_shared<SoLoud::Speech>();
	speech->setText(speechText.c_str());
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

int SoundManager::playSound(Sound sound)
{
	int handle = soLoud.play(*sound.speech);

	soLoud.setVolume(handle, sound.playbackVolume);
	soundList.push_back(sound.speech); //should find a way to clean up

	return handle;
}
