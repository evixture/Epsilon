#include "main.hpp"

//SOUND BASE
Sound::Sound(std::string speechText, float worldVol, float playVol)
	: worldVolume(worldVol), playbackVolume(playVol), position(std::pair<bool, Position4>(false, Position4(0, 0, 0, 0)))
{
	speech = std::make_shared<SoLoud::Speech>();
	speech->setText(speechText.c_str());
}

std::pair<bool, Position4> Sound::getPosition() //not getting passed on to derived classes?
{
	return position;
}

//POSITIONAL SOUND THAT DOES NOT TRACK
PositionalStaticSound::PositionalStaticSound(std::string speechText, Position4 sourcePosition, float worldVol, float playVol)
	: Sound(speechText, worldVol, playVol), sourcePosition(sourcePosition)
{
	speech = std::make_shared<SoLoud::Speech>();
	speech->setText(speechText.c_str());
	position = std::pair<bool, Position4>(true, sourcePosition);
}

//POSITIONAL SOUND THAT TRACKS COORDS
PositionalTrackedSound::PositionalTrackedSound(std::string speechText, Position4* sourcePosition, float worldVol, float playVol)
	: Sound(speechText, worldVol, playVol), sourcePosition(sourcePosition)
{
	speech = std::make_shared<SoLoud::Speech>();
	speech->setText(speechText.c_str());
	position = std::pair<bool, Position4>(true, *sourcePosition);
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
	soundList.clear();
	soLoud.deinit(); //crashes on exit, even if removed | WORKS AS VAR INSTEAD OF POINTER
}

void Audio::update()
{
	if (soLoud.getActiveVoiceCount() < soundList.size()) //if more sounds are in the list than are actually playing
	{
		static int excessSounds = (int)soundList.size() - soLoud.getActiveVoiceCount();

		for (int e = 0; e < excessSounds; e++) soundList.erase(soundList.begin()); //delete the front (oldest) sound until all old sounds are gone //may delete old long playing sounds?
	}

	soLoud.set3dListenerPosition((float)WORLD->debugmap->player->mapPosition.x, (float)WORLD->debugmap->player->mapPosition.h, (float)WORLD->debugmap->player->mapPosition.y);
	for (int i = 0; i < soundList.size(); i++)
	{
		if (soundList[i].second.getPosition().first == true) //if sound of the sound list is 3d
		{
			soLoud.set3dSourcePosition(soundList[i].first, (float)soundList[i].second.getPosition().second.x, (float)soundList[i].second.getPosition().second.h, (float)soundList[i].second.getPosition().second.y); //should update tracked sounds
		}
	}

	soLoud.update3dAudio();
}

int Audio::playSound(Sound sound)
{
	int handle = 0; //no static?

	if (sound.getPosition().first == true) //3d sound
	{
		handle = soLoud.play3d(*sound.speech, (float)sound.getPosition().second.x, (float)sound.getPosition().second.h, (float)sound.getPosition().second.y);
		soLoud.set3dSourceAttenuation(handle, SoLoud::AudioSource::ATTENUATION_MODELS::EXPONENTIAL_DISTANCE, .5f); //sound distance decay

		if (GUI->activeWindow == Gui::ActiveWindow::NONE || GUI->activeWindow == Gui::ActiveWindow::INVENTORYFULL) //map active
		{
			WORLD->addSound(sound); //put sound in world so ai can react
		}
	}
	else //2d sound
	{
		handle = soLoud.play(*sound.speech, sound.playbackVolume);
	}

	soLoud.setVolume(handle, sound.playbackVolume);
	soundList.push_back(std::pair<int, Sound>(handle, sound)); //should find a way to clean up

	return handle;
}

