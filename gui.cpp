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
	TCODConsole::root->printf(1, 1, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
	TCODConsole::root->printf(1, 2, "The quick brown fox jumps over a lazy dog");
}

void Gui::update()
{
}

void Gui::render()
{
}