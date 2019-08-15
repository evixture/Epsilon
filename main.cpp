#include "main.hpp"

Engine engine(120, 64);

int main()
{
	while (!TCODConsole::isWindowClosed())
	{
		TCODConsole::root->clear();
		engine.render();
		TCODConsole::flush();
	}
	return 0;
}