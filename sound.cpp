#include "main.hpp"

Sound::Sound(Position4 sourcePosition, int worldVol, int playVol)
	: sourcePosition(sourcePosition), worldVolume(worldVol), playbackVolume(playVol), reactable(true)
{
	//speech.setText("123 123 this is a test");
	//
	//sound.load("E:\\Projects\\Visual Studio\\Libtcod Development\\Epsilon\\data\\audio\\beep - 03.wav");
	//
	//ENGINE->audio->play(speech);
}
