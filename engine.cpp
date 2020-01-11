#include "main.hpp"

Engine::Engine(int screenCharWidth, int screenCharHeight)
	:gamestate(Gamestate::MAIN), settings(std::make_shared<Settings>(screenCharWidth, screenCharHeight)), gui(std::make_shared<Gui>(screenCharWidth, screenCharHeight))
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

	if (SETTINGS->fpsCount >= 250) //automatic frame skip
	{
		settings->render();
		gui->render();

		if (WORLD->player->health < 75)
		{
			TCODConsole::flush();
		}
	}

	if (WORLD->player->health >= 75)
	{
		TCODConsole::flush();	
	}
}
