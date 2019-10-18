#include "main.hpp"

 std::shared_ptr<Engine> engine = std::make_shared<Engine>(120, 64);

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
