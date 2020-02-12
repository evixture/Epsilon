#include "main.hpp"

Pane::Pane(int consoleWidth, int consoleHeight, TCODColor backgroundColor, TCODColor foregroundColor)
	:consoleWidth(consoleWidth), consoleHeight(consoleHeight), backgroundColor(backgroundColor), foregroundColor(foregroundColor)
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

Ribon::Ribon(std::string windowName, int consoleWidth)
	: windowName(windowName), consoleWidth(consoleWidth)
{
	ribonWindow = std::make_shared<Pane>(consoleWidth, 1, ep::color::gui::Panel_Ribbon_BG, ep::color::gui::Panel_Ribbon_FG);
}

void Ribon::render() const
{
	ribonWindow->render();
	ribonWindow->console->printf(0, 0, "|%s|", windowName.c_str());
}

//----------------------------------------------------------------------------------------------------

Window::Window(int consoleWidth, int consoleHeight, std::string panelName, int rx, int ry)
	:consoleWidth(consoleWidth), consoleHeight(consoleHeight), panelName(panelName), renderpos(Position3(rx, ry, NULL))
{
	mainWindow = std::make_shared<Pane>(consoleWidth, consoleHeight, ep::color::gui::Root_BG, ep::color::gui::Root_FG);
	ribon = std::make_shared<Ribon>(panelName, consoleWidth);
	drawWindow = std::make_shared<Pane>(consoleWidth, consoleHeight - 1, ep::color::gui::Panel_Draw_BG, ep::color::gui::Panel_Draw_FG);
}

void Window::update()
{
	return;
}

void Window::clearWindow() const
{
	ribon->render();
	drawWindow->render();
}

void Window::pushWindow() const
{
	ribon->ribonWindow->console->blit(ribon->ribonWindow->console, 0, 0, ribon->ribonWindow->consoleWidth, ribon->ribonWindow->consoleHeight, mainWindow->console, 0, 0, 1, 1);
	drawWindow->console->blit(drawWindow->console, 0, 0, drawWindow->consoleWidth, drawWindow->consoleHeight, mainWindow->console, 0, 1, 1, 1);
	mainWindow->console->blit(mainWindow->console, 0, 0, consoleWidth, consoleHeight, TCODConsole::root, renderpos.x, renderpos.y, 1, 1);
}

void Window::render() const
{
	clearWindow(); 
	pushWindow();
}
