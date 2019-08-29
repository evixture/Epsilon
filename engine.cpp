#include "main.hpp"

//Engine Class
Engine::Engine(int windowX, int windowY)
	:gamestate(INIT)
{
	gui = std::make_shared<Gui>(windowX, windowY);
	settings = std::make_shared<Settings>(windowX, windowY);
	gui->mapWindow->map->computeFov();
	gamestate = MAIN;
	TCODConsole::root->setDefaultBackground(TCODColor::black);
}

//Head of Update Loop
void Engine::update()
{
	settings->update(gui->mapWindow->map->player);
}

//Head of Render Loop
void Engine::render()
{
	TCODConsole::root->clear();
	settings->printLogo();
	gui->render();
	TCODConsole::flush();
}
