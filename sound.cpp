#include "main.hpp"

Sound::Sound(Position4 sourcePosition, int worldVol, int playVol)
	: sourcePosition(sourcePosition), worldVolume(worldVol), playbackVolume(playVol), reactable(true)
{
	//speech.setText("123 123 this is a test");
	//
	//sound.load("E:\\Projects\\Visual Studio\\Libtcod Development\\Epsilon\\data\\audio\\beep - 03.wav");
	//
	//ENGINE->audio->play(speech);

	//// Define a couple of variables
	SoLoud::Soloud soloud;  // SoLoud engine core
	//SoLoud::Speech speech;  // A sound source (speech, in this case)

	// Configure sound source
	speech.setText("1 2 3   1 2 3   Hello world. Welcome to So-Loud.");

	// initialize SoLoud.
	soloud.init();

	// Play the sound source (we could do this several times if we wanted)
	soloud.play(speech);

	// Wait until sounds have finished
	while (soloud.getActiveVoiceCount() > 0) //needed to play sound but acts weird
	{
		// Still going, sleep for a bit
		SoLoud::Thread::sleep(100);
	}

	// Clean up SoLoud
	soloud.deinit();
}
