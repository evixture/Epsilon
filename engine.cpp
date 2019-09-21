#include "main.hpp"

//Engine Class
Engine::Engine(int screenX, int screenY)
	:gamestate(MAIN), gui(std::make_shared<Gui>(screenX, screenY)), settings(std::make_shared<Settings>(screenX, screenY))
{
	TCODConsole::root->setDefaultBackground(TCODColor::black);
}

//Head of Update Loop
void Engine::update()
{
	settings->update(gui->mapPane->world->player);
	gui->update();
}

//Head of Render Loop
void Engine::render()
{
	TCODConsole::root->clear();

	settings->render();
	gui->render();

	TCODConsole::flush();
}
