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

Ribon::Ribon(const char* windowName, int windowW)
	: windowName(windowName), windowW(windowW)
{
	ribonWindow = Window(windowW, 1);
	ribonWindow.console->setDefaultBackground(TCODColor::darkBlue);
	ribonWindow.console->setDefaultForeground(TCODColor::white);
}

void Ribon::render(std::shared_ptr<Window> window)
{
	window->console->printf(2, 1, windowName);
}

GuiWindow::GuiWindow(int w, int h, const char* guiWindowName)
	:w(w), h(h), guiWindowName(guiWindowName)
{
	mainWindow = Window(w, h);
	ribon = Ribon(guiWindowName, w);
	drawWindow = Window(w, h - 1);
}

