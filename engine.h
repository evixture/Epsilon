#include "main.hpp"

class Engine
{
public:
	enum Gamestate { INIT, MAIN, MENU, GAMEOVER, EXIT } gamestate;

	std::shared_ptr<Gui> gui;
	std::shared_ptr<Settings> settings;

	Engine(int windowX, int windowY);

	void update();

	void render();
};

extern Engine engine;