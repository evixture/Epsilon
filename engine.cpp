#include "main.hpp"

//Engine
Engine::Engine(int windowX, int windowY)
	:gamestate(INIT)
{
	map = std::make_shared<Map>(62, 62);
}

void Engine::render()
{
	map->render();
}