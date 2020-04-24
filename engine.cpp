#include "main.hpp"

Engine::Engine(int screenCharWidth, int screenCharHeight)
	:gamestate(Gamestate::MAIN), settings(std::make_shared<Settings>(screenCharWidth, screenCharHeight)), gui(std::make_shared<Gui>(screenCharWidth, screenCharHeight))//, audio(std::make_shared<SoLoud::Soloud>())
{
	//audio->init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::AUTO);
	TCODConsole::root->setDefaultBackground(ep::color::rootBG);
}

Engine::~Engine()
{
	//audio->deinit();
}

void Engine::update()
{
	settings->update();
	gui->update();
	//audio->update3dAudio();
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
