#include "main.hpp"

Engine::Engine(int winx, int winy)
	:windowx(winx), windowy(winy), gameState(STARTUP)
{
	TCODConsole::initRoot(128, 72, "TEMP TITLE", false, TCOD_RENDERER_OPENGL2);
	//TCODConsole::setCustomFont("\data\terminal16x16_gs_ro.png", TCOD_FONT_TYPE_GRAYSCALE|TCOD_FONT_LAYOUT_ASCII_INROW, 0, 0);
	TCODSystem::setFps(maxFps);
	TCODConsole::root->printf(1, 1, "THIS IS A TEST");
	//createCanvas(winx, winy);
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
	//TCODConsole::initRoot(x, y, "TEMP TITLE", false, TCOD_RENDERER_OPENGL2);
	////TCODConsole::setCustomFont("\data\terminal16x16_gs_ro.png", TCOD_FONT_TYPE_GRAYSCALE|TCOD_FONT_LAYOUT_ASCII_INROW, 0, 0);
	//TCODSystem::setFps(maxFps);
	//TCODConsole::root->printf(1, 1, "THIS IS A TEST");
}