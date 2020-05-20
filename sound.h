#include "main.hpp"

/*

SOUND CLASS
	Sound class contains the soloud audio thing
	can be derived for other forms and control

	Sound manager takes the sound soloud audio and plays it after pushing

HOW TO GET SOLOUD TO WORK
	SoLoud core needs to be normal var
	sounds in current form needs to be pointer so it stays
*/


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
	std::vector<std::shared_ptr<SoLoud::Speech>> soundList; //Speech needs to be pointer

	SoLoud::Soloud soLoud; //trying as not pointer

	SoundManager();
	~SoundManager();
	
	void update();

	void playSound();  //make return handle??
};