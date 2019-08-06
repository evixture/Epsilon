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

//TODO : test static class vectors that pushes itself in constructor
//TODO : optimization
//TODO : test renderer performance
//TODO : add more structs
//TODO : try templates
//TODO : add logger