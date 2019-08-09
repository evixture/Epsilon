#include "main.hpp"

Gui::Gui(int windowx, int windowy)
	:windowx(windowx), windowy(windowy), windowTitle("TEMP TITLE"), windowFullscreen(false), maxFps(60)
{
}

Gui::~Gui()
{
}

void Gui::createCanvas()
{
	TCODConsole::initRoot(windowx, windowy, windowTitle, windowFullscreen, TCOD_RENDERER_OPENGL2);
	TCODSystem::setFps(maxFps);
}

void Gui::update()
{
}

void Gui::render()
{
}