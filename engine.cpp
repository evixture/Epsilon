#include "main.hpp"

//Engine Class
Engine::Engine(int windowX, int windowY)
	:gamestate(INIT)
{
	gui = std::make_shared<Gui>(windowX, windowY);
	settings = std::make_shared<Settings>(windowX, windowY);
	gamestate = MAIN;
	TCODConsole::root->setDefaultBackground(TCODColor::black);
}

//Head of Update Loop
void Engine::update()
{
	TCODConsole::root->clear();
	settings->update(gui->mapWindow->map->player);
}

//Head of Render Loop
void Engine::render()
{
	settings->printLogo();
	gui->render();
	TCODConsole::flush();
}
