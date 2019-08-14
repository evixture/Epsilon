#include "main.hpp"

Engine engine(120, 64);

int main()
{
	while (TCODConsole::isWindowClosed())
	{
		TCODConsole::root->clear();
	}
	return 0;
}