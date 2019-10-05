#include "main.hpp"

//Initializtion Through Engine Constructor
std::shared_ptr<Engine> engine = std::make_shared<Engine>(120, 64);
//std::shared_ptr<Engine> engine(120, 64);

//Main Game Loop

//change to main to have terminal show up
//else use WinMain to hide console
int WinMain()
{
	while (!TCODConsole::isWindowClosed() && engine->gamestate != Engine::EXIT)
	{
		engine->update();
		engine->render();
	}
	TCOD_quit();
	return 0;
}
