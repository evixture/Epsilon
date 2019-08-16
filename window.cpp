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
	ribonWindow = std::make_shared<Window>(windowW, 1);
	ribonWindow->console->setDefaultBackground(TCODColor::darkBlue);
	ribonWindow->console->setDefaultForeground(TCODColor::white);
}

void Ribon::render(std::shared_ptr<Window> window)
{
	window->console->printf(2, 1, windowName);
}

Message::Message(const char* text, TCODColor color)
	:text(text), color(color)
{
}

GuiWindow::GuiWindow(int w, int h, const char* guiWindowName, int rx, int ry)
	:w(w), h(h), guiWindowName(guiWindowName), renderpos(Position(rx, ry))
{
	mainWindow = std::make_shared<Window>(w, h);
	ribon = std::make_shared<Ribon>(guiWindowName, w);
	drawWindow = std::make_shared<Window>(w, h - 1);
}

void GuiWindow::render(std::shared_ptr<Window> window)
{
	ribon->render(window);
	ribon->ribonWindow->console->blit(ribon->ribonWindow->console, 1, 1, w, 1, mainWindow->console, 1, 1);

	//draw window render

	mainWindow->console->blit(mainWindow->console, 1, 1, w, h, TCODConsole::root, renderpos.x, renderpos.y);
}