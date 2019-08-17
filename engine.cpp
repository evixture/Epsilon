#include "main.hpp"

//Engine
Engine::Engine(int windowX, int windowY)
	:gamestate(INIT)
{
	gui = std::make_shared<Gui>(windowX, windowY);
	settings = std::make_shared<Settings>(windowX, windowY);
	gamestate = MAIN;
}

void Engine::update()
{
	settings->update(gui->guiMap->map->player);
}

void Engine::render()
{
	settings->printLogo();
	gui->render();
}