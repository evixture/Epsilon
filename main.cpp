#include "main.hpp"

std::shared_ptr<Engine> engine = std::make_shared<Engine>(120, 64);

int WinMain()
{
	while (!TCODConsole::isWindowClosed() && engine->gamestate != Engine::Gamestate::EXIT) //while the window is not closed and the gamestate is not to exit
	{
		//SETTINGS->beforeFrameTime = std::chrono::high_resolution_clock::now();
		engine->update();
		engine->render();
		//SETTINGS->afterFrameTime = std::chrono::high_resolution_clock::now();
		//SETTINGS->lastFrameTime = SETTINGS->systemClock.restart();
	}
	
	TCOD_quit(); //does proper cleanup on exit
	return 0;

	//pugi::xml_document document;
	//pugi::xml_parse_result result = document.load_file("data/maps/testxmlmap.xml");
}
