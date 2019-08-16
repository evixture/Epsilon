#include "main.hpp"

//Engine
Engine::Engine(int windowX, int windowY)
	:gamestate(INIT)
{
	//gui
	settings = std::make_shared<Settings>(windowX, windowY);
}

void Engine::update()
{
	//input
}

void Engine::render()
{
	//gui->render();
}