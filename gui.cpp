#include "main.hpp"

Window::Window(int windowx, int windowy)
	:windowx(windowx), windowy(windowy), windowTitle("TEMP TITLE"), windowFullscreen(false), maxFps(60)
{
}

void Window::createCanvas()
{
	TCODConsole::initRoot(windowx, windowy, windowTitle, windowFullscreen, TCOD_RENDERER_OPENGL2);
	TCODSystem::setFps(maxFps);
}