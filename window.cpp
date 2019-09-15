#include "main.hpp"

//Window Struct
Pane::Pane(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor)
	:consoleW(consoleW), consoleH(consoleH), bgColor(bgColor), fgColor(fgColor)
{
	console = new TCODConsole(consoleW, consoleH);
}

Pane::~Pane()
{
	delete console;
}

void Pane::render()
{
	console->setDefaultBackground(bgColor);
	console->setDefaultForeground(fgColor);
	console->clear();
}

//Ribon Struct
Ribon::Ribon(const char* windowName, int windowW)
	: windowName(windowName), windowW(windowW)
{
	ribonWindow = std::make_shared<Pane>(windowW, 1, RIBONBGCOLOR, TCODColor::white);
}

//Ribon Render
void Ribon::render()
{
	ribonWindow->render();
	ribonWindow->console->printf(0, 0, "[%s]", windowName);
}

//GuiWindow Struct
Window::Window(int w, int h, const char* panelName, int rx, int ry)
	: w(w), h(h), panelName(panelName), renderpos(Position(rx, ry))
{
	mainWindow = std::make_shared<Pane>(w, h, TCODColor::black, TCODColor::white);
	ribon = std::make_shared<Ribon>(panelName, w);
	drawWindow = std::make_shared<Pane>(w, h - 1, TCODColor::darkestGrey, TCODColor::white);
}

void Window::update()
{
	return;
}

//GuiWindow clears window
void Window::clearWindow()
{
	ribon->render();
	drawWindow->render();
}

//GuiWindow blits the consoles together and pushes to root
void Window::pushWindow()
{
	ribon->ribonWindow->console->blit(ribon->ribonWindow->console, 0, 0, ribon->ribonWindow->consoleW, ribon->ribonWindow->consoleH, mainWindow->console, 0, 0, 1, 1);
	drawWindow->console->blit(drawWindow->console, 0, 0, drawWindow->consoleW, drawWindow->consoleH, mainWindow->console, 0, 1, 1, 1);
	mainWindow->console->blit(mainWindow->console, 0, 0, w, h, TCODConsole::root, renderpos.x, renderpos.y, 1, 1);
}

void Window::render()
{
	clearWindow();
	pushWindow();
}
