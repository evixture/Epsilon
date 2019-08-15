#include "main.hpp"

//Engine
Engine::Engine(int windowX, int windowY)
	:gamestate(INIT)
{
	//gui
	settings = std::make_shared<Settings>(windowX, windowY);
}

void Engine::render()
{
	//map->render();
}