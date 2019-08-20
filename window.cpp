#include "main.hpp"

Window::Window(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor)
	:consoleW(consoleW), consoleH(consoleH), bgColor(bgColor), fgColor(fgColor)
{
	console = new TCODConsole(consoleW, consoleH);
	console->setDefaultBackground(bgColor);
	console->setDefaultForeground(fgColor);
}

Window::~Window()
{
	delete console;
}

Ribon::Ribon(const char* windowName, int windowW)
	: windowName(windowName), windowW(windowW)
{
	ribonWindow = std::make_shared<Window>(windowW, 1, TCODColor::darkBlue, TCODColor::white);
}

void Ribon::render(std::shared_ptr<Window> window)
{
	for (int x = 0; x < windowW; x++)
	{
		ribonWindow->console->setCharBackground(x, 0, ribonWindow->bgColor);
	}

	window->console->printf(2, 1, windowName);
}

Message::Message(const char* text, TCODColor color)
	:text(text), color(color)
{
}

GuiWindow::GuiWindow(int w, int h, const char* guiWindowName, int rx, int ry)
	: w(w), h(h), guiWindowName(guiWindowName), renderpos(Position(rx, ry))
{
	mainWindow = std::make_shared<Window>(w, h, TCODColor::black, TCODColor::white);
	ribon = std::make_shared<Ribon>(guiWindowName, w);
	drawWindow = std::make_shared<Window>(w, h - 1, TCODColor::black, TCODColor::white);
}

void GuiWindow::render()
{
	ribon->render(mainWindow);
	ribon->ribonWindow->console->blit(ribon->ribonWindow->console, 1, 1, w, 1, mainWindow->console, 1, 1);

	//draw window render

	mainWindow->console->blit(mainWindow->console, 1, 1, w, h, TCODConsole::root, renderpos.x, renderpos.y);

	//mainWindow->console->printf(0, 0, "Test");
}

GuiMap::GuiMap(int w, int h, int rx, int ry)
{
	mapWindow = std::make_shared<GuiWindow>(w, h, "Map", rx, ry);
	map = std::make_shared<Map>(w, h);
}

void GuiMap::render()
{
	map->render(mapWindow);
	mapWindow->render();
}