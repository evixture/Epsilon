#include "main.hpp"

struct Window
{
	int consoleW;
	int consoleH;
	TCODConsole* console;

	Window(int consoleW, int consoleH);
	~Window();
};

class Gui
{
	std::shared_ptr<Map> map;
};
