#include "main.hpp"

//Initializtion Through Engine Constructor
 std::shared_ptr<Engine> engine = std::make_shared<Engine>(120, 64);

//Main Game Loop

//change to main to have terminal show up
//else use WinMain to hide console
int WinMain()
{
	std::thread updateThread(&Engine::update, engine);
	std::thread renderThread(&Engine::render, engine);
	
	//if (!TCODConsole::isWindowClosed() && engine->gamestate != Engine::EXIT)
	//{
	//	//error here, thread cannot be reinitialized multiple times
	//	//updateThread;// = std::thread(&Engine::update, engine);
	//	//renderThread;// = std::thread(&Engine::render, engine);
	//
	//	std::thread updateThread(&Engine::update, engine);
	//	std::thread renderThread(&Engine::render, engine);
	//}

	if (TCODConsole::isWindowClosed())
	{
		updateThread.join();
		renderThread.join();
	}

	TCOD_quit();
	return 0;
}
