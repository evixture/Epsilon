#include "main.hpp"

//Engine Class
Engine::Engine(int screenCharWidth, int screenCharHeight)
	:gamestate(INIT), settings(std::make_shared<Settings>(screenCharWidth, screenCharHeight)), gui(std::make_shared<Gui>(screenCharWidth, screenCharHeight))
{
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	gamestate = MAIN;
}

void Engine::update()
{
	settings->update(gui->mapPane->world->player);
	gui->update();
}

void Engine::render() const
{
	TCODConsole::root->clear();

	settings->render();
	gui->render();

	TCODConsole::flush();	
}
