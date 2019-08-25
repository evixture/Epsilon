#include "main.hpp"

//Initializtion Through Engine Constructor
Engine engine(120, 64);

//Main Game Loop
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