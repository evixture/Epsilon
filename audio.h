#include "main.hpp"

/*

SOUND CLASS
	Sound class contains the soloud audio thing
	can be derived for other forms and control

	Sound manager takes the sound soloud audio and plays it after pushing

HOW TO GET SOLOUD TO WORK
	SoLoud core needs to be normal var
	sounds in current form needs to be pointer so it stays, can be in a class
*/


struct Sound //deals with world sounds and playback
{
	std::shared_ptr<SoLoud::Speech> speech;

	virtual std::pair<bool, Position4> getPosition(); //returns if sound is 3d and its position

	float worldVolume; //the volume of the sound to other creatures in decibels
	float playbackVolume; //the volume of the sound to the player

	Sound(std::string speechText, float worldVol, float playVol);
};

struct PositionalStaticSound : public Sound
{
	Position4 sourcePosition; //later move to derived class

	std::pair<bool, Position4> getPosition(); //returns if sound is 3d and its position

	PositionalStaticSound(std::string speechText, Position4 sourcePosition, float worldVol, float playVol);
};

struct PositionalTrackedSound : public Sound
{
	Position4* sourcePosition; //pointer to position so location is always accurate

	std::pair<bool, Position4> getPosition(); //returns if sound is 3d and its position

	PositionalTrackedSound(std::string speechText, Position4* sourcePosition, float worldVol, float playVol);
};

/*

DECIBEL VOLUME TABLE

	120 : gunshot
	060 : conversation

*/

struct Audio
{
	std::vector<std::pair<int, Sound>> soundList; //list of sounds with their handles

	Audio();
	~Audio();
	
	void update();

	int playSound(Sound sound);

private:
	SoLoud::Soloud soLoud; //trying as not pointer
};

/*

STRUCTURE
	Engine
		Settings
		Gui
		Audio

			Sound Types
				2d sound, not reactable
				3d static, reactable
				3d tracked, reactable
*/