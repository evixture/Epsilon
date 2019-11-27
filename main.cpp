#include "main.hpp"

 std::shared_ptr<Engine> engine = std::make_shared<Engine>(120, 64);

int WinMain()
{
	while (!TCODConsole::isWindowClosed() && engine->gamestate != Engine::EXIT)
	{
		engine->update();
		engine->render();
		//Gui::Color::Panel_Draw_BG = TCODColor(0, 0, 0);
	}

	TCOD_quit();
	return 0;
}
