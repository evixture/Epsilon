#include "main.hpp"

//TODO : add more defs for colors in window
#define RIBONBGCOLOR TCODColor::darkBlue

//Window Struct
Window::Window(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor)
	:consoleW(consoleW), consoleH(consoleH), bgColor(bgColor), fgColor(fgColor)
{
	console = new TCODConsole(consoleW, consoleH);
}

Window::~Window()
{
	delete console;
}

void Window::render()
{
	console->setDefaultBackground(bgColor);
	console->setDefaultForeground(fgColor);
	console->clear();
}

//Ribon Struct
Ribon::Ribon(const char* windowName, int windowW)
	: windowName(windowName), windowW(windowW)
{
	ribonWindow = std::make_shared<Window>(windowW, 1, RIBONBGCOLOR, TCODColor::white);
}

//Ribon Render
void Ribon::render()
{
	ribonWindow->render();
	ribonWindow->console->printf(0, 0, "[%s]", windowName);
}

//GuiWindow Struct
Panel::Panel(int w, int h, const char* panelName, int rx, int ry)
	: w(w), h(h), panelName(panelName), renderpos(Position(rx, ry))
{
	mainWindow = std::make_shared<Window>(w, h, TCODColor::black, TCODColor::white);
	ribon = std::make_shared<Ribon>(panelName, w);
	drawWindow = std::make_shared<Window>(w, h - 1, TCODColor::darkestGrey, TCODColor::white);
}

void Panel::update()
{
	return;
}

//GuiWindow clears window
void Panel::clearWindow()
{
	ribon->render();
	drawWindow->render();
}

//GuiWindow blits the consoles together and pushes to root
void Panel::pushWindow()
{
	ribon->ribonWindow->console->blit(ribon->ribonWindow->console, 0, 0, ribon->ribonWindow->consoleW, ribon->ribonWindow->consoleH, mainWindow->console, 0, 0, 1, 1);
	drawWindow->console->blit(drawWindow->console, 0, 0, drawWindow->consoleW, drawWindow->consoleH, mainWindow->console, 0, 1, 1, 1);
	mainWindow->console->blit(mainWindow->console, 0, 0, w, h, TCODConsole::root, renderpos.x, renderpos.y, 1, 1);
}

void Panel::render()
{
	clearWindow();
	pushWindow();
}

//GuiMap Struct
GuiMap::GuiMap(int w, int h, int rx, int ry)
	:Panel(w, h, "Map", rx, ry)
{
	mapSidePanel = std::make_shared<Window>(1, 61, RIBONBGCOLOR, TCODColor::white);

	map = std::make_shared<Map>();
	map->createMap(map->debugmap);
	map->updateProperties(drawWindow);
}

void GuiMap::update()
{
	map->update(drawWindow);
}

//GuiMap Render
void GuiMap::render()
{
	clearWindow();
	map->render(drawWindow);
	pushWindow();

	mapSidePanel->render();
	mapSidePanel->console->blit(mapSidePanel->console, 0, 0, 1, 61, TCODConsole::root, 62, 2, 1, 1);
}
