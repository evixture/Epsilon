#include "main.hpp"

//Engine Class
class Engine
{
public:
	enum Gamestate { INIT, MAIN, MENU, GAMEOVER, EXIT } gamestate;

	std::shared_ptr<Gui> gui;
	std::shared_ptr<Settings> settings;

	Engine(int screenX, int screenY);

	void update();
	void render();
};

extern std::shared_ptr<Engine> engine;
