#include "main.hpp"

//SOUND BASE
Sound::Sound(std::string speechText, Position4 sourcePosition, float worldVol, float playVol)
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

//SOUND MANAGER
SoundManager::SoundManager()
{
	soLoud = SoLoud::Soloud();
	soLoud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, SoLoud::Soloud::SDL2); //left handed coords make crash on exit
	soLoud.setGlobalVolume(.006f); //so your ears don't get blown out
	soLoud.set3dSoundSpeed(560.0f); //speed of sound (1125 fps) div 2 bc tiles are 2 ft
}

SoundManager::~SoundManager()
{
	soLoud.deinit(); //crashes on exit, even if removed | WORKS AS VAR INSTEAD OF POINTER
}

void SoundManager::update()
{
	//update 3d sound eventually
	soLoud.set3dListenerPosition(WORLD->debugmap->player->mapPosition.x, WORLD->debugmap->player->mapPosition.height, WORLD->debugmap->player->mapPosition.y);


	soLoud.update3dAudio();
}

int SoundManager::playSound(Sound sound)
{
	int handle = soLoud.play3d(*sound.speech, sound.sourcePosition.x, sound.sourcePosition.height, sound.sourcePosition.y);

	soLoud.setVolume(handle, sound.playbackVolume);
	soundList.push_back(sound.speech); //should find a way to clean up
	soLoud.set3dSourceAttenuation(handle, SoLoud::AudioSource::ATTENUATION_MODELS::EXPONENTIAL_DISTANCE, .5f); //sound distance decay

	return handle;
}
