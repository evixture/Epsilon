#include "main.hpp"

std::shared_ptr<Engine> engine = std::make_shared<Engine>(120, 64);

int WinMain()
{
	while (!TCODConsole::isWindowClosed() && engine->gamestate != Engine::Gamestate::EXIT) //while the window is not closed and the gamestate is not to exit
	{
		engine->update();
		engine->render();
	}

	TCOD_quit(); //does proper cleanup on exit
	return 0;
}
