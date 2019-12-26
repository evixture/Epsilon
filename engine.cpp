#include "main.hpp"

//Engine Class
Engine::Engine(int screenCharWidth, int screenCharHeight)
	:gamestate(MAIN), settings(std::make_shared<Settings>(screenCharWidth, screenCharHeight)), gui(std::make_shared<Gui>(screenCharWidth, screenCharHeight))
{
	TCODConsole::root->setDefaultBackground(UICOLOR_Root_BG);
}

void Engine::update()
{
	settings->update(gui->worldWindow->world->player);
	gui->update();
}

void Engine::render() const
{
	TCODConsole::root->clear();

	if (SETTINGS->fpsCount >= 60) //automatic frame skip
	{
		settings->render();
		gui->render();
	}

	TCODConsole::flush();	
}
