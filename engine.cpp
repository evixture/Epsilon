#include "main.hpp"

//Engine Class
Engine::Engine(int screenX, int screenY)
	:gamestate(INIT), gui(std::make_shared<Gui>(screenX, screenY)), settings(std::make_shared<Settings>(screenX, screenY))
{
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	gamestate = MAIN;
}

//Head of Update Loop
void Engine::update()
{
	settings->update(gui->mapPane->world->player);
	gui->update();
}

//Head of Render Loop
void Engine::render() const
{
	TCODConsole::root->clear();

	settings->render();
	gui->render();

	TCODConsole::flush();
}
