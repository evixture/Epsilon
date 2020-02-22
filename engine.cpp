#include "main.hpp"

Engine::Engine(int screenCharWidth, int screenCharHeight)
	:gamestate(Gamestate::MAIN), settings(std::make_shared<Settings>(screenCharWidth, screenCharHeight)), gui(std::make_shared<Gui>(screenCharWidth, screenCharHeight))
{
	TCODConsole::root->setDefaultBackground(UICOLOR_Root_BG);
}

void Engine::update()
{
	settings->update();
	gui->update();
}

void Engine::render() const
{
	TCODConsole::root->clear();

	if (SETTINGS->fpsCount >= 60) //automatic frame skip, does not work well
	{
		settings->render();
		gui->render();
	}

	TCODConsole::flush();	
}
