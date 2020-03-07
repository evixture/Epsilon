#include "main.hpp"

struct Sound //deals with world sounds and playback
{
	Position4 soundSource;

	//sound file

	int worldVolume; //the volume of the sound to other creatures in decibels
	int playbackVolume; //the volume of the sound to the player

	//effects properties

	Sound(Position4 soundSource, int worldVol, int playVol);
};

/*

DECIBEL VOLUME TABLE

	120 : gunshot
	060 : conversation

*/