#include "main.hpp"

Engine::Engine(int winx, int winy)
	:windowx(winx), windowy(winy), gameState(STARTUP)
{
	createCanvas(winx, winy);
}

Engine::~Engine()
{
}

void Engine::update()
{
}

void Engine::input()
{
}

void Engine::render()
{
}

void Engine::createCanvas(int x, int y)
{
	TCODConsole::setCustomFont("data/moddedterminal16x16_gs_ro.png", TCOD_FONT_TYPE_GRAYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW);
	TCODConsole::initRoot(x, y, "TEMP TITLE", false, TCOD_RENDERER_OPENGL2);

	TCODSystem::setFps(maxFps);
	TCODConsole::root->printf(1, 1, "THIS IS A TEST");
	TCODConsole::root->printf(1, 2, "to see and CHecK VisaBilIty AnD REadaILity");
}