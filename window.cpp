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

//Message Struct
Message::Message(const char* text, TCODColor color)
	:text(text), color(color)
{
}

//GuiWindow Struct
GuiWindow::GuiWindow(int w, int h, const char* guiWindowName, int rx, int ry)
	: w(w), h(h), guiWindowName(guiWindowName), renderpos(Position(rx, ry))
{
	mainWindow = std::make_shared<Window>(w, h, TCODColor::black, TCODColor::white);
	ribon = std::make_shared<Ribon>(guiWindowName, w);
	drawWindow = std::make_shared<Window>(w, h - 1, TCODColor::darkestGrey, TCODColor::white);
}

//GuiWindow clears window
void GuiWindow::clearWindow()
{
	ribon->render();
	drawWindow->render();
}

//GuiWindow blits the consoles together and pushes to root
void GuiWindow::pushWindow()
{
	ribon->ribonWindow->console->blit(ribon->ribonWindow->console, 0, 0, ribon->ribonWindow->consoleW, ribon->ribonWindow->consoleH, mainWindow->console, 0, 0, 1, 1);
	drawWindow->console->blit(drawWindow->console, 0, 0, drawWindow->consoleW, drawWindow->consoleH, mainWindow->console, 0, 1, 1, 1);
	mainWindow->console->blit(mainWindow->console, 0, 0, w, h, TCODConsole::root, renderpos.x, renderpos.y, 1, 1);
}

void GuiWindow::render()
{
	clearWindow();
	pushWindow();
}

//NEED TO LOOK INTO HOW THE MAP IS CREATED

//GuiMap Struct
GuiMap::GuiMap(int w, int h, int rx, int ry)
	:GuiWindow(w, h, "Map", rx, ry)
{
	mapSidePanel = std::make_shared<Window>(1, 61, RIBONBGCOLOR, TCODColor::white);

	map = std::make_shared<Map>(w, h);
	map->createMap("data/maps/debugmap.txt");
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
