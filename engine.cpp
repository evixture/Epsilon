#include "main.hpp"

//Engine Class
Engine::Engine(int windowX, int windowY)
	:gamestate(MAIN)
{
	gui = std::make_shared<Gui>(windowX, windowY);
	settings = std::make_shared<Settings>(windowX, windowY);
	TCODConsole::root->setDefaultBackground(TCODColor::black);
}

//Head of Update Loop
void Engine::update()
{
	//gamestate = MAIN;
	settings->update(gui->mapWindow->map->player);
	gui->update();
}

//Head of Render Loop
void Engine::render()
{
	TCODConsole::root->clear();
	settings->printLogo();
	settings->printFps();
	gui->render();
	TCODConsole::flush();
}
