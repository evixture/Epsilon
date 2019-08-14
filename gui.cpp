#include "main.hpp"

Window::Window(int consoleW, int consoleH)
	:consoleW(consoleW), consoleH(consoleH)
{
	console = new TCODConsole(consoleW, consoleH);
}

Window::~Window()
{
	delete console;
}