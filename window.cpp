#include "main.hpp"

Pane::Pane(int consoleWidth, int consoleHeight, TCODColor backgroundColor, TCODColor foregroundColor)
	: consoleWidth(consoleWidth), consoleHeight(consoleHeight), backgroundColor(backgroundColor), foregroundColor(foregroundColor)
{
	console = new TCODConsole(consoleWidth, consoleHeight);
}

Pane::~Pane()
{}

void Pane::render() const
{
	console->setDefaultBackground(backgroundColor);
	console->setDefaultForeground(foregroundColor);
	console->clear();
}

//----------------------------------------------------------------------------------------------------

Ribbon::Ribbon(std::string windowName, int consoleWidth)
	: windowName(windowName), consoleWidth(consoleWidth), ribonWindow(Pane(consoleWidth, 1, ep::color::ribbonBG, ep::color::ribbonFG))
{
}

void Ribbon::render() const
{
	ribonWindow.render();
	ribonWindow.console->printf(0, 0, "|%s|", windowName.c_str());
}

//----------------------------------------------------------------------------------------------------

Window::Window(int consoleWidth, int consoleHeight, std::string panelName, int rx, int ry)
	: consoleWidth(consoleWidth), consoleHeight(consoleHeight), renderpos(Position3(rx, ry, NULL)), 
	mainPane(Pane(consoleWidth, consoleHeight, ep::color::rootBG, ep::color::rootFG)), ribbon(Ribbon(panelName, consoleWidth)), drawPane(Pane(consoleWidth, consoleHeight - 1, ep::color::drawBG, ep::color::drawFG))
{
}

void Window::clearWindow() const
{
	ribbon.render();
	drawPane.render();
}

void Window::pushWindow() const
{
	ribbon.ribonWindow.console->blit(ribbon.ribonWindow.console, 0, 0, ribbon.ribonWindow.consoleWidth, ribbon.ribonWindow.consoleHeight, mainPane.console, 0, 0, 1, 1);
	drawPane.console->blit(drawPane.console, 0, 0, drawPane.consoleWidth, drawPane.consoleHeight, mainPane.console, 0, 1, 1, 1);
	mainPane.console->blit(mainPane.console, 0, 0, consoleWidth, consoleHeight, TCODConsole::root, renderpos.x, renderpos.y, 1, 1);
}

