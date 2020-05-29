#include "main.hpp"

/*
	Audio
	|	PositionalTrackedSound
	|	L	Sound
	|	PositionalStaticSound
	|	L	Sound
	|	Sound
*/

struct Sound //deals with world sounds and playback
{
	std::shared_ptr<SoLoud::Speech> speech;

	virtual std::pair<bool, Position4> getPosition(); //returns if sound is 3d and its position

	float worldVolume; //the volume of the sound to other creatures in decibels
	float playbackVolume; //the volume of the sound to the player

	Sound(std::string speechText, float worldVol, float playVol);

protected:
	std::pair<bool, Position4> position;
};

struct PositionalStaticSound : public Sound
{
	PositionalStaticSound(std::string speechText, Position4 sourcePosition, float worldVol, float playVol);

private:
	Position4 sourcePosition; //later move to derived class
};

struct PositionalTrackedSound : public Sound
{
	PositionalTrackedSound(std::string speechText, Position4* sourcePosition, float worldVol, float playVol);

private:
	Position4* sourcePosition; //pointer to position so location is always accurate
};

/*
DECIBEL VOLUME TABLE

	120 : gunshot
	060 : conversation
*/

struct Audio
{
	SoLoud::Soloud soLoud;
	std::vector<std::pair<int, Sound>> soundList; //list of sounds with their handles

	Audio();
	~Audio();
	
	void update();

	int playSound(Sound sound);
};
