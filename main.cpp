#include "main.hpp"

//current window dimensions goos for 1080p
Engine engine(120, 64);

int main()
{
	while (!TCODConsole::isWindowClosed())
	{
		engine.update();
		engine.render();
		TCODConsole::flush();
	}
}

//TODO : ACTUALLY MAKE SOMETHING