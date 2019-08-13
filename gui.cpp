#include "main.hpp"

Gui::Gui(int windowx, int windowy)
	:windowx(windowx), windowy(windowy), windowTitle("TEMP TITLE"), windowFullscreen(false), maxFps(60)
{
	TCODConsole::initRoot(windowx, windowy, windowTitle, windowFullscreen, TCOD_RENDERER_OPENGL2);
	TCODConsole::root->setDefaultBackground(TCODColor(23, 23, 23));
	TCODSystem::setFps(maxFps);
}

Gui::~Gui()
{
}