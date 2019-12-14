#include "main.hpp"

//Pane Struct
Pane::Pane(int consoleW, int consoleH, TCODColor bgColor, TCODColor fgColor)
	:consoleW(consoleW), consoleH(consoleH), bgColor(bgColor), fgColor(fgColor)
{
	console = new TCODConsole(consoleW, consoleH);
}

Pane::~Pane()
{
}

void Pane::render() const
{
	console->setDefaultBackground(bgColor);
	console->setDefaultForeground(fgColor);
	console->clear();
}

//----------------------------------------------------------------------------------------------------

//Ribon Struct
Ribon::Ribon(std::string windowName, int windowW)
	: windowName(windowName), windowW(windowW)
{
	ribonWindow = std::make_shared<Pane>(windowW, 1, UICOLOR_Panel_Ribbon_BG, UICOLOR_Panel_Ribbon_FG);
}

void Ribon::render() const
{
	ribonWindow->render();
	ribonWindow->console->printf(0, 0, "|%s|", windowName.c_str());
}

//----------------------------------------------------------------------------------------------------

//Window Struct
Window::Window(int windowW, int windowH, std::string panelName, int rx, int ry)
	: windowW(windowW), windowH(windowH), panelName(panelName), renderpos(Position(rx, ry, NULL))
{
	mainWindow = std::make_shared<Pane>(windowW, windowH, UICOLOR_Root_BG, UICOLOR_Root_FG);
	ribon = std::make_shared<Ribon>(panelName, windowW);
	drawWindow = std::make_shared<Pane>(windowW, windowH - 1, UICOLOR_Panel_Draw_BG, UICOLOR_Panel_Draw_FG);
}

void Window::update()
{
	return;
}

void Window::setRibonName(std::string ribonName)
{
	ribon->windowName = ribonName;
}

void Window::clearWindow() const
{
	ribon->render();
	drawWindow->render();
}

void Window::pushWindow() const
{
	ribon->ribonWindow->console->blit(ribon->ribonWindow->console, 0, 0, ribon->ribonWindow->consoleW, ribon->ribonWindow->consoleH, mainWindow->console, 0, 0, 1, 1);
	drawWindow->console->blit(drawWindow->console, 0, 0, drawWindow->consoleW, drawWindow->consoleH, mainWindow->console, 0, 1, 1, 1);
	mainWindow->console->blit(mainWindow->console, 0, 0, windowW, windowH, TCODConsole::root, renderpos.x, renderpos.y, 1, 1);
}

void Window::render() const
{
	clearWindow(); 
	pushWindow();
}
