#include "main.hpp"

struct Sound //deals with world sounds and playback
{
	Position4 sourcePosition;

	//testing
	SoLoud::Soloud audio;
	SoLoud::Speech speech;

	//
	bool reactable;
	bool completed;

	int worldVolume; //the volume of the sound to other creatures in decibels
	int playbackVolume; //the volume of the sound to the player

	//effects properties

	Sound(Position4 sourcePosition, int worldVol, int playVol);
	~Sound();

	void update();
};

/*

DECIBEL VOLUME TABLE

	120 : gunshot
	060 : conversation

*/

struct SoundManager
{
	std::vector<std::shared_ptr<SoLoud::Speech>> soundList; //make pointer? need?

	std::shared_ptr<SoLoud::Soloud> soLoud;

	SoundManager();
	~SoundManager();
	
	void update();

	void playSound();  //make return handle??
};