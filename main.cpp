#include "main.hpp"

Engine engine(128, 72);

int main()
{
	while (!TCODConsole::isWindowClosed())
	{
		engine.update();
		engine.render();
		TCODConsole::flush();
	}
}