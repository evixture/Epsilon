#include "main.hpp"

std::shared_ptr<Engine> engine = std::make_shared<Engine>(120, 64);

//std::shared_ptr<SoLoud::Soloud> soloud;

int main() //should switch to winmain later if ready for the headache
{
	//SoLoud::Soloud soloud; //move to engine

	//soloud.init();

	while (!TCODConsole::isWindowClosed() && engine->gamestate != Engine::Gamestate::EXIT) //while the window is not closed and the gamestate is not to exit
	{
		engine->update();
		engine->render();
	}
	
	////soloud->deinit();
	TCOD_quit(); //does proper cleanup on exit
	//
	//// Define a couple of variables
	//SoLoud::Soloud soloud;  // SoLoud engine core
	//
	//// initialize SoLoud.
	//soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::SDL2);
	//
	//
	//// Configure sound source
	//SoLoud::Speech speech;  // A sound source (speech, in this case)
	//speech.setText("1 2 3   1 2 3   Hello world. Welcome to So-Loud.");
	//
	//// Play the sound source (we could do this several times if we wanted)
	//soloud.play(speech);
	//
	//// Wait until sounds have finished
	//while (soloud.getActiveVoiceCount() > 0)
	//{
	//	// Still going, sleep for a bit
	//	SoLoud::Thread::sleep(100);
	//}
	//
	//// Clean up SoLoud
	//soloud.deinit();

	// All done.
	//return 0;

	return 0;
}
