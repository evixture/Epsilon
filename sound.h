#include "main.hpp"

/*

SOUND CLASS
	Sound class contains the soloud audio thing
	can be derived for other forms and control

	Sound manager takes the sound soloud audio and plays it after pushing

HOW TO GET SOLOUD TO WORK
	SoLoud core needs to be normal var
	sounds in current form needs to be pointer so it stays

PROBLEMS
	How to get rid of source position with base sound class?
*/


struct Sound //deals with world sounds and playback
{
	Position4 sourcePosition; //later move to derived class

	//testing
	std::shared_ptr<SoLoud::Speech> speech;

	//
	bool reactable;
	bool completed;

	float worldVolume; //the volume of the sound to other creatures in decibels
	float playbackVolume; //the volume of the sound to the player

	//effects properties

	Sound(std::string speechText, Position4 sourcePosition, float worldVol, float playVol);
	~Sound();

	void update();
};

struct PositionalSound : public Sound
{
	Position4 sourcePosition; //later move to derived class
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

	int playSound(Sound sound);
};