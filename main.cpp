#include "main.hpp"

Engine engine(120, 64);

int main()
{
	while (!TCODConsole::isWindowClosed())
	{
		TCODConsole::root->clear();
		engine.update();
		engine.render();
		TCODConsole::flush();
		TCODConsole::root->printf(1, 1, "Test");
	}
	TCOD_quit();
	return 0;
}