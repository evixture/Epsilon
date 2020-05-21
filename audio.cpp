#include "main.hpp"

//SOUND BASE
Sound::Sound(std::string speechText, bool positional, Position4 sourcePosition, float worldVol, float playVol)
	: sourcePosition(sourcePosition), positional(positional), worldVolume(worldVol), playbackVolume(playVol), reactable(true), completed(false)
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
Audio::Audio()
{
	soLoud = SoLoud::Soloud();
	soLoud.init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, SoLoud::Soloud::SDL2); //left handed coords make crash on exit
	soLoud.setGlobalVolume(.006f); //so your ears don't get blown out
	soLoud.set3dSoundSpeed(560.0f); //speed of sound; (1125 fps) div 2 bc tiles are 2 ft
}

Audio::~Audio()
{
	soLoud.deinit(); //crashes on exit, even if removed | WORKS AS VAR INSTEAD OF POINTER
}

void Audio::update()
{
	//update 3d sound eventually
	//soLoud.set3dListenerPosition((float)WORLD->debugmap->player->mapPosition.x, (float)WORLD->debugmap->player->mapPosition.height, (float)WORLD->debugmap->player->mapPosition.y);

	soLoud.update3dAudio();
}

int Audio::playSound(Sound sound)
{
	int handle = 0;

	if (sound.positional) //3d sound
	{
		handle = soLoud.play3d(*sound.speech, (float)sound.sourcePosition.x, (float)sound.sourcePosition.height, (float)sound.sourcePosition.y); //is it able to be moved to play function in sound?
		soLoud.set3dSourceAttenuation(handle, SoLoud::AudioSource::ATTENUATION_MODELS::EXPONENTIAL_DISTANCE, .5f); //sound distance decay
	}
	else //2d sound
	{
		handle = soLoud.play(*sound.speech);
	}

	soLoud.setVolume(handle, sound.playbackVolume);
	soundList.push_back(sound.speech); //should find a way to clean up

	return handle;
}
