#include "main.hpp"

Engine engine(20, 20);

int main()
{
	while (!TCODConsole::isWindowClosed())
	{
		engine.update();
		engine.render();
		TCODConsole::flush();
	}
}