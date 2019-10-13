#include "main.hpp"

//Engine Class
Engine::Engine(int screenCharWidth, int screenCharHeight)
	:gamestate(INIT), gui(std::make_shared<Gui>(screenCharWidth, screenCharHeight)), settings(std::make_shared<Settings>(screenCharWidth, screenCharHeight))
{
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	gamestate = MAIN;
}

//Head of Update Loop
void Engine::update()
{
	//std::lock_guard<std::mutex> lock(mutex);
	mutex.lock();
	while (!TCODConsole::isWindowClosed() && engine->gamestate != Engine::EXIT)
	{
		settings->update(gui->mapPane->world->player);
		gui->update();
	}
	mutex.unlock();
}

//Head of Render Loop
void Engine::render() const
{
	//std::lock_guard<mutable std::mutex> lock(mutexC);
	mutexC.lock();
	while (!TCODConsole::isWindowClosed() && engine->gamestate != Engine::EXIT)
	{
		//error here, tcod methods create error
		TCODConsole::root->clear();
		settings->render();
		gui->render();
		TCODConsole::flush();
	}
	mutexC.unlock();
}
