#include "main.hpp"

//Initializtion Through Engine Constructor
Engine engine(120, 64);

//Main Game Loop

//change to main to have terminal show up
//else use WinMain to hide console
int main ()
{
	while (!TCODConsole::isWindowClosed())
	{
		engine.update();
		engine.render();
	}
	TCOD_quit();
	return 0;
}
